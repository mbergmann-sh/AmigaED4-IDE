/****************************************************************************
**
** Copyright (C) 2018/2019 Michael Bergmann. Placed in the public domain
** with some restrictions!
**
** This file is part of my enhanced AmigaED editor example, using classes of
** the Qt and QScintilla toolkits.
**
** You may use or enhance this piece of software anyway you want to - as long
** as you don't violate laws or copyright issues.
** I hereby explicitely prohibit the usage of my work for people who believe
** in racism, fascism and any kind of attitude against democratic lifestyle.
** It is self-explanatory that this prohibits the usage of my work to any
** member or fan of the german AfD party.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** Further information about QScintilla licensing is available at:
** https://www.riverbankcomputing.com/software/qscintilla/license or by
** contacting sales@riverbankcomputing.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMessageBox>
#include <QString>
#include <QStyle>
#include <QStyleFactory>
#include <QPalette>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QStringConverter>
#include <QStatusBar>
#include <QLabel>
#include <QLCDNumber>
#include <QComboBox>
#include <QStatusTipEvent>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintEngine>
#include <QPageSize>
#include <QAction>
#include <QActionGroup>
#include <QActionEvent>
#include <QPoint>
#include <QProcess>
#include <QSplitter>
#include <QTabWidget>
#include <QTreeWidget>
#include "project.h"
#include <QListView>
#include <QTextBrowser>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QGroupBox>
#include <QPair>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QShortcut>
#include <QTimer>
#include <QElapsedTimer>
#include <QTime>
#include <QGridLayout>
#include <QFormLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QSpacerItem>

#include <QDialog>
#include <QDebug>
#include <QTranslator>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>

#include <utility>

class QAction;
class QActionGroup;
class QMenu;
class QLabel;
class QComboBox;
class QsciScintilla;
class QsciLexer;
class PrefsDialog;
class aboutDialog;
class QTimer;
class QTime;
class QElapsedTimer;
class QtGui;

#define CHECKTIME(x)  \
    QElapsedTimer CONCAT(sb_, __LINE__); \
    CONCAT(sb_, __LINE__).start(); \
    x \
    qDebug() << __FUNCTION__ << ":" << __LINE__ << " Elapsed time: " <<  CONCAT(sb_, __LINE__).elapsed() << " ms.";

class MainWindow : public QMainWindow
{
    // give the meta compiler a chance!
    Q_OBJECT

public:
    // Search and replace: which of the four buttons (or Enter in the
    // Find field, which behaves like FindNext) triggered
    // doSearchAndReplace(). Replaces the old, never-actually-checked
    // QString action_str ("0".."4") the previous implementation took
    // but never switched on.
    enum class SearchReplaceAction { FindNext, FindPrevious, Replace, ReplaceAll };

    explicit MainWindow(QString cmdFileName);
    bool fileExists(QString path);
    bool allready_selected = false;
    int last_selected_line = -1;
    #define MY_MARKER_ID 0

    // Layout and content for searchGroup
    QGridLayout *gridLayout_2;
    QLabel *label_find;
    QLineEdit *lineEdit_find;
    QGridLayout *gridLayout;
    QPushButton *btn_previous;
    QPushButton *btn_next;
    QLabel *label_replace;
    QLineEdit *lineEdit_replace;
    QFormLayout *formLayout_2;
    QPushButton *btn_replace;
    QPushButton *btn_replace_all;
    QPushButton *btn_hide;
    QFormLayout *formLayout;
    QCheckBox *checkBox_CaseSensitive;
    QCheckBox *checkBox_WholeWords;
    QCheckBox *checkBox_SearchForwards;
    QSpacerItem *horizontalSpacer_2;
    QWidget *centerSearchForm;

    QString line;
    QStringList fields;
    QElapsedTimer timerCompile; // Timer for compilation time
    int nMilliseconds;          // keeping compile time
    QString successMessage;     // build success message with compile time
    int line_nr = 0, column_nr = 0, error_nr = 0;   // keeps line and column numbers
    QString errortype;                              // keeps error type ('error' or 'warning')
    QString debugfilename;                          // keeps filename or unixpath


    // vars for controlling file header comments
    QString p_author;
    QString p_email;
    QString p_website;
    QString p_version = "1.0";
    QString p_revision = "0";
    QString p_compiler ;                     // C-Compiler to call...
    QString p_compiler_call;
    QString p_compiler_gcc;             // Path to gcc executable
    QString p_compiler_gpp;             // Path to g++ executable
    QString p_compiler_vc;              // Path to vc executable
    QString p_compiler_vasm;            // Path to vasm executable - used directly (not via vc) to assemble .asm/.s project sources, see regenerateProjectMakefiles()
    // Compiler + linker opts, split per compiler AND per target OS (OS 1.3
    // vs OS 3.x) - used for single-file compiles, the "New Project"
    // Compiler/Linker Options prompts, and the generated Makefiles' CCARGS
    // (compiler opts) / LDFLAGS (linker opts).
    QString p_compiler_gcc13_call;               // GCC compiler opts, OS 1.3
    QString p_compiler_gcc30_call;               // GCC compiler opts, OS 3.x
    QString p_compiler_gcc13_linker_call;        // GCC linker opts, OS 1.3 (e.g. "-mcrt=nix13 -noixemul -lamiga")
    QString p_compiler_gcc30_linker_call;        // GCC linker opts, OS 3.x
    QString p_compiler_gpp13_call;               // G++ compiler opts, OS 1.3
    QString p_compiler_gpp30_call;               // G++ compiler opts, OS 3.x
    QString p_compiler_gpp13_linker_call;        // G++ linker opts, OS 1.3
    QString p_compiler_gpp30_linker_call;        // G++ linker opts, OS 3.x
    QString p_compiler_vc13_call;                // VBCC compiler opts, OS 1.3 (must keep '+kick13' as its first token)
    QString p_compiler_vc30_call;                // VBCC compiler opts, OS 3.x (must keep '+aos68k' as its first token)
    QString p_compiler_vc13_linker_call;         // VBCC linker opts, OS 1.3
    QString p_compiler_vc30_linker_call;         // VBCC linker opts, OS 3.x
    QString p_compiler_sc_call;                  // SAS/C default opts (e.g. "ICONS ERRORREXX") - used ONLY for generating Makefile.sc; SAS/C itself is never invoked by AmigaED (it only runs on a real Amiga/emulator), and makes no OS 1.3/3.x distinction
    int  p_compiler_vc_default_target;          // holds the status bar's shared target-OS gadget value (0 = OS 1.3, 1 = OS 3.x) - despite the name, used by whichever compiler (VBCC, GCC or G++) is currently selected
    QString p_selected_compiler;                // holds value of compiler to use for recent compilation
    QString p_lastMakeExecutable;                // full path AmigaED last tried to run "make" from - used by compilerError() for a clear message
    QString p_selected_compiler_args;           // holds value for default compiler args to use (constructed in method 'startCompiler()' )
    QString p_vbcc_config_dir;                  // Path to vbcc config dir, unused ATM
    QString p_make;                             // Path to make, unused ATM
    bool p_lastRunWasProjectBuild = false;      // set before a project Build/Clean run, so stopCommand() reports it differently than a single-file compile
    bool p_projectModified = false;              // true when currentProject has changes not yet written to its .aep - see saveCurrentProject()/markProjectModified()
    QString p_expectedProjectBuildTarget;        // full path to the executable a "Build Project" run should produce - set by actionBuildProject() only (empty for Clean), so stopCommand() can verify "make" actually produced it rather than trusting its exit code alone
    QString p_lastRunCompilerLabel;              // short display name ("gcc"/"g++"/"vbcc") of the compiler in use for the build/compile run currently in flight - captured at start (actionBuildProject()/actionCleanProject()/startCompiler()) so status bar messages stay correct even if the selection changes before the run finishes
    QString p_strip;                            // Path to strip, unused ATM
    QString p_compiledFile;                     // keep the currently compiled file for file checking
    QString p_compiledFileSuffix;               // keep filename suffix for compiled output
    QFileInfo p_stripped_name;                  // stripped filename for constructing exe file name and icon
    QString p_emulator;             // path to emulator to start, used as 'command'
    QString p_os13_config;          // path to OS 1.3 emulator config-file, used as 'argument'
    QString p_os30_config;          // path to OS 3.x emulator config-file, used as 'argument'
    int p_defaultEmulator;          // default emulator to start, setted by prefs > emulator tab combobox
    QString p_emulator_to_start;    // Argument for default OS to start in UAE, depends on p_defaultEmulator
    QString p_projectsRootDir;      // Path to default folder to store projects in (use that path as a hd mount in UAE in order to test compiled app!)
    QStringList p_Compilers = {"VBCC - C", "GNU - C", "GNU - C++"};    // used for building compiler preselection combobox entries
    QStringList p_targetOS = {"OS 1.3", "OS 3.x"};
    int p_defaultCompiler;          // set from prefs file
    QString p_default_style;        // set from prefs file
    // Guards the "apply live" branch in readSettings() - false during the
    // very first call (constructor's first statement, before tabWidget or
    // any tab exists; the constructor applies the initial style itself
    // right afterwards), true for every later call (Prefs closed /
    // Shift+F12). Deliberately NOT gated on a widget pointer such as
    // tabWidget - unlike recentFilesMenue/guiLanguageEnglishAct, tabWidget
    // itself has no "= nullptr" initializer and is still uninitialized
    // memory at this point (see the "crash on startup" fix above for why
    // that distinction matters).
    bool p_styleInitialized = false;
    bool p_show_indentation;        // show indentation guidelines by default?
    bool p_mydebug = false;         // show or hide debugging informations
    bool p_no_lcd_statusbar;        // use normal text instead of LCD for cursor position view
    bool p_no_compilerbuttons;      // hide compiler selector and compile button from statusbar
    bool p_simple_statusbar;        // show basic statusbar only
    bool p_create_icon;             // create icon for compiled program?
    bool p_console_on_fail;         // open console only if compilation fails
    bool p_no_warn_requesters;      // reduce messegebox show-up
    bool p_show_gcc_opts;           // show options requester every time compilation with gcc/g++ is triggered?
    bool p_show_vbcc_opts;          // show options requester every time compilation with vbcc is triggered?
    QString p_guiLanguage = "en";   // GUI language: "en" (default/source language) or "de"

    // Setter for prefs vars
    void setCompilerGCC(QString compiler);


public slots:
    // Custom context menue:
    void showCustomContextMenue(const QPoint &pos); // implements custom context menu for QScintilla
    // methods for launching a compiler
    void error(QProcess::ProcessError error);
    void compilerError(QProcess::ProcessError error);   // reports a failed compiler start (wrong path in Prefs)
    void stateChanged(QProcess::ProcessState state);

    void readyReadStandardError();
    void readyReadStandardOutput();
    void started();
    void emu_started();                                 // emulator-specific "process actually started" handling - kept separate from started() so a compiler run no longer disables the emulator toolbar buttons (see actionEmulator())
    void emu_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void checkEmulatorStillRunning();                   // periodic fallback net for emulatorPollTimer - see constructor
    void emu_readyReadStandardOutput();
    void compiler_readyReadStandardOutput();
    void SelectCompiler(int index);
    void debugVars();
    void runCommand(QString command, QStringList arguments);
    void readCommand();
    int stopCommand(int exitCode, QProcess::ExitStatus exitStatus);
    void actionKillEmulator();
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void readPosSettings();
    void readSettings();                                // read app settings
    void jumpCompilerWarnings();                        // jump to error or warning, load file of occurance if not opened (unfinisched yet!)
    void setDefaultTargetOS(int default_os);            // change the currently active compiler's (VBCC, GCC or G++) target OS at runtime, driven by the status bar's target-OS gadget
    void on_output_cursorPositionChanged();             // react on user click in order to jump to error/warning in editor window
    bool checkVBCC(QString str_to_search);              // RegEX VBCC messages : jump to line x - returns true if the line actually matched a diagnostic
    bool checkGCC(QString str_to_search);               // RegEX GCC/G++ messages : jump to line x - returns true if the line actually matched a diagnostic
    void jumpToError(int error_line, int error_column); // set cursor to error line and column, switching file if needed
    void highlightOutputDiagnostics();                  // colours every error/warning line in the output pane (theme-aware) - called after new compiler output is appended
    QString resolveDebugFilePath(const QString &fileName); // resolve a compiler-reported filename to a loadable path

    // --- Project management (AmigaED v3.3) --------------------------
    void actionNewProjectEmptyC();
    void actionNewProjectShell();
    void actionNewProjectAmigaOS13();
    void actionNewProjectAmigaOS3x();
    void actionNewProjectReAction();
    void actionNewProjectMUI();
    void actionImportExistingProject();
    void actionLoadProject();
    void loadProjectFile(const QString &fileName);   // shared implementation behind actionLoadProject()/openRecentProject()
    void openRecentProject();
    void actionBuildProject();
    void actionCleanProject();
    void onProjectTreeDoubleClicked(QTreeWidgetItem *item, int column);
    void onFunctionsTreeDoubleClicked(QTreeWidgetItem *item, int column);
    void onProjectTreeContextMenu(const QPoint &pos);
    void actionAddFileToProject();
    void actionRemoveFileFromProject();
    void actionSaveProject();
    void actionCloseProject();     // closes the current project's tabs (with save prompts) and returns to the "no project loaded" state
    void actionSetAsMainFile();

    // --- Tabbed editor (AmigaED v3.2) -------------------------------
    QsciScintilla *newEditorTab();                          // create+configure+activate a new empty "untitled.c" tab
    void applyLexerForFileExtension(const QString &fileName);   // auto-select a syntax lexer by suffix/filename after opening a file - see openFileInTab()
    QsciScintilla *findEditorForFile(const QString &fileName) const; // find an already-open tab for this file, if any
    void openFileInTab(const QString &fileName);             // switch to an already-open tab, or open a new one, for this file
    void updateWindowTitle();                                // refresh window title from curFile (no modified-state/file-path changes)
    bool maybeSaveAll();                                      // maybeSave() for every open tab - used before quitting

private slots:
    void onTabChanged(int index);                    // active tab switched - update textEdit/curFile/window title
    void onTabCloseRequested(int index);              // a tab's [x] was clicked - check for unsaved changes, then close it
    void call_do_search_and_replace();
    void clearMarkers();                            // clear marked occourances in case of new search
    int startCompiler();                            // starts a process (f.e. Compiler)
    void popNotImplemented();                       // shows "not implemented" MessageBox
    void newFile();                                 // sets editor into new file mode
    void open();                                    // loads a file
    void openRecentFile();                          // loads a file from the "Recent files" submenu
    bool save();                                    // saves current file
    bool saveAs();                                  // saves current file as...
    void about();                                   // pops up "about" MessageBox
    void actionShowManual();                        // opens (or raises) the non-modal Manual viewer window, in the current GUI language
    void startPrefs();                              // Workaround to start prefsDialog with a parameter
    void setEmulatorMenu();                         // disable emulator menu entries if no config was specified
    void actionResetFontSize();                      // zoomTo(0) wrapper, since QAction::triggered() has no args
    void actionUndo();                                // forwards to the active tab's textEdit->undo()
    void actionRedo();                                // forwards to the active tab's textEdit->redo()
    void actionCut();                                 // forwards to the active tab's textEdit->cut()
    void actionCopy();                                // forwards to the active tab's textEdit->copy()
    void actionPaste();                               // forwards to the active tab's textEdit->paste()
    void actionZoomIn();                              // forwards to the active tab's textEdit->zoomIn()
    void actionZoomOut();                             // forwards to the active tab's textEdit->zoomOut()

    // GUI creation...
    void initializeGUI();
    void initializeLexerCPP();
    void initializeLexerMakefile();
    void initializeLexerPascal();
    void initializeLexerBatch();
    void initializeLexerInstaller();
    void initializeLexerAmigaGuide();
    void initializeLexerM68kAsm();
    void initializeLexerNone(QsciScintilla *editor = nullptr, bool announceChange = true);   // 'editor' defaults to the active tab; 'announceChange' set to false by reapplyEditorTheme() to suppress the status/debug messages when just recoloring an already-plain-text tab
    void initializeFolding();
    void initializeMargin(QsciScintilla *editor = nullptr);      // 'editor' defaults to the active tab - see reapplyEditorTheme()
    void initializeCaretLine(QsciScintilla *editor = nullptr);   // 'editor' defaults to the active tab - see reapplyEditorTheme()
    void initializeFont();
    // misc. methods for GUI manipulation
    void showCurrendCursorPosition();       // shows current cursor position in statusbar
    void documentWasModified();             // marks document as modified if text was changed
    void fitMarginLines();                  // adjusts margin sizes to fit line numbers
    // Menu actions...
    void actionPrefsDialog(int tabindex);   // open Preferences at given TAB
    void printFile();                   // print current file
    void actionGotoTop();               // jump to line #1
    void actionGotoBottom();            // jump to last line in text
    void actionGoto_Line();             // jump to line X
    void actionGoto_matching_brace();   // jumps to matching brace
    int actionCompile();                // calls compilation of current file
    // Emulator
    bool actionEmulator();              // starts default UAE
    bool isEmulatorProcessRunningExternally() const;   // true if an emulator process is already running on the system, whether or not AmigaED itself started it - see actionEmulator()
    void killExternalEmulatorProcess();                // terminates a matching emulator process AmigaED didn't itself start (see p_externalEmulatorTracked) - used by actionKillEmulator()
    void actionEmuOS13();               // sets UAE default to Workbench 1.3 and calls actionEmulator()
    void actionEmuOS30();               // sets UAE default to Workbench 3.x and calls actionEmulator()

    // editMenu
    void actionSearch();                  // opens a search dialog
    // viewMenue and submenue actions
    void actionShowLineNumbers();         // show or hide line numbers
    void actionShowCaretLine();           // show or hide caret line
    void actionSelectTheme();             // View/Theme entry clicked - applies the clicked action's own text as the new p_default_style
    void actionShowDebug();               // sets showing or hideing for debugging informations
    void actionShowEOL();                 // show or hide EOL character
    void actionShowUnprintable();         // show or hide unprintable characters
    void actionShowIndentationGuides();   // show or hide indentation guides
    // insertMenue and submenue actions:
    void actionInsertInclude();
    void actionInsertAmigaIncludes();
    void actionInsertDefine();
    void actionInsertIfdef();
    void actionInsertIfdefined();
    void actionInsertIfdefinedCompiler();
    void actionInsertIfndef();
    void actionInsertOpenLibrary();
    void actionInsertCloseLibrary();
    void actionInsertIf();
    void actionInsertIfElse();
    void actionInsertWhile();
    void actionInsertForLoop();
    void actionInsertDoWhile();
    void actionInsertSwitch();
    void actionInsertMain();
    void actionInsertEnum();
    void actionInsertConsoleDebugMessage();
    void actionInsertFunction();
    void actionInsertAmigaVersionString();
    void actionInsertFileheaderComment();
    void actionToggleCommentBlock();       // comments/uncomments the selected block (or current line) with "// "
    void actionInsertCSingleComment();
    void actionInsertCMultiComment();
    void actionInsertCppSingleComment();
    void actionInsertCLineDevideComment();
    void actionSelectCompilerVBCC();
    void actionSelectCompilerGCC();
    void actionSelectCompilerGPP();
    void actionToggleGccDefaultOptsDialog();
    void actionToggleVbccDefaultOptsDialog();
    void actionSetGuiLanguageEnglish();             // switch GUI language to English (source language, no translator)
    void actionSetGuiLanguageGerman();               // switch GUI language to German (installs amigaed_de.qm)
    void actionShowFunctionsBrowser();               // show the Functions panel
    void actionHideFunctionsBrowser();               // hide the Functions panel
    // Splitter
    void actionCloseOutputConsole();
    void actionShowOutputConsole();

    // search and replace connections:
    void doSearchAndReplace(SearchReplaceAction action);  // shared implementation behind the four search/replace buttons
    void on_btn_next();                         // search for next occurance of matching word
    void on_btn_previous();                 // search for previous occurance ofmatching word
    void on_btn_replace();                  // replace current occurance of matching word
    void on_btn_replace_all();              // replace all occurances of matching word
    void on_btn_hide();                     // hide search & replace, set p_search_allready_open to false
    void actionSearchReplaceFromContext();  // context-menu entry: pre-fills Find: with the word under the click, then opens/focuses the search panel

private:
    QProcess *cmd;
    QProcess proc;
    QProcess myProcess;                                                 // we need a QProcess to run a compiler...
    // Heap-allocated (not a plain value member) and deliberately given NO
    // QObject parent, precisely so that when the user chooses to leave
    // the emulator running while AmigaED exits (see closeEvent()), we can
    // simply not touch it at all: QProcess's own destructor kills the
    // child process if it's still running when destroyed (confirmed: this
    // produced a spurious "CrashExit - UAE has a problem!!" report right
    // as AmigaED closed, even after choosing "leave it open" - the
    // destructor's kill()+waitForFinished() synchronously triggers
    // finished() with CrashExit before returning), and a QObject parented
    // to MainWindow would be destroyed automatically right along with it
    // for the same reason. Left un-deleted at exit, on purpose, it simply
    // survives - the OS reclaims the (harmless, one-time, only-at-actual-
    // process-exit) memory regardless of whether our own destructor ran.
    QProcess *myEmulator = nullptr;

    // True once the user has been told a matching emulator process is
    // already running externally (see actionEmulator()'s "start another
    // instance anyway?" prompt) and chose not to start a second one -
    // Start/Stop then need to apply to THAT process instead of
    // myEmulator (which stays NotRunning/null in this case, since
    // AmigaED never actually launched anything itself). Reset back to
    // false once that process is confirmed gone, whether by
    // killExternalEmulatorProcess() or by checkEmulatorStillRunning()'s
    // periodic poll noticing it ended on its own.
    bool p_externalEmulatorTracked = false;

    // GUI creation...
    void createActions();                                               // defines actions for menues and toolbars
    void createMenus();                                                 // creates menues from actions
    void createToolBars();                                              // creates toolbars from actions
    void retranslateUi();                                               // re-applies all tr() strings after a runtime GUI-language change
    void applyGuiLanguage(const QString &langCode);                     // installs/removes the QTranslator for "en"/"de", saves the choice, calls retranslateUi()
    void createStatusBarMessage(QString statusmessage, int timeout);    // sets up the statusbar with a custom message
    // GUI methods...
    void SetLexerAtFileExtension(QString fileName);     // Helper to set approbiate Lexer according to a file's .ext
    void actionSelectCompiler(int index);               // Helper for selcting a compiler to use
    void writeSettings();                               // write app settings
    bool maybeSave(QsciScintilla *editor = nullptr);    // will be called if user quits while text has changed; defaults to the active tab
    int loadNonExistantFile(const QString &fileName);   // ask for creation if a file does NOT exist (used for command line loading)
    void loadFile(const QString &fileName);             // open an existing file
    void updateRecentFilesMenu();                       // rebuilds the "Recent files" submenu from p_recentFiles
    void addToRecentFiles(const QString &fileName);      // adds a file to the recent-files list (no duplicates, max MaxRecentFiles entries)
    void removeFromRecentFiles(const QString &fileName); // removes a (no longer existing) file from the recent-files list
    void updateRecentProjectsMenu();                      // rebuilds the "Recent Projects" submenu from p_recentProjects
    void addToRecentProjects(const QString &fileName);     // adds a .aep to the recent-projects list (no duplicates, max MaxRecentProjects entries)
    void removeFromRecentProjects(const QString &fileName); // removes a (no longer existing) project from the recent-projects list
    bool closeProjectTabs();                                // closes tabs belonging to the currently loaded project plus any untitled tabs, with per-tab save confirmation; false if the user cancelled
    bool saveFile(const QString &fileName);             // save current file
    void setCurrentFile(const QString &fileName);       // will be called to store current filename and put it into window title
    QString strippedName(const QString &fullFileName);  // gives back current filename without path
    void activateGUIdefaultSettings();

    // --- Application/editor "Dark" theme ---------------------------------
    // "Dark" is a synthetic entry in the "Default application style"
    // chooser (Prefs > Misc), not a real QStyleFactory key - it forces the
    // cross-platform "Fusion" style plus a dark QPalette (native styles
    // such as "windowsvista" mostly ignore custom palettes for their own
    // chrome), and additionally darkens the QScintilla editor itself
    // (paper/text colours per lexer, margins, caret line, selection, ...)
    // so the whole application looks consistent, not just the app chrome.
    bool isDarkTheme() const;                     // true if p_default_style == "Dark"
    bool isWorkbench13Theme() const;               // true if p_default_style == "Workbench 1.3"
    bool isWorkbench31Theme() const;               // true if p_default_style == "Workbench 3.1"
    void applyApplicationStyle();                 // applies p_default_style to QApplication (called from the constructor and, live, from readSettings() when the style changed)
    QPalette darkApplicationPalette() const;       // the dark QPalette used together with "Fusion" for "Dark"
    QPalette workbench13ApplicationPalette() const; // the QPalette used together with "Fusion" for "Workbench 1.3"
    QPalette workbench31ApplicationPalette() const; // the QPalette used together with "Fusion" for "Workbench 3.1"
    void applyLexerDarkColors(QsciLexer *lexer);   // recolors an already-created lexer's styles for the current theme - dark colours for "Dark", or a reset back to this app's own light-theme colours otherwise
    void reapplyEditorTheme();                     // re-applies margin/caret/selection/lexer colors to every currently open tab - used when the style changes at runtime (Prefs closed / Shift+F12)
    void buildThemeMenu();                         // populates View/Theme with one checkable, mutually-exclusive entry per available style (native styles + Dark/Workbench 1.3/Workbench 3.1) - called once from the constructor
    void syncThemeMenuCheckedState();              // ensures the entry matching p_default_style is checked - called after buildThemeMenu() and whenever the style changes elsewhere (e.g. Prefs dialog)


    // Qscintila Editor widget instance - since AmigaED v3.2, always points
    // at whichever tab is currently active (see tabWidget/onTabChanged()).
    QsciScintilla *textEdit;
    // Hosts one QsciScintilla per open file (AmigaED v3.2 tabbed editor).
    QTabWidget *tabWidget = nullptr;

    // --- Project management (AmigaED v3.3) -----------------------------
    Project *currentProject = nullptr;           // nullptr when no project is loaded
    QSplitter *mainSplitter;                     // horizontal: [projectPanel | splitter]
    QWidget *projectPanel;                        // tree + Add/Remove buttons
    QGroupBox *projectGroupBox = nullptr;         // "Project" - groups the tree and its buttons
    QTreeWidget *projectTree;
    QPushButton *addProjectFileBtn;
    QPushButton *removeProjectFileBtn;
    QTreeWidgetItem *projectSourceGroupItem;      // "C/C++ Files" category node
    QTreeWidgetItem *projectHeaderGroupItem;       // "Header Files" category node
    QTreeWidgetItem *projectInstallerGroupItem;    // "Installer Scripts" category node
    QTreeWidgetItem *projectAmigaGuideGroupItem;   // "AmigaGuide" category node
    QTreeWidgetItem *projectAssemblyGroupItem;     // "Assembler Sources" category node (.asm/.s)
    QTreeWidgetItem *projectExecutableGroupItem;   // "Executable" category node - see refreshProjectTree()
    QTreeWidgetItem *projectOtherGroupItem;        // "Other Files" category node (ProjectFileType::Other - anything not C/C++, header, or installer script)
    QTreeWidgetItem *projectMakefileGroupItem;     // "Makefiles" category node (auto-generated files, not part of Project::files)

    void createProjectPanel();                    // builds projectPanel/tree/buttons - called once from the constructor
    void refreshProjectTree();                     // rebuild tree items from currentProject
    void regenerateProjectMakefiles();              // (re)writes Makefile.gcc and Makefile.vbcc in the project directory
    QString resolveMakeExecutable() const;           // finds a usable "make" binary (Prefs, then next to the selected compiler, then bare "make")
    bool promptCompilerLinkerOptions(QString &compilerOpts, QString &linkerOpts, int templateKind); // asks before Makefiles are (re)created; pre-fills known-good defaults for certain template/compiler combinations
    void createNewProject(int templateKind);         // shared implementation for all "New Project" menu entries
    void importExistingProject();                     // shared implementation for "Import existing Project..." - scans a chosen folder, builds a Project from what it finds and saves it as a new .aep
    bool isImportSkippableFile(const QString &path) const;  // true for a file an import scan should leave out: .o, .lnk, or an executable
    void applyProjectTargetOSIfNeeded();               // switches the status bar's target-OS gadget to match the project's template ("OS 1.3" vs "OS 3.x") - applies to VBCC, GCC and G++ alike
    void getCompilerAndLinkerOptsForTarget(int compiler, int targetOS, QString &compilerOpts, QString &linkerOpts) const;   // central (compiler, target OS) -> (compiler opts, linker opts) lookup
    QString compilerDisplayLabel(int compiler) const;   // short status-bar-friendly compiler name: "gcc"/"g++"/"vbcc"
    QString dedupTokens(const QString &args) const;                          // removes duplicate whitespace-separated tokens, keeping the first occurrence of each
    void reloadEditorFromDiskIfOpen(const QString &fileName);               // refreshes an already-open tab's content from disk, if that file is open (used after auto-regenerating Makefiles)
    QString removeTokensAlsoIn(const QString &text, const QString &reference) const;  // drops any token from 'text' that also appears in 'reference'
    bool saveCurrentProject();                                             // persists currentProject to its .aep and clears the "unsaved changes" state
    void markProjectModified();                                            // flags currentProject as having unsaved changes (see p_projectModified)
    void addFilesToProject(const QStringList &filePaths);                  // shared add-file(s)-then-persist-and-refresh-everything logic, used by actionAddFileToProject() and the project tree's drag'n'drop handler
    QString ensureFileInProjectDir(const QString &sourcePath);              // copies a file into the project's own directory if it isn't already there, returning the path to actually track (empty if the user declined an overwrite or the copy failed)
    bool eventFilter(QObject *obj, QEvent *event) override;                // handles drag'n'drop of .c/.h/.cpp files and Makefiles onto the project tree
    bool isDragDropAcceptableProjectFile(const QString &path) const;       // true for .c/.h/.cpp (and common variants incl. .c++/.h++), .guide, .txt/.readme (or a bare "README"), .asm/.s, .pas, or a Makefile - see eventFilter()
    bool projectUsesFloatingPoint() const;   // heuristic scan for "float"/"double" in the project's own C/C++ sources - see regenerateProjectMakefiles()
    QString mainFileTemplateContent(int templateKind, const QString &baseName) const; // skeleton content for a new project's main file
    bool writeProgramIcon(const QString &executablePath, long stackSize) const;   // writes AmigaED's own built-in tool icon (see resources/amigaed_tool.info) to "<executablePath>.info", with do_StackSize patched to the given value
    void maybeOfferAddToProject(const QString &fileName); // called after a successful save - offers to add an untracked file

    // --- Functions panel (AmigaED v3.4) ---------------------------------
    QGroupBox *functionsGroupBox = nullptr;        // "Functions" - sits to the right of the editor
    QTreeWidget *functionsTree;                     // grouped by file, one child per detected function
    QTimer *functionsRefreshTimer;                   // periodic fallback refresh - see constructor
    QTimer *emulatorPollTimer;                       // periodic fallback check for the emulator toolbar buttons - see constructor

    void createFunctionsPanel();                     // builds functionsGroupBox/functionsTree - called once from the constructor
    void refreshFunctionsList();                      // rebuild functionsTree from currentProject's C/C++ files
    QList<QPair<QString, int>> scanFunctionsInFile(const QString &filePath) const; // heuristic (regex-based) function-definition scan; returns (name, 0-based line) pairs

    // Instances for Splitter
    QSplitter *splitter;
    QListView *lview;
    QPlainTextEdit *output = nullptr;
    QGroupBox *outputGroup;
    QGroupBox *searchGroup;
    QPushButton *btnCloseOutput;

    // stores name of the file currently in use
    QString curFile;

    // Editor font size (QScintilla zoom level), read from settings in the
    // constructor before textEdit exists yet - applied later in
    // initializeGUI() once the widget/lexer are set up.
    int p_zoomLevel = 0;

    // "Recent files" submenu: full paths of the most recently opened files,
    // oldest entry first, no duplicates, capped at MaxRecentFiles
    QStringList p_recentFiles;
    static const int MaxRecentFiles = 10;

    // "Recent Projects" submenu: full paths of the most recently used .aep
    // project files, oldest entry first, no duplicates, capped at MaxRecentProjects
    QStringList p_recentProjects;
    static const int MaxRecentProjects = 5;

    // Menues
    QMenu *fileMenue;           // holds file manipulating actions
    QMenu *recentFilesMenue = nullptr;    // Submenue of fileMenue, holds the most recently opened files ("Recent files")
    QMenu *recentProjectsMenue = nullptr; // Submenue of fileMenue, holds the most recently used projects ("Recent Projects")
    QMenu *editMenue;           // holds copy, paste and some more...
    QMenu *insertMenue;         // holds snippet insertions
    QMenu *buildMenue;          // holds compiler / build actions
    QMenu *navigationMenue;     // holds actions to move around in text
    QMenu *viewMenue;           // holds actions to change editors view
    QMenu *themeMenue = nullptr;         // View/Theme - one checkable entry per available style, mutually exclusive (see buildThemeMenu())
    QActionGroup *themeActionGroup = nullptr;   // enforces the mutual exclusion (radio-button behaviour) for themeMenue's entries
    QMenu *tabwidthMenue;       // Submenu of viewMenu, holds different values for tab width
    QMenu *syntaxMenue;         // holds actions to change syntax lexers
    QMenu *toolsMenue;          // holds misc actions
    QMenu *emulatorMenue;       // Submenue of toolsMenue, holds startups for different Amiga emulation models
    QMenu *helpMenue;           // holds help topics
    QMenu *preprocessorMenue;   // Submenue of insertMenue, holds preprocessor inserts
    QMenu *libraryMenue;        // Submenue of insertMenue, holds library inserts
    QMenu *conditionsMenue;     // Submenue of insertMenue, holds C condition inserts
    QMenu *loopsMenue;          // Submenue of insertMenue, holds C/++ loops inserts
    QMenu *commentsMenue;       // Submenue of insertMenue, holds comment inserts
    QMenu *compilerMenue;       // Submenue of buildMenue, holds entries for selecting a certain compiler
    QMenu *charMenue;           // Submenue of viewMenue, holds entries for EOL and unprintable characters

    // Toolbars
    QToolBar *fileToolBar;          // holds file manipulating actions
    QToolBar *editToolBar;          // holds copy, paste and some more...
    QToolBar *navigationToolBar;    // holds actions to move around in text
    QToolBar *buildToolBar;         // holds compiler / build actions
    QToolBar *toolsToolBar;         // holds misc actions
    QToolBar *searchToolBar;        // holds search/replace

    // synatxMenue/tabwithMenue mutual exclude ActionGroups
    QActionGroup *syntaxGroup;      // holds different Lexers for mutual exclusion in menue
    QActionGroup *tabwidthGroup;     // holds different values for tab with
    QActionGroup *compilerGroup;     // holds different values for compiler to use: 0 = vc, 1 = gcc, 2 = g++

    // GUI Language (I18n) - View menue, top entry
    QMenu *guiLanguageMenue = nullptr;
    QAction *guiLanguageEnglishAct = nullptr;
    QAction *guiLanguageGermanAct = nullptr;
    QActionGroup *guiLanguageGroup = nullptr;   // holds English/Deutsch for mutual exclusion in menue
    QTranslator *p_guiTranslator = nullptr;   // currently installed translator (nullptr while English/source language is active)

    // Manual viewer (Help > Manual, F1) - non-modal, single instance. nullptr
    // while closed; reset back to nullptr via its destroyed() signal once
    // the user closes it (see actionShowManual()).
    QDialog *p_manualWindow = nullptr;

    // Functions Browser visibility - View menue, right after GUI Language
    QAction *showFunctionsBrowserAct = nullptr;
    QAction *hideFunctionsBrowserAct = nullptr;
    QActionGroup *functionsBrowserGroup = nullptr;   // holds Show/Hide for mutual exclusion in menue

    //Actions for fileMenue
    QAction *newAct;                // create new empty window
    QAction *openAct;               // open file
    // --- Project management (AmigaED v3.3) ---
    QMenu *newProjectMenue;
    QAction *newProjectEmptyCAct;
    QAction *newProjectShellAct;
    QAction *newProjectAmigaOS13Act;
    QAction *newProjectAmigaOS3xAct;
    QAction *newProjectReActionAct;
    QAction *newProjectMUIAct;
    QAction *importExistingProjectAct;
    QAction *loadProjectAct;
    QAction *saveProjectAct;
    QAction *closeProjectAct;
    QAction *addFilesToProjectAct;    // "Add files to Project..." menu entry - same slot as the project panel's "Add..." button (actionAddFileToProject())
    QAction *buildProjectAct;
    QAction *cleanProjectAct;
    QAction *saveAct;               // save file
    QAction *saveAsAct;             // save file as...
    QAction *prefsAct;              // open prefs dialog
    QAction *prefsReloadAct;        // reload prefs
    QAction *printAct;              // print current file
    QAction *exitAct;               // quit the app
    // Actions for editMenue
    QAction *undoAct;               // undo the last edit
    QAction *redoAct;               // redo the last undone edit
    QAction *cutAct;                // copy marked text into clipboard and delete original
    QAction *copyAct;               // copy marked text into clipboard
    QAction *pasteAct;              // paste clipboard
    QAction *searchAct;             // search for text
    QAction *contextSearchReplaceAct;  // "Search and Replace..." - context menu only, topmost entry (see showCustomContextMenue())
    // Actions for helpMenue
    QAction *manualAct;             // opens the non-modal HTML Manual viewer (F1)
    QAction *aboutAct;              // show about message
    QAction *aboutQtAct;            // show about-Qt message
    // Actions for navigationMenue
    QAction *gotoTopAct;            // jump to line #1...
    QAction *gotoBottomAct;         // jump to line #1...
    QAction *gotoLineAct;           // jump to line X...    
    QAction *gotoMatchingBraceAct;  // jump to matching brace
    // Actions for viewMenue
    QAction *showLineNumbersAct;        // toggle visibility of Line numbers
    QAction *showCaretLineAct;          // toggle caret line
    QAction *toggleFoldAct;             // toggle text folding
    QAction *showIndentationGuidesAct;  // toggle visibility of indentation lines
    QAction *showDebugInfoAct;          // show or hide debugging informations
    QAction *zoomInAct;                 // increase editor font size
    QAction *zoomOutAct;                // decrease editor font size
    QAction *zoomResetAct;              // reset editor font size to normal
    QAction *showEOLAct;                // toggle visbility of EOL
    QAction *showUnprintableAct;        // toggle visibility of unprintable characters
    QAction *toggleAutoIndentAct;       // toggle automatic indentation
    QAction *toggleIndentUsesTabAct;    // use TAB or whitespace for indentation
    QAction *tabWith2Act;               // to be used in submenu tabwithMenue, sets tab with to 2
    QAction *tabWith4Act;               // to be used in submenu tabwithMenue, sets tab with to 4 (default)
    QAction *tabWith6Act;               // to be used in submenu tabwithMenue, sets tab with to 6
    QAction *tabWith8Act;               // to be used in submenu tabwithMenue, sets tab with to 8
    // Actions for buildMenue
    QAction *selectCompilerAct;          // select the compiler to use (vbcc, gcc, g++)
    QAction *selectCompilerVBCCAct;      // select the compiler to use (vbcc, gcc, g++)
    QAction *selectCompilerGCCAct;       // select the compiler to use (vbcc, gcc, g++)
    QAction *selectCompilerGPPAct;       // select the compiler to use (vbcc, gcc, g++)
    QAction *compileAct;                 // calls compilation of current file
    QAction *showOutputAct;              // pops up compiler output pane
    QAction *hideOutputAct;              // pops up compiler output pane
    QAction *toggleGccDefaultOptsAct;    // show or hide gcc/g++ default options dialog
    QAction *toggleVbccDefaultOptsAct;   // show or hide vc default options dialog
    // Actions for toolsMenue
    QAction *emulatorAct;             // start default UAE
    QAction *emulator13Act;           // start UAE with Workbench 1.3
    QAction *emulator30Act;           // start UAE with Workbench 3.x
    QAction *killEmulatorAct;         // attempt to kil a running Emulation
    // Actions for syntaxMenue
    QAction *lexCPPAct;             // switch lexer to C++ syntax
    QAction *lexBatchAct;           // switch lexer to Batch / Shell syntax
    QAction *lexMakefileAct;        // switch lexer to Makefile syntax
    QAction *lexInstallerAct;         // switch lexer to Amiga Installer (e.g. more like LISP) syntax
    QAction *lexAmigaGuideAct;        // switch lexer to AmigaGuide (.guide hypertext) syntax
    QAction *lexM68kAsmAct;           // switch lexer to m68k assembler syntax
    QAction *lexPascalAct;          // switch lexer to Pascal syntax
    QAction *lexPlainTextAct;       // switch lexer to no syntax highlighting
    // Actions for insertMenue
    // Preprocessor
    QAction *includeAct;            // inserts #include <file>
    QAction *amigaIncludesAct;      // inserts the most common Amiga #include files
    QAction *defineAct;             // inserts #define SOME_VALUE
    QAction *ifdefAct;              // inserts #ifdef ... #endif
    QAction *ifdefinedAct;          // inserts #if defined(SOMETHING) ... #elif defined(SOME_OTHER_THING) ... #endif
    QAction *ifdefinedCompilerAct;  // inserts compiler identification via macro
    QAction *ifndefAct;             // inserts #ifndef ... #endif
    // Library
    QAction *OpenLibraryAct;        // inserts OpenLibrary(some.library", 0L);
    QAction *CloseLibraryAct;       // inserts CloseLibrary(some.library);
    // Conditions
    QAction *ifAct;                 // inserts if(condition){..} statement
    QAction *if_elseAct;            // inserts if(condition){...} else {...} statement
    // Loops
    QAction *whileAct;              // inserts while(condition) {...} loop
    QAction *forAct;                // inserts while(condition) {...}do loop
    QAction *do_whileAct;           // inserts do{...}while(condition) loop
    QAction *switchAct;             // inserts switch(condition) select case statements
    // Comments
    QAction *toggleCommentBlockAct; // comments/uncomments the selected block (or current line) with "// "
    QAction *fileheaderAct;         // inserts a fileheader comment
    QAction *c_singleAct;           // inserts a C-style single line comment
    QAction *c_multiAct;            // inserts a C-style multi line comment
    QAction *cpp_singleAct;         // inserts a C++-style single line comment
    QAction *lineDevideCommentAct;  // inserts a C-style comment line devider
    // main(), Function, Enum, Amiga version string...
    QAction *mainAct;
    QAction *functionAct;            // inserts C function skeletton
    QAction *enumAct;                // inserts C enumeration skeletton
    QAction *consoleDebugAct;        // inserts if(myDebug){...} console debugging block
    QAction *versionStringAct;       // inserts Amiga C version string

    // statusbar widgets
    QLabel *statusLabelX;
    QLabel *statusLabelY;
    QLabel *compilerLabel;
    QLCDNumber  *statusLCD_X;       // shows cursor's line coordinate
    QLCDNumber  *statusLCD_Y;       // shows cursor's column coordinate
    QLabel *statusContainer_X;      // alternative view for cursor position
    QLabel *statusContainer_Y;      // alternative view for cursor position
    QComboBox *compilerCombo;       // puts a Compobox for compiler selection into statusbar
    QComboBox *osCombo;             // puts a Compobox for AmigaOS target selection into statusbar
    QPushButton *compilerButton;    // puts a dice button for compiler start into statusbar

    // Font in use
    QFont myfont;

    // is search allready opened?
    bool p_search_is_open = false;
    // Word under the cursor at the moment the editor's context menu was
    // opened (see showCustomContextMenue()) - captured there (while
    // 'pos' still refers to that click) and read back by
    // actionSearchReplaceFromContext() once the user actually picks
    // "Search and Replace..." from it. Empty if there was no word there.
    QString p_contextMenuWordAtClick;

    // Tracks which state the LAST "Fold/Unfold all" invocation forced the
    // whole document into (see initializeFolding()) - alternates on each
    // click, independent of Scintilla's own per-line fold state, so it
    // reliably alternates fold/unfold even after the user manually folded
    // or expanded individual blocks by hand in between clicks.
    bool foldall = false;
    bool p_show_compilerbutton = true;  // enable or disable Button in statusbar via prefs

    // check if there is allready a main() function in a file
    bool p_main_set = false;
    bool p_versionstring_set = false;
    int p_proc_is_started = 0;
    int p_index = 0;

protected:
    void closeEvent(QCloseEvent *event);        // catch close() event
    //void mousePressEvent(QMouseEvent *event);   // catch mouse press event
};

#endif
