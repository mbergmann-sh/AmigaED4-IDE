/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "showCustomContextMenue",
        "",
        "QPoint",
        "pos",
        "error",
        "QProcess::ProcessError",
        "compilerError",
        "started",
        "emu_started",
        "emu_finished",
        "exitCode",
        "QProcess::ExitStatus",
        "exitStatus",
        "checkEmulatorStillRunning",
        "emu_readyReadStandardOutput",
        "compiler_readyReadStandardOutput",
        "SelectCompiler",
        "index",
        "debugVars",
        "runCommand",
        "command",
        "arguments",
        "readCommand",
        "stopCommand",
        "actionKillEmulator",
        "finished",
        "migrateLegacySettingsIfNeeded",
        "readPosSettings",
        "readSettings",
        "jumpCompilerWarnings",
        "setDefaultTargetOS",
        "default_os",
        "on_output_cursorPositionChanged",
        "showOutputContextMenu",
        "on_output_markAllAndCopy",
        "on_output_emptyConsole",
        "checkVBCC",
        "str_to_search",
        "checkGCC",
        "jumpToError",
        "error_line",
        "error_column",
        "highlightOutputDiagnostics",
        "resolveDebugFilePath",
        "fileName",
        "actionNewProjectEmptyC",
        "actionNewProjectShell",
        "actionNewProjectAmigaOS13",
        "actionNewProjectAmigaOS3x",
        "actionNewProjectReAction",
        "actionNewProjectMUI",
        "actionNewProjectAssembler",
        "actionImportExistingProject",
        "actionLoadProject",
        "loadProjectFile",
        "openRecentProject",
        "actionBuildProject",
        "actionCleanProject",
        "actionProjectOptions",
        "actionOpenShell",
        "closeAllOpenShells",
        "rebuildToolsMenu",
        "launchToolAction",
        "path",
        "params",
        "updateGuiBuilderProjectActions",
        "actionNewGuiBuilderProjectMUI",
        "actionNewGuiBuilderProjectGadTools",
        "actionNewGuiBuilderProjectReAction",
        "launchGuiBuilderForNewProject",
        "kind",
        "onGuiBuilderProcessFinished",
        "importGuiBuilderProject",
        "onProjectTreeDoubleClicked",
        "QTreeWidgetItem*",
        "item",
        "column",
        "onFunctionsTreeDoubleClicked",
        "onProjectTreeContextMenu",
        "actionAddFileToProject",
        "actionRemoveFileFromProject",
        "actionSaveProject",
        "actionCloseProject",
        "actionSetAsMainFile",
        "newEditorTab",
        "QsciScintilla*",
        "applyLexerForFileExtension",
        "findEditorForFile",
        "openFileInTab",
        "updateWindowTitle",
        "maybeSaveAll",
        "saveModifiedProjectFiles",
        "onTabChanged",
        "onTabCloseRequested",
        "onApplicationStateChanged",
        "Qt::ApplicationState",
        "state",
        "checkForExternallyModifiedFiles",
        "call_do_search_and_replace",
        "clearMarkers",
        "startCompiler",
        "newFile",
        "open",
        "openRecentFile",
        "save",
        "saveAs",
        "about",
        "actionShowManual",
        "actionShowAutodocReader",
        "actionJumpToExplanation",
        "actionLookupCppRef",
        "actionLookupAsmRef",
        "startPrefs",
        "setEmulatorMenu",
        "actionResetFontSize",
        "actionUndo",
        "actionRedo",
        "actionCut",
        "actionCopy",
        "actionPaste",
        "reindentPastedLines",
        "editor",
        "fromLine",
        "toLine",
        "actionZoomIn",
        "actionZoomOut",
        "initializeGUI",
        "initializeLexerCPP",
        "initializeLexerMakefile",
        "initializeLexerPascal",
        "initializeLexerBatch",
        "initializeLexerInstaller",
        "initializeLexerAmigaGuide",
        "initializeLexerM68kAsm",
        "initializeLexerNone",
        "announceChange",
        "syncSyntaxMenuToCurrentLexer",
        "initializeFolding",
        "initializeMargin",
        "initializeCaretLine",
        "initializeFont",
        "showCurrendCursorPosition",
        "documentWasModified",
        "fitMarginLines",
        "actionPrefsDialog",
        "tabindex",
        "printFile",
        "actionGotoTop",
        "actionGotoBottom",
        "actionGoto_Line",
        "actionGoto_matching_brace",
        "onBraceBlockCursorMoved",
        "applyBraceBlockHighlight",
        "bracePos",
        "matchPos",
        "clearBraceBlockHighlight",
        "actionCompile",
        "actionEmulator",
        "forcedTarget",
        "isEmulatorProcessRunningExternally",
        "killExternalEmulatorProcess",
        "actionEmuOS13",
        "actionEmuOS30",
        "actionSearch",
        "actionShowLineNumbers",
        "actionShowCaretLine",
        "actionSelectTheme",
        "actionSelectIndentation",
        "actionShowDebug",
        "actionShowEOL",
        "actionShowUnprintable",
        "actionShowIndentationGuides",
        "actionInsertInclude",
        "actionInsertAmigaIncludes",
        "actionInsertDefine",
        "actionInsertIfdef",
        "actionInsertIfdefined",
        "actionInsertIfdefinedCompiler",
        "actionInsertIfndef",
        "actionInsertOpenLibrary",
        "actionInsertCloseLibrary",
        "actionInsertIf",
        "actionInsertIfElse",
        "actionInsertWhile",
        "actionInsertForLoop",
        "actionInsertDoWhile",
        "actionInsertSwitch",
        "actionInsertMain",
        "actionInsertEnum",
        "actionInsertConsoleDebugMessage",
        "actionInsertFunction",
        "actionInsertAmigaVersionString",
        "actionInsertFileheaderComment",
        "actionToggleCommentBlock",
        "actionInsertCSingleComment",
        "actionInsertCMultiComment",
        "actionInsertCppSingleComment",
        "actionInsertCLineDevideComment",
        "actionSelectCompilerVBCC",
        "actionSelectCompilerGCC",
        "actionSelectCompilerGPP",
        "actionSelectCompilerVasm",
        "actionSelectCompilerGnuAs",
        "actionToggleGccDefaultOptsDialog",
        "actionToggleVbccDefaultOptsDialog",
        "actionSetGuiLanguageEnglish",
        "actionSetGuiLanguageGerman",
        "actionShowFunctionsBrowser",
        "actionHideFunctionsBrowser",
        "actionCloseOutputConsole",
        "actionShowOutputConsole",
        "doSearchAndReplace",
        "SearchReplaceAction",
        "action",
        "on_btn_next",
        "on_btn_previous",
        "on_btn_replace",
        "on_btn_replace_all",
        "on_btn_hide",
        "actionSearchReplaceFromContext"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'showCustomContextMenue'
        QtMocHelpers::SlotData<void(const QPoint &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'error'
        QtMocHelpers::SlotData<void(QProcess::ProcessError)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 5 },
        }}),
        // Slot 'compilerError'
        QtMocHelpers::SlotData<void(QProcess::ProcessError)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 5 },
        }}),
        // Slot 'started'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'emu_started'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'emu_finished'
        QtMocHelpers::SlotData<void(int, QProcess::ExitStatus)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { 0x80000000 | 12, 13 },
        }}),
        // Slot 'checkEmulatorStillRunning'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'emu_readyReadStandardOutput'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'compiler_readyReadStandardOutput'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'SelectCompiler'
        QtMocHelpers::SlotData<void(int)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'debugVars'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'runCommand'
        QtMocHelpers::SlotData<void(QString, QStringList)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 21 }, { QMetaType::QStringList, 22 },
        }}),
        // Slot 'readCommand'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'stopCommand'
        QtMocHelpers::SlotData<int(int, QProcess::ExitStatus)>(24, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 11 }, { 0x80000000 | 12, 13 },
        }}),
        // Slot 'actionKillEmulator'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'finished'
        QtMocHelpers::SlotData<void(int, QProcess::ExitStatus)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { 0x80000000 | 12, 13 },
        }}),
        // Slot 'migrateLegacySettingsIfNeeded'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'readPosSettings'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'readSettings'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'jumpCompilerWarnings'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setDefaultTargetOS'
        QtMocHelpers::SlotData<void(int)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 32 },
        }}),
        // Slot 'on_output_cursorPositionChanged'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'showOutputContextMenu'
        QtMocHelpers::SlotData<void(const QPoint &)>(34, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'on_output_markAllAndCopy'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'on_output_emptyConsole'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'checkVBCC'
        QtMocHelpers::SlotData<bool(QString)>(37, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 38 },
        }}),
        // Slot 'checkGCC'
        QtMocHelpers::SlotData<bool(QString)>(39, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 38 },
        }}),
        // Slot 'jumpToError'
        QtMocHelpers::SlotData<void(int, int)>(40, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 41 }, { QMetaType::Int, 42 },
        }}),
        // Slot 'highlightOutputDiagnostics'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'resolveDebugFilePath'
        QtMocHelpers::SlotData<QString(const QString &)>(44, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 45 },
        }}),
        // Slot 'actionNewProjectEmptyC'
        QtMocHelpers::SlotData<void()>(46, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionNewProjectShell'
        QtMocHelpers::SlotData<void()>(47, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionNewProjectAmigaOS13'
        QtMocHelpers::SlotData<void()>(48, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionNewProjectAmigaOS3x'
        QtMocHelpers::SlotData<void()>(49, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionNewProjectReAction'
        QtMocHelpers::SlotData<void()>(50, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionNewProjectMUI'
        QtMocHelpers::SlotData<void()>(51, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionNewProjectAssembler'
        QtMocHelpers::SlotData<void()>(52, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionImportExistingProject'
        QtMocHelpers::SlotData<void()>(53, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionLoadProject'
        QtMocHelpers::SlotData<void()>(54, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'loadProjectFile'
        QtMocHelpers::SlotData<void(const QString &)>(55, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 45 },
        }}),
        // Slot 'openRecentProject'
        QtMocHelpers::SlotData<void()>(56, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionBuildProject'
        QtMocHelpers::SlotData<void()>(57, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionCleanProject'
        QtMocHelpers::SlotData<void()>(58, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionProjectOptions'
        QtMocHelpers::SlotData<void()>(59, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionOpenShell'
        QtMocHelpers::SlotData<void()>(60, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'closeAllOpenShells'
        QtMocHelpers::SlotData<void()>(61, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'rebuildToolsMenu'
        QtMocHelpers::SlotData<void()>(62, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'launchToolAction'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(63, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 64 }, { QMetaType::QString, 65 },
        }}),
        // Slot 'updateGuiBuilderProjectActions'
        QtMocHelpers::SlotData<void()>(66, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionNewGuiBuilderProjectMUI'
        QtMocHelpers::SlotData<void()>(67, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionNewGuiBuilderProjectGadTools'
        QtMocHelpers::SlotData<void()>(68, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionNewGuiBuilderProjectReAction'
        QtMocHelpers::SlotData<void()>(69, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'launchGuiBuilderForNewProject'
        QtMocHelpers::SlotData<void(int)>(70, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 71 },
        }}),
        // Slot 'onGuiBuilderProcessFinished'
        QtMocHelpers::SlotData<void(int, QProcess::ExitStatus)>(72, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { 0x80000000 | 12, 13 },
        }}),
        // Slot 'importGuiBuilderProject'
        QtMocHelpers::SlotData<bool()>(73, 2, QMC::AccessPublic, QMetaType::Bool),
        // Slot 'onProjectTreeDoubleClicked'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *, int)>(74, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 75, 76 }, { QMetaType::Int, 77 },
        }}),
        // Slot 'onFunctionsTreeDoubleClicked'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *, int)>(78, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 75, 76 }, { QMetaType::Int, 77 },
        }}),
        // Slot 'onProjectTreeContextMenu'
        QtMocHelpers::SlotData<void(const QPoint &)>(79, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'actionAddFileToProject'
        QtMocHelpers::SlotData<void()>(80, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionRemoveFileFromProject'
        QtMocHelpers::SlotData<void()>(81, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionSaveProject'
        QtMocHelpers::SlotData<void()>(82, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionCloseProject'
        QtMocHelpers::SlotData<void()>(83, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'actionSetAsMainFile'
        QtMocHelpers::SlotData<void()>(84, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'newEditorTab'
        QtMocHelpers::SlotData<QsciScintilla *()>(85, 2, QMC::AccessPublic, 0x80000000 | 86),
        // Slot 'applyLexerForFileExtension'
        QtMocHelpers::SlotData<void(const QString &)>(87, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 45 },
        }}),
        // Slot 'findEditorForFile'
        QtMocHelpers::SlotData<QsciScintilla *(const QString &) const>(88, 2, QMC::AccessPublic, 0x80000000 | 86, {{
            { QMetaType::QString, 45 },
        }}),
        // Slot 'openFileInTab'
        QtMocHelpers::SlotData<void(const QString &)>(89, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 45 },
        }}),
        // Slot 'updateWindowTitle'
        QtMocHelpers::SlotData<void()>(90, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'maybeSaveAll'
        QtMocHelpers::SlotData<bool()>(91, 2, QMC::AccessPublic, QMetaType::Bool),
        // Slot 'saveModifiedProjectFiles'
        QtMocHelpers::SlotData<bool()>(92, 2, QMC::AccessPublic, QMetaType::Bool),
        // Slot 'onTabChanged'
        QtMocHelpers::SlotData<void(int)>(93, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'onTabCloseRequested'
        QtMocHelpers::SlotData<void(int)>(94, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'onApplicationStateChanged'
        QtMocHelpers::SlotData<void(Qt::ApplicationState)>(95, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 96, 97 },
        }}),
        // Slot 'checkForExternallyModifiedFiles'
        QtMocHelpers::SlotData<void()>(98, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'call_do_search_and_replace'
        QtMocHelpers::SlotData<void()>(99, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'clearMarkers'
        QtMocHelpers::SlotData<void()>(100, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'startCompiler'
        QtMocHelpers::SlotData<int()>(101, 2, QMC::AccessPrivate, QMetaType::Int),
        // Slot 'newFile'
        QtMocHelpers::SlotData<void()>(102, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'open'
        QtMocHelpers::SlotData<void()>(103, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'openRecentFile'
        QtMocHelpers::SlotData<void()>(104, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'save'
        QtMocHelpers::SlotData<bool()>(105, 2, QMC::AccessPrivate, QMetaType::Bool),
        // Slot 'saveAs'
        QtMocHelpers::SlotData<bool()>(106, 2, QMC::AccessPrivate, QMetaType::Bool),
        // Slot 'about'
        QtMocHelpers::SlotData<void()>(107, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionShowManual'
        QtMocHelpers::SlotData<void()>(108, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionShowAutodocReader'
        QtMocHelpers::SlotData<void()>(109, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionJumpToExplanation'
        QtMocHelpers::SlotData<void()>(110, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionLookupCppRef'
        QtMocHelpers::SlotData<void()>(111, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionLookupAsmRef'
        QtMocHelpers::SlotData<void()>(112, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'startPrefs'
        QtMocHelpers::SlotData<void()>(113, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setEmulatorMenu'
        QtMocHelpers::SlotData<void()>(114, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionResetFontSize'
        QtMocHelpers::SlotData<void()>(115, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionUndo'
        QtMocHelpers::SlotData<void()>(116, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionRedo'
        QtMocHelpers::SlotData<void()>(117, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionCut'
        QtMocHelpers::SlotData<void()>(118, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionCopy'
        QtMocHelpers::SlotData<void()>(119, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionPaste'
        QtMocHelpers::SlotData<void()>(120, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'reindentPastedLines'
        QtMocHelpers::SlotData<void(QsciScintilla *, int, int)>(121, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 86, 122 }, { QMetaType::Int, 123 }, { QMetaType::Int, 124 },
        }}),
        // Slot 'actionZoomIn'
        QtMocHelpers::SlotData<void()>(125, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionZoomOut'
        QtMocHelpers::SlotData<void()>(126, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'initializeGUI'
        QtMocHelpers::SlotData<void()>(127, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'initializeLexerCPP'
        QtMocHelpers::SlotData<void()>(128, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'initializeLexerMakefile'
        QtMocHelpers::SlotData<void()>(129, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'initializeLexerPascal'
        QtMocHelpers::SlotData<void()>(130, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'initializeLexerBatch'
        QtMocHelpers::SlotData<void()>(131, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'initializeLexerInstaller'
        QtMocHelpers::SlotData<void()>(132, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'initializeLexerAmigaGuide'
        QtMocHelpers::SlotData<void()>(133, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'initializeLexerM68kAsm'
        QtMocHelpers::SlotData<void()>(134, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'initializeLexerNone'
        QtMocHelpers::SlotData<void(QsciScintilla *, bool)>(135, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 86, 122 }, { QMetaType::Bool, 136 },
        }}),
        // Slot 'initializeLexerNone'
        QtMocHelpers::SlotData<void(QsciScintilla *)>(135, 2, QMC::AccessPrivate | QMC::MethodCloned, QMetaType::Void, {{
            { 0x80000000 | 86, 122 },
        }}),
        // Slot 'initializeLexerNone'
        QtMocHelpers::SlotData<void()>(135, 2, QMC::AccessPrivate | QMC::MethodCloned, QMetaType::Void),
        // Slot 'syncSyntaxMenuToCurrentLexer'
        QtMocHelpers::SlotData<void()>(137, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'initializeFolding'
        QtMocHelpers::SlotData<void()>(138, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'initializeMargin'
        QtMocHelpers::SlotData<void(QsciScintilla *)>(139, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 86, 122 },
        }}),
        // Slot 'initializeMargin'
        QtMocHelpers::SlotData<void()>(139, 2, QMC::AccessPrivate | QMC::MethodCloned, QMetaType::Void),
        // Slot 'initializeCaretLine'
        QtMocHelpers::SlotData<void(QsciScintilla *)>(140, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 86, 122 },
        }}),
        // Slot 'initializeCaretLine'
        QtMocHelpers::SlotData<void()>(140, 2, QMC::AccessPrivate | QMC::MethodCloned, QMetaType::Void),
        // Slot 'initializeFont'
        QtMocHelpers::SlotData<void()>(141, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showCurrendCursorPosition'
        QtMocHelpers::SlotData<void()>(142, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'documentWasModified'
        QtMocHelpers::SlotData<void()>(143, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'fitMarginLines'
        QtMocHelpers::SlotData<void()>(144, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionPrefsDialog'
        QtMocHelpers::SlotData<void(int)>(145, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 146 },
        }}),
        // Slot 'printFile'
        QtMocHelpers::SlotData<void()>(147, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionGotoTop'
        QtMocHelpers::SlotData<void()>(148, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionGotoBottom'
        QtMocHelpers::SlotData<void()>(149, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionGoto_Line'
        QtMocHelpers::SlotData<void()>(150, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionGoto_matching_brace'
        QtMocHelpers::SlotData<void()>(151, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBraceBlockCursorMoved'
        QtMocHelpers::SlotData<void()>(152, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'applyBraceBlockHighlight'
        QtMocHelpers::SlotData<void(QsciScintilla *, long, long)>(153, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 86, 122 }, { QMetaType::Long, 154 }, { QMetaType::Long, 155 },
        }}),
        // Slot 'clearBraceBlockHighlight'
        QtMocHelpers::SlotData<void()>(156, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionCompile'
        QtMocHelpers::SlotData<int()>(157, 2, QMC::AccessPrivate, QMetaType::Int),
        // Slot 'actionEmulator'
        QtMocHelpers::SlotData<bool(int)>(158, 2, QMC::AccessPrivate, QMetaType::Bool, {{
            { QMetaType::Int, 159 },
        }}),
        // Slot 'actionEmulator'
        QtMocHelpers::SlotData<bool()>(158, 2, QMC::AccessPrivate | QMC::MethodCloned, QMetaType::Bool),
        // Slot 'isEmulatorProcessRunningExternally'
        QtMocHelpers::SlotData<bool() const>(160, 2, QMC::AccessPrivate, QMetaType::Bool),
        // Slot 'killExternalEmulatorProcess'
        QtMocHelpers::SlotData<void()>(161, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionEmuOS13'
        QtMocHelpers::SlotData<void()>(162, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionEmuOS30'
        QtMocHelpers::SlotData<void()>(163, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionSearch'
        QtMocHelpers::SlotData<void()>(164, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionShowLineNumbers'
        QtMocHelpers::SlotData<void()>(165, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionShowCaretLine'
        QtMocHelpers::SlotData<void()>(166, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionSelectTheme'
        QtMocHelpers::SlotData<void()>(167, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionSelectIndentation'
        QtMocHelpers::SlotData<void()>(168, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionShowDebug'
        QtMocHelpers::SlotData<void()>(169, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionShowEOL'
        QtMocHelpers::SlotData<void()>(170, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionShowUnprintable'
        QtMocHelpers::SlotData<void()>(171, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionShowIndentationGuides'
        QtMocHelpers::SlotData<void()>(172, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertInclude'
        QtMocHelpers::SlotData<void()>(173, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertAmigaIncludes'
        QtMocHelpers::SlotData<void()>(174, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertDefine'
        QtMocHelpers::SlotData<void()>(175, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertIfdef'
        QtMocHelpers::SlotData<void()>(176, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertIfdefined'
        QtMocHelpers::SlotData<void()>(177, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertIfdefinedCompiler'
        QtMocHelpers::SlotData<void()>(178, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertIfndef'
        QtMocHelpers::SlotData<void()>(179, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertOpenLibrary'
        QtMocHelpers::SlotData<void()>(180, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertCloseLibrary'
        QtMocHelpers::SlotData<void()>(181, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertIf'
        QtMocHelpers::SlotData<void()>(182, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertIfElse'
        QtMocHelpers::SlotData<void()>(183, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertWhile'
        QtMocHelpers::SlotData<void()>(184, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertForLoop'
        QtMocHelpers::SlotData<void()>(185, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertDoWhile'
        QtMocHelpers::SlotData<void()>(186, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertSwitch'
        QtMocHelpers::SlotData<void()>(187, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertMain'
        QtMocHelpers::SlotData<void()>(188, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertEnum'
        QtMocHelpers::SlotData<void()>(189, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertConsoleDebugMessage'
        QtMocHelpers::SlotData<void()>(190, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertFunction'
        QtMocHelpers::SlotData<void()>(191, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertAmigaVersionString'
        QtMocHelpers::SlotData<void()>(192, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertFileheaderComment'
        QtMocHelpers::SlotData<void()>(193, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionToggleCommentBlock'
        QtMocHelpers::SlotData<void()>(194, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertCSingleComment'
        QtMocHelpers::SlotData<void()>(195, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertCMultiComment'
        QtMocHelpers::SlotData<void()>(196, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertCppSingleComment'
        QtMocHelpers::SlotData<void()>(197, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionInsertCLineDevideComment'
        QtMocHelpers::SlotData<void()>(198, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionSelectCompilerVBCC'
        QtMocHelpers::SlotData<void()>(199, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionSelectCompilerGCC'
        QtMocHelpers::SlotData<void()>(200, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionSelectCompilerGPP'
        QtMocHelpers::SlotData<void()>(201, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionSelectCompilerVasm'
        QtMocHelpers::SlotData<void()>(202, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionSelectCompilerGnuAs'
        QtMocHelpers::SlotData<void()>(203, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionToggleGccDefaultOptsDialog'
        QtMocHelpers::SlotData<void()>(204, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionToggleVbccDefaultOptsDialog'
        QtMocHelpers::SlotData<void()>(205, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionSetGuiLanguageEnglish'
        QtMocHelpers::SlotData<void()>(206, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionSetGuiLanguageGerman'
        QtMocHelpers::SlotData<void()>(207, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionShowFunctionsBrowser'
        QtMocHelpers::SlotData<void()>(208, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionHideFunctionsBrowser'
        QtMocHelpers::SlotData<void()>(209, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionCloseOutputConsole'
        QtMocHelpers::SlotData<void()>(210, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionShowOutputConsole'
        QtMocHelpers::SlotData<void()>(211, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'doSearchAndReplace'
        QtMocHelpers::SlotData<void(enum SearchReplaceAction)>(212, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 213, 214 },
        }}),
        // Slot 'on_btn_next'
        QtMocHelpers::SlotData<void()>(215, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btn_previous'
        QtMocHelpers::SlotData<void()>(216, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btn_replace'
        QtMocHelpers::SlotData<void()>(217, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btn_replace_all'
        QtMocHelpers::SlotData<void()>(218, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btn_hide'
        QtMocHelpers::SlotData<void()>(219, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actionSearchReplaceFromContext'
        QtMocHelpers::SlotData<void()>(220, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->showCustomContextMenue((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 1: _t->error((*reinterpret_cast<std::add_pointer_t<QProcess::ProcessError>>(_a[1]))); break;
        case 2: _t->compilerError((*reinterpret_cast<std::add_pointer_t<QProcess::ProcessError>>(_a[1]))); break;
        case 3: _t->started(); break;
        case 4: _t->emu_started(); break;
        case 5: _t->emu_finished((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        case 6: _t->checkEmulatorStillRunning(); break;
        case 7: _t->emu_readyReadStandardOutput(); break;
        case 8: _t->compiler_readyReadStandardOutput(); break;
        case 9: _t->SelectCompiler((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->debugVars(); break;
        case 11: _t->runCommand((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[2]))); break;
        case 12: _t->readCommand(); break;
        case 13: { int _r = _t->stopCommand((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QProcess::ExitStatus>>(_a[2])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->actionKillEmulator(); break;
        case 15: _t->finished((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        case 16: _t->migrateLegacySettingsIfNeeded(); break;
        case 17: _t->readPosSettings(); break;
        case 18: _t->readSettings(); break;
        case 19: _t->jumpCompilerWarnings(); break;
        case 20: _t->setDefaultTargetOS((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 21: _t->on_output_cursorPositionChanged(); break;
        case 22: _t->showOutputContextMenu((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 23: _t->on_output_markAllAndCopy(); break;
        case 24: _t->on_output_emptyConsole(); break;
        case 25: { bool _r = _t->checkVBCC((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 26: { bool _r = _t->checkGCC((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 27: _t->jumpToError((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 28: _t->highlightOutputDiagnostics(); break;
        case 29: { QString _r = _t->resolveDebugFilePath((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 30: _t->actionNewProjectEmptyC(); break;
        case 31: _t->actionNewProjectShell(); break;
        case 32: _t->actionNewProjectAmigaOS13(); break;
        case 33: _t->actionNewProjectAmigaOS3x(); break;
        case 34: _t->actionNewProjectReAction(); break;
        case 35: _t->actionNewProjectMUI(); break;
        case 36: _t->actionNewProjectAssembler(); break;
        case 37: _t->actionImportExistingProject(); break;
        case 38: _t->actionLoadProject(); break;
        case 39: _t->loadProjectFile((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 40: _t->openRecentProject(); break;
        case 41: _t->actionBuildProject(); break;
        case 42: _t->actionCleanProject(); break;
        case 43: _t->actionProjectOptions(); break;
        case 44: _t->actionOpenShell(); break;
        case 45: _t->closeAllOpenShells(); break;
        case 46: _t->rebuildToolsMenu(); break;
        case 47: _t->launchToolAction((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 48: _t->updateGuiBuilderProjectActions(); break;
        case 49: _t->actionNewGuiBuilderProjectMUI(); break;
        case 50: _t->actionNewGuiBuilderProjectGadTools(); break;
        case 51: _t->actionNewGuiBuilderProjectReAction(); break;
        case 52: _t->launchGuiBuilderForNewProject((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 53: _t->onGuiBuilderProcessFinished((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        case 54: { bool _r = _t->importGuiBuilderProject();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 55: _t->onProjectTreeDoubleClicked((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 56: _t->onFunctionsTreeDoubleClicked((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 57: _t->onProjectTreeContextMenu((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 58: _t->actionAddFileToProject(); break;
        case 59: _t->actionRemoveFileFromProject(); break;
        case 60: _t->actionSaveProject(); break;
        case 61: _t->actionCloseProject(); break;
        case 62: _t->actionSetAsMainFile(); break;
        case 63: { QsciScintilla* _r = _t->newEditorTab();
            if (_a[0]) *reinterpret_cast<QsciScintilla**>(_a[0]) = std::move(_r); }  break;
        case 64: _t->applyLexerForFileExtension((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 65: { QsciScintilla* _r = _t->findEditorForFile((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QsciScintilla**>(_a[0]) = std::move(_r); }  break;
        case 66: _t->openFileInTab((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 67: _t->updateWindowTitle(); break;
        case 68: { bool _r = _t->maybeSaveAll();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 69: { bool _r = _t->saveModifiedProjectFiles();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 70: _t->onTabChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 71: _t->onTabCloseRequested((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 72: _t->onApplicationStateChanged((*reinterpret_cast<std::add_pointer_t<Qt::ApplicationState>>(_a[1]))); break;
        case 73: _t->checkForExternallyModifiedFiles(); break;
        case 74: _t->call_do_search_and_replace(); break;
        case 75: _t->clearMarkers(); break;
        case 76: { int _r = _t->startCompiler();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 77: _t->newFile(); break;
        case 78: _t->open(); break;
        case 79: _t->openRecentFile(); break;
        case 80: { bool _r = _t->save();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 81: { bool _r = _t->saveAs();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 82: _t->about(); break;
        case 83: _t->actionShowManual(); break;
        case 84: _t->actionShowAutodocReader(); break;
        case 85: _t->actionJumpToExplanation(); break;
        case 86: _t->actionLookupCppRef(); break;
        case 87: _t->actionLookupAsmRef(); break;
        case 88: _t->startPrefs(); break;
        case 89: _t->setEmulatorMenu(); break;
        case 90: _t->actionResetFontSize(); break;
        case 91: _t->actionUndo(); break;
        case 92: _t->actionRedo(); break;
        case 93: _t->actionCut(); break;
        case 94: _t->actionCopy(); break;
        case 95: _t->actionPaste(); break;
        case 96: _t->reindentPastedLines((*reinterpret_cast<std::add_pointer_t<QsciScintilla*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 97: _t->actionZoomIn(); break;
        case 98: _t->actionZoomOut(); break;
        case 99: _t->initializeGUI(); break;
        case 100: _t->initializeLexerCPP(); break;
        case 101: _t->initializeLexerMakefile(); break;
        case 102: _t->initializeLexerPascal(); break;
        case 103: _t->initializeLexerBatch(); break;
        case 104: _t->initializeLexerInstaller(); break;
        case 105: _t->initializeLexerAmigaGuide(); break;
        case 106: _t->initializeLexerM68kAsm(); break;
        case 107: _t->initializeLexerNone((*reinterpret_cast<std::add_pointer_t<QsciScintilla*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 108: _t->initializeLexerNone((*reinterpret_cast<std::add_pointer_t<QsciScintilla*>>(_a[1]))); break;
        case 109: _t->initializeLexerNone(); break;
        case 110: _t->syncSyntaxMenuToCurrentLexer(); break;
        case 111: _t->initializeFolding(); break;
        case 112: _t->initializeMargin((*reinterpret_cast<std::add_pointer_t<QsciScintilla*>>(_a[1]))); break;
        case 113: _t->initializeMargin(); break;
        case 114: _t->initializeCaretLine((*reinterpret_cast<std::add_pointer_t<QsciScintilla*>>(_a[1]))); break;
        case 115: _t->initializeCaretLine(); break;
        case 116: _t->initializeFont(); break;
        case 117: _t->showCurrendCursorPosition(); break;
        case 118: _t->documentWasModified(); break;
        case 119: _t->fitMarginLines(); break;
        case 120: _t->actionPrefsDialog((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 121: _t->printFile(); break;
        case 122: _t->actionGotoTop(); break;
        case 123: _t->actionGotoBottom(); break;
        case 124: _t->actionGoto_Line(); break;
        case 125: _t->actionGoto_matching_brace(); break;
        case 126: _t->onBraceBlockCursorMoved(); break;
        case 127: _t->applyBraceBlockHighlight((*reinterpret_cast<std::add_pointer_t<QsciScintilla*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<long>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<long>>(_a[3]))); break;
        case 128: _t->clearBraceBlockHighlight(); break;
        case 129: { int _r = _t->actionCompile();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 130: { bool _r = _t->actionEmulator((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 131: { bool _r = _t->actionEmulator();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 132: { bool _r = _t->isEmulatorProcessRunningExternally();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 133: _t->killExternalEmulatorProcess(); break;
        case 134: _t->actionEmuOS13(); break;
        case 135: _t->actionEmuOS30(); break;
        case 136: _t->actionSearch(); break;
        case 137: _t->actionShowLineNumbers(); break;
        case 138: _t->actionShowCaretLine(); break;
        case 139: _t->actionSelectTheme(); break;
        case 140: _t->actionSelectIndentation(); break;
        case 141: _t->actionShowDebug(); break;
        case 142: _t->actionShowEOL(); break;
        case 143: _t->actionShowUnprintable(); break;
        case 144: _t->actionShowIndentationGuides(); break;
        case 145: _t->actionInsertInclude(); break;
        case 146: _t->actionInsertAmigaIncludes(); break;
        case 147: _t->actionInsertDefine(); break;
        case 148: _t->actionInsertIfdef(); break;
        case 149: _t->actionInsertIfdefined(); break;
        case 150: _t->actionInsertIfdefinedCompiler(); break;
        case 151: _t->actionInsertIfndef(); break;
        case 152: _t->actionInsertOpenLibrary(); break;
        case 153: _t->actionInsertCloseLibrary(); break;
        case 154: _t->actionInsertIf(); break;
        case 155: _t->actionInsertIfElse(); break;
        case 156: _t->actionInsertWhile(); break;
        case 157: _t->actionInsertForLoop(); break;
        case 158: _t->actionInsertDoWhile(); break;
        case 159: _t->actionInsertSwitch(); break;
        case 160: _t->actionInsertMain(); break;
        case 161: _t->actionInsertEnum(); break;
        case 162: _t->actionInsertConsoleDebugMessage(); break;
        case 163: _t->actionInsertFunction(); break;
        case 164: _t->actionInsertAmigaVersionString(); break;
        case 165: _t->actionInsertFileheaderComment(); break;
        case 166: _t->actionToggleCommentBlock(); break;
        case 167: _t->actionInsertCSingleComment(); break;
        case 168: _t->actionInsertCMultiComment(); break;
        case 169: _t->actionInsertCppSingleComment(); break;
        case 170: _t->actionInsertCLineDevideComment(); break;
        case 171: _t->actionSelectCompilerVBCC(); break;
        case 172: _t->actionSelectCompilerGCC(); break;
        case 173: _t->actionSelectCompilerGPP(); break;
        case 174: _t->actionSelectCompilerVasm(); break;
        case 175: _t->actionSelectCompilerGnuAs(); break;
        case 176: _t->actionToggleGccDefaultOptsDialog(); break;
        case 177: _t->actionToggleVbccDefaultOptsDialog(); break;
        case 178: _t->actionSetGuiLanguageEnglish(); break;
        case 179: _t->actionSetGuiLanguageGerman(); break;
        case 180: _t->actionShowFunctionsBrowser(); break;
        case 181: _t->actionHideFunctionsBrowser(); break;
        case 182: _t->actionCloseOutputConsole(); break;
        case 183: _t->actionShowOutputConsole(); break;
        case 184: _t->doSearchAndReplace((*reinterpret_cast<std::add_pointer_t<enum SearchReplaceAction>>(_a[1]))); break;
        case 185: _t->on_btn_next(); break;
        case 186: _t->on_btn_previous(); break;
        case 187: _t->on_btn_replace(); break;
        case 188: _t->on_btn_replace_all(); break;
        case 189: _t->on_btn_hide(); break;
        case 190: _t->actionSearchReplaceFromContext(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 191)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 191;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 191)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 191;
    }
    return _id;
}
QT_WARNING_POP
