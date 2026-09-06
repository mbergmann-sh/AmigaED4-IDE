# AmigaED 4.0 – Revisions

Changelog for AmigaED 4.0, newest revision first. The revision number
shown here matches `AMIGAED_REVISION` in `version.h`, which in turn
appears in every window title as `AmigaED 4.0 rev.<n>`.

> Revisions 53–79 predate this changelog and are not individually
> documented here (see the "Earlier milestones" section at the bottom
> for what's known about the wider rev1–52 range).

## rev.112
- ReAction template rewritten again, this time checked line-by-line
  against a confirmed-working ReAction/vbcc program the user provided
  (window.class + gadtools.library menu + requester.class). Found and
  fixed several real discrepancies that earlier revisions had gotten
  wrong by inference alone:
  - Menu layout: switched from `LayoutMenusA(menuStrip, visualInfo,
    TAG_DONE)` to `LayoutMenus(menuStrip, visualInfo, GTMN_NewLookMenus,
    TRUE, TAG_DONE)` - the varargs form, with `GTMN_NewLookMenus` for a
    proper ReAction-style menu appearance, matching the reference
    exactly (the "A" form isn't what the working program uses).
  - Window content: `WINDOW_ParentGroup` is the actual window.class
    tag for attaching the root gadget/layout tree - `WINDOW_Layout`,
    used in rev.110/111, isn't a real window.class tag at all, and
    (silently ignored rather than erroring) would have left the window
    just as content-less as before that "fix".
  - Object creation: switched every `NewObject(NULL, "xxx.class"/
    "xxx.gadget", ...)` call to `NewObject(XXX_GetClass(), NULL, ...)`
    using the `_GetClass()` accessors from `<proto/window.h>`/`<proto/
    layout.h>`/`<proto/string.h>`/`<proto/requester.h>` - what the
    reference program actually uses throughout, rather than passing
    class names as plain strings.
  - Window open/input/close: switched from raw `DoMethod(obj, WM_OPEN/
    WM_HANDLEINPUT/WM_CLOSE)` calls to the `RA_OpenWindow()`/
    `RA_HandleInput()`/`RA_CloseWindow()` macros from `<reaction/
    reaction_macros.h>` (also newly included, alongside `<reaction/
    reaction.h>`), and menu strip attachment moved to `SetMenuStrip()`
    called AFTER opening the window, rather than a `WINDOW_MenuStrip`
    tag at creation time - both match the reference exactly.
  - Library versions: intuition.library/gadtools.library/window.class/
    string.gadget/requester.class all opened with version 0L ("any"),
    except `gadgets/layout.gadget` specifically at 47L - matching the
    reference's own version numbers exactly, rather than 39/44
    guessed in earlier revisions.
  - Dropped `utility.library` entirely - unused in the reference
    program, and not actually needed by anything else in this
    template either.
  - `<clib/alib_protos.h>` in place of `<proto/alib.h>` for DoMethod/
    NewObject/etc.'s prototypes, matching the reference's own include
    (both likely work, but this matches confirmed-working code
    exactly rather than a plausible alternative).

## rev.132
- Both manuals (HTML - `help/manual_en.html`/`manual_de.html` - and PDF
  - `DOC/AmigaED_Guide_EN.pdf`/`AmigaED_Anleitung_DE.pdf`) updated to
  cover everything shipped since the last documentation pass: the
  built-in program icon and its per-target stack sizes, File > Close
  Project, the safer file-by-file Clean Project, Makefiles now also
  regenerating before every build, case-insensitive FLOAT/DOUBLE
  detection, the three themes (Dark/Workbench 1.3/Workbench 3.1) and
  the View > Theme menu, the emulator "leave it open?" prompt and
  external-process detection, and the Compiler Output pane's
  colour-coded errors/warnings with click-to-jump.
- New **Toolbar Reference** chapter: every one of the 19 toolbar
  buttons, illustrated with the app's own real icon images (not a
  mockup) pulled directly from its `images/` resources, each mapped to
  its equivalent menu entry and a one-line description.
- New **Quick Start: From Template to Clean Build** chapter: a
  concrete, illustrated (4 new screenshots) walk-through of creating a
  project from a template, writing code, building it, and cleaning it
  up again - the same workflow Mode 2 already describes, made
  concrete.
- New **The Compiler Output Pane** chapter covering the colour-coding
  and click-to-jump behaviour, with the two recognized VBCC/GCC
  message formats.
- The PDF manuals are now rendered directly from the same HTML source
  (via `QTextDocument::print()`, `docbuild/render_pdf.py`) instead of
  a separate, hand-maintained set of ReportLab scripts (one per
  chapter) - a single source of truth for both formats, and no more
  risk of the two silently drifting apart. See `docbuild/README.md`
  for the full pipeline and how to regenerate everything after a
  future change.

## rev.131
- **Fixed the Linux startup crash** (confirmed via a real backtrace:
  `SIGSEGV` inside `QTabWidget::count()`, immediately after
  `readSettings()`'s own debug output). Root cause: `tabWidget` and
  `output` had no `= nullptr` default member initializer, so before
  their own `new QTabWidget(this)`/`new QPlainTextEdit` lines run later
  in the constructor, they held whatever indeterminate value happened
  to already occupy that memory - not reliably null. `applyApplicationStyle()`
  runs very early in the constructor (well before either of those
  lines), and since rev.124 it also calls `reapplyEditorTheme()`/
  `highlightOutputDiagnostics()`, both of which guard themselves with
  `if (!tabWidget)`/`if (!output)` - a guard that only actually protects
  anything if the pointer is guaranteed null beforehand, which these
  weren't. Windows/MinGW's memory for that address apparently
  happened to read back as zero, masking the bug entirely there;
  Debian's didn't, causing a hard crash on every single startup. Both
  now have an explicit `= nullptr` default, so their guards are
  reliable regardless of what the pointer used to contain. Also
  checked every other pointer given a similar `if (!ptr)` guard in
  recent revisions (`themeMenue`, `themeActionGroup`, `myEmulator`) -
  all three already had a proper `= nullptr` default and were never
  at risk.

## rev.130
- **Serious fix**: "Clean Project" could wipe an entire project
  directory on Windows - confirmed by a real test case: the .aep,
  every source file, both other Makefiles, everything in the folder
  was gone after clicking Clean Project once. Root cause: rev.128's
  Windows fix for the generated Makefile's clean: rule
  (`cmd /c del /Q $(OBJS) $(TARGET)`) - with $(TARGET) being an
  extension-less filename (the normal case for an Amiga executable,
  e.g. "ftest2", not "ftest2.exe"), something in the sh.exe/cmd.exe/
  DEL interaction deleted far more than intended. The exact mechanism
  wasn't pinned down with certainty even after reviewing the generated
  Makefile line by line - reproducing the exact failure without a real
  Windows machine to test against wasn't possible - so rather than
  patch that shell command further and hope, this removes the risk
  entirely instead of chasing it further:
  - **AmigaED's own Clean Project button no longer runs "make clean"
    at all.** It now computes the exact list of object files, the
    target, and its .info icon (mirroring what
    `regenerateProjectMakefiles()` itself computes) and deletes each
    one directly via Qt (`QFile::remove()`, one exact path at a time) -
    no shell, no wildcards, no multi-file command line of any kind.
    Each removal (or failure) is listed in the output pane.
  - The Makefile's own `clean:` rule (for anyone running `make clean`
    directly from a shell, outside AmigaED) is hardened as a best-
    effort second layer: one explicit, quoted, existence-checked
    `if exist "file" del /Q "file"` line per object file and the
    target individually, rather than a single multi-argument `del`
    line - but AmigaED's own button above no longer depends on this
    rule at all, and is the safe path to prefer.
  - The SAS/C Makefile's own `clean:` (`delete $(TARGET)`, using the
    Amiga Shell's own `delete` command) is unrelated to this - it runs
    on a real Amiga/emulator, never on the host machine, and was never
    part of this bug.

## rev.129
- Fixed one more leftover from the same "stuck on the previous theme"
  bug class as rev.126/127: `initializeMargin()`'s light-theme branch
  never called `setFoldMarginColors()` at all - only the dark branch
  did - so the fold margin (the thin strip just left of the line-number
  gutter) stayed black indefinitely after switching away from "Dark"
  to any other theme, confirmed by screenshot. Swept the rest of the
  file for the same pattern (an `isDarkTheme()` branch with no light-
  theme counterpart setting the same property) - this was the last one
  left.

## rev.128
- Fixed generated Makefiles always using `rm -f` for their "clean"
  target, even on Windows, where it fails outright unless the
  toolchain happens to have a real coreutils `rm.exe` on PATH (not
  guaranteed). Windows now gets `cmd /c del /Q $(OBJS) $(TARGET)`
  instead - the same sh.exe/cmd.exe handoff already established for
  the icon-copy code (rev.94/98/105) applies here too: `del` is a
  cmd.exe built-in, not a standalone executable, so a bare `del` in a
  Makefile recipe fails the same way a bare `copy` did.
- Fixed `projectUsesFloatingPoint()` (decides whether the generated
  Makefiles add `-lm`/`-lmieee`/`MATH=IEEE`) missing floating-point
  usage written with exec/types.h's own all-caps `FLOAT`/`DOUBLE`
  typedefs - at least as common in idiomatic Amiga C as the plain
  lowercase keywords, and confirmed against a real project using
  `DOUBLE` that this case-sensitive check missed entirely, silently
  generating Makefiles with no math library linked in at all. Now
  case-insensitive.
- Fixed `actionBuildProject()` never actually regenerating the
  Makefiles before building - it only checked that one already existed
  on disk from whenever it was last generated (project creation, or
  the last time a file was added/removed), so a plain CONTENT change
  to an already-tracked file (such as newly adding floating-point
  usage) was never picked up by a build until something else also
  happened to add or remove a file. Project builds now regenerate the
  Makefiles fresh every time, immediately before checking for one -
  matching what most other project actions already did.

## rev.127
- rev.126's editor-theme fix didn't actually work (confirmed by
  screenshot: an already-open tab stayed fully dark after switching to
  "Workbench 3.1") - it looped `QsciLexer::defaultColor(style)`/
  `defaultPaper(style)` across style indices 0-127 regardless of
  whether the lexer actually defines that many styles, which evidently
  doesn't repaint an already-open tab reliably (and may well be the
  cause of a separate report: AmigaED crashing with a memory access
  violation on startup on Debian 13 - looping to arbitrary, possibly
  undefined style indices is exactly the kind of thing that could
  behave unpredictably depending on the local QScintilla build).
  Replaced with the same explicit, bounded, named-style-constant
  mechanism (dynamic_cast per lexer type, blanket setPaper()/setColor()
  then individual re-tints) already proven to work for the dark
  branch - just with light-appropriate colours instead - for all seven
  lexer types (C/C++, Makefile, Batch, Pascal, and the three custom
  Amiga-specific lexers). If the Debian crash was indeed caused by that
  loop, this revision should also resolve it - please retest and let
  me know either way, ideally with a backtrace (e.g. via `gdb
  --args ./AmigaED`, then `run` and `bt` once it crashes) if it still
  happens, since I can't otherwise tell it apart from an unrelated
  cause.
