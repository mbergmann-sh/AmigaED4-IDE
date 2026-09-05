# AmigaED 4.0 – Revisions

Changelog for AmigaED 4.0, newest revision first. The revision number
shown here matches `AMIGAED_REVISION` in `version.h`, which in turn
appears in every window title as `AmigaED 4.0 rev.<n>`.

> Revisions 53–79 predate this changelog and are not individually
> documented here (see the "Earlier milestones" section at the bottom
> for what's known about the wider rev1–52 range).

## rev.100
- Reverted rev.99's `QWebEngineView` for the Help > Manual viewer back
  to `QTextBrowser`: Qt WebEngine (Chromium) does not compile with
  MinGW at all - a hard upstream limitation, not a missing Qt
  Maintenance Tool component - and this project's Qt kit uses MinGW.
  Removed `webenginewidgets` from `AmigaED.pro` again.
- Instead, found and fixed the actual rendering bug that motivated
  trying WebEngine in the first place: images in the manual's HTML
  were overlapping the heading/caption around them. Root cause -
  confirmed by rendering the HTML through the real QTextDocument engine
  offscreen (not just eyeballing it in a browser, which uses a
  completely different, far more capable engine) - was the `line-height`
  CSS property anywhere in the stylesheet, combined with this page's
  non-uniform body padding (`24px 34px 60px 34px`): together they made
  QTextBrowser drastically under-reserve vertical space for any image
  that followed. Removed `line-height` from the manual's CSS entirely;
  also switched image centering from `style="margin:auto"` to the
  old-school `<p align="center">`, and gave every `<img>` explicit
  pixel `width`/`height` attributes (computed from each image's real
  size) instead of percentage-based sizing - both were separately
  confirmed, the same way, to make QTextBrowser reserve the correct
  space. Added `<a name="...">` anchors alongside each heading's `id`
  attribute as a belt-and-suspenders measure for the Table of Contents'
  jump links (verified working either way).

## rev.99
- The Help > Manual viewer (F1) now renders the HTML manual with a real
  Chromium engine (`QWebEngineView`) instead of `QTextBrowser` - whose
  limited HTML/CSS subset rendered the manual's layout, tables, and
  images poorly. Requires the new `webenginewidgets` Qt module (added
  to `AmigaED.pro`) and, on the Qt kit used to build it, the "Qt
  WebEngine" component installed via the Qt Maintenance Tool if not
  already present. This is a substantially heavier dependency than
  before - it adds Chromium's own binaries/resources to a built
  install (`windeployqt` already stages these automatically, no
  install-script changes were needed) - but renders the manual exactly
  as intended.

## rev.98
- Fixed two build errors reported for rev.97:
  - `actionGoto_matching_brace()`: `SendScintilla(SCI_BRACEMATCH, bracePos, 0)`
    failed to compile ("call ... is ambiguous") because the literal `0`
    matched multiple overloads at once (`long`, `void *`, `const char *`).
    Fixed with explicit casts to the exact `(unsigned long, long)` overload.
  - A `//` comment line ending in a single trailing backslash was merging
    with the next line (`-Wcomment` "multi-line comment" warning) -
    reworded so no comment line ends in `\`.
- New **Undo**/**Redo** actions, added to the Edit menu (above Cut) and
  the Edit toolbar, with newly drawn matching curved-arrow icons and
  the platform-standard shortcuts (`QKeySequence::Undo`/`::Redo` - e.g.
  Ctrl+Z/Ctrl+Y on Windows/Linux). Simply forward to the active tab's
  `textEdit->undo()`/`redo()` - Scintilla's own undo buffer isn't capped
  to any fixed step count by AmigaED, so it comfortably covers "at least
  10 steps" in both directions without any extra bookkeeping.

## rev.97
- New action **"Comment/Uncomment Block"** (Ctrl+/), added to the
  Comments submenu - which, being shared between the main Inserts menu
  and the editor's context menu, automatically appears in both.
  Toggles a `// ` line-comment prefix on every line of the current
  selection (or just the current line if nothing is selected): if every
  non-blank line in range already starts with `//`, it strips the
  prefix from each; otherwise it adds `// ` to each. Grouped into a
  single undo action, and re-selects the affected block afterwards so
  an immediate second press toggles it straight back.

## rev.96
- Renamed View menu entry "Fold all..." to **"Fold/Unfold all..."** and
  rewrote its logic: it now uses Scintilla's own `SCI_FOLDALL` message
  with an explicit contract/expand action, forcing every fold point in
  the document to the same state in one call - reliable even when some
  blocks were already folded/unfolded by hand first (the previous
  `foldAll(bool)`-based version toggled each fold header based on its
  own current state, which produced an inconsistent result from a mixed
  starting point). Repeated presses alternate between "fold everything"
  and "unfold everything", tracked independently of Scintilla's
  per-line state.
- Navigation/"Go to matching bracket" rewritten: it now explicitly
  checks both the character right before AND right after the caret for
  a bracket - the previous version relied solely on QScintilla's
  `moveToMatchingBrace()`, which only reliably matches a bracket
  sitting at the caret's position (to its right), silently doing
  nothing for the more common case of the caret sitting right *after*
  a bracket. The caret now always lands right after whichever bracket
  it jumped to, in both directions.

## rev.95
- **Search and Replace fully rewritten.** The previous implementation
  took an action parameter but never actually branched on it, so
  Find-next/Find-previous/Replace/Replace-all all silently did the
  exact same (broken) thing, and the replace field was never even
  read - nothing was ever replaced. Rewritten from scratch using
  QsciScintilla's own `findFirst()`/`findNext()`/`replace()` directly:
  Find-next/previous now search forward/backward correctly; Replace
  replaces the current match and advances to the next one; Replace-all
  processes the whole document from the start (safe against infinite
  loops even when the replacement text contains the search text) and
  reports how many occurrences it replaced.
- New context menu entry **"Search and Replace..."**, topmost, followed
  by a separator above "What to insert?". If a word sits under the
  click, it is used as the initial search term (via QScintilla's
  `wordAtPoint()`).
- Preprocessor/"Identify Amiga compiler": the GNU gcc branch now also
  checks the GCC version at compile time - GCC >= 6.5 appends an extra
  line, "Probably Beppo's amiga-gcc or one of its forks.", to
  `compiler_string`.
- New toolbar button for the existing "Clean Project" action (a
  freshly drawn, multi-colored broom icon, same size/style as the
  other toolbar icons), placed right next to "Build Project".
- Fixed the default-icon-copy step in generated `Makefile.gcc` failing
  with "Syntaxfehler" for some icon paths on Windows: the path had its
  backslashes doubled before being embedded in the `cmd /c copy`
  recipe line, inconsistent with (and apparently not as reliable as)
  the already-proven single-backslash pattern used for the same sh/cmd
  handoff elsewhere in this project (`install_stage.bat`'s invocation
  in `AmigaED.pro`). Removed the doubling.
- Both manuals' "Recommendations for Amiga C Programmers" chapter:
  expanded the Codecraft entry with its full description and added a
  second source link (http://boemann.dk/codecraft/), in the PDFs, the
  HTML exports, and both languages.

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
