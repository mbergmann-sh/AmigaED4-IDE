#!/bin/bash
#
# build_appimage.sh - builds AmigaED and packages it as a self-contained
# AppImage (bundles Qt6, QScintilla and their plugins, so it runs on
# distros that don't have those installed at all).
#
# Usage:
#   cd AppImage
#   ./build_appimage.sh
#
# Result: AppImage/AmigaED-x86_64.AppImage
#
# ---------------------------------------------------------------------
# IMPORTANT - which machine to build this on:
#
# An AppImage bundles the LIBRARIES the app needs (Qt6, QScintilla,
# etc.) - but the AmigaED binary itself is still linked against
# whatever glibc the BUILD machine has, and glibc compatibility only
# goes one way: a binary built against a newer glibc will refuse to
# run ("GLIBC_2.XX not found") on a system with an OLDER glibc, but a
# binary built against an OLDER glibc runs fine on newer systems too.
#
# So: build this on the OLDEST distro you need to support. For "Debian
# 12 and Debian 13", that means building on a real Debian 12 machine
# (or a Debian 12 container/VM) - the resulting AppImage will then also
# run on Debian 13 (and generally most distros from around that era
# onward). Building on Debian 13 (or anything newer, e.g. this
# project's own Ubuntu 24 dev sandbox) is NOT guaranteed to produce
# something that still runs on Debian 12.
# ---------------------------------------------------------------------

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$SCRIPT_DIR/build"
APPDIR="$SCRIPT_DIR/AppDir"
TOOLS_DIR="$SCRIPT_DIR/tools"

echo "=== AmigaED AppImage build ==="
echo "Project dir: $PROJECT_DIR"

# --- 1. Sanity-check the toolchain -----------------------------------
QMAKE_BIN="qmake6"
if ! command -v "$QMAKE_BIN" >/dev/null 2>&1; then
    if command -v qmake >/dev/null 2>&1; then
        QMAKE_BIN="qmake"
    else
        echo "ERROR: qmake6 (or qmake) not found." >&2
        echo "Install the Qt6 dev packages first, e.g. on Debian/Ubuntu:" >&2
        echo "  sudo apt install qt6-base-dev qt6-base-dev-tools libqscintilla2-qt6-dev" >&2
        exit 1
    fi
fi
echo "Using qmake: $(command -v "$QMAKE_BIN")"

if ! command -v make >/dev/null 2>&1; then
    echo "ERROR: make not found - install build-essential." >&2
    exit 1
fi

# --- 2. Build AmigaED (out-of-source, so this never touches your ----
#        normal Qt Creator build directory) --------------------------
echo "--- Building AmigaED ---"
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
(
    cd "$BUILD_DIR"
    "$QMAKE_BIN" "$PROJECT_DIR/AmigaED.pro"
    make -j"$(nproc)"
)

AMIGAED_BIN="$BUILD_DIR/AmigaED"
if [ ! -x "$AMIGAED_BIN" ]; then
    echo "ERROR: build finished but $AMIGAED_BIN was not produced." >&2
    exit 1
fi

# --- 3. Fetch linuxdeploy + its Qt plugin, if not already cached ----
# (github.com/release-assets.githubusercontent.com - both already
# reachable in this project's own build sandbox; on your own machine
# this just needs ordinary internet access once.)
mkdir -p "$TOOLS_DIR"
LINUXDEPLOY="$TOOLS_DIR/linuxdeploy-x86_64.AppImage"
LINUXDEPLOY_QT="$TOOLS_DIR/linuxdeploy-plugin-qt-x86_64.AppImage"

fetch_tool() {
    local url="$1" dest="$2"
    if [ ! -x "$dest" ]; then
        echo "--- Fetching $(basename "$dest") ---"
        curl -L --fail -o "$dest" "$url"
        chmod +x "$dest"
    fi
}

fetch_tool "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage" "$LINUXDEPLOY"
fetch_tool "https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage" "$LINUXDEPLOY_QT"

# linuxdeploy's own AppImages need FUSE to run directly. Two things are
# needed to avoid that entirely (both matter - the CLI flag alone does
# NOT cover the Qt plugin, since linuxdeploy launches it as its own
# subprocess without passing the flag along):
#   1. --appimage-extract-and-run on the linuxdeploy invocation itself
#   2. the APPIMAGE_EXTRACT_AND_RUN=1 environment variable, which IS
#      inherited by that subprocess (linuxdeploy-plugin-qt) and makes
#      it extract-and-run too, instead of trying to FUSE-mount itself
#      and failing with a bare "exit code 127" if FUSE isn't installed.
export APPIMAGE_EXTRACT_AND_RUN=1
LINUXDEPLOY_RUN=("$LINUXDEPLOY" --appimage-extract-and-run)
LINUXDEPLOY_QT_RUN="$LINUXDEPLOY_QT"

# --- 4. Assemble the AppDir -------------------------------------------
echo "--- Assembling AppDir ---"
rm -rf "$APPDIR"
mkdir -p "$APPDIR/usr/bin"
cp "$AMIGAED_BIN" "$APPDIR/usr/bin/AmigaED"
cp "$SCRIPT_DIR/AmigaED.desktop" "$APPDIR/"
# AppImage/desktop icons must be square - amiga_classic.png (the same
# icon already used for the Windows .exe/taskbar, see AmigaED.pro's
# RC_ICONS) isn't, so a pre-made, padded-to-square 256x256 version
# lives right here as AmigaED.png and is used as-is (linuxdeploy
# rejects non-square icons outright: "Failed to deploy icon"). Swap in
# a nicer square PNG later if you'd like, same filename/location.
cp "$SCRIPT_DIR/AmigaED.png" "$APPDIR/AmigaED.png"

# --- 5. Run linuxdeploy + its Qt plugin to bundle Qt6/QScintilla/ ----
#        plugins and produce the final AppImage ----------------------
echo "--- Running linuxdeploy (bundles Qt6, QScintilla, platform plugins) ---"
export QMAKE="$(command -v "$QMAKE_BIN")"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH:-}"
export PATH="$TOOLS_DIR:$PATH"   # so linuxdeploy can find its Qt plugin by name

(
    cd "$SCRIPT_DIR"
    "${LINUXDEPLOY_RUN[@]}" \
        --appdir "$APPDIR" \
        --executable "$APPDIR/usr/bin/AmigaED" \
        --desktop-file "$APPDIR/AmigaED.desktop" \
        --icon-file "$APPDIR/AmigaED.png" \
        --plugin qt \
        --output appimage
)

RESULT=$(ls "$SCRIPT_DIR"/AmigaED*.AppImage 2>/dev/null | head -n1)
if [ -z "$RESULT" ]; then
    echo "ERROR: linuxdeploy did not produce an .AppImage file." >&2
    exit 1
fi

echo ""
echo "=== Done: $RESULT ==="
echo "Test it with: $RESULT"