- Fixed a related, previously-unnoticed gap in the same area:
  `initializeCaretLine()`'s light-theme branch only ever reset the
  caret line's own background colour - selection background,
  indentation guides, whitespace, and matched/unmatched brace colours
  all stayed stuck showing dark-theme values indefinitely after
  switching away from "Dark" too. All of them now have an explicit
  light-theme counterpart, symmetric with the dark branch.

## rev.126
- Fixed the editor sometimes staying in the previous theme's colours
  after switching themes: `applyLexerDarkColors()` and
  `initializeLexerNone()` (used for plain-text tabs with no lexer at
  all) were both a one-way "make it dark" - neither had any code path
  to undo that and restore the light-theme colours when switching AWAY
  from "Dark" to any other theme (native styles, Workbench 1.3,
  Workbench 3.1), so an already-open tab that had been dark-coloured
  stayed that way indefinitely afterward. Both now explicitly reset
  every style back to the lexer's own class defaults (plus this app's
  own small set of additional light-theme customizations -
  QsciLexerCPP's GlobalClass/KeywordSet2) when the theme isn't "Dark",
  exactly mirroring what a brand new tab gets.
- Fixed "Console Debugging Message" (Inserts menu / context menu)
  always inserting its `if (myDebug) {...}` block flush against column
  0 regardless of where the cursor actually was, breaking the visual
  indentation of whatever code it was inserted into - a hardcoded `0`
  where the cursor's own original column should have been used
  throughout. Every line of the block, and the final cursor position,
  now use that original column instead.

