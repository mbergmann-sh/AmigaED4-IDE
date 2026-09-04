# AmigaED4-IDE
A cross-platform C/C++ and m68k Assembler Tabbed Editor/IDE for m68k-amigaos-gccIDE for classic Amiga development, built with Qt6.
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
