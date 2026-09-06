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

#include <QMainWindow>
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QFont>
#include <QFontInfo>
#include <QIcon>
#include <QWindow>
#include <QInputDialog>
#include <QTextLayout>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPoint>
#include <QSettings>
#include <QSet>
#include <QRegularExpression>
#include <QSize>
#include <QStatusBar>
#include <QTextStream>
#include <QToolBar>
#include <QSplitter>
#include <QListView>
#include <QStyleFactory>

#include <Qsci/qsciscintilla.h>
#include <Qsci/qsciscintillabase.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexermakefile.h>
#include <Qsci/qscilexerbatch.h>
#include <Qsci/qscilexerpascal.h>
#include <Qsci/qscilexerbash.h>
#include <Qsci/qsciprinter.h>

#include "amigalexercpp.h"
#include "amigainstallerlexer.h"
#include "amigaguidelexer.h"
#include "m68kasmlexer.h"

#include <search.h>

// allways get your defaults...
#include "mainwindow.h"
#include "version.h"
#include "prefsdialog.h"
#include "aboutdialog.h"

// Processes to start:
static QProcess myProcess;      // the process for using as spare for further extensions
static QProcess cmd;            // the process for running the compiler

// Open MainWindow with given filename...
MainWindow::MainWindow(QString cmdFileName)
{
    // load preferences
    // restores last saved position and size of the editor window, load other defaults
    readPosSettings();
    readSettings();
    debugVars();

    applyApplicationStyle();
    p_styleInitialized = true;   // from here on, readSettings() may re-apply the style live

    //QsciLexerBash *lexerbash = new QsciLexerBash;
    //lexer->setFoldComments(true);

    // Build a vertical Splitter
    QList<int> sizes;
    sizes << 320 <<150 << 200;
    splitter = new QSplitter(this);
    btnCloseOutput = new QPushButton(tr("Hide compiler output"), this);
    btnCloseOutput->setGeometry(50, 40, 75, 30);

    // Prepare Splitter components
    textEdit = nullptr;   // will point at whichever tab is currently active - see onTabChanged()
    lview = new QListView;
    outputGroup = new QGroupBox(tr("Compiler output"));
    searchGroup = new QGroupBox(tr("Search and replace"));
    searchGroup->setFixedHeight(120);   // avoid auto sizeing - don't let our serch & replace group grow to high...
    output = new QPlainTextEdit;
    output->setReadOnly(true);
    centerSearchForm = new QWidget;

    // Fill search form with life...
    if (centerSearchForm->objectName().isEmpty())
        centerSearchForm->setObjectName(QString::fromUtf8("centerSearchForm"));

    centerSearchForm->resize(573, 93);
    centerSearchForm->setMaximumSize(QSize(16777215, 16777215));
    gridLayout_2 = new QGridLayout(centerSearchForm);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    label_find = new QLabel(centerSearchForm);
    label_find->setObjectName(QString::fromUtf8("label_find"));

    gridLayout_2->addWidget(label_find, 0, 0, 1, 1);

    lineEdit_find = new QLineEdit(centerSearchForm);
    lineEdit_find->setObjectName(QString::fromUtf8("lineEdit_find"));
    lineEdit_find->setClearButtonEnabled(true);

    gridLayout_2->addWidget(lineEdit_find, 0, 1, 1, 2);

    gridLayout = new QGridLayout();
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    btn_previous = new QPushButton(centerSearchForm);
    btn_previous->setObjectName(QString::fromUtf8("btn_previous"));
    btn_previous->setMaximumSize(QSize(18, 18));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/btn_prev.png"), QSize(), QIcon::Normal, QIcon::Off);
    btn_previous->setIcon(icon);

    gridLayout->addWidget(btn_previous, 0, 0, 1, 1);

    btn_next = new QPushButton(centerSearchForm);
    btn_next->setObjectName(QString::fromUtf8("btn_next"));
    btn_next->setMaximumSize(QSize(18, 18));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/images/btn_next.png"), QSize(), QIcon::Normal, QIcon::Off);
    btn_next->setIcon(icon1);

    gridLayout->addWidget(btn_next, 0, 1, 1, 1);


    gridLayout_2->addLayout(gridLayout, 0, 3, 1, 1);

    label_replace = new QLabel(centerSearchForm);
    label_replace->setObjectName(QString::fromUtf8("label_replace"));

    gridLayout_2->addWidget(label_replace, 1, 0, 1, 1);

    lineEdit_replace = new QLineEdit(centerSearchForm);
    lineEdit_replace->setObjectName(QString::fromUtf8("lineEdit_replace"));
    lineEdit_replace->setFrame(true);
    lineEdit_replace->setClearButtonEnabled(true);

    gridLayout_2->addWidget(lineEdit_replace, 1, 1, 1, 2);

    formLayout_2 = new QFormLayout();
    formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
    btn_replace = new QPushButton(centerSearchForm);
    btn_replace->setObjectName(QString::fromUtf8("btn_replace"));
    btn_replace->setMaximumSize(QSize(18, 18));
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/images/find-and-replace.png"), QSize(), QIcon::Normal, QIcon::Off);
    btn_replace->setIcon(icon2);

    formLayout_2->setWidget(0, QFormLayout::LabelRole, btn_replace);

    btn_replace_all = new QPushButton(centerSearchForm);
    btn_replace_all->setObjectName(QString::fromUtf8("btn_replace_all"));
    btn_replace_all->setMaximumSize(QSize(18, 18));
    QIcon icon3;
    icon3.addFile(QString::fromUtf8(":/images/ico_out.png"), QSize(), QIcon::Normal, QIcon::Off);
    btn_replace_all->setIcon(icon3);

    formLayout_2->setWidget(0, QFormLayout::FieldRole, btn_replace_all);


    gridLayout_2->addLayout(formLayout_2, 1, 3, 1, 1);

    btn_hide = new QPushButton(centerSearchForm);
    btn_hide->setObjectName(QString::fromUtf8("btn_hide"));
    btn_hide->setMaximumSize(QSize(18, 18));
    QIcon icon4;
    icon4.addFile(QString::fromUtf8(":/images/fileexit.png"), QSize(), QIcon::Normal, QIcon::Off);
    btn_hide->setIcon(icon4);

    gridLayout_2->addWidget(btn_hide, 1, 4, 1, 1);

    gridLayout = new QGridLayout();
    gridLayout->setSpacing(6);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    checkBox_CaseSensitive = new QCheckBox(centerSearchForm);
    checkBox_CaseSensitive->setObjectName(QString::fromUtf8("checkBox_CaseSensitive"));

    gridLayout->addWidget(checkBox_CaseSensitive, 0, 0, 1, 1);

    checkBox_WholeWords = new QCheckBox(centerSearchForm);
    checkBox_WholeWords->setObjectName(QString::fromUtf8("checkBox_WholeWords"));

    gridLayout->addWidget(checkBox_WholeWords, 0, 1, 1, 1);

    checkBox_SearchForwards = new QCheckBox(centerSearchForm);
    checkBox_SearchForwards->setObjectName(QString::fromUtf8("checkBox_SearchForwards"));
    checkBox_SearchForwards->setChecked(true);

    gridLayout->addWidget(checkBox_SearchForwards, 0, 2, 1, 1);

    gridLayout_2->addLayout(gridLayout, 2, 1, 1, 1);

    horizontalSpacer_2 = new QSpacerItem(204, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout_2->addItem(horizontalSpacer_2, 2, 2, 1, 1);

#ifndef QT_NO_SHORTCUT
    label_find->setBuddy(lineEdit_find);
    label_replace->setBuddy(lineEdit_replace);
#endif // QT_NO_SHORTCUT

    centerSearchForm->setWindowTitle(QString());
    label_find->setText(QApplication::translate("centerSearchForm", "Find:", nullptr));
#ifndef QT_NO_TOOLTIP
    btn_previous->setToolTip(QApplication::translate("centerSearchForm", "goto previous occourance", nullptr));
#endif // QT_NO_TOOLTIP
    btn_previous->setText(QString());
#ifndef QT_NO_TOOLTIP
    btn_next->setToolTip(QApplication::translate("centerSearchForm", "goto next occourance", nullptr));
#endif // QT_NO_TOOLTIP
    btn_next->setText(QString());
    label_replace->setText(QApplication::translate("centerSearchForm", "Replace:", nullptr));
#ifndef QT_NO_TOOLTIP
    btn_replace->setToolTip(QApplication::translate("centerSearchForm", "Replace current occurance", nullptr));
#endif // QT_NO_TOOLTIP
    btn_replace->setText(QString());
#ifndef QT_NO_TOOLTIP
    btn_replace_all->setToolTip(QApplication::translate("centerSearchForm", "Replace ALL occurances", nullptr));
#endif // QT_NO_TOOLTIP
    btn_replace_all->setText(QString());
#ifndef QT_NO_TOOLTIP
    btn_hide->setToolTip(QApplication::translate("centerSearchForm", "Hide Search and Replace...", nullptr));
#endif // QT_NO_TOOLTIP
    btn_hide->setText(QString());
    checkBox_CaseSensitive->setText(QApplication::translate("centerSearchForm", "Case sensitive search", nullptr));
    checkBox_WholeWords->setText(QApplication::translate("centerSearchForm", "Whole words", nullptr));
    checkBox_SearchForwards->setText(QApplication::translate("MainWindow", "Search forwards", nullptr));



    QVBoxLayout *vsearch = new QVBoxLayout;
    vsearch->addWidget(centerSearchForm);
    searchGroup->setLayout(gridLayout_2);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(output);
    vbox->addWidget(btnCloseOutput);
    outputGroup->setLayout(vbox);

    splitter->setOrientation(Qt::Vertical);
    splitter->setHandleWidth(4);
    // The single, permanent QsciScintilla widget has become one tab
    // widget per open file (AmigaED v3.2). 'tabWidget' hosts them all;
    // 'textEdit' keeps pointing at whichever tab is currently active (see
    // onTabChanged()), so the hundreds of existing 'textEdit->...' call
    // sites throughout this file keep working unchanged.
    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    tabWidget->setMovable(true);
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));
    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));

    splitter->insertWidget(0,tabWidget);
    splitter->insertWidget(1,outputGroup);
    splitter->insertWidget(1,searchGroup);
    splitter->setSizes(sizes);

    outputGroup->hide();
    searchGroup->hide();

    // Project panel (tree view + Add/Remove buttons) sits to the left of
    // the existing tabs/output/search splitter, in its own horizontally
    // resizable pane (AmigaED v3.3).
    createProjectPanel();
    // Functions panel sits to the right of the editor, listing every
    // function AmigaED can find in the project's C/C++ files (v3.4).
    createFunctionsPanel();

    mainSplitter = new QSplitter(this);
    mainSplitter->setOrientation(Qt::Horizontal);
    mainSplitter->setHandleWidth(4);
    mainSplitter->insertWidget(0, projectPanel);
    mainSplitter->insertWidget(1, splitter);
    mainSplitter->insertWidget(2, functionsGroupBox);
    QList<int> mainSizes;
    mainSizes << 220 << 700 << 220;
    mainSplitter->setSizes(mainSizes);
    mainSplitter->setStretchFactor(0, 0);
    mainSplitter->setStretchFactor(1, 1);
    mainSplitter->setStretchFactor(2, 0);

    setCentralWidget(mainSplitter);

    // Refresh the function list automatically every 10 seconds, as a
    // robustness net alongside the event-driven refreshes triggered
    // directly at the points where project files actually change (project
    // load/create, add/remove file, successful save - see
    // refreshFunctionsList() call sites) - those cover the common cases
    // immediately, this catches anything else (e.g. a file edited outside
    // AmigaED while it's open).
    functionsRefreshTimer = new QTimer(this);
    connect(functionsRefreshTimer, &QTimer::timeout, this, &MainWindow::refreshFunctionsList);
    functionsRefreshTimer->start(10000);

    // Robustness net alongside actionEmulator()'s QProcess::started()/
    // finished() signal handling (emu_started()/emu_finished()) - those
    // cover the normal start/stop cycle immediately, but some ways an
    // emulator process can end (varies by platform/emulator build -
    // e.g. its own window being closed rather than a clean process exit
    // QProcess notices) don't reliably deliver a finished() signal at
    // all. This periodically re-syncs the toolbar/menu state with
    // reality instead - a cheap no-op whenever they already agree.
    emulatorPollTimer = new QTimer(this);
    connect(emulatorPollTimer, &QTimer::timeout, this, &MainWindow::checkEmulatorStillRunning);
    emulatorPollTimer->start(2000);

    initializeGUI();    // most initializations are done within tis method!

    // Create the first (and, at startup, only) editor tab. This must
    // happen AFTER initializeGUI() (which creates cutAct/copyAct/... and
    // the status bar widgets that showCurrendCursorPosition() needs) and
    // BEFORE activateGUIdefaultSettings() below, since that calls
    // actionShowIndentationGuides(), which dereferences 'textEdit'.
    newEditorTab();

    activateGUIdefaultSettings();

    setEmulatorMenu();  // disable emulator menu entries if no config was set

    // disable Emulator kill menu entry by default
    killEmulatorAct->setDisabled(true);

    // If an emulator process is already running (e.g. left open on
    // purpose from a previous AmigaED session - see closeEvent()'s "do
    // you want to leave it open?" prompt, or started outside AmigaED
    // entirely), reflect that in the toolbar/menu right away rather than
    // only discovering it once the user clicks Start and gets asked
    // there (see actionEmulator()'s own, interactive version of this
    // same check). No dialog here, unlike that one - this only ever
    // adjusts the UI to match reality, it never launches anything.
    if (isEmulatorProcessRunningExternally())
    {
        p_externalEmulatorTracked = true;
        emulatorAct->setDisabled(true);
        emulatorMenue->setDisabled(true);
        killEmulatorAct->setEnabled(true);
    }

    // react on buttons of searchGroup:
    connect(btn_hide, SIGNAL(clicked(bool)), this, SLOT(on_btn_hide()));
    connect(btn_next, SIGNAL(clicked(bool)), this, SLOT(on_btn_next()));
    connect(btn_previous, SIGNAL(clicked(bool)), this, SLOT(on_btn_previous()));
    connect(btn_replace, SIGNAL(clicked(bool)), this, SLOT(on_btn_replace()));
    connect(btn_replace_all, SIGNAL(clicked(bool)), this, SLOT(on_btn_replace_all()));
    connect(lineEdit_find, SIGNAL(returnPressed()), this, SLOT(call_do_search_and_replace()));
    connect(lineEdit_find, SIGNAL(textChanged(const QString &)), this, SLOT(clearMarkers())); // Reset marked text items

    // NOTE: the "react if document was modified" / "notify if cursor
    // position changed" connects that used to live here now happen once
    // per tab, inside newEditorTab() - there's no single fixed textEdit
    // left to connect at this point in the constructor.
    connect(output, SIGNAL(cursorPositionChanged()), this, SLOT(on_output_cursorPositionChanged()));


    cmd = new QProcess(this);
    cmd->setProcessChannelMode(QProcess::MergedChannels);

    ////////////////////////////////////////////////////////////////
    // Process handling for output to QTextBrowser or QTextEdit ///
    //////////////////////////////////////////////////////////////
    // process has some data to read
    connect(cmd, SIGNAL (readyRead()), this, SLOT (readCommand()));
    //process finished
    //connect(cmd, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT (stopCommand(int, QProcess::ExitStatus)));
    QObject::connect(cmd, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &MainWindow::stopCommand);
    // without this, a failed compiler start (e.g. wrong GCC/G++/VBCC path
    // in Prefs) fails completely silently: "Compiler run has been started"
    // stays in the status bar forever, with no further feedback at all
    connect(cmd, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(compilerError(QProcess::ProcessError)));

    ////////////////////////////////////////////
    /// command line handling for file input //
    //////////////////////////////////////////
    // (the first tab already starts out as an empty, "untitled.c" buffer -
    // see newEditorTab()/setCurrentFile("") above - so there's nothing
    // further to reset here before loading an optional command-line file.)

    // Load a file if specified on command line...
    if(!(cmdFileName.isEmpty()))
    {
        // Let's use our method for checking if a file allready
        // exists and create a new one if wanted...
        int response = loadNonExistantFile(cmdFileName);

        if(p_mydebug)
        {
            qDebug() << "loadNonExistantFile() returned " << response;
            // return values:
            //  0 - new file created
            // -1 - file creation failed
            //  1 - existing file loaded
            // 10 - quit app due to failed file creation
        }

        if (response == 10)
        {
            // if file creation went wrong
            // and MessageBox was answered 'Cancel':
            exit(-1);   // kill the App!
        }


    }
    else
    {
        if(p_mydebug)
        {
            qDebug() << "AmigaED started with empty file.";
        }

    }
}

//
// catch close() event and react
// on changes in current text document
//
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSaveAll())
    {
        // Only ask about (and possibly touch) the emulator once the app is
        // actually committed to closing - asking earlier, or unconditionally
        // killing it beforehand (the previous behaviour here), meant a
        // close the user then cancelled (e.g. via "Cancel" on an
        // unsaved-changes prompt in maybeSaveAll() above) could still have
        // needlessly killed a running emulator.
        if ((myEmulator && myEmulator->state() != QProcess::NotRunning) || p_externalEmulatorTracked)
        {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this, tr(AMIGAED_VERSION_STRING),
                tr("AmigaED is shutting down while your Amiga Emulator is up and "
                   "running. Do you want me to leave the Emulator open?"),
                QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

            if (reply == QMessageBox::No)
            {
                // Routes correctly either way: to the owned-QProcess
                // terminate()/kill() path, or (if p_externalEmulatorTracked)
                // to killExternalEmulatorProcess() instead - see
                // actionKillEmulator() itself.
                actionKillEmulator();
            }
            // else ("Yes", leave it open): deliberately do NOT touch
            // myEmulator at all here - not even disconnect its signals.
            // It's a heap-allocated QProcess with no QObject parent (see
            // its declaration in mainwindow.h) specifically so that
            // simply leaving it alone lets the emulator survive AmigaED's
            // own exit intact, rather than being killed by QProcess's own
            // destructor. (p_externalEmulatorTracked's process was never
            // AmigaED's own to begin with, so there's nothing to leave
            // alone there beyond simply not calling killExternalEmulatorProcess().)
        }

        writeSettings();
        event->accept();    // OK: Quit the app!
    }
    else
    {
        event->ignore();    // CANCEL: just stay where we are... ;)
    }
}


//
// create a new, empty file (always in its own new tab - never touches
// any other tab that may currently be open)
//
void MainWindow::newFile()
{
    newEditorTab();
    p_main_set = false;
    p_versionstring_set = false;
}

//
// open file from disk - opens in its own tab, or switches to it if it's
// already open in one (see openFileInTab()). No longer needs a maybeSave()
// guard: unlike the old single-buffer editor, opening a file can no longer
// overwrite unsaved work in another tab.
//
void MainWindow::open()
{
    QString startDir = curFile.isEmpty() ? p_projectsRootDir : curFile;
    if (startDir.isEmpty())
        startDir = QDir::currentPath();

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open source file"),
                                                    startDir,    // look up for files in PROGDIR first!
                                                    tr("C/C++ files (*.c *.cpp *.h *.hpp) ;; "
                                                    "ASM files (*.a *.asm *.s *.S *.m) ;; Makefiles (Make*.* *.mak) ;; "
                                                    "AmigaE files (*.e *.m) ;; Pascal files (*.p *.pas) ;; "
                                                    "Text files (*.txt *.md) ;; All files (*.*)"));

    if (!fileName.isEmpty())
        openFileInTab(fileName);

    p_main_set = false;
    p_versionstring_set = false;
}

//
// save file to disk
//
bool MainWindow::save()
{
    // if no file name has been given until now..
    if (curFile.isEmpty())
    {
        qDebug() << "curFile: " << curFile;
        return saveAs();        // ...call saveAs dialog!
    }
    else
    {
        return saveFile(curFile);
    }
}

//
// save file, let user choose a filename
//
bool MainWindow::saveAs()
{
    if(curFile.isEmpty())
        curFile = p_projectsRootDir;

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save source file"),
                                                    curFile,    // save files into last used folder
                                                    tr("C/C++ files (*.c *.cpp *.h *.hpp) ;; "
                                                    "ASM files (*.a *.asm *.s *.S *.m) ;; Makefiles (Make*.* *.mak) ;; "
                                                    "AmigaE files (*.e *.m) ;; Pascal files (*.p *.pas) ;; "
                                                    "Text files (*.txt *.md) ;; All files (*.*)"));

    if (fileName.isEmpty())
        return false;

    if(p_mydebug)
    {
        qDebug() << " File to save: " << fileName;
    }

    return saveFile(fileName);
}

//
// show information about this app...
//
void MainWindow::about()
{
    aboutDialog *aceAbout = new aboutDialog(this);
    aceAbout->exec();
}

//
// Open (or, if already open, simply raise) the non-modal Manual viewer
// window. Shows the HTML manual matching the current GUI language
// (p_guiLanguage) - embedded as Qt resources (see application.qrc), so
// this works regardless of where AmigaED was installed from.
//
// Deliberately non-modal (QDialog::show(), never exec()): the user must
// be able to keep working in the editor with the manual open alongside
// it. p_manualWindow enforces the "only once" requirement - a second F1
// press (or Help > Manual click) while it's already open just raises
// the existing window instead of creating another one; its destroyed()
// signal resets the pointer back to nullptr once the user closes it, so
// the next call knows to create a fresh instance.
//
void MainWindow::actionShowManual()
{
    if (!p_manualWindow)
    {
        p_manualWindow = new QDialog(this);
        p_manualWindow->setAttribute(Qt::WA_DeleteOnClose);
        p_manualWindow->setWindowTitle(tr("AmigaED Manual"));
        p_manualWindow->setWindowFlag(Qt::WindowMinMaxButtonsHint, true);
        p_manualWindow->resize(920, 720);

        // QTextBrowser (back again as of rev.100 - see Revisions.md for
        // the QWebEngineView detour in rev.99 and why it had to be
        // reverted: Qt WebEngine/Chromium simply does not compile with
        // MinGW on Windows, which is what this project's own Qt kit
        // uses - not a missing Maintenance Tool component, a hard
        // upstream Chromium limitation). help/manual_en.html and
        // manual_de.html were reworked instead to render cleanly within
        // QTextBrowser's own (QTextDocument) HTML/CSS subset - notably,
        // avoid the "line-height" CSS property anywhere in that
        // stylesheet: combined with this specific page's non-uniform
        // body padding, it made this engine drastically under-reserve
        // vertical space for any image that followed, so headings and
        // captions ended up overlapping the image above them instead of
        // sitting cleanly below it.
        QTextBrowser *browser = new QTextBrowser(p_manualWindow);
        browser->setOpenExternalLinks(true);   // http(s):// links (e.g. in "Recommendations") open in the system browser

        // The manual's HTML is authored with fixed light colours (like a
        // printed page), but AmigaED's "Dark" application style (Prefs >
        // Misc) works by installing a dark QApplication-wide QPalette -
        // which this new window would otherwise inherit, leaving anything
        // the HTML doesn't itself paint (scrollbar track, any margin
        // around the page) dark while the page content stays light.
        // Forcing a plain light palette here, independent of the app's
        // current theme, keeps the whole window readable either way; the
        // HTML's own explicit "background-color:#fff" on <html>/<body>
        // (see help/manual_en.html / manual_de.html) already covers the
        // page content itself, so this is belt-and-suspenders for the
        // chrome around it.
        QPalette lightPalette = browser->palette();
        lightPalette.setColor(QPalette::Base, Qt::white);
        lightPalette.setColor(QPalette::Text, Qt::black);
        lightPalette.setColor(QPalette::Window, Qt::white);
        lightPalette.setColor(QPalette::WindowText, Qt::black);
        browser->setPalette(lightPalette);
        browser->setAutoFillBackground(true);
        p_manualWindow->setPalette(lightPalette);
        p_manualWindow->setAutoFillBackground(true);

        const QString resourcePath = (p_guiLanguage == "de")
                ? QStringLiteral("qrc:/help/manual_de.html")
                : QStringLiteral("qrc:/help/manual_en.html");
        browser->setSource(QUrl(resourcePath));

        QVBoxLayout *layout = new QVBoxLayout(p_manualWindow);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(browser);
        p_manualWindow->setLayout(layout);

        // WA_DeleteOnClose means the QDialog object itself is destroyed
        // when closed - catch that here to null the pointer out, rather
        // than leaving it dangling until the next actionShowManual() call.
        connect(p_manualWindow, &QDialog::destroyed, this, [this]() {
            p_manualWindow = nullptr;
        });
    }

    p_manualWindow->show();
    p_manualWindow->raise();
    p_manualWindow->activateWindow();
}

//
// react on SIGNAL textChanged() if text was modified
//
// Since AmigaED v3.2, this is connected per-tab (see newEditorTab()), so
// it can fire for a tab that isn't the currently active one (e.g. a
// background tab modified programmatically). Only reflect the change in
// the window's [*] indicator if it actually came from the active tab -
// otherwise switching tabs later (onTabChanged()) already refreshes it.
//
void MainWindow::documentWasModified()
{
    QsciScintilla *edited = qobject_cast<QsciScintilla *>(sender());
    if (edited && edited != textEdit)
        return;

    setWindowModified(textEdit->isModified());  // put asterix (*) into filename!
}

//
// Helper to set approbiate Lexer according to a file's .ext
//
void MainWindow::SetLexerAtFileExtension(QString)
{
    if(p_mydebug)
    {
        qDebug() << "Lexer changed!";
    }
}

//
// you'll need some fucking actions first if you
// want to create menues!
//
void MainWindow::createActions()
{
    // Menue actions consist of:
    /* --- File ---------------------------------------------------*/
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);  // this is the action itsself, equipped with an image
    // and a shortcut (<alt> + <n>) for opening the menue

    newAct->setShortcut(tr("Ctrl+N"));                                  // this is the instant shortcut for calling the action
    // without opening the menue first

    newAct->setStatusTip(tr("Create a new file"));                      // Display a help message in app's status bar
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));        // connect the action to a Qt SIGNAL for calling
    // the approbiate class method to handle the request.
    // Here: MainWindow::newFile()
    // see "mainwindow.h" for details!

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    /* --- Project management (AmigaED v3.3) --- */
    newProjectEmptyCAct = new QAction(tr("Empty Amiga C Project"), this);
    connect(newProjectEmptyCAct, SIGNAL(triggered()), this, SLOT(actionNewProjectEmptyC()));

    newProjectShellAct = new QAction(tr("Shell Project"), this);
    connect(newProjectShellAct, SIGNAL(triggered()), this, SLOT(actionNewProjectShell()));

    newProjectAmigaOS13Act = new QAction(tr("AmigaOS 1.3 Project"), this);
    connect(newProjectAmigaOS13Act, SIGNAL(triggered()), this, SLOT(actionNewProjectAmigaOS13()));

    newProjectAmigaOS3xAct = new QAction(tr("AmigaOS 3.x Project"), this);
    connect(newProjectAmigaOS3xAct, SIGNAL(triggered()), this, SLOT(actionNewProjectAmigaOS3x()));

    newProjectReActionAct = new QAction(tr("ReAction Project"), this);
    connect(newProjectReActionAct, SIGNAL(triggered()), this, SLOT(actionNewProjectReAction()));

    newProjectMUIAct = new QAction(tr("MUI Project"), this);
    connect(newProjectMUIAct, SIGNAL(triggered()), this, SLOT(actionNewProjectMUI()));

    importExistingProjectAct = new QAction(tr("Import existing Project..."), this);
    importExistingProjectAct->setStatusTip(tr("Import an existing C/C++ project folder that AmigaED doesn't know yet"));
    connect(importExistingProjectAct, SIGNAL(triggered()), this, SLOT(actionImportExistingProject()));

    loadProjectAct = new QAction(tr("Load Project..."), this);
    loadProjectAct->setStatusTip(tr("Load an AmigaED project (.aep)"));
    connect(loadProjectAct, SIGNAL(triggered()), this, SLOT(actionLoadProject()));

    saveProjectAct = new QAction(tr("Save Project"), this);
    saveProjectAct->setStatusTip(tr("Save the current project's file list and settings to its .aep, if it has unsaved changes"));
    saveProjectAct->setEnabled(false);   // nothing unsaved yet - see markProjectModified()/saveCurrentProject()
    connect(saveProjectAct, SIGNAL(triggered()), this, SLOT(actionSaveProject()));

    closeProjectAct = new QAction(tr("Close Project"), this);
    closeProjectAct->setStatusTip(tr("Close the current project and all of its open tabs"));
    connect(closeProjectAct, SIGNAL(triggered()), this, SLOT(actionCloseProject()));

    addFilesToProjectAct = new QAction(tr("Add files to Project..."), this);
    addFilesToProjectAct->setStatusTip(tr("Add one or more existing files to the current project"));
    connect(addFilesToProjectAct, SIGNAL(triggered()), this, SLOT(actionAddFileToProject()));

    buildProjectAct = new QAction(QIcon(":/images/build_project.png"), tr("Build Project"), this);
    buildProjectAct->setShortcut(tr("Shift+F6"));
    buildProjectAct->setStatusTip(tr("Run the project's Makefile (target \"all\") for the currently selected compiler"));
    connect(buildProjectAct, SIGNAL(triggered()), this, SLOT(actionBuildProject()));

    cleanProjectAct = new QAction(QIcon(":/images/clean_project.png"), tr("Clean Project"), this);
    cleanProjectAct->setStatusTip(tr("Run the project's Makefile (target \"clean\") for the currently selected compiler"));
    connect(cleanProjectAct, SIGNAL(triggered()), this, SLOT(actionCleanProject()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(QIcon(":/images/filesaveas.png"),tr("Save &As..."), this);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    prefsAct = new QAction(QIcon(":/images/prefs.png"),tr("Global prefs..."), this);
    prefsAct->setShortcut(tr("F12"));
    prefsAct->setStatusTip(tr("Open global preferences..."));
    connect(prefsAct, SIGNAL(triggered()), this, SLOT(startPrefs()));

    prefsReloadAct = new QAction(tr("Reload settings"), this);
    prefsReloadAct->setShortcut(tr("Shift+F12"));
    prefsReloadAct->setStatusTip(tr("Reload global settings..."));
    connect(prefsReloadAct, SIGNAL(triggered()), this, SLOT(readSettings()));

    printAct = new QAction(QIcon(":/images/printer.png"),tr("&Print file..."), this);
    printAct->setShortcut(tr("Ctrl+p"));
    printAct->setStatusTip(tr("Prepare for printing..."));
    connect(printAct, SIGNAL(triggered()), this, SLOT(printFile()));

    exitAct = new QAction(QIcon(":/images/fileexit.png"), tr("&Exit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    /* --- Edit -----------------------------------------------------------------------*/
    undoAct = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
    undoAct->setShortcut(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last edit"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(actionUndo()));

    redoAct = new QAction(QIcon(":/images/redo.png"), tr("&Redo"), this);
    redoAct->setShortcut(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last undone edit"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(actionRedo()));

    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    // NOTE: these used to connect directly to the app's single, permanent
    // textEdit widget. Since AmigaED v3.2 there is one QsciScintilla per
    // open tab, so 'textEdit' now points at whichever tab is currently
    // active and can change at runtime - a fixed connect() made once here
    // would only ever reach the tab that happened to be active when
    // createActions() ran. Routing through small MainWindow slots (see
    // actionCut()/actionCopy()/actionPaste() below) means every triggered()
    // is dispatched to whatever 'textEdit' currently is.
    connect(cutAct, SIGNAL(triggered()), this, SLOT(actionCut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(actionCopy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(actionPaste()));

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    // The per-tab copyAvailable() -> cutAct/copyAct connections are made
    // once per editor instance in newEditorTab(), for the same reason as
    // above - there is no longer a single fixed textEdit to connect here.

    searchAct = new QAction(QIcon(":/images/search.png"), tr("Sea&rch..."), this);
    searchAct->setShortcut(tr("Ctrl+f"));
    searchAct->setStatusTip(tr("Search text in document"));
    connect(searchAct, SIGNAL(triggered()), this, SLOT(actionSearch()));

    // Context-menu-only entry (see showCustomContextMenue()); not part
    // of any menu-bar menu, unlike the "Inserts" section mirrored below
    // it - this is a shortcut into the same search-and-replace feature
    // as searchAct above, not an insert template.
    contextSearchReplaceAct = new QAction(QIcon(":/images/search.png"), tr("Search and Replace..."), this);
    contextSearchReplaceAct->setStatusTip(tr("Open Search and Replace, using the word under the click as the search term"));
    connect(contextSearchReplaceAct, SIGNAL(triggered()), this, SLOT(actionSearchReplaceFromContext()));

    /* --- Navigation -------------------------------------------------------------------*/
    gotoTopAct = new QAction( tr("&Goto top..."), this);
    gotoTopAct->setShortcut(tr("Ctrl+Home"));
    gotoTopAct->setStatusTip(tr("Goto top of file..."));
    connect(gotoTopAct, SIGNAL(triggered()), this, SLOT(actionGotoTop()));

    gotoBottomAct = new QAction( tr("&Goto bottom..."), this);
    gotoBottomAct->setShortcut(tr("Ctrl+End"));
    gotoBottomAct->setStatusTip(tr("Goto bottom of file..."));
    connect(gotoBottomAct, SIGNAL(triggered()), this, SLOT(actionGotoBottom()));

    gotoLineAct = new QAction(QIcon(":/images/gotoline.png"), tr("&Goto Line..."), this);
    gotoLineAct->setShortcut(tr("Ctrl+G"));
    gotoLineAct->setStatusTip(tr("Goto line X..."));
    connect(gotoLineAct, SIGNAL(triggered()), this, SLOT(actionGoto_Line()));

    gotoMatchingBraceAct = new QAction(QIcon(":/images/brackets.png"), tr("Goto &matching bracket {} ... [] ... ()..."), this);
    gotoMatchingBraceAct->setShortcut(tr("Ctrl+b"));
    gotoMatchingBraceAct->setStatusTip(tr("Goto matching bracket..."));
    connect(gotoMatchingBraceAct, SIGNAL(triggered()), this, SLOT(actionGoto_matching_brace()));

    /* --- View -----------------------------------------------------------------------*/
    toggleFoldAct = new QAction(tr("&Fold/Unfold all..."), this);
    toggleFoldAct->setShortcut(tr("Ctrl+Alt+f"));
    toggleFoldAct->setStatusTip(tr("Fold or unfold the whole document"));
    connect(toggleFoldAct, SIGNAL(triggered()), this, SLOT(initializeFolding()));

    showLineNumbersAct = new QAction(tr("Show line numbers..."), this);
    showLineNumbersAct->setCheckable(true);
    showLineNumbersAct->setChecked(true);
    showLineNumbersAct->setStatusTip(tr("Show or hide line numbers"));
    connect(showLineNumbersAct, SIGNAL(triggered()), this, SLOT(actionShowLineNumbers()));

    showCaretLineAct = new QAction(tr("Show caret line..."), this);
    showCaretLineAct->setShortcut(tr("Ctrl+#"));
    showCaretLineAct->setCheckable(true);
    showCaretLineAct->setChecked(false);
    showCaretLineAct->setStatusTip(tr("Show or hide caret line"));
    connect(showCaretLineAct, SIGNAL(triggered()), this, SLOT(actionShowCaretLine()));

    showDebugInfoAct = new QAction(tr("Show debug output"), this);
    showDebugInfoAct->setCheckable(true);
    showDebugInfoAct->setChecked(p_mydebug);
    showDebugInfoAct->setStatusTip(tr("Toggle debug output visibility"));
    connect(showDebugInfoAct, SIGNAL(triggered()), this, SLOT(actionShowDebug()));

    zoomInAct = new QAction(tr("Increase font size"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setStatusTip(tr("Increase the editor's font size"));
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(actionZoomIn()));

    zoomOutAct = new QAction(tr("Decrease font size"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setStatusTip(tr("Decrease the editor's font size"));
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(actionZoomOut()));

    zoomResetAct = new QAction(tr("Reset font size"), this);
    zoomResetAct->setShortcut(tr("Ctrl+0"));
    zoomResetAct->setStatusTip(tr("Reset the editor's font size to normal"));
    connect(zoomResetAct, SIGNAL(triggered()), this, SLOT(actionResetFontSize()));

    showEOLAct = new QAction(tr("Show EOL character"), this);
    showEOLAct->setCheckable(true);
    showEOLAct->setChecked(false);
    showEOLAct->setStatusTip(tr("Toggle EOL visibility"));
    connect(showEOLAct, SIGNAL(triggered()), this, SLOT(actionShowEOL()));

    showUnprintableAct = new QAction(tr("Show unprintable characters"), this);
    showUnprintableAct->setCheckable(true);
    showUnprintableAct->setChecked(false);
    showUnprintableAct->setStatusTip(tr("Toggle unprintable characters visibility"));
    connect(showUnprintableAct, SIGNAL(triggered()), this, SLOT(actionShowUnprintable()));

    showIndentationGuidesAct = new QAction(tr("Show indentation guides"), this);
    showIndentationGuidesAct->setCheckable(true);
    showIndentationGuidesAct->setChecked(p_show_indentation);
    showIndentationGuidesAct->setStatusTip(tr("Toggle indentation guides visibility"));
    connect(showIndentationGuidesAct, SIGNAL(triggered()), this, SLOT(actionShowIndentationGuides()));

    /* --- Build -----------------------------------------------------------------------*/
    selectCompilerVBCCAct = new QAction(QIcon(":/images/c-logo.png"), tr("VBCC vc (C mode only)..."), this);
    selectCompilerVBCCAct->setShortcut(tr("Shift+Ctrl+v"));
    selectCompilerVBCCAct->setStatusTip(tr("Set Compiler to VBCC (C mode only)..."));
    selectCompilerVBCCAct->setCheckable(true);
    selectCompilerVBCCAct->setChecked(false);
    connect(selectCompilerVBCCAct, SIGNAL(triggered()), this, SLOT(actionSelectCompilerVBCC()));

    selectCompilerGCCAct = new QAction(QIcon(":/images/c-logo.png"), tr("GNU gcc (C mode)..."), this);
    selectCompilerGCCAct->setShortcut(tr("Shift+Ctrl+g"));
    selectCompilerGCCAct->setStatusTip(tr("Set Compiler to GNU gcc (C mode)..."));
    selectCompilerGCCAct->setCheckable(true);
    selectCompilerGCCAct->setChecked(false);
    connect(selectCompilerGCCAct, SIGNAL(triggered()), this, SLOT(actionSelectCompilerGCC()));

    selectCompilerGPPAct = new QAction(QIcon(":/images/cpp-logo.png"), tr("GNU g++ (C++ mode)..."), this);
    selectCompilerGPPAct->setShortcut(tr("Shift+Ctrl+c"));
    selectCompilerGPPAct->setStatusTip(tr("Set Compiler to GNU g++ (C++ mode)..."));
    selectCompilerGPPAct->setCheckable(true);
    selectCompilerGPPAct->setChecked(false);
    connect(selectCompilerGPPAct, SIGNAL(triggered()), this, SLOT(actionSelectCompilerGPP()));

    toggleGccDefaultOptsAct = new QAction(tr("Show gcc/g++ options dialog..."), this);
    toggleGccDefaultOptsAct->setCheckable(true);
    toggleGccDefaultOptsAct->setChecked(p_show_gcc_opts);
    toggleGccDefaultOptsAct->setStatusTip(tr("Show or hide gcc/g++ options dialog"));
    connect(toggleGccDefaultOptsAct, SIGNAL(triggered()), this, SLOT(actionToggleGccDefaultOptsDialog()));

    toggleVbccDefaultOptsAct = new QAction(tr("Show vbcc options dialog..."), this);
    toggleVbccDefaultOptsAct->setCheckable(true);
    toggleVbccDefaultOptsAct->setChecked(p_show_vbcc_opts);
    toggleVbccDefaultOptsAct->setStatusTip(tr("Show or hide vbcc options dialog"));
    connect(toggleVbccDefaultOptsAct, SIGNAL(triggered()), this, SLOT(actionToggleVbccDefaultOptsDialog()));

    // this will put the compilers in our menue into a mutual exclusive
    // group for automatically checking/unchecking each other:
    compilerGroup = new QActionGroup(this);
    compilerGroup->addAction(selectCompilerVBCCAct);
    compilerGroup->addAction(selectCompilerGCCAct);
    compilerGroup->addAction(selectCompilerGPPAct);

    // GUI Language (I18n) - "English" is the source language (no QTranslator
    // needed), "Deutsch" installs amigaed_de.qm. p_guiLanguage was already
    // populated by readSettings() (called before createActions() from the
    // constructor), so the correct entry starts out checked.
    guiLanguageEnglishAct = new QAction(tr("English"), this);
    guiLanguageEnglishAct->setStatusTip(tr("Switch the GUI language to English"));
    guiLanguageEnglishAct->setCheckable(true);
    connect(guiLanguageEnglishAct, SIGNAL(triggered()), this, SLOT(actionSetGuiLanguageEnglish()));

    guiLanguageGermanAct = new QAction(tr("Deutsch"), this);
    guiLanguageGermanAct->setStatusTip(tr("Switch the GUI language to German"));
    guiLanguageGermanAct->setCheckable(true);
    connect(guiLanguageGermanAct, SIGNAL(triggered()), this, SLOT(actionSetGuiLanguageGerman()));

    guiLanguageGroup = new QActionGroup(this);
    guiLanguageGroup->addAction(guiLanguageEnglishAct);
    guiLanguageGroup->addAction(guiLanguageGermanAct);
    guiLanguageEnglishAct->setChecked(p_guiLanguage != "de");
    guiLanguageGermanAct->setChecked(p_guiLanguage == "de");

    // Functions Browser visibility - mutually exclusive Show/Hide, mirrors
    // the GUI Language pair above. Visible by default at startup (matches
    // functionsGroupBox's existing default state).
    showFunctionsBrowserAct = new QAction(tr("Show Functions Browser"), this);
    showFunctionsBrowserAct->setShortcut(tr("Ctrl+Shift+B"));
    showFunctionsBrowserAct->setShortcutContext(Qt::ApplicationShortcut);
    showFunctionsBrowserAct->setStatusTip(tr("Show the Functions panel"));
    showFunctionsBrowserAct->setCheckable(true);
    showFunctionsBrowserAct->setChecked(true);
    connect(showFunctionsBrowserAct, SIGNAL(triggered()), this, SLOT(actionShowFunctionsBrowser()));

    hideFunctionsBrowserAct = new QAction(tr("Hide Functions Browser"), this);
    hideFunctionsBrowserAct->setShortcut(tr("Ctrl+Shift+H"));
    hideFunctionsBrowserAct->setShortcutContext(Qt::ApplicationShortcut);
    hideFunctionsBrowserAct->setStatusTip(tr("Hide the Functions panel"));
    hideFunctionsBrowserAct->setCheckable(true);
    connect(hideFunctionsBrowserAct, SIGNAL(triggered()), this, SLOT(actionHideFunctionsBrowser()));

    functionsBrowserGroup = new QActionGroup(this);
    functionsBrowserGroup->addAction(showFunctionsBrowserAct);
    functionsBrowserGroup->addAction(hideFunctionsBrowserAct);

    compileAct = new QAction(QIcon(":/images/dice.png"), tr("Comp&ile..."), this);
    compileAct->setShortcut(tr("F6"));
    compileAct->setStatusTip(tr("Compile current file..."));
    connect(compileAct, SIGNAL(triggered()), this, SLOT(actionCompile()));

    showOutputAct = new QAction(tr("Show output pane..."), this);
    showOutputAct->setShortcut(tr("F9"));
    showOutputAct->setStatusTip(tr("Show Compiler output..."));
    connect(showOutputAct, SIGNAL(triggered()), this, SLOT(actionShowOutputConsole()));

    hideOutputAct = new QAction(tr("Hide output pane..."), this);
    hideOutputAct->setShortcut(tr("Shift+F9"));
    hideOutputAct->setStatusTip(tr("Hide Compiler output..."));
    connect(hideOutputAct, SIGNAL(triggered()), this, SLOT(actionCloseOutputConsole()));

    /* --- Tools -----------------------------------------------------------------------*/
    emulatorAct = new QAction(QIcon(":/images/start-emu.png"), tr("Start default Workbench in UA&E..."), this);
    emulatorAct->setShortcut(tr("Alt+e"));
    emulatorAct->setStatusTip(tr("Start Amiga Emulation..."));
    connect(emulatorAct, SIGNAL(triggered()), this, SLOT(actionEmulator()));

    emulator13Act = new QAction(QIcon(":/images/start-emu.png"), tr("Start Workbench 1.3 in UAE..."), this);
    emulator13Act->setShortcut(tr("Alt+f"));
    emulator13Act->setStatusTip(tr("Start Amiga Emulation..."));
    connect(emulator13Act, SIGNAL(triggered()), this, SLOT(actionEmuOS13()));

    emulator30Act = new QAction(QIcon(":/images/start-emu.png"), tr("Start Workbench 3.x in UAE..."), this);
    emulator30Act->setShortcut(tr("Alt+h"));
    emulator30Act->setStatusTip(tr("Start Amiga Emulation..."));
    connect(emulator30Act, SIGNAL(triggered()), this, SLOT(actionEmuOS30()));

    killEmulatorAct = new QAction(QIcon(":/images/stop-emu.png"), tr("Stop running Emulation..."), this);
    killEmulatorAct->setShortcut(tr("Alt+s"));
    killEmulatorAct->setStatusTip(tr("Stop Amiga Emulation..."));
    connect(killEmulatorAct, SIGNAL(triggered()), this, SLOT(actionKillEmulator()));

    /* --- Syntax -----------------------------------------------------------------------*/
    lexCPPAct = new QAction(tr("C/C++..."), this);
    lexCPPAct->setStatusTip(tr("highlight C/C++ syntax"));
    lexCPPAct->setCheckable(true);
    lexCPPAct->setChecked(true);
    connect(lexCPPAct, SIGNAL(triggered()), this, SLOT(initializeLexerCPP()));

    lexMakefileAct = new QAction(tr("Makefile..."), this);
    lexMakefileAct->setStatusTip(tr("highlight Makefile syntax"));
    lexMakefileAct->setCheckable(true);
    lexMakefileAct->setChecked(false);
    connect(lexMakefileAct, SIGNAL(triggered()), this, SLOT(initializeLexerMakefile()));

    lexBatchAct = new QAction(tr("C/C++..."), this);
    lexBatchAct->setStatusTip(tr("highlight AmigaShell syntax"));
    lexBatchAct->setCheckable(true);
    lexBatchAct->setChecked(false);
    connect(lexBatchAct, SIGNAL(triggered()), this, SLOT(initializeLexerBatch()));

    lexInstallerAct = new QAction(tr("Amiga installer..."), this);
    lexInstallerAct->setStatusTip(tr("highlight Amiga Installer syntax"));
    lexInstallerAct->setCheckable(true);
    lexInstallerAct->setChecked(false);
    connect(lexInstallerAct, SIGNAL(triggered()), this, SLOT(initializeLexerInstaller()));

    lexAmigaGuideAct = new QAction(tr("AmigaGuide..."), this);
    lexAmigaGuideAct->setStatusTip(tr("highlight AmigaGuide syntax"));
    lexAmigaGuideAct->setCheckable(true);
    lexAmigaGuideAct->setChecked(false);
    connect(lexAmigaGuideAct, SIGNAL(triggered()), this, SLOT(initializeLexerAmigaGuide()));

    lexM68kAsmAct = new QAction(tr("m68k Assembler..."), this);
    lexM68kAsmAct->setStatusTip(tr("highlight m68k Assembler syntax"));
    lexM68kAsmAct->setCheckable(true);
    lexM68kAsmAct->setChecked(false);
    connect(lexM68kAsmAct, SIGNAL(triggered()), this, SLOT(initializeLexerM68kAsm()));

    lexPascalAct = new QAction(tr("Pascal..."), this);
    lexPascalAct->setStatusTip(tr("highlight Pascal syntax"));
    lexPascalAct->setCheckable(true);
    lexPascalAct->setChecked(false);
    connect(lexPascalAct, SIGNAL(triggered()), this, SLOT(initializeLexerPascal()));

    lexPlainTextAct = new QAction(tr("Plain Text..."), this);
    lexPlainTextAct->setStatusTip(tr("show Plain Text only"));
    lexPlainTextAct->setCheckable(true);
    lexPlainTextAct->setChecked(false);
    connect(lexPlainTextAct, SIGNAL(triggered()), this, SLOT(initializeLexerNone()));

    // this will put the lexers in our menue into a mutual exclusive
    // group for automatically checking/unchecking each other:
    syntaxGroup = new QActionGroup(this);
    syntaxGroup->addAction(lexCPPAct);
    syntaxGroup->addAction(lexMakefileAct);
    syntaxGroup->addAction(lexInstallerAct);
    syntaxGroup->addAction(lexAmigaGuideAct);
    syntaxGroup->addAction(lexM68kAsmAct);
    syntaxGroup->addAction(lexPascalAct);
    syntaxGroup->addAction(lexPlainTextAct);

    /*
     * insertMenue actions:
     */
    includeAct = new QAction(tr("#include"), this); // inserts into insertMenue => preprocessorMenue
    includeAct->setShortcut(tr("Ctrl+i"));
    includeAct->setStatusTip(tr("insert #include <file>..."));
    connect(includeAct, SIGNAL(triggered()), this, SLOT(actionInsertInclude()));

    amigaIncludesAct = new QAction(tr("Amiga #include files"), this); // inserts into insertMenue => preprocessorMenue
    amigaIncludesAct->setStatusTip(tr("insert most commonly used Amiga #include files..."));
    connect(amigaIncludesAct, SIGNAL(triggered()), this, SLOT(actionInsertAmigaIncludes()));

    defineAct = new QAction(tr("#define"), this); // inserts into insertMenue => preprocessorMenue
    defineAct->setShortcut(tr("Alt+d"));
    defineAct->setStatusTip(tr("insert #define SOME_VALUE..."));
    connect(defineAct, SIGNAL(triggered()), this, SLOT(actionInsertDefine()));

    ifdefAct = new QAction(tr("#ifdef"), this); // inserts into insertMenue => preprocessorMenue
    ifdefAct->setStatusTip(tr("insert #ifdef ... #endif..."));
    connect(ifdefAct, SIGNAL(triggered()), this, SLOT(actionInsertIfdef()));

    ifdefinedAct = new QAction(tr("#if defined(...)"), this); // inserts into insertMenue => preprocessorMenue
    ifdefinedAct->setStatusTip(tr("insert #if defined(...) ... #endif..."));
    connect(ifdefinedAct, SIGNAL(triggered()), this, SLOT(actionInsertIfdefined()));

    ifdefinedCompilerAct = new QAction(tr("Identify Amiga compiler"), this); // inserts into insertMenue => preprocessorMenue
    ifdefinedCompilerAct->setStatusTip(tr("identify compiler in use..."));
    connect(ifdefinedCompilerAct, SIGNAL(triggered()), this, SLOT(actionInsertIfdefinedCompiler()));

    ifndefAct = new QAction(tr("#ifndef"), this); // inserts into insertMenue => preprocessorMenue
    ifndefAct->setStatusTip(tr("insert #ifndef ... #endif..."));
    connect(ifndefAct, SIGNAL(triggered()), this, SLOT(actionInsertIfndef()));

    OpenLibraryAct = new QAction(tr("OpenLibrary()"), this); // inserts into insertMenue => libraryMenue
    OpenLibraryAct->setStatusTip(tr("insert OpenLibrary()"));
    connect(OpenLibraryAct, SIGNAL(triggered()), this, SLOT(actionInsertOpenLibrary()));

    CloseLibraryAct = new QAction(tr("CloseLibrary()"), this); // inserts into insertMenue => libraryMenue
    CloseLibraryAct->setStatusTip(tr("insert CloseLibrary()"));
    connect(CloseLibraryAct, SIGNAL(triggered()), this, SLOT(actionInsertCloseLibrary()));

    ifAct = new QAction(tr("if(..) {...}"), this); // inserts into insertMenue => conditionsMenue
    ifAct->setShortcut(tr("Ctrl+Alt+i"));
    ifAct->setStatusTip(tr("insert if(..) {...}"));
    connect(ifAct, SIGNAL(triggered()), this, SLOT(actionInsertIf()));

    if_elseAct = new QAction(tr("if(..) {...} else {...}"), this); // inserts into insertMenue => conditionsMenue
    if_elseAct->setStatusTip(tr("insert if(..) {...} else {...}"));
    connect(if_elseAct, SIGNAL(triggered()), this, SLOT(actionInsertIfElse()));

    whileAct = new QAction(tr("while(...) {...}"), this); // inserts into insertMenue => loopsMenue
    whileAct->setStatusTip(tr("insert while(...) {...}"));
    connect(whileAct, SIGNAL(triggered()), this, SLOT(actionInsertWhile()));

    forAct = new QAction(tr("for(...) {...}"), this); // inserts into insertMenue => loopsMenue
    forAct->setStatusTip(tr("insert for(...) {...}"));
    connect(forAct, SIGNAL(triggered()), this, SLOT(actionInsertForLoop()));

    do_whileAct = new QAction(tr("do...{...}while(...)"), this); // inserts into insertMenue => loopsMenue
    do_whileAct->setStatusTip(tr("insert do...{...}while(...)"));
    connect(do_whileAct, SIGNAL(triggered()), this, SLOT(actionInsertDoWhile()));

    switchAct = new QAction(tr("switch(...)"), this); // inserts into insertMenue => loopsMenue
    switchAct->setStatusTip(tr("insert switch(...)"));
    connect(switchAct, SIGNAL(triggered()), this, SLOT(actionInsertSwitch()));

    mainAct = new QAction(tr("int main {...}"), this); // inserts into insertMenue
    mainAct->setStatusTip(tr("insert main() {...}"));
    connect(mainAct, SIGNAL(triggered()), this, SLOT(actionInsertMain()));

    functionAct = new QAction(tr("int function {...}"), this); // inserts into insertMenue
    functionAct->setStatusTip(tr("insert C function definition"));
    connect(functionAct, SIGNAL(triggered()), this, SLOT(actionInsertFunction()));

    enumAct = new QAction(tr("enum {...}"), this); // inserts into insertMenue
    enumAct->setStatusTip(tr("insert enum {...}"));
    connect(enumAct, SIGNAL(triggered()), this, SLOT(actionInsertEnum()));

    consoleDebugAct = new QAction(tr("Console Debugging Message"), this); // inserts into insertMenue
    consoleDebugAct->setStatusTip(tr("insert if(myDebug){...} debugging block"));
    connect(consoleDebugAct, SIGNAL(triggered()), this, SLOT(actionInsertConsoleDebugMessage()));

    fileheaderAct = new QAction(tr("Fileheader comment..."), this); // inserts into insertMenue => commentsMenue
    fileheaderAct->setStatusTip(tr("insert Fileheader comment"));
    connect(fileheaderAct, SIGNAL(triggered()), this, SLOT(actionInsertFileheaderComment()));

    toggleCommentBlockAct = new QAction(tr("Comment/Uncomment Block"), this); // standalone top-level entry in insertMenue AND the context menu (not nested in commentsMenue - see rev.101)
    toggleCommentBlockAct->setShortcut(tr("Ctrl+/"));
    toggleCommentBlockAct->setStatusTip(tr("Comment out the selected lines with \"// \", or remove it if they're already commented"));
    connect(toggleCommentBlockAct, SIGNAL(triggered()), this, SLOT(actionToggleCommentBlock()));

    versionStringAct = new QAction(tr("Amiga C version string"), this); // inserts into insertMenue => preprocessorMenue
    versionStringAct->setStatusTip(tr("insert $VER: programname version.revision (dd.mm.yyyy)"));
    connect(versionStringAct, SIGNAL(triggered()), this, SLOT(actionInsertAmigaVersionString()));

    c_singleAct = new QAction(tr("C-style single line comment..."), this); // inserts into insertMenue => commentsMenue
    c_singleAct->setStatusTip(tr("insert C-style single line comment"));
    connect(c_singleAct, SIGNAL(triggered()), this, SLOT(actionInsertCSingleComment()));

    c_multiAct = new QAction(tr("C-style multi line comment..."), this); // inserts into insertMenue => commentsMenue
    c_multiAct->setStatusTip(tr("insert C-style multi line comment"));
    connect(c_multiAct, SIGNAL(triggered()), this, SLOT(actionInsertCMultiComment()));

    cpp_singleAct = new QAction(tr("C++ style single line comment..."), this); // inserts into insertMenue => commentsMenue
    cpp_singleAct->setStatusTip(tr("insert C++ style single line comment"));
    connect(cpp_singleAct, SIGNAL(triggered()), this, SLOT(actionInsertCppSingleComment()));

    lineDevideCommentAct = new QAction(tr("C-style single line code dividing comment..."), this); // inserts into insertMenue => commentsMenue
    lineDevideCommentAct->setStatusTip(tr("insert code dividing comment: /* --- COMMENT -------*/"));
    connect(lineDevideCommentAct, SIGNAL(triggered()), this, SLOT(actionInsertCLineDevideComment()));

    /* --- Help -------------------------------------------------------------------------*/
    manualAct = new QAction(tr("Manual"), this);
    manualAct->setShortcut(QKeySequence(Qt::Key_F1));
    manualAct->setStatusTip(tr("Open the AmigaED manual"));
    connect(manualAct, SIGNAL(triggered()), this, SLOT(actionShowManual()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

//
// ...now that we've created actions, let's create some menues for our app!
//
void MainWindow::createMenus()
{
    // File menu
    fileMenue = menuBar()->addMenu(tr("&File")); // this is a main menue entry as shown in the menue bar of the app window (File)
    fileMenue->addAction(newAct);                // this is menue entry (File/New)
    newProjectMenue = fileMenue->addMenu(tr("New Project..."));
    newProjectMenue->addAction(importExistingProjectAct);
    newProjectMenue->addSeparator();
    newProjectMenue->addAction(newProjectEmptyCAct);
    newProjectMenue->addAction(newProjectShellAct);
    newProjectMenue->addAction(newProjectAmigaOS13Act);
    newProjectMenue->addAction(newProjectAmigaOS3xAct);
    newProjectMenue->addAction(newProjectReActionAct);
    newProjectMenue->addAction(newProjectMUIAct);
    fileMenue->addAction(loadProjectAct);
    fileMenue->addAction(saveProjectAct);
    fileMenue->addAction(closeProjectAct);
    fileMenue->addAction(addFilesToProjectAct);
    recentProjectsMenue = fileMenue->addMenu(tr("Recent Projects"));
    updateRecentProjectsMenu();   // populate with whatever was loaded from settings in readSettings()
    fileMenue->addAction(openAct);
    recentFilesMenue = fileMenue->addMenu(tr("Recent files"));
    updateRecentFilesMenu();      // populate with whatever was loaded from settings in readSettings()
    fileMenue->addAction(saveAct);
    fileMenue->addAction(saveAsAct);
    fileMenue->addSeparator();
    fileMenue->addAction(printAct);
    fileMenue->addSeparator();
    fileMenue->addAction(prefsAct);
    fileMenue->addAction(prefsReloadAct);
    fileMenue->addSeparator();
    fileMenue->addAction(exitAct);

    // Edit menue
    editMenue = menuBar()->addMenu(tr("&Edit"));
    editMenue->addAction(undoAct);
    editMenue->addAction(redoAct);
    editMenue->addSeparator();
    editMenue->addAction(cutAct);
    editMenue->addAction(copyAct);
    editMenue->addAction(pasteAct);
    editMenue->addSeparator();
    editMenue->addAction(searchAct);

    menuBar()->addSeparator();

    // Inserts menue
    insertMenue = menuBar()->addMenu(tr("&Inserts"));
    insertMenue->addAction(toggleCommentBlockAct);
    insertMenue->addSeparator();
    preprocessorMenue = insertMenue->addMenu(tr("Preprocessor..."));
    preprocessorMenue->addAction(includeAct);
    preprocessorMenue->addAction(defineAct);
    preprocessorMenue->addAction(ifdefAct);
    preprocessorMenue->addAction(ifndefAct);
    preprocessorMenue->addAction(ifdefinedAct);
    preprocessorMenue->addSeparator();
    preprocessorMenue->addAction(amigaIncludesAct);
    preprocessorMenue->addSeparator();
    preprocessorMenue->addAction(ifdefinedCompilerAct);
    preprocessorMenue->addSeparator();
    preprocessorMenue->addAction(versionStringAct);
    insertMenue->addSeparator();
    libraryMenue = insertMenue->addMenu(tr("Libraries..."));
    libraryMenue->addAction(OpenLibraryAct);
    libraryMenue->addAction(CloseLibraryAct);
    insertMenue->addSeparator();
    conditionsMenue = insertMenue->addMenu(tr("Condition..."));
    conditionsMenue->addAction(ifAct);
    conditionsMenue->addAction(if_elseAct);
    insertMenue->addSeparator();
    loopsMenue = insertMenue->addMenu(tr("Loops..."));
    loopsMenue->addAction(whileAct);
    loopsMenue->addAction(forAct);
    loopsMenue->addAction(do_whileAct);
    loopsMenue->addAction(switchAct);
    insertMenue->addSeparator();
    insertMenue->addAction(mainAct);
    insertMenue->addAction(functionAct);
    insertMenue->addAction(enumAct);
    insertMenue->addSeparator();
    insertMenue->addAction(consoleDebugAct);
    insertMenue->addSeparator();
    commentsMenue = insertMenue->addMenu(tr("Comments..."));
    commentsMenue->addAction(fileheaderAct);
    commentsMenue->addSeparator();
    commentsMenue->addAction(c_singleAct);
    commentsMenue->addAction(c_multiAct);
    commentsMenue->addSeparator();
    commentsMenue->addAction(cpp_singleAct);
    commentsMenue->addSeparator();
    commentsMenue->addAction(lineDevideCommentAct);

    menuBar()->addSeparator();

    // Build menue
    buildMenue = menuBar()->addMenu(tr("&Build"));
    compilerMenue = buildMenue->addMenu(tr("Select Compiler..."));
    compilerMenue->addAction(selectCompilerVBCCAct);
    compilerMenue->addSeparator();
    compilerMenue->addAction(selectCompilerGCCAct);
    compilerMenue->addSeparator();
    compilerMenue->addAction(selectCompilerGPPAct);
    buildMenue->addSeparator();
    buildMenue->addAction(compileAct);
    buildMenue->addAction(buildProjectAct);
    buildMenue->addAction(cleanProjectAct);
    buildMenue->addSeparator();
    buildMenue->addAction(toggleGccDefaultOptsAct);
    buildMenue->addAction(toggleVbccDefaultOptsAct);


    menuBar()->addSeparator();

    // Navigation menue
    navigationMenue = menuBar()->addMenu(tr("&Navigation"));
    navigationMenue->addAction(gotoTopAct);
    navigationMenue->addAction(gotoBottomAct);
    navigationMenue->addAction(gotoLineAct);
    navigationMenue->addSeparator();
    navigationMenue->addAction(gotoMatchingBraceAct);

    menuBar()->addSeparator();

    // View menue
    viewMenue = menuBar()->addMenu(tr("&View"));
    guiLanguageMenue = viewMenue->addMenu(tr("GUI Language"));
    guiLanguageMenue->addAction(guiLanguageEnglishAct);
    guiLanguageMenue->addAction(guiLanguageGermanAct);
    buildThemeMenu();
    viewMenue->addSeparator();
    viewMenue->addAction(showFunctionsBrowserAct);
    viewMenue->addAction(hideFunctionsBrowserAct);
    viewMenue->addSeparator();
    viewMenue->addAction(showOutputAct);
    viewMenue->addAction(hideOutputAct);
    viewMenue->addSeparator();
    viewMenue->addAction(showLineNumbersAct);
    viewMenue->addSeparator();
    viewMenue->addAction(showCaretLineAct);
    viewMenue->addSeparator();
    viewMenue->addAction(showIndentationGuidesAct);
    viewMenue->addSeparator();
    viewMenue->addAction(toggleFoldAct);
    viewMenue->addSeparator();
    charMenue = viewMenue->addMenu(tr("Special characters..."));
    charMenue->addAction(showEOLAct);
    charMenue->addAction(showUnprintableAct);

    viewMenue->addAction(showDebugInfoAct);
    viewMenue->addSeparator();
    viewMenue->addAction(zoomInAct);
    viewMenue->addAction(zoomOutAct);
    viewMenue->addAction(zoomResetAct);

    menuBar()->addSeparator();

    // Syntax menue
    syntaxMenue = menuBar()->addMenu(tr("Synta&x"));
    syntaxMenue->addAction(lexCPPAct);
    syntaxMenue->addSeparator();
    syntaxMenue->addAction(lexMakefileAct);
    syntaxMenue->addSeparator();
    syntaxMenue->addAction(lexInstallerAct);
    syntaxMenue->addSeparator();
    syntaxMenue->addAction(lexAmigaGuideAct);
    syntaxMenue->addSeparator();
    syntaxMenue->addAction(lexM68kAsmAct);
    syntaxMenue->addSeparator();
    syntaxMenue->addAction(lexPascalAct);
    syntaxMenue->addSeparator();
    syntaxMenue->addAction(lexPlainTextAct);

    menuBar()->addSeparator();

    // Tools menue
    toolsMenue = menuBar()->addMenu(tr("&Tools"));
    emulatorMenue = toolsMenue->addMenu(tr("Emulator..."));
    emulatorMenue->addAction(emulator13Act);
    emulatorMenue->addAction(emulator30Act);
    emulatorMenue->addSeparator();
    emulatorMenue->addAction(emulatorAct);
    toolsMenue->addSeparator();
    toolsMenue->addAction(killEmulatorAct);

    // Help menue
    helpMenue = menuBar()->addMenu(tr("&Help"));
    helpMenue->addAction(manualAct);
    helpMenue->addSeparator();
    helpMenue->addAction(aboutAct);
    helpMenue->addAction(aboutQtAct);
}

//
// Let's put some of our actions into the app's toolbar!
// You can put any action into any toolbar section - as
// long as you have created that section before!
//
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));   // this is a section, correspondending to main menue "File"
    fileToolBar->addAction(newAct);         // this is a section entry, correspondending to menue entry "File/New"
    fileToolBar->addSeparator();
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(saveAsAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(printAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(undoAct);
    editToolBar->addAction(redoAct);
    editToolBar->addSeparator();
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);

    searchToolBar = addToolBar(tr("File"));
    searchToolBar->addAction(searchAct);

    navigationToolBar = addToolBar(tr("Navigation"));
    navigationToolBar->addAction(gotoLineAct);
    navigationToolBar->addAction(gotoMatchingBraceAct);

    buildToolBar = addToolBar(tr("Build"));
    buildToolBar->addAction(compileAct);
    buildToolBar->addAction(buildProjectAct);   // mirrors menu entry Build/Build Project
    buildToolBar->addAction(cleanProjectAct);   // mirrors menu entry Build/Clean Project
    buildToolBar->addSeparator();
    buildToolBar->addAction(emulatorAct);
    buildToolBar->addAction(killEmulatorAct);

    toolsToolBar = addToolBar(tr("Navigation"));
    toolsToolBar->addAction(exitAct);
}

//
// Show initial message in the app's status bar or change it on demand
//

//
// Re-apply all translatable GUI strings after a runtime language change.
// Mirrors the text set up in createActions()/createMenus()/createToolBars()/
// createProjectPanel()/createFunctionsPanel() and the status bar, so every
// user-visible string picks up the newly installed QTranslator without
// requiring an application restart.
//
void MainWindow::retranslateUi()
{
    // -- Actions (menu items, toolbar buttons) --
    newAct->setText(tr("&New"));
    openAct->setText(tr("&Open..."));
    newProjectEmptyCAct->setText(tr("Empty Amiga C Project"));
    newProjectShellAct->setText(tr("Shell Project"));
    newProjectAmigaOS13Act->setText(tr("AmigaOS 1.3 Project"));
    newProjectAmigaOS3xAct->setText(tr("AmigaOS 3.x Project"));
    newProjectReActionAct->setText(tr("ReAction Project"));
    newProjectMUIAct->setText(tr("MUI Project"));
    importExistingProjectAct->setText(tr("Import existing Project..."));
    importExistingProjectAct->setStatusTip(tr("Import an existing C/C++ project folder that AmigaED doesn't know yet"));
    loadProjectAct->setText(tr("Load Project..."));
    saveProjectAct->setText(tr("Save Project"));
    closeProjectAct->setText(tr("Close Project"));
    addFilesToProjectAct->setText(tr("Add files to Project..."));
    buildProjectAct->setText(tr("Build Project"));
    cleanProjectAct->setText(tr("Clean Project"));
    saveAct->setText(tr("&Save"));
    saveAsAct->setText(tr("Save &As..."));
    prefsAct->setText(tr("Global prefs..."));
    prefsReloadAct->setText(tr("Reload settings"));
    printAct->setText(tr("&Print file..."));
    exitAct->setText(tr("&Exit"));
    undoAct->setText(tr("&Undo"));
    redoAct->setText(tr("&Redo"));
    cutAct->setText(tr("Cu&t"));
    copyAct->setText(tr("&Copy"));
    pasteAct->setText(tr("&Paste"));
    searchAct->setText(tr("Sea&rch..."));
    contextSearchReplaceAct->setText(tr("Search and Replace..."));
    gotoTopAct->setText(tr("&Goto top..."));
    gotoBottomAct->setText(tr("&Goto bottom..."));
    gotoLineAct->setText(tr("&Goto Line..."));
    gotoMatchingBraceAct->setText(tr("Goto &matching bracket {} ... [] ... ()..."));
    toggleFoldAct->setText(tr("&Fold/Unfold all..."));
    showLineNumbersAct->setText(tr("Show line numbers..."));
    showCaretLineAct->setText(tr("Show caret line..."));
    showDebugInfoAct->setText(tr("Show debug output"));
    zoomInAct->setText(tr("Increase font size"));
    zoomOutAct->setText(tr("Decrease font size"));
    zoomResetAct->setText(tr("Reset font size"));
    showEOLAct->setText(tr("Show EOL character"));
    showUnprintableAct->setText(tr("Show unprintable characters"));
    showIndentationGuidesAct->setText(tr("Show indentation guides"));
    selectCompilerVBCCAct->setText(tr("VBCC vc (C mode only)..."));
    selectCompilerGCCAct->setText(tr("GNU gcc (C mode)..."));
    selectCompilerGPPAct->setText(tr("GNU g++ (C++ mode)..."));
    toggleGccDefaultOptsAct->setText(tr("Show gcc/g++ options dialog..."));
    toggleVbccDefaultOptsAct->setText(tr("Show vbcc options dialog..."));
    compileAct->setText(tr("Comp&ile..."));
    showOutputAct->setText(tr("Show output pane..."));
    hideOutputAct->setText(tr("Hide output pane..."));
    emulatorAct->setText(tr("Start default Workbench in UA&E..."));
    emulator13Act->setText(tr("Start Workbench 1.3 in UAE..."));
    emulator30Act->setText(tr("Start Workbench 3.x in UAE..."));
    killEmulatorAct->setText(tr("Stop running Emulation..."));
    lexCPPAct->setText(tr("C/C++..."));
    lexMakefileAct->setText(tr("Makefile..."));
    lexBatchAct->setText(tr("C/C++..."));
    lexInstallerAct->setText(tr("Amiga installer..."));
    lexAmigaGuideAct->setText(tr("AmigaGuide..."));
    lexM68kAsmAct->setText(tr("m68k Assembler..."));
    lexPascalAct->setText(tr("Pascal..."));
    lexPlainTextAct->setText(tr("Plain Text..."));
    includeAct->setText(tr("#include"));
    amigaIncludesAct->setText(tr("Amiga #include files"));
    defineAct->setText(tr("#define"));
    ifdefAct->setText(tr("#ifdef"));
    ifdefinedAct->setText(tr("#if defined(...)"));
    ifdefinedCompilerAct->setText(tr("Identify Amiga compiler"));
    ifndefAct->setText(tr("#ifndef"));
    OpenLibraryAct->setText(tr("OpenLibrary()"));
    CloseLibraryAct->setText(tr("CloseLibrary()"));
    ifAct->setText(tr("if(..) {...}"));
    if_elseAct->setText(tr("if(..) {...} else {...}"));
    whileAct->setText(tr("while(...) {...}"));
    forAct->setText(tr("for(...) {...}"));
    do_whileAct->setText(tr("do...{...}while(...)"));
    switchAct->setText(tr("switch(...)"));
    mainAct->setText(tr("int main {...}"));
    functionAct->setText(tr("int function {...}"));
    enumAct->setText(tr("enum {...}"));
    consoleDebugAct->setText(tr("Console Debugging Message"));
    toggleCommentBlockAct->setText(tr("Comment/Uncomment Block"));
    fileheaderAct->setText(tr("Fileheader comment..."));
    versionStringAct->setText(tr("Amiga C version string"));
    c_singleAct->setText(tr("C-style single line comment..."));
    c_multiAct->setText(tr("C-style multi line comment..."));
    cpp_singleAct->setText(tr("C++ style single line comment..."));
    lineDevideCommentAct->setText(tr("C-style single line code dividing comment..."));
    manualAct->setText(tr("Manual"));
    aboutAct->setText(tr("&About"));
    aboutQtAct->setText(tr("About &Qt"));

    // -- Status tips (shown in the status bar on hover) --
    newAct->setStatusTip(tr("Create a new file"));
    openAct->setStatusTip(tr("Open an existing file"));
    loadProjectAct->setStatusTip(tr("Load an AmigaED project (.aep)"));
    saveProjectAct->setStatusTip(tr("Save the current project's file list and settings to its .aep, if it has unsaved changes"));
    closeProjectAct->setStatusTip(tr("Close the current project and all of its open tabs"));
    addFilesToProjectAct->setStatusTip(tr("Add one or more existing files to the current project"));
    buildProjectAct->setStatusTip(tr("Run the project's Makefile (target \"all\") for the currently selected compiler"));
    cleanProjectAct->setStatusTip(tr("Run the project's Makefile (target \"clean\") for the currently selected compiler"));
    saveAct->setStatusTip(tr("Save the document to disk"));
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    prefsAct->setStatusTip(tr("Open global preferences..."));
    prefsReloadAct->setStatusTip(tr("Reload global settings..."));
    printAct->setStatusTip(tr("Prepare for printing..."));
    exitAct->setStatusTip(tr("Exit the application"));
    searchAct->setStatusTip(tr("Search text in document"));
    contextSearchReplaceAct->setStatusTip(tr("Open Search and Replace, using the word under the click as the search term"));
    gotoTopAct->setStatusTip(tr("Goto top of file..."));
    gotoBottomAct->setStatusTip(tr("Goto bottom of file..."));
    gotoLineAct->setStatusTip(tr("Goto line X..."));
    gotoMatchingBraceAct->setStatusTip(tr("Goto matching bracket..."));
    toggleFoldAct->setStatusTip(tr("Fold or unfold the whole document"));
    showLineNumbersAct->setStatusTip(tr("Show or hide line numbers"));
    showCaretLineAct->setStatusTip(tr("Show or hide caret line"));
    showDebugInfoAct->setStatusTip(tr("Toggle debug output visibility"));
    zoomInAct->setStatusTip(tr("Increase the editor's font size"));
    zoomOutAct->setStatusTip(tr("Decrease the editor's font size"));
    zoomResetAct->setStatusTip(tr("Reset the editor's font size to normal"));
    showEOLAct->setStatusTip(tr("Toggle EOL visibility"));
    showUnprintableAct->setStatusTip(tr("Toggle unprintable characters visibility"));
    showIndentationGuidesAct->setStatusTip(tr("Toggle indentation guides visibility"));
    selectCompilerVBCCAct->setStatusTip(tr("Set Compiler to VBCC (C mode only)..."));
    selectCompilerGCCAct->setStatusTip(tr("Set Compiler to GNU gcc (C mode)..."));
    selectCompilerGPPAct->setStatusTip(tr("Set Compiler to GNU g++ (C++ mode)..."));
    toggleGccDefaultOptsAct->setStatusTip(tr("Show or hide gcc/g++ options dialog"));
    toggleVbccDefaultOptsAct->setStatusTip(tr("Show or hide vbcc options dialog"));
    compileAct->setStatusTip(tr("Compile current file..."));
    showOutputAct->setStatusTip(tr("Show Compiler output..."));
    hideOutputAct->setStatusTip(tr("Hide Compiler output..."));
    emulatorAct->setStatusTip(tr("Start Amiga Emulation..."));
    emulator13Act->setStatusTip(tr("Start Amiga Emulation..."));
    emulator30Act->setStatusTip(tr("Start Amiga Emulation..."));
    killEmulatorAct->setStatusTip(tr("Stop Amiga Emulation..."));
    lexCPPAct->setStatusTip(tr("highlight C/C++ syntax"));
    lexMakefileAct->setStatusTip(tr("highlight Makefile syntax"));
    lexBatchAct->setStatusTip(tr("highlight AmigaShell syntax"));
    lexInstallerAct->setStatusTip(tr("highlight Amiga Installer syntax"));
    lexAmigaGuideAct->setStatusTip(tr("highlight AmigaGuide syntax"));
    lexM68kAsmAct->setStatusTip(tr("highlight m68k Assembler syntax"));
    lexPascalAct->setStatusTip(tr("highlight Pascal syntax"));
    lexPlainTextAct->setStatusTip(tr("show Plain Text only"));
    includeAct->setStatusTip(tr("insert #include <file>..."));
    amigaIncludesAct->setStatusTip(tr("insert most commonly used Amiga #include files..."));
    defineAct->setStatusTip(tr("insert #define SOME_VALUE..."));
    ifdefAct->setStatusTip(tr("insert #ifdef ... #endif..."));
    ifdefinedAct->setStatusTip(tr("insert #if defined(...) ... #endif..."));
    ifdefinedCompilerAct->setStatusTip(tr("identify compiler in use..."));
    ifndefAct->setStatusTip(tr("insert #ifndef ... #endif..."));
    OpenLibraryAct->setStatusTip(tr("insert OpenLibrary()"));
    CloseLibraryAct->setStatusTip(tr("insert CloseLibrary()"));
    ifAct->setStatusTip(tr("insert if(..) {...}"));
    if_elseAct->setStatusTip(tr("insert if(..) {...} else {...}"));
    whileAct->setStatusTip(tr("insert while(...) {...}"));
    forAct->setStatusTip(tr("insert for(...) {...}"));
    do_whileAct->setStatusTip(tr("insert do...{...}while(...)"));
    switchAct->setStatusTip(tr("insert switch(...)"));
    mainAct->setStatusTip(tr("insert main() {...}"));
    functionAct->setStatusTip(tr("insert C function definition"));
    enumAct->setStatusTip(tr("insert enum {...}"));
    consoleDebugAct->setStatusTip(tr("insert if(myDebug){...} debugging block"));
    toggleCommentBlockAct->setStatusTip(tr("Comment out the selected lines with \"// \", or remove it if they're already commented"));
    fileheaderAct->setStatusTip(tr("insert Fileheader comment"));
    versionStringAct->setStatusTip(tr("insert $VER: programname version.revision (dd.mm.yyyy)"));
    c_singleAct->setStatusTip(tr("insert C-style single line comment"));
    c_multiAct->setStatusTip(tr("insert C-style multi line comment"));
    cpp_singleAct->setStatusTip(tr("insert C++ style single line comment"));
    lineDevideCommentAct->setStatusTip(tr("insert code dividing comment: /* --- COMMENT -------*/"));
    manualAct->setStatusTip(tr("Open the AmigaED manual"));
    aboutAct->setStatusTip(tr("Show the application's About box"));
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    addProjectFileBtn->setStatusTip(tr("Add an existing file to the project"));
    removeProjectFileBtn->setStatusTip(tr("Remove the selected file from the project"));
    compilerCombo->setStatusTip(tr("Select compiler to use for this file"));
    osCombo->setStatusTip(tr("Change default target OS"));
    compilerButton->setStatusTip(tr("Compile current file..."));

    // -- Menu titles --
    fileMenue->setTitle(tr("&File"));
    newProjectMenue->setTitle(tr("New Project..."));
    recentProjectsMenue->setTitle(tr("Recent Projects"));
    recentFilesMenue->setTitle(tr("Recent files"));
    editMenue->setTitle(tr("&Edit"));
    insertMenue->setTitle(tr("&Inserts"));
    preprocessorMenue->setTitle(tr("Preprocessor..."));
    libraryMenue->setTitle(tr("Libraries..."));
    conditionsMenue->setTitle(tr("Condition..."));
    loopsMenue->setTitle(tr("Loops..."));
    commentsMenue->setTitle(tr("Comments..."));
    buildMenue->setTitle(tr("&Build"));
    compilerMenue->setTitle(tr("Select Compiler..."));
    navigationMenue->setTitle(tr("&Navigation"));
    viewMenue->setTitle(tr("&View"));
    charMenue->setTitle(tr("Special characters..."));
    syntaxMenue->setTitle(tr("Synta&x"));
    toolsMenue->setTitle(tr("&Tools"));
    emulatorMenue->setTitle(tr("Emulator..."));
    helpMenue->setTitle(tr("&Help"));

    // -- Toolbar titles (shown in the toolbar right-click context menu) --
    fileToolBar->setWindowTitle(tr("File"));
    editToolBar->setWindowTitle(tr("Edit"));
    searchToolBar->setWindowTitle(tr("File"));
    navigationToolBar->setWindowTitle(tr("Navigation"));
    buildToolBar->setWindowTitle(tr("Build"));
    toolsToolBar->setWindowTitle(tr("Navigation"));

    // -- Project panel (createProjectPanel()) --
    projectGroupBox->setTitle(tr("Project"));
    projectSourceGroupItem->setText(0, tr("C/C++ Files"));
    projectHeaderGroupItem->setText(0, tr("Header Files"));
    projectInstallerGroupItem->setText(0, tr("Installer Scripts"));
    projectMakefileGroupItem->setText(0, tr("Makefiles"));
    addProjectFileBtn->setText(tr("Add..."));
    addProjectFileBtn->setStatusTip(tr("Add an existing file to the project"));
    removeProjectFileBtn->setText(tr("Remove"));
    removeProjectFileBtn->setStatusTip(tr("Remove the selected file from the project"));
    refreshProjectTree();   // re-derive the "[main]" suffix and file tooltips via tr()

    // -- Functions panel (createFunctionsPanel()) --
    functionsGroupBox->setTitle(tr("Functions"));
    refreshFunctionsList(); // re-derive the "%1, line %2" tooltips via tr()

    // -- Status bar --
    if (!(p_no_compilerbuttons) && compilerLabel)
        compilerLabel->setText(tr("Compiler:"));
    statusLabelX->setText(tr("Line:"));
    statusLabelY->setText(tr("Column:"));

    // -- Search & replace form (built by hand from the former centerSearchForm .ui) --
    label_find->setText(QApplication::translate("centerSearchForm", "Find:", nullptr));
    btn_previous->setToolTip(QApplication::translate("centerSearchForm", "goto previous occourance", nullptr));
    btn_next->setToolTip(QApplication::translate("centerSearchForm", "goto next occourance", nullptr));
    label_replace->setText(QApplication::translate("centerSearchForm", "Replace:", nullptr));
    btn_replace->setToolTip(QApplication::translate("centerSearchForm", "Replace current occurance", nullptr));
    btn_replace_all->setToolTip(QApplication::translate("centerSearchForm", "Replace ALL occurances", nullptr));
    btn_hide->setToolTip(QApplication::translate("centerSearchForm", "Hide Search and Replace...", nullptr));
    checkBox_CaseSensitive->setText(QApplication::translate("centerSearchForm", "Case sensitive search", nullptr));
    checkBox_WholeWords->setText(QApplication::translate("centerSearchForm", "Whole words", nullptr));
    checkBox_SearchForwards->setText(QApplication::translate("MainWindow", "Search forwards", nullptr));
    btnCloseOutput->setText(tr("Hide compiler output"));
    outputGroup->setTitle(tr("Compiler output"));
    searchGroup->setTitle(tr("Search and replace"));

    // -- Window title --
    updateWindowTitle();

    // -- GUI-language menu (View menu, added separately - see createMenus()) --
    guiLanguageMenue->setTitle(tr("GUI Language"));
    guiLanguageEnglishAct->setText(tr("English"));
    guiLanguageGermanAct->setText(tr("Deutsch"));

    // -- Theme menu (View menu, added separately - see buildThemeMenu()) --
    // Only the submenu's own title is translatable - its entries (native
    // style names, plus "Dark"/"Workbench 1.3"/"Workbench 3.1") are kept
    // untranslated on purpose, same as Prefs > Misc's matching combo box -
    // see buildThemeMenu()'s own comment for why.
    if (themeMenue)
        themeMenue->setTitle(tr("Theme"));

    // -- Functions Browser visibility menu (View menu, added separately - see createMenus()) --
    showFunctionsBrowserAct->setText(tr("Show Functions Browser"));
    showFunctionsBrowserAct->setStatusTip(tr("Show the Functions panel"));
    hideFunctionsBrowserAct->setText(tr("Hide Functions Browser"));
    hideFunctionsBrowserAct->setStatusTip(tr("Hide the Functions panel"));
}

void MainWindow::createStatusBarMessage(QString statusmessage, int timeout)
{
    statusBar()->showMessage(statusmessage, timeout);
}

void MainWindow::readPosSettings()
{
    QSettings settings("MB-SoftWorX", "Amiga Cross Editor");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);

    // Editor font size (QScintilla zoom level). textEdit doesn't exist
    // yet at this point in the constructor, so just remember the value -
    // it gets applied later in initializeGUI().
    p_zoomLevel = settings.value("EditorZoomLevel", 0).toInt();
}

//
// read app's last saved prefs
//
void MainWindow::readSettings()
{
    QSettings settings("MB-SoftWorX", "Amiga Cross Editor");
    // TAB: Project
    p_author = (settings.value("Project/Author").toString());
    p_email = (settings.value("Project/Email").toString());
    p_website = (settings.value("Project/Website").toString());
    p_projectsRootDir = (settings.value("Project/ProjectRootDir").toString());

    // TAB: GCC
    p_compiler_gcc = (settings.value("GCC/GccPath").toString());
    p_compiler_gpp = (settings.value("GCC/GppPath").toString());
    p_make = (settings.value("GCC/MakePath").toString());
    p_strip = (settings.value("GCC/StripPath").toString());
    // Defaults below target the m68k-amigaos-gcc ("Bebbo") toolchain.
    // -mcrt=nix13/-noixemul select which C runtime to link against -
    // libnix built for Kickstart 1.3 vs. Kickstart 2.0+ respectively -
    // NOT a CPU/optimization setting, and (per the toolchain's own docs)
    // must be given as the LAST option on the command line, same as
    // vbcc's '+config' below; if regenerateProjectMakefiles() ever grows
    // gcc-specific option reordering the way it already does for vbcc's
    // '+config', keep that constraint in mind. '-lamiga' (confirmed
    // supported by this toolchain) mirrors vbcc's own OS 3.x linker
    // default below - needed by a Workbench-capable OS 3.x program (the
    // "AmigaOS 3.x"/ReAction/MUI templates - see
    // mainFileTemplateContent()); no equivalent for OS 1.3, whose
    // template is deliberately console-only and never touches an Amiga
    // library.
    p_compiler_gcc13_call = (settings.value("GCC/Gcc13CompilerOpts", "-Wall -O2 -mcrt=nix13").toString());
    p_compiler_gcc30_call = (settings.value("GCC/Gcc30CompilerOpts", "-Wall -O2 -noixemul").toString());
    p_compiler_gcc13_linker_call = (settings.value("GCC/Gcc13LinkerOpts").toString());
    p_compiler_gcc30_linker_call = (settings.value("GCC/Gcc30LinkerOpts", "-lamiga").toString());
    p_compiler_gpp13_call = (settings.value("GCC/Gpp13CompilerOpts", "-Wall -O2 -mcrt=nix13").toString());
    p_compiler_gpp30_call = (settings.value("GCC/Gpp30CompilerOpts", "-Wall -O2 -noixemul").toString());
    p_compiler_gpp13_linker_call = (settings.value("GCC/Gpp13LinkerOpts").toString());
    p_compiler_gpp30_linker_call = (settings.value("GCC/Gpp30LinkerOpts", "-lamiga").toString());
    p_show_gcc_opts = (settings.value("GCC/ShowGccDefaultOpts").toBool());

    // TAB: VBCC
    p_compiler_vc = (settings.value("VBCC/VcPath").toString());
    p_compiler_vasm = (settings.value("VBCC/VasmPath").toString());
    p_vbcc_config_dir = (settings.value("VBCC/VcConfigPath").toString());
    // '+kick13'/'+aos68k' are vbcc's target CONFIGs (see vc's manual,
    // "vc +config ..."), not ordinary switches - regenerateProjectMakefiles()
    // already strips any OTHER '+'-prefixed token a user might add here,
    // since vc only accepts one, as the very first argument. '-lamiga
    // -lauto' (vbcc's own replacement for the classic amiga.lib, plus
    // auto-opening of library bases - see the vbcc manual, section "C
    // Library") is what a Workbench-capable OS 3.x program (the
    // "AmigaOS 3.x"/ReAction/MUI templates - see mainFileTemplateContent())
    // actually needs; a plain console-only OS 1.3 program doesn't touch
    // any Amiga library, hence no linker default there.
    p_compiler_vc13_call = (settings.value("VBCC/VcDefaultOpts13", "+kick13 -c99").toString());
    p_compiler_vc30_call = (settings.value("VBCC/VcDefaultOpts30", "+aos68k -c99").toString());
    p_compiler_vc13_linker_call = (settings.value("VBCC/Vc13LinkerOpts").toString());
    p_compiler_vc30_linker_call = (settings.value("VBCC/Vc30LinkerOpts", "-lamiga -lauto").toString());

    // TAB: SAS/C
    p_compiler_sc_call = (settings.value("SASC/DefaultOpts").toString());
    p_compiler_vc_default_target = (settings.value("VBCC/VcDefaultTarget").toInt());
    p_show_vbcc_opts = (settings.value("VBCC/ShowVbccDefaultOpts").toBool());

    // TAB: Emulator
    p_emulator = (settings.value("UAE/UaePath").toString());
    p_os13_config = (settings.value("UAE/Os13ConfigPath").toString());
    p_os30_config = (settings.value("UAE/Os30ConfigPath").toString());
    p_defaultEmulator = (settings.value("UAE/DefaultConfig").toInt());

    // TAB: Misc
    QString newDefaultStyle = settings.value("MISC/DefaultStyle").toString();
    bool defaultStyleChanged = (newDefaultStyle != p_default_style);
    p_default_style = newDefaultStyle;
    // Apply live (Prefs closed / Shift+F12 "Reload settings") - but not on
    // this very first call from the constructor (no tabWidget/tabs exist
    // yet at that point; the constructor applies the initial style itself
    // right after readSettings(), and newEditorTab() builds the first tab
    // already theme-aware). Guarded by p_styleInitialized, not by a widget
    // pointer - see its declaration for why.
    if (defaultStyleChanged && p_styleInitialized)
    {
        applyApplicationStyle();   // also re-applies editor tab colours and output-pane highlighting - see its own end for why
    }
    p_show_indentation = (settings.value("MISC/ShowIndentGuide").toBool());
    p_mydebug = (settings.value("MISC/ShowDebugOutput").toBool());
    p_no_lcd_statusbar = (settings.value("MISC/NoLCDstatusbar").toBool());
    p_defaultCompiler = (settings.value("MISC/DefaultCrossCompiler").toInt());
    p_no_compilerbuttons = (settings.value("MISC/NoCompileButton").toBool());
    p_simple_statusbar = (settings.value("MISC/SimpleStatusbar").toBool());
    p_create_icon = (settings.value("MISC/CreateIcon").toBool());
    p_console_on_fail = (settings.value("MISC/OpenConsoleOnFail").toBool());
    p_no_warn_requesters = (settings.value("MISC/NoWarnRequester").toBool());

    // GUI Language: applyGuiLanguage() itself is safe to call before any
    // widget exists (see its comment) - this is exactly what happens on the
    // very first call, since readSettings() is the first statement in the
    // constructor. Only re-apply when the value actually changed, so this
    // doesn't do needless work (or flicker) on every "Reload settings".
    QString newGuiLanguage = settings.value("MISC/DefaultGUILanguage", "en").toString();
    if (newGuiLanguage != p_guiLanguage || !guiLanguageEnglishAct)
        applyGuiLanguage(newGuiLanguage);

    // "Recent files" submenu content
    p_recentFiles = (settings.value("RecentFiles/List").toStringList());
    if (recentFilesMenue)               // not yet created on the very first call (constructor runs
        updateRecentFilesMenu();        // readSettings() before createMenus()) - refresh only if it exists

    // "Recent Projects" submenu content
    p_recentProjects = (settings.value("RecentProjects/List").toStringList());
    if (recentProjectsMenue)
        updateRecentProjectsMenu();
}

//
// Store app's last known position and sizes
//
void MainWindow::writeSettings()
{
    QSettings settings("MB-SoftWorX", "Amiga Cross Editor");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("EditorZoomLevel", static_cast<int>(textEdit->SendScintilla(textEdit->QsciScintilla::SCI_GETZOOM)));
}

//
//
// react on changed document text, allow the user to
// decide if he wants to keep or abandon his work
//
// 'editor' defaults to the currently active tab ('textEdit'), which keeps
// every existing call site unchanged. Pass a specific editor (e.g. when
// closing one particular tab that isn't the active one) to check that
// tab instead.
//
bool MainWindow::maybeSave(QsciScintilla *editor)
{
    if (!editor)
        editor = textEdit;

    if (editor && editor->isModified()) {
        // Temporarily make 'editor' the active tab for the save prompt,
        // so save()/saveAs() (which both operate on 'textEdit'/'curFile')
        // act on the right file even if 'editor' wasn't already active
        // (e.g. when closing a background tab via its own [x] button).
        QsciScintilla *previouslyActive = textEdit;
        int previousIndex = tabWidget ? tabWidget->indexOf(previouslyActive) : -1;
        int editorIndex = tabWidget ? tabWidget->indexOf(editor) : -1;
        bool switchedTemporarily = (editorIndex >= 0 && editor != previouslyActive);

        if (switchedTemporarily)
            tabWidget->setCurrentIndex(editorIndex);   // onTabChanged() updates textEdit/curFile

        QString shownName = strippedName(editor->property("amigaed_filePath").toString());
        if (shownName.isEmpty())
            shownName = tr("untitled.c");

        QMessageBox::StandardButton ret = QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                                                               tr("The document \"%1\" has been modified.\n"
                                                                  "Do you want to save your changes?").arg(shownName),
                                                               QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                                               QMessageBox::Yes);
        bool result = true;
        if (ret == QMessageBox::Yes)
            result = save();
        else if (ret == QMessageBox::Cancel)
            result = false;

        // Restore whichever tab was active before, unless the user just
        // cancelled (stay put so they can see/fix the tab in question) or
        // we never switched away in the first place.
        if (switchedTemporarily && result && previousIndex >= 0)
            tabWidget->setCurrentIndex(previousIndex);

        return result;
    }
    return true;
}

//
// user defined loading for source files
//
void MainWindow::loadFile(const QString &fileName)
{
    if(p_mydebug)
    {
        qDebug() << "loadFile() called with parameter: " << fileName;
    }
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                             tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    // Classic AmigaOS text (source files, comments, project metadata) is
    // encoded in ISO-8859-1 / Latin-1. Qt6's QTextStream defaults to
    // UTF-8, which would corrupt any umlaut or other non-ASCII byte on load.
    in.setEncoding(QStringConverter::Latin1);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    textEdit->setText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    addToRecentFiles(fileName);
}

//
// (Re-)build the "Recent files" submenu from p_recentFiles.
// Most recently opened file is shown at the top.
//
void MainWindow::updateRecentFilesMenu()
{
    recentFilesMenue->clear();

    if (p_recentFiles.isEmpty())
    {
        QAction *emptyAct = recentFilesMenue->addAction(tr("(no recent files)"));
        emptyAct->setEnabled(false);
        return;
    }

    for (int i = p_recentFiles.count() - 1; i >= 0; --i)
    {
        QAction *act = recentFilesMenue->addAction(p_recentFiles.at(i));
        act->setData(p_recentFiles.at(i));      // full path, independent of the displayed text
        connect(act, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    recentFilesMenue->addSeparator();
    QAction *forgetAct = recentFilesMenue->addAction(tr("Forget recent files"));
    connect(forgetAct, &QAction::triggered, this, [this]()
    {
        p_recentFiles.clear();
        QSettings settings("MB-SoftWorX", "Amiga Cross Editor");
        settings.setValue("RecentFiles/List", p_recentFiles);
        updateRecentFilesMenu();
    });
}

//
// Add a freshly opened file to the "Recent files" list:
// - no duplicates (re-opening a listed file just moves it back to the top)
// - capped at MaxRecentFiles entries; the oldest one is dropped once full
//
void MainWindow::addToRecentFiles(const QString &fileName)
{
    if (fileName.isEmpty())
        return;

    p_recentFiles.removeAll(fileName);
    p_recentFiles.append(fileName);

    while (p_recentFiles.count() > MaxRecentFiles)
        p_recentFiles.removeFirst();

    QSettings settings("MB-SoftWorX", "Amiga Cross Editor");
    settings.setValue("RecentFiles/List", p_recentFiles);

    updateRecentFilesMenu();
}

//
// Remove a (no longer existing) file from the "Recent files" list
//
void MainWindow::removeFromRecentFiles(const QString &fileName)
{
    p_recentFiles.removeAll(fileName);

    QSettings settings("MB-SoftWorX", "Amiga Cross Editor");
    settings.setValue("RecentFiles/List", p_recentFiles);

    updateRecentFilesMenu();
}

//
// (Re-)build the "Recent Projects" submenu from p_recentProjects, same
// pattern as updateRecentFilesMenu() above. Most recently used project
// shown at the top, displayed by its .aep basename (full path in the
// tooltip and as the action's data for openRecentProject()).
//
void MainWindow::updateRecentProjectsMenu()
{
    recentProjectsMenue->clear();

    if (p_recentProjects.isEmpty())
    {
        QAction *emptyAct = recentProjectsMenue->addAction(tr("(no recent projects)"));
        emptyAct->setEnabled(false);
        return;
    }

    for (int i = p_recentProjects.count() - 1; i >= 0; --i)
    {
        QAction *act = recentProjectsMenue->addAction(strippedName(p_recentProjects.at(i)));
        act->setToolTip(p_recentProjects.at(i));
        act->setData(p_recentProjects.at(i));      // full path, independent of the displayed text
        connect(act, SIGNAL(triggered()), this, SLOT(openRecentProject()));
    }

    recentProjectsMenue->addSeparator();
    QAction *forgetAct = recentProjectsMenue->addAction(tr("Forget recent Projects"));
    connect(forgetAct, &QAction::triggered, this, [this]()
    {
        p_recentProjects.clear();
        QSettings settings("MB-SoftWorX", "Amiga Cross Editor");
        settings.setValue("RecentProjects/List", p_recentProjects);
        updateRecentProjectsMenu();
    });
}

//
// Add a freshly created/loaded project to the "Recent Projects" list -
// no duplicates, capped at MaxRecentProjects entries.
//
void MainWindow::addToRecentProjects(const QString &fileName)
{
    if (fileName.isEmpty())
        return;

    p_recentProjects.removeAll(fileName);
    p_recentProjects.append(fileName);

    while (p_recentProjects.count() > MaxRecentProjects)
        p_recentProjects.removeFirst();

    QSettings settings("MB-SoftWorX", "Amiga Cross Editor");
    settings.setValue("RecentProjects/List", p_recentProjects);

    updateRecentProjectsMenu();
}

//
// Remove a (no longer existing) project from the "Recent Projects" list
//
void MainWindow::removeFromRecentProjects(const QString &fileName)
{
    p_recentProjects.removeAll(fileName);

    QSettings settings("MB-SoftWorX", "Amiga Cross Editor");
    settings.setValue("RecentProjects/List", p_recentProjects);

    updateRecentProjectsMenu();
}

//
// Called when the user clicks an entry in the "Recent Projects" submenu -
// loads that project the same way actionLoadProject() does (closing the
// previous project's tabs first, with save confirmation).
//
void MainWindow::openRecentProject()
{
    QAction *act = qobject_cast<QAction *>(sender());
    if (!act)
        return;

    QString fileName = act->data().toString();

    if (!QFile::exists(fileName))
    {
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                             tr("The project file no longer exists:\n%1\n\n"
                                "It will be removed from the \"Recent Projects\" list.")
                                 .arg(fileName));
        removeFromRecentProjects(fileName);
        return;
    }

    loadProjectFile(fileName);
}

//
// Called when the user clicks an entry in the "Recent files" submenu
//
void MainWindow::openRecentFile()
{
    QAction *act = qobject_cast<QAction *>(sender());
    if (!act)
        return;

    QString fileName = act->data().toString();

    if (!QFile::exists(fileName))
    {
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                             tr("The file no longer exists:\n%1\n\n"
                                "It will be removed from the \"Recent files\" list.")
                                 .arg(fileName));
        removeFromRecentFiles(fileName);
        return;
    }

    openFileInTab(fileName);
}

//
// user defined file creation loading for source files
// used for command line file loading
//
int MainWindow::loadNonExistantFile(const QString &fileName)
{
    int success = 1;

    // We need a qualified path in order to create and load the new file,
    // so let's build it:
    QString fileToOpen = QDir::currentPath();   // Path part of the filename
    fileToOpen.append(QDir::separator());       // add a unique separator after filename
    fileToOpen.append(fileName);                // add the fileName that we've requested on command line

    if(p_mydebug)
    {
        qDebug() << "created Filepath and -name: " << fileToOpen;
    }

    // Now let's create a QFile instance to open our file and maybe write to it!
    QFile file(fileToOpen);
    QFileInfo fileInfo(file);
    curFile = file.fileName();
    if(p_mydebug)
    {
        qDebug() << "Filepath given to the app: " << file.fileName();
    }

    // does our requested file allready exist?
    if (!file.open(QFile::ReadOnly))            // NO! File does NOT exist until now!
    {
        // Let's ask if it should be created!
        QMessageBox::StandardButton ret = QMessageBox::question(this, tr(AMIGAED_VERSION_STRING),
                                                                tr("File does not exist: %1<br>"
                                                                   "<br>Do you want me to create it?")
                                                                    .arg(fileName),
                                                                QMessageBox::Yes | QMessageBox::Cancel,
                                                                QMessageBox::Yes);

        // YES - we want the file to be created!
        if (ret == QMessageBox::Yes)
        {
            if(p_mydebug)
            {
                qDebug() << "Now creating requested file!";
            }

            if(file.open(QIODevice::WriteOnly))
            {
                if(p_mydebug)
                {
                    qDebug() << "File opened successfull for streaming...";
                }

                QTextStream stream(&file);                      // instanciate a stream to write to...
                stream.setEncoding(QStringConverter::Latin1);   // see loadFile() for rationale
                stream << "/*\n * File:\t\t" << fileName;       // stream some comments into file..
                stream << "\n * Version:\t\t" << p_version;
                stream << "\n * Revision:\t\t" << p_revision;
                stream << "\n *";
                stream << "\n * Author:\t" << p_author;
                stream << "\n * Email:\t" << p_email;
                stream << "\n * Web:\t" << p_website;
                stream << "\n *";
                stream << "\n*/" << Qt::endl;

                // ...close the freshly created file so we will
                // be able to load it into the editor window!
                file.close();

                if(p_mydebug)
                {
                    qDebug() << "Status: " << stream.status();
                    qDebug() << curFile;
                }
                loadFile(curFile);     //...and finally open that file in editor window

                // jump to last line in file
                actionGotoBottom();

                return 0;
            }
            else
            {
                if(p_mydebug)
                {
                    qDebug() << "ERROR: Could not create file!";
                    qDebug() << curFile;
                }

                int ret = QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                                               tr("<b>Something went terribly wrong!</b>"
                                                  "<br>File could <b>not</b> been created: %1<br>"
                                                  "<br>You will have to <b><i>save as...</i></b> an empty file,"
                                                  "<br>providing the requested file name.<br>"
                                                  "<br><b>Hint:</b> Selecting CANCEL quits AmigaED!")
                                                   .arg(fileName),
                                               QMessageBox::Ok | QMessageBox::Cancel );
                if (ret == QMessageBox::Cancel )
                {
                    return 10;
                }
                else
                {
                    return -1;
                }

            }
        }
        // NO - abandon file creation, stay on the empty tab we already have
        else if (ret == QMessageBox::Cancel)
        {
            if(p_mydebug)
            {
                qDebug() << "File creation abandoned!";
            }
            // NOTE: this runs during startup, right after the constructor
            // already created one empty "untitled.c" tab (see
            // newEditorTab() in MainWindow::MainWindow()) - calling
            // newFile() here would just open a redundant second empty
            // tab, so we simply keep using the one that's already there.
        }
    }

    if(p_mydebug)
    {
        qDebug() << "DEBUG: Now trying to load file into editor!";
    }
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Latin1);   // see loadFile() for rationale
    QApplication::setOverrideCursor(Qt::WaitCursor);
    textEdit->setText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 6000);
    addToRecentFiles(fileName);

    // jump to last line in file
    actionGotoBottom();

    return success;
}

//
// save current file
//
bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                             tr("Cannot write file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Latin1);   // see loadFile() for rationale
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->text();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    addToRecentFiles(fileName);
    maybeOfferAddToProject(fileName);

    // Refresh the function list right away if the saved file is (already)
    // part of the current project - most edits that would change it
    // happen on a file that's been in the project for a while, not one
    // that's only just been offered for adding above.
    if (currentProject && currentProject->contains(fileName))
        refreshFunctionsList();

    return true;
}

//
// set current file name, placing name and status
// in app's window title, and - since AmigaED v3.2's tabbed editor -
// remembering this tab's own file path plus syncing its tab label.
//
void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    textEdit->setModified(false);
    setWindowModified(false);

    // Every open tab tracks its own file path independently of every
    // other tab, via a plain Qt dynamic property on its QsciScintilla
    // widget - see onTabChanged(), findEditorForFile().
    textEdit->setProperty("amigaed_filePath", curFile);

    QString shownName = curFile.isEmpty() ? tr("untitled.c") : strippedName(curFile);

    updateWindowTitle();

    // Keep this tab's own label (and tooltip, for the full path) in sync
    // with the file it now holds.
    if (tabWidget)
    {
        int index = tabWidget->indexOf(textEdit);
        if (index >= 0)
        {
            tabWidget->setTabText(index, shownName);
            tabWidget->setTabToolTip(index, curFile);
        }
    }
}

//
// Update the window title from 'curFile' (the active tab's file path).
// Factored out of setCurrentFile() so onTabChanged() can refresh the
// title on a plain tab switch, without touching that tab's modified
// state or file path the way setCurrentFile() does.
//
void MainWindow::updateWindowTitle()
{
    QString shownName = curFile.isEmpty() ? tr("untitled.c") : strippedName(curFile);
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr(AMIGAED_VERSION_STRING)));
}

//
// Strip path from current file name
//
QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

//
// Create a new editor tab, configured exactly like AmigaED's original
// single, permanent QsciScintilla widget used to be (same font, margins,
// caret line, default lexer, EOL/indentation/tab/brace-matching settings,
// zoom level, context menu, and signal wiring). Adds it to tabWidget,
// makes it the active tab, and marks it as an empty "untitled.c" buffer.
// Returns the new editor so callers can load a file into it right away
// (see loadFile()/openFileInTab()).
//
QsciScintilla *MainWindow::newEditorTab()
{
    QsciScintilla *editor = new QsciScintilla;

    int index = tabWidget->addTab(editor, tr("untitled.c"));
    tabWidget->setCurrentIndex(index);   // triggers onTabChanged() -> textEdit = editor

    // One-time-per-instance setup that used to happen only once, directly
    // on the app's single shared QsciScintilla widget (see initializeGUI()
    // and createActions() prior to AmigaED v3.2):
    editor->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(editor, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showCustomContextMenue(const QPoint &)));
    connect(editor, SIGNAL(textChanged()), this, SLOT(documentWasModified()));
    connect(editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(showCurrendCursorPosition()));
    connect(editor, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
    connect(editor, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));

    editor->setEolMode(QsciScintilla::EolUnix);
    editor->setIndentationsUseTabs(true);
    editor->setIndentationWidth(4);
    editor->setTabWidth(4);
    editor->setAutoIndent(true);
    editor->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    editor->SendScintilla(editor->QsciScintilla::SCI_STYLESETCHARACTERSET, 1, QsciScintilla::SC_CHARSET_ANSI);
    editor->zoomTo(p_zoomLevel);

    // Same per-tab appearance setup the app always ran once at startup -
    // now applied to this specific new tab. All of these operate on
    // 'textEdit', which already points at 'editor' at this point (see
    // setCurrentIndex() above).
    initializeFont();
    initializeMargin();
    initializeCaretLine();
    initializeLexerCPP();      // default assumption: new/blank files are C/C++

    setCurrentFile("");        // mark this tab as an untitled, unsaved buffer

    return editor;
}

//
// Find an already-open tab for the given file, if any - so opening a
// file that's already open switches to it instead of opening a duplicate
// tab. Comparison uses QFileInfo, which is already platform-aware about
// case sensitivity (see the note in jumpToError()).
//
QsciScintilla *MainWindow::findEditorForFile(const QString &fileName) const
{
    if (fileName.isEmpty() || !tabWidget)
        return nullptr;

    QFileInfo target(fileName);
    for (int i = 0; i < tabWidget->count(); ++i)
    {
        QsciScintilla *editor = qobject_cast<QsciScintilla *>(tabWidget->widget(i));
        if (!editor)
            continue;
        QString path = editor->property("amigaed_filePath").toString();
        if (!path.isEmpty() && QFileInfo(path) == target)
            return editor;
    }
    return nullptr;
}

//
// If 'fileName' happens to be open in a tab, refresh that tab's content
// from disk - without switching to it or touching any other tab. Used
// after regenerateProjectMakefiles() rewrites a Makefile: those files are
// documented as "regenerated automatically" and never meant to be
// hand-edited, so an open tab showing one is silently kept in sync with
// what's actually on disk, exactly like openFileInTab() would show if
// the tab were freshly opened right now - rather than the pre-existing
// tab-reuse behaviour (see openFileInTab()) leaving it stuck on
// whatever content happened to be loaded before the regeneration.
//
void MainWindow::reloadEditorFromDiskIfOpen(const QString &fileName)
{
    QsciScintilla *editor = findEditorForFile(fileName);
    if (!editor)
        return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return;   // silent - this is a background refresh, not a user-initiated open

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Latin1);
    editor->setText(in.readAll());
    editor->setModified(false);
}

//
// Open 'fileName' in its own tab: switches to it if it's already open in
// one, otherwise creates a new tab and loads the file into it. Used by
// open(), openRecentFile() and jumpToError() alike.
//
void MainWindow::openFileInTab(const QString &fileName)
{
    QsciScintilla *existing = findEditorForFile(fileName);
    if (existing)
    {
        tabWidget->setCurrentWidget(existing);   // onTabChanged() does the rest
        return;
    }

    newEditorTab();       // creates + activates a fresh empty tab (defaults to the C/C++ lexer)
    loadFile(fileName);   // loads into 'textEdit', which now IS that new tab

    applyLexerForFileExtension(fileName);
}

//
// Auto-select a syntax lexer for a just-opened file, by suffix (or, for
// Makefiles, by filename) - covers every lexer with an unambiguous file
// extension in AmigaED's own model. Always shows the same "Syntax
// changed to ..." status message a manual View > Syntax choice would,
// for consistency.
//
// Deliberately NOT extended to Amiga Installer scripts or AmigaShell
// scripts: both are conventionally extension-less (see
// Project::typeForFile()'s "ext.isEmpty()" fallback for Installer
// scripts), which would make either guess wrong at least as often as
// right for any other plain extension-less text file (README, COPYING,
// ...) - picking a syntax for those still needs a manual View > Syntax
// choice, or their project-tree category for a file already added to a
// project.
//
void MainWindow::applyLexerForFileExtension(const QString &fileName)
{
    QFileInfo info(fileName);
    QString suffix = info.suffix();

    if (suffix.compare(QStringLiteral("guide"), Qt::CaseInsensitive) == 0)
    {
        initializeLexerAmigaGuide();
    }
    else if (suffix.compare(QStringLiteral("asm"), Qt::CaseInsensitive) == 0 ||
             suffix.compare(QStringLiteral("s"), Qt::CaseInsensitive) == 0)
    {
        initializeLexerM68kAsm();
    }
    else if (suffix.compare(QStringLiteral("pas"), Qt::CaseInsensitive) == 0)
    {
        initializeLexerPascal();
    }
    else if (info.fileName().startsWith(QStringLiteral("Makefile"), Qt::CaseInsensitive))
    {
        initializeLexerMakefile();
    }
    else if (suffix.compare(QStringLiteral("c"), Qt::CaseInsensitive) == 0 ||
             suffix.compare(QStringLiteral("cpp"), Qt::CaseInsensitive) == 0 ||
             suffix.compare(QStringLiteral("cc"), Qt::CaseInsensitive) == 0 ||
             suffix.compare(QStringLiteral("cxx"), Qt::CaseInsensitive) == 0 ||
             suffix.compare(QStringLiteral("h"), Qt::CaseInsensitive) == 0 ||
             suffix.compare(QStringLiteral("hpp"), Qt::CaseInsensitive) == 0 ||
             suffix.compare(QStringLiteral("hxx"), Qt::CaseInsensitive) == 0)
    {
        // Already newEditorTab()'s default for a fresh tab - called
        // again explicitly anyway so opening a .c/.h file always shows
        // the same "Syntax changed to ..." confirmation every other
        // recognized type gets, rather than silently relying on it
        // already being active.
        initializeLexerCPP();
    }
}

//
// React to the user switching tabs: point 'textEdit' at the newly active
// tab's editor and refresh everything that mirrors "the current file"
// (curFile, window title, modified-indicator, cursor position display).
// Does NOT touch any tab's actual content or modified state - a plain
// tab switch must never look like an edit.
//
void MainWindow::onTabChanged(int index)
{
    if (index < 0 || !tabWidget)
        return; // no tabs left - shouldn't normally happen, see onTabCloseRequested()

    QsciScintilla *editor = qobject_cast<QsciScintilla *>(tabWidget->widget(index));
    if (!editor)
        return;

    textEdit = editor;
    curFile = textEdit->property("amigaed_filePath").toString();

    updateWindowTitle();
    setWindowModified(textEdit->isModified());
    showCurrendCursorPosition();
}

//
// React to the user clicking a tab's close [x] button (or any other
// trigger of QTabWidget::tabCloseRequested()). Checks for unsaved changes
// in THAT specific tab (which might not be the currently active one),
// removes it, and - since the app should always have at least one editor
// open - creates a fresh empty tab if that was the last one left.
//
void MainWindow::onTabCloseRequested(int index)
{
    if (!tabWidget)
        return;

    QsciScintilla *editor = qobject_cast<QsciScintilla *>(tabWidget->widget(index));
    if (!editor)
        return;

    if (!maybeSave(editor))
        return; // user cancelled - keep the tab open

    tabWidget->removeTab(index);
    editor->deleteLater();

    if (tabWidget->count() == 0)
        newEditorTab();   // never leave the editor with zero tabs open
}

//
// Check every open tab for unsaved changes before quitting the app -
// maybeSave() alone only guards the currently active tab.
//
bool MainWindow::maybeSaveAll()
{
    if (!tabWidget)
        return true;

    for (int i = 0; i < tabWidget->count(); ++i)
    {
        QsciScintilla *editor = qobject_cast<QsciScintilla *>(tabWidget->widget(i));
        if (editor && !maybeSave(editor))
            return false;   // user cancelled - abort the whole quit
    }
    return true;
}

//
// react on toogle vbcc show options changed...
//
void MainWindow::actionToggleVbccDefaultOptsDialog()
{
    if(toggleVbccDefaultOptsAct->isChecked())
        p_show_vbcc_opts = true;
    else
        p_show_vbcc_opts = false;
}


//
// react on toogle gcc/g++ show options changed...
//
void MainWindow::actionToggleGccDefaultOptsDialog()
{
    if(toggleGccDefaultOptsAct->isChecked())
        p_show_gcc_opts = true;
    else
        p_show_gcc_opts = false;
}


//
// GUI Language (I18n): menu handlers just delegate to the shared helper.
//
void MainWindow::actionSetGuiLanguageEnglish()
{
    applyGuiLanguage("en");
}

void MainWindow::actionSetGuiLanguageGerman()
{
    applyGuiLanguage("de");
}

//
// Functions Browser visibility: functionsGroupBox lives as a plain widget
// inside mainSplitter (see createProjectPanel()/constructor) - a QSplitter
// handles a hidden child gracefully, simply reclaiming its space.
//
void MainWindow::actionShowFunctionsBrowser()
{
    functionsGroupBox->setVisible(true);
}

void MainWindow::actionHideFunctionsBrowser()
{
    functionsGroupBox->setVisible(false);
}

//
// Switch the running application's GUI language: (un)installs the
// QTranslator, persists the choice for the next program start, updates
// the menu's checked state and re-applies every translatable string via
// retranslateUi() - all without requiring a restart.
//
void MainWindow::applyGuiLanguage(const QString &langCode)
{
    if (p_guiTranslator)
    {
        qApp->removeTranslator(p_guiTranslator);
        delete p_guiTranslator;
        p_guiTranslator = nullptr;
    }

    if (langCode == "de")
    {
        p_guiTranslator = new QTranslator(this);
        if (p_guiTranslator->load(QLocale(QLocale::German), "amigaed", "_", ":/translations"))
        {
            qApp->installTranslator(p_guiTranslator);
        }
        else
        {
            qDebug() << "Could not load amigaed_de.qm - staying with English.";
            delete p_guiTranslator;
            p_guiTranslator = nullptr;
        }
    }

    p_guiLanguage = (p_guiTranslator) ? "de" : "en";

    QSettings settings("MB-SoftWorX", "Amiga Cross Editor");
    settings.setValue("MISC/DefaultGUILanguage", p_guiLanguage);

    // The very first call happens from readSettings() at the top of the
    // constructor, before createActions()/createMenus() have run - there is
    // no menu or retranslateUi() target yet. The installed translator alone
    // is enough: every tr() call from here on (during initial GUI build-up)
    // already resolves against it. On every later call (menu click, Prefs
    // dialog closed, "Reload settings"), update the menu's checked state and
    // re-apply every translatable string live.
    if (guiLanguageEnglishAct)
    {
        guiLanguageEnglishAct->setChecked(p_guiLanguage != "de");
        guiLanguageGermanAct->setChecked(p_guiLanguage == "de");
        retranslateUi();
    }
}


//
// jump to line #1
//
// BUG: Shortcut not responding! To be fixed...
//
void MainWindow::actionGotoTop()
{
    const int i = 1;

    // check if text is folded!
    QsciScintilla::FoldStyle state = static_cast<QsciScintilla::FoldStyle>((!textEdit->folding()) * 5);
    // if folded: unfold first!!
    if (state > 0)
    {
        textEdit->foldAll(false);
    }
    textEdit->setCursorPosition(i-1,0);
    textEdit->setCaretLineVisible(true);
}

//
// jump to last line in text
//
// BUG: Shortcut not responding! To be fixed...
//
void MainWindow::actionGotoBottom()
{
    const int max_lines = textEdit->lines();    // number of lines in text!
    // check if text is folded!
    QsciScintilla::FoldStyle state = static_cast<QsciScintilla::FoldStyle>((!textEdit->folding()) * 5);
    // if folded: unfold first!!
    if (state > 0)
    {
        textEdit->foldAll(false);
    }
    textEdit->setCursorPosition(max_lines, 0);
}

//
// Jump to line No. X...
//
void MainWindow::actionGoto_Line()
{
    int max = textEdit->lines(); // max. count of lines in code window
    bool ok;
    int i = QInputDialog::getInt(this, tr("Goto line"),
                                 tr("Line number:"), 1, 0, max, 1, &ok);
    if (ok)
    {
        // check if text is folded!
        QsciScintilla::FoldStyle state = static_cast<QsciScintilla::FoldStyle>((!textEdit->folding()) * 5);

        // if folded: unfold first!!
        if (state > 0)
        {
            textEdit->foldAll(false);
        }
        textEdit->setCursorPosition(i-1,0);
    }
}

//
// jump to matching brace: {...}, [...], (...)
//
// Rewritten (rev.96): the previous one-liner just called QScintilla's
// own moveToMatchingBrace(), which only reliably checks the character
// AT the caret position (i.e. immediately to its RIGHT) for a brace -
// Scintilla's underlying SCI_BRACEMATCH message matches the brace AT a
// given document position, not "whichever brace is nearest the caret".
// That silently failed to do anything for the very case the user
// described: caret sitting right AFTER a bracket (the bracket is then
// the character to the caret's LEFT, at position-1, not at its
// position). This version explicitly checks both sides of the caret -
// left first (matching "the cursor is right behind a bracket"), then
// right - so it works from either side and in either search direction
// (opening-to-closing and closing-to-opening are the same
// SCI_BRACEMATCH call either way). The caret always ends up right
// AFTER the matching bracket, whichever one that is.
//
void MainWindow::actionGoto_matching_brace()
{
    if (!textEdit)
        return;

    auto isBracket = [](char c) {
        return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']';
    };

    const long curPos = textEdit->SendScintilla(QsciScintillaBase::SCI_GETCURRENTPOS);

    // Check the character immediately to the LEFT of the caret first -
    // this is what "the cursor stands right behind a bracket" means -
    // then fall back to the character immediately to its RIGHT, so a
    // caret sitting right before an opening bracket (about to type past
    // it) still works too.
    long bracePos = -1;
    if (curPos > 0 && isBracket(static_cast<char>(textEdit->SendScintilla(QsciScintillaBase::SCI_GETCHARAT, curPos - 1))))
        bracePos = curPos - 1;
    else if (isBracket(static_cast<char>(textEdit->SendScintilla(QsciScintillaBase::SCI_GETCHARAT, curPos))))
        bracePos = curPos;

    if (bracePos < 0)
    {
        createStatusBarMessage(tr("No bracket next to the cursor."), 3000);
        return;
    }

    // Explicit casts needed here: passing a plain "0" as the unused
    // third (lParam) argument is genuinely ambiguous between this
    // overload's "long", the "void *", and the "const char *"
    // overloads (0 is a valid null-pointer-constant for either of the
    // latter two as well as a valid integer for the former) - the
    // compiler can't pick one on its own. Casting both arguments to
    // match the (unsigned long, long) overload exactly resolves it.
    const long matchPos = textEdit->SendScintilla(QsciScintillaBase::SCI_BRACEMATCH,
                                                   static_cast<unsigned long>(bracePos),
                                                   static_cast<long>(0));
    if (matchPos < 0)   // Scintilla returns INVALID_POSITION (-1) if the bracket has no partner
    {
        createStatusBarMessage(tr("No matching bracket found."), 3000);
        return;
    }

    // Land right AFTER the matching bracket, regardless of which
    // direction the jump went - matches the same "cursor sits right
    // behind the bracket" convention the search above started from.
    textEdit->SendScintilla(QsciScintillaBase::SCI_GOTOPOS, matchPos + 1);
    textEdit->setFocus();
}

//
// select a compiler to use: VBCC
//
void MainWindow::actionSelectCompilerVBCC()
{
    qDebug() << "VBCC selection called.";
    if(!(p_no_compilerbuttons))    // react on user prefs: show or hide compiler combo and -button
    {
        compilerCombo->setCurrentIndex(0);
    }
    p_defaultCompiler = 0;
    SelectCompiler(0);
}

//
// select a compiler to use: gcc
//
void MainWindow::actionSelectCompilerGCC()
{
    qDebug() << "GCC selection called.";
    if(!(p_no_compilerbuttons))    // react on user prefs: show or hide compiler combo and -button
    {
        compilerCombo->setCurrentIndex(1);
    }
    p_defaultCompiler = 1;
    SelectCompiler(1);
}

//
// select a compiler to use: VBCC
//
void MainWindow::actionSelectCompilerGPP()
{
    qDebug() << "g++ selection called.";
    if(!(p_no_compilerbuttons))    // react on user prefs: show or hide compiler combo and -button
    {
        compilerCombo->setCurrentIndex(2);
    }
    p_defaultCompiler = 2;
    SelectCompiler(2);
}

//
// Remove duplicate whitespace-separated tokens from 'args', keeping the
// first occurrence of each. Several sources of compiler/linker options
// can legitimately overlap (a toolchain's Prefs-driven baseline, its
// OS-1.3-specific extras, and a project's own saved "extra options" -
// especially since the "New Project" dialog now pre-fills exactly that
// baseline as a starting point and many users simply accept it as-is) -
// without this, the same switch could appear twice on the compile/link
// command line.
//
QString MainWindow::dedupTokens(const QString &args) const
{
    const QStringList tokens = args.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    QSet<QString> seen;
    QStringList kept;
    for (const QString &tok : tokens)
    {
        if (seen.contains(tok))
            continue;
        seen.insert(tok);
        kept << tok;
    }
    return kept.join(" ");
}

//
// Remove any token from 'text' that also appears in 'reference' - used to
// keep a Makefile's CFLAGS/LDFLAGS free of switches already supplied via
// CCARGS (both end up on the same compile/link command line), without
// folding CCARGS' own content into CFLAGS/LDFLAGS.
//
QString MainWindow::removeTokensAlsoIn(const QString &text, const QString &reference) const
{
    const QStringList refTokens = reference.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    const QSet<QString> refSet(refTokens.begin(), refTokens.end());

    QStringList kept;
    for (const QString &tok : text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts))
        if (!refSet.contains(tok))
            kept << tok;
    return kept.join(" ");
}

//
// Central lookup: given a compiler (0=VBCC, 1=GCC, 2=G++) and a target OS
// (0=OS 1.3, 1=OS 3.x), return that combination's configured compiler
// opts and linker opts (Prefs > GCC/VBCC tabs). Used for single-file
// compiles, the "New Project" Compiler/Linker Options prompts, and the
// generated Makefiles' CCARGS/LDFLAGS - a single source of truth so all
// three stay consistent with each other.
//
void MainWindow::getCompilerAndLinkerOptsForTarget(int compiler, int targetOS, QString &compilerOpts, QString &linkerOpts) const
{
    bool isOS13 = (targetOS == 0);

    switch (compiler)
    {
    case 0:   // VBCC
        compilerOpts = isOS13 ? p_compiler_vc13_call : p_compiler_vc30_call;
        linkerOpts = isOS13 ? p_compiler_vc13_linker_call : p_compiler_vc30_linker_call;
        break;
    case 1:   // GCC
        compilerOpts = isOS13 ? p_compiler_gcc13_call : p_compiler_gcc30_call;
        linkerOpts = isOS13 ? p_compiler_gcc13_linker_call : p_compiler_gcc30_linker_call;
        break;
    case 2:   // G++
        compilerOpts = isOS13 ? p_compiler_gpp13_call : p_compiler_gpp30_call;
        linkerOpts = isOS13 ? p_compiler_gpp13_linker_call : p_compiler_gpp30_linker_call;
        break;
    default:
        compilerOpts.clear();
        linkerOpts.clear();
        break;
    }
}

//
// Short, status-bar-friendly display name for a compiler index (0=VBCC,
// 1=GCC, 2=G++) - used to prefix build/compile status messages (e.g.
// "gcc: Building project ...") so it's clear at a glance which toolchain
// a message refers to.
//
QString MainWindow::compilerDisplayLabel(int compiler) const
{
    switch (compiler)
    {
    case 0: return QStringLiteral("vbcc");
    case 1: return QStringLiteral("gcc");
    case 2: return QStringLiteral("g++");
    default: return QString();
    }
}

//
// select a compiler to use (vbcc, gcc, g++)
//
void MainWindow::SelectCompiler(int index)
{
    if(p_mydebug)
    {
        qDebug() << "in SelectCompiler(index)";
        qDebug() << "index: " << index;
        qDebug() << "p_selected_compiler_args: " << p_selected_compiler_args;
        qDebug() << "\nCommand: " << p_defaultCompiler;
        qDebug() << "Arguments: " << p_compiler_call;

    }


    // Toggle statusbar combobox:
    if(!(p_no_compilerbuttons))    // react on user prefs: show or hide compiler combo and -button
    {
        this->compilerCombo->setCurrentIndex(index);
        switch(index)
        {
        case 0: // VBCC
            osCombo->setEnabled(true);
            p_defaultCompiler = 0;
            break;
        case 1: // GCC
            osCombo->setEnabled(true);
            p_defaultCompiler = 1;
            break;
        case 2: // G++
            osCombo->setEnabled(true);
            p_defaultCompiler = 2;
            break;
        }
    }

    switch(index)
    {
    // VBCC
    case 0:
    {
        QString compilerOpts, linkerOpts;
        getCompilerAndLinkerOptsForTarget(0, p_compiler_vc_default_target, compilerOpts, linkerOpts);
        p_selected_compiler_args = dedupTokens(compilerOpts + " " + linkerOpts);
        p_selected_compiler = p_compiler_vc;
        p_compiledFileSuffix = "_vc";
        // check selected menu item, uncheck others
        selectCompilerVBCCAct->setChecked(true);
        break;
    }
    // GCC
    case 1:
    {
        QString compilerOpts, linkerOpts;
        getCompilerAndLinkerOptsForTarget(1, p_compiler_vc_default_target, compilerOpts, linkerOpts);
        p_selected_compiler = p_compiler_gcc;
        p_selected_compiler_args = dedupTokens(compilerOpts + " " + linkerOpts);
        p_compiledFileSuffix = "_gcc";
        // check selected menu item, uncheck others
        selectCompilerGCCAct->setChecked(true);
        break;
    }
    // G++
    case 2:
    {
        QString compilerOpts, linkerOpts;
        getCompilerAndLinkerOptsForTarget(2, p_compiler_vc_default_target, compilerOpts, linkerOpts);
        p_selected_compiler = p_compiler_gpp;
        p_selected_compiler_args = dedupTokens(compilerOpts + " " + linkerOpts);
        p_compiledFileSuffix = "_g++";
        // check selected menu item, uncheck others
        selectCompilerGPPAct->setChecked(true);
        break;
    }
    }

    if(p_mydebug)
    {
        qDebug() << "in SelectCompiler(index) AFTER index selection";
        qDebug() << "index: " << index;
        qDebug() << "p_selected_compiler_args: " << p_selected_compiler_args;
        qDebug() << "\nCommand: " << p_defaultCompiler;
        qDebug() << "Arguments: " << p_compiler_call;

    }
}

//
// Close Output console in Splitter...
//
void MainWindow::actionCloseOutputConsole()
{
    outputGroup->hide();
}

//
// OpenOutput console in Splitter...
//
void MainWindow::actionShowOutputConsole()
{
    outputGroup->show();
    jumpCompilerWarnings();
}

//
// Compile current file
// CHANGE THIS according to your compiler and opts!
//
int MainWindow::actionCompile()
{
    QString new_compiler_args;

    // check if we have a valid compiler to call:
    if(p_selected_compiler.isEmpty())
    {
        if(p_mydebug)
            qDebug() << "in actionCompile:\nNo compiler selected in prefs!\n";

        (void)QMessageBox::critical(this, tr(AMIGAED_VERSION_STRING),
                                     tr("There is a problem with your compiler presets!\n"
                                        "Please set preferences according to your compiler locations and default options.\n\n"
                                        "You need to restart Amiga Cross Editor afterwards!"),
                                     QMessageBox::Ok);
        if(selectCompilerGCCAct->isChecked() || selectCompilerGPPAct->isChecked())
            actionPrefsDialog(1);
        else
            actionPrefsDialog(2);

        return 1;
    }

    // check if we have valid compiler arguments in prefs:
    if(p_compiler_gcc13_call.isEmpty() || p_compiler_gcc30_call.isEmpty() ||
       p_compiler_gpp13_call.isEmpty() || p_compiler_gpp30_call.isEmpty() ||
       p_compiler_vc13_call.isEmpty() || p_compiler_vc30_call.isEmpty())
    {
        if(p_mydebug)
            qDebug() << "in actionCompile:\nNo valid arguments selected in prefs!\n";

        (void)QMessageBox::critical(this, tr(AMIGAED_VERSION_STRING),
                                     tr("There is a problem with your compiler presets!\n"
                                        "Please set preferences according to your compiler locations and default options.\n\n"
                                        "You need to restart Amiga Cross Editor afterwards!"),
                                     QMessageBox::Ok);
        if(selectCompilerGCCAct->isChecked() || selectCompilerGPPAct->isChecked())
            actionPrefsDialog(1);
        else
            actionPrefsDialog(2);

        return 1;
    }

    // Don't start compiler on an empty source!
    if(!(textEdit->text().isEmpty()))
    {
        QString temp_compiler_call, mbox_title;
        QFileInfo file(curFile);

        // set approbriate title for QInputDialog:
        if(selectCompilerVBCCAct->isChecked())
        {
            if(p_mydebug)
                qDebug() << "Extension: " << file.suffix();

            if((file.suffix() == "cpp") || (file.suffix() == "CPP"))
            {
                if(p_mydebug)
                    qDebug() << "vbcc ERROR C++";

                selectCompilerVBCCAct->setChecked(false);
                selectCompilerGPPAct->setChecked(true);
                if(!(p_no_compilerbuttons))    // react on user prefs: show or hide compiler combo and -button
                {
                    compilerCombo->setCurrentIndex(2);
                }
                else
                {
                    if(selectCompilerGPPAct->isChecked())
                        SelectCompiler(2);
                }

                // give a user warning
                (void)QMessageBox::warning(this,
                                            AMIGAED_VERSION_STRING, "VBCC does <i><b>NOT</b> permit</i> to compile <b><i>C++ sources!</i></b><br> "
                                            "Compiler was set to <b>GNU g++</b> instead.<br>"
                                            "<br>This usually makes more sense, ya know?!",
                                            QMessageBox::Ok);

            } else {mbox_title = "vbcc";}
        }

        if(selectCompilerGCCAct->isChecked())
        {
            if((file.suffix() == "cpp") || (file.suffix() == "CPP"))
            {
                if(p_mydebug)
                    qDebug() << "gcc ERROR C++";

                selectCompilerGCCAct->setChecked(false);
                selectCompilerGPPAct->setChecked(true);
                if(!(p_no_compilerbuttons))    // react on user prefs: show or hide compiler combo and -button
                {
                    compilerCombo->setCurrentIndex(2);
                }
                else
                {
                    if(selectCompilerGPPAct->isChecked())
                        SelectCompiler(2);
                }


                // give a user warning
                (void)QMessageBox::warning(this,
                                            AMIGAED_VERSION_STRING, "GCC does <i><b>NOT</b> permit</i> to compile <b><i>C++ sources!</i></b><br> "
                                            "Compiler was set to <b>GNU g++</b> instead.<br>"
                                            "<br>This usually makes more sense, ya know?!",
                                            QMessageBox::Ok);

            }
            else
            {
                mbox_title = "m68k-amigaos-gcc";
            }
        }


        if(selectCompilerGPPAct->isChecked())
            mbox_title = "m68k-amigaos-g++";


        // 'text' holds selected compiler args, 'ok' determines if to use them
        QString text = p_selected_compiler_args;
        bool ok;

        if(p_mydebug)
            qDebug() << "START: Proc started " << p_proc_is_started << " times.";

        // Now let's check if user wants to see options dialog(s) for gcc, g++ and vc...
        if(selectCompilerGCCAct->isChecked() || selectCompilerGPPAct->isChecked())          // Are gcc or g++ selected default compiler...?
        {
            if(p_show_gcc_opts)
            {
                text = QInputDialog::getText(this, mbox_title,
                                             tr("Compiler Options:"), QLineEdit::Normal,
                                             p_selected_compiler_args, &ok);
            }
            else
            {
                ok = true;
                text = p_selected_compiler_args;
            }
        }
        else    // ...NO? So vc must be the selected default compiler!
        {
            if(p_show_vbcc_opts)
            {
                text = QInputDialog::getText(this, mbox_title,
                                             tr("Compiler Options:"), QLineEdit::Normal,
                                             p_selected_compiler_args, &ok);
            }
            else
            {
                ok = true;
                text = p_selected_compiler_args;
            }
        }   // END checking for show/hide default compiler opts dialog

        if (ok && !text.isEmpty())
        {
            new_compiler_args = text;   // store args if changed!
            qDebug() << "text in line 1850: " << text;
            save();
            QString outName = QFileInfo(curFile).baseName();
            QString outPath = QFileInfo(curFile).absolutePath();

            // construct path and name of compiled file for file checking:
            p_compiledFile = outPath + QDir::separator() + outName + p_compiledFileSuffix;

            if(p_mydebug)
                qDebug() << "compiled file: " << p_compiledFile;

            temp_compiler_call = p_selected_compiler_args;  // store compiler parameters temporarily
            text.append(" ");   // add one space to separate arguments!!
            text.append(curFile + " -o " + outPath + QDir::separator() + outName + p_compiledFileSuffix);        // add output file name

            if(p_mydebug)
                qDebug() << "Text not empty: " << text;

            p_selected_compiler_args = text;

            if(!(p_console_on_fail))
            {
                // make output window visible:
                actionShowOutputConsole();
            }

            if(p_mydebug)
            {
                qDebug() << "in actionCompile()";

                qDebug() << "p_selected_compiler_args: " << p_selected_compiler_args;
                qDebug() << "text: " << text;

            }

            //
            // put REAL compiler start HERE!
            // (uses p_compiler and p_compiler_call in mainwindow.h as default options)
            //
            timerCompile.start();
            startCompiler();
            nMilliseconds = timerCompile.elapsed();

            if(p_mydebug)
                qDebug() << "Compilation took " << nMilliseconds << "Milliseconds";


            // afterwards, reset everything to its defaults!
            text.clear();
            p_selected_compiler_args.clear();
            p_selected_compiler_args = temp_compiler_call;
        }
        else
        {
            text.append(curFile);
            p_compiler_call = text;

            if(p_mydebug)
            {
                qDebug() << "Text: " << text;
                qDebug() << "Compiler call: " << p_compiler_call;
            }
        }

        text.clear();
    }
    else
    {
        // give a user warning
        (void)QMessageBox::critical(this,
                                     AMIGAED_VERSION_STRING, "It makes <i><b>no sense</b></i> to compile <b><i>empty source files!</i></b><br> "
                                     "Compilation was <b>terminated</b> instead.<br>"
                                     "<br>This usually makes more sense, ya know?!",
                                     QMessageBox::Ok);
    }

    // keep changes if any...
    p_selected_compiler_args = new_compiler_args;

    return 0;
}

//
// Show or hide debugging output
// Value is stored in p_mydebug
//
void MainWindow::actionShowDebug()
{
    if(showDebugInfoAct->isChecked())
    {
        p_mydebug = true;
    }
    else
        p_mydebug = false;

    if(p_mydebug)
        qDebug() << "p_mydebug = " << p_mydebug;
}

//
// Reset editor font size to normal (QAction::triggered() carries no
// argument, so zoomTo(0) needs this thin wrapper slot)
//
void MainWindow::actionResetFontSize()
{
    textEdit->zoomTo(0);
}

//
// Thin forwarding slots for Cut/Copy/Paste/Zoom actions (see createActions()
// for why these can no longer connect straight to a single, fixed
// QsciScintilla widget's slots since AmigaED v3.2's tabbed editor: 'textEdit'
// always points at whichever tab is currently active, and can change
// between the time an action is created and the time it's triggered.
//
// Undo/redo history itself is QScintilla's own (Scintilla's built-in undo
// buffer, one per document/tab) - it isn't capped to any fixed number of
// steps by AmigaED, so "at least 10 steps forward and back" is comfortably
// covered without any extra bookkeeping here.
//
void MainWindow::actionUndo()
{
    if (textEdit)
        textEdit->undo();
}

void MainWindow::actionRedo()
{
    if (textEdit)
        textEdit->redo();
}

void MainWindow::actionCut()
{
    if(textEdit)
        textEdit->cut();
}

void MainWindow::actionCopy()
{
    if(textEdit)
        textEdit->copy();
}

void MainWindow::actionPaste()
{
    if(textEdit)
        textEdit->paste();
}

void MainWindow::actionZoomIn()
{
    if(textEdit)
        textEdit->zoomIn();
}

void MainWindow::actionZoomOut()
{
    if(textEdit)
        textEdit->zoomOut();
}

//
// Show or hide indentaation guides
//
void MainWindow::actionShowIndentationGuides()
{
    if(showIndentationGuidesAct->isChecked())
    {
        textEdit->setIndentationGuides(true);
    }
    else
    {
        textEdit->setIndentationGuides(false);
    }
}

/* ------------ Begin insertMenue Actions -----------------*/
//
// Insert #include <file.h>
//
void MainWindow::actionInsertInclude()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert our text:
    QString insertText = "#include    <some_header.h>";
    textEdit->insert(insertText);

    // Move the caret to the end of what we just inserted, generate a
    // line feed there, and land on the fresh new line - rather than
    // leaving the caret in the middle of "<some_header.h>" as before,
    // this puts the user straight into position to keep typing.
    textEdit->setCursorPosition(line, index + insertText.length());
    textEdit->insert("\n");
    textEdit->setCursorPosition(line + 1, 0);
}

//
// this will insert some of the #includes mostly used for Amiga app development
//
void MainWindow::actionInsertAmigaIncludes()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...
    // 'index' (the starting column) is no longer needed afterwards now
    // that the caret ends up at column 0 on a fresh line - but
    // getCursorPosition() still requires an out-parameter for it.
    Q_UNUSED(index);

    // we need some #includes, so let's insert them:
    textEdit->insertAt("\n", ++line, 0);  // insert empty line!
    textEdit->insertAt("/* ------------- OS 3.x  INCLUDE FILES ---------------------------------- */\n", ++line, 0);
    textEdit->insertAt("/* --- typical standard headers --- */\n", ++line, 0);
    textEdit->insertAt("#include\t<stdio.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<stdlib.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<string.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<ctype.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<stdarg.h>\n", ++line, 0);
    textEdit->insertAt("\n", ++line, 0);  // insert empty line!
    textEdit->insertAt("/* --- Amiga stuff ---------------- */\n", ++line, 0);
    textEdit->insertAt("#include\t<exec/exec.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<dos/dos.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<dos/dostags.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<dos/dosextens.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<dos/datetime.h>\n", ++line, 0);
    textEdit->insertAt("\n", ++line, 0);  // insert empty line!
    textEdit->insertAt("#include\t<graphics/gfx.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<graphics/gfxmacros.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<graphics/layers.h>\n", ++line, 0);
    textEdit->insertAt("\n", ++line, 0);  // insert empty line!
    textEdit->insertAt("#include\t<intuition/intuition.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<intuition/intuitionbase.h>\n", ++line, 0);
    textEdit->insertAt("\n", ++line, 0);  // insert empty line!
    textEdit->insertAt("#include\t<workbench/workbench.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<workbench/startup.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<workbench/icon.h>\n", ++line, 0);
    textEdit->insertAt("\n", ++line, 0);  // insert empty line!
    textEdit->insertAt("#include\t<datatypes/pictureclass.h>\n", ++line, 0);
    textEdit->insertAt("\n", ++line, 0);  // insert empty line!
    textEdit->insertAt("#include\t<libraries/asl.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<libraries/commodities.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<libraries/gadtools.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<libraries/iffparse.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<libraries/locale.h>\n", ++line, 0);
    textEdit->insertAt("\n", ++line, 0);  // insert empty line!
    textEdit->insertAt("#include\t<rexx/rxslib.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<rexx/storage.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<rexx/errors.h>\n", ++line, 0);
    textEdit->insertAt("\n", ++line, 0);  // insert empty line!
    textEdit->insertAt("#include\t<utility/hooks.h>\n", ++line, 0);
    textEdit->insertAt("\n", ++line, 0);  // insert empty line!
    textEdit->insertAt("/* --- protos ----------------- */\n", ++line, 0);
    textEdit->insertAt("#if !defined(__MAXON__)\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/asl.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/commodities.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/datatypes.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/diskfont.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/dos.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/exec.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/gadtools.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/graphics.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/icon.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/iffparse.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/intuition.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/layers.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/locale.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/rexxsyslib.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/utility.h>\n", ++line, 0);
    textEdit->insertAt("#include\t<proto/wb.h>\n", ++line, 0);
    textEdit->insertAt("#endif\n", ++line, 0);

    // finally, move caret to the new line the block's trailing "\n"
    // already created, right after everything we just inserted (rather
    // than back into the middle of the block, as before).
    textEdit->setCursorPosition(line + 1, 0);
}

//
// Insert #define SOME_VALUE 0
//
void MainWindow::actionInsertDefine()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert our text:
    textEdit->insert("#define\tSOME_VALUE\t0\n");
    // finally, move caret to next line.
    textEdit->setCursorPosition(line + 1, index);
    qDebug() << "leaving actionInsertDefine now...";
}

//
// Insert #ifdef ... #endif
//
void MainWindow::actionInsertIfdef()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("\n");
    // next, we need to continue printing at a certain location:
    textEdit->insertAt("#ifdef __SOME_DEFINITION__\n", ++line, 0);
    textEdit->insertAt("\t/* some_action */\n", ++line, 0);
    textEdit->insertAt("\t\n", ++line, 0);
    textEdit->insertAt("#endif\n", ++line, 0);

    // finally, we set our caret to the next following empty line!
    textEdit->setCursorPosition(line + 1, index);
}

//
// Insert #if defined(..) ... #elif defined(...) ... #endif
//
void MainWindow::actionInsertIfdefined()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("\n");
    // next, we need to continue printing at a certain location:
    textEdit->insertAt("#if defined(__SOME_DEFINITION__)\n", ++line, 0);
    textEdit->insertAt("\t/* some_action */\n", ++line, 0);
    textEdit->insertAt("#elif defined(__SOME_OTHER_DEFINITION__)\n", ++line, 0);
    textEdit->insertAt("\t/* some_other_action */\n", ++line, 0);
    textEdit->insertAt("#else\n", ++line, 0);
    textEdit->insertAt("\t/* nothing matched... */\n", ++line, 0);
    textEdit->insertAt("\t\n", ++line, 0);
    textEdit->insertAt("#endif\n", ++line, 0);

    // finally, we set our caret to the next following empty line!
    textEdit->setCursorPosition(line + 1, index);
}

//
// Inserts a compiler check via #if defined(..) ... #elif defined(...) ...#else(...) ... #endif
//
void MainWindow::actionInsertIfdefinedCompiler()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("\n");
    // next, we need to continue printing at a certain location:

    // STR_HELPER/STR stringize a macro's VALUE (rather than its name) at
    // preprocessing time via the standard two-level "#" trick - needed
    // below so __GNUC__/__GNUC_MINOR__/__GNUC_PATCHLEVEL__'s numeric
    // values can be folded into a plain string constant (via adjacent
    // string literal concatenation) instead of a printf() format string.
    textEdit->insertAt("#define STR_HELPER(x) #x\n", ++line, 0);
    textEdit->insertAt("#define STR(x) STR_HELPER(x)\n", ++line, 0);
    textEdit->insertAt("\n", ++line, 0);
    textEdit->insertAt("#if defined(__STORM__)\n", ++line, 0);
    textEdit->insertAt("\t/* Compiler is StormC3 */\n", ++line, 0);
    textEdit->insertAt("\tconst char *compiler_string = \"compiled with StormC3.\\n\\n\";\n", ++line, 0);
    textEdit->insertAt("#elif defined(__STORMGCC__)\n", ++line, 0);
    textEdit->insertAt("\t/* Compiler is StormGCC4 */\n", ++line, 0);
    textEdit->insertAt("\tconst char *compiler_string = \"compiled with GNU gcc, StormC4 flavour.\\n\\n\";\n", ++line, 0);
    textEdit->insertAt("#elif defined(__MAXON__)\n", ++line, 0);
    textEdit->insertAt("\t/* Compiler is Maxon/HiSoft C++ */\n", ++line, 0);
    textEdit->insertAt("\tconst char *compiler_string = \"compiled with Maxon/HiSoft C++.\\n\\n\";\n", ++line, 0);
    textEdit->insertAt("#elif defined(__GNUC__)\n", ++line, 0);
    textEdit->insertAt("\t/* Compiler is gcc */\n", ++line, 0);
    textEdit->insertAt("\tconst char *compiler_string = \"compiled with GNU gcc v\" STR(__GNUC__) \".\" STR(__GNUC_MINOR__) \" Patchlevel \" STR(__GNUC_PATCHLEVEL__) \".\\n\"\n", ++line, 0);
    textEdit->insertAt("#if (__GNUC__ > 6) || (__GNUC__ == 6 && __GNUC_MINOR__ >= 5)\n", ++line, 0);
    textEdit->insertAt("\t\"Probably Beppo's amiga-gcc or one of its forks.\\n\"\n", ++line, 0);
    textEdit->insertAt("#endif\n", ++line, 0);
    textEdit->insertAt("\t\"\\n\";\n", ++line, 0);
    textEdit->insertAt("#elif defined(__VBCC__)\n", ++line, 0);
    textEdit->insertAt("\t/* Compiler is vbcc */\n", ++line, 0);
    textEdit->insertAt("\tconst char *compiler_string = \"compiled with vbcc.\\n\\n\";\n", ++line, 0);
    textEdit->insertAt("#elif defined(__SASC)\n", ++line, 0);
    textEdit->insertAt("\t/* Compiler is SAS/C */\n", ++line, 0);
    textEdit->insertAt("\tconst char *compiler_string = \"compiled with SAS/C.\\n\\n\";\n", ++line, 0);
    textEdit->insertAt("#elif defined(LATTICE)\n", ++line, 0);
    textEdit->insertAt("\t/* Compiler is Lattice C */\n", ++line, 0);
    textEdit->insertAt("\tconst char *compiler_string = \"compiled with Lattice C.\\n\\n\";\n", ++line, 0);
    textEdit->insertAt("#elif defined(AZTEC_C)\n", ++line, 0);
    textEdit->insertAt("\t/* Compiler is Aztec C */\n", ++line, 0);
    textEdit->insertAt("\tconst char *compiler_string = \"compiled with Manx Aztec C.\\n\\n\";\n", ++line, 0);
    textEdit->insertAt("#elif defined(_DCC)\n", ++line, 0);
    textEdit->insertAt("\t/* Compiler is dice */\n", ++line, 0);
    textEdit->insertAt("\tconst char *compiler_string = \"compiled with dice.\\n\\n\";\n", ++line, 0);
    textEdit->insertAt("#else\n", ++line, 0);
    textEdit->insertAt("\t/* Compiler not identified */\n", ++line, 0);
    textEdit->insertAt("\tconst char *compiler_string = \"Compiler was not identified.\\n\\n\";\n", ++line, 0);
    textEdit->insertAt("#endif\n", ++line, 0);

    // finally, we set our caret to the next following empty line!
    textEdit->setCursorPosition(line + 1, index);
}

//
// Insert #ifndef ... #endif
//
void MainWindow::actionInsertIfndef()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("\n");
    // next, we need to continue printing at a certain location:
    textEdit->insertAt("#ifndef __SOME_DEFINITION__\n", ++line, 0);
    textEdit->insertAt("\t/* some_action */\n", ++line, 0);
    textEdit->insertAt("\t\n", ++line, 0);
    textEdit->insertAt("#endif\n", ++line, 0);

    // finally, we set our caret to the next following empty line!
    textEdit->setCursorPosition(line + 1, index);
}

//
// Insert OpenLibrary("some.library", 0L)
//
void MainWindow::actionInsertOpenLibrary()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("\n");
    // next, we need to continue printing at a certain location:
    textEdit->insertAt("if( !(DummyBase = (struct DummyBase *) OpenLibrary(\"dummy.library\", 0L)) )\n", ++line, 0);
    textEdit->insertAt("{\n", ++line, 0);
    textEdit->insertAt("\tPutStr(\"Error: dummy.library is missing.\\n\");\n", ++line, 0);
    textEdit->insertAt("\treturn 20;\n", ++line, 0);
    textEdit->insertAt("}\n", ++line, 0);

    // finally, we set our caret to the next following empty line!
    textEdit->setCursorPosition(line + 1, index);
}

//
// Insert CloseLibrary("some.library")
//
void MainWindow::actionInsertCloseLibrary()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("\n");
    // next, we need to continue printing at a certain location:
    textEdit->insertAt("if (DummyBase) CloseLibrary( (struct Library *)DummyBase );\n", ++line, 0);

    // finally, we set our caret to the next following empty line!
    textEdit->setCursorPosition(line + 1, index);
}

//
// Insert if(...){...}
//
void MainWindow::actionInsertIf()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("\n");
    // next, we need to continue printing at a certain location:
    textEdit->insertAt("if( condition )\n", ++line, 0);
    textEdit->insertAt("{\n", ++line, 0);
    textEdit->insertAt("\t/* some_action */\n", ++line, 0);
    textEdit->insertAt("\t\n", ++line, 0);
    textEdit->insertAt("}\n", ++line, 0);

    // finally, we set our caret to the next following empty line!
    textEdit->setCursorPosition(line + 1, index);
}

//
// Insert if(...){...} else{..}
//
void MainWindow::actionInsertIfElse()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("\n");
    // next, we need to continue printing at a certain location:
    textEdit->insertAt("if( condition )\n", ++line, 0);
    textEdit->insertAt("{\n", ++line, 0);
    textEdit->insertAt("\t/* some_action */\n", ++line, 0);
    textEdit->insertAt("\t\n", ++line, 0);
    textEdit->insertAt("}\n", ++line, 0);
    textEdit->insertAt("else\n", ++line, 0);
    textEdit->insertAt("{\n", ++line, 0);
    textEdit->insertAt("\t/* some_other_action */\n", ++line, 0);
    textEdit->insertAt("\t\n", ++line, 0);
    textEdit->insertAt("}\n", ++line, 0);

    // finally, we set our caret to the next following empty line!
    textEdit->setCursorPosition(line + 1, index);
}

//
// Insert while(...){...}
//
void MainWindow::actionInsertWhile()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("\n");
    // next, we need to continue printing at a certain location:
    textEdit->insertAt("int var = 0;\n", ++line, 0);
    textEdit->insertAt("while( condition )\n", ++line, 0);
    textEdit->insertAt("{\n", ++line, 0);
    textEdit->insertAt("\t/* some_action */\n", ++line, 0);
    textEdit->insertAt("\t\n", ++line, 0);
    textEdit->insertAt("}\n", ++line, 0);

    // finally, we set our caret to the next following empty line!
    textEdit->setCursorPosition(line + 1, index);
}

//
// Insert for(...){...} loop
//
void MainWindow::actionInsertForLoop()
{
    qDebug() << "in while do";
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("\n");
    // next, we need to continue printing at a certain location:
    textEdit->insertAt("int var = 0;\n", ++line, 0);
    textEdit->insertAt("for( var = 0; var < 5; var++ )\n", ++line, 0);
    textEdit->insertAt("{\n", ++line, 0);
    textEdit->insertAt("\t/* some_action */\n", ++line, 0);
    textEdit->insertAt("\t\n", ++line, 0);
    textEdit->insertAt("}\n", ++line, 0);

    // finally, we set our caret to the next following empty line!
    textEdit->setCursorPosition(line + 1, index);
}

//
// Insert do{...}while(...)
//
void MainWindow::actionInsertDoWhile()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insertAt("\n", line, index);
    // next, we need to continue printing at a certain location:
    textEdit->insertAt("int var = 0;\r", ++line, index);
    textEdit->insertAt("do\r", ++line, index);
    textEdit->insertAt("{\r", ++line, index);
    textEdit->insertAt("\t/* some_action */\n", ++line, index);
    textEdit->insertAt("\t\n", ++line, index);
    textEdit->insertAt("} while( condition );\n", ++line, index);

    // finally, we set our caret to the next following empty line!
    textEdit->setCursorPosition(line + 1, index);
}

//
// Insert switch(...){select case...break}
//
void MainWindow::actionInsertSwitch()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("\n");
    // next, we need to continue printing at a certain location:
    textEdit->insertAt("switch( condition )\n", ++line, 0);
    textEdit->insertAt("{\n", ++line, 0);
    textEdit->insertAt("\tcase dummy:\n", ++line, 0);
    textEdit->insertAt("\t\tbreak;\n", ++line, 0);
    textEdit->insertAt("}\n", ++line, 0);

    // finally, we set our caret to the next following empty line!
    textEdit->setCursorPosition(line + 1, index);
}

//
// Insert int main(int argc, char *argv[]){...}
//
void MainWindow::actionInsertMain()
{
    // is there allready a main() function in this file?
    if(!(p_main_set))
    {
        // we need the caret's ("cursor") recent position stored as a starting point for insertion!
        int line, index;
        textEdit->getCursorPosition(&line, &index); // get the position...

        // ...now insert the first line of text!
        textEdit->insert("\n");
        // next, we need to continue printing at a certain location:
        textEdit->insertAt("/*************************\n", ++line, 0);
        textEdit->insertAt(" **	main() function    **\n", ++line, 0);
        textEdit->insertAt(" ************************/\n", ++line, 0);
        textEdit->insertAt("int main(int argc, char* argv[])\n", ++line, 0);
        textEdit->insertAt("{\n", ++line, 0);
        textEdit->insertAt("\t/* TODO: Write your code! */\n", ++line, 0);
        textEdit->insertAt("\tprintf(\"Now produce something usefull!\\n\");\n", ++line, 0);
        textEdit->insertAt("\n\n\treturn(0);\n}\n", ++line, 0);

        // finally, we set our caret to a new line right after the function
        // (previously landed back inside the function body).
        textEdit->setCursorPosition(line + 1, index);
        p_main_set = true;
    }
    else
    {
        (void)QMessageBox::information(this,
                                        AMIGAED_VERSION_STRING, "It seems there is allready a <i><b>main() </b>function</i> in this document!<br> "
                                        "It makes absolutely <b>no sense</b> to add another one."
                                        "<br>Insertion will be cancelled, ya know?!",
                                        QMessageBox::Ok);

    }

}

//
// Insert C function skelleton
//
void MainWindow::actionInsertFunction()
{
    qDebug() << "in Function";

    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("\n");
    // next, we need to continue printing at a certain location:

    // first, let's define a function prototype:
    textEdit->insertAt("/*\n", ++line, 0);
    textEdit->insertAt(" * -- Function prototype --\n", ++line, 0);
    textEdit->insertAt(" * EDIT, then cut & paste BEFORE main() !\n", ++line, 0);
    textEdit->insertAt(" */\n", ++line, 0);
    textEdit->insertAt("int some_function(int arg1, int arg2);\n", ++line, 0);
    textEdit->insertAt("\n", ++line, 0);
    // now let's create the function...
    textEdit->insertAt("/*\n", ++line, 0);
    textEdit->insertAt(" *\tName:\tint some_function(int arg1, int arg2)\n", ++line, 0);
    textEdit->insertAt(" *\tPurpose:\tdo something usefull...\n", ++line, 0);
    textEdit->insertAt(" */\n", ++line, 0);
    textEdit->insertAt("int some_function(int arg1, int arg2)\n", ++line, 0);
    textEdit->insertAt("{\n", ++line, 0);
    textEdit->insertAt("\t/* TODO: Write your code! */\n", ++line, 0);
    textEdit->insertAt("\tprintf(\"Now let your function do some work...\\n\");\n", ++line, 0);
    textEdit->insertAt("\n\treturn(0);\n}\n", ++line, 0);

    // finally, we set our caret to a new line right after the function
    // (previously landed back inside the function body).
    textEdit->setCursorPosition(line + 1, index);
}

//
// Insert C enumeration
//
void MainWindow::actionInsertEnum()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("\n");
    // next, we need to continue printing at a certain location:
    textEdit->insertAt("enum SomeEnum\n", ++line, 0);
    textEdit->insertAt("{\n", ++line, 0);
    textEdit->insertAt("\tENUM_VALUE_ONE,\n", ++line, 0);
    textEdit->insertAt("\tENUM_VALUE_TWO,\n", ++line, 0);
    textEdit->insertAt("\tENUM_VALUE_THREE\n", ++line, 0);
    textEdit->insertAt("};\n", ++line, 0);

    // finally, we set our caret to the next following empty line!
    textEdit->setCursorPosition(line + 1, index);
}

//
// Insert if(myDebug){...} console debugging block
//
void MainWindow::actionInsertConsoleDebugMessage()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("\n");
    // Every line of the block starts at 'index' - the cursor's own
    // original column - not a hardcoded 0: confirmed the whole block
    // always landed flush against the start of the line regardless of
    // where the cursor actually was, breaking the visual indentation of
    // whatever code it was inserted into (e.g. inside an already-indented
    // function body).
    textEdit->insertAt("if (myDebug)\n", ++line, index);
    textEdit->insertAt("{\n", ++line, index);
    textEdit->insertAt("\t/* --- Insert debugging messages here: --- */\n", ++line, index);
    textEdit->insertAt("\t\n", ++line, index);
    textEdit->insertAt("}\n", ++line, index);

    // unlike most other insertMenue templates, the caret lands INSIDE
    // the block - on the blank (indented) line right after the comment,
    // ready for the user to start typing debug output calls - rather
    // than after the whole block. 'line' was last incremented for the
    // "}\n" insertion above, so the blank line sits one line before it;
    // column 'index + 1' lands right after that line's own leading tab
    // (itself right after the 'index' columns of base indentation).
    textEdit->setCursorPosition(line - 1, index + 1);
}

//
// Comment/Uncomment Block: toggles a "// " line-comment prefix on every
// line of the current selection (or just the current line, if nothing
// is selected).
//
// Detection: the block counts as "already commented" if every non-blank
// line within it already starts with "//" - in which case the prefix
// ("// " if present, else just "//") is removed from each line instead.
// Blank/whitespace-only lines are ignored when deciding which way to
// toggle (so a comment block with a blank line in the middle doesn't
// force "add comments" mode), but are otherwise left untouched either
// way - there's nothing meaningful to comment or uncomment on an empty
// line.
//
void MainWindow::actionToggleCommentBlock()
{
    if (!textEdit)
        return;

    int lineFrom, indexFrom, lineTo, indexTo;
    textEdit->getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);
    const bool hadSelection = (lineFrom != -1);

    if (!hadSelection)
    {
        // Nothing selected - act on just the current line.
        textEdit->getCursorPosition(&lineFrom, &indexFrom);
        lineTo = lineFrom;
    }
    else if (indexTo == 0 && lineTo > lineFrom)
    {
        // The selection's last line has no characters actually selected
        // on it (the drag ended right at its very start) - exclude it,
        // matching the common editor convention for line-based commands
        // like this one, so selecting up to the start of the following
        // line doesn't unexpectedly comment that line too.
        --lineTo;
    }

    // First pass: does EVERY non-blank line in range already start with
    // "//"? That decides whether this run comments or uncomments.
    bool allCommented = true;
    bool sawNonBlankLine = false;
    for (int line = lineFrom; line <= lineTo; ++line)
    {
        const QString trimmed = textEdit->text(line).trimmed();
        if (trimmed.isEmpty())
            continue;
        sawNonBlankLine = true;
        if (!trimmed.startsWith(QStringLiteral("//")))
        {
            allCommented = false;
            break;
        }
    }
    const bool shouldUncomment = sawNonBlankLine && allCommented;

    // Second pass: actually apply it. Grouped into a single undo action
    // so Ctrl+Z undoes the whole block at once, not line by line.
    textEdit->beginUndoAction();
    for (int line = lineFrom; line <= lineTo; ++line)
    {
        const QString lineText = textEdit->text(line);
        if (shouldUncomment)
        {
            if (lineText.startsWith(QStringLiteral("// ")))
            {
                textEdit->setSelection(line, 0, line, 3);
                textEdit->removeSelectedText();
            }
            else if (lineText.startsWith(QStringLiteral("//")))
            {
                textEdit->setSelection(line, 0, line, 2);
                textEdit->removeSelectedText();
            }
            // else: this particular line doesn't start with "//" (e.g.
            // it's blank) - nothing to remove, leave it as it is.
        }
        else
        {
            textEdit->insertAt(QStringLiteral("// "), line, 0);
        }
    }
    textEdit->endUndoAction();

    // Leave the whole affected block selected again afterwards (if it
    // was a selection to begin with) so an immediate second press
    // toggles it straight back. text(line) includes the line's EOL
    // characters, which setSelection()/setCursorPosition() would
    // otherwise place the caret past (into the start of the next line)
    // - strip them first so the length here is the visible column count.
    QString lastLineText = textEdit->text(lineTo);
    while (lastLineText.endsWith(QLatin1Char('\n')) || lastLineText.endsWith(QLatin1Char('\r')))
        lastLineText.chop(1);
    const int lastLineLength = lastLineText.length();
    if (hadSelection)
        textEdit->setSelection(lineFrom, 0, lineTo, lastLineLength);
    else
        textEdit->setCursorPosition(lineFrom, lastLineLength);

    createStatusBarMessage(shouldUncomment ? tr("Block uncommented.") : tr("Block commented."), 2000);
}

//
// Insert fileheader comment...
//
void MainWindow::actionInsertFileheaderComment()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("/*\n");
    // next, we need to continue printing at a certain location:
    textEdit->insertAt(" *\tFile:\t\t " + strippedName(curFile)+ "\n", ++line, 0);
    textEdit->insertAt(" *\tVersion:\t\t1.0\n", ++line, 0);
    textEdit->insertAt(" *\tRevision:\t\t0\n", ++line, 0);
    textEdit->insertAt(" *\n", ++line, 0);
    textEdit->insertAt(" *\tAuthor:\t\t" + p_author + "\n", ++line, 0);
    textEdit->insertAt(" *\tEmail:\t\t" + p_email + "\n", ++line, 0);
    textEdit->insertAt(" *\tWeb:\t\t" + p_website + "\n", ++line, 0);
    textEdit->insertAt(" */\n", ++line, 0);

    // finally, we set our caret to the next following empty line!
    textEdit->setCursorPosition(line + 1, index);
}

//
// Insert C-style single line comment
//
void MainWindow::actionInsertCSingleComment()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert our text:
    textEdit->insert("\t/* NOTE: */");
    // finally, move caret to (N)OTE:
    textEdit->setCursorPosition(line, index + 4);
}

//
// Insert C-style multi line comment
//
void MainWindow::actionInsertCMultiComment()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert the first line of text!
    textEdit->insert("/*\n");
    // next, we need to continue printing at a certain location:
    textEdit->insertAt(" *\tComment:\n", ++line, 0);
    textEdit->insertAt(" */", ++line, 0);

    // finally, we set our caret to the next following empty line!
    textEdit->setCursorPosition(line - 1, index + 3);
}

//
// Insert C++style single line comment
//
void MainWindow::actionInsertCppSingleComment()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert our text:
    textEdit->insert("\t// NOTE:");
    // finally, move caret to (N)OTE:
    textEdit->setCursorPosition(line, index + 4);
}

//
// Insert C++style single line comment
//
void MainWindow::actionInsertCLineDevideComment()
{
    // we need the caret's ("cursor") recent position stored as a starting point for insertion!
    int line, index;
    textEdit->getCursorPosition(&line, &index); // get the position...

    // ...now insert our text:
    textEdit->insert("/* ------------- COMMENT --------------------------------------------- */\n");
    // finally, move caret to next line.
    textEdit->setCursorPosition(line + 1, index);
}

//
// Insert Amiga Version String...
//
void MainWindow::actionInsertAmigaVersionString()
{
    if(!(p_versionstring_set))
    {
        QString my_versionstring = "const char *ver = \"\\0$VER: my_program 1.0 (31.12.2019)\";";
        // we need the caret's ("cursor") recent position stored as a starting point for insertion!
        int line, index;
        textEdit->getCursorPosition(&line, &index); // get the position...

        // ...now insert the first line of text!
        textEdit->insert("\n");
        // next, we need to continue printing at a certain location:
        textEdit->insertAt(my_versionstring + "\n", ++line, 0);

        // finally, we set our caret to the next following empty line!
        textEdit->setCursorPosition(line + 1, index);

        p_versionstring_set = true;
    }
    else
    {
        (void)QMessageBox::information(this,
                                        AMIGAED_VERSION_STRING, "It seems there is allready a <i><b>version string</b></i> in this document!"
                                        "How many of them do you want?"
                                        "<br>Insertion will be cancelled, ya know?!",
                                        QMessageBox::Ok);

    }
}
/* ------------ End insertMenue Actions -------------------*/

//
// Start UAE emulation
//
// True if a process matching the configured emulator's executable name
// (Prefs > Emulator) is currently running anywhere on this system -
// whether AmigaED itself started it (in a previous session, then left
// it open - see closeEvent()) or it was launched some other way
// entirely. Used by actionEmulator() to warn before starting a second,
// likely-redundant instance - starting an emulator is slow enough that
// avoiding an unnecessary second one is worth the check.
//
// Matches by executable filename only (via the platform's own process
// listing, not AmigaED's own bookkeeping - myEmulator->state() already
// covers "did WE start it and is it still running", which this
// complements rather than replaces): it can't distinguish two different
// configs of the same emulator (e.g. separate OS 1.3 / OS 3.x WinUAE
// setups) from each other, so this asks rather than silently refusing -
// running two on purpose is a legitimate thing to do here.
//
bool MainWindow::isEmulatorProcessRunningExternally() const
{
    const QString exeName = QFileInfo(p_emulator).fileName();
    if (exeName.isEmpty())
        return false;

    QProcess probe;
#if defined(Q_OS_WIN)
    // "/NH" (no header) keeps the output to just matching rows (or
    // none) - if nothing matches, tasklist prints an "INFO:" line
    // instead of a row, which won't contain exeName, so the
    // contains() check below still comes out false correctly either
    // way.
    probe.start(QStringLiteral("tasklist"),
                QStringList() << "/FI" << QStringLiteral("IMAGENAME eq %1").arg(exeName) << "/NH");
#else
    // pgrep -x matches the process name exactly (not a substring of the
    // full command line), same spirit as tasklist's IMAGENAME filter
    // above. Available by default on every mainstream Linux distro
    // (part of procps/procps-ng).
    probe.start(QStringLiteral("pgrep"), QStringList() << "-x" << exeName);
#endif

    if (!probe.waitForStarted(2000))
        return false;   // tasklist/pgrep itself unavailable - can't tell, so don't block starting

    probe.waitForFinished(3000);

#if defined(Q_OS_WIN)
    const QString output = QString::fromLocal8Bit(probe.readAllStandardOutput());
    return output.contains(exeName, Qt::CaseInsensitive);
#else
    return probe.exitCode() == 0 && !probe.readAllStandardOutput().trimmed().isEmpty();
#endif
}

// CHANGE Programm Prefs according to your installation path and UAE flavour!
//
// Refuses to start a second instance while one is already running - UAE
// (and emulators generally) don't support being launched twice against
// the same config. QProcess::state() reflects "starting" synchronously,
// the moment start() is called below (well before the OS process has
// actually spawned), so this guard also closes the small window where a
// rapid double-click on the toolbar button could otherwise slip a second
// start() in before the UI had a chance to disable it.
//
// myEmulator->state() alone only catches an instance THIS AmigaED process
// itself started and is still tracking - it says nothing about one left
// running by a PREVIOUS AmigaED session (see closeEvent()'s "leave the
// emulator open?" prompt) or started outside AmigaED entirely.
// isEmulatorProcessRunningExternally() below covers that gap by asking
// the OS directly (by process name), rather than relying on AmigaED's
// own bookkeeping.
//
bool MainWindow::actionEmulator()
{
    if ((myEmulator && myEmulator->state() != QProcess::NotRunning) || p_externalEmulatorTracked)
    {
        createStatusBarMessage(tr("An emulator instance is already running - stop it first."), 4000);
        return false;
    }

    if (isEmulatorProcessRunningExternally())
    {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, tr(AMIGAED_VERSION_STRING),
            tr("An emulator process (%1) already appears to be running - "
               "possibly left open from a previous AmigaED session, or "
               "started outside AmigaED entirely.\n\n"
               "Start another instance anyway?").arg(QFileInfo(p_emulator).fileName()),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (reply == QMessageBox::No)
        {
            // Keep going with the SAME emulator process rather than
            // starting a second one - Stop Emulator needs to be able to
            // end it despite AmigaED never having launched it itself
            // (myEmulator stays NotRunning/null throughout), and Start
            // needs to stay disabled meanwhile, exactly as if AmigaED
            // had started it - see actionKillEmulator() and
            // checkEmulatorStillRunning().
            p_externalEmulatorTracked = true;
            emulatorAct->setDisabled(true);
            emulatorMenue->setDisabled(true);
            killEmulatorAct->setEnabled(true);

            createStatusBarMessage(tr("Not starting a second emulator instance."), 4000);
            return false;
        }
    }

    QString command = p_emulator;
    QStringList arguments;

    switch(p_defaultEmulator)
    {
    case 0:
        p_emulator_to_start = p_os13_config;
        break;

    case 1:
        p_emulator_to_start = p_os30_config;
        break;

    default:
        p_emulator_to_start = "--help";
    }

    if(p_emulator_to_start.isEmpty())
    {
        // give a user warning
        (void)QMessageBox::critical(this,
                                     AMIGAED_VERSION_STRING, "There seems to be <i><b>NO config file</b></i> for your requested <b><i>Emulation startup!</i></b><br> "
                                     "Please revisit the Prefs editor and name a configuration.<br>"
                                     "<br>This helps, ya know?!",
                                     QMessageBox::Ok);

        actionPrefsDialog(3);

        return (false);
    }

    if(p_emulator.isEmpty())
    {
        // the config file above may be set correctly, but without a valid
        // path to the emulator executable itself, QProcess::start() would
        // silently fail (FailedToStart) with no visible feedback at all
        (void)QMessageBox::critical(this,
                                     tr(AMIGAED_VERSION_STRING), tr("There seems to be <i><b>NO path to the emulator "
                                        "executable</b></i> configured (UAE path)!<br>"
                                        "Please revisit the Prefs editor, Emulator tab, and set it.<br>"
                                        "<br>This helps, ya know?!"),
                                     QMessageBox::Ok);

        actionPrefsDialog(3);

        return (false);
    }

    arguments << p_emulator_to_start;

    createStatusBarMessage(tr("Attempting to start UAE..."), 0);

    // Created once (if not already), then reused across every subsequent
    // start/stop cycle for the lifetime of the app - EXCEPT if a previous
    // session's closeEvent() ever left one running on purpose, which
    // can't happen mid-session, only at actual app exit.
    if (!myEmulator)
        myEmulator = new QProcess();   // no parent - see its declaration in mainwindow.h for why

    // Qt::UniqueConnection matters here: actionEmulator() runs again on
    // every subsequent start (once per stop/restart cycle, for the
    // lifetime of the app) - without it, each restart stacked another
    // set of connections on top of the previous ones, so finished()/
    // started()/output signals would each fire multiple times per real
    // event after the second start.
    QObject::connect(myEmulator, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)), Qt::UniqueConnection);
    QObject::connect(myEmulator, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(emu_finished(int,QProcess::ExitStatus)), Qt::UniqueConnection);
    QObject::connect(myEmulator, SIGNAL(readyReadStandardOutput()), this, SLOT(emu_readyReadStandardOutput()), Qt::UniqueConnection);
    QObject::connect(myEmulator, SIGNAL(started()), this, SLOT(emu_started()), Qt::UniqueConnection);

    // Reflected immediately rather than only once the asynchronous
    // started() signal arrives - emu_started() re-applies the same
    // state once it fires, and error() below reverts it again if the
    // process turns out to have failed to start after all.
    emulatorAct->setDisabled(true);
    emulatorMenue->setDisabled(true);
    killEmulatorAct->setEnabled(true);

    myEmulator->start(command, arguments);

    if(p_mydebug)
        qDebug() << "process pid: " << myEmulator->processId();

    return (true);
}

//
// start Workbench 1.3 emulation
//
void MainWindow::actionEmuOS13()
{
    p_defaultEmulator = 0;
    // actionEmulator() already handles user feedback for every failure
    // case itself (including opening Prefs when the cause is a missing
    // config/path) - it can also now fail simply because an emulator is
    // already running, which must NOT send the user into Prefs.
    actionEmulator();
}

//
// start Workbench 3.x emulation
//
void MainWindow::actionEmuOS30()
{
    p_defaultEmulator = 1;
    actionEmulator();
}

//
// Stop a running Emulator.
//
// Safe to call unconditionally, even when no emulator is currently
// running (closeEvent() does exactly that) - it's then a cheap no-op.
//
// terminate() asks the process to close itself gracefully (WM_CLOSE on
// Windows, SIGTERM elsewhere) - UAE, like most well-behaved programs,
// reacts to that and shuts down cleanly. If it doesn't do so within a
// few seconds (hung, or simply doesn't handle the request on some
// platform/build), kill() force-terminates it instead - so this
// function never leaves a zombie emulator running behind AmigaED, and
// never blocks the UI indefinitely either.
//
// Deliberately doesn't touch the toolbar/menu state itself - emu_finished()
// (still connected at this point) does that once the process actually
// exits, so there is exactly one place responsible for it rather than
// two copies that could drift out of sync.
//
void MainWindow::actionKillEmulator()
{
    if (p_externalEmulatorTracked)
    {
        // Not a process AmigaED itself started (myEmulator stays
        // NotRunning/null throughout this case) - terminate it by name
        // via the platform's own tools instead (killExternalEmulatorProcess()),
        // then resync the toolbar/menu state here directly, since
        // emu_finished() (which normally does that once a QProcess
        // AmigaED itself started actually exits) never fires for a
        // process that was never started via QProcess in the first place.
        createStatusBarMessage(tr("Stopping emulator..."), 0);
        killExternalEmulatorProcess();
        p_externalEmulatorTracked = false;
        emulatorMenue->setEnabled(true);
        emulatorAct->setEnabled(true);
        killEmulatorAct->setDisabled(true);
        return;
    }

    if (!myEmulator || myEmulator->state() == QProcess::NotRunning)
        return;

    createStatusBarMessage(tr("Stopping emulator..."), 0);

    myEmulator->terminate();

    if (!myEmulator->waitForFinished(3000))
    {
        if(p_mydebug)
            qDebug() << "Emulator did not terminate gracefully within 3s - forcing kill().";
        myEmulator->kill();
        myEmulator->waitForFinished(3000);
    }
}

//
// Terminates a matching emulator process AmigaED did not itself start
// (see p_externalEmulatorTracked / actionEmulator()'s "start another
// instance anyway?" prompt) - by executable name, via the platform's
// own process-management tools, since there's no QProcess object (and
// therefore no terminate()/kill()) for a process AmigaED never
// launched. The Windows/Linux counterpart to
// isEmulatorProcessRunningExternally()'s own tasklist/pgrep detection.
//
void MainWindow::killExternalEmulatorProcess()
{
    const QString exeName = QFileInfo(p_emulator).fileName();
    if (exeName.isEmpty())
        return;

    QProcess killer;
#if defined(Q_OS_WIN)
    // /F forces termination (SIGTERM-less platforms don't get a
    // "please shut down gracefully" option here the way QProcess::
    // terminate() gives an owned process) - matches actionKillEmulator()'s
    // own eventual kill() fallback for an owned process that didn't
    // respond to terminate() in time.
    killer.start(QStringLiteral("taskkill"), QStringList() << "/IM" << exeName << "/F");
#else
    killer.start(QStringLiteral("pkill"), QStringList() << "-x" << exeName);
#endif
    killer.waitForFinished(3000);
}

//
// Toggle visibility of caret line
//
void MainWindow::actionShowCaretLine()
{
    qDebug() << "in carret line";
    //popNotImplemented();
    if(showCaretLineAct->isChecked())
    {
        // show caret line
        textEdit->setCaretLineVisible(true);
    }
    else
    {
        // don't show caret line
        textEdit->setCaretLineVisible(false);
    }
}

//
// Toggle visibility of line numbers
//
void MainWindow::actionShowLineNumbers()
{
    QFontMetrics fontmetrics = QFontMetrics(textEdit->font());
    qDebug() << "in line numbers";
    // popNotImplemented();
    if(showLineNumbersAct->isChecked())
    {
        // show line numbers
        textEdit->setMarginLineNumbers(0, true);
        textEdit->setMarginWidth(0, fontmetrics.horizontalAdvance(QString::number(textEdit->lines())) + 10);
    }
    else
    {
        // don't show line numbers
        textEdit->setMarginLineNumbers(0, false);
        textEdit->setMarginWidth(0, "");
    }
}

//
//  TOGGLE: Show or hide unprintable characters
//
void MainWindow::actionShowUnprintable()
{
    if(showUnprintableAct->isChecked())
    {
        // show unprintable characters
        textEdit->setEolVisibility(true);
        textEdit->setIndentationGuides(true);
        textEdit->setWhitespaceVisibility(QsciScintilla::WsVisible);
        this->showEOLAct->setChecked(true);
        this->showEOLAct->setEnabled(false);
        this->showIndentationGuidesAct->setChecked(true);
        this->showIndentationGuidesAct->setEnabled(false);
    }
    else
    {
        // don't show unprintable characters
        textEdit->setEolVisibility(false);
        textEdit->setIndentationGuides(false);
        textEdit->setWhitespaceVisibility(QsciScintilla::WsInvisible);
        this->showEOLAct->setChecked(false);
        this->showEOLAct->setEnabled(true);
        this->showIndentationGuidesAct->setChecked(false);
        this->showIndentationGuidesAct->setEnabled(true);
    }
}

//
//  TOGGLE: Show or hide EOL character
//
void MainWindow::actionShowEOL()
{
    if(showEOLAct->isChecked())
    {
        // show EOL character
        textEdit->setEolVisibility(true);
    }
    else
    {
        // don't show EOL character
        textEdit->setEolVisibility(true);
    }
}

//
//  Open global preferences dialog
//
void MainWindow::actionPrefsDialog(int tabindex = 0)
{
    PrefsDialog *acePrefs = new PrefsDialog(this, tabindex);
    acePrefs->exec();

    // afterwards insert new prefs into MainWindow variables for instant use!
    readSettings();
}

//
// search for given argument...
//
void MainWindow::actionSearch()
{
    if (!p_search_is_open)
    {
        searchGroup->show();
        p_search_is_open = true;
    }
    // Always (re-)focus and select the existing text, even if the panel
    // was already open - previously, a second Ctrl+F while it was
    // already open did nothing at all, which also would have made the
    // context menu's "Search and Replace..." entry (see
    // actionSearchReplaceFromContext()) fail to focus/select the word
    // it just filled in whenever the panel happened to be open already.
    lineEdit_find->setFocus();
    lineEdit_find->selectAll();
}

//
// Context-menu entry "Search and Replace...": pre-fills Find: with the
// word under the click (captured in showCustomContextMenue(), since by
// the time this slot fires the click position no longer means anything),
// then opens/focuses the panel exactly like Ctrl+F.
//
void MainWindow::actionSearchReplaceFromContext()
{
    if (!p_contextMenuWordAtClick.isEmpty())
        lineEdit_find->setText(p_contextMenuWordAtClick);

    actionSearch();
}


//
// find strings "error", "Warning" and "file" in output pane - make their line information jumpable
//
void MainWindow::jumpCompilerWarnings()
{
    ;
}

//
// true if the configured "Default application style" (Prefs > Misc) is
// the synthetic "Dark" entry - not a real QStyleFactory key, handled
// specially by applyApplicationStyle()/applyLexerDarkColors() below.
//
bool MainWindow::isDarkTheme() const
{
    return p_default_style == QLatin1String("Dark");
}

//
// true if p_default_style selects one of the two nostalgic Workbench
// themes - see workbench13ApplicationPalette()/workbench31ApplicationPalette()
// below and buildThemeMenu()/View menu > Theme.
//
bool MainWindow::isWorkbench13Theme() const
{
    return p_default_style == QLatin1String("Workbench 1.3");
}

bool MainWindow::isWorkbench31Theme() const
{
    return p_default_style == QLatin1String("Workbench 3.1");
}

//
// Builds the dark QPalette used together with the "Fusion" style for the
// "Dark" application style. Native styles (e.g. "windowsvista" on
// Windows) mostly ignore a custom QPalette for their own chrome - Fusion
// is the only style that reliably honours it on every platform, which is
// why applyApplicationStyle() always forces Fusion whenever "Dark" is
// selected, regardless of what p_default_style used to say before.
//
QPalette MainWindow::darkApplicationPalette() const
{
    QPalette palette;

    palette.setColor(QPalette::Window,            QColor(0x35, 0x35, 0x35));
    palette.setColor(QPalette::WindowText,        QColor(0xd4, 0xd4, 0xd4));
    palette.setColor(QPalette::Base,              QColor(0x23, 0x23, 0x23));
    palette.setColor(QPalette::AlternateBase,     QColor(0x35, 0x35, 0x35));
    palette.setColor(QPalette::ToolTipBase,       QColor(0x35, 0x35, 0x35));
    palette.setColor(QPalette::ToolTipText,       QColor(0xd4, 0xd4, 0xd4));
    palette.setColor(QPalette::Text,              QColor(0xd4, 0xd4, 0xd4));
    palette.setColor(QPalette::Button,            QColor(0x3c, 0x3c, 0x3c));
    palette.setColor(QPalette::ButtonText,        QColor(0xd4, 0xd4, 0xd4));
    palette.setColor(QPalette::BrightText,        QColor(0xf4, 0x47, 0x47));
    palette.setColor(QPalette::Link,              QColor(0x56, 0x9c, 0xd6));
    palette.setColor(QPalette::LinkVisited,       QColor(0xb3, 0x92, 0xf0));
    palette.setColor(QPalette::Highlight,         QColor(0x26, 0x4f, 0x78));
    palette.setColor(QPalette::HighlightedText,   QColor(0xff, 0xff, 0xff));

    // Disabled widgets: dimmer text, everything else unchanged.
    palette.setColor(QPalette::Disabled, QPalette::WindowText,      QColor(0x7f, 0x7f, 0x7f));
    palette.setColor(QPalette::Disabled, QPalette::Text,            QColor(0x7f, 0x7f, 0x7f));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText,      QColor(0x7f, 0x7f, 0x7f));
    palette.setColor(QPalette::Disabled, QPalette::Highlight,       QColor(0x50, 0x50, 0x50));
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(0x7f, 0x7f, 0x7f));

    return palette;
}

//
// Evokes the classic Workbench 1.3 (Kickstart 1.2/1.3, 1985-1988) look:
// the iconic solid BLUE desktop background with white icon-label text,
// and white title bars/gadgets with blue text - colours measured
// directly from a real Workbench 1.3 screenshot (#0055AA blue desktop,
// white title bar, white icon-label text on the blue, black-on-white
// everywhere else), not guessed. Orange is kept as the selection accent
// (no direct screenshot evidence either way, but a well-established,
// historically accurate choice - also the accent AmigaED's own built-in
// tool icon uses, for a consistent "Amiga" identity). Deliberately flat
// (no gradients/3D shading) to match 1.3's own pre-bevel era look.
//
QPalette MainWindow::workbench13ApplicationPalette() const
{
    QPalette palette;

    const QColor blue(0x00, 0x55, 0xAA);
    const QColor orange(0xFF, 0x88, 0x00);

    palette.setColor(QPalette::Window,            blue);
    palette.setColor(QPalette::WindowText,        Qt::white);
    palette.setColor(QPalette::Base,              Qt::white);
    palette.setColor(QPalette::AlternateBase,     blue.lighter(130));
    palette.setColor(QPalette::ToolTipBase,       Qt::white);
    palette.setColor(QPalette::ToolTipText,       Qt::black);
    palette.setColor(QPalette::Text,              Qt::black);
    palette.setColor(QPalette::Button,            Qt::white);
    palette.setColor(QPalette::ButtonText,        blue);
    palette.setColor(QPalette::BrightText,        orange);
    palette.setColor(QPalette::Link,              Qt::white);
    palette.setColor(QPalette::LinkVisited,       QColor(0xDD, 0xDD, 0xDD));
    palette.setColor(QPalette::Highlight,         orange);
    palette.setColor(QPalette::HighlightedText,   Qt::black);

    palette.setColor(QPalette::Disabled, QPalette::WindowText,      QColor(0x99, 0xBB, 0xDD));
    palette.setColor(QPalette::Disabled, QPalette::Text,            QColor(0x99, 0x99, 0x99));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText,      QColor(0x99, 0xBB, 0xDD));
    palette.setColor(QPalette::Disabled, QPalette::Highlight,       QColor(0xCC, 0xAA, 0x77));
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(0x66, 0x66, 0x66));

    return palette;
}

//
// Evokes the classic Workbench 3.1 (Kickstart 3.1, 1994) look: a plain,
// flat, NEUTRAL GRAY desktop (no blue tint at all) with white title
// bars/gadgets and black text throughout - colours measured directly
// from a real Workbench 3.1 screenshot (#AAAAAA gray desktop, white
// title bar, black text/labels everywhere), not guessed - it's
// considerably plainer than Workbench 1.3's blue desktop above, exactly
// as the real thing is. The classic Amiga blue is kept as the selection
// accent (consistent with 1.3's own dominant colour, just demoted here
// from "the whole desktop" to "just the highlight"), giving the two
// themes a family resemblance while staying clearly distinguishable.
//
QPalette MainWindow::workbench31ApplicationPalette() const
{
    QPalette palette;

    const QColor gray(0xAA, 0xAA, 0xAA);
    const QColor blue(0x00, 0x55, 0xAA);

    palette.setColor(QPalette::Window,            gray);
    palette.setColor(QPalette::WindowText,        Qt::black);
    palette.setColor(QPalette::Base,              Qt::white);
    palette.setColor(QPalette::AlternateBase,     gray.lighter(115));
    palette.setColor(QPalette::ToolTipBase,       Qt::white);
    palette.setColor(QPalette::ToolTipText,       Qt::black);
    palette.setColor(QPalette::Text,              Qt::black);
    palette.setColor(QPalette::Button,            Qt::white);
    palette.setColor(QPalette::ButtonText,        Qt::black);
    palette.setColor(QPalette::BrightText,        QColor(0xCC, 0x33, 0x22));
    palette.setColor(QPalette::Link,              blue);
    palette.setColor(QPalette::LinkVisited,       blue.darker(120));
    palette.setColor(QPalette::Highlight,         blue);
    palette.setColor(QPalette::HighlightedText,   Qt::white);

    palette.setColor(QPalette::Disabled, QPalette::WindowText,      QColor(0x77, 0x77, 0x77));
    palette.setColor(QPalette::Disabled, QPalette::Text,            QColor(0x77, 0x77, 0x77));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText,      QColor(0x77, 0x77, 0x77));
    palette.setColor(QPalette::Disabled, QPalette::Highlight,       QColor(0x99, 0x99, 0x99));
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(0xDD, 0xDD, 0xDD));

    return palette;
}

//
// Applies the configured "Default application style" (Prefs > Misc) to
// the running application. Called once from the constructor (initial
// startup style) and again, live, from readSettings() whenever the value
// actually changed (Prefs dialog closed, or Shift+F12 "Reload settings") -
// so switching to/from "Dark" takes effect immediately, no restart needed.
//
void MainWindow::applyApplicationStyle()
{
    if (isDarkTheme())
    {
        QApplication::setStyle(QStyleFactory::create("Fusion"));
        QApplication::setPalette(darkApplicationPalette());
    }
    else if (isWorkbench13Theme())
    {
        QApplication::setStyle(QStyleFactory::create("Fusion"));
        QApplication::setPalette(workbench13ApplicationPalette());
    }
    else if (isWorkbench31Theme())
    {
        QApplication::setStyle(QStyleFactory::create("Fusion"));
        QApplication::setPalette(workbench31ApplicationPalette());
    }
    else
    {
        QApplication::setStyle(p_default_style);
        // Undo a previously applied dark/Workbench palette (if any) - go
        // back to the newly chosen style's own standard palette, rather
        // than leaving a stale one in place after switching away from one
        // of the three synthetic themes above.
        if (QApplication::style())
            QApplication::setPalette(QApplication::style()->standardPalette());
    }

    if (isWorkbench13Theme())
    {
        // Fusion's own menu bar rendering synthesizes a shading gradient
        // from a single QPalette::Window colour - against this theme's
        // saturated blue, that washed out badly enough to make the
        // (white, per the palette above) menu text unreadable in places.
        // An explicit stylesheet for just the menu bar/menus sidesteps
        // that gradient synthesis entirely, rather than fighting it.
        this->setStyleSheet(
            "QMenuBar { background-color: #0055AA; color: white; }"
            "QMenuBar::item { background-color: #0055AA; color: white; }"
            "QMenuBar::item:selected, QMenuBar::item:pressed { background-color: #FF8800; color: black; }"
            "QMenu { background-color: white; color: black; border: 1px solid #0055AA; }"
            "QMenu::item:selected { background-color: #FF8800; color: black; }"
        );
    }
    else
    {
        this->setStyleSheet(QString());   // clear any stale stylesheet override, e.g. after switching away from "Workbench 1.3"/"Dark"
    }

    syncThemeMenuCheckedState();      // keep View > Theme's checkmark in sync, however the style just changed (Prefs, this menu itself, or Shift+F12 reload)

    // Re-colour everything else that's theme-dependent but isn't part of
    // the QPalette/stylesheet above: every open editor tab's margin/
    // caret-line/lexer colours, and the output pane's error/warning
    // highlighting. Centralized here rather than left to each individual
    // caller (Prefs dialog, View > Theme menu, Shift+F12 reload) so all
    // three stay in sync automatically - confirmed missing for the
    // View > Theme menu specifically before this: it called
    // applyApplicationStyle() directly, never reapplyEditorTheme(), so
    // switching to/from "Dark" through that menu left already-open tabs'
    // colours stale, and the output pane's highlighting wouldn't move to
    // the newly selected theme's colours either.
    reapplyEditorTheme();
    highlightOutputDiagnostics();
}

//
// Populates View > Theme with one checkable entry per style available on
// this platform (QStyleFactory::keys() - e.g. "Fusion", "Windows",
// "windowsvista" on Windows; "Fusion", "Breeze" etc. on Linux, depending
// on what's installed), followed by a separator and the three synthetic
// entries (Dark, Workbench 1.3, Workbench 3.1 - see is/applyXxxTheme()
// above) that aren't real QStyleFactory keys. All of them share one
// QActionGroup (themeActionGroup) for mutual exclusion - Qt shows this
// as a bullet/checkmark per platform convention, exactly one at a time -
// and one shared slot (actionSelectTheme()), which reads whichever
// action actually fired via sender() rather than needing a separate
// slot per entry. Called once from the constructor, after p_default_style
// itself has already been loaded (readSettings()) and applied
// (applyApplicationStyle()) - see their call sites for the ordering.
//
void MainWindow::buildThemeMenu()
{
    themeMenue = viewMenue->addMenu(tr("Theme"));
    themeActionGroup = new QActionGroup(this);
    themeActionGroup->setExclusive(true);

    const QStringList nativeStyles = QStyleFactory::keys();
    for (const QString &styleName : nativeStyles)
    {
        QAction *act = themeMenue->addAction(styleName);
        act->setCheckable(true);
        themeActionGroup->addAction(act);
        connect(act, SIGNAL(triggered()), this, SLOT(actionSelectTheme()));
    }

    themeMenue->addSeparator();

    // Kept untranslated, like the native style names above (Qt style
    // keys aren't translated either, and neither is Prefs > Misc's own
    // matching combo-box entry - MISC/DefaultStyle is saved/restored as
    // plain text via that combo box's currentText(), which this menu
    // must keep matching exactly).
    const QStringList syntheticThemes = { QStringLiteral("Dark"),
                                           QStringLiteral("Workbench 1.3"),
                                           QStringLiteral("Workbench 3.1") };
    for (const QString &name : syntheticThemes)
    {
        QAction *act = themeMenue->addAction(name);
        act->setCheckable(true);
        themeActionGroup->addAction(act);
        connect(act, SIGNAL(triggered()), this, SLOT(actionSelectTheme()));
    }

    syncThemeMenuCheckedState();
}

//
// View > Theme entry clicked: applies the clicked action's own text as
// the new p_default_style, persists it (matching Prefs > Misc's own
// MISC/DefaultStyle key exactly, so either place shows the other's
// change), and applies it immediately.
//
void MainWindow::actionSelectTheme()
{
    QAction *act = qobject_cast<QAction *>(sender());
    if (!act)
        return;

    p_default_style = act->text();

    QSettings settings("MB-SoftWorX", "Amiga Cross Editor");
    settings.setValue("MISC/DefaultStyle", p_default_style);

    applyApplicationStyle();
}

//
// Ensures the entry matching the current p_default_style is checked (and,
// via themeActionGroup's exclusivity, every other entry isn't) - called
// after buildThemeMenu() and every time applyApplicationStyle() runs, so
// this menu stays in sync no matter where the style was actually changed
// (this menu itself, the Prefs dialog, or Shift+F12 "Reload settings").
// A no-op if the menu hasn't been built yet (see buildThemeMenu()'s call
// site in the constructor vs. applyApplicationStyle()'s own, earlier one).
//
void MainWindow::syncThemeMenuCheckedState()
{
    if (!themeMenue)
        return;

    const QList<QAction *> actions = themeMenue->actions();
    for (QAction *act : actions)
    {
        if (act->isCheckable())
            act->setChecked(act->text() == p_default_style);
    }
}

//
// Recolors an already-created lexer's styles for the CURRENT theme -
// dark colours for "Dark", or a reset back to this app's own light-theme
// colours for every other theme (native styles, Workbench 1.3,
// Workbench 3.1) - see the reset branch's own comment for why that
// second half exists at all. Called right after every lexer is created
// (initializeLexerCPP()/.../initializeLexerPascal()) AND, unchanged, from
// reapplyEditorTheme() to recolor an existing tab's lexer IN PLACE when
// the style is switched live: it deliberately never recreates the lexer
// (setLexer(new ...)) itself - that would needlessly discard the tab's
// current fold state and, worse, would silently switch a tab the user
// had deliberately set to a different lexer (e.g. Makefile) back to
// whatever that lexer's own initializeLexerXxx() defaults to.
//
// Dark-branch approach: paint every style of the lexer with one dark
// background/light foreground pair first - QsciLexer::setColor()/
// setPaper() apply to ALL styles when no explicit style index is given,
// exactly like the existing lexer->setFont(myfont) calls elsewhere in
// this file already rely on. That guarantees there is no leftover white
// patch for any style not hand-tuned below (Operator, Identifier, UUID,
// Regex, Asm, ...). Only the styles that actually benefit from a
// distinct colour (comments, keywords, strings, numbers, ...) are then
// re-tinted individually - tuned for readability on a dark background
// rather than reusing the light-theme colours as-is (e.g. QsciLexerCPP's
// own default navy-blue Keyword, or this project's own indigo
// GlobalClass/firebrick KeywordSet2 set in initializeLexerCPP() above,
// would all have poor contrast on a near-black background).
//
void MainWindow::applyLexerDarkColors(QsciLexer *lexer)
{
    if (!lexer)
        return;

    if (!isDarkTheme())
    {
        // Mirrors the dark branch below exactly (same blanket-then-
        // individual-retint mechanism, same named styles per lexer type)
        // but with light-appropriate colours - deliberately NOT using
        // QsciLexer::defaultColor()/defaultPaper() in a generic loop (an
        // earlier version of this did): confirmed that didn't actually
        // repaint already-open tabs correctly, whereas this exact
        // pattern is already proven to work for the dark branch, just
        // with different colour values. Necessary because switching
        // AWAY from "Dark" to any other theme left already-open tabs'
        // lexers stuck showing dark-theme colours indefinitely - this
        // function used to only ever handle "make it dark", with
        // nothing to undo that afterward.
        lexer->setPaper(Qt::white);
        lexer->setColor(Qt::black);

        const QColor comment(0x00, 0x7f, 0x00);
        const QColor keyword(0x00, 0x00, 0x7f);
        const QColor number(0x00, 0x7f, 0x7f);
        const QColor string(0x7f, 0x00, 0x7f);
        const QColor preprocessor(0x80, 0x80, 0x00);
        const QColor amigaType(0x4b, 0x00, 0x82);       // GlobalClass - indigo, matches initializeLexerCPP()'s own
        const QColor amigaFunction(0xb2, 0x22, 0x22);   // KeywordSet2 - firebrick, matches initializeLexerCPP()'s own

        if (auto *cpp = dynamic_cast<QsciLexerCPP *>(lexer))
        {
            cpp->setColor(comment,       QsciLexerCPP::Comment);
            cpp->setColor(comment,       QsciLexerCPP::CommentLine);
            cpp->setColor(comment,       QsciLexerCPP::CommentDoc);
            cpp->setColor(comment,       QsciLexerCPP::CommentLineDoc);
            cpp->setColor(number,        QsciLexerCPP::Number);
            cpp->setColor(keyword,       QsciLexerCPP::Keyword);
            cpp->setColor(string,        QsciLexerCPP::DoubleQuotedString);
            cpp->setColor(string,        QsciLexerCPP::SingleQuotedString);
            cpp->setColor(string,        QsciLexerCPP::UnclosedString);
            cpp->setColor(preprocessor,  QsciLexerCPP::PreProcessor);
            cpp->setColor(amigaType,     QsciLexerCPP::GlobalClass);
            QFont typeFont = cpp->font(QsciLexerCPP::GlobalClass);
            typeFont.setBold(true);
            cpp->setFont(typeFont, QsciLexerCPP::GlobalClass);
            cpp->setColor(amigaFunction, QsciLexerCPP::KeywordSet2);
        }
        else if (auto *mk = dynamic_cast<QsciLexerMakefile *>(lexer))
        {
            mk->setColor(comment,      QsciLexerMakefile::Comment);
            mk->setColor(preprocessor, QsciLexerMakefile::Preprocessor);
            mk->setColor(amigaType,    QsciLexerMakefile::Variable);
            mk->setColor(keyword,      QsciLexerMakefile::Target);
            mk->setColor(QColor(0xcc, 0x00, 0x00), QsciLexerMakefile::Error);
        }
        else if (auto *bat = dynamic_cast<QsciLexerBatch *>(lexer))
        {
            bat->setColor(comment,       QsciLexerBatch::Comment);
            bat->setColor(keyword,       QsciLexerBatch::Keyword);
            bat->setColor(amigaFunction, QsciLexerBatch::Label);
            bat->setColor(preprocessor,  QsciLexerBatch::HideCommandChar);
            bat->setColor(number,        QsciLexerBatch::ExternalCommand);
            bat->setColor(amigaType,     QsciLexerBatch::Variable);
        }
        else if (auto *pas = dynamic_cast<QsciLexerPascal *>(lexer))
        {
            pas->setColor(comment,       QsciLexerPascal::Comment);
            pas->setColor(comment,       QsciLexerPascal::CommentParenthesis);
            pas->setColor(comment,       QsciLexerPascal::CommentLine);
            pas->setColor(preprocessor,  QsciLexerPascal::PreProcessor);
            pas->setColor(preprocessor,  QsciLexerPascal::PreProcessorParenthesis);
            pas->setColor(number,        QsciLexerPascal::Number);
            pas->setColor(number,        QsciLexerPascal::HexNumber);
            pas->setColor(keyword,       QsciLexerPascal::Keyword);
            pas->setColor(string,        QsciLexerPascal::SingleQuotedString);
            pas->setColor(string,        QsciLexerPascal::UnclosedString);
            pas->setColor(string,        QsciLexerPascal::Character);
            pas->setColor(amigaFunction, QsciLexerPascal::Asm);
        }
        else if (auto *inst = dynamic_cast<AmigaInstallerLexer *>(lexer))
        {
            inst->setColor(comment,       AmigaInstallerLexer::Comment);
            inst->setColor(string,        AmigaInstallerLexer::String);
            inst->setColor(keyword,       AmigaInstallerLexer::Keyword);
            inst->setColor(number,        AmigaInstallerLexer::Number);
            inst->setColor(preprocessor,  AmigaInstallerLexer::Symbol);
            inst->setColor(amigaFunction, AmigaInstallerLexer::Variable);
        }
        else if (auto *guide = dynamic_cast<AmigaGuideLexer *>(lexer))
        {
            guide->setColor(keyword,       AmigaGuideLexer::Command);
            guide->setColor(string,        AmigaGuideLexer::String);
            guide->setColor(preprocessor,  AmigaGuideLexer::Link);
            guide->setColor(comment,       AmigaGuideLexer::Comment);
        }
        else if (auto *asmLexer = dynamic_cast<M68kAsmLexer *>(lexer))
        {
            asmLexer->setColor(comment,       M68kAsmLexer::Comment);
            asmLexer->setColor(string,        M68kAsmLexer::String);
            asmLexer->setColor(keyword,       M68kAsmLexer::Mnemonic);
            asmLexer->setColor(preprocessor,  M68kAsmLexer::Directive);
            asmLexer->setColor(amigaFunction, M68kAsmLexer::Register);
            asmLexer->setColor(number,        M68kAsmLexer::Number);
            asmLexer->setColor(amigaType,     M68kAsmLexer::Label);
        }

        return;
    }

    lexer->setPaper(QColor(0x1e, 0x1e, 0x1e));
    lexer->setColor(QColor(0xd4, 0xd4, 0xd4));

    const QColor comment(0x6a, 0x99, 0x55);
    const QColor keyword(0x56, 0x9c, 0xd6);
    const QColor number(0xb5, 0xce, 0xa8);
    const QColor string(0xce, 0x91, 0x78);
    const QColor preprocessor(0xc5, 0x86, 0xc0);
    const QColor amigaType(0xb3, 0x92, 0xf0);       // GlobalClass - was indigo, brightened for dark-background contrast
    const QColor amigaFunction(0xe5, 0xc0, 0x7b);   // KeywordSet2 - was firebrick, warm amber instead

    if (auto *cpp = dynamic_cast<QsciLexerCPP *>(lexer))
    {
        // Also covers AmigaLexerCPP (amigalexercpp.h), a QsciLexerCPP subclass.
        cpp->setColor(comment,       QsciLexerCPP::Comment);
        cpp->setColor(comment,       QsciLexerCPP::CommentLine);
        cpp->setColor(comment,       QsciLexerCPP::CommentDoc);
        cpp->setColor(comment,       QsciLexerCPP::CommentLineDoc);
        cpp->setColor(number,        QsciLexerCPP::Number);
        cpp->setColor(keyword,       QsciLexerCPP::Keyword);
        cpp->setColor(string,        QsciLexerCPP::DoubleQuotedString);
        cpp->setColor(string,        QsciLexerCPP::SingleQuotedString);
        cpp->setColor(string,        QsciLexerCPP::UnclosedString);
        cpp->setColor(preprocessor,  QsciLexerCPP::PreProcessor);
        cpp->setColor(amigaType,     QsciLexerCPP::GlobalClass);     // Amiga NDK/MUI types+constants
        cpp->setColor(amigaFunction, QsciLexerCPP::KeywordSet2);     // Amiga NDK/MUI function names
    }
    else if (auto *mk = dynamic_cast<QsciLexerMakefile *>(lexer))
    {
        mk->setColor(comment,      QsciLexerMakefile::Comment);
        mk->setColor(preprocessor, QsciLexerMakefile::Preprocessor);
        mk->setColor(amigaType,    QsciLexerMakefile::Variable);
        mk->setColor(keyword,      QsciLexerMakefile::Target);
        mk->setColor(QColor(0xf4, 0x47, 0x47), QsciLexerMakefile::Error);
    }
    else if (auto *bat = dynamic_cast<QsciLexerBatch *>(lexer))
    {
        bat->setColor(comment,       QsciLexerBatch::Comment);
        bat->setColor(keyword,       QsciLexerBatch::Keyword);
        bat->setColor(amigaFunction, QsciLexerBatch::Label);
        bat->setColor(preprocessor,  QsciLexerBatch::HideCommandChar);
        bat->setColor(number,        QsciLexerBatch::ExternalCommand);
        bat->setColor(amigaType,     QsciLexerBatch::Variable);
    }
    else if (auto *pas = dynamic_cast<QsciLexerPascal *>(lexer))
    {
        pas->setColor(comment,       QsciLexerPascal::Comment);
        pas->setColor(comment,       QsciLexerPascal::CommentParenthesis);
        pas->setColor(comment,       QsciLexerPascal::CommentLine);
        pas->setColor(preprocessor,  QsciLexerPascal::PreProcessor);
        pas->setColor(preprocessor,  QsciLexerPascal::PreProcessorParenthesis);
        pas->setColor(number,        QsciLexerPascal::Number);
        pas->setColor(number,        QsciLexerPascal::HexNumber);
        pas->setColor(keyword,       QsciLexerPascal::Keyword);
        pas->setColor(string,        QsciLexerPascal::SingleQuotedString);
        pas->setColor(string,        QsciLexerPascal::UnclosedString);
        pas->setColor(string,        QsciLexerPascal::Character);
        pas->setColor(amigaFunction, QsciLexerPascal::Asm);
    }
    else if (auto *inst = dynamic_cast<AmigaInstallerLexer *>(lexer))
    {
        inst->setColor(comment,       AmigaInstallerLexer::Comment);
        inst->setColor(string,        AmigaInstallerLexer::String);
        inst->setColor(keyword,       AmigaInstallerLexer::Keyword);
        inst->setColor(number,        AmigaInstallerLexer::Number);
        inst->setColor(preprocessor,  AmigaInstallerLexer::Symbol);
        inst->setColor(amigaFunction, AmigaInstallerLexer::Variable);
        // Operator already got the uniform light-gray 'fg' from the pass
        // above - fine, its own light-theme default is just a neutral
        // dark grey too, no dedicated dark-theme colour needed.
    }
    else if (auto *guide = dynamic_cast<AmigaGuideLexer *>(lexer))
    {
        guide->setColor(keyword,       AmigaGuideLexer::Command);
        guide->setColor(string,        AmigaGuideLexer::String);
        guide->setColor(preprocessor,  AmigaGuideLexer::Link);
        guide->setColor(comment,       AmigaGuideLexer::Comment);
    }
    else if (auto *asmLexer = dynamic_cast<M68kAsmLexer *>(lexer))
    {
        asmLexer->setColor(comment,       M68kAsmLexer::Comment);
        asmLexer->setColor(string,        M68kAsmLexer::String);
        asmLexer->setColor(keyword,       M68kAsmLexer::Mnemonic);
        asmLexer->setColor(preprocessor,  M68kAsmLexer::Directive);
        asmLexer->setColor(amigaFunction, M68kAsmLexer::Register);
        asmLexer->setColor(number,        M68kAsmLexer::Number);
        asmLexer->setColor(amigaType,     M68kAsmLexer::Label);
    }
}

//
// Re-applies margin/caret-line/selection/lexer colors to every currently
// open tab. Used when the "Default application style" changes at runtime
// (Prefs closed / Shift+F12 "Reload settings", or the View > Theme menu)
// so already-open tabs don't keep looking like the OLD theme until
// reopened or the app is restarted - see applyApplicationStyle().
//
void MainWindow::reapplyEditorTheme()
{
    if (!tabWidget)
        return;

    for (int i = 0; i < tabWidget->count(); ++i)
    {
        QsciScintilla *editor = qobject_cast<QsciScintilla *>(tabWidget->widget(i));
        if (!editor)
            continue;

        initializeMargin(editor);
        initializeCaretLine(editor);
        if (editor->lexer())
            applyLexerDarkColors(editor->lexer());
        else
            initializeLexerNone(editor, false);   // plain-text tab: no lexer to recolor - redo the low-level STYLE_DEFAULT colouring instead, without the "syntax changed" status message
    }
}

//
// set default fonts, depending on OS
//
void MainWindow::initializeFont()
{
// set a readable default font for Linux and Windows:
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    QFont font("Courier New", 10);
#elif defined(__APPLE__)
    QFont font("SF Mono Regular", 11);
    if(p_mydebug)
        qDebug() << "Running on Mac. Font is SF Mono Regular now!";
#elif defined(__unix__)
    QFont font("Source Code Pro", 9);
    if(p_mydebug)
        qDebug() << "Linux detected. Setting font to Source Code Pro";
#endif

    myfont = font;
    myfont.setFixedPitch(true);
    // Belt-and-braces: if the named font above isn't installed on this
    // system, Qt's font substitution should still prefer a monospace
    // replacement rather than silently falling back to a proportional one.
    myfont.setStyleHint(QFont::Monospace, QFont::PreferMatch);

#if defined(__unix__) && !defined(__APPLE__)
    // Root cause of the "editor text crumbles/overlaps itself" bug reported
    // on Linux: setFixedPitch(true)/setStyleHint(Monospace) above only
    // steer *our own* 'myfont' object. QScintilla itself does NOT reuse
    // that QFont internally - every syntax-highlighting style (see the
    // various initializeLexerXxx() functions below, all of which do
    // lexer->setFont(myfont)) is handed just the family name and point
    // size, and QScintilla's own Qt platform layer then builds a brand
    // new, *unhinted* QFont from that family+size alone for each style it
    // draws with - our setFixedPitch()/setStyleHint() flags never reach
    // those internal font objects. So if "Source Code Pro" isn't actually
    // installed (it needs a separate font package on most distros and is
    // often missing), Qt's font matching has no monospace hint to fall
    // back on for THOSE internal fonts and can silently substitute a
    // *proportional* font (e.g. plain "DejaVu Sans") - while our own
    // hinted 'myfont' here still reports a perfectly fine fixed-pitch
    // replacement, hiding the problem from this code. Scintilla then
    // measures and positions every character as if it were on a fixed-
    // width grid, so a proportional font's naturally uneven glyph widths
    // (and its kerning) make neighbouring style runs land right on top of
    // each other or swallow the space between them - worst exactly at
    // syntax-colour boundaries, matching the reported symptom precisely.
    //
    // Fix: check whether the requested font *actually* resolves to a
    // fixed-pitch font on this system and, if not, fall back to
    // Fontconfig's "monospace" generic family. Being a generic alias (not
    // a real font name), Fontconfig always resolves it to a genuine
    // fixed-pitch font on any correctly configured Linux system, with no
    // hinting needed on our end - so it stays correct even for
    // QScintilla's own internal, unhinted font objects.
    QFontInfo resolvedFontInfo(font);
    if (!resolvedFontInfo.fixedPitch())
    {
        if(p_mydebug)
            qDebug() << "Preferred editor font" << font.family()
                      << "is not installed / not fixed-pitch on this system"
                      << "(resolved to" << resolvedFontInfo.family() << ") -"
                      << "falling back to the system default monospace font.";
        font = QFont(QStringLiteral("monospace"), 9);
        myfont = font;
        myfont.setFixedPitch(true);
        myfont.setStyleHint(QFont::Monospace, QFont::PreferMatch);
    }
#endif

    textEdit->setFont(myfont);
}

//
// initialize editor's margins with decent values according to text sizes in use
//
void MainWindow::initializeMargin(QsciScintilla *editor)
{
    if (!editor)
        editor = textEdit;

    QFontMetrics fontmetrics = QFontMetrics(editor->font());
    editor->setMarginsFont(editor->font());
    editor->setMarginWidth(0, fontmetrics.horizontalAdvance(QString::number(editor->lines())) + 10);
    editor->setMarginLineNumbers(0, true);

    if (isDarkTheme())
    {
        // Dark-theme margin colours - subdued line-number gutter, still
        // clearly separated from the editor background itself.
        editor->setMarginsBackgroundColor(QColor("#252526"));
        editor->setMarginsForegroundColor(QColor("#858585"));
        editor->setFoldMarginColors(QColor("#3c3c3c"), QColor("#252526"));
    }
    else
    {
        // Make background the same color than the applied stylesheet for MainWindow
        // rgb: (175, 175, 175), hex: (#afafaf)
#if !defined(__APPLE__)
        editor->setMarginsBackgroundColor(QColor("#afafaf"));
        editor->setFoldMarginColors(QColor("#808080"), QColor("#afafaf"));
#else
        editor->setMarginsBackgroundColor(QColor("#cccccccc"));
        editor->setFoldMarginColors(QColor("#808080"), QColor("#cccccccc"));
#endif
        editor->setMarginsForegroundColor(QColor("#ff0000ff"));
    }

    // resize line numbers margin if needed! Qt::UniqueConnection avoids
    // piling up duplicate connections when this runs again on an editor
    // that's already connected - happens routinely (every lexer switch
    // already called this before the "Dark" theme existed) and now also
    // whenever reapplyEditorTheme() re-applies colors to every open tab.
    connect(editor, SIGNAL(textChanged()), this, SLOT(fitMarginLines()), Qt::UniqueConnection);
}

//
// resize line numbers margin
//
// Connected per-tab (see initializeMargin(), called from newEditorTab()),
// so guard against a background tab's textChanged() reaching here - same
// reasoning as documentWasModified() above.
//
void MainWindow::fitMarginLines()
{
    QsciScintilla *edited = qobject_cast<QsciScintilla *>(sender());
    if (edited && edited != textEdit)
        return;

    QFontMetrics fontmetrics = textEdit->fontMetrics();
    textEdit->setMarginWidth(0, fontmetrics.horizontalAdvance(QString::number(textEdit->lines())) + 10);
}

//
// initialize plain text "lexer" (NO syntax highlighting!)
// e.g. KILL existing lexer...
//
void MainWindow::initializeLexerNone(QsciScintilla *editor, bool announceChange)
{
    if (!editor)
        editor = textEdit;

    // We want to get rid of the Lexer that is allready initialized!
    editor->setLexer(nullptr);
    // Now that it's gone - let's take care that everything looks like before...
    // (no lexer object anymore, so the widget's own font applies directly)
    editor->setFont(myfont);
    editor->SendScintilla(QsciScintilla::SCI_STYLESETCHARACTERSET, 1, QsciScintilla::SC_CHARSET_ANSI);

    // With no lexer attached, Scintilla's low-level STYLE_DEFAULT governs
    // the editor's own colours directly - apply the dark theme here too,
    // the same background/foreground pair used as the base for every
    // actual lexer in applyLexerDarkColors(). The "else" branch resetting
    // back to plain white/black is just as necessary as the dark one:
    // confirmed a plain-text tab stayed stuck showing dark colours
    // indefinitely after switching away from "Dark" to any other theme -
    // this function used to only ever handle "make it dark", with
    // nothing to undo that afterward.
    if (isDarkTheme())
    {
        editor->SendScintilla(QsciScintilla::SCI_STYLESETBACK, QsciScintilla::STYLE_DEFAULT, QColor(0x1e, 0x1e, 0x1e));
        editor->SendScintilla(QsciScintilla::SCI_STYLESETFORE, QsciScintilla::STYLE_DEFAULT, QColor(0xd4, 0xd4, 0xd4));
        editor->SendScintilla(QsciScintilla::SCI_STYLECLEARALL);
    }
    else
    {
        editor->SendScintilla(QsciScintilla::SCI_STYLESETBACK, QsciScintilla::STYLE_DEFAULT, QColor(Qt::white));
        editor->SendScintilla(QsciScintilla::SCI_STYLESETFORE, QsciScintilla::STYLE_DEFAULT, QColor(Qt::black));
        editor->SendScintilla(QsciScintilla::SCI_STYLECLEARALL);
    }

    initializeMargin(editor);
    // We don't want to have the fold margin visible, since it's plain text we're displaying...
    editor->setFolding(QsciScintilla::NoFoldStyle);
    // Make sure everything is unfolded!
    QsciScintilla::FoldStyle state = static_cast<QsciScintilla::FoldStyle>((!editor->folding()) * 5);
    if (!state)
    {
        editor->foldAll(false);
    }

    // 'announceChange' is false when reapplyEditorTheme() calls this purely
    // to recolor an already-plain-text tab for a live style switch - no
    // actual syntax change happened, so the status/debug messages below
    // (meant for the View > Syntax menu action) would be misleading noise.
    if (announceChange)
    {
        qDebug() << "Faltstatus: " << state;
        createStatusBarMessage(tr("Syntax changed to PlainText"), 0);
    }
}

//
// initialize C/C++ lexer bei default
//
void MainWindow::initializeLexerCPP()
{
    // AmigaLexerCPP (amigalexercpp.h) is a thin QsciLexerCPP subclass that
    // additionally recognises the AmigaOS NDK 3.2R4 data types from
    // exec/types.h (keyword set 4) and the AmigaOS NDK 3.2R4 library
    // function names (keyword set 2).
    AmigaLexerCPP *lexer = new AmigaLexerCPP();
    textEdit->setLexer(lexer);
    // Apply the app's fixed-pitch font to every style of this lexer -
    // otherwise QsciLexerCPP's own per-style default fonts would take
    // over as soon as a lexer is attached, silently undoing the
    // monospace font set in initializeFont().
    lexer->setFont(myfont);
    textEdit->SendScintilla(textEdit->QsciScintilla::SCI_STYLESETCHARACTERSET, 1, QsciScintilla::SC_CHARSET_ANSI);
    textEdit->setFolding(QsciScintilla::BoxedTreeFoldStyle);

    // QsciLexerCPP does not assign a distinct default colour to the
    // "GlobalClass" style, so without this the Amiga types would be
    // recognised but look like ordinary text. Indigo, bold - clearly
    // distinct from Keyword (dark blue), Number (teal), String (purple),
    // Comment (green) and PreProcessor (olive).
    lexer->setColor(QColor(0x4b, 0x00, 0x82), QsciLexerCPP::GlobalClass);
    QFont typeFont = lexer->font(QsciLexerCPP::GlobalClass);
    typeFont.setBold(true);
    lexer->setFont(typeFont, QsciLexerCPP::GlobalClass);

    // Same reasoning for "KeywordSet2" (the Amiga library function names):
    // firebrick red, not bold - functions occur far more often per line
    // than types, so bolding all of them would get visually noisy.
    lexer->setColor(QColor(0xb2, 0x22, 0x22), QsciLexerCPP::KeywordSet2);

    // Overrides paper/text/comment/keyword/.../GlobalClass/KeywordSet2
    // colours above with dark-theme-tuned ones instead - a no-op unless
    // the "Dark" application style is active (Prefs > Misc).
    applyLexerDarkColors(lexer);

    // Unlike every other initializeLexerXxx() function, this one was
    // missing its own initializeMargin() call - harmless for a brand new
    // tab (newEditorTab() already calls it once, right after creating
    // the tab's own initial C/C++ lexer), but setLexer() below runs a
    // SECOND time for any .c/.h/.cpp file actually opened afterward (via
    // applyLexerForFileExtension(), called from openFileInTab()) -
    // replacing that first lexer with a fresh one here reset the margin
    // to its own un-dark default colours, which nothing afterward ever
    // corrected back. Confirmed: a project's .c files opened this way
    // showed a light-gray line-number gutter even with the "Dark" style
    // active, while a freshly created blank tab (never re-lexed) did not.
    initializeMargin();

    createStatusBarMessage(tr("Syntax changed to C/C++"), 0);
}

//
// initialize lexer for Makefiles
//
void MainWindow::initializeLexerMakefile()
{
    QsciLexerMakefile *lexer = new QsciLexerMakefile;
    //lexer->setFoldComments(true);
    textEdit->setLexer(lexer);
    lexer->setFont(myfont);
    textEdit->SendScintilla(textEdit->QsciScintilla::SCI_STYLESETCHARACTERSET, 1, QsciScintilla::SC_CHARSET_ANSI);
    applyLexerDarkColors(lexer);   // applies dark-theme colours, or resets to this app's own light-theme ones - see its own comment

    textEdit->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    initializeMargin();
    createStatusBarMessage(tr("Syntax changed to Makefiles"), 0);
}

//
// initialize lexer for Batch files
//
void MainWindow::initializeLexerBatch()
{
    QsciLexerBatch *lexer = new QsciLexerBatch;
    //lexer->setFoldComments(true);
    textEdit->setLexer(lexer);
    lexer->setFont(myfont);
    textEdit->SendScintilla(textEdit->QsciScintilla::SCI_STYLESETCHARACTERSET, 1, QsciScintilla::SC_CHARSET_ANSI);
    applyLexerDarkColors(lexer);   // applies dark-theme colours, or resets to this app's own light-theme ones - see its own comment

    textEdit->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    initializeMargin();
    createStatusBarMessage(tr("Syntax changed to Shell"), 0);
}

//
// initialize lexer for Amiga Installer scripts
//
void MainWindow::initializeLexerInstaller()
{
    AmigaInstallerLexer *lexer = new AmigaInstallerLexer(this);
    textEdit->setLexer(lexer);
    lexer->setFont(myfont);
    textEdit->SendScintilla(textEdit->QsciScintilla::SCI_STYLESETCHARACTERSET, 1, QsciScintilla::SC_CHARSET_ANSI);
    applyLexerDarkColors(lexer);   // applies dark-theme colours, or resets to this app's own light-theme ones - see its own comment

    // The Installer language is fully parenthesized (LISP-style), so
    // brace/paren matching is far more useful here than the C-style
    // block folding used for C/C++ - box folding relies on Scintilla's
    // own fold-point detection, which our custom lexer does not (yet)
    // provide, so folding is disabled instead of showing an empty margin.
    textEdit->setFolding(QsciScintilla::NoFoldStyle);
    initializeMargin();
    createStatusBarMessage(tr("Syntax changed to Amiga installer"), 0);
}

//
// initialize lexer for AmigaGuide files
//
void MainWindow::initializeLexerAmigaGuide()
{
    AmigaGuideLexer *lexer = new AmigaGuideLexer(this);
    textEdit->setLexer(lexer);
    lexer->setFont(myfont);
    textEdit->SendScintilla(textEdit->QsciScintilla::SCI_STYLESETCHARACTERSET, 1, QsciScintilla::SC_CHARSET_ANSI);
    applyLexerDarkColors(lexer);   // applies dark-theme colours, or resets to this app's own light-theme ones - see its own comment

    // Like the Installer lexer above, AmigaGuideLexer doesn't provide
    // Scintilla fold points (its '@node'/'@endnode' pairing isn't
    // brace-like enough to reuse SloppyBraceMatch either) - no folding
    // margin rather than an empty, non-functional one.
    textEdit->setFolding(QsciScintilla::NoFoldStyle);
    initializeMargin();
    createStatusBarMessage(tr("Syntax changed to AmigaGuide"), 0);
}

//
// initialize lexer for m68k assembler files
//
void MainWindow::initializeLexerM68kAsm()
{
    M68kAsmLexer *lexer = new M68kAsmLexer(this);
    textEdit->setLexer(lexer);
    lexer->setFont(myfont);
    textEdit->SendScintilla(textEdit->QsciScintilla::SCI_STYLESETCHARACTERSET, 1, QsciScintilla::SC_CHARSET_ANSI);
    applyLexerDarkColors(lexer);   // applies dark-theme colours, or resets to this app's own light-theme ones - see its own comment

    // No Scintilla fold points here either (see AmigaGuideLexer above) -
    // assembler source isn't brace-delimited the way C/C++ is.
    textEdit->setFolding(QsciScintilla::NoFoldStyle);
    initializeMargin();
    createStatusBarMessage(tr("Syntax changed to m68k Assembler"), 0);
}

//
// initialize lexer for Pascal
//
void MainWindow::initializeLexerPascal()
{
    QsciLexerPascal *lexer = new QsciLexerPascal;
    //lexer->setFoldComments(true);
    textEdit->setLexer(lexer);
    lexer->setFont(myfont);
    textEdit->SendScintilla(textEdit->QsciScintilla::SCI_STYLESETCHARACTERSET, 1, QsciScintilla::SC_CHARSET_ANSI);
    applyLexerDarkColors(lexer);   // applies dark-theme colours, or resets to this app's own light-theme ones - see its own comment

    textEdit->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    initializeMargin();
    createStatusBarMessage(tr("Syntax changed to Pascal"), 0);
}

//
// give decent values to carret line
//
void MainWindow::initializeCaretLine(QsciScintilla *editor)
{
    if (!editor)
        editor = textEdit;

    // Current line visible with special background color
    editor->setCaretLineVisible(false);

    if (isDarkTheme())
    {
        editor->setCaretLineBackgroundColor(QColor("#2d2d2d"));
        editor->setCaretForegroundColor(QColor("#ffffff"));
        // Selection background only - deliberately no
        // setSelectionForegroundColor() call, so each token keeps its own
        // syntax-highlighting colour when selected instead of all text
        // turning into one flat colour.
        editor->setSelectionBackgroundColor(QColor("#264f78"));
        editor->setIndentationGuidesForegroundColor(QColor("#3b3b3b"));
        editor->setIndentationGuidesBackgroundColor(QColor("#1e1e1e"));
        editor->setWhitespaceForegroundColor(QColor("#3b3b3b"));
        editor->setWhitespaceBackgroundColor(QColor("#1e1e1e"));
        editor->setMatchedBraceForegroundColor(QColor("#ffd700"));
        editor->setMatchedBraceBackgroundColor(QColor("#3a3d41"));
        editor->setUnmatchedBraceForegroundColor(QColor("#f44747"));
        editor->setUnmatchedBraceBackgroundColor(QColor("#1e1e1e"));
    }
    else
    {
        // Every property the dark branch above sets needs an explicit
        // light-theme counterpart here too - confirmed this branch used
        // to only reset setCaretLineBackgroundColor(), leaving selection
        // background, indentation guides, whitespace, and matched/
        // unmatched brace colours stuck showing dark-theme values
        // indefinitely after switching away from "Dark".
        editor->setCaretLineBackgroundColor(QColor("#a7edfe"));
        editor->setCaretForegroundColor(QColor("#000000"));
        editor->setSelectionBackgroundColor(QColor("#add6ff"));
        editor->setIndentationGuidesForegroundColor(QColor("#c0c0c0"));
        editor->setIndentationGuidesBackgroundColor(QColor("#ffffff"));
        editor->setWhitespaceForegroundColor(QColor("#c0c0c0"));
        editor->setWhitespaceBackgroundColor(QColor("#ffffff"));
        editor->setMatchedBraceForegroundColor(QColor("#0000ff"));
        editor->setMatchedBraceBackgroundColor(QColor("#b4eeb4"));
        editor->setUnmatchedBraceForegroundColor(QColor("#ff0000"));
        editor->setUnmatchedBraceBackgroundColor(QColor("#ffffff"));
    }
}

//
// Menu View/Navigation entry "Fold/Unfold all" - forces every fold point
// in the current document to the same explicit state.
//
// Rewritten (rev.96): the previous version toggled based on
// textEdit->folding() (whether the fold MARGIN/style is currently
// enabled at all - a per-lexer display setting, set independently in
// initializeLexerXxx()) and then called QScintilla's foldAll(bool),
// which toggles each top-level fold header based on ITS OWN current
// state. That combination had two problems: it conflated "is the fold
// margin shown" with "is the document folded", and foldAll() itself
// produces an inconsistent result whenever the document starts in a
// MIXED state (some blocks already folded/unfolded by hand via the
// margin's +/- markers) - exactly the case this menu entry is supposed
// to handle cleanly.
//
// This version leaves the fold margin/style alone entirely (that
// remains whatever initializeLexerXxx() set it to for this file type)
// and instead sends Scintilla's own SCI_FOLDALL message with an
// explicit CONTRACT or EXPAND action, which forces every single fold
// point in the document to that exact state in one call, regardless of
// whatever mixed state it started in. foldall (bool) simply remembers
// which state this command last forced, so repeated presses alternate
// fold/unfold rather than needing to inspect Scintilla's (potentially
// still-mixed) per-line state to decide what "the opposite" would mean.
//
void MainWindow::initializeFolding()
{
    if (!textEdit)
        return;

    if (textEdit->folding() == QsciScintilla::NoFoldStyle)
    {
        createStatusBarMessage(tr("Folding is not available for this file type."), 3000);
        return;
    }

    foldall = !foldall;
    const int action = foldall ? QsciScintillaBase::SC_FOLDACTION_CONTRACT
                                : QsciScintillaBase::SC_FOLDACTION_EXPAND;
    textEdit->SendScintilla(QsciScintillaBase::SCI_FOLDALL, action);

    createStatusBarMessage(foldall ? tr("All folds collapsed.") : tr("All folds expanded."), 2000);
}

//
// show current cursor position and display
// line and row in app's status bar's LCD widgets
//
// Connected per-tab (see newEditorTab()) to cursorPositionChanged(), so
// guard against a background tab's signal reaching here - same reasoning
// as documentWasModified() above. Also called directly (sender() == null)
// from onTabChanged() to refresh the display after switching tabs, which
// must always go through.
//
void MainWindow::showCurrendCursorPosition()
{
    QsciScintilla *edited = qobject_cast<QsciScintilla *>(sender());
    if (edited && edited != textEdit)
        return;

    int line, index;
    textEdit->getCursorPosition(&line, &index);
    // did we want LCD display?
    if(!(p_no_lcd_statusbar))
    {
        statusLCD_X->display(line + 1);
        statusLCD_Y->display(index +1);
    }
    // ...or did we want plain text display?
    else
    {
        statusContainer_X->setText(QString::number(line + 1));
        statusContainer_Y->setText(QString::number(index + 1));
    }
}

//
// this method initializes the whole GUI!
//
//
// Build the project panel: a QTreeWidget (grouped by file type) plus
// Add/Remove buttons, shown to the left of the tabs (AmigaED v3.3).
// Called once from the constructor, before any project is loaded.
//
void MainWindow::createProjectPanel()
{
    projectPanel = new QWidget(this);

    // Group the tree and its Add/Remove buttons together visually, with a
    // clear "Project" label, instead of having them float directly in the
    // splitter pane.
    projectGroupBox = new QGroupBox(tr("Project"), projectPanel);

    projectTree = new QTreeWidget(projectGroupBox);
    projectTree->setHeaderHidden(true);
    projectTree->setColumnCount(1);
    projectTree->setContextMenuPolicy(Qt::CustomContextMenu);
    projectTree->setAcceptDrops(true);            // drop .c/.h/.cpp files or a Makefile here to add them to the project - see eventFilter()
    projectTree->viewport()->installEventFilter(this);
    connect(projectTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(onProjectTreeDoubleClicked(QTreeWidgetItem *, int)));
    connect(projectTree, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onProjectTreeContextMenu(const QPoint &)));

    projectSourceGroupItem = new QTreeWidgetItem(projectTree, QStringList(tr("C/C++ Files")));
    projectHeaderGroupItem = new QTreeWidgetItem(projectTree, QStringList(tr("Header Files")));
    projectInstallerGroupItem = new QTreeWidgetItem(projectTree, QStringList(tr("Installer Scripts")));
    projectAmigaGuideGroupItem = new QTreeWidgetItem(projectTree, QStringList(tr("AmigaGuide")));
    projectAssemblyGroupItem = new QTreeWidgetItem(projectTree, QStringList(tr("Assembler Sources")));
    projectExecutableGroupItem = new QTreeWidgetItem(projectTree, QStringList(tr("Executable")));
    projectOtherGroupItem = new QTreeWidgetItem(projectTree, QStringList(tr("Other Files")));
    projectMakefileGroupItem = new QTreeWidgetItem(projectTree, QStringList(tr("Makefiles")));
    for (QTreeWidgetItem *group : {projectSourceGroupItem, projectHeaderGroupItem, projectInstallerGroupItem, projectAmigaGuideGroupItem, projectAssemblyGroupItem, projectExecutableGroupItem, projectOtherGroupItem, projectMakefileGroupItem})
    {
        QFont f = group->font(0);
        f.setBold(true);
        group->setFont(0, f);
        group->setFlags(Qt::ItemIsEnabled);   // category headers: not selectable, not draggable
        group->setIcon(0, QIcon(":/images/drawer.png"));
    }
    projectTree->expandAll();

    addProjectFileBtn = new QPushButton(tr("Add..."), projectGroupBox);
    addProjectFileBtn->setStatusTip(tr("Add an existing file to the project"));
    connect(addProjectFileBtn, SIGNAL(clicked(bool)), this, SLOT(actionAddFileToProject()));

    removeProjectFileBtn = new QPushButton(tr("Remove"), projectGroupBox);
    removeProjectFileBtn->setStatusTip(tr("Remove the selected file from the project"));
    connect(removeProjectFileBtn, SIGNAL(clicked(bool)), this, SLOT(actionRemoveFileFromProject()));

    QHBoxLayout *buttonRow = new QHBoxLayout();
    buttonRow->addWidget(addProjectFileBtn);
    buttonRow->addWidget(removeProjectFileBtn);

    QVBoxLayout *groupLayout = new QVBoxLayout(projectGroupBox);
    groupLayout->addWidget(projectTree);
    groupLayout->addLayout(buttonRow);

    QVBoxLayout *panelLayout = new QVBoxLayout(projectPanel);
    panelLayout->addWidget(projectGroupBox);
    panelLayout->setContentsMargins(2, 2, 2, 2);
}

//
// Rebuild the tree's file entries from 'currentProject', grouped under the
// three category headers. Category headers themselves are never removed;
// only their children are cleared and re-added. The currently designated
// main compile file is marked with a " [main]" suffix and bold font.
//
//
// Human-readable file size for the "Executable" tree category (e.g.
// "42.3 KB", "1.2 MB") - matches the common convention of using 1024-byte
// units while still labeling them "KB"/"MB" (not "KiB"/"MiB"), since
// that's what most users expect to see in a file browser.
//
static QString formatFileSize(qint64 bytes)
{
    static const char *units[] = { "bytes", "KB", "MB", "GB" };
    double size = bytes;
    int unitIndex = 0;
    while (size >= 1024.0 && unitIndex < 3)
    {
        size /= 1024.0;
        ++unitIndex;
    }
    if (unitIndex == 0)
        return QStringLiteral("%1 %2").arg(bytes).arg(units[unitIndex]);
    return QStringLiteral("%1 %2").arg(size, 0, 'f', 1).arg(units[unitIndex]);
}

void MainWindow::refreshProjectTree()
{
    qDeleteAll(projectSourceGroupItem->takeChildren());
    qDeleteAll(projectHeaderGroupItem->takeChildren());
    qDeleteAll(projectInstallerGroupItem->takeChildren());
    qDeleteAll(projectAmigaGuideGroupItem->takeChildren());
    qDeleteAll(projectAssemblyGroupItem->takeChildren());
    qDeleteAll(projectExecutableGroupItem->takeChildren());
    qDeleteAll(projectOtherGroupItem->takeChildren());
    qDeleteAll(projectMakefileGroupItem->takeChildren());

    if (!currentProject)
        return;

    for (const ProjectFile &f : currentProject->files)
    {
        QTreeWidgetItem *parent = projectOtherGroupItem;
        if (f.type == ProjectFileType::CSource)
            parent = projectSourceGroupItem;
        else if (f.type == ProjectFileType::Header)
            parent = projectHeaderGroupItem;
        else if (f.type == ProjectFileType::InstallerScript)
            parent = projectInstallerGroupItem;
        else if (f.type == ProjectFileType::AmigaGuide)
            parent = projectAmigaGuideGroupItem;
        else if (f.type == ProjectFileType::Assembly)
            parent = projectAssemblyGroupItem;
        else if (f.type == ProjectFileType::Executable)
            parent = projectExecutableGroupItem;

        bool isMain = (f.type == ProjectFileType::CSource &&
                        !currentProject->mainFile.isEmpty() &&
                        QFileInfo(f.path) == QFileInfo(currentProject->mainFile));

        QString label = strippedName(f.path);
        if (isMain)
            label += tr(" [main]");
        if (f.type == ProjectFileType::Executable)
            label += QStringLiteral(" (%1)").arg(formatFileSize(QFileInfo(f.path).size()));

        QTreeWidgetItem *item = new QTreeWidgetItem(parent, QStringList(label));
        item->setData(0, Qt::UserRole, f.path);
        item->setToolTip(0, f.path);

        if (f.type == ProjectFileType::CSource)
            item->setIcon(0, QIcon(":/images/filetype_c.png"));
        else if (f.type == ProjectFileType::Header)
            item->setIcon(0, QIcon(":/images/filetype_h.png"));
        else if (f.type == ProjectFileType::InstallerScript)
            item->setIcon(0, QIcon(":/images/filetype_installer.png"));
        else if (f.type == ProjectFileType::AmigaGuide)
            item->setIcon(0, QIcon(":/images/filetype_guide.png"));
        else if (f.type == ProjectFileType::Assembly)
            item->setIcon(0, QIcon(":/images/filetype_asm.png"));
        else if (f.type == ProjectFileType::Executable)
            item->setIcon(0, QIcon(":/images/filetype_exe.png"));
        else if (f.type == ProjectFileType::Other)
        {
            // "Other" covers anything not otherwise categorized - most of
            // it has no sensible icon of its own, but .txt/.readme (and
            // a bare, extension-less "README"/"readme") and .pas are
            // common enough in an Amiga project (docs, release notes,
            // Pascal sources) to be worth their own icon rather than
            // showing no icon at all.
            QFileInfo fi(f.path);
            QString suffix = fi.suffix().toLower();
            if (suffix == "readme" || fi.fileName().compare(QStringLiteral("readme"), Qt::CaseInsensitive) == 0)
                item->setIcon(0, QIcon(":/images/filetype_readme.png"));
            else if (suffix == "txt")
                item->setIcon(0, QIcon(":/images/filetype_txt.png"));
            else if (suffix == "pas")
                item->setIcon(0, QIcon(":/images/filetype_pas.png"));
        }

        if (isMain)
        {
            QFont mf = item->font(0);
            mf.setBold(true);
            item->setFont(0, mf);
        }
    }

    // Makefiles are auto-generated (see regenerateProjectMakefiles()), not
    // tracked in Project::files/the .aep - so list whichever of them
    // actually exist on disk right now, freshly, every time. Since this
    // rebuilds the whole "Makefiles" branch from scratch (the takeChildren()
    // above), re-scanning the same fixed names can never produce a
    // duplicate entry.
    const QStringList makefileNames = { "Makefile.gcc", "Makefile.vbcc", "Makefile.sc" };
    QString dir = currentProject->projectDir();
    for (const QString &makefileName : makefileNames)
    {
        QString path = dir + QDir::separator() + makefileName;
        if (!QFileInfo::exists(path))
            continue;

        QTreeWidgetItem *item = new QTreeWidgetItem(projectMakefileGroupItem, QStringList(makefileName));
        item->setData(0, Qt::UserRole, path);
        item->setToolTip(0, path);
        item->setIcon(0, QIcon(":/images/filetype_makefile.png"));
    }

    // Like the Makefiles above, the project's own freshly built executable
    // isn't tracked in Project::files/the .aep either - it's a build
    // artifact regenerated (and deleted by "Clean") on demand, not
    // something to persist. Scanned fresh every refresh, by the same name
    // actionBuildProject()/regenerateProjectMakefiles() already compute as
    // the link target - so this reliably picks it up right after a
    // successful build (see stopCommand()) without AmigaED needing to
    // remember anything about it between runs.
    //
    // Skipped if that same path is already shown via the Project::files
    // loop above (an imported foreign project's pre-existing executable
    // - see Project::typeForFile()'s Amiga-hunk-magic-number check -
    // could in principle share its project's own target name) - avoids
    // ever listing the same file twice.
    {
        QString targetName = currentProject->name;
        targetName.replace(QRegularExpression("[^A-Za-z0-9_\\-]"), "_");
        QString exePath = dir + QDir::separator() + targetName;

        if (!targetName.isEmpty() && QFileInfo::exists(exePath) && QFileInfo(exePath).isFile() &&
            !currentProject->contains(exePath))
        {
            QString label = targetName + QStringLiteral(" (%1)").arg(formatFileSize(QFileInfo(exePath).size()));
            QTreeWidgetItem *item = new QTreeWidgetItem(projectExecutableGroupItem, QStringList(label));
            item->setData(0, Qt::UserRole, exePath);
            item->setToolTip(0, exePath);
            item->setIcon(0, QIcon(":/images/filetype_exe.png"));
        }
    }

    projectTree->expandAll();
}

//
// Build the "Functions" panel: a QTreeWidget (grouped by file) inside a
// QGroupBox, shown to the right of the editor (AmigaED v3.4).
//
void MainWindow::createFunctionsPanel()
{
    functionsGroupBox = new QGroupBox(tr("Functions"), this);

    functionsTree = new QTreeWidget(functionsGroupBox);
    functionsTree->setHeaderHidden(true);
    functionsTree->setColumnCount(1);
    connect(functionsTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(onFunctionsTreeDoubleClicked(QTreeWidgetItem *, int)));

    QVBoxLayout *groupLayout = new QVBoxLayout(functionsGroupBox);
    groupLayout->addWidget(functionsTree);
}

//
// Heuristic (regex-based) scan for C/C++ function DEFINITIONS in a source
// file - not a real parser. Looks for "<type> <name>(<params>) {" allowing
// the opening brace on its own line (K&R style), and skips anything that
// ends in ';' (prototypes/declarations) or is a control-flow keyword that
// happens to superficially match (if/for/while/...). This will miss some
// unusual formatting and can occasionally misfire on tricky macros, but
// covers ordinary Amiga C/C++ code well enough to be useful for
// navigation. Returns (function name, 0-based line number) pairs.
//
QList<QPair<QString, int>> MainWindow::scanFunctionsInFile(const QString &filePath) const
{
    QList<QPair<QString, int>> result;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return result;

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Latin1);   // see loadFile() for rationale
    QString content = in.readAll();
    file.close();

    static const QRegularExpression re(
        "^[ \\t]*[A-Za-z_][A-Za-z0-9_:<>]*(?:[ \\t]+[A-Za-z_][A-Za-z0-9_:<>]*)*[ \\t\\*&]+"
        "([A-Za-z_~][A-Za-z0-9_]*)"
        "[ \\t]*\\(([^;{}]*)\\)"
        "\\s*(?:const\\s*)?"   // '\s' (not just '[ \t]') so a brace on its own
        "\\{",                 // line (K&R style) still matches across the newline
        QRegularExpression::MultilineOption);

    static const QSet<QString> keywordExclusions = {
        "if", "for", "while", "switch", "return", "sizeof", "typedef",
        "struct", "union", "enum", "else", "do"
    };

    QRegularExpressionMatchIterator it = re.globalMatch(content);
    while (it.hasNext())
    {
        QRegularExpressionMatch m = it.next();
        QString name = m.captured(1);
        if (keywordExclusions.contains(name))
            continue;

        int offset = m.capturedStart(1);
        int line = content.left(offset).count(QLatin1Char('\n'));   // 0-based
        result.append(qMakePair(name, line));
    }

    return result;
}

//
// Rebuild the "Functions" tree from currentProject's C/C++ files, grouped
// by file. Called both periodically (functionsRefreshTimer, every 10s -
// see the constructor) and immediately at the points where the project's
// files actually change (load/create, add/remove file, successful save),
// so in normal use the list is already up to date well before the timer
// would ever fire.
//
void MainWindow::refreshFunctionsList()
{
    if (!functionsTree)
        return;

    functionsTree->clear();

    if (!currentProject)
        return;

    for (const ProjectFile &f : currentProject->files)
    {
        // Scan headers too, not just .c/.cpp files - some lightweight
        // Amiga C headers define small functions directly (not just
        // prototypes), and the regex in scanFunctionsInFile() already
        // requires a matching '{' right after the parameter list, so a
        // header with only plain ';'-terminated prototypes simply won't
        // match anything - no false positives from scanning it too.
        if (f.type != ProjectFileType::CSource && f.type != ProjectFileType::Header)
            continue;

        QList<QPair<QString, int>> functions = scanFunctionsInFile(f.path);
        if (functions.isEmpty())
            continue;

        QTreeWidgetItem *fileItem = new QTreeWidgetItem(functionsTree, QStringList(strippedName(f.path)));
        QFont ff = fileItem->font(0);
        ff.setBold(true);
        fileItem->setFont(0, ff);
        fileItem->setFlags(Qt::ItemIsEnabled);   // category header: not selectable
        fileItem->setIcon(0, QIcon(":/images/drawer.png"));

        for (const auto &fn : std::as_const(functions))
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(fileItem, QStringList(fn.first + "()"));
            item->setData(0, Qt::UserRole, f.path);
            item->setData(0, Qt::UserRole + 1, fn.second);
            item->setToolTip(0, tr("%1, line %2").arg(strippedName(f.path)).arg(fn.second + 1));
        }
    }

    functionsTree->expandAll();
}

//
// Double-clicking a function entry opens its file (or switches to its
// already-open tab - see openFileInTab()) and moves the cursor to the
// start of that function's line. Double-clicking a file header node
// (which carries no Qt::UserRole path) does nothing.
//
void MainWindow::onFunctionsTreeDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if (!item)
        return;

    QString path = item->data(0, Qt::UserRole).toString();
    if (path.isEmpty())
        return;

    int line = item->data(0, Qt::UserRole + 1).toInt();

    openFileInTab(path);
    if (!textEdit)
        return;

    textEdit->setFocus();

    // check if text is folded - same convention as jumpToError()
    QsciScintilla::FoldStyle state = static_cast<QsciScintilla::FoldStyle>((!textEdit->folding()) * 5);
    if (state > 0)
        textEdit->foldAll(false);

    textEdit->setCursorPosition(line, 0);
    textEdit->ensureLineVisible(line);
}

//
// Ask for extra compiler/linker options before a new project's Makefiles
// are generated. Cancelling either dialog aborts project creation - an
// empty answer (just pressing OK) means "no extra options".
//
// Pre-fills each field with the CURRENTLY selected compiler's own Prefs
// baseline for the effective target OS (see getCompilerAndLinkerOptsForTarget()
// - a single source of truth also used by SelectCompiler()/
// setDefaultTargetOS() for single-file compiles, and by
// regenerateProjectMakefiles() for the generated Makefile), so what the
// user sees here always matches what actually gets used elsewhere. The
// user can still edit or clear the pre-filled text before confirming -
// it's purely a convenient starting point.
//
// An explicit "AmigaOS 1.3/3.x Project" template overrides whatever the
// status bar's target-OS gadget currently happens to show, mirroring
// applyProjectTargetOSIfNeeded() (which performs the same override right
// after the project is actually created).
//
bool MainWindow::promptCompilerLinkerOptions(QString &compilerOpts, QString &linkerOpts, int templateKind)
{
    bool ok = false;

    int effectiveTarget = p_compiler_vc_default_target;
    if (templateKind == 2)
        effectiveTarget = 0;   // "AmigaOS 1.3 Project" -> OS 1.3, regardless of the gadget's current state
    else if (templateKind == 3)
        effectiveTarget = 1;   // "AmigaOS 3.x Project" -> OS 3.x, regardless of the gadget's current state

    QString defaultCompilerOpts, defaultLinkerOpts;
    getCompilerAndLinkerOptsForTarget(p_defaultCompiler, effectiveTarget, defaultCompilerOpts, defaultLinkerOpts);
    defaultCompilerOpts = dedupTokens(defaultCompilerOpts);
    defaultLinkerOpts = dedupTokens(defaultLinkerOpts);

    compilerOpts = QInputDialog::getText(this, tr("Compiler Options"),
                                          tr("Additional compiler options for this project (optional):"),
                                          QLineEdit::Normal, defaultCompilerOpts, &ok);
    if (!ok)
        return false;

    linkerOpts = QInputDialog::getText(this, tr("Linker Options"),
                                        tr("Additional linker options for this project (optional):"),
                                        QLineEdit::Normal, defaultLinkerOpts, &ok);
    if (!ok)
        return false;

    return true;
}

//
// (Re-)write Makefile.gcc (m68k-amigaos-gcc), Makefile.vbcc (vc), and -
// for projects that include at least one plain .c file - Makefile.sc
// (SAS/C) in the project's directory, from its current file list. Called
// automatically whenever a file is added to or removed from the project.
// Makefile.gcc/Makefile.vbcc build ".c" and ".asm"/".s" sources (a C++
// project's '.cpp' files still need their own pattern rule added here,
// which isn't done yet). Makefile.sc is never invoked by AmigaED itself -
// SAS/C only runs on a real Amiga/emulator, so it's purely generated for
// manual use there later.
//
// Only CSource/Assembly-typed project files (see Project::typeForFile())
// ever end up in a Makefile's SRCS/OBJS - anything else tracked in the
// project (AmigaGuide docs, README/txt, Installer scripts, ...) is
// deliberately never treated as buildable source, no matter what it's
// named.
//
//
// Heuristic (not a real parser): true if any of the project's own C/C++
// source/header files contain "float" or "double" as a standalone word -
// used by regenerateProjectMakefiles() to decide whether the generated
// Makefiles need to add each toolchain's floating-point math library
// (gcc: -lm, vbcc: -lmieee, SAS/C: MATH=IEEE), since none of the three
// link one in by default. \b...\b word-boundary matching avoids false
// positives from identifiers that merely contain "float"/"double" as a
// substring (e.g. "floatValue", "doubleClick") - it does NOT try to
// distinguish real code from comments/string literals, so a source file
// that only ever *mentions* float/double in a comment will still (mildly
// over-cautiously) count as "uses floating point".
//
// Case-insensitive: exec/types.h's own FLOAT/DOUBLE typedefs (all caps)
// are at least as common in idiomatic Amiga C as the plain lowercase
// keywords - confirmed this heuristic missed a project using "DOUBLE"
// entirely with a case-sensitive match, silently generating Makefiles
// with no math library linked in at all.
//
bool MainWindow::projectUsesFloatingPoint() const
{
    if (!currentProject)
        return false;

    static const QRegularExpression floatRe(QStringLiteral("\\b(float|double)\\b"),
                                              QRegularExpression::CaseInsensitiveOption);

    for (const ProjectFile &f : currentProject->files)
    {
        if (f.type != ProjectFileType::CSource && f.type != ProjectFileType::Header)
            continue;

        QFile file(f.path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            continue;

        // Latin1, not UTF-8: matches how AmigaED reads/edits source files
        // everywhere else (8-bit Amiga text, not Unicode) - see loadFile().
        QString content = QString::fromLatin1(file.readAll());
        if (floatRe.match(content).hasMatch())
            return true;
    }

    return false;
}

void MainWindow::regenerateProjectMakefiles()
{
    if (!currentProject)
        return;

    QString dir = currentProject->projectDir();

    QString targetName = currentProject->name;
    targetName.replace(QRegularExpression("[^A-Za-z0-9_\\-]"), "_");

    QStringList sources;
    for (const ProjectFile &f : currentProject->files)
        if (f.type == ProjectFileType::CSource)
            sources << QFileInfo(f.path).fileName();

    // .asm/.s (m68k assembler) sources build alongside the C/C++ ones -
    // each toolchain needs its own assembler invoked for them though
    // (see the AS variable and %.o: %.asm/%.o: %.s rules in
    // writeMakefile() below): vasm directly for vbcc (vc itself doesn't
    // assemble hand-written .asm files), gcc's own "-x assembler" for
    // m68k-amigaos-gcc (which otherwise only recognizes ".s"/".S" by
    // extension, not ".asm").
    QStringList asmSources;
    for (const ProjectFile &f : currentProject->files)
        if (f.type == ProjectFileType::Assembly)
            asmSources << QFileInfo(f.path).fileName();

    if (sources.isEmpty() && asmSources.isEmpty())
    {
        // Nothing to build yet - leave any existing Makefiles alone, but
        // let the user know why none were (re-)generated, rather than
        // failing silently.
        createStatusBarMessage(tr("No C/C++/assembler source file in the project yet - Makefiles not (re)generated."), 4000);
        return;
    }

    // SAS/C doesn't support C++ (or being handed a separate assembler
    // step here) - only plain ".c" files go into Makefile.sc (see below).
    // A pure C++ or assembly-only project simply won't get one.
    QStringList scSources;
    for (const ProjectFile &f : currentProject->files)
        if (f.type == ProjectFileType::CSource && QFileInfo(f.path).suffix().compare("c", Qt::CaseInsensitive) == 0)
            scSources << QFileInfo(f.path).fileName();

    QStringList objs;
    for (const QString &s : sources)
    {
        QString o = s;
        int dot = o.lastIndexOf('.');
        if (dot >= 0)
            o = o.left(dot);
        objs << o + ".o";
    }
    QStringList asmObjs;
    for (const QString &s : asmSources)
    {
        QString o = s;
        int dot = o.lastIndexOf('.');
        if (dot >= 0)
            o = o.left(dot);
        asmObjs << o + ".o";
    }

    QString srcList = sources.join(" ");
    QString objList = (objs + asmObjs).join(" ");   // linked together - the C/C++ vs. assembler split only matters for which pattern rule builds each .o

    bool writeFailed = false;
    QStringList failedFiles;

    // vbcc's "vc" frontend accepts only ONE '+config' switch, and it must
    // be the very first argument on every invocation. Strip out any other
    // '+'-prefixed token from a given options string, keeping everything
    // else (e.g. -cpu=/-fpu=) intact.
    auto stripPlusPrefixedTokens = [](const QString &args) -> QString
    {
        QStringList kept;
        const QStringList tokens = args.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        for (const QString &token : tokens)
            if (!token.startsWith('+'))
                kept << token;
        return kept.join(" ");
    };

    // Which target OS applies to this project: an explicit "AmigaOS
    // 1.3/3.x Project" template overrides whatever the status bar's
    // target-OS gadget currently happens to show (mirrors
    // promptCompilerLinkerOptions(), so what the user confirmed there and
    // what ends up in the Makefile always agree); any other template
    // falls back to the live gadget value.
    int effectiveTarget = p_compiler_vc_default_target;
    if (currentProject->templateKind == 2)
        effectiveTarget = 0;
    else if (currentProject->templateKind == 3)
        effectiveTarget = 1;

    // vbcc's "vc" frontend requires a '+config' target selection (e.g.
    // '+aos68k') as the very first argument on EVERY invocation - without
    // it, vc fails immediately ("No match for +config" or similar). This
    // is exactly what AmigaED's single-file compile flow already supplies
    // via the Prefs > VBCC compiler-opts fields (chosen by the effective
    // OS target) - reuse the same string here so the generated Makefile
    // actually works, instead of silently omitting it.
    QString vbccCompilerOptsBaseline, vbccLinkerOptsBaseline;
    getCompilerAndLinkerOptsForTarget(0, effectiveTarget, vbccCompilerOptsBaseline, vbccLinkerOptsBaseline);

    QString vbccAlwaysFirstArgs;
    QString vbccCFlags = currentProject->extraVbccCompilerOptions;
    QString vbccLDFlags = dedupTokens(vbccLinkerOptsBaseline + " " + currentProject->extraVbccLinkerOptions);

    if (currentProject->templateKind == 2)   // "AmigaOS 1.3 Project" template
    {
        // This project explicitly targets AmigaOS 1.3/Kickstart 1.3 - vc
        // MUST be called with '+kick13' as its one and only '+' switch,
        // regardless of whatever the user's GLOBAL VBCC OS-target
        // preference currently happens to be set to (that preference is
        // per-app, not per-project, and might well be set to 3.x). Keep
        // any non-'+' opts configured for OS 1.3 in Prefs (e.g.
        // -cpu=/-fpu=), but strip any stray '+' switch out of both that
        // string and the project's own extra compiler/linker options, so
        // '+kick13' is guaranteed to be the only one present.
        QString base = stripPlusPrefixedTokens(vbccCompilerOptsBaseline);
        vbccAlwaysFirstArgs = base.isEmpty() ? QStringLiteral("+kick13") : (QStringLiteral("+kick13 ") + base);
        vbccCFlags = stripPlusPrefixedTokens(vbccCFlags);
        vbccLDFlags = stripPlusPrefixedTokens(vbccLDFlags);
    }
    else
    {
        vbccAlwaysFirstArgs = vbccCompilerOptsBaseline;   // already starts with "+aos68k"/"+kick13" per its own Prefs field
    }

    // m68k-amigaos-gcc's own configured baseline (Prefs > GCC > "GCC OS
    // 1.3/3.x compiler opts", chosen by the effective target) applies
    // unconditionally, mirroring vbcc's vbccAlwaysFirstArgs above - like
    // vbcc's '+kick13', an OS-1.3 project's '-mcrt=nix13' must be present
    // at BOTH compile and link time, so it lives in the OS-1.3 compiler-
    // opts field and goes into CCARGS rather than just CFLAGS. Makefile.gcc
    // always invokes gcc itself (never g++, see the writeMakefile() call
    // below), so compiler index 1 (GCC) is used unconditionally here.
    QString gccCompilerOptsBaseline, gccLinkerOptsBaseline;
    getCompilerAndLinkerOptsForTarget(1, effectiveTarget, gccCompilerOptsBaseline, gccLinkerOptsBaseline);
    QString gccAlwaysFirstArgs = dedupTokens(gccCompilerOptsBaseline);
    QString gccLDFlags = dedupTokens(gccLinkerOptsBaseline + " " + currentProject->extraGccLinkerOptions);

    // Final safety pass for BOTH toolchains: drop from CFLAGS/LDFLAGS
    // anything already present in CCARGS - both appear together on the
    // same command line ("$(CC) $(CCARGS) $(CFLAGS) ..." /
    // "$(CC) $(CCARGS) ... $(LDFLAGS) ..."), and the project's own extra
    // options frequently overlap with the toolchain baseline (the "New
    // Project" dialog pre-fills exactly that baseline as a starting
    // point - see promptCompilerLinkerOptions() - and many users simply
    // accept it as-is).
    vbccCFlags = removeTokensAlsoIn(dedupTokens(vbccCFlags), vbccAlwaysFirstArgs);
    vbccLDFlags = removeTokensAlsoIn(vbccLDFlags, vbccAlwaysFirstArgs);
    QString gccCFlags = removeTokensAlsoIn(dedupTokens(currentProject->extraGccCompilerOptions), gccAlwaysFirstArgs);
    gccLDFlags = removeTokensAlsoIn(gccLDFlags, gccAlwaysFirstArgs);

    // Floating point ("float"/"double") needs an explicit math library on
    // all three toolchains - none of them link one in by default. Added
    // here, after the cleanup pass above, so it's never mistaken for an
    // already-configured option and stripped back out; dedupTokens()
    // still means it's a no-op if the user (or the Prefs baseline) already
    // included it themselves.
    bool usesFloatingPoint = projectUsesFloatingPoint();
    if (usesFloatingPoint)
    {
        gccLDFlags = dedupTokens(gccLDFlags + QStringLiteral(" -lm"));
        vbccLDFlags = dedupTokens(vbccLDFlags + QStringLiteral(" -lmieee"));
    }

    auto writeMakefile = [&](const QString &fileBaseName, const QString &toolchainLabel, const QString &ccPath,
                              const QString &alwaysFirstArgs, const QString &cflags, const QString &ldflags,
                              bool useSeparateAssembler, const QString &asPath)
    {
        QFile file(dir + QDir::separator() + fileBaseName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            writeFailed = true;
            failedFiles << fileBaseName;
            return;
        }

        QTextStream out(&file);
        out.setEncoding(QStringConverter::Latin1);
        out << "# Auto-generated by AmigaED - regenerated automatically whenever a\n";
        out << "# file is added to or removed from the project \"" << currentProject->name << "\".\n";
        out << "# Toolchain: " << toolchainLabel << "\n";
        if (currentProject->templateKind == 2 && toolchainLabel.startsWith("vbcc"))
            out << "# AmigaOS 1.3 project - '+kick13' is fixed and other '+' switches are stripped.\n";
        if (currentProject->templateKind == 2 && toolchainLabel.startsWith("m68k-amigaos-gcc"))
            out << "# AmigaOS 1.3 project - includes Prefs > GCC > \"default OS 1.3 opts\" (-mcrt=nix13).\n";
        out << "# CCARGS includes the toolchain's configured Prefs baseline; CFLAGS/LDFLAGS\n";
        out << "# hold this project's own extra options, with anything already in CCARGS removed.\n";
        if (!asmSources.isEmpty() && useSeparateAssembler)
            out << "# AS is vasm (Prefs > VBCC > \"Path to vasm\") - vc itself doesn't assemble\n"
                   "# hand-written .asm/.s sources, only its own C-generated ones.\n";
        out << "\n";
        out << "CC      = " << ccPath << "\n";
        if (!asmSources.isEmpty() && useSeparateAssembler)
            out << "AS      = " << asPath << "\n";
        out << "TARGET  = " << targetName << "\n";
        out << "SRCS    = " << srcList << (asmSources.isEmpty() ? QString() : (QStringLiteral(" ") + asmSources.join(" "))) << "\n";
        out << "OBJS    = " << objList << "\n";
        if (!alwaysFirstArgs.isEmpty())
            out << "CCARGS  = " << alwaysFirstArgs << "  # must stay the first thing after $(CC) - see comment above\n";
        out << "CFLAGS  = " << cflags << "\n";
        out << "LDFLAGS = " << ldflags << "\n\n";
        out << ".PHONY: all clean\n\n";
        out << "all: $(TARGET)\n\n";
        out << "$(TARGET): $(OBJS)\n";
        out << "\t$(CC) $(CCARGS) $(OBJS) $(LDFLAGS) -o $(TARGET)\n";
        // Icon creation (Prefs > Misc > "create icon") no longer happens
        // here: AmigaED writes its own built-in tool icon directly, in
        // Qt/C++, right after a successful build (single-file compile or
        // project build - see stopCommand()/writeProgramIcon()). Doing it
        // in the app itself instead of via a shell command embedded in
        // this Makefile sidesteps the recurring Windows sh.exe/cmd.exe
        // quoting breakage that approach kept running into (rev.94,
        // rev.98) - at the cost of the icon no longer appearing when
        // "make" is run directly outside AmigaED.
        out << "\n";
        out << "%.o: %.c\n";
        out << "\t$(CC) $(CCARGS) $(CFLAGS) -c $< -o $@\n\n";
        if (!asmSources.isEmpty())
        {
            if (useSeparateAssembler)
            {
                // vasm assembles directly - it knows nothing about CCARGS/
                // CFLAGS (those are gcc/vc-specific compiler options), and
                // "-Fhunk" is the classic AmigaOS executable/object format
                // every m68k Amiga toolchain (vbcc, gcc, SAS/C) expects.
                out << "%.o: %.asm\n";
                out << "\t$(AS) -Fhunk -o $@ $<\n\n";
                out << "%.o: %.s\n";
                out << "\t$(AS) -Fhunk -o $@ $<\n\n";
            }
            else
            {
                // gcc IS the assembler driver here (invokes gas itself) -
                // no separate AS needed. It only recognizes ".s"/".S" as
                // assembler source by extension, not ".asm" - "-x assembler"
                // tells it explicitly what a .asm file actually is.
                out << "%.o: %.asm\n";
                out << "\t$(CC) $(CCARGS) -x assembler -c $< -o $@\n\n";
                out << "%.o: %.s\n";
                out << "\t$(CC) $(CCARGS) -c $< -o $@\n\n";
            }
        }
        out << "clean:\n";
#if defined(Q_OS_WIN)
        // Same sh.exe/cmd.exe handoff reasoning already established for
        // the icon-copy code (see Revisions.md rev.94/98/105): "del" is a
        // cmd.exe built-in, not a standalone executable, so a bare "del"
        // in a Makefile recipe fails when mingw32-make's own sh.exe tries
        // to run it directly - "cmd /c" gives sh an actual program
        // (cmd.exe itself) to invoke.
        //
        // One explicit, quoted, existence-checked deletion PER FILE,
        // rather than a single "del /Q $(OBJS) $(TARGET)" line: confirmed
        // that single-line form could wipe an ENTIRE project directory
        // instead of just the object files and target - $(TARGET) for a
        // typical Amiga executable has no extension (e.g. "ftest2", not
        // "ftest2.exe"), and that appears to be the trigger, but the
        // exact sh.exe/cmd.exe/DEL interaction responsible wasn't pinned
        // down with certainty even after reviewing this generated line by
        // line - reproducing the exact failure without a real Windows
        // machine to test against wasn't possible. AmigaED's own Clean
        // Project button (actionCleanProject()) no longer uses this rule
        // at all for exactly that reason - it deletes each file directly
        // via Qt instead - so this rewritten version is a best-effort
        // second layer for "make clean" run directly from a shell,
        // outside AmigaED, not the primary safeguard.
        for (const QString &o : (objs + asmObjs))
            out << "\t-cmd /c if exist \"" << o << "\" del /Q \"" << o << "\"\n";
        out << "\t-cmd /c if exist \"" << targetName << "\" del /Q \"" << targetName << "\"\n";
#else
        out << "\t-rm -f $(OBJS) $(TARGET)\n";
#endif
        out.flush();

        if (file.error() != QFile::NoError)
        {
            writeFailed = true;
            failedFiles << fileBaseName;
        }
        else
        {
            reloadEditorFromDiskIfOpen(dir + QDir::separator() + fileBaseName);
        }
    };

    writeMakefile("Makefile.gcc", "m68k-amigaos-gcc",
                   p_compiler_gcc.isEmpty() ? QStringLiteral("m68k-amigaos-gcc") : p_compiler_gcc,
                   gccAlwaysFirstArgs, gccCFlags, gccLDFlags,
                   false, QString());
    writeMakefile("Makefile.vbcc", "vbcc (vc)",
                   p_compiler_vc.isEmpty() ? QStringLiteral("vc") : p_compiler_vc,
                   vbccAlwaysFirstArgs, vbccCFlags, vbccLDFlags,
                   true, p_compiler_vasm.isEmpty() ? QStringLiteral("vasmm68k_mot") : p_compiler_vasm);

    // SAS/C (Makefile.sc): only for the project's plain .c files (SAS/C
    // doesn't support C++) - skip entirely if there are none (e.g. a pure
    // C++ project). No CFLAGS/LDFLAGS split and no OS 1.3/3.x distinction
    // is made, since "sc" compiles and links in a single invocation and
    // SAS/C targets are source/binary compatible across OS 1.3 and 3.x
    // for typical use. SAS/C only runs on a real Amiga/emulator, so
    // AmigaED never invokes this Makefile itself - it's purely a hint for
    // whoever builds it by hand later (e.g. via smake).
    if (!scSources.isEmpty())
    {
        // Only add MATH=IEEE if the project doesn't already configure a
        // MATH= mode of its own (SAS/C also supports MATH=68881/MATH=FFP
        // for a real FPU/Fast Floating Point instead) - never override an
        // explicit user choice, only fill in a sensible default when
        // nothing at all was specified and the project needs one.
        QString scOpts = p_compiler_sc_call;
        if (usesFloatingPoint && !scOpts.contains(QStringLiteral("MATH="), Qt::CaseInsensitive))
            scOpts = scOpts.isEmpty() ? QStringLiteral("MATH=IEEE") : (scOpts + QStringLiteral(" MATH=IEEE"));

        QFile scFile(dir + QDir::separator() + "Makefile.sc");
        if (scFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&scFile);
            out.setEncoding(QStringConverter::Latin1);
            out << "# Auto-generated by AmigaED - regenerated automatically whenever a\n";
            out << "# file is added to or removed from the project \"" << currentProject->name << "\".\n";
            out << "# Toolchain: SAS/C (sc)\n";
            out << "#\n";
            out << "# SAS/C only runs on a real Amiga (or emulator) - AmigaED never runs\n";
            out << "# this Makefile itself. Copy the project over and build it by hand\n";
            out << "# (e.g. via smake) once you want to cross-check the build with SAS/C.\n";
            out << "\n";
            out << "SCOPTS = " << scOpts << "\n";
            out << "TARGET = " << targetName << "\n";
            out << "SRCS   = " << scSources.join(" ") << "\n\n";
            out << "all: $(TARGET)\n\n";
            out << "$(TARGET): $(SRCS)\n";
            out << "\tsc $(SCOPTS) $(SRCS) LINK TO $(TARGET)\n\n";
            out << "clean:\n";
            out << "\tdelete $(TARGET)\n";
            out.flush();

            if (scFile.error() != QFile::NoError)
            {
                writeFailed = true;
                failedFiles << "Makefile.sc";
            }
            else
            {
                reloadEditorFromDiskIfOpen(dir + QDir::separator() + "Makefile.sc");
            }
        }
        else
        {
            writeFailed = true;
            failedFiles << "Makefile.sc";
        }
    }

    if (writeFailed)
    {
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                              tr("Could not write the following Makefile(s) in\n%1:\n\n%2")
                                  .arg(dir, failedFiles.join("\n")));
    }
    else
    {
        createStatusBarMessage(tr("Makefiles updated."), 2000);
    }
}

//
// Skeleton content for a new project's main C/C++ file. These are
// deliberately minimal STARTING POINTS (they compile and open the
// relevant libraries where applicable) rather than complete, tested
// ReAction/MUI applications - fill in the marked TODOs.
//
QString MainWindow::mainFileTemplateContent(int templateKind, const QString &baseName) const
{
    QString header =
        "/*\n"
        " * File:\t\t" + baseName + ".c\n"
        " * Description:\tAmigaED project main file\n"
        " * TODO:\t\tFill in author/description as needed\n"
        " */\n\n";

    switch (templateKind)
    {
    case 1: // Shell Project
        return header +
            "#include <exec/types.h>\n"
            "#include <dos/dos.h>\n"
            "#include <proto/exec.h>\n"
            "#include <proto/dos.h>\n\n"
            "#define myDebug TRUE\n\n"
            "int main(int argc, char *argv[])\n"
            "{\n"
            "\tPrintf(\"Hello from an AmigaShell program!\\n\");\n\n"
            "\t/* TODO: Write your code! */\n\n"
            "\treturn 0;\n"
            "}\n";

    case 2: // AmigaOS 1.3 Project
        return header +
            "/* Targets AmigaOS 1.3 (Kickstart/Workbench 1.3, NDK 1.3) - avoid APIs\n"
            " * introduced in later Kickstart/Workbench versions. NDK 1.3 has no\n"
            " * <exec/types.h>, and no dos.library Printf() the way later NDKs\n"
            " * provide it either - use plain ANSI C <stdio.h>/printf() for\n"
            " * console output instead. Add <proto/exec.h>/<proto/dos.h> (and\n"
            " * OpenLibrary()/CloseLibrary() calls) yourself once you actually\n"
            " * need an AmigaOS-specific library - a plain console program like\n"
            " * this one doesn't need them at all. */\n"
            "#include <stdio.h>\n\n"
            "#define myDebug TRUE\n\n"
            "int main(int argc, char *argv[])\n"
            "{\n"
            "\t/* TODO: Write your code! */\n"
            "\tprintf(\"Hello from an AmigaOS 1.3 program!\\n\");\n\n"
            "\treturn 0;\n"
            "}\n";

    case 3: // AmigaOS 3.x Project
        return header +
            "/* Targets AmigaOS 3.x (NDK 3.2R4). Unlike the other templates, this\n"
            " * one can be started BOTH from the Shell and from Workbench (icon\n"
            " * double-click) - see main()/wbmain() below, the standard AmigaOS way\n"
            " * to write a dual-mode program. Config holds whatever Init() sets up\n"
            " * and Main() needs; add your own fields to it as the program grows. */\n"
            "#include <exec/types.h>\n"
            "#include <exec/memory.h>\n"
            "#include <dos/dos.h>\n"
            "#include <workbench/startup.h>\n"
            "#include <proto/exec.h>\n"
            "#include <proto/dos.h>\n\n"
            "#define myDebug TRUE\n\n"
            "#define PROGRAMNAME \"" + baseName + "\"\n"
            "#define VERSIONSTRING \"1.0\"\n"
            "#define WANTED_VERSION 39\n\n"
            "/* SAS/C, vbcc and gcc each expose the build date under a different\n"
            " * name - picked here so the \"$VER:\" string Workbench's Version\n"
            " * command reads always has one, whichever toolchain builds this. */\n"
            "#if defined(__SASC)\n"
            "const UBYTE VersionTag[] = \"$VER: \" PROGRAMNAME \" \" VERSIONSTRING \" \" __AMIGADATE__ \"\\n\";\n"
            "#elif defined(_DCC)\n"
            "const UBYTE VersionTag[] = \"$VER: \" PROGRAMNAME \" \" VERSIONSTRING \" (\" __COMMODORE_DATE__ \")\\n\";\n"
            "#else\n"
            "const UBYTE VersionTag[] = \"$VER: \" PROGRAMNAME \" \" VERSIONSTRING \" (\" __DATE__ \")\\n\";\n"
            "#endif\n\n"
            "struct Config\n"
            "{\n"
            "\t/* TODO: add your own global/config data here */\n"
            "\tint dummy;\n"
            "};\n\n"
            "struct Config *Init(void);\n"
            "int            Main(struct Config *config);\n"
            "void           CleanUp(struct Config *config);\n"
            "int            wbmain(struct WBStartup *wbs);\n\n"
            "/* Shell entry point. Belt-and-suspenders dual-mode dispatch: on most\n"
            " * toolchains' startup code, wbmain() below is called directly when\n"
            " * launched from Workbench and main() isn't involved at all - but not\n"
            " * every crt0 makes that guarantee, so main() also checks argc itself\n"
            " * and hands off to wbmain() if it's 0 (Workbench passes no command\n"
            " * line, only a WBStartup message, arriving here via argv). */\n"
            "int main(int argc, char *argv[])\n"
            "{\n"
            "\tint rc = 20;\n\n"
            "\tif (argc)\n"
            "\t{\n"
            "\t\tstruct Config *config;\n\n"
            "\t\tif ((config = Init()))\n"
            "\t\t{\n"
            "\t\t\trc = Main(config);\n"
            "\t\t\tCleanUp(config);\n"
            "\t\t}\n"
            "\t}\n"
            "\telse\n"
            "\t\trc = wbmain((struct WBStartup *)argv);\n\n"
            "\treturn rc;\n"
            "}\n\n"
            "/* Workbench entry point (icon double-clicked) */\n"
            "int wbmain(struct WBStartup *wbs)\n"
            "{\n"
            "\tstruct Config *config;\n"
            "\tint rc = 20;\n\n"
            "\tif ((config = Init()))\n"
            "\t{\n"
            "\t\trc = Main(config);\n"
            "\t\tCleanUp(config);\n"
            "\t}\n\n"
            "\treturn rc;\n"
            "}\n\n"
            "/* Allocate/initialize the config structure - called from both entry\n"
            " * points above, exactly once per run. */\n"
            "struct Config *Init(void)\n"
            "{\n"
            "\tstruct Config *config = AllocVec(sizeof(struct Config), MEMF_CLEAR);\n\n"
            "\t/* TODO: any one-time setup (OpenLibrary() at WANTED_VERSION etc.)\n"
            "\t * goes here - check the result, and don't forget to release it\n"
            "\t * again in CleanUp() below. */\n\n"
            "\treturn config;\n"
            "}\n\n"
            "int Main(struct Config *config)\n"
            "{\n"
            "\t/* TODO: Write your code! */\n"
            "\tPrintf(\"Hello from an AmigaOS 3.x program!\\n\");\n\n"
            "\treturn 0;\n"
            "}\n\n"
            "/* Free/close whatever Init() allocated/opened - called from both entry\n"
            " * points above, exactly once per run, even if Main() returned early. */\n"
            "void CleanUp(struct Config *config)\n"
            "{\n"
            "\tif (config)\n"
            "\t{\n"
            "\t\t/* TODO: free/close anything Init() allocated/opened */\n\n"
            "\t\tFreeVec(config);\n"
            "\t}\n"
            "}\n";

    case 4: // ReAction Project
        return header +
            "/* ReAction skeleton, matching the library-opening, menu-layout and\n"
            " * window-creation sequence of a confirmed-working ReAction/vbcc\n"
            " * program (window.class + gadtools.library menu + requester.class),\n"
            " * builds a File menu (About/Quit) and a small window, then runs the\n"
            " * standard ReAction event loop. Add further gadgets to the window's\n"
            " * WINDOW_ParentGroup layout as your program grows. */\n"
            "#include <exec/types.h>\n"
            "#include <dos/dos.h>\n"
            "#include <clib/alib_protos.h>\n"
            "#include <proto/exec.h>\n"
            "#include <proto/dos.h>\n"
            "#include <proto/intuition.h>\n"
            "#include <proto/gadtools.h>\n"
            "#include <proto/window.h>\n"
            "#include <proto/layout.h>\n"
            "#include <proto/label.h>\n"
            "#include <proto/requester.h>\n"
            "#include <libraries/gadtools.h>\n"
            "#include <reaction/reaction.h>\n"
            "#include <reaction/reaction_macros.h>\n"
            "#include <intuition/gadgetclass.h>\n"
            "#include <intuition/icclass.h>\n"
            "#include <classes/window.h>\n"
            "#include <classes/requester.h>\n\n"
            "#define myDebug TRUE\n\n"
            "#define MENU_ABOUT 1\n"
            "#define MENU_QUIT  2\n\n"
            "/* IntuitionBase is NOT declared here: <proto/intuition.h> already\n"
            " * declares it itself, as \"struct IntuitionBase *\" (intuition.library's\n"
            " * own extended library-base type) rather than a plain \"struct\n"
            " * Library *\" - declaring it again here with that more generic type\n"
            " * conflicts (confirmed: \"redeclaration of var IntuitionBase with new\n"
            " * type\"). None of the other libraries opened below have such a\n"
            " * library-specific base type, so declaring those ourselves is fine. */\n"
            "struct Library *GadToolsBase;\n"
            "struct Library *WindowBase;\n"
            "struct Library *LayoutBase;\n"
            "struct Library *LabelBase;\n"
            "struct Library *RequesterBase;\n\n"
            "struct Screen *pubScreen;\n"
            "APTR visualInfo;\n\n"
            "int main(int argc, char *argv[])\n"
            "{\n"
            "\tint rc = 0;\n\n"
            "\t/* Version 0L (\"any version will do\") for everything except\n"
            "\t * layout.gadget, which needs at least v47 here - both confirmed\n"
            "\t * against a real, working ReAction/vbcc program. */\n"
            "\tIntuitionBase = (struct IntuitionBase *)OpenLibrary(\"intuition.library\", 0L);\n"
            "\tGadToolsBase  = OpenLibrary(\"gadtools.library\", 0L);\n"
            "\tWindowBase    = OpenLibrary(\"window.class\", 0L);\n"
            "\t/* layout.gadget/label.image (unlike window.class/requester.class)\n"
            "\t * live under LIBS:Gadgets/ and LIBS:Images/ respectively, not\n"
            "\t * directly under LIBS: itself - the \"gadgets/\"/\"images/\" prefixes\n"
            "\t * here are required, not optional. */\n"
            "\tLayoutBase    = OpenLibrary(\"gadgets/layout.gadget\", 47L);\n"
            "\tLabelBase     = OpenLibrary(\"images/label.image\", 0L);\n"
            "\tRequesterBase = OpenLibrary(\"requester.class\", 0L);\n\n"
            "\tif (IntuitionBase && GadToolsBase && WindowBase && LayoutBase && LabelBase && RequesterBase)\n"
            "\t{\n"
            "\t\tpubScreen  = LockPubScreen(NULL);\n"
            "\t\tvisualInfo = pubScreen ? GetVisualInfo(pubScreen, TAG_DONE) : NULL;\n\n"
            "\t\tif (pubScreen && visualInfo)\n"
            "\t\t{\n"
            "\t\t\t/* Classic Intuition NewMenu array - still how ReAction apps build\n"
            "\t\t\t * their menu strip too, via gadtools.library's CreateMenusA()/\n"
            "\t\t\t * LayoutMenus(), attached to the window below via SetMenuStrip()\n"
            "\t\t\t * AFTER opening it (not a WINDOW_MenuStrip tag at creation time). */\n"
            "\t\t\tstruct NewMenu nm[] =\n"
            "\t\t\t{\n"
            "\t\t\t\t{ NM_TITLE, (STRPTR)\"File\",     NULL,        0, 0, NULL },\n"
            "\t\t\t\t{  NM_ITEM, (STRPTR)\"About...\", NULL,        0, 0, (APTR)MENU_ABOUT },\n"
            "\t\t\t\t{  NM_ITEM, (STRPTR)\"Quit\",     (STRPTR)\"Q\", 0, 0, (APTR)MENU_QUIT },\n"
            "\t\t\t\t{ NM_END,   NULL,               NULL,        0, 0, NULL }\n"
            "\t\t\t};\n\n"
            "\t\t\tstruct Menu *menuStrip;\n"
            "\t\t\tObject *textLabel;\n"
            "\t\t\tObject *layoutObj;\n"
            "\t\t\tObject *winObj;\n\n"
            "\t\t\tmenuStrip = CreateMenusA(nm, TAG_END);\n"
            "\t\t\tif (menuStrip)\n"
            "\t\t\t\tLayoutMenus(menuStrip, visualInfo, GTMN_NewLookMenus, TRUE, TAG_DONE);\n\n"
            "\t\t\t/* A window with no content at all (no WINDOW_ParentGroup) may fail\n"
            "\t\t\t * to open, or open with nothing usable in it, without window.class\n"
            "\t\t\t * reporting any error for it anywhere - the same underlying problem\n"
            "\t\t\t * the MUI template's window had before a placeholder Text object\n"
            "\t\t\t * was added there. This placeholder label.image, inside a minimal\n"
            "\t\t\t * single-child layout.gadget, is the ReAction equivalent - replace\n"
            "\t\t\t * both with your actual gadgets as the program grows. label.image\n"
            "\t\t\t * (a plain, non-interactive IMAGE, added via LAYOUT_AddImage, not\n"
            "\t\t\t * LAYOUT_AddChild) is used here rather than a string.gadget forced\n"
            "\t\t\t * read-only: that took two separate rounds to get wrong - GA_ReadOnly\n"
            "\t\t\t * doesn't exist at all (SAS/C: \"undefined identifier\"), and\n"
            "\t\t\t * STRINGA_Editable, despite being genuinely documented elsewhere,\n"
            "\t\t\t * isn't defined in this particular NDK/vbcc header set either\n"
            "\t\t\t * (\"unknown identifier\") - label.image sidesteps the whole question\n"
            "\t\t\t * by using the class actually meant for static text in the first\n"
            "\t\t\t * place, exactly as the reference program's own CHILD_Label entries\n"
            "\t\t\t * do (just attached directly via LAYOUT_AddImage here, rather than\n"
            "\t\t\t * as another gadget's CHILD_Label).\n"
            "\t\t\t * *_GetClass() (from <proto/window.h>/<proto/layout.h>/<proto/\n"
            "\t\t\t * label.h>/<proto/requester.h>) is the confirmed-correct way to get\n"
            "\t\t\t * each class's Class* for NewObject() - passing NULL plus a\n"
            "\t\t\t * \"xxx.class\"/\"xxx.gadget\"/\"xxx.image\" string instead (an earlier\n"
            "\t\t\t * version of this template did) also runs, but *_GetClass() is what\n"
            "\t\t\t * a real, confirmed-working ReAction program actually uses throughout.\n"
            "\t\t\t * All four declarations above are deliberately grouped together,\n"
            "\t\t\t * ahead of any statement in this block (the CreateMenusA() call\n"
            "\t\t\t * right below is an assignment, not a declaration) - unlike C99\n"
            "\t\t\t * (vbcc, gcc), C89 (SAS/C) rejects a declaration once even one\n"
            "\t\t\t * statement has already appeared earlier in the same block\n"
            "\t\t\t * (confirmed: SAS/C errors 218/77/90/... on exactly that). */\n"
            "\t\t\ttextLabel = NewObject(LABEL_GetClass(), NULL,\n"
            "\t\t\t\tLABEL_Text, (ULONG)\"Hello from ReAction!\",\n"
            "\t\t\t\tTAG_DONE);\n\n"
            "\t\t\tlayoutObj = NewObject(LAYOUT_GetClass(), NULL,\n"
            "\t\t\t\tLAYOUT_Orientation, LAYOUT_ORIENT_VERT,\n"
            "\t\t\t\tLAYOUT_AddImage,    (ULONG)textLabel,\n"
            "\t\t\t\tTAG_DONE);\n\n"
            "\t\t\twinObj = NewObject(WINDOW_GetClass(), NULL,\n"
            "\t\t\t\tWA_Title,             (ULONG)\"" + baseName + "\",\n"
            "\t\t\t\tWA_Activate,          TRUE,\n"
            "\t\t\t\tWA_DepthGadget,       TRUE,\n"
            "\t\t\t\tWA_DragBar,           TRUE,\n"
            "\t\t\t\tWA_CloseGadget,       TRUE,\n"
            "\t\t\t\tWA_SizeGadget,        TRUE,\n"
            "\t\t\t\tWA_Width,             320,\n"
            "\t\t\t\tWA_Height,            120,\n"
            "\t\t\t\t/* Without WA_IDCMP explicitly requesting these, the window\n"
            "\t\t\t\t * never actually receives menu-pick or close-gadget events at\n"
            "\t\t\t\t * all - confirmed: an earlier version of this template omitted\n"
            "\t\t\t\t * WA_IDCMP entirely, and neither About nor Quit ever fired from\n"
            "\t\t\t\t * the menu, even with the WMHI_MENUMASK fix below already in\n"
            "\t\t\t\t * place - RA_HandleInput() simply never reported WMHI_MENUPICK/\n"
            "\t\t\t\t * WMHI_CLOSEWINDOW in the first place. Add more flags here\n"
            "\t\t\t\t * (IDCMP_GADGETUP, IDCMP_NEWSIZE, ...) as you add more gadgets. */\n"
            "\t\t\t\tWA_IDCMP,             IDCMP_CLOSEWINDOW | IDCMP_MENUPICK,\n"
            "\t\t\t\tWINDOW_Position,      WPOS_CENTERSCREEN,\n"
            "\t\t\t\tWINDOW_ParentGroup,   (ULONG)layoutObj,\n"
            "\t\t\t\tWINDOW_IconifyGadget, FALSE,\n"
            "\t\t\t\tTAG_DONE);\n\n"
            "\t\t\tif (winObj)\n"
            "\t\t\t{\n"
            "\t\t\t\t/* RA_OpenWindow()/RA_HandleInput()/RA_CloseWindow() (from\n"
            "\t\t\t\t * <reaction/reaction_macros.h>) are the confirmed-correct,\n"
            "\t\t\t\t * idiomatic way to drive a window.class object - thin wrappers\n"
            "\t\t\t\t * around DoMethod(obj, WM_OPEN)/WM_HANDLEINPUT/WM_CLOSE. */\n"
            "\t\t\t\tstruct Window *win = RA_OpenWindow(winObj);\n\n"
            "\t\t\t\tif (win)\n"
            "\t\t\t\t{\n"
            "\t\t\t\t\tBOOL running = TRUE;\n"
            "\t\t\t\t\tULONG winSig = 0;\n\n"
            "\t\t\t\t\tGetAttr(WINDOW_SigMask, winObj, &winSig);\n"
            "\t\t\t\t\tif (menuStrip)\n"
            "\t\t\t\t\t\tSetMenuStrip(win, menuStrip);\n\n"
            "\t\t\t\t\twhile (running)\n"
            "\t\t\t\t\t{\n"
            "\t\t\t\t\t\tULONG sigs = Wait(winSig | SIGBREAKF_CTRL_C);\n"
            "\t\t\t\t\t\tULONG result;\n"
            "\t\t\t\t\t\tUWORD code;\n\n"
            "\t\t\t\t\t\tif (sigs & SIGBREAKF_CTRL_C)\n"
            "\t\t\t\t\t\t\tbreak;\n\n"
            "\t\t\t\t\t\twhile ((result = RA_HandleInput(winObj, &code)) != WMHI_LASTMSG)\n"
            "\t\t\t\t\t\t{\n"
            "\t\t\t\t\t\t\tswitch (result & WMHI_CLASSMASK)\n"
            "\t\t\t\t\t\t\t{\n"
            "\t\t\t\t\t\t\tcase WMHI_CLOSEWINDOW:\n"
            "\t\t\t\t\t\t\t\trunning = FALSE;\n"
            "\t\t\t\t\t\t\t\tbreak;\n\n"
            "\t\t\t\t\t\t\tcase WMHI_MENUPICK:\n"
            "\t\t\t\t\t\t\t{\n"
            "\t\t\t\t\t\t\t\t/* result & WMHI_MENUMASK (NOT a raw (UWORD)result cast, which\n"
            "\t\t\t\t\t\t\t\t * an earlier version of this template used - confirmed wrong:\n"
            "\t\t\t\t\t\t\t\t * neither MENU_ABOUT nor MENU_QUIT ever fired) extracts the\n"
            "\t\t\t\t\t\t\t\t * packed MENUNUM/ITEMNUM back out of result; ItemAddress() +\n"
            "\t\t\t\t\t\t\t\t * GTMENUITEM_USERDATA() then resolve that to the NewMenu entry's\n"
            "\t\t\t\t\t\t\t\t * own nm_UserData (MENU_ABOUT/MENU_QUIT above) - the standard\n"
            "\t\t\t\t\t\t\t\t * gadtools.library way to identify which item was picked. */\n"
            "\t\t\t\t\t\t\t\tstruct MenuItem *item = ItemAddress(menuStrip, result & WMHI_MENUMASK);\n\n"
            "\t\t\t\t\t\t\t\tif (item)\n"
            "\t\t\t\t\t\t\t\t{\n"
            "\t\t\t\t\t\t\t\t\tswitch ((ULONG)GTMENUITEM_USERDATA(item))\n"
            "\t\t\t\t\t\t\t\t\t{\n"
            "\t\t\t\t\t\t\t\t\tcase MENU_ABOUT:\n"
            "\t\t\t\t\t\t\t\t\t{\n"
            "\t\t\t\t\t\t\t\t\t\t/* requester.class - ReAction's own BOOPSI requester,\n"
            "\t\t\t\t\t\t\t\t\t\t * used here instead of the older Intuition EasyRequest()\n"
            "\t\t\t\t\t\t\t\t\t\t * for a look consistent with the rest of the ReAction GUI.\n"
            "\t\t\t\t\t\t\t\t\t\t * RM_OPENREQ opens it modally over 'win' and blocks until\n"
            "\t\t\t\t\t\t\t\t\t\t * the user picks the (single, \"OK\") gadget. */\n"
            "\t\t\t\t\t\t\t\t\t\tObject *reqObj = NewObject(REQUESTER_GetClass(), NULL,\n"
            "\t\t\t\t\t\t\t\t\t\t\tREQ_TitleText,  (ULONG)\"About this Program\",\n"
            "\t\t\t\t\t\t\t\t\t\t\tREQ_BodyText,   (ULONG)\"This Programm was created with the help of AmigaED 4.0\",\n"
            "\t\t\t\t\t\t\t\t\t\t\tREQ_GadgetText, (ULONG)\"OK\",\n"
            "\t\t\t\t\t\t\t\t\t\t\tREQ_Image,      REQIMAGE_INFO,\n"
            "\t\t\t\t\t\t\t\t\t\t\tTAG_DONE);\n\n"
            "\t\t\t\t\t\t\t\t\t\tif (reqObj)\n"
            "\t\t\t\t\t\t\t\t\t\t{\n"
            "\t\t\t\t\t\t\t\t\t\t\tDoMethod(reqObj, RM_OPENREQ, NULL, win, NULL);\n"
            "\t\t\t\t\t\t\t\t\t\t\tDisposeObject(reqObj);\n"
            "\t\t\t\t\t\t\t\t\t\t}\n"
            "\t\t\t\t\t\t\t\t\t\tbreak;\n"
            "\t\t\t\t\t\t\t\t\t}\n\n"
            "\t\t\t\t\t\t\t\t\tcase MENU_QUIT:\n"
            "\t\t\t\t\t\t\t\t\t\trunning = FALSE;\n"
            "\t\t\t\t\t\t\t\t\t\tbreak;\n"
            "\t\t\t\t\t\t\t\t\t}\n"
            "\t\t\t\t\t\t\t\t}\n"
            "\t\t\t\t\t\t\t\tbreak;\n"
            "\t\t\t\t\t\t\t}\n"
            "\t\t\t\t\t\t\t}\n"
            "\t\t\t\t\t\t}\n"
            "\t\t\t\t\t}\n\n"
            "\t\t\t\t\tRA_CloseWindow(winObj);\n"
            "\t\t\t\t}\n"
            "\t\t\t\telse\n"
            "\t\t\t\t{\n"
            "\t\t\t\t\tPrintf(\"Failed to open the window (RA_OpenWindow returned NULL).\\n\");\n"
            "\t\t\t\t}\n\n"
            "\t\t\t\tDisposeObject(winObj);\n"
            "\t\t\t}\n"
            "\t\t\telse\n"
            "\t\t\t{\n"
            "\t\t\t\tPrintf(\"Failed to create the window object (NewObject returned NULL).\\n\");\n"
            "\t\t\t}\n\n"
            "\t\t\tif (menuStrip) FreeMenus(menuStrip);\n"
            "\t\t}\n"
            "\t\telse\n"
            "\t\t{\n"
            "\t\t\tPrintf(\"Failed to lock the screen / get VisualInfo.\\n\");\n"
            "\t\t}\n\n"
            "\t\tif (visualInfo) FreeVisualInfo(visualInfo);\n"
            "\t\tif (pubScreen)  UnlockPubScreen(NULL, pubScreen);\n"
            "\t}\n"
            "\telse\n"
            "\t{\n"
            "\t\tPrintf(\"Failed to open one or more required libraries:\\n\");\n"
            "\t\tif (!IntuitionBase) Printf(\" - intuition.library\\n\");\n"
            "\t\tif (!GadToolsBase)  Printf(\" - gadtools.library\\n\");\n"
            "\t\tif (!WindowBase)    Printf(\" - window.class\\n\");\n"
            "\t\tif (!LayoutBase)    Printf(\" - gadgets/layout.gadget\\n\");\n"
            "\t\tif (!LabelBase)     Printf(\" - images/label.image\\n\");\n"
            "\t\tif (!RequesterBase) Printf(\" - requester.class\\n\");\n"
            "\t\trc = 20;\n"
            "\t}\n\n"
            "\tif (RequesterBase) CloseLibrary(RequesterBase);\n"
            "\tif (LabelBase)     CloseLibrary(LabelBase);\n"
            "\tif (LayoutBase)    CloseLibrary(LayoutBase);\n"
            "\tif (WindowBase)    CloseLibrary(WindowBase);\n"
            "\tif (GadToolsBase)  CloseLibrary(GadToolsBase);\n"
            "\tif (IntuitionBase) CloseLibrary((struct Library *)IntuitionBase);\n\n"
            "\treturn rc;\n"
            "}\n";

    case 5: // MUI Project
        return header +
            "/* MUI 5 (Stephan Stuntz) skeleton: opens muimaster.library, builds an\n"
            " * Application object with a File menu (About/Quit) and a small window,\n"
            " * and runs the standard MUI event loop. Add further gadgets to the\n"
            " * window's MUIA_Window_RootObject as your program grows.\n"
            " * <libraries/gadtools.h> is needed here purely for struct NewMenu and\n"
            " * the NM_TITLE/NM_ITEM/NM_END constants below - gadtools.library\n"
            " * itself is never opened/called, MUI_MakeObject(MUIO_MenustripNM, ...)\n"
            " * converts that classic NewMenu array into a MUI menustrip object. */\n"
            "#include <exec/types.h>\n"
            "#include <dos/dos.h>\n"
            "#include <libraries/mui.h>\n"
            "#include <libraries/gadtools.h>\n"
            "#include <proto/exec.h>\n"
            "#include <proto/dos.h>\n"
            "#include <proto/muimaster.h>\n\n"
            "#define myDebug TRUE\n\n"
            "#define MENU_ABOUT 1\n"
            "#define MENU_QUIT  2\n\n"
            "struct Library *MUIMasterBase;\n\n"
            "int main(int argc, char *argv[])\n"
            "{\n"
            "\tAPTR app, win, menustrip;\n"
            "\tint rc = 0;\n\n"
            "\t/* Classic Intuition NewMenu array - MUI_MakeObject(MUIO_MenustripNM, ...)\n"
            "\t * below turns this into a proper MUI menustrip object. */\n"
            "\tstruct NewMenu nm[] =\n"
            "\t{\n"
            "\t\t{ NM_TITLE, (STRPTR)\"File\",     NULL,         0, 0, NULL },\n"
            "\t\t{  NM_ITEM, (STRPTR)\"About...\", NULL,         0, 0, (APTR)MENU_ABOUT },\n"
            "\t\t{  NM_ITEM, (STRPTR)\"Quit\",     (STRPTR)\"Q\", 0, 0, (APTR)MENU_QUIT },\n"
            "\t\t{ NM_END,   NULL,               NULL,         0, 0, NULL }\n"
            "\t};\n\n"
            "\tMUIMasterBase = OpenLibrary(MUIMASTER_NAME, MUIMASTER_VMIN);\n"
            "\tif (!MUIMasterBase)\n"
            "\t\treturn 20;\n\n"
            "\tmenustrip = MUI_MakeObject(MUIO_MenustripNM, nm, 0);\n\n"
            "\tapp = MUI_NewObject(MUIC_Application,\n"
            "\t\tMUIA_Application_Title,       (ULONG)\"" + baseName + "\",\n"
            "\t\tMUIA_Application_Version,     (ULONG)\"$VER: " + baseName + " 1.0\",\n"
            "\t\tMUIA_Application_Copyright,   (ULONG)\"(C) You\",\n"
            "\t\tMUIA_Application_Author,      (ULONG)\"You\",\n"
            "\t\tMUIA_Application_Description, (ULONG)\"An MUI application\",\n"
            "\t\tMUIA_Application_Base,        (ULONG)\"" + baseName.toUpper() + "\",\n"
            "\t\tMUIA_Application_Menustrip,   (ULONG)menustrip,\n"
            "\t\tMUIA_Application_Window,      win = MUI_NewObject(MUIC_Window,\n"
            "\t\t\tMUIA_Window_Title,      (ULONG)\"" + baseName + "\",\n"
            "\t\t\tMUIA_Window_Width,      320,\n"
            "\t\t\tMUIA_Window_Height,     120,\n"
            "\t\t\t/* A window with no content (no RootObject at all) is not a\n"
            "\t\t\t * valid, functioning MUI window - it may fail to open, or open\n"
            "\t\t\t * with nothing visible, without MUI reporting any error for it\n"
            "\t\t\t * anywhere. Replace this placeholder Text object with your\n"
            "\t\t\t * actual gadgets/groups as the program grows. */\n"
            "\t\t\tMUIA_Window_RootObject, MUI_NewObject(MUIC_Text,\n"
            "\t\t\t\tMUIA_Text_Contents, (ULONG)\"Hello from MUI!\",\n"
            "\t\t\t\tMUIA_Frame,         MUIV_Frame_Text,\n"
            "\t\t\t\tMUIA_Background,    MUII_TextBack,\n"
            "\t\t\t\tTAG_DONE),\n"
            "\t\t\tTAG_DONE),\n"
            "\t\tTAG_DONE);\n\n"
            "\tif (app && win)\n"
            "\t{\n"
            "\t\tBOOL running = TRUE;\n"
            "\t\tULONG sigs = 0;\n\n"
            "\t\t/* Both the window's close gadget and the Quit menu item feed into\n"
            "\t\t * the same Application_ReturnID mechanism, so the event loop below\n"
            "\t\t * only has to check for MUIV_Application_ReturnID_Quit once. */\n"
            "\t\tDoMethod(win, MUIM_Notify, MUIA_Window_CloseRequest, TRUE,\n"
            "\t\t\t(ULONG)app, 2, MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);\n\n"
            "\t\tset(win, MUIA_Window_Open, TRUE);\n\n"
            "\t\twhile (running)\n"
            "\t\t{\n"
            "\t\t\tULONG id = DoMethod(app, MUIM_Application_NewInput, &sigs);\n\n"
            "\t\t\tswitch (id)\n"
            "\t\t\t{\n"
            "\t\t\tcase MENU_ABOUT:\n"
            "\t\t\t\t/* MUI_Request() - the standard MUI way to pop up a simple modal\n"
            "\t\t\t\t * info requester (like EasyRequest(), but MUI-native/themeable). */\n"
            "\t\t\t\tMUI_Request(app, win, 0, \"About this Program\", \"OK\",\n"
            "\t\t\t\t\t\"This Programm was created with the help of AmigaED 4.0\");\n"
            "\t\t\t\tbreak;\n\n"
            "\t\t\tcase MENU_QUIT:\n"
            "\t\t\tcase (ULONG)MUIV_Application_ReturnID_Quit:\n"
            "\t\t\t\trunning = FALSE;\n"
            "\t\t\t\tbreak;\n"
            "\t\t\t}\n\n"
            "\t\t\tif (running && sigs)\n"
            "\t\t\t\tsigs = Wait(sigs | SIGBREAKF_CTRL_C);\n\n"
            "\t\t\tif (sigs & SIGBREAKF_CTRL_C)\n"
            "\t\t\t\trunning = FALSE;\n"
            "\t\t}\n\n"
            "\t\tset(win, MUIA_Window_Open, FALSE);\n"
            "\t}\n"
            "\telse\n"
            "\t{\n"
            "\t\tPrintf(\"Failed to create the Application/Window objects.\\n\");\n"
            "\t\trc = 20;\n"
            "\t}\n\n"
            "\tif (app)\n"
            "\t\tMUI_DisposeObject(app);\n"
            "\tCloseLibrary(MUIMasterBase);\n"
            "\treturn rc;\n"
            "}\n";

    case 0: // Empty Amiga C Project
    default:
        return header +
            "#include <stdio.h>\n"
            "#include <dos/dos.h>\n"
            "#include <proto/dos.h>\n\n"
            "#define myDebug TRUE\n\n"
            "int main(int argc, char *argv[])\n"
            "{\n"
            "\t/* TODO: Write your code! */\n"
            "\tprintf(\"Hello, Amiga!\\n\");\n\n"
            "\treturn 0;\n"
            "}\n";
    }
}

//
// Writes AmigaED's own built-in Amiga tool icon (a small, multi-coloured
// classic Workbench icon - see resources/amigaed_tool.info, and
// iconbuild/make_icon.py in the AmigaED source tree for exactly how it was
// built byte-for-byte) to "<executablePath>.info", with its do_StackSize
// field patched to the given value.
//
// Replaces the old approach (a user-selected "default icon" file, copied
// into place by a shell command embedded in the generated Makefile) - that
// copy command was a recurring source of Windows-specific breakage (sh.exe/
// cmd.exe quoting/escaping quirks - see rev.94/rev.98's fixes for the same
// underlying issue). Writing the icon directly from within AmigaED itself,
// in Qt/C++, sidesteps the whole shell-quoting problem entirely, and means
// there's no per-user "default icon" file to configure or lose track of.
//
// do_StackSize sits at a fixed byte offset (74) within the classic
// DiskObject-format .info file - see make_icon.py's own on-disk layout
// comments for the full structure this offset comes from.
//
bool MainWindow::writeProgramIcon(const QString &executablePath, long stackSize) const
{
    QFile templateFile(QStringLiteral(":/icons/amigaed_tool.info"));
    if (!templateFile.open(QIODevice::ReadOnly))
        return false;

    QByteArray bytes = templateFile.readAll();
    templateFile.close();

    const int stackSizeOffset = 74;
    if (bytes.size() < stackSizeOffset + 4)
        return false;   // the embedded template is corrupt/truncated - shouldn't happen

    bytes[stackSizeOffset + 0] = static_cast<char>((stackSize >> 24) & 0xFF);
    bytes[stackSizeOffset + 1] = static_cast<char>((stackSize >> 16) & 0xFF);
    bytes[stackSizeOffset + 2] = static_cast<char>((stackSize >> 8) & 0xFF);
    bytes[stackSizeOffset + 3] = static_cast<char>(stackSize & 0xFF);

    QFile out(executablePath + QStringLiteral(".info"));
    if (!out.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    bool ok = (out.write(bytes) == bytes.size());
    out.close();
    return ok;
}

//
// Shared implementation behind all six "New Project" menu entries:
// choose a directory + name, ask for extra compiler/linker options, write
// the main file's skeleton, save the .aep, and generate its Makefiles.
//
// Close every open tab that belongs to the currently loaded project - either
// tracked in currentProject->files, OR simply sitting in the project's own
// directory (this also catches its auto-generated Makefiles, which are
// deliberately NOT tracked in currentProject->files - see
// actionRemoveFileFromProject() - but are still very much part of the
// project if a tab happens to have one open) - or is still an untitled,
// never-saved buffer ("untitled.c"), with the usual per-tab save
// confirmation (see maybeSave()). Tabs open on unrelated, already-saved
// files that are NOT part of the project are left untouched, so unrelated
// work in progress never gets closed just because a different project is
// being opened. Returns false (and leaves everything as-is) if the user
// cancelled a save prompt partway through.
//
bool MainWindow::closeProjectTabs()
{
    if (!tabWidget)
        return true;

    // Iterate backwards: removing tab i never shifts the index of any tab
    // before it, so this is safe to do in a single pass.
    for (int i = tabWidget->count() - 1; i >= 0; --i)
    {
        QsciScintilla *editor = qobject_cast<QsciScintilla *>(tabWidget->widget(i));
        if (!editor)
            continue;

        QString path = editor->property("amigaed_filePath").toString();
        bool isUntitled = path.isEmpty();
        bool belongsToOldProject = false;
        if (currentProject && !isUntitled)
        {
            belongsToOldProject = currentProject->contains(path) ||
                    QFileInfo(path).absolutePath() == currentProject->projectDir();
        }

        if (!isUntitled && !belongsToOldProject)
            continue;   // an unrelated, already-saved file - leave it open

        if (!maybeSave(editor))
            return false;   // user cancelled - abort, leave remaining tabs as they are

        tabWidget->removeTab(i);
        editor->deleteLater();
    }

    // NOTE: deliberately NOT recreating a blank "untitled.c" tab here even
    // if this leaves zero tabs open - both callers (createNewProject(),
    // loadProjectFile()) immediately open at least one real file right
    // after calling this, and doing it here too would leave a redundant
    // extra "untitled.c" tab behind alongside the project's real file(s).
    // They each carry their own zero-tabs safety net for the (unlikely)
    // case that they end up not opening anything after all.

    return true;
}

//
// A project built for a specific OS needs the matching target OS active -
// not whatever the user's global default happens to be set to - so switch
// the status bar's target-OS gadget (and the target itself) to match.
// Applies regardless of which compiler is currently selected (VBCC, GCC
// or G++ all now honour this gadget). The "AmigaOS 1.3 Project" template
// needs "OS 1.3"; every other template - including an unknown one
// (templateKind < 0: a hand-assembled .aep, or one saved before this
// field existed) - defaults to "OS 3.x", since that's the more common/
// modern target and leaving the combo at whatever it happened to show
// before would be actively misleading rather than merely uninformative.
//
// Called both when a project is newly created (createNewProject()) and
// whenever one is loaded (loadProjectFile(), via actionLoadProject()/
// openRecentProject()/"Recent Projects").
//
void MainWindow::applyProjectTargetOSIfNeeded()
{
    if (!currentProject)
        return;

    int target = (currentProject->templateKind == 2) ? 0 : 1;   // 0 = "OS 1.3", 1 = "OS 3.x"
    p_compiler_vc_default_target = target;
    osCombo->setCurrentIndex(target);   // reflect it in the status bar; also triggers setDefaultTargetOS()
}

void MainWindow::createNewProject(int templateKind)
{
    QString startDir = p_projectsRootDir.isEmpty() ? QDir::currentPath() : p_projectsRootDir;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a directory for the new project"), startDir);
    if (dir.isEmpty())
        return;

    bool ok = false;
    QString name = QInputDialog::getText(this, tr("New Project"), tr("Project name:"), QLineEdit::Normal, QString(), &ok);
    if (!ok || name.trimmed().isEmpty())
        return;
    name = name.trimmed();

    QString compilerOpts, linkerOpts;
    if (!promptCompilerLinkerOptions(compilerOpts, linkerOpts, templateKind))
        return;   // user cancelled

    QString mainFileName = name;
    mainFileName.replace(QRegularExpression("[^A-Za-z0-9_\\-]"), "_");
    QString mainFilePath = dir + QDir::separator() + mainFileName + ".c";

    QFile file(mainFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                              tr("Could not create the project's main file:\n%1").arg(mainFilePath));
        return;
    }
    {
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Latin1);
        out << mainFileTemplateContent(templateKind, mainFileName);
    }
    file.close();

    Project *project = new Project();
    project->name = name;
    // Makefile.gcc always invokes gcc itself, never g++ (see
    // regenerateProjectMakefiles()) - so both GCC and G++ map to the
    // same "gcc" toolchain slot here; only VBCC gets its own.
    if (p_defaultCompiler == 0)
    {
        project->extraVbccCompilerOptions = compilerOpts;
        project->extraVbccLinkerOptions = linkerOpts;
    }
    else
    {
        project->extraGccCompilerOptions = compilerOpts;
        project->extraGccLinkerOptions = linkerOpts;
    }
    project->templateKind = templateKind;
    project->addFile(mainFilePath);
    project->mainFile = mainFilePath;

    QString aepPath = dir + QDir::separator() + name + ".aep";
    if (!project->save(aepPath))
    {
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                              tr("Could not save the project file:\n%1").arg(aepPath));
        delete project;
        return;
    }

    // Close the previously loaded project's tabs, plus any still-untitled
    // tabs, first (see closeProjectTabs()) - with the usual save
    // confirmation per modified file. If the user cancels, keep everything
    // as it was; the newly created project files stay on disk and can be
    // loaded later via File > Load Project / Recent Projects.
    if (!closeProjectTabs())
    {
        delete project;
        return;
    }

    delete currentProject;
    currentProject = project;
    p_projectModified = false;   // freshly created and already saved (see above) - nothing unsaved yet
    if (saveProjectAct)
        saveProjectAct->setEnabled(false);
    regenerateProjectMakefiles();   // must run BEFORE refreshProjectTree() - it scans disk for which Makefiles exist
    refreshProjectTree();
    refreshFunctionsList();

    applyProjectTargetOSIfNeeded();

    openFileInTab(mainFilePath);
    if (tabWidget->count() == 0)
        newEditorTab();   // safety net - shouldn't normally trigger, see closeProjectTabs()
    addToRecentProjects(aepPath);

    createStatusBarMessage(tr("Project \"%1\" created.").arg(name), 0);
}

void MainWindow::actionNewProjectEmptyC()    { createNewProject(0); }
void MainWindow::actionNewProjectShell()     { createNewProject(1); }
void MainWindow::actionNewProjectAmigaOS13() { createNewProject(2); }
void MainWindow::actionNewProjectAmigaOS3x() { createNewProject(3); }
void MainWindow::actionNewProjectReAction()  { createNewProject(4); }
void MainWindow::actionNewProjectMUI()       { createNewProject(5); }

//
// True for a file an "Import existing Project..." scan should leave out:
// build artifacts (object files, Amiga linker files) and any executable -
// none of these are human-authored project sources worth tracking, and
// an executable in particular could just as easily be some unrelated
// program that happens to sit in the chosen folder.
//
bool MainWindow::isImportSkippableFile(const QString &path) const
{
    QFileInfo info(path);
    QString suffix = info.suffix().toLower();

    if (suffix == "o" || suffix == "lnk")
        return true;

    if (info.isExecutable())
        return true;

    return false;
}

void MainWindow::actionImportExistingProject() { importExistingProject(); }

//
// "Import existing Project...": lets the user point at a folder holding
// some C/C++ project AmigaED doesn't know yet, sorts its files into a new
// Project (skipping build artifacts/executables - see
// isImportSkippableFile()), has the user pick which file is the main
// compile unit, then saves it all as a regular .aep - from that point on
// an imported project behaves exactly like one created via File > New
// Project.
//
// Only the folder's top-level files are scanned (not subfolders) - same
// as every other AmigaED project, whose files are expected to sit
// directly in the project directory (see ensureFileInProjectDir()).
//
void MainWindow::importExistingProject()
{
    QString startDir = p_projectsRootDir.isEmpty() ? QDir::currentPath() : p_projectsRootDir;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose the existing project's folder"), startDir);
    if (dir.isEmpty())
        return;

    QStringList candidateFiles;
    const QFileInfoList entries = QDir(dir).entryInfoList(QDir::Files, QDir::Name);
    for (const QFileInfo &info : entries)
    {
        if (isImportSkippableFile(info.absoluteFilePath()))
            continue;
        candidateFiles << info.absoluteFilePath();
    }

    if (candidateFiles.isEmpty())
    {
        QMessageBox::information(this, tr(AMIGAED_VERSION_STRING),
                                  tr("No importable files were found in:\n%1").arg(dir));
        return;
    }

    // The user has to pick the project's main file - only files
    // Project::typeForFile() recognizes as C/C++ source are offered,
    // since that's what "main file" means everywhere else in AmigaED
    // (see createNewProject()).
    QStringList mainFileCandidates;
    for (const QString &path : candidateFiles)
        if (Project::typeForFile(path) == ProjectFileType::CSource)
            mainFileCandidates << path;

    if (mainFileCandidates.isEmpty())
    {
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                              tr("No C/C++ source file (.c/.cpp) was found in:\n%1\n\n"
                                 "AmigaED needs one to import the project.").arg(dir));
        return;
    }

    QStringList mainFileNames;
    for (const QString &path : mainFileCandidates)
        mainFileNames << strippedName(path);

    bool ok = false;
    QString chosenName = QInputDialog::getItem(this, tr("Import existing Project"),
                                                 tr("Main file of the project:"),
                                                 mainFileNames, 0, false, &ok);
    if (!ok)
        return;
    QString mainFilePath = mainFileCandidates.at(mainFileNames.indexOf(chosenName));

    bool nameOk = false;
    QString name = QInputDialog::getText(this, tr("Import existing Project"), tr("Project name:"),
                                          QLineEdit::Normal, QFileInfo(dir).fileName(), &nameOk);
    if (!nameOk || name.trimmed().isEmpty())
        return;
    name = name.trimmed();

    Project *project = new Project();
    project->name = name;
    project->templateKind = -1;   // imported project - not built from one of AmigaED's own templates
    for (const QString &path : candidateFiles)
        project->addFile(path);
    project->mainFile = mainFilePath;

    QString aepPath = dir + QDir::separator() + name + ".aep";
    if (QFileInfo::exists(aepPath))
    {
        QMessageBox::StandardButton ret = QMessageBox::question(this, tr(AMIGAED_VERSION_STRING),
            tr("A project file named \"%1\" already exists in that folder.\n\nOverwrite it?").arg(name + ".aep"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (ret != QMessageBox::Yes)
        {
            delete project;
            return;
        }
    }

    if (!project->save(aepPath))
    {
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                              tr("Could not save the project file:\n%1").arg(aepPath));
        delete project;
        return;
    }

    // Close the previously loaded project's tabs, plus any still-untitled
    // tabs, first (see closeProjectTabs()) - with the usual save
    // confirmation per modified file. If the user cancels, keep
    // everything as it was; the freshly written .aep and its files stay
    // on disk and can be imported/loaded again later.
    if (!closeProjectTabs())
    {
        delete project;
        return;
    }

    delete currentProject;
    currentProject = project;
    p_projectModified = false;   // freshly imported and already saved (see above) - nothing unsaved yet
    if (saveProjectAct)
        saveProjectAct->setEnabled(false);
    regenerateProjectMakefiles();   // must run BEFORE refreshProjectTree() - it scans disk for which Makefiles exist
    refreshProjectTree();
    refreshFunctionsList();

    applyProjectTargetOSIfNeeded();

    openFileInTab(mainFilePath);
    if (tabWidget->count() == 0)
        newEditorTab();   // safety net - shouldn't normally trigger, see closeProjectTabs()
    addToRecentProjects(aepPath);

    createStatusBarMessage(tr("Project \"%1\" imported (%2 file(s)).").arg(name).arg(candidateFiles.count()), 0);
}

//
// Load an existing .aep project: replaces 'currentProject' and opens
// every one of its files, each in its own tab (existing tabs are left
// untouched - see openFileInTab()).
//
void MainWindow::actionLoadProject()
{
    QString startDir = p_projectsRootDir.isEmpty() ? QDir::currentPath() : p_projectsRootDir;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Project"), startDir, tr("AmigaED projects (*.aep)"));
    if (fileName.isEmpty())
        return;

    loadProjectFile(fileName);
}

//
// Shared implementation behind actionLoadProject() and openRecentProject():
// closes the previous project's tabs (with save confirmation), loads the
// given .aep, opens all of its files each in its own tab, and records it
// in "Recent Projects".
//
void MainWindow::loadProjectFile(const QString &fileName)
{
    Project *project = new Project();
    if (!project->load(fileName))
    {
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING), tr("Could not load project file:\n%1").arg(fileName));
        delete project;
        return;
    }

    // Close the previously loaded project's tabs first (if any) - with the
    // usual save confirmation per modified file. If the user cancels, keep
    // everything as it was and don't load the new project after all.
    if (!closeProjectTabs())
    {
        delete project;
        return;
    }

    delete currentProject;
    currentProject = project;
    p_projectModified = false;   // just loaded from disk - nothing unsaved yet
    if (saveProjectAct)
        saveProjectAct->setEnabled(false);

    // A loaded project might never have gone through createNewProject()
    // (e.g. a hand-assembled .aep, or one from an older AmigaED version
    // that predates Makefile generation) - make sure it always has
    // up-to-date Makefiles too, not just newly-created projects. This must
    // run BEFORE refreshProjectTree(), which scans disk for which
    // Makefiles currently exist.
    regenerateProjectMakefiles();
    refreshProjectTree();
    refreshFunctionsList();

    applyProjectTargetOSIfNeeded();

    for (const ProjectFile &f : currentProject->files)
        openFileInTab(f.path);
    if (tabWidget->count() == 0)
        newEditorTab();   // safety net for a project with an empty file list - shouldn't normally trigger

    // The loop above leaves whichever file happened to be LAST in the
    // list as the active tab (openFileInTab() always switches to the
    // tab it just opened/found) - not necessarily a useful starting
    // point. Re-activate the main file's own tab (already open at this
    // point, so this is just a switch, not a second load) so a loaded
    // project always comes up focused on its actual entry point.
    if (!currentProject->mainFile.isEmpty())
        openFileInTab(currentProject->mainFile);

    addToRecentProjects(fileName);

    createStatusBarMessage(tr("Project \"%1\" loaded.").arg(currentProject->name), 0);
}

//
// Run the project's Makefile (chosen by the currently selected compiler -
// VBCC uses Makefile.vbcc, GCC/G++ use Makefile.gcc) with the given target
// ("all" or "clean"). Shared by actionBuildProject()/actionCleanProject().
//
//
// Find a usable "make" executable: AmigaED's own p_make preference has no
// Preferences-dialog field yet (it's marked "unused ATM" - see mainwindow.h),
// so it's essentially always empty in practice. Falling back straight to a
// bare "make" then relies on it being on the system PATH, which usually
// isn't the case on a Windows machine that only has an amiga-gcc/vbcc
// install.
//
// amiga-gcc distributions (e.g. bebbo's) reliably bundle their own
// make.exe right next to the compiler; vbcc distributions on Windows
// generally do NOT (users are expected to add a separate GNU Make - see
// e.g. the "vbcc-windows" project, which exists specifically to build one
// for vbcc). Since any GNU-Make-compatible binary works fine regardless
// of which cross-compiler the Makefile itself invokes, always try the
// amiga-gcc directory first, then fall back to the vbcc directory, before
// giving up on a bare "make".
//
QString MainWindow::resolveMakeExecutable() const
{
    if (!p_make.isEmpty() && QFileInfo::exists(p_make))
        return p_make;

    QStringList compilerDirsToTry;
    if (!p_compiler_gcc.isEmpty())
        compilerDirsToTry << QFileInfo(p_compiler_gcc).absolutePath();
    if (!p_compiler_vc.isEmpty())
        compilerDirsToTry << QFileInfo(p_compiler_vc).absolutePath();

    for (const QString &compilerDir : compilerDirsToTry)
    {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        QString candidate = compilerDir + QDir::separator() + "make.exe";
#else
        QString candidate = compilerDir + QDir::separator() + "make";
#endif
        if (QFileInfo::exists(candidate))
            return candidate;
    }

    return QStringLiteral("make");   // last resort: hope it's on PATH
}

void MainWindow::actionBuildProject()
{
    if (!currentProject)
    {
        QMessageBox::information(this, tr(AMIGAED_VERSION_STRING), tr("No project is currently loaded."));
        return;
    }

    // Regenerate before every build, not just when a file is added/
    // removed - confirmed the Makefiles otherwise never picked up a
    // CONTENT change (e.g. floating-point usage newly added to an
    // existing, already-tracked file) until something ELSE happened to
    // also add or remove a file and trigger a regeneration as a side
    // effect. This also means projectUsesFloatingPoint()'s check (and
    // therefore -lm/-lmieee) is now always evaluated against the file's
    // current content on disk at build time, not whatever it was the
    // last time the file list itself changed.
    regenerateProjectMakefiles();

    QString makefileName = (p_defaultCompiler == 0) ? "Makefile.vbcc" : "Makefile.gcc";
    QString makefilePath = currentProject->projectDir() + QDir::separator() + makefileName;

    if (!QFileInfo::exists(makefilePath))
    {
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                              tr("Makefile not found:\n%1\n\nAdd at least one C/C++ file to the project first.").arg(makefilePath));
        return;
    }

    QString makeExe = resolveMakeExecutable();
    if (!QFileInfo::exists(makeExe) && makeExe != QStringLiteral("make"))
    {
        // We resolved to a specific path but it doesn't actually exist -
        // tell the user precisely instead of letting QProcess fail silently.
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                              tr("Could not find a \"make\" executable at:\n%1").arg(makeExe));
        return;
    }
    p_lastMakeExecutable = makeExe;

    QString targetName = currentProject->name;
    targetName.replace(QRegularExpression("[^A-Za-z0-9_\\-]"), "_");
    p_expectedProjectBuildTarget = currentProject->projectDir() + QDir::separator() + targetName;

    cmd->setWorkingDirectory(currentProject->projectDir());
    p_lastRunWasProjectBuild = true;
    p_lastRunCompilerLabel = compilerDisplayLabel(p_defaultCompiler);

    if(!(p_console_on_fail))
        actionShowOutputConsole();

    createStatusBarMessage(tr("%1: Building project \"%2\"...").arg(p_lastRunCompilerLabel, currentProject->name), 0);

    QStringList arguments;
    arguments << "-f" << makefileName << "all";
    runCommand(makeExe, arguments);
}

//
// Deletes each build artifact directly (QFile::remove(), one exact path
// at a time) rather than invoking "make clean" through a shell.
//
// Confirmed (Windows): "-cmd /c del /Q $(OBJS) $(TARGET)" wiped an
// ENTIRE project directory - the .aep, every source file, both other
// Makefiles, everything - instead of just the object file(s) and
// target it was supposed to remove. $(TARGET) for a typical Amiga
// executable has no extension (e.g. "ftest2", not "ftest2.exe"), and
// that appears to be the trigger, but the exact sh.exe/cmd.exe/DEL
// interaction responsible wasn't pinned down with certainty even after
// reviewing the generated Makefile line by line - reproducing the exact
// failure without a real Windows machine to test against wasn't
// possible. Rather than patch that shell command further and hope,
// this sidesteps shell-based deletion for AmigaED's own Clean Project
// entirely: every path deleted below is an exact, individually computed
// filename, never a multi-file shell command line.
//
// "make clean" run directly from a shell, outside AmigaED, still goes
// through the Makefile's own clean: rule - see the Windows-specific
// rewrite of that rule (one explicit, quoted, existence-checked
// deletion per file) in regenerateProjectMakefiles() for the same
// reasoning applied there as a best-effort second layer.
//
void MainWindow::actionCleanProject()
{
    if (!currentProject)
    {
        QMessageBox::information(this, tr(AMIGAED_VERSION_STRING), tr("No project is currently loaded."));
        return;
    }

    QString dir = currentProject->projectDir();

    QString targetName = currentProject->name;
    targetName.replace(QRegularExpression("[^A-Za-z0-9_\\-]"), "_");

    QStringList toDelete;
    for (const ProjectFile &f : currentProject->files)
    {
        if (f.type != ProjectFileType::CSource && f.type != ProjectFileType::Assembly)
            continue;
        toDelete << dir + QDir::separator() + QFileInfo(f.path).completeBaseName() + ".o";
    }
    toDelete << dir + QDir::separator() + targetName;
    toDelete << dir + QDir::separator() + targetName + ".info";   // the icon, if Prefs > Misc > "create icon" wrote one

    if(!(p_console_on_fail))
        actionShowOutputConsole();

    createStatusBarMessage(tr("%1: Cleaning project \"%2\"...").arg(compilerDisplayLabel(p_defaultCompiler), currentProject->name), 0);

    output->clear();
    int removedCount = 0;
    for (const QString &path : toDelete)
    {
        if (!QFileInfo::exists(path))
            continue;

        if (QFile::remove(path))
        {
            output->appendPlainText(tr("Removed: %1").arg(QDir::toNativeSeparators(path)));
            ++removedCount;
        }
        else
        {
            output->appendPlainText(tr("Could not remove: %1").arg(QDir::toNativeSeparators(path)));
        }
    }

    if (removedCount == 0)
        output->appendPlainText(tr("Nothing to clean - no build artifacts found."));

    refreshProjectTree();   // the executable/icon just removed should disappear from the "Executable" category
    createStatusBarMessage(tr("%1: Project \"%2\" cleaned.").arg(compilerDisplayLabel(p_defaultCompiler), currentProject->name), 4000);
}

//
// Persist currentProject to its .aep and clear the "unsaved changes"
// state. The single choke point every project save (auto or via the
// explicit "Save Project" menu entry) goes through, so
// p_projectModified/saveProjectAct's enabled state always stay in sync
// with what's actually on disk.
//
bool MainWindow::saveCurrentProject()
{
    if (!currentProject)
        return false;

    bool ok = currentProject->save(currentProject->projectFilePath);
    if (ok)
    {
        p_projectModified = false;
        if (saveProjectAct)
            saveProjectAct->setEnabled(false);
    }
    return ok;
}

//
// Flag currentProject as having changes not yet written to its .aep.
// Every project-mutating action calls this before its own (usually
// immediate) save - see saveCurrentProject().
//
void MainWindow::markProjectModified()
{
    p_projectModified = true;
    if (saveProjectAct)
        saveProjectAct->setEnabled(true);
}

//
// "Save Project" menu entry (File menu): an explicit, always-available
// counterpart to the auto-save every other project-modifying action
// already performs - useful as a safety net (e.g. after dragging files
// onto the project tree) and to reassure the user there are no unsaved
// changes left. A harmless no-op when nothing has actually changed.
//
void MainWindow::actionSaveProject()
{
    if (!currentProject)
    {
        QMessageBox::information(this, tr(AMIGAED_VERSION_STRING), tr("No project is currently loaded."));
        return;
    }

    if (!p_projectModified)
    {
        createStatusBarMessage(tr("Project \"%1\" is already up to date.").arg(currentProject->name), 2000);
        return;
    }

    if (!saveCurrentProject())
    {
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                              tr("Could not save the project file:\n%1").arg(currentProject->projectFilePath));
        return;
    }

    createStatusBarMessage(tr("Project \"%1\" saved.").arg(currentProject->name), 2000);
}

//
// File/Close Project: closes every tab belonging to the current project
// (prompting to save any with unsaved changes first, via the same
// closeProjectTabs() used when loading a different project - see
// loadProjectFile()) and returns to the "no project loaded" state.
// Cancelling any one of those save prompts aborts the whole thing,
// leaving the project and all its tabs open exactly as they were.
//
void MainWindow::actionCloseProject()
{
    if (!currentProject)
    {
        QMessageBox::information(this, tr(AMIGAED_VERSION_STRING), tr("No project is currently loaded."));
        return;
    }

    const QString projectName = currentProject->name;

    if (!closeProjectTabs())
        return;   // user cancelled a save prompt - leave the project and its tabs open

    delete currentProject;
    currentProject = nullptr;
    p_projectModified = false;
    saveProjectAct->setEnabled(false);

    refreshProjectTree();
    refreshFunctionsList();

    if (tabWidget->count() == 0)
        newEditorTab();   // safety net - always leave at least one tab open

    createStatusBarMessage(tr("Project \"%1\" closed.").arg(projectName), 3000);
}

//
// Copy a file into the project's own directory if it isn't already
// sitting there directly, returning the path to actually track (the copy
// if one was made, otherwise the original path unchanged). Every
// generated Makefile (Makefile.gcc/.vbcc/.sc) lists SRCS as bare
// filenames with no directory component (see regenerateProjectMakefiles())
// and its compile rule looks for them right next to itself - a source
// file living somewhere else entirely wouldn't actually be found by
// "make", so it needs a local copy, not just a reference to wherever it
// originally was.
//
// Returns an empty string if the user declined to overwrite an existing
// same-named file, or if the copy itself failed - the caller should skip
// that file rather than tracking a path that was never actually copied.
//
QString MainWindow::ensureFileInProjectDir(const QString &sourcePath)
{
    if (!currentProject)
        return sourcePath;

    QFileInfo source(sourcePath);
    if (QFileInfo(source.path()) == QFileInfo(currentProject->projectDir()))
        return sourcePath;   // already directly in the project directory - nothing to do

    QString destPath = currentProject->projectDir() + QDir::separator() + source.fileName();

    if (QFileInfo::exists(destPath))
    {
        QMessageBox::StandardButton ret = QMessageBox::question(this, tr(AMIGAED_VERSION_STRING),
            tr("A file named \"%1\" already exists in the project directory.\n\n"
               "Overwrite it with the file you're adding?").arg(source.fileName()),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (ret != QMessageBox::Yes)
            return QString();
        QFile::remove(destPath);
    }

    if (!QFile::copy(sourcePath, destPath))
    {
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                              tr("Could not copy \"%1\" into the project directory.").arg(source.fileName()));
        return QString();
    }

    return destPath;
}

//
// Shared "add these files to the project" logic: used both by the
// "Add..." button (actionAddFileToProject()) and by dropping files onto
// the project tree (see eventFilter()). Each file is first copied into
// the project's own directory if it isn't already there (see
// ensureFileInProjectDir()) - so both paths behave consistently, and the
// generated Makefiles can actually find every source file. Adding a file
// this way that contains C functions makes them show up in the Functions
// Browser too, via the refreshFunctionsList() call at the end - it scans
// every project file from disk, not just whichever one happens to be
// open.
//
void MainWindow::addFilesToProject(const QStringList &filePaths)
{
    if (!currentProject || filePaths.isEmpty())
        return;

    for (const QString &f : filePaths)
    {
        QString pathToAdd = ensureFileInProjectDir(f);
        if (pathToAdd.isEmpty())
            continue;   // user declined an overwrite, or the copy failed - skip this one
        currentProject->addFile(pathToAdd);
    }

    markProjectModified();
    saveCurrentProject();
    regenerateProjectMakefiles();   // must run BEFORE refreshProjectTree() - it scans disk for which Makefiles exist
    refreshProjectTree();
    refreshFunctionsList();
}

//
// True for a file the project tree's drag'n'drop (see eventFilter())
// should offer to add: a C/C++ source or header (matching the same
// extensions Project::typeForFile() already classifies as such), or a
// Makefile - any file named "Makefile" (optionally with an extra
// extension, e.g. a hand-written "Makefile.mine" from elsewhere),
// EXCEPT one of the current project's own auto-generated ones
// (Makefile.gcc/.vbcc/.sc in its own directory) - those are managed
// separately and were never meant to become a regular tracked
// Project::files entry, exactly as with the same exclusion in
// maybeOfferAddToProject().
//
bool MainWindow::isDragDropAcceptableProjectFile(const QString &path) const
{
    QFileInfo info(path);
    QString suffix = info.suffix().toLower();

    if (suffix == "c" || suffix == "cpp" || suffix == "cc" || suffix == "cxx" || suffix == "c++" ||
        suffix == "h" || suffix == "hpp" || suffix == "hxx" || suffix == "h++" ||
        suffix == "guide" || suffix == "txt" || suffix == "readme" ||
        suffix == "asm" || suffix == "s" || suffix == "pas")
        return true;

    if (info.fileName().compare(QStringLiteral("readme"), Qt::CaseInsensitive) == 0)
        return true;   // a bare, extension-less "README"/"readme" - see refreshProjectTree()'s icon logic

    if (info.fileName().startsWith("Makefile", Qt::CaseInsensitive))
    {
        if (currentProject)
        {
            static const QStringList generatedMakefileNames = { "Makefile.gcc", "Makefile.vbcc", "Makefile.sc" };
            if (QFileInfo(info.path()) == QFileInfo(currentProject->projectDir()) &&
                generatedMakefileNames.contains(info.fileName()))
                return false;
        }
        return true;
    }

    return false;
}

//
// Drag'n'drop of external files onto the project tree: accepts .c/.h/.cpp
// (and common variants) or a Makefile (see isDragDropAcceptableProjectFile()),
// and - after confirmation - adds them via the same addFilesToProject()
// path the "Add..." button uses. Installed on projectTree->viewport() (see
// createProjectPanel()), since that's what actually receives drag/drop
// events for a QAbstractItemView-derived widget like QTreeWidget, not the
// tree widget itself.
//
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (projectTree && obj == projectTree->viewport())
    {
        switch (event->type())
        {
        case QEvent::DragEnter:
        case QEvent::DragMove:
        {
            // QDragEnterEvent/QDragMoveEvent both derive from QDropEvent,
            // which is enough here - only mimeData()/acceptProposedAction()
            // are needed, and both are inherited from it.
            auto *dropEvent = static_cast<QDropEvent *>(event);
            if (currentProject && dropEvent->mimeData()->hasUrls())
            {
                for (const QUrl &url : dropEvent->mimeData()->urls())
                {
                    if (url.isLocalFile() && isDragDropAcceptableProjectFile(url.toLocalFile()))
                    {
                        dropEvent->acceptProposedAction();
                        break;
                    }
                }
            }
            return true;   // handled either way - never let the tree's own (unconfigured) internal drag handling react
        }
        case QEvent::Drop:
        {
            auto *dropEvent = static_cast<QDropEvent *>(event);
            if (!currentProject || !dropEvent->mimeData()->hasUrls())
                return true;

            QStringList accepted;
            for (const QUrl &url : dropEvent->mimeData()->urls())
                if (url.isLocalFile() && isDragDropAcceptableProjectFile(url.toLocalFile()))
                    accepted << url.toLocalFile();

            if (accepted.isEmpty())
                return true;

            dropEvent->acceptProposedAction();

            QString question;
            if (accepted.size() == 1)
            {
                question = tr("Add \"%1\" to the project \"%2\"?").arg(strippedName(accepted.first()), currentProject->name);
            }
            else
            {
                QString listText;
                for (const QString &path : accepted)
                    listText += "\n" + strippedName(path);
                question = tr("Add these %1 files to the project \"%2\"?%3").arg(accepted.size()).arg(currentProject->name, listText);
            }

            QMessageBox::StandardButton ret = QMessageBox::question(this, tr(AMIGAED_VERSION_STRING), question,
                                                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if (ret == QMessageBox::Yes)
                addFilesToProject(accepted);

            return true;
        }
        default:
            break;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

//
// Add existing file(s) to the current project (Add... button).
//
void MainWindow::actionAddFileToProject()
{
    if (!currentProject)
    {
        QMessageBox::information(this, tr(AMIGAED_VERSION_STRING), tr("No project is currently loaded. Use File > New Project... or File > Load Project first."));
        return;
    }

    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Add files to project"),
                                                            currentProject->projectDir(),
                                                            tr("All files (*.*)"));
    if (fileNames.isEmpty())
        return;

    addFilesToProject(fileNames);
}

//
// Remove the file(s) currently selected in the project tree (Remove
// button). Only removes project bookkeeping - never deletes the file
// itself from disk.
//
void MainWindow::actionRemoveFileFromProject()
{
    if (!currentProject || !projectTree)
        return;

    QList<QTreeWidgetItem *> selected = projectTree->selectedItems();
    if (selected.isEmpty())
    {
        createStatusBarMessage(tr("Select a file in the project tree to remove first."), 3000);
        return;
    }

    for (QTreeWidgetItem *item : selected)
    {
        if (item->parent() == projectMakefileGroupItem)
            continue;   // auto-generated Makefiles aren't tracked in Project::files - nothing to remove there

        QString path = item->data(0, Qt::UserRole).toString();
        if (!path.isEmpty())
            currentProject->removeFile(path);
    }

    markProjectModified();
    saveCurrentProject();
    regenerateProjectMakefiles();   // must run BEFORE refreshProjectTree() - it scans disk for which Makefiles exist
    refreshProjectTree();
    refreshFunctionsList();
}

//
// Mark the currently selected C/C++ file as the project's main compile
// file (context menu action).
//
void MainWindow::actionSetAsMainFile()
{
    if (!currentProject || !projectTree)
        return;

    QTreeWidgetItem *item = projectTree->currentItem();
    if (!item)
        return;

    QString path = item->data(0, Qt::UserRole).toString();
    if (path.isEmpty() || Project::typeForFile(path) != ProjectFileType::CSource)
        return;

    currentProject->mainFile = path;
    markProjectModified();
    saveCurrentProject();
    regenerateProjectMakefiles();   // must run BEFORE refreshProjectTree() - it scans disk for which Makefiles exist
    refreshProjectTree();
}

//
// Double-clicking a file entry in the project tree opens it - or switches
// to its tab if it's already open (see openFileInTab()). Double-clicking
// a category header (which carries no Qt::UserRole path) does nothing.
//
void MainWindow::onProjectTreeDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if (!item)
        return;

    QString path = item->data(0, Qt::UserRole).toString();
    if (path.isEmpty())
        return;

    openFileInTab(path);
}

//
// Right-click context menu on a project tree file entry: Open / Set as
// main file (C/C++ files only) / Remove from project.
//
void MainWindow::onProjectTreeContextMenu(const QPoint &pos)
{
    if (!currentProject || !projectTree)
        return;

    QTreeWidgetItem *item = projectTree->itemAt(pos);
    if (!item)
        return;

    QString path = item->data(0, Qt::UserRole).toString();
    if (path.isEmpty())
        return;   // a category header - nothing to offer

    bool isMakefile = (item->parent() == projectMakefileGroupItem);
    bool isExecutable = (item->parent() == projectExecutableGroupItem);
    bool isTracked = currentProject->contains(path);   // false for a freshly-built .exe/Makefile the tree only scanned off disk - see refreshProjectTree()

    projectTree->setCurrentItem(item);

    QMenu menu(this);
    QAction *openAction = nullptr;
    QAction *setMainAction = nullptr;
    QAction *renameAction = nullptr;
    QAction *removeAction = nullptr;
    if (isExecutable)
    {
        // A compiled binary - "Open" (into the text editor) and "Set as
        // main file for compilation" simply don't apply to it, unlike
        // every other category.
        renameAction = menu.addAction(tr("Rename..."));
        if (isTracked)
        {
            menu.addSeparator();
            removeAction = menu.addAction(tr("Remove from project"));
        }
    }
    else
    {
        openAction = menu.addAction(tr("Open"));
        if (!isMakefile)
        {
            // Makefiles are auto-generated and not tracked in Project::files
            // (see regenerateProjectMakefiles()/refreshProjectTree()) - "set as
            // main"/"remove from project" simply don't apply to them.
            if (Project::typeForFile(path) == ProjectFileType::CSource)
                setMainAction = menu.addAction(tr("Set as main file for compilation"));
            menu.addSeparator();
            removeAction = menu.addAction(tr("Remove from project"));
        }
    }

    QAction *chosen = menu.exec(projectTree->viewport()->mapToGlobal(pos));
    if (!chosen)
        return;

    if (chosen == openAction)
    {
        openFileInTab(path);
    }
    else if (chosen == setMainAction)
    {
        actionSetAsMainFile();
    }
    else if (chosen == renameAction)
    {
        QFileInfo oldInfo(path);
        bool ok = false;
        QString newName = QInputDialog::getText(this, tr("Rename executable"), tr("New name:"),
                                                  QLineEdit::Normal, oldInfo.fileName(), &ok);
        if (!ok)
            return;
        newName = newName.trimmed();
        if (newName.isEmpty() || newName == oldInfo.fileName())
            return;
        if (newName.contains(QLatin1Char('/')) || newName.contains(QLatin1Char('\\')))
        {
            QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING), tr("The name can't contain a path separator."));
            return;
        }

        QString newPath = oldInfo.absolutePath() + QDir::separator() + newName;
        if (QFileInfo::exists(newPath))
        {
            QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING), tr("A file named \"%1\" already exists.").arg(newName));
            return;
        }

        if (!QFile::rename(path, newPath))
        {
            QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING), tr("Could not rename the file."));
            return;
        }

        if (isTracked)
        {
            currentProject->removeFile(path);
            currentProject->addFile(newPath);
            markProjectModified();
            saveCurrentProject();
        }
        // An untracked, freshly-scanned executable (see refreshProjectTree()'s
        // "Executable" category scan) needed no Project::files update - it
        // was never tracked to begin with. Renamed away from the name the
        // next build expects though, so it simply won't reappear here
        // until the project is rebuilt - the same way it would on a real
        // Amiga if you renamed a binary by hand.
        refreshProjectTree();
    }
    else if (chosen == removeAction)
    {
        currentProject->removeFile(path);
        markProjectModified();
        saveCurrentProject();
        regenerateProjectMakefiles();   // must run BEFORE refreshProjectTree() - it scans disk for which Makefiles exist
        refreshProjectTree();
        refreshFunctionsList();
    }
}

//
// Called after a successful save (see saveFile()): if a project is
// currently loaded and the just-saved file isn't part of it yet, offer to
// add it. Never offers this for one of the project's own auto-generated
// Makefiles (Makefile.gcc/Makefile.vbcc/Makefile.sc) - those are already
// "part of the project" conceptually (shown under their own "Makefiles"
// category in the tree, regenerated automatically whenever the file list
// changes) and were never meant to become a regular tracked
// Project::files entry: doing so would misclassify them (no extension
// pattern fits C/C++/header/installer, so they'd fall into whichever
// category is used as the fallback) for no benefit, since they're
// rewritten from scratch on the next regeneration regardless.
//
void MainWindow::maybeOfferAddToProject(const QString &fileName)
{
    if (!currentProject || fileName.isEmpty())
        return;

    if (currentProject->contains(fileName))
        return;

    QFileInfo target(fileName);
    if (QFileInfo(target.path()) == QFileInfo(currentProject->projectDir()))
    {
        static const QStringList generatedMakefileNames = { "Makefile.gcc", "Makefile.vbcc", "Makefile.sc" };
        if (generatedMakefileNames.contains(target.fileName()))
            return;
    }

    QMessageBox::StandardButton ret = QMessageBox::question(this, tr(AMIGAED_VERSION_STRING),
                                                              tr("\"%1\" is not part of the current project \"%2\".\n\n"
                                                                 "Add it to the project?").arg(strippedName(fileName), currentProject->name),
                                                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (ret != QMessageBox::Yes)
        return;

    currentProject->addFile(fileName);
    markProjectModified();
    saveCurrentProject();
    regenerateProjectMakefiles();   // must run BEFORE refreshProjectTree() - it scans disk for which Makefiles exist
    refreshProjectTree();
    refreshFunctionsList();
}

void MainWindow::initializeGUI()
{
// if on Mac, immitate its GUI behaviour:
#if defined(__APPLE__)
    this->setUnifiedTitleAndToolBarOnMac(true);
    if(p_mydebug)
    {
        qDebug() << "running on some kind of Mac...";
    }
#endif

    // give an icon and a name to the app:
    this->setWindowIcon(QIcon(":/images/amiga_classic.png"));
    this->setWindowTitle(tr(AMIGAED_VERSION_STRING));

    // set a decent start value for app's size
    this->setMinimumSize(600, 450);

    // prepare and initialize statusbar items:
    if(!(p_no_compilerbuttons))    // react on user prefs: show or hide compiler combo and -button
    {
        this->compilerLabel = new QLabel(this);
        this->compilerCombo = new QComboBox(this);
        this->osCombo = new QComboBox(this);
    }

    this->statusLabelX = new QLabel(this);

    if(!(p_no_lcd_statusbar))
    {
        this->statusLCD_X = new QLCDNumber(this);
        this->statusLCD_X->display(0);
    }
    else
    {
        this->statusContainer_X = new QLabel(this);
        statusContainer_X->setFrameShape(QFrame::Panel);
        statusContainer_X->setFrameShadow(QFrame::Sunken);
        statusContainer_X->setMinimumWidth(42);
        statusContainer_X->setAlignment(Qt::AlignRight);
        statusContainer_X->setText("0");
    }

    this->statusLabelY = new QLabel(this);

    if(!(p_no_lcd_statusbar))
    {
        this->statusLCD_Y = new QLCDNumber(this);
        this->statusLCD_Y->display(0);
    }
    else
    {
        this->statusContainer_Y = new QLabel(this);
        statusContainer_Y->setFrameShape(QFrame::Panel);
        statusContainer_Y->setFrameShadow(QFrame::Sunken);
        statusContainer_Y->setMinimumWidth(42);
        statusContainer_Y->setAlignment(Qt::AlignRight);
        statusContainer_Y->setText("0");

    }

    if(!(p_no_compilerbuttons))    // react on user prefs: show or hide compiler combo and -button
    {
        this->compilerButton = new QPushButton(NULL,this);
        compilerButton->setIcon(QIcon(":/images/dice.png"));
        compilerButton->setIconSize(QSize(18,18));
    }

    // permanently add the controls to the status bar
    if(!(p_no_compilerbuttons))    // react on user prefs: show or hide compiler combo and -button
    {
        statusBar()->addPermanentWidget(compilerLabel);
        compilerLabel->setText(tr("Compiler:"));
        statusBar()->addPermanentWidget(compilerCombo);
        compilerCombo->addItems(p_Compilers);
        compilerCombo->setItemIcon(0, QIcon(":/images/c-logo.png"));
        compilerCombo->setItemIcon(1, QIcon(":/images/c-logo.png"));
        compilerCombo->setItemIcon(2, QIcon(":/images/cpp-logo.png"));
        compilerCombo->setCurrentIndex(p_defaultCompiler);
        compilerCombo->setStatusTip(tr("Select compiler to use for this file"));

        osCombo->addItems(p_targetOS);
        osCombo->setItemIcon(0, QIcon(":/images/workbench.png"));
        osCombo->setItemIcon(1, QIcon(":/images/amiga_classic.png"));
        osCombo->setCurrentIndex(p_compiler_vc_default_target);
        osCombo->setMinimumWidth(90);
        osCombo->setDisabled(true);
        osCombo->setStatusTip(tr("Change default target OS"));
        statusBar()->addPermanentWidget(osCombo);
        statusBar()->addPermanentWidget(compilerButton);

        compilerButton->setStatusTip(tr("Compile current file..."));
    }

    statusBar()->addPermanentWidget(statusLabelX);
    statusLabelX->setText(tr("Line:"));
    if(!(p_no_lcd_statusbar))
    {
        statusBar()->addPermanentWidget(statusLCD_X);
        statusLCD_X->display(1);
    }
    else
    {
        statusBar()->addPermanentWidget(statusContainer_X);
        statusContainer_X->setText("1");
    }
    statusBar()->addPermanentWidget(statusLabelY);
    statusLabelY->setText(tr("Column:"));
    if(!(p_no_lcd_statusbar))
    {
        statusBar()->addPermanentWidget(statusLCD_Y);
        statusLCD_Y->display(1);
    }
    else
    {
        statusBar()->addPermanentWidget(statusContainer_Y);
        statusContainer_Y->setText("1");
    }

    // Per-editor-instance setup (context menu, font, margins, caret line,
    // default lexer, EOL/indentation/tab/brace-matching settings, zoom
    // level) has moved to newEditorTab() since AmigaED v3.2 - each open
    // tab gets its own QsciScintilla now, configured identically to what
    // used to be this app's single, permanent textEdit widget. The first
    // tab is created right after this function returns (see constructor).

    // create actions, menues, toolbars and status bar:
    createActions();
    createMenus();
    createToolBars();
    createStatusBarMessage(tr("Ready"), 0);


    if(!(p_no_compilerbuttons))    // react on user prefs: show or hide compiler combo and -button
    {
        connect(compilerCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(SelectCompiler(int)));
        connect(osCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setDefaultTargetOS(int)));
        connect(compilerButton, SIGNAL(clicked(bool)), this, SLOT(actionCompile()));

        if(p_defaultCompiler == 0)
            osCombo->setEnabled(true);
    }

    connect(btnCloseOutput, SIGNAL(clicked(bool)), this, SLOT(actionCloseOutputConsole()));

    QObject::connect(&myProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
    QObject::connect(&myProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(compiler_readyReadStandardOutput()));
    QObject::connect(&myProcess, SIGNAL(started()), this, SLOT(started()));
}

//
// print current file
//
// TODO:    make printing printer prefs-aware!
//          print printing header ("file %f printed on dd.mm.yyyy")
//          ...use QPainter for that?
//
void MainWindow::printFile()
{
    // Check if editor window contains some text...
    int textsize = textEdit->text().size();  // if max > 0 then there must be some text in that editor window!
    if (textsize > 0)                        // more than one character? So there is some text available. Let's check for a printer!
    {
        this->createStatusBarMessage(tr("Calling printer dialog......"), 6000);

        // create a high resolution QScintilla printer instance...
        QsciPrinter printer(QPrinter::HighResolution);
        // ...now we need to know wich printer to use! Call standard system printer dialog:
        QPrintDialog printDialog(&printer, this);

        if (printDialog.exec() == QDialog::Accepted)    // if printer dialog was told to print...
        {
            this->createStatusBarMessage(tr("Printing started!"), 6000);
            // set paper size to DIN A4
            printer.setPageSize(QPageSize(QPageSize::A4));

            // print ...
            // printer.printRange(textEdit, 1, max); REFUSES TO PRINT! Use this instead:
            printer.printRange(textEdit);

            this->createStatusBarMessage(tr("File was send to printer and should be printed soon..."), 6000);
        }
        else    // ...if printer dialog was canceled
        {
            this->createStatusBarMessage(tr("Printing canceled."), 6000);
        }
    }
    // textEdit->text().size() == 0. No text available. We will not print anything!
    else
    {
        this->createStatusBarMessage(tr("Printing canceled tue to wasting!"), 6000);
        (void)QMessageBox::information(this,
                                        "Printing - " AMIGAED_VERSION_STRING, "It seems there is <i><b>no text</b></i> in this editor window!<br> Printing is cancelled due to waste of paper."
                                        "<br>There's allways a unicorn dying if you waste things, ya know?!",
                                        QMessageBox::Ok);
    }
}

//
// inform user about unimplemented feature
//
void MainWindow::popNotImplemented()
{
    (void)QMessageBox::information(this,
                                    "Not implemented - " AMIGAED_VERSION_STRING, "You have requested a feature that is <i><b>not</b></i> fully<br>implemented yet! The requested feature might not work as expected...",
                                    QMessageBox::Ok);

}

//
// Let's talk about mouse press events...
//
//void MainWindow::mousePressEvent(QMouseEvent *event)

//{
//    if (event->button() == Qt::RightButton)
//    {
//        qDebug() << "Right mousebutton pressed!";
//        this->menuWidget()->setContextMenuPolicy(Qt::CustomContextMenu);
//    }
//    if (event->button() == Qt::LeftButton)
//        qDebug() << "Left mousebutton pressed!";
//    if (event->button() == Qt::MiddleButton)
//        qDebug() << "Middle mousebutton pressed!";

//}

//
// Let's implement a custom right-click context menu for QScintilla textEdit
//
void MainWindow::showCustomContextMenue(const QPoint &pos)
{
    // Capture the word under the click (if any) now, while 'pos' still
    // refers to it - actionSearchReplaceFromContext() reads this back
    // once the user actually picks "Search and Replace..." below, which
    // happens later (from inside contextMenu.exec() further down), by
    // which point 'pos' itself no longer means anything (the context
    // menu is what's under the cursor by then).
    p_contextMenuWordAtClick = textEdit ? textEdit->wordAtPoint(pos) : QString();

    // name the context menue
    QMenu contextMenu(tr("Inserts"), this);

    // Comment/Uncomment Block - the very topmost entry, ahead of even
    // Search and Replace below. Deliberately promoted out of the
    // "Comments..." submenu it still sits in on the main Inserts menu:
    // as a quick, frequently-reached-for test/debug tool, one that
    // needed a submenu hover to reach here was a real usability
    // complaint - a plain top-level entry needs just one click.
    contextMenu.addAction(toggleCommentBlockAct);
    contextMenu.addSeparator();

    // Search and Replace - also topmost (just below Comment/Uncomment
    // Block), ahead of (and separated from) the "Inserts" section below.
    // Not part of that mirrored section (see the comment on
    // contextMenu.addAction(&pseudo_action) further down) since
    // searching/replacing isn't a code-insertion template.
    contextMenu.addAction(contextSearchReplaceAct);
    contextMenu.addSeparator();

    // define a pseudo action to show some kind of menue title - disabled,
    // so it reads as a heading rather than a dead, clickable-looking entry
    QAction pseudo_action(tr("What to insert?"), this);
    pseudo_action.setEnabled(false);

    // Mirrors the real "Inserts" menu (menu bar) exactly, by reusing the
    // SAME submenu objects rather than hand-listing a separate, easily
    // outdated copy of their contents here. This is deliberate: those
    // submenus already group everything sensibly (Preprocessor, Libraries,
    // Condition, Loops, Comments) - repeating
    // that grouping by hand here would just be a second place to keep in
    // sync (see the old flat version of this function, which drifted out
    // of sync with the real menu over time - several items were missing,
    // one existed here but nowhere else). A QMenu can be referenced by
    // more than one parent menu's action at once, so this is safe even
    // though those submenus are also permanently attached to insertMenue.
    contextMenu.addAction(&pseudo_action);
    contextMenu.addSeparator();
    contextMenu.addMenu(preprocessorMenue);
    contextMenu.addMenu(libraryMenue);
    contextMenu.addMenu(conditionsMenue);
    contextMenu.addMenu(loopsMenue);
    contextMenu.addSeparator();
    contextMenu.addAction(mainAct);
    contextMenu.addAction(functionAct);
    contextMenu.addAction(enumAct);
    contextMenu.addSeparator();
    contextMenu.addAction(consoleDebugAct);
    contextMenu.addSeparator();
    contextMenu.addMenu(commentsMenue);

    // QCursor::pos() rather than mapToGlobal(pos): 'pos' arrives in the
    // coordinates of whichever editor tab emitted customContextMenuRequested,
    // but this function only has 'this' (MainWindow) to map from - using
    // that mismatched widget placed the menu at the wrong screen position
    // (offset by the toolbar/project panel/tab bar). The cursor's actual
    // screen position is correct regardless of which widget's coordinate
    // space 'pos' came from.
    contextMenu.exec(QCursor::pos());
}

bool MainWindow::fileExists(QString path)
{
    QFileInfo check_file(path);

    // check if exists and if yes: Is it really a file?
    if(check_file.exists() && check_file.isFile())
    {
        if(p_mydebug)
        {
            qDebug() << "compiled file: " << p_compiledFile << " exists.";
        }
        return true;
    }
    else
    {
        if(p_mydebug)
        {
            qDebug() << "compiled file: " << p_compiledFile << " does not exist!";
        }
        return false;
    }
}

/**************************************
 * Stuff for launching a Compiler... **
 **************************************/
int MainWindow::startCompiler()
{
    myProcess.setProcessChannelMode(QProcess::MergedChannels);
    if(p_mydebug)
    {
        qDebug() << "startCompiler() called.";
    }
    //debugVars();

    QString command = p_selected_compiler;
    QStringList arguments;
    // IMPORTANT! 'arguments' must be a QStringList, NOT a QString, else the compiler call will not work!
    // Therefore, we'll have to put each argument separately.
    // Each argument in our string is separated by a whitespace.
    // We use the split() function to isolate them, giving each argument separately to our QStringList.
    arguments << p_selected_compiler_args.split(" ");

    p_lastRunCompilerLabel = compilerDisplayLabel(p_defaultCompiler);
    createStatusBarMessage(tr("%1: Compiler run has been started.").arg(p_lastRunCompilerLabel),0);

    if(p_mydebug)
    {
        qDebug() << "in startCompiler()";

        qDebug() << "p_selected_compiler_args: " << p_selected_compiler_args;
        qDebug() << "\nCommand: " << command;
        qDebug() << "Arguments: " << arguments;

    }

    // Fire up compiler!
    runCommand(command, arguments);

    return 0;
}


void MainWindow::readyReadStandardError()
{
    if(p_mydebug)
        qDebug() << "ReadyError";
}

void MainWindow::readyReadStandardOutput()
{
    if(p_mydebug)
    {
        qDebug() << "readyOut";
    }
    QProcess *myProcess = (QProcess *)sender();
    QByteArray buf = myProcess->readAllStandardOutput();

    QFile data(p_projectsRootDir + QDir::separator() + "compiler_out.txt");

    if(p_mydebug)
    {
        qDebug() << "logfile: " << p_projectsRootDir + QDir::separator() + "compiler_out.txt";
    }
    if (data.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream out(&data);
        out << buf;
    }
}

//
// Generic QProcess::started() handler - currently only wired to the
// compiler's myProcess. Kept separate from emu_started() below: it used
// to be shared with the emulator too, which meant every compiler run
// also showed "UAE successfully started" in the status bar and disabled
// the emulator toolbar button for good (nothing ever re-enabled it,
// since only the emulator's own finished() handler did that) - a
// compile without ever touching the emulator would permanently break
// the "Start emulator" button.
//
void MainWindow::started()
{
    if(p_mydebug)
    {
        qDebug() << "START: Proc Started";
    }
}

//
// Emulator-specific started() handler (myEmulator only - see
// actionEmulator()).
//
void MainWindow::emu_started()
{
    createStatusBarMessage(tr("UAE successfully started."), 0);
    if(p_mydebug)
    {
        qDebug() << "UAE seems to have started!";
    }

    emulatorAct->setDisabled(true);
    emulatorMenue->setDisabled(true);
    killEmulatorAct->setEnabled(true);
}

void MainWindow::emu_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(exitCode == 0)
    {
        if(p_mydebug)
        {
            qDebug() << "Emulator terminated regularily.";
        }
    }
    else
    {
        if(p_mydebug)
        {
            qDebug() << "Emulator NOT terminated regularily.";
        }
    }

    if(p_mydebug)
    {
        qDebug() << "Finished with ExitCode: " << exitCode;
    }

    if (exitStatus==QProcess::CrashExit || exitCode!=0)
    {
        createStatusBarMessage(tr("CrashExit - UAE has a problem!!"), 0);
    }
    else
    {
        createStatusBarMessage(tr("UAE terminated regularly."), 0);
    }
    emulatorMenue->setEnabled(true);
    emulatorAct->setEnabled(true);
    killEmulatorAct->setDisabled(true);
}

//
// Periodic fallback net (see emulatorPollTimer in the constructor):
// if the toolbar/menu still show "an emulator is running" but the
// process actually isn't anymore, resync them - the same reset
// emu_finished() above does, just triggered by polling instead of a
// signal that, for whatever reason, didn't fire this time. A cheap
// no-op the overwhelming majority of the time this runs (every 2s),
// since normally emu_finished() has already done this by the time it
// would fire.
//
void MainWindow::checkEmulatorStillRunning()
{
    if (p_externalEmulatorTracked)
    {
        // A process AmigaED itself didn't start (myEmulator stays
        // NotRunning/null throughout this case) - re-check by name
        // periodically instead, since there's no QProcess::finished()
        // signal to tell us the moment it exits. Handled as an early
        // return of its own: myEmulator being NotRunning/null here too
        // would otherwise also satisfy the plain-old-tracking check
        // below and reset the UI immediately, even while this
        // externally-tracked process is still very much running.
        if (!isEmulatorProcessRunningExternally())
        {
            if(p_mydebug)
                qDebug() << "checkEmulatorStillRunning(): externally-tracked emulator no longer running - resyncing.";

            p_externalEmulatorTracked = false;
            emulatorMenue->setEnabled(true);
            emulatorAct->setEnabled(true);
            killEmulatorAct->setDisabled(true);
            createStatusBarMessage(tr("Emulator is no longer running."), 4000);
        }
        return;
    }

    if ((!myEmulator || myEmulator->state() == QProcess::NotRunning) && killEmulatorAct->isEnabled())
    {
        if(p_mydebug)
            qDebug() << "checkEmulatorStillRunning(): emulator no longer running, but toolbar still showed it as active - resyncing.";

        emulatorMenue->setEnabled(true);
        emulatorAct->setEnabled(true);
        killEmulatorAct->setDisabled(true);
        createStatusBarMessage(tr("Emulator is no longer running."), 4000);
    }
}

void MainWindow::emu_readyReadStandardOutput()
{
    if(p_mydebug)
        qDebug() << "readyOut";

    QProcess *myEmulator = (QProcess *)sender();
    QByteArray buf = myEmulator->readAllStandardOutput();

    if(p_mydebug)
        qDebug() << buf;
}

void MainWindow::compiler_readyReadStandardOutput()
{
    if(p_mydebug)
        qDebug() << "readyReadStandardOut";

    QProcess *myProcess = (QProcess *)sender();
    QByteArray buf = myProcess->readAllStandardOutput();

    if(p_mydebug)
        qDebug() << buf;
}


//
// set GUI according to default prefs
//
void MainWindow::activateGUIdefaultSettings()
{
    // setup Compiler Environment
    SelectCompiler(p_defaultCompiler);
    actionShowIndentationGuides();
}

////////////////////////////
// Debug Helper           //
///////////////////////////
void MainWindow::debugVars()
{
    if(p_mydebug)
    {
        // TAB: Project
        qDebug() << "p_author: " << p_author;
        qDebug() << "p_email: " << p_email;
        qDebug() << "p_website: " << p_website;
        qDebug() << "p_projectsRootDir: " << p_projectsRootDir;
        // TAB: GCC
        qDebug() << "p_compiler_gcc: " << p_compiler_gcc;
        qDebug() << "p_compiler_gpp: " << p_compiler_gpp;
        qDebug() << "p_make: " << p_make;
        qDebug() << "p_strip: " << p_strip;
        qDebug() << "p_compiler_gcc13_call: " << p_compiler_gcc13_call;
        qDebug() << "p_compiler_gcc30_call: " << p_compiler_gcc30_call;
        qDebug() << "p_compiler_gcc13_linker_call: " << p_compiler_gcc13_linker_call;
        qDebug() << "p_compiler_gcc30_linker_call: " << p_compiler_gcc30_linker_call;
        qDebug() << "p_compiler_gpp13_call: " << p_compiler_gpp13_call;
        qDebug() << "p_compiler_gpp30_call: " << p_compiler_gpp30_call;
        qDebug() << "p_compiler_gpp13_linker_call: " << p_compiler_gpp13_linker_call;
        qDebug() << "p_compiler_gpp30_linker_call: " << p_compiler_gpp30_linker_call;
        // TAB: VBCC
        qDebug() << "p_compiler_vc: " << p_compiler_vc;
        qDebug() << "pp_compiler_vasm: " << p_compiler_vasm;
        qDebug() << "p_vbcc_config_dir: " << p_vbcc_config_dir;
        qDebug() << "p_compiler_vc13_call: " << p_compiler_vc13_call;
        qDebug() << "p_compiler_vc30_call: " << p_compiler_vc30_call;
        qDebug() << "p_compiler_vc13_linker_call: " << p_compiler_vc13_linker_call;
        qDebug() << "p_compiler_vc30_linker_call: " << p_compiler_vc30_linker_call;
        // TAB: SAS/C
        qDebug() << "p_compiler_sc_call: " << p_compiler_sc_call;
        // TAB: Emulator
        qDebug() << "p_emulator: " << p_emulator;
        qDebug() << "p_os13_config: " << p_os13_config;
        qDebug() << "p_os30_config: " << p_os30_config;
        qDebug() << "p_defaultEmulator: " << p_defaultEmulator;
        // TAB: Misc
        qDebug() << "p_default_style: " << p_default_style;
        qDebug() << "p_show_indentation: " << p_show_indentation;
        qDebug() << "p_mydebug: " << p_mydebug;
        qDebug() << "p_defaultCompiler: " << p_defaultCompiler;
        qDebug() << "p_no_lcd_statusbar: " << p_no_lcd_statusbar;
        qDebug() << "p_no_compilerbuttons: " << p_no_compilerbuttons;
        qDebug() << "p_simple_statusbar: " << p_simple_statusbar;
        qDebug() << "p_create_icon: " << p_create_icon;
        qDebug() << "p_console_on_fail: " << p_console_on_fail;
        qDebug() << "p_no_warn_requesters: " << p_no_warn_requesters;
    }
}

/////////////////////////////////////////////////////////////////////////
/// new process handling methods for compiler output to QTextBrowser ///
///////////////////////////////////////////////////////////////////////
void MainWindow::runCommand(QString command, QStringList arguments)
{
    if(p_mydebug)
    {
        qDebug() << "in runCommand()";

        qDebug() << "p_selected_compiler_args: " << p_selected_compiler_args;
        qDebug() << "\nCommand: " << command;
        qDebug() << "Arguments: " << arguments;

    }
    cmd->setProcessChannelMode(QProcess::MergedChannels);

    // The compiler executable (m68k-amigaos-gcc.exe etc. on Windows; plain
    // ELF binaries with .so dependencies on Linux; dylibs on macOS) may
    // depend on further shared libraries that live in the same "bin"
    // directory as the compiler itself. QProcess only inherits the
    // environment AmigaED itself was started with, which usually does NOT
    // contain that "bin" directory in PATH. Without it, the compiler's
    // sub-processes (e.g. cc1) can fail with "error while loading shared
    // libraries" / "image not found" depending on OS.
    // Fix: derive the compiler's "bin" directory from 'command' (the full
    // path configured in the Prefs "gcc" tab) and prepend it to PATH for
    // this process only.
    //
    // IMPORTANT: the PATH entry separator differs per OS - ';' on Windows,
    // ':' on Linux/macOS (POSIX). Getting this wrong doesn't just fail to
    // add the compiler's directory - it corrupts every other PATH entry
    // for this process, which can break the whole build. Conditional
    // compilation picks the correct separator for the platform actually
    // being built for.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    const QChar pathListSeparator = QLatin1Char(';');
#elif defined(__APPLE__) || defined(__unix__)
    const QChar pathListSeparator = QLatin1Char(':');
#else
    // Unknown platform - fall back to Qt's own runtime-correct separator
    // (this is what Qt itself uses internally for QDir::current().path()
    // style PATH handling) rather than guessing wrong.
    const QChar pathListSeparator = QDir::listSeparator();
#endif

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString compilerBinDir = QFileInfo(command).absolutePath();
    if(!compilerBinDir.isEmpty())
    {
        // Use OS-native path separators ('\' on Windows, '/' elsewhere) for
        // the directory entry itself, so it matches what other tools in
        // PATH look like on this platform.
        compilerBinDir = QDir::toNativeSeparators(compilerBinDir);

        QString currentPath = env.value("PATH");
        env.insert("PATH", compilerBinDir + pathListSeparator + currentPath);
        if(p_mydebug)
            qDebug() << "runCommand(): prepending to PATH: " << compilerBinDir;
    }
    cmd->setProcessEnvironment(env);

    // check if there's allready a compiled file with that name
    // if it's there - delete it. Just to be sure...
    bool fileExists = QFileInfo::exists(p_compiledFile) && QFileInfo(p_compiledFile).isFile();
    if(fileExists)
    {
        if(p_mydebug)
            qDebug() << "Executable allready exists. Now trying to delete!";
        if(!(QFile::remove(p_compiledFile)))
            // delete file first!
            if(p_mydebug)
                qDebug() << "Icon could not be deleted!";
    }

    // Empty output widget
    output->clear();
    // Give message about started process:
    //output->append("Run process...\nCompiler started:\n");

    // fire up our process:
    cmd->start(command, arguments);
}

void MainWindow::readCommand()
{
    // Show the compiler/linker's raw output as-is - no longer filtered
    // down to "only lines containing the literal word error/warning".
    // That filter was dropping a lot of genuinely relevant output:
    // multi-line GCC diagnostics (the source snippet + "^~~~~" caret line
    // that follows a "warning:"/"error:" line don't contain either word
    // themselves), and linker errors like "undefined reference to `foo'"
    // (which likewise don't literally say "error" - only a possible
    // final "collect2: error: ld returned 1 exit status" summary line
    // would have survived the old filter). Matches stopCommand()'s
    // existing unfiltered appendPlainText() for whatever's left in the
    // buffer at process end, so streamed and final output are now
    // handled consistently.
    output->appendPlainText(cmd->readAll());
    highlightOutputDiagnostics();
}


//
// things to do when execution of startCompiler() has stopped
//
int MainWindow::stopCommand(int exitCode, QProcess::ExitStatus exitStatus)
{
    output->appendPlainText(cmd->readAll());
    highlightOutputDiagnostics();

    if(p_mydebug)
        qDebug() << "In stopCommand()\nexitCode: " << QString::number(exitCode);

    // Project "Build"/"Clean" runs (see actionBuildProject()/
    // actionCleanProject()) share this same 'cmd' process/finished signal
    // with the single-file compile flow below, but their success/failure
    // is much simpler to report - no jump-to-error parsing, no "created an
    // executable" check against p_compiledFile (which wasn't even set for
    // a project build). Handle that case separately and return early.
    if (p_lastRunWasProjectBuild)
    {
        p_lastRunWasProjectBuild = false;
        QString expectedTarget = p_expectedProjectBuildTarget;
        p_expectedProjectBuildTarget.clear();

        if (exitStatus == QProcess::CrashExit || exitCode != 0)
        {
            createStatusBarMessage(tr("%1: Project build failed (exit code %2) - see compiler output.").arg(p_lastRunCompilerLabel).arg(exitCode), 0);
            // Unlike the single-file compile flow below, a project build
            // was started without necessarily showing the console first
            // (see actionBuildProject()/actionCleanProject()) - make sure
            // a failure is actually visible instead of only appearing in
            // the status bar for a moment.
            actionShowOutputConsole();
        }
        else if (!expectedTarget.isEmpty() && !QFileInfo::exists(expectedTarget))
        {
            // "make" exited 0 (this was a "Build", not a "Clean" run - see
            // actionBuildProject()/actionCleanProject() - expectedTarget
            // is only ever set for the former), yet the executable it was
            // supposed to produce doesn't actually exist. Trusting the
            // exit code alone here would be misleading: this can happen
            // e.g. if "make" considers $(TARGET) already up to date
            // against stale/mismatched file timestamps (common after
            // copying a project between machines) and skips the recipe
            // entirely, or if a compiler sub-step failed without make
            // itself propagating a nonzero exit code.
            createStatusBarMessage(tr("%1: \"make\" reported success, but no executable was created (expected: %2) - see compiler output.")
                                        .arg(p_lastRunCompilerLabel, QDir::toNativeSeparators(expectedTarget)), 0);
            actionShowOutputConsole();
        }
        else
        {
            createStatusBarMessage(tr("%1: Project build finished successfully.").arg(p_lastRunCompilerLabel), 0);
            refreshProjectTree();   // pick up the freshly built executable - see the "Executable" category scan there

            if (p_create_icon && currentProject)
            {
                // OS 1.3 needs the least stack of the four; MUI's own
                // overhead needs considerably more than a plain console
                // program or even a ReAction one - see createNewProject()
                // for what each numeric templateKind means.
                long stackSize;
                switch (currentProject->templateKind)
                {
                case 2:  stackSize = 4096;  break;   // AmigaOS 1.3
                case 4:  stackSize = 16000; break;   // ReAction
                case 5:  stackSize = 34000; break;   // MUI
                default: stackSize = 8192;  break;   // Empty C / Shell / AmigaOS 3.x
                }
                writeProgramIcon(expectedTarget, stackSize);
            }
        }

        return exitCode;
    }

    if(QString::number(exitCode) == "0")
    {
        createStatusBarMessage(tr("%1: File compiled.").arg(p_lastRunCompilerLabel), 0);
    }
    else
    {
        createStatusBarMessage(tr("%1: There where errors...").arg(p_lastRunCompilerLabel), 0);
    }

    if (exitStatus==QProcess::CrashExit || exitCode!=0)
    {
        createStatusBarMessage(tr("%1: Compiler error!").arg(p_lastRunCompilerLabel), 0);
        textEdit->setCaretLineVisible(true);
        this->showCaretLineAct->setChecked(true);
        textEdit->setFocus();

        if(p_console_on_fail)
        {
            actionShowOutputConsole();
        }

        if(!(p_no_warn_requesters))
        {
            (void)QMessageBox::critical(this, tr(AMIGAED_VERSION_STRING),
                                         tr("Build error!\n"
                                            "Please check source for errors and recompile."),
                                         QMessageBox::Ok);
        }
    }
    else
    {
        // Let's check if the compiler produced an executable file:
        if(fileExists(p_compiledFile))
        {
            successMessage = "Compiler run finished, took " + QString::number(nMilliseconds) + " mSecs";
            createStatusBarMessage(successMessage, 0);

            if(!(p_no_warn_requesters))
            {
                (void)QMessageBox::information(this, tr("Compilation finished - " AMIGAED_VERSION_STRING),
                                                tr("Successfully compiled.\nCompilation took %1 milliseconds to finish.\n\n"
                                                   "You may now want to test your program in UAE.").arg(nMilliseconds),
                                                QMessageBox::Ok);
            }

            successMessage = "Compiled successfully. Compile time: " + QString::number(nMilliseconds) + " mSecs";
            createStatusBarMessage(successMessage, 0);


            // create icon for this app?
            if(p_create_icon)
            {
                // Single-file (ad-hoc) compiles have no Project/templateKind
                // to go by, so the OS 1.3 vs 3.x stack size split falls
                // back to the toolbar's own target-OS selection (the same
                // one that already picks OS 1.3 vs OS 3.x compiler/linker
                // options for this very compile) - MUI/ReAction sizes only
                // apply to their own "New Project" templates, never here.
                long stackSize = (p_compiler_vc_default_target == 0) ? 4096 : 8192;

                if (!writeProgramIcon(p_compiledFile, stackSize))
                {
                    if(p_mydebug)
                        qDebug() << "Icon could not be written!";

                    (void)QMessageBox::information(this, tr(AMIGAED_VERSION_STRING),
                                                    tr("Sorry - icon file could not be created!\n"),
                                                    QMessageBox::Ok);
                }
                else
                {
                    if(p_mydebug)
                        qDebug() << "Icon was successfully written!";
                }
            }
        }
        else
        {
            (void)QMessageBox::information(this, tr(AMIGAED_VERSION_STRING),
                                            tr("No success in building your executable file!.\n"
                                               "Please check for Errors and recompile."),
                                            QMessageBox::Ok);

            successMessage = "Compiled successfully. Compile time: " + QString::number(nMilliseconds) + " mSecs";
            createStatusBarMessage(successMessage, 0);
        }

    }
    return 0;
}

void MainWindow::error(QProcess::ProcessError error)
{
    qDebug() << "Error" << error;

    QString reason;
    switch(error)
    {
    case QProcess::FailedToStart:
        reason = tr("The emulator could not be started. The most likely "
                    "cause is that the path to the emulator executable "
                    "(UAE path, Prefs > Emulator tab) is wrong, missing, "
                    "or the file is not executable.<br><br>"
                    "Path currently configured:<br><b>%1</b>").arg(p_emulator);
        break;
    case QProcess::Crashed:
        reason = tr("The emulator crashed after starting.");
        break;
    default:
        reason = tr("The emulator could not be started or crashed "
                    "(QProcess error code: %1).").arg(static_cast<int>(error));
    }

    // A FailedToStart error means the process never actually launched,
    // so finished()/emu_finished() - the usual place button state gets
    // reset - never fires for this attempt. Without this, the toolbar
    // would be stuck showing "an emulator is running" (Start disabled,
    // Stop enabled) even though none is. Harmless to also do this for
    // Crashed (emu_finished() will run too in that case and repeat it).
    emulatorAct->setEnabled(true);
    emulatorMenue->setEnabled(true);
    killEmulatorAct->setDisabled(true);

    (void)QMessageBox::critical(this, tr(AMIGAED_VERSION_STRING), reason, QMessageBox::Ok);
}

//
// reports a failed compiler process start - without this, a wrong
// compiler path in Prefs left "Compiler run has been started." in the
// status bar forever, with no further feedback whatsoever
//
void MainWindow::compilerError(QProcess::ProcessError error)
{
    qDebug() << "Compiler error" << error;

    QString reason;
    switch(error)
    {
    case QProcess::FailedToStart:
        if (p_lastRunWasProjectBuild)
        {
            reason = tr("\"make\" could not be started for the project build/clean.<br><br>"
                        "Path tried:<br><b>%1</b><br><br>"
                        "Both the amiga-gcc and vbcc toolchains normally ship their own "
                        "make(.exe) right next to the compiler binary - check that the "
                        "compiler path in Prefs > GCC/VBCC actually points there.").arg(p_lastMakeExecutable);
        }
        else
        {
            reason = tr("The compiler could not be started. The most likely "
                        "cause is that the path to the compiler executable "
                        "(Prefs > GCC or VBCC tab, depending on your selected "
                        "compiler) is wrong, missing, or the file is not "
                        "executable.<br><br>"
                        "Path currently configured:<br><b>%1</b>").arg(p_selected_compiler);
        }
        break;
    case QProcess::Crashed:
        reason = tr("The compiler crashed while running.");
        break;
    default:
        reason = tr("The compiler could not be started or crashed "
                    "(QProcess error code: %1).").arg(static_cast<int>(error));
    }

    // This error interrupts the run entirely - clear the project-build
    // flag now, since stopCommand() (which normally clears it) never gets
    // called when the process fails to even start.
    p_lastRunWasProjectBuild = false;

    (void)QMessageBox::critical(this, tr(AMIGAED_VERSION_STRING), reason, QMessageBox::Ok);
}

void MainWindow::stateChanged(QProcess::ProcessState state)
{
    qDebug() << "Process::stateChanged" << state;
}

//
// prefDialog start helper
//
void MainWindow::startPrefs()
{
    actionPrefsDialog(0);
}

//
// HELPER: disable Emulator menu entries if no configuration was specified
//
void MainWindow::setEmulatorMenu()
{
    if(p_os13_config.isEmpty())
        emulator13Act->setDisabled(true);
    else
        emulator13Act->setEnabled(true);

    if(p_os30_config.isEmpty())
        emulator30Act->setDisabled(true);
    else
        emulator30Act->setEnabled(true);
}

//
// Change the target OS at runtime - reacts to the status bar's shared
// target-OS gadget (osCombo). Applies to whichever compiler is currently
// selected (VBCC, GCC or G++), via the same central lookup used
// everywhere else (see getCompilerAndLinkerOptsForTarget()).
//
void MainWindow::setDefaultTargetOS(int default_os)
{
    p_compiler_vc_default_target = default_os;   // keep the shared target-OS value in sync, regardless of which compiler reacts to it below

    QString compilerOpts, linkerOpts;
    getCompilerAndLinkerOptsForTarget(p_defaultCompiler, default_os, compilerOpts, linkerOpts);
    p_selected_compiler_args = dedupTokens(compilerOpts + " " + linkerOpts);
}

//
// Convenience method - will never really be executed...
//
void MainWindow::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(p_mydebug)
        qDebug() << "We are in MainWindow::finished\n(...this place should have never been reached.)\nThis is the exitCode: " << exitCode;

    if(exitCode == 0)
    {
        if(p_mydebug)
            qDebug() << "Finished: " << exitCode;

    }
    else
    {
        if(p_mydebug)
            qDebug() << "ExitCode " << exitCode;

    }
    if(p_mydebug)
        qDebug() << "Finished: " << exitCode;

    if (exitStatus==QProcess::CrashExit || exitCode!=0)
    {
        createStatusBarMessage(tr("Compiler error!"), 0);
        if(!(p_no_warn_requesters))
        {
            (void)QMessageBox::critical(this, tr(AMIGAED_VERSION_STRING),
                                         tr("Build error!\n"
                                            "Please check source for errors and recompile."),
                                         QMessageBox::Ok);
        }
        //output->moveCursor(QTextCursor::Start);
    }
    else
    {
        // Let's check if the compiler produced an executable file:
        if(fileExists(p_compiledFile))
        {
            successMessage = "Compiled successfully. Compile time: " + QString::number(nMilliseconds) + " mSecs";
            createStatusBarMessage(successMessage, 0);

            (void)QMessageBox::information(this, tr("Compilation finished - " AMIGAED_VERSION_STRING),
                                            tr("Successfully compiled.\nCompilation took %1 milliseconds to finish.\n\n"
                                               "You may now want to test your program in UAE.").arg(nMilliseconds),
                                            QMessageBox::Ok);

            successMessage = "Compiled successfully. Compile time: " + QString::number(nMilliseconds) + " mSecs";
            createStatusBarMessage(successMessage, 0);
        }
        else
        {
            (void)QMessageBox::information(this, tr(AMIGAED_VERSION_STRING),
                                            tr("No success in building your executable file!.\n"
                                               "Please check for Errors and recompile."),
                                            QMessageBox::Ok);

            createStatusBarMessage(tr("Compiler run finished unsuccessfully."), 0);
        }

    }
}

//
// get line of text that was selected and store it in variable text_to_search
// connect to output!
//
void MainWindow::on_output_cursorPositionChanged()
{
    QString text_to_search;                         // keeps the line of text to parse

    QTextCursor txtCursor = output->textCursor();

    // Read the clicked line directly from the document's own block
    // structure - NOT via output->toPlainText().split('\n',
    // Qt::SkipEmptyParts) (the previous approach): SkipEmptyParts
    // removes blank lines from that list, but txtCursor.blockNumber()
    // still counts them, so the two fell out of sync the moment the
    // build output contained so much as one blank line before the
    // clicked one - confirmed to be why clicking a line didn't always
    // jump to the right place (or anywhere at all). findBlockByNumber()
    // reads the exact block the cursor is actually in, with no
    // re-splitting or index bookkeeping to get out of sync.
    QTextBlock block = output->document()->findBlockByNumber(txtCursor.blockNumber());
    if (block.isValid())
        text_to_search = block.text();

    // Now let's do all the work for jumping to error/warning!
    if (!(text_to_search.isEmpty()))
    {
        switch(p_defaultCompiler)
        {
        case 0:
            if(p_mydebug)
                qDebug() << "Now checking for VBCC";

            if (checkVBCC(text_to_search))
                jumpToError(line_nr, 0);
            break;
        case 1:
        case 2:
            if(p_mydebug)
                qDebug() << "Now checking for gcc/g++";

            if (checkGCC(text_to_search))
                jumpToError(line_nr, column_nr - 1);
            break;
        }
    }

    if(p_mydebug)
        qDebug() << "//--- END on_outputCursorPositionChanged() DEBUG --------//";
}

//
// Colours every error/warning line currently in the output pane -
// errors and warnings each get their own colour (bold text + a tinted
// background), chosen to stay readable against both a dark and a light
// Base colour, since that's what actually differs between AmigaED's
// themes (Dark uses a dark Base; every other theme - native styles,
// Workbench 1.3, Workbench 3.1 - uses a plain white one). Reuses
// checkVBCC()/checkGCC() themselves (selected by p_defaultCompiler, the
// same as on_output_cursorPositionChanged()) to classify each line, so
// a line is only ever coloured if it would also actually be clickable -
// the two are deliberately kept driven by the exact same parse.
//
// Called after every batch of new compiler output is appended
// (readCommand()/stopCommand()) - re-scans the whole pane each time
// rather than trying to track incrementally-appended positions, which
// is simpler and, for the size of output a single build/compile
// produces, in practice instant.
//
void MainWindow::highlightOutputDiagnostics()
{
    if (!output)
        return;

    QColor errorBg, errorFg, warningBg, warningFg;
    if (isDarkTheme())
    {
        errorBg   = QColor(0x5A, 0x1A, 0x1A);
        errorFg   = QColor(0xFF, 0x6B, 0x6B);
        warningBg = QColor(0x5A, 0x46, 0x14);
        warningFg = QColor(0xFF, 0xCC, 0x66);
    }
    else
    {
        // Shared by every other theme (native styles, Workbench 1.3,
        // Workbench 3.1) - all of them use a plain white output-pane
        // background, so one light-appropriate scheme covers all three.
        errorBg   = QColor(0xFF, 0xDD, 0xDD);
        errorFg   = QColor(0xAA, 0x00, 0x00);
        warningBg = QColor(0xFF, 0xF3, 0xCD);
        warningFg = QColor(0x99, 0x66, 0x00);
    }

    QTextCharFormat errorFmt;
    errorFmt.setBackground(errorBg);
    errorFmt.setForeground(errorFg);
    errorFmt.setFontWeight(QFont::Bold);

    QTextCharFormat warningFmt;
    warningFmt.setBackground(warningBg);
    warningFmt.setForeground(warningFg);
    warningFmt.setFontWeight(QFont::Bold);

    QTextCharFormat plainFmt;   // clears background/bold on non-matching lines

    QTextCursor cursor(output->document());
    QTextBlock block = output->document()->begin();
    while (block.isValid())
    {
        const QString lineText = block.text();
        bool matched = (p_defaultCompiler == 0) ? checkVBCC(lineText) : checkGCC(lineText);

        cursor.setPosition(block.position());
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

        if (matched && errortype.contains(QStringLiteral("error"), Qt::CaseInsensitive))
            cursor.setCharFormat(errorFmt);
        else if (matched && errortype.contains(QStringLiteral("warning"), Qt::CaseInsensitive))
            cursor.setCharFormat(warningFmt);
        else
            cursor.setCharFormat(plainFmt);

        block = block.next();
    }
}

//
// RegEx parse VBCC output
//
bool MainWindow::checkVBCC(QString str_to_search)
{
    if(str_to_search.isEmpty())
        return false;

    // VBCC message format (both warnings and - what VBCC still calls a
    // "warning" for many syntax errors - fatal problems) looks like:
    //   warning 67 in line 1 of "test.h": type defaults to int
    //   warning 121 in line 6 of "D:\path\to\test.c": return type of main is not int
    // i.e.: (type) (number) in line (line) of "(file)": (message)
    // The filename is simply everything between the quotes, so this
    // handles plain relative names ("test.h") as well as full Windows
    // paths with backslashes ("D:\...\test.c") in one go.
    QRegularExpression rx_vbcc("^(\\w+)\\s+(\\d+)\\s+in line\\s+(\\d+)\\s+of\\s+\"(.+)\":\\s*(.*)$");
    QRegularExpressionMatch match = rx_vbcc.match(str_to_search);

    bool matched = match.hasMatch();

    if(matched)
    {
        errortype     = match.captured(1);          // "warning" or "error"
        error_nr      = match.captured(2).toInt();  // VBCC message number
        line_nr       = match.captured(3).toInt();
        debugfilename = match.captured(4);          // relative name or full Windows path
        column_nr     = 0;                          // VBCC does not report a column in this format
    }
    else
    {
        // Not a VBCC diagnostic line (e.g. plain status/info output, or
        // one of vc's own "Compiling..."/"Assembling..." progress lines)
        // - clear the stale filename so jumpToError() won't reload a
        // previously matched file, or jump to a stale line, by mistake.
        debugfilename.clear();
        if(p_mydebug)
            qDebug() << "checkVBCC(): line did not match VBCC message format - not a warning/error line.";
    }

    if(p_mydebug)
    {
        qDebug() << "|-----------------------------------------------------|";
        qDebug() << "error_nr: " << error_nr;
        qDebug() << "errortype: " << errortype;
        qDebug() << "line_nr: " << line_nr;
        qDebug() << "column_nr: " << column_nr;
        qDebug() << "debugfilename: " << debugfilename;
        qDebug() << "|-----------------------------------------------------|";
    }

    return matched;
} // END checkVBCC()

//
// RegEx parse GCC/G++ output
//
bool MainWindow::checkGCC(QString str_to_search)
{
    if (str_to_search.isEmpty())
        return false;

    // GCC/G++ diagnostic format: "filename:line:column: message" (the
    // vast majority of modern diagnostics), with an older/rarer
    // columnless "filename:line: message" form as a fallback. Matched
    // directly via one anchored regex per form, capturing exactly the
    // filename/line/column - NOT by collecting every digit sequence
    // found anywhere in the line and hoping there happen to be exactly
    // two (the previous approach), which broke the moment the path or
    // message contained any other number - e.g. a project folder
    // literally named "gcctest3", or a message like "expected 3
    // arguments, have 4".
    // (\w+(?:\s+\w+)?) instead of a plain (\w+): GCC's most serious
    // diagnostic level is literally two words, "fatal error:" - a bare
    // \w+ can't match that at all (there's a space before "error", not
    // the colon the rest of the pattern expects next), so those lines
    // fell through as unrecognized entirely. Still matches plain single-
    // word "error"/"warning"/"note" exactly as before.
    // [A-Za-z]+, not \w+: \w+ also matches pure digits, which produced a
    // confirmed false-positive match on GCC's own "In file included
    // from foo.c:12:0:" lines (no message after the *first* colon-less
    // form's pattern point, but "0" alone still satisfied a bare \w+ as
    // if it were a diagnostic keyword) - silently making that line
    // clickable (though never coloured, since "0" contains neither
    // "error" nor "warning"). A real diagnostic keyword is always
    // letters only, so this excludes that case structurally rather than
    // trying to special-case "In file included from" by name.
    static const QRegularExpression rx_gcc_with_col("^(.+?):(\\d+):(\\d+):\\s*([A-Za-z]+(?:\\s+[A-Za-z]+)?):\\s*(.*)$");
    static const QRegularExpression rx_gcc_no_col("^(.+?):(\\d+):\\s*([A-Za-z]+(?:\\s+[A-Za-z]+)?):\\s*(.*)$");

    QRegularExpressionMatch match = rx_gcc_with_col.match(str_to_search);
    bool matched = match.hasMatch();

    if (matched)
    {
        debugfilename = match.captured(1);
        line_nr       = match.captured(2).toInt();
        column_nr     = match.captured(3).toInt();
        errortype     = match.captured(4);          // "error"/"warning"/"note"/...
    }
    else
    {
        match = rx_gcc_no_col.match(str_to_search);
        matched = match.hasMatch();
        if (matched)
        {
            debugfilename = match.captured(1);
            line_nr       = match.captured(2).toInt();
            column_nr     = 0;                       // this form doesn't report a column
            errortype     = match.captured(3);
        }
    }

    if (matched)
    {
        // Belt-and-suspenders on top of the [A-Za-z]+ fix above: only
        // GCC's own actual diagnostic keywords count as a real match -
        // guards against any other coincidental "word before a colon"
        // text structurally matching the pattern without actually being
        // a compiler diagnostic.
        static const QRegularExpression rx_known_type("^(error|warning|note|fatal\\s+error)$",
                                                        QRegularExpression::CaseInsensitiveOption);
        if (!rx_known_type.match(errortype).hasMatch())
            matched = false;
    }

    if (!matched)
    {
        // Not a "file:line[:col]: message" diagnostic line (e.g. a
        // source snippet or "^~~~~" caret line that follows one, plain
        // status output, or a linker line with no filename:line prefix
        // at all, like "undefined reference to `foo'") - clear the
        // stale filename so jumpToError() won't reload a previously
        // matched file, or jump to a stale line, by mistake.
        debugfilename.clear();
        if (p_mydebug)
            qDebug() << "checkGCC(): line did not match GCC diagnostic format - not a warning/error line.";
    }

    if(p_mydebug)
    {
        qDebug() << "|-----------------------------------------------------|";
        qDebug() << "errortype: " << errortype;
        qDebug() << "line_nr: " << line_nr;
        qDebug() << "column_nr: " << column_nr;
        qDebug() << "debugfilename: " << debugfilename;
        qDebug() << "|-----------------------------------------------------|";
    }

    return matched;
}

//
// Resolve a filename as reported by the compiler (e.g. "foo.h" from a
// header included via #include "foo.h", or an NDK header found through
// an -I search path) to an actual, loadable path on disk.
// Returns an absolute path if found, or an empty string if the file
// could not be located anywhere reasonable.
//
QString MainWindow::resolveDebugFilePath(const QString &fileName)
{
    if (fileName.isEmpty())
        return QString();

    QFileInfo fi(fileName);

    // Already an absolute path and it exists -> nothing to resolve.
    if (fi.isAbsolute() && fi.exists())
        return fi.absoluteFilePath();

    // Most local #include "..." headers live next to (or in a
    // subdirectory relative to) the currently edited source file -
    // try that first, since it's the common case for AmigaED projects.
    if (!curFile.isEmpty())
    {
        QString candidate = QFileInfo(curFile).absoluteDir().filePath(fileName);
        if (QFileInfo::exists(candidate))
            return QFileInfo(candidate).absoluteFilePath();
    }

    // Fall back to the application's current working directory - this is
    // also the directory the compiler QProcess runs in (no explicit
    // setWorkingDirectory() is used), so relative paths reported by gcc
    // (e.g. from -I include paths) are often relative to it.
    QString candidate = QDir::current().filePath(fileName);
    if (QFileInfo::exists(candidate))
        return QFileInfo(candidate).absoluteFilePath();

    // Last resort: let Qt resolve it as-is (relative to whatever it
    // considers "current"); if that doesn't exist either, give up.
    if (fi.exists())
        return fi.absoluteFilePath();

    return QString();
}

//
// Jump to line No. X...
// If the compiler message refers to a different file than the one
// currently open (typically an included header), that file is opened in
// its own tab first (or switched to, if it's already open in one) - see
// openFileInTab() - so the cursor ends up at the real error location
// instead of at the (wrong) line/column in the currently active tab.
//
void MainWindow::jumpToError(int error_line, int error_column)
{
    if (!debugfilename.isEmpty())
    {
        QString resolvedPath = resolveDebugFilePath(debugfilename);

        // Note on cross-platform correctness: QFileInfo::operator==() is
        // already platform-aware in Qt itself - it compares case-sensitively
        // on Linux and case-insensitively on Windows/macOS, matching each
        // platform's filesystem semantics. No extra #ifdef needed here.
        if (!resolvedPath.isEmpty() && QFileInfo(resolvedPath) != QFileInfo(curFile))
        {
            // Different file than the one currently active - open it in
            // its own tab (or switch to it, if already open elsewhere).
            // No maybeSave() needed any more: unlike the old single-buffer
            // editor, this can no longer overwrite unsaved work - every
            // other open tab, including the one we're jumping away from,
            // is left completely untouched.
            openFileInTab(resolvedPath);
        }
        else if (resolvedPath.isEmpty() &&
                 QFileInfo(debugfilename).fileName() != QFileInfo(curFile).fileName())
        {
            // Compiler reported a file we could not locate anywhere -
            // tell the user instead of silently jumping in the wrong file.
            createStatusBarMessage(tr("Could not locate file '%1' reported by the compiler.").arg(debugfilename), 0);
            return;
        }
    }

    bool ok = true;
    textEdit->setFocus();

    if (ok)
    {
        // check if text is folded!
        QsciScintilla::FoldStyle state = static_cast<QsciScintilla::FoldStyle>((!textEdit->folding()) * 5);

        // if folded: unfold first!!
        if (state > 0)
        {
            textEdit->foldAll(false);
        }
        textEdit->setCursorPosition(error_line-1, error_column);
    }
}


//
// search and replace:
// Helper slot for compatible call of doSearchAndReplace()
// from lineEdit_find's returnPressed() - Enter in the Find field
// behaves the same as clicking "next".
//
void MainWindow::call_do_search_and_replace()
{
    doSearchAndReplace(SearchReplaceAction::FindNext);
}

//
// search & replace: shared implementation behind the four buttons (and
// Enter in the Find field, via call_do_search_and_replace() above).
//
// Rewritten from scratch (rev.95) - the previous version took an
// action_str parameter but never actually branched on it (no switch/if
// anywhere), so every button did the exact same thing: a broken "find,
// then try to mark all occurrences" that mixed flat QString character
// offsets (docText.indexOf()) with Scintilla's own line/index and byte-
// position addressing - two different coordinate systems that don't
// reliably agree once a document contains anything beyond plain ASCII.
// lineEdit_replace was never even read, so nothing was ever replaced.
//
// This version uses QsciScintilla's own find/replace primitives
// (findFirst()/findNext()/replace()) directly instead of hand-rolling
// position bookkeeping, and actually branches on which action was
// requested.
//
void MainWindow::doSearchAndReplace(SearchReplaceAction action)
{
    if (!textEdit)
        return;

    const QString findText = lineEdit_find->text();
    if (findText.isEmpty())
    {
        createStatusBarMessage(tr("Nothing to search for."), 4000);
        return;
    }

    const bool caseSensitive = checkBox_CaseSensitive->isChecked();
    const bool wholeWords = checkBox_WholeWords->isChecked();
    const bool useRegExp = false;

    switch (action)
    {
    case SearchReplaceAction::FindNext:
    case SearchReplaceAction::FindPrevious:
    {
        const bool forward = (action == SearchReplaceAction::FindNext);
        const bool found = textEdit->findFirst(findText, useRegExp, caseSensitive, wholeWords,
                                                /*wrap=*/true, forward);
        if (!found)
            createStatusBarMessage(tr("\"%1\" not found.").arg(findText), 4000);
        break;
    }

    case SearchReplaceAction::Replace:
    {
        const QString replaceText = lineEdit_replace->text();

        // Rewritten (rev.101): the previous version only replaced when
        // the CURRENT selection's text compared exactly equal to the
        // search term (case/whole-word aware) - reported (and
        // reproduced) as needing the button pressed multiple times
        // before it ever actually replaced anything, apparently only
        // "waking up" after a Replace All had already run once. Rather
        // than chase that exact comparison's failure mode further, this
        // now simply trusts that ANY current selection is the match
        // Find/Next just left behind - the same assumption virtually
        // every editor's single "Replace" button makes - and replaces
        // it outright, then advances to the next occurrence so repeated
        // clicks step through the document one at a time.
        if (textEdit->hasSelectedText())
        {
            textEdit->replace(replaceText);
            if (!textEdit->findNext())
                createStatusBarMessage(tr("No more occurrences of \"%1\".").arg(findText), 4000);
        }
        else
        {
            const bool found = textEdit->findFirst(findText, useRegExp, caseSensitive, wholeWords,
                                                    /*wrap=*/true, /*forward=*/true);
            if (!found)
                createStatusBarMessage(tr("\"%1\" not found.").arg(findText), 4000);
        }
        break;
    }

    case SearchReplaceAction::ReplaceAll:
    {
        const QString replaceText = lineEdit_replace->text();

        // Always work through the whole document, from the very start,
        // regardless of where the caret currently sits.
        textEdit->setCursorPosition(0, 0);

        int count = 0;
        // Deliberately wrap=false here: after each replace() below, the
        // selection sits right after the freshly-inserted replacement
        // text, so searching onward from there (without wrapping back
        // to the start) can never re-match text this same pass already
        // replaced - safe even if replaceText itself contains findText.
        bool found = textEdit->findFirst(findText, useRegExp, caseSensitive, wholeWords,
                                          /*wrap=*/false, /*forward=*/true);
        while (found)
        {
            textEdit->replace(replaceText);
            ++count;
            found = textEdit->findNext();
        }

        if (count == 0)
            createStatusBarMessage(tr("\"%1\" not found.").arg(findText), 4000);
        else
            createStatusBarMessage(count == 1
                    ? tr("Replaced 1 occurrence of \"%1\".").arg(findText)
                    : tr("Replaced %1 occurrences of \"%2\".").arg(count).arg(findText), 5000);
        break;
    }
    }
}

//
// search & replace:
// on_btn_next() - search for next matching word
//
void MainWindow::on_btn_next()
{
    doSearchAndReplace(SearchReplaceAction::FindNext);
}

//
// search & replace:
// on_btn_previous() - search for previous matching word
//
void MainWindow::on_btn_previous()
{
    doSearchAndReplace(SearchReplaceAction::FindPrevious);
}

//
// search & replace:
// on_btn_replace() - replace current occourance of matching word
//
void MainWindow::on_btn_replace()
{
    doSearchAndReplace(SearchReplaceAction::Replace);
}

//
// search & replace:
// on_btn_replace_all() - replace all occourances of matching word
//
void MainWindow::on_btn_replace_all()
{
    doSearchAndReplace(SearchReplaceAction::ReplaceAll);
}

//
// search & replace:
// on_btn_hide() - hide searchGroup
//
void MainWindow::on_btn_hide()
{
    searchGroup->hide();
    p_search_is_open = false;
}

//
// Reset search Markers
//
void MainWindow::clearMarkers()
{
    int lastLine = textEdit->lines() - 1;
    textEdit->clearIndicatorRange( 0, 0, lastLine, textEdit->text( lastLine ).length() - 1, MY_MARKER_ID );
}