## rev.125
- Fixed a false-positive match in the GCC regex, found via a real
  multi-file test build: GCC's own "In file included from foo.c:12:0:"
  lines were silently matching the columnless diagnostic pattern - a
  bare `\w+` also matches pure digits, so the "0" (a column number,
  not a diagnostic keyword) satisfied it as if it were one, making
  that line clickable (though never coloured, since "0" contains
  neither "error" nor "warning"). Tightened the diagnostic-keyword
  group to letters only (`[A-Za-z]+` instead of `\w+`), and added a
  second check requiring the captured word(s) to actually be one of
  GCC's real diagnostic keywords (error/warning/note/fatal error)
  before accepting a match at all - both confirmed against the exact
  lines from the test build that surfaced this.

## rev.124
- Output pane error/warning highlighting (rev.123) now updates live
  when the theme changes, output already on screen included - it
  previously only ever coloured newly-appended output, so switching
  themes while errors/warnings were already showing left them in the
  old theme's colours. Found and fixed a related, slightly older gap
  while at it: the View > Theme menu (rev.115) called
  `applyApplicationStyle()` directly but never `reapplyEditorTheme()`,
  so switching to/from "Dark" through that menu specifically (as
  opposed to via Prefs, which already called both) left already-open
  editor tabs' margin/caret-line/lexer colours stale too. Both
  `reapplyEditorTheme()` and the new `highlightOutputDiagnostics()`
  call are now made once, centrally, at the end of
  `applyApplicationStyle()` itself - so every path that changes the
  theme (Prefs, View > Theme, Shift+F12 reload) re-colours both
  consistently, automatically, with nothing to remember at each call
  site.

