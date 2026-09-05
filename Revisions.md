# AmigaED 4.0 – Revisions

Changelog for AmigaED 4.0, newest revision first. The revision number
shown here matches `AMIGAED_REVISION` in `version.h`, which in turn
appears in every window title as `AmigaED 4.0 rev.<n>`.

> Revisions 53–79 predate this changelog and are not individually
> documented here (see the "Earlier milestones" section at the bottom
> for what's known about the wider rev1–52 range).

## rev.94
- Fixed the Manual window (Help > Manual, F1) being unreadable while
  AmigaED's "Dark" application style is active: that style works by
  installing a dark QApplication-wide QPalette, which the manual
  window - authored with fixed light colours, like a printed page -
  otherwise inherited. Fixed on both ends: the HTML itself now sets an
  explicit white `background-color` on `<html>`/`<body>`, and
  `actionShowManual()` additionally forces a plain light QPalette on
  the dialog and QTextBrowser regardless of the app's current theme.

## rev.93
- Both manuals are now also available as self-contained HTML exports
  (`help/manual_en.html`, `help/manual_de.html`), embedded into the
  application as Qt resources (`application.qrc`, new `/help` prefix)
  so they work regardless of install location. Same content, styling,
  and screenshots as the PDFs, with an in-page Table of Contents
  (jump links) instead of PDF page-link annotations.
- New menu entry **Help > Manual** (shortcut **F1**), followed by a
  separator before About/About Qt. Opens a non-modal window (a
  `QDialog` with a `QTextBrowser`) showing the English or German HTML
  manual depending on the current GUI language. The window opens only
  once - a repeat F1 press or menu click just raises the existing
  window instead of opening a second one - and being non-modal, the
  editor stays fully usable while it is open.

## rev.92
- Both manuals (`DOC/AmigaED_Guide_EN.pdf`, `DOC/AmigaED_Anleitung_DE.pdf`) now open
  with a new cover page ("AmigaED 4.0 Quick Manual", with a newly drawn illustration
  of an Amiga 1200 with monitor and mouse) followed by a Table of Contents /
  Inhaltsverzeichnis page. Every TOC entry is a clickable link jumping straight to
  its chapter, and a full PDF outline (bookmarks/sidebar navigation) was added as
  well. All prior content is unchanged and simply shifted two pages later (both
  manuals now 10 pages).

## rev.91
- Both manuals (`DOC/AmigaED_Guide_EN.pdf`, `DOC/AmigaED_Anleitung_DE.pdf`) gained a
  new chapter, "The Editor's Context Menu" / "Das Kontextmenü des Editors", inserted
  right after "The Preferences Editor" / "Der Einstellungs-Editor" and before
  "Suggested compiler & linker switches" / "Empfohlene Compiler- & Linker-Schalter"
  (both manuals now 8 pages). It includes a new illustrated mockup (styled to match
  the existing screenshots) of the editor's right-click context menu with all ten
  numbered entries, followed by a numbered explanation of each one (including the
  contents of its four submenus: Preprocessor..., Libraries..., Condition..., Loops...,
  and Comments...).

## rev.90
- All six "New Project" templates (Empty Amiga C, Shell, AmigaOS 1.3,
  AmigaOS 3.x, ReAction, MUI) now include `#define myDebug TRUE` right
  after their `#include` lines, followed by a blank line.
- New context menu / Insert menu entry "Console Debugging Message",
  inserting:
  ```c
  if (myDebug)
  {
  	/* --- Insert debugging messages here: --- */

  }
  ```
  at the caret position. Unlike most other insert templates, the caret
  lands *inside* the block (the blank line after the comment) rather
  than after it.
- Note: the requested `#define myDebug = TRUE` was corrected to
  `#define myDebug TRUE` (no `=`) - with the `=`, the preprocessor
  would expand `myDebug` to `= TRUE`, breaking `if (myDebug)` into
  `if ( = TRUE)`, a syntax error.

## rev.89
- Preprocessor/"Identify Amiga compiler": now generates a `const char
  *compiler_string = "...";` per branch instead of a `printf()` call
  with the same text. The GNU gcc branch (which prints
  `__GNUC__`/`__GNUC_MINOR__`/`__GNUC_PATCHLEVEL__` at runtime via a
  `printf` format string) now folds those macro values into the string
  constant at compile time instead, via a standard `STR`/`STR_HELPER`
  stringizing-macro pair and adjacent string literal concatenation.

## rev.88
- Added this changelog (`Revisions.md`) to the project, listed in
  `DISTFILES` so it shows up in Qt Creator's project tree.

## rev.87
- Context menu / Insert menu cleanup:
  - `#include`: caret now jumps to the end of the inserted line, adds a
    line break, and lands on the new line (previously landed mid-text).
  - Amiga `#include` block: caret now lands on a new line after the
    whole block (previously landed back inside it).
  - `OpenLibrary()` / `CloseLibrary()`: implemented (previously
    "not implemented" stubs) - inserts a dummy.library
    Open/CloseLibrary() template.
  - `switch(...)`: implemented (previously a stub) - inserts a
    `switch`/`case dummy: break;` skeleton.
  - `int main {...}` / `int function {...}`: caret now lands on a new
    line after the function (previously landed back inside the body).
  - `enum {...}`: implemented (previously a stub) - inserts a complete
    C enum skeleton.
  - `struct name {...}`: removed entirely (action, menu entries, slot,
    header declaration, translations).
  - "Class..." (C-style/C++-style) and "Snippets..." (#1-#4) submenus:
    removed entirely (actions, menu entries, slots, header
    declarations, translations).

## rev.86
- Windows installer (`AmigaED_install\AmigaED.iss`): fixed
  `CreateCustomForm` call for Inno Setup 6.6.0/7's changed signature
  (ClientWidth/ClientHeight/KeepSizeX/KeepSizeY are now constructor
  parameters instead of settable properties afterwards).

## rev.85
- Windows installer: fixed a Pascal Script compile error - Inno
  Setup's scripting dialect doesn't support nested functions. Moved
  the button-creation helper out of `AskInstallChoice` into its own
  top-level function (`AddInstallChoiceButton`).

## rev.84
- Fixed an "unused parameter 'pos'" compiler warning in
  `showCustomContextMenue()` via `Q_UNUSED(pos)`.

## rev.83
- Windows installer: the "already installed?" prompt now offers three
  real choices instead of two, via a small custom dialog
  (`AskInstallChoice`) since a stock `MsgBox` can't carry custom
  button captions:
  - **Reinstall** - installs over the existing installation (old
    behaviour).
  - **Update** (new) - silently uninstalls the old version first, then
    installs fresh (clean update instead of just copying over it).
  - **Uninstall** - hands off to the existing uninstaller (old
    behaviour).
  - All captions/messages are fully localized (German/English) via
    `[CustomMessages]`, fixing a pre-existing bug where this dialog's
    text was hardcoded German regardless of the chosen setup language.

## rev.82
- Removed the "Application Templates" entry (and its three submenu
  items: AmigaShell/ANSI C/C++ app templates) from both the editor's
  context menu and the main "Inserts" menu - redundant now that whole
  projects can be created from a template via the main menu. Actions,
  slot implementations, header declarations, and the corresponding
  German translations were all removed.

## rev.81
- Windows installer:
  - After a successful build, the `DOC` folder (manuals) is now also
    staged into `AmigaED_install\install_src\DOC` (via
    `install_stage.bat`).
  - The `.iss` script now asks the user (in whichever language they
    picked for Setup) whether to install the documentation, and only
    copies `DOC` into the chosen install directory on "Yes".
- Removed the unused `projectverwaltung.txt` file from the project.
- Added `DOC\AmigaED_Anleitung_DE.pdf` and `DOC\AmigaED_Guide_EN.pdf`
  (German/English manuals) to the project tree.

## rev.80
- (Baseline for this changelog - uploaded project state as of this
  revision; DOC folder with both manuals already present.)

---

## Earlier milestones (rev1–52, informally tracked)

- **rev20**: Dark theme added (Prefs > Misc > "Default application
  style" > "Dark").
- **rev21**: Fixed overlapping/garbled text in the QScintilla editor
  widget under Linux (font-fallback fix in `initializeFont()`).
- **rev22**: Own program icon on startup under Linux
  (`app.setWindowIcon` in `main.cpp`).
- **rev23**: Multi-color toolbar icon for "Build/Build Project".
- **rev24**: Own program icon for `AmigaED.exe` under Windows
  (`RC_ICONS` in the `.pro` file).
- **rev25**: New "Import existing Project..." menu entry (File > New
  Project...); all window titles now read "AmigaED 4.0" instead of
  "Amiga Cross Editor".
- **rev26**: Project file renamed from `application.pro` to
  `AmigaED.pro`.
- **rev27**: `windeployqt` now runs automatically after a successful
  Windows build (`QMAKE_POST_LINK`).
- **rev28**: `qscintilla2_qt6.dll` now copied to the output directory
  automatically alongside `windeployqt`.
- **rev29–34**: Various Windows build-step fixes (the m68k-amigaos-gcc
  toolchain's own `sh.exe` on PATH was interfering with
  `mingw32-make`'s recipe shell).
- **rev35**: First version of the Windows installer
  (`AmigaED_install\AmigaED.iss`, `install_src` staging folder).
- **rev46**: UAE emulator start/stop toolbar buttons made robust
  (no double-start, correct button state, emulator closed on exit);
  installer welcome page with image added.
- **rev47**: New `stop-emu.png` icon.
- **rev48**: AppImage build recipe for Linux (Debian 12/13 target).
- **rev52**: Switched from AppImage to native `.deb` packages
  (`Debian/build_deb.sh`) due to AppImage's `LD_LIBRARY_PATH`/`PATH`
  interfering with compiler child processes.
