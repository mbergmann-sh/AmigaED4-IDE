#!/bin/bash
#
# build_deb.sh - builds AmigaED and packages it as a native .deb for
# whichever Debian version it's run on.
#
# Usage:
#   cd Debian
#   ./build_deb.sh
#
# Result: Debian/AmigaED_<rev>_<codename>_amd64.deb
#
# Unlike the AppImage recipe (see ../AppImage/), this produces an
# ordinary, dynamically-linked binary that relies on Qt6/QScintilla
# already being installed on the target system (via apt, as regular
# Depends: - see below) - it does NOT bundle them. That's a deliberate
# trade-off: AmigaED itself launches external compiler toolchains
# (m68k-amigaos-gcc, vbcc) as child processes, and an AppImage's own
# LD_LIBRARY_PATH/PATH wrapping (needed so AmigaED finds ITS bundled
# Qt6/QScintilla) leaks into those child processes too - which can
# break an external compiler that links against different versions of
# the same-named system libraries. A .deb-installed binary runs with a
# perfectly ordinary system environment, so this problem doesn't arise.
#
# Because a .deb only carries the app itself (not Qt6/QScintilla), it
# MUST be built separately on each target Debian version - the runtime
# library package names differ between Debian 12 and Debian 13 (the
# "t64" 64-bit-time_t package rename that landed between them), so a
# .deb built on one won't necessarily install cleanly on the other.
# This script detects the correct Depends: automatically either way
# (see dpkg-shlibdeps below) - just run it separately on each machine.

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$SCRIPT_DIR/build"
PKG_ROOT="$SCRIPT_DIR/pkgroot"

echo "=== AmigaED .deb build ==="
echo "Project dir: $PROJECT_DIR"

# --- 1. Sanity-check the toolchain -----------------------------------
QMAKE_BIN="qmake6"
if ! command -v "$QMAKE_BIN" >/dev/null 2>&1; then
    if command -v qmake >/dev/null 2>&1; then
        QMAKE_BIN="qmake"
    else
        echo "ERROR: qmake6 (or qmake) not found." >&2
        echo "Install the Qt6 dev packages first, e.g.:" >&2
        echo "  sudo apt install qt6-base-dev qt6-base-dev-tools libqscintilla2-qt6-dev" >&2
        exit 1
    fi
fi
echo "Using qmake: $(command -v "$QMAKE_BIN")"

for tool in make dpkg-deb dpkg-shlibdeps; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo "ERROR: $tool not found - install build-essential and dpkg-dev." >&2
        exit 1
    fi
done

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

# --- 3. Figure out version + target Debian codename -------------------
REVISION=$(grep -oP '(?<=AMIGAED_REVISION )\d+' "$PROJECT_DIR/version.h" || echo "0")
PKG_VERSION="4.0.${REVISION}"

CODENAME="unknown"
if [ -r /etc/os-release ]; then
    # shellcheck disable=SC1091
    . /etc/os-release
    CODENAME="${VERSION_CODENAME:-$CODENAME}"
fi
echo "Package version: $PKG_VERSION   Debian codename: $CODENAME"

# --- 4. Assemble the package tree -------------------------------------
echo "--- Assembling package tree ---"
rm -rf "$PKG_ROOT"
mkdir -p "$PKG_ROOT/usr/bin"
mkdir -p "$PKG_ROOT/usr/share/applications"
mkdir -p "$PKG_ROOT/usr/share/icons/hicolor/256x256/apps"
mkdir -p "$PKG_ROOT/DEBIAN"

install -m 755 "$AMIGAED_BIN" "$PKG_ROOT/usr/bin/AmigaED"
install -m 644 "$SCRIPT_DIR/../AppImage/AmigaED.desktop" "$PKG_ROOT/usr/share/applications/AmigaED.desktop"
# Same pre-made, padded-to-square icon the AppImage recipe uses (see
# ../AppImage/README.md) - the hicolor icon theme also requires square
# images.
install -m 644 "$SCRIPT_DIR/../AppImage/AmigaED.png" "$PKG_ROOT/usr/share/icons/hicolor/256x256/apps/AmigaED.png"

# --- 5. Let dpkg figure out the actual runtime Depends: ----------------
# Deliberately not a hand-written list of package names: Debian 12 vs
# 13 use DIFFERENT names for the same Qt6 runtime libraries (the
# "t64" 64-bit-time_t transition renamed most of them between the
# two releases) - hardcoding one set would silently produce a .deb
# that fails to install on the other. dpkg-shlibdeps instead inspects
# AmigaED's actual dynamic dependencies and looks up whichever package
# actually provides each one on THIS system - so it's automatically
# correct for Debian 12 and Debian 13 alike, each built on its own
# machine.
echo "--- Resolving runtime dependencies (dpkg-shlibdeps) ---"
(
    cd "$SCRIPT_DIR"
    # dpkg-shlibdeps insists on a "debian/control" file to read for
    # context, even used standalone like this (writes its real output
    # to stdout via -O instead, so the actual content here barely
    # matters) - a bare minimum with just Source:/Package: satisfies it.
    mkdir -p debian
    printf 'Source: amigaed\n\nPackage: amigaed\nArchitecture: amd64\n' > debian/control
    dpkg-shlibdeps -O "$PKG_ROOT/usr/bin/AmigaED" > shlibdeps.out
)
SHLIBS_DEPENDS=$(sed -n 's/^shlibs:Depends=//p' "$SCRIPT_DIR/shlibdeps.out")
rm -rf "$SCRIPT_DIR/debian" "$SCRIPT_DIR/shlibdeps.out"

if [ -z "$SHLIBS_DEPENDS" ]; then
    echo "WARNING: dpkg-shlibdeps returned no dependencies - check shlibdeps.out output above if something looks wrong." >&2
fi

INSTALLED_SIZE=$(du -sk "$PKG_ROOT/usr" | cut -f1)

# --- 6. Write DEBIAN/control -------------------------------------------
cat > "$PKG_ROOT/DEBIAN/control" <<EOF
Package: amigaed
Version: $PKG_VERSION
Section: devel
Priority: optional
Architecture: amd64
Installed-Size: $INSTALLED_SIZE
Depends: $SHLIBS_DEPENDS
Maintainer: MB-SoftWorX <noreply@example.com>
Description: C/C++ cross-development editor for classic Amiga
 AmigaED (Amiga Cross Editor) is a Qt-based source editor and project
 manager for cross-compiling C/C++ software for classic AmigaOS
 (1.3/3.x), targeting the m68k-amigaos-gcc and vbcc toolchains.
EOF
# Edit the Maintainer line above to your real contact details before
# distributing this package to anyone outside your own machines.

# --- 7. Build the .deb --------------------------------------------------
OUT_DEB="$SCRIPT_DIR/AmigaED_${PKG_VERSION}_${CODENAME}_amd64.deb"
rm -f "$OUT_DEB"
dpkg-deb --root-owner-group --build "$PKG_ROOT" "$OUT_DEB"

echo ""
echo "=== Done: $OUT_DEB ==="
echo "Install with: sudo apt install $OUT_DEB"
echo "(or: sudo dpkg -i $OUT_DEB   - use apt if dpkg reports missing dependencies, so it can fetch them)"
