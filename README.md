# AmigaED4-IDE
A cross-platform C/C++ and m68k Assembler Tabbed Editor/IDE for m68k-amigaos-gcc and vbcc for classic Amiga development, built with Qt6.
<img width="1405" height="961" alt="grafik" src="https://github.com/user-attachments/assets/11037a21-2197-4a6a-87f8-bdbfd7426478" />
<img width="1408" height="961" alt="grafik" src="https://github.com/user-attachments/assets/31e3cd55-fbad-43d4-b8c2-1e2daec18ee0" />
<img width="1408" height="961" alt="grafik" src="https://github.com/user-attachments/assets/fb3adcde-01fa-4950-9b4e-6c3563941029" />
AmigaED brings project management, syntax highlighting, and one-click builds to AmigaOS 1.3/3.x cross-development, with out-of-the-box support for vbcc, m68k-amigaos-gcc, and SAS/C — plus integrated UAE emulator launching, so you can edit, compile, and test your Amiga software without ever leaving the editor.

## Features

> **Status:** early alpha. Core functionality works and has been tested during development, but it hasn't seen broad real-world use yet — expect rough edges, and please [file an issue](../../issues) if you hit one.

### Editor
- Tabbed editor (multiple files open at once), powered by Qt6 + QScintilla
- Syntax highlighting for:
  - C/C++ — including AmigaOS NDK 3.2R4 data types and library function names, specially highlighted
  - m68k Assembler (custom lexer) — mnemonics, directives, registers (`d0`–`d7`/`a0`–`a7`/`sp`/`pc`/`sr`/`ccr`), `$hex`/`%binary` numbers, column-zero labels
  - AmigaGuide hypertext (custom lexer) — `@node`/`@endnode` commands, `@{...}` inline links/styling, `@remark` comments
  - Amiga Installer scripts (custom lexer)
  - Makefiles, AmigaShell scripts, Pascal, plain text
  - Automatic syntax selection by file extension on open (`.c`/`.cpp`/`.h`, `.asm`/`.s`, `.guide`, `.pas`, `Makefile*`, …), with a status-bar confirmation
- Light and Dark application themes
- English and German UI, switchable at runtime (View → GUI Language)
- Function list widget — lists every C/C++ function in the current project for quick navigation
- Drag-and-drop: drop supported source/doc files (C/C++, headers, `.asm`/`.s`, `.guide`, `.pas`, `.txt`/`.readme`, Makefiles) straight onto the project tree to add them
- Recently opened files/projects, each with a one-click "Forget recent…" to clear the list

### Project management
- Project files (`.aep`) with a categorized tree view: C/C++ Sources, Headers, Assembler Sources, AmigaGuide, Installer Scripts, Executables, Other Files, and auto-generated Makefiles
- Compiled executables are auto-detected and listed with their file size — including ones from an imported foreign project, identified by their AmigaOS "hunk" binary signature rather than guesswork — with rename support right from the tree
- "New Project" templates:
  - Empty C, AmigaShell
  - AmigaOS 1.3 (console-only, NDK 1.3-safe)
  - AmigaOS 3.x (NDK 3.2R4) — dual Shell/Workbench entry point out of the box
  - ReAction GUI skeleton
  - MUI 5 GUI skeleton