## rev.123
- Fixed clicking a line in the output pane not reliably jumping to the
  matching error/warning: `on_output_cursorPositionChanged()` looked up
  the clicked line via `output->toPlainText().split('\n',
  Qt::SkipEmptyParts)` - since SkipEmptyParts drops blank lines from
  that list while the cursor's own blockNumber() still counts them, the
  two fell out of sync the moment the build output contained even one
  blank line before the clicked one (routine in real compiler output),
  silently checking the wrong line's text against the regex. Now reads
  the exact clicked line directly via the document's own block
  structure (`document()->findBlockByNumber()`) instead - no
  re-splitting, nothing to fall out of sync.
- Output pane error/warning lines are now colour-highlighted (bold text
  + tinted background, errors and warnings in their own colours),
  theme-aware (one scheme for the dark theme, one shared by every
  light-background theme - native styles, Workbench 1.3, Workbench
  3.1), reusing the exact same VBCC/GCC regex parsers
  (`checkVBCC()`/`checkGCC()`, selected by the currently selected
  compiler) that already drive click-to-jump - so a line is coloured
  if and only if it's also clickable.
- Fixed the GCC regex not recognizing "fatal error:" lines at all (a
  plain `\w+` can't match two words) - `(\w+(?:\s+\w+)?)` now allows
  one optional second word before the colon, still matching plain
  single-word "error"/"warning"/"note" exactly as before.

## rev.122
- Replaced the ReAction template's placeholder "read-only string
  gadget" approach entirely - both attempts at forcing a string.gadget
  read-only failed, each on a different compiler: `GA_ReadOnly` isn't a
  real identifier at all (SAS/C: "undefined identifier"), and
  `STRINGA_Editable`, though genuinely documented elsewhere, isn't
  defined in this particular NDK/vbcc header set either ("unknown
  identifier"). Switched to `label.image` (`LABEL_GetClass()`,
  `LABEL_Text`, opened as `images/label.image`, added to the layout via
  `LAYOUT_AddImage` rather than `LAYOUT_AddChild`) instead - the class
  actually meant for static, non-interactive text, exactly what the
  reference program's own `CHILD_Label` entries use (here attached
  directly to the layout rather than as another gadget's label), and
  sidesteps the whole read-only-attribute-naming question entirely.

## rev.121
- Fixed the ReAction template's placeholder string gadget not
  compiling under SAS/C ("undefined identifier <GA_ReadOnly>") -
  `GA_ReadOnly` isn't a real, documented BOOPSI/string.gadget
  attribute at all (vbcc's headers apparently let the undefined
  identifier through without complaint; SAS/C's correctly rejected
  it). Replaced with `STRINGA_Editable, FALSE` - the actual,
  documented string.gadget attribute for making it a non-editable,
  read-only text display.

## rev.120
- Fixed the Workbench 1.3 theme's menu bar being unreadable: Fusion's
  own menu bar rendering synthesizes its shading from a single
  `QPalette::Window` colour, and against this theme's saturated blue
  that washed out badly enough to make the menu text (white, per the
  palette) blend completely into the background - confirmed by
  rendering it offscreen with and without a fix. Added an explicit
  stylesheet for just the menu bar/menus while this theme is active,
  sidestepping Fusion's gradient synthesis entirely rather than fighting
  it further; cleared again automatically when switching to any other
  theme/style.

## rev.119
- Corrected both Workbench themes (rev.115) against real Workbench 1.3
  and 3.1 screenshots - the two were effectively swapped/invented
  before: Workbench 1.3's actual desktop is a solid BLUE (#0055AA,
  sampled directly from a screenshot) with a white title bar and white
  icon-label text, not the flat gray guessed originally; Workbench 3.1
  is the plainer, NEUTRAL GRAY one (#AAAAAA, also sampled directly)
  with black text throughout, not the invented periwinkle-blue-gray
  before. The classic Amiga blue now appears as 1.3's dominant desktop
  colour and, appropriately demoted, as 3.1's selection highlight only -
  giving the two themes a family resemblance while keeping them clearly
  distinguishable, matching how the real desktops actually differ.

## rev.118
- Fixed the ReAction template not compiling under SAS/C (vbcc/gcc, both
  C99, compiled it fine): SAS/C uses C89, which requires every
  declaration in a block to come before any statement in that same
  block. `Object *textGadget = ...;`/`layoutObj`/`winObj` were each
  declared-with-initializer after `if (menuStrip) LayoutMenus(...);` (a
  statement) earlier in the same block - legal C99, but exactly the
  SAS/C errors reported (218 "declaration found in statement block",
  77, 90, ...). Restructured to plain declarations
  (`Object *textGadget;` etc.) grouped together at the top of that
  block, assigned via separate statements afterward in their original
  positions. Checked the other five templates (Empty C, Shell, AmigaOS
  1.3, AmigaOS 3.x, MUI) for the same category of issue too - none of
  them mix declarations and statements in a way C89 would reject.

## rev.117
- Fixed AmigaED not recognizing an emulator already running at startup
  (e.g. one left open on purpose from a previous session): the
  external-process check (`isEmulatorProcessRunningExternally()`,
  added in rev.111) was only ever run when the user clicked Start -
  nothing checked at startup itself. Now checked once during
  construction too, right after the toolbar/menu are built; if found,
  Start is disabled and Stop enabled immediately, with no popup (unlike
  clicking Start while one is already running, this never launches
  anything, so there's nothing to confirm).
- Fixed the real reason the ReAction template's menu still did nothing
  even after rev.116's `WMHI_MENUMASK` fix: the window never requested
  `WA_IDCMP` at all, so `RA_HandleInput()` never actually received
  `WMHI_MENUPICK` (or `WMHI_CLOSEWINDOW`) events in the first place -
  confirmed against the reference program, which explicitly requests
  `IDCMP_MENUPICK`/`IDCMP_CLOSEWINDOW` (among others) via `WA_IDCMP`.
  Added `WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_MENUPICK` to the window's
  own tag list, with a comment on adding more flags (`IDCMP_GADGETUP`,
  `IDCMP_NEWSIZE`, ...) as more gadgets are added.

## rev.116
- Fixed the ReAction template's menu picks doing nothing at all
  (confirmed: window opened fine, but neither About nor Quit worked
  from the menu) - `ItemAddress(menuStrip, (UWORD)result)` used a raw
  cast of the whole packed `WMHI_MENUPICK` result instead of masking
  out just the menu/item number with `result & WMHI_MENUMASK` (as the
  reference program actually does), so `ItemAddress()` never resolved
  to the right (or any) `MenuItem`, and `GTMENUITEM_USERDATA()` on it
  never matched `MENU_ABOUT`/`MENU_QUIT` either.

## rev.115
- Two new nostalgic application themes, alongside the existing native
  styles and "Dark": **Workbench 1.3** (flat mid-gray chrome, black
  text, classic Amiga blue selection, orange accent - the same 4-colour
  palette AmigaED's own built-in tool icon uses) and **Workbench 3.1**
  (the cooler, richer periwinkle-blue chrome and deeper navy selection
  AmigaOS 3.x's Workbench became known for). Both are synthetic
  entries handled the same way "Dark" already was (force the "Fusion"
  style plus a custom `QPalette`), added to Prefs > Misc's "Default
  application style" combo box.
- New **View > Theme** submenu, listing every style available on this
  platform (native `QStyleFactory` styles, plus Dark/Workbench 1.3/
  Workbench 3.1) as checkable, mutually exclusive entries (one shared
  `QActionGroup`) - picking one changes AmigaED's theme immediately, no
  restart needed, and stays in sync with Prefs > Misc's own style
  setting no matter which of the two is used to change it.

## rev.114
- Prefs dialog layout tightened up (user-provided `prefsdialog.ui`),
  mainly closing the large empty gaps left in the Project and Emulator
  tabs since rev.105 removed the "Default icon" row - fields now sit
  compactly instead of spread out with dead space between them.
  Checked before merging: all 97 widgets present under their original
  names and classes (nothing `prefsdialog.cpp` relies on could have
  broken), no duplicate widget/layout names, all buddy/tabstop
  references still valid, all icon resource paths still resolve, and
  the dialog renders cleanly across all six tabs.

## rev.113
- Fixed Start/Stop Emulator (toolbar buttons, and the equivalent Tools
  menu entries - the exact same `QAction`s in both places) not
  toggling when an externally-tracked emulator process was found
  running (rev.111's "start another instance anyway?" prompt,
  answered "No"): since AmigaED never actually launched that process
  itself, `myEmulator` stayed `NotRunning`/null throughout, so nothing
  ever disabled Start or enabled Stop for it. New `p_externalEmulatorTracked`
  flag covers this case explicitly: Start gets disabled and Stop
  enabled right when the user declines starting a second instance;
  Stop, when triggered for this case, now terminates that process by
  executable name via the platform's own tools (`taskkill /F` on
  Windows, `pkill -x` on Linux/macOS - there's no `QProcess` handle to
  call `terminate()`/`kill()` on for a process AmigaED didn't start)
  rather than doing nothing; and the periodic poll
  (`checkEmulatorStillRunning()`) now also re-checks by name whether
  this externally-tracked process is still running, resyncing
  Start/Stop back once it (however it happened) ends. closeEvent()'s
  "leave the emulator open?" prompt now also triggers for this case,
  not just one AmigaED itself started.
- ReAction template rewritten again - see the detailed rev.112 entry
  above for the full list of corrections found by checking it
  line-by-line against a confirmed-working ReAction/vbcc program.

## rev.111
- Fixed the ReAction template's "Failed to create/layout the menu
  strip." reported by rev.110's new diagnostics: `LayoutMenusA()` was
  passed `NULL` as its VisualInfo argument, which makes it fail
  outright - it needs a real one (fonts/pens for the screen the menu
  will appear on), obtained via `LockPubScreen(NULL)` (locks the
  default public screen, i.e. Workbench) + `GetVisualInfoA()`, freed
  again (`FreeVisualInfo()`/`UnlockPubScreen()`) right after layout,
  since nothing needs it to stay alive past that point.
- Fixed a real bug in the new "leave the emulator open on exit"
  feature (rev.110): choosing to leave it open still produced a
  spurious "CrashExit - UAE has a problem!!" report and killed it
  anyway, because `QProcess`'s own destructor kills its still-running
  child process when destroyed - which happened regardless of skipping
  AmigaED's own `actionKillEmulator()` call, once the `QProcess` member
  itself got destroyed along with the rest of `MainWindow` at app exit.
  `myEmulator` is now a heap-allocated `QProcess *` with no `QObject`
  parent (rather than a plain value member) specifically so it can be
  left deliberately un-destroyed in that one case, letting the actual
  emulator process survive AmigaED's exit intact.
- Corrected the shutdown prompt's wording to match exactly: "AmigaED
  is shutting down while your Amiga Emulator is up and running. Do you
  want me to leave the Emulator open?" - the previous wording didn't
  make clear enough what "it"/"open" specifically referred to.

## rev.110
- ReAction template: attempted fix for the window never actually
  opening (program ran, exited cleanly, but showed nothing - neither
  from Workbench nor the Shell). The window had no content at all (no
  `WINDOW_Layout`) and no explicit size - the same underlying class of
  problem the MUI template's window had before rev.108's fix. Added a
  minimal placeholder read-only string gadget inside a single-child
  `layout.gadget`, plus explicit `WA_Width`/`WA_Height`. Also added
  `Printf()` diagnostics at every failure point (library opens, menu
  creation, window object creation, `WM_OPEN`) that were previously
  completely silent, so if this doesn't fully resolve it, running the
  program from a Shell will now say exactly which step failed.
- New: closeEvent() no longer unconditionally kills a running emulator
  on exit - if one is still running once AmigaED is actually committed
  to closing (i.e. after any unsaved-changes prompts), it now asks
  "AmigaED is shutting down while your Amiga emulator is still
  running. Do you want to leave it open?" (Yes/No, defaulting to Yes)
  before deciding whether to stop it.
- New: actionEmulator() (Start Emulator) now also checks, via the
  platform's own process list (`tasklist` on Windows, `pgrep` on
  Linux/macOS) rather than just AmigaED's own bookkeeping, whether a
  process matching the configured emulator's executable name is
  already running anywhere on the system - covering the case where a
  previous AmigaED session left one open (see above), or it was
  started outside AmigaED entirely - and asks before starting a
  second instance rather than doing so unconditionally. Matches by
  executable name only, so it can't distinguish two different configs
  of the same emulator (e.g. separate OS 1.3 / OS 3.x setups) from
  each other - deliberately asks rather than silently refusing, since
  running two on purpose is legitimate here.

## rev.109
- Fixed three real VBCC compile problems in the ReAction template:
  - `struct Library *IntuitionBase;` conflicted with `<proto/intuition.h>`'s
    own extern declaration of it as the more specific
    `struct IntuitionBase *` (intuition.library's own extended
    library-base type) - reported as "redeclaration of var
    <IntuitionBase> with new type". Removed the redundant declaration
    and cast `OpenLibrary()`'s result to `struct IntuitionBase *` at
    the assignment instead. UtilityBase/GadToolsBase have no such
    library-specific base type, so those two are still declared
    (as plain `struct Library *`) same as before.
  - `WINDOWCLASS` and `REQUESTERCLASS` ("unknown identifier" errors) -
    this NDK/vbcc header set doesn't actually define those convenience
    macros even with `<classes/window.h>`/`<classes/requester.h>`
    included. Both `NewObject(NULL, ...)` calls now pass the plain
    string ("window.class"/"requester.class") directly instead -
    exactly what those macros would have expanded to anyway.
  - "implicit declaration of function <DoMethod>" - the BOOPSI helper
    functions (`DoMethod`, `NewObject`, `DisposeObject`, `SetAttrs`,
    `GetAttr`, ...) are declared in `<proto/alib.h>` (mirroring the
    classic `<clib/alib_protos.h>`), not by any of the headers already
    included - added.

## rev.108
- Fixed the MUI template compiling cleanly but never actually showing
  its window: the window object was created with no
  `MUIA_Window_RootObject` at all - a MUI window with no content isn't
  a valid, functioning window, and neither MUI nor the template's own
  `if (app && win)` check caught this (both `app` and `win` still come
  back non-NULL), so it failed completely silently. Added a minimal
  placeholder `MUIC_Text` object as the window's root content, and a
  `Printf()` diagnostic in the (until now silent) failure branch for
  if object creation ever does fail outright.

## rev.107
- Fixed the line-number margin turning light gray (instead of staying
  dark) for any .c/.h/.cpp file actually opened, while the "Dark"
  application style was active - a brand new blank tab was unaffected,
  which is what made this easy to miss. Cause: `initializeLexerCPP()`
  was the only one of AmigaED's lexer-init functions that never called
  `initializeMargin()` itself. That's harmless the first time (a new
  tab already got its margin set once, right after its own initial
  C/C++ lexer was created), but opening an actual .c/.h file re-runs
  `initializeLexerCPP()` a second time on that same tab (via
  `applyLexerForFileExtension()`), and setting a fresh lexer that
  second time reset the margin to its own non-dark default colours -
  which nothing afterward corrected back. Added the missing call.

## rev.106
- Fixed a real compile error in the MUI template (VBCC: "error 43 ...
  initialization of incomplete struct" on the `struct NewMenu nm[] =
  {...}` array): `<libraries/gadtools.h>`, which actually declares
  `struct NewMenu` and the `NM_TITLE`/`NM_ITEM`/`NM_END` constants used
  there, was missing - `<libraries/mui.h>` alone doesn't provide them.
  Added (gadtools.library itself is still never opened/called in the
  MUI template - only the struct/constant definitions are needed).
- All "New Project" templates except AmigaOS 1.3 (which has no
  dos.library Printf() the way later NDKs provide it) now include
  `<dos/dos.h>` and `<proto/dos.h>`, so `Printf()` - dos.library's
  byte-saving, AmigaOS 3.x-typical alternative to ANSI C's `printf()`
  - is available for error output without adding these by hand.
  Affects the ReAction, MUI, and Empty C templates (Shell and AmigaOS
  3.x already had both).

## rev.105
- Replaced the "default icon" file entirely with a built-in AmigaED
  icon: a small, multi-coloured classic Amiga Tool icon (DiskObject +
  Gadget + Image, 2 bitplanes/4 colours, valid on Kickstart 1.3 through
  3.x), embedded as a Qt resource (`resources/amigaed_tool.info`) and
  hand-built byte-for-byte per the documented on-disk icon format - no
  Amiga tools involved in generating it.
- AmigaED now WRITES this icon itself, directly (`writeProgramIcon()`),
  right after a successful build - both a single-file (ad-hoc) compile
  and a project build - whenever Prefs > Misc > "create icon" is
  checked. This replaces copying a user-chosen file via a shell
  command embedded in the generated Makefile, which was a recurring
  source of Windows-specific breakage (sh.exe/cmd.exe quoting/escaping
  - see rev.94, rev.98) - writing it from within Qt/C++ sidesteps that
  category of bug entirely, at the cost of the icon no longer being
  created if "make" is run directly outside AmigaED.
- The icon's `do_StackSize` field is set per target: 4096 for AmigaOS
  1.3, 8192 for AmigaOS 3.x (also the default for Empty C/Shell
  projects, and for ad-hoc single-file compiles targeting OS 3.x),
  16000 for ReAction projects, 34000 for MUI projects.
- Removed the now-unneeded "Default icon" file/browse widgets from
  Prefs > Project (the "create icon" checkbox itself, in Prefs > Misc,
  stays).

## rev.104
- ReAction template's About box now uses `requester.class`
  (`REQUESTERCLASS`/`RM_OPENREQ`) instead of the older Intuition
  `EasyRequestArgs()` - ReAction's own BOOPSI requester, matching the
  rest of the template's window.class/gadtools.library-based approach.
  Opens `requester.class` alongside the other libraries (added to the
  startup check and the cleanup CloseLibrary() calls too).

## rev.103
- Fixed "Close Project" leaving other project tabs open, closing only
  the main .c file. `closeProjectTabs()` only recognized a tab as
  belonging to the project if its file was tracked in
  `currentProject->files` - but the project's own auto-generated
  Makefiles are deliberately NOT tracked there (see
  actionRemoveFileFromProject()), so a Makefile tab (or any other
  untracked file physically sitting in the project's own directory)
  was treated as "unrelated" and left open. Now also matches by
  directory: any open tab whose file lives directly in the project's
  own folder counts as belonging to it, tracked or not.

## rev.102
- Moved "Show output pane..." / "Hide output pane..." from the Build
  menu to the View menu, directly below "Hide Functions Browser".
- MUI and ReAction "New Project" templates rewritten from bare
  library-opening skeletons into actually working small GUI programs:
  both now build a real window with a **File** menu containing
  **About** and **Quit**. About pops up a modal requester titled
  "About this Program" with the text "This Programm was created with
  the help of AmigaED 4.0" and an "OK" button - via `MUI_Request()` for
  the MUI template, and via `EasyRequestArgs()` (the equivalent
  AmigaOS/ReAction-native call, no MUI dependency) for the ReAction
  template. Quit closes the window and ends the program cleanly in
  both. The ReAction template builds its menu the standard
  gadtools.library way (`CreateMenusA()`/`LayoutMenusA()`/
  `ItemAddress()`/`GTMENUITEM_USERDATA()`) and its window via
  `window.class` (`WM_OPEN`/`WM_HANDLEINPUT`/`WM_CLOSE`), matching
  AmigaOS 3.2/NDK3.2R4-typical ReAction code rather than MUI's own API.

## rev.101
- "Comment/Uncomment Block" promoted out of the context menu's
  "Comments..." submenu (one click away, no submenu hover needed) to
  the very topmost entry of the context menu, followed by a separator
  - ahead of even "Search and Replace...". On the main Inserts menu it
  is likewise now a standalone top-level entry rather than nested in
  "Comments...".
- Search and Replace: fixed "Replace" (single occurrence) not actually
  replacing anything until pressed several times, and only then after
  a Replace All had already run once. The previous version only
  replaced when the current selection's text compared exactly equal
  to the search term; rewritten to simply trust that any current
  selection is the match Find/Next just left behind (the same
  assumption most editors' Replace button makes) and replace it
  outright, then advance to the next occurrence.
- New menu entry **File > Close Project**: closes every tab belonging
  to the current project (prompting to save any with unsaved changes
  first, same as switching to a different project), then returns to
  the "no project loaded" state. Cancelling a save prompt leaves the
  project and all its tabs open untouched.

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