- "Import existing Project…" — turn an existing, non-AmigaED C/C++ folder into an AmigaED project, auto-detecting its main file and correctly sorting oddities like SAS/C build artifacts, stray executables, or `.info` icons into the right category instead of miscategorizing them
- "Add files to Project…" (multi-select dialog, also reachable via the project panel's Add button)
- Automatic `Makefile.gcc` (m68k-amigaos-gcc) / `Makefile.vbcc` (vbcc) generation on every file add/remove, with:
  - The correct assembler wired in per toolchain for `.asm`/`.s` sources — `vasm` directly for vbcc, `gcc -x assembler` (gas) for gcc/g++
  - Automatic detection of floating-point usage in the project's own sources, adding the right math library per toolchain (`-lm` for gcc/g++, `-lmieee` for vbcc, `MATH=IEEE` for SAS/C) only when actually needed
  - Optional automatic `.info` icon creation for the built executable (from a configurable default icon), handled per-platform (`copy` on Windows, `cp` on Linux/macOS) — including the Windows-specific quirks of `make` sometimes running recipes through a bundled `sh.exe` instead of `cmd.exe`
  - A `Makefile.sc` (SAS/C) generated alongside for manual on-Amiga builds
  - Sensible, research-backed default compiler/linker flags per toolchain and target OS (including `-lamiga`/`-lauto` for Workbench-capable

## Building AmigaED

AmigaED is a Qt6/qmake project (QScintilla-based). There are two ways to build it: from within Qt Creator, or entirely from the command line. Both work identically on Windows, Linux, and macOS.

### Prerequisites (all platforms)

- Qt 6 (developed against 6.11.2, but any reasonably recent Qt6 should work) — with the "Desktop" component and qmake
- QScintilla 2, built against Qt6
- A C++ compiler toolchain:
  - **Windows:** MinGW 13.1.0 64-bit (as shipped with the Qt online installer) — MSVC is untested but should work with minor `.pro` adjustments
  - **Linux/macOS:** GCC or Clang, whichever your Qt6 installation targets

### Option A: Building with Qt Creator

1. Open `AmigaED.pro` in Qt Creator.
2. Select the Qt6 kit you want to build with.
3. Build ▸ Run qmake, then Build ▸ Build Project "AmigaED" (or just hit **Run**).

This is the easiest path, especially on Windows, since Qt Creator manages the MinGW toolchain and Qt paths for you.

### Option B: Building from the command line

```bash
qmake6 AmigaED.pro   # or "qmake AmigaED.pro" if qmake6 isn't on your PATH
make                 # mingw32-make.exe on Windows
```

The resulting binary (and, on Windows, the staged `windeployqt`/QScintilla DLL output — see below) ends up in the build directory qmake configured (`release/` or `debug/` by default, depending on build config).

### Platform-specific notes

**Windows:** The `.pro` file's `QMAKE_POST_LINK` step automatically runs `windeployqt` and copies the QScintilla DLL into the output folder after every successful build, so the result is a self-contained, runnable folder without manually hunting down DLLs. A ready-to-compile Inno Setup script is provided separately under `AmigaED_install/` for producing a proper Windows installer, once you have a build.

**Linux:** Builds natively with no extra steps beyond the prerequisites below. Two additional packaging scripts are provided if you want a distributable package rather than just a local build:
- `AppImage/build_appimage.sh` — bundles Qt6/QScintilla into a portable AppImage (needs `linuxdeploy` and the Qt plugin for it — not packaged via apt, see the script's own header comment for where to download them)
- `Debian/build_deb.sh` — builds a native `.deb` with correctly detected runtime dependencies (via `dpkg-shlibdeps`)

Neither script runs automatically as part of the normal build — both are invoked manually, after a normal build has already succeeded.

**macOS:** Builds the same way as Linux (qmake + make, or via Qt Creator). No packaging script (e.g. for a `.app` bundle/`.dmg`) exists yet.

### Required `.deb` packages (Debian/Ubuntu)

To build AmigaED itself on a Debian- or Ubuntu-based system:

```bash
sudo apt install build-essential qt6-base-dev qt6-base-dev-tools libqscintilla2-qt6-dev
```

| Package | What it provides |
|---|---|
| `build-essential` | GCC/G++ and `make` |
| `qt6-base-dev` | Qt6 core/widgets/gui headers and libraries |
| `qt6-base-dev-tools` | `qmake6` and other Qt6 build tools |
| `libqscintilla2-qt6-dev` | QScintilla 2 editor widget, built against Qt6 (headers + library) |

Two more are only needed for specific extra steps, not for a normal build:

| Package | Needed for |
|---|---|
| `dpkg-dev` | Building the `.deb` package (`Debian/build_deb.sh`) — provides `dpkg-shlibdeps`/`dpkg-deb` |
| `qt6-l10n-tools` | Regenerating translations (`lupdate`/`lrelease`) after changing translatable strings — not needed just to build the app, since the compiled `.qm` translation file is already checked in |
