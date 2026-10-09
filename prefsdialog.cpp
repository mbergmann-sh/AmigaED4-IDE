#include "prefsdialog.h"
#include "version.h"
#include "ui_prefsdialog.h"
#include "mainwindow.h"
#include <QMap>

PrefsDialog::PrefsDialog(QWidget *parent, int tabindex) :
    QDialog(parent),
    ui(new Ui::PrefsDialog)
{
    ui->setupUi(this);
    // Remove Close-, help-, size gadgets
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    // Overrides prefsdialog.ui's static "Prefs - AmigaED 4.0 " title with
    // one that includes the current revision (see version.h) - the .ui
    // file's own title is only ever seen in Designer, never at runtime.
    setWindowTitle(tr("Prefs - %1").arg(tr(AMIGAED_VERSION_STRING)));

    // Start Tabwidget with first tab visible allways
    ui->tabWidget->setCurrentIndex(tabindex);

    // set items for default style combobox:
    p_style_items << QStyleFactory::keys();
    ui->comboBoxDefaultStyle->addItems(p_style_items);

    // "Dark" is a synthetic entry, not a real Qt style key - see
    // MainWindow::applyApplicationStyle() for what selecting it actually
    // does (forces the "Fusion" style plus a dark QPalette, and switches
    // the editor itself to a matching dark colour scheme). Kept
    // untranslated on purpose, like the real style names above (Qt style
    // keys aren't translated either) - MISC/DefaultStyle is saved/restored
    // via currentText()/setCurrentText(), which would break across GUI
    // languages if this label were run through tr().
    ui->comboBoxDefaultStyle->insertSeparator(ui->comboBoxDefaultStyle->count());
    ui->comboBoxDefaultStyle->addItem(QStringLiteral("Dark"));
    // Same synthetic-entry treatment as "Dark" above - see MainWindow::
    // isWorkbench13Theme()/isWorkbench31Theme()/applyApplicationStyle()
    // for what selecting either one actually does.
    ui->comboBoxDefaultStyle->addItem(QStringLiteral("Workbench 1.3"));
    ui->comboBoxDefaultStyle->addItem(QStringLiteral("Workbench 3.1"));
    // Same synthetic-entry treatment again - a second, distinct dark
    // theme (rev.158) that reproduces Microsoft Visual Studio Code's own
    // "Dark+" colours as closely as possible, both for the application
    // chrome (MainWindow::vscodeApplicationPalette()) and for the editor
    // itself (MainWindow::applyLexerDarkColors()) - see MainWindow::
    // isVSCodeTheme() for what selecting it actually does.
    ui->comboBoxDefaultStyle->addItem(QStringLiteral("Visual Studio Code Dark"));

    // set items for default compiler combobox:
    ui->comboBoxDefaultCompiler->addItems(p_Compilers);

    // set items for default GUI language combobox (data = settings code,
    // text = the language's own name - shown untranslated on purpose, so
    // it stays readable no matter which language is currently active):
    ui->comboBoxDefaultGuiLanguage->addItem("English", "en");
    ui->comboBoxDefaultGuiLanguage->addItem("Deutsch", "de");

    // make "Cancel" the default button
    ui->btn_CancelSave->setFocus();

    // load global configuration
    load_mySettings();
    simpleStatusbar();
}

PrefsDialog::~PrefsDialog()
{
    delete ui;
}

void PrefsDialog::on_btn_SavePrefs_clicked()
{
   // Prefs > Tools: warn (but don't block saving - same non-blocking
   // spirit as every other path field in this dialog, e.g. the GCC/VBCC/
   // Emulator paths above are never checked either) about any non-empty
   // tool path that doesn't actually exist on THIS machine. This mirrors
   // exactly the existence check MainWindow::rebuildToolsMenu() performs
   // itself before adding a Tools-menu entry for that tool, so the user
   // understands up front why a configured tool's menu entry might not
   // show up.
   struct ToolPathCheck { QLineEdit *pathField; QString label; };
   const QList<ToolPathCheck> toolPathChecks = {
       { ui->lineEdit_Tool1Path, tr("MUI GUI Designer") },
       { ui->lineEdit_Tool2Path, tr("GadTools GUI Designer") },
       { ui->lineEdit_Tool3Path, tr("ReAction GUI Designer") },
       { ui->lineEdit_Tool4Path, tr("User Tool") },
   };
   QStringList missingToolPaths;
   for (const ToolPathCheck &check : toolPathChecks)
   {
       const QString path = check.pathField->text().trimmed();
       if (!path.isEmpty() && !QFileInfo::exists(path))
           missingToolPaths << tr("%1: %2").arg(check.label, path);
   }
   if (!missingToolPaths.isEmpty())
   {
       QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
           tr("The following Tools path(s) do not currently exist on this "
              "machine:\n\n%1\n\n"
              "Prefs will still be saved as entered, but the Tools menu "
              "will only show an entry for a tool once its path actually "
              "exists.").arg(missingToolPaths.join(QStringLiteral("\n"))));
   }

   save_mySettings();
   this->close();  // quit PrefsDialog

   QMessageBox::information(this, tr(AMIGAED_VERSION_STRING),
                       tr("Prefs saved.\n"
                          "Changes will be activated after restarting the application!"
                          "\n\nYou might consider saving all your work and restart now."),
                            QMessageBox::Ok);
}

void PrefsDialog::on_btn_getProjectRootDir_clicked()
{
    // getDir dialog
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_projectsRootDir->setText(dir);


}

void PrefsDialog::on_btn_getGCCexefile_clicked()
{
    QString exestring;
#if defined(__unix__)
    exestring = "/opt/amiga/bin/m68k-amigaos-gcc";
#else
    exestring = "m68k-amigaos-gcc.exe";
#endif
    // ToDO: FIX for executable without file extension!
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to gcc"), exestring,
            tr("All Files (*);;Executable (*.exe)"));
    ui->lineEdit_getGCCexefile->setText(fileName);

}

void PrefsDialog::on_btn_getGPPexefile_clicked()
{
    QString exestring;
#if defined(__unix__)
    exestring = "/opt/amiga/bin/m68k-amigaos-g++";
#else
    exestring = "m68k-amigaos-g++.exe";
#endif
    // ToDO: FIX for executable without file extension!
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to g++"), exestring,
            tr("All Files (*);;Executable (*.exe)"));
    ui->lineEdit_getGPPexefile->setText(fileName);
}

void PrefsDialog::on_btn_getMAKEexefile_clicked()
{
    QString exestring;
#if defined(__unix__)
    exestring = "/usr/bin/make";
#else
    exestring = "m68k-amigaos-make.exe";
#endif
    // ToDO: FIX for executable without file extension!
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to make"), exestring,
            tr("All Files (*);;Executable (*.exe)"));
    ui->lineEdit_getMAKEexefile->setText(fileName);
}

void PrefsDialog::on_btn_getSTRIPexefile_clicked()
{
    QString exestring;
#if defined(__unix__)
    exestring = "/opt/amiga/bin/m68k-amigaos-strip";
#else
    exestring = "m68k-amigaos-strip.exe";
#endif
    // ToDO: FIX for executable without file extension!
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to strip"), exestring,
            tr("All Files (*);;Executable (*.exe)"));
    ui->lineEdit_getSTRIPexefile->setText(fileName);
}

void PrefsDialog::on_btn_getASexefile_clicked()
{
    QString exestring;
#if defined(__unix__)
    exestring = "/opt/amiga/bin/m68k-amigaos-as";
#else
    exestring = "m68k-amigaos-as.exe";
#endif
    // ToDO: FIX for executable without file extension!
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to GNU as"), exestring,
            tr("All Files (*);;Executable (*.exe)"));
    ui->lineEdit_getASexefile->setText(fileName);
}

void PrefsDialog::on_btn_getLDexefile_clicked()
{
    QString exestring;
#if defined(__unix__)
    exestring = "/opt/amiga/bin/m68k-amigaos-ld";
#else
    exestring = "m68k-amigaos-ld.exe";
#endif
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to GNU ld"), exestring,
            tr("All Files (*);;Executable (*.exe)"));
    ui->lineEdit_getLDexefile->setText(fileName);
}

void PrefsDialog::on_btn_getASIncludeDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open GNU as Include Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
        ui->lineEdit_getASIncludeDir->setText(dir);
}

void PrefsDialog::on_btn_getVCexefile_clicked()
{
    QString exestring;
#if defined(__unix__)
    exestring = "/opt/amiga/bin/vc";
#else
    exestring = "vc";
#endif
    // ToDO: FIX for executable without file extension!
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to vc"), exestring,
            tr("All Files (*);;Executable (*.exe)"));
    ui->lineEdit_getVCexefile->setText(fileName);
}

void PrefsDialog::on_btn_getVASMexefile_clicked()
{
    QString exestring;
#if defined(__unix__)
    exestring = "/opt/amiga/bin/vasmm68k_mot";
#else
    exestring = "vasmm68k_mot.exe";
#endif
    // ToDO: FIX for executable without file extension!
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to vasm"), exestring,
            tr("All Files (*);;Executable (*.exe)"));
    ui->lineEdit_getVASMexefile->setText(fileName);
}

void PrefsDialog::on_btn_getVCconfigDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open VBCC config Directory"),
                                                 "/etc/",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_getVCconfigDir->setText(dir);
}

void PrefsDialog::on_btn_getVASMIncludeDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open vasm Include Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
        ui->lineEdit_getVASMIncludeDir->setText(dir);
}

void PrefsDialog::on_btn_getEmulatorExefile_clicked()
{
    QString exestring;
#if defined(__unix__)
    exestring = "/usr/bin/fs-uae";
#else
    exestring = "winUAE";
#endif
    // ToDO: FIX for executable without file extension!
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to Amiga emulator"), exestring,
            tr("All Files (*);;Executable (*.exe)"));
    ui->lineEdit_getEmulatorExefile->setText(fileName);
}


void PrefsDialog::on_btn_getOS13Configfile_clicked()
{
    QString exestring;
#if defined(__unix__)
    exestring = "/home";
#else
    exestring = "/home";
#endif
    // ToDO: FIX for executable without file extension!
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to AmigaOS 1.3 config file"), exestring,
            tr("All Files (*);;Executable (*.exe)"));
    ui->lineEdit_getOS13Configfile->setText(fileName);
}

void PrefsDialog::on_btn_getOS3Configfile_clicked()
{
    QString exestring;
#if defined(__unix__)
    exestring = "/home";
#else
    exestring = "/home";
#endif
    // ToDO: FIX for executable without file extension!
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to AmigaOS 3.x config file"), exestring,
            tr("All Files (*);;Executable (*.exe)"));
    ui->lineEdit_getOS3Configfile->setText(fileName);
}

//
// Opens filePath in the platform's own text editor - used by the two
// "Edit" buttons added next to the OS 1.3/3.x UAE config fields above,
// so the user can quickly hand-edit an existing (or not-yet-existing)
// UAE config file without having to go hunting for a text editor
// themselves outside AmigaED. Deliberately launches a concrete, known
// editor per platform rather than going through
// QDesktopServices::openUrl()'s OS-level file-association lookup - UAE
// config files typically have no file association at all (unlike a
// well-known extension like .txt), so openUrl() would often either do
// nothing or make the OS ask the user to pick an application, which
// defeats the point of a dedicated "Edit" button. Mirrors the same
// per-platform QProcess approach MainWindow::actionOpenShell() already
// uses for opening a terminal.
//
void PrefsDialog::openFileInSystemEditor(const QString &filePath)
{
    if (filePath.trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                              tr("Please select a config file first."));
        return;
    }

    bool started = false;

#if defined(Q_OS_WIN)
    started = QProcess::startDetached(QStringLiteral("notepad.exe"), { filePath });
#elif defined(Q_OS_MAC)
    // "-e" is Apple's documented way of forcing TextEdit specifically,
    // regardless of whatever application (if any) is actually
    // associated with the file.
    started = QProcess::startDetached(QStringLiteral("open"),
                                       { QStringLiteral("-e"), filePath });
#else
    // Linux has no single canonical default text editor, the same
    // problem actionOpenShell() already solves for terminals. xdg-open
    // is tried first - on a properly configured desktop it respects
    // whatever the user has set as their own default text editor - with
    // a short list of common GUI editors as a fallback for a system
    // with no such default configured at all. Note this only confirms
    // the chosen program itself could be launched, same as
    // actionOpenShell()'s own terminal candidates - it can't detect
    // xdg-open silently failing to find a handler internally once it's
    // running as its own detached process.
    static const QStringList candidates = {
        QStringLiteral("xdg-open"),
        QStringLiteral("gnome-text-editor"),
        QStringLiteral("gedit"),
        QStringLiteral("kate"),
        QStringLiteral("mousepad"),
        QStringLiteral("leafpad")
    };
    for (const QString &editor : candidates)
    {
        started = QProcess::startDetached(editor, { filePath });
        if (started)
            break;
    }
#endif

    if (!started)
    {
        QMessageBox::warning(this, tr(AMIGAED_VERSION_STRING),
                              tr("Could not open a text editor for:\n%1").arg(filePath));
    }
}

void PrefsDialog::on_btn_editOS13Configfile_clicked()
{
    openFileInSystemEditor(ui->lineEdit_getOS13Configfile->text());
}

void PrefsDialog::on_btn_editOS3Configfile_clicked()
{
    openFileInSystemEditor(ui->lineEdit_getOS3Configfile->text());
}

// Prefs > Emulator > "AutoDocs folder:" - a directory, not a single file
// (an NDK AutoDocs drawer holds one *.doc per library, e.g. exec.doc,
// dos.doc, ...) - see AutodocReader::parseAutodocsFolder() for how
// Build > AutoDoc Reader... consumes this setting.
void PrefsDialog::on_btn_getAutodocsDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open NDK AutoDocs Folder"),
                                                 ui->lineEdit_getAutodocsDir->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
        ui->lineEdit_getAutodocsDir->setText(dir);
}

//
// Prefs > Tools: the 4 file-selection buttons, one per GUI-builder/user
// tool slot - each just opens a file requester (starting at whatever the
// slot's own Path field already holds, same convention as the AutoDocs
// folder picker above) and writes the result back into that field. The
// actual "does this exist" check happens once, centrally, in
// on_btn_SavePrefs_clicked() below - not here, since the user may well be
// typing/pasting a path by hand instead of using this button at all.
//
void PrefsDialog::on_btn_getTool1Path_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to MUI GUI Designer"), ui->lineEdit_Tool1Path->text(),
            tr("All Files (*);;Executable (*.exe)"));
    if (!fileName.isEmpty())
        ui->lineEdit_Tool1Path->setText(fileName);
}

void PrefsDialog::on_btn_getTool2Path_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to GadTools GUI Designer"), ui->lineEdit_Tool2Path->text(),
            tr("All Files (*);;Executable (*.exe)"));
    if (!fileName.isEmpty())
        ui->lineEdit_Tool2Path->setText(fileName);
}

void PrefsDialog::on_btn_getTool3Path_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to ReAction GUI Designer"), ui->lineEdit_Tool3Path->text(),
            tr("All Files (*);;Executable (*.exe)"));
    if (!fileName.isEmpty())
        ui->lineEdit_Tool3Path->setText(fileName);
}

void PrefsDialog::on_btn_getTool4Path_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to User Tool"), ui->lineEdit_Tool4Path->text(),
            tr("All Files (*);;Executable (*.exe)"));
    if (!fileName.isEmpty())
        ui->lineEdit_Tool4Path->setText(fileName);
}


void PrefsDialog::on_btn_CancelSave_clicked()
{
    this->close();
}

void PrefsDialog::save_mySettings()
{
    // TAB: Project
    mySettings.setValue("Project/Author", ui->lineEdit_author->text());
    mySettings.setValue("Project/Email", ui->lineEdit_email->text());
    mySettings.setValue("Project/Website", ui->lineEdit_website->text());
    mySettings.setValue("Project/ProjectRootDir", ui->lineEdit_projectsRootDir->text());
    mySettings.setValue("Project/ProjectRootAmiga", ui->lineEdit_projectsRootAmiga->text().trimmed());   // rev.159
    mySettings.setValue("Project/SaveFilesAutomatically", ui->checkBox_saveProjectFilesAutomatically->isChecked());
    // Formerly two separate, independently-driftable UI fields (VBCC
    // tab's "Default Target OS" and Emulator tab's "Default config") for
    // what was always meant to be ONE setting - by explicit user
    // decision, now a single combo box here instead, right below the
    // checkbox above.
    mySettings.setValue("VBCC/VcDefaultTarget", ui->comboBox_defaultTargetOS->currentIndex());

    // TAB: GCC
    mySettings.setValue("GCC/GccPath", ui->lineEdit_getGCCexefile->text());
    mySettings.setValue("GCC/GppPath", ui->lineEdit_getGPPexefile->text());
    mySettings.setValue("GCC/MakePath", ui->lineEdit_getMAKEexefile->text());
    mySettings.setValue("GCC/StripPath", ui->lineEdit_getSTRIPexefile->text());
    mySettings.setValue("GCC/AsPath", ui->lineEdit_getASexefile->text());
    mySettings.setValue("GCC/LdPath", ui->lineEdit_getLDexefile->text());
    mySettings.setValue("GCC/AsIncludePath", ui->lineEdit_getASIncludeDir->text());
    mySettings.setValue("GCC/Gcc13CompilerOpts", ui->lineEdit_GCC13CompilerOpts->text());
    mySettings.setValue("GCC/Gcc30CompilerOpts", ui->lineEdit_GCC30CompilerOpts->text());
    mySettings.setValue("GCC/Gcc13LinkerOpts", ui->lineEdit_GCC13LinkerOpts->text());
    mySettings.setValue("GCC/Gcc30LinkerOpts", ui->lineEdit_GCC30LinkerOpts->text());
    mySettings.setValue("GCC/Gpp13CompilerOpts", ui->lineEdit_GPP13CompilerOpts->text());
    mySettings.setValue("GCC/Gpp30CompilerOpts", ui->lineEdit_GPP30CompilerOpts->text());
    mySettings.setValue("GCC/Gpp13LinkerOpts", ui->lineEdit_GPP13LinkerOpts->text());
    mySettings.setValue("GCC/Gpp30LinkerOpts", ui->lineEdit_GPP30LinkerOpts->text());
    mySettings.setValue("GCC/ShowGccDefaultOpts", ui->checkBox_ShowGccOpts->isChecked());

    // TAB: VBCC
    mySettings.setValue("VBCC/VcPath", ui->lineEdit_getVCexefile->text());
    mySettings.setValue("VBCC/VasmPath", ui->lineEdit_getVASMexefile->text());
    mySettings.setValue("VBCC/VcConfigPath", ui->lineEdit_getVCconfigDir->text());
    mySettings.setValue("VBCC/VasmIncludePath", ui->lineEdit_getVASMIncludeDir->text());
    mySettings.setValue("VBCC/VcDefaultOpts13", ui->lineEdit_VCdefaultOptsOS13->text());
    mySettings.setValue("VBCC/VcDefaultOpts30", ui->lineEdit_VCdefaultOptsOS30->text());
    mySettings.setValue("VBCC/Vc13LinkerOpts", ui->lineEdit_VC13LinkerOpts->text());
    mySettings.setValue("VBCC/Vc30LinkerOpts", ui->lineEdit_VCdefaultLinkerOpts->text());
    mySettings.setValue("SASC/DefaultOpts", ui->lineEdit_SASCdefaultOpts->text());
    mySettings.setValue("VBCC/ShowVbccDefaultOpts", ui->checkBox_ShowVbccOpts->isChecked());

    // TAB: Emulator
     mySettings.setValue("UAE/UaePath", ui->lineEdit_getEmulatorExefile->text());
     mySettings.setValue("UAE/Os13ConfigPath", ui->lineEdit_getOS13Configfile->text());
     mySettings.setValue("UAE/Os30ConfigPath", ui->lineEdit_getOS3Configfile->text());
     // Not a UAE setting itself (kept in its own "NDK" group), just placed
     // on this same tab - see AutodocReader for what consumes it.
     mySettings.setValue("NDK/AutodocsPath", ui->lineEdit_getAutodocsDir->text());

     // TAB: Tools - the 4 external GUI-builder/user tool slots. See
     // MainWindow::rebuildToolsMenu() for how these become the dynamic
     // Tools > GUI Builders / Tools > <User Tool name> menu entries (only
     // once BOTH Path and Name are non-empty AND Path exists on disk).
     mySettings.setValue("Tools/Tool1Path", ui->lineEdit_Tool1Path->text());
     mySettings.setValue("Tools/Tool1Params", ui->lineEdit_Tool1Params->text());
     mySettings.setValue("Tools/Tool1Name", ui->lineEdit_Tool1Name->text());
     mySettings.setValue("Tools/Tool2Path", ui->lineEdit_Tool2Path->text());
     mySettings.setValue("Tools/Tool2Params", ui->lineEdit_Tool2Params->text());
     mySettings.setValue("Tools/Tool2Name", ui->lineEdit_Tool2Name->text());
     mySettings.setValue("Tools/Tool3Path", ui->lineEdit_Tool3Path->text());
     mySettings.setValue("Tools/Tool3Params", ui->lineEdit_Tool3Params->text());
     mySettings.setValue("Tools/Tool3Name", ui->lineEdit_Tool3Name->text());
     mySettings.setValue("Tools/Tool4Path", ui->lineEdit_Tool4Path->text());
     mySettings.setValue("Tools/Tool4Params", ui->lineEdit_Tool4Params->text());
     mySettings.setValue("Tools/Tool4Name", ui->lineEdit_Tool4Name->text());
     mySettings.setValue("Tools/FlexCatPath", ui->lineEdit_FlexCatPath->text().trimmed());   // rev.159

     // TAB: vamos (rev.159) - see MainWindow::runExecutableInVamos() for
     // how these build the vamos command line.
     mySettings.setValue("VAMOS/Command", ui->lineEdit_vamosCommand->text().trimmed());
     mySettings.setValue("VAMOS/UseWSL", ui->checkBox_vamosWSL->isChecked());
     mySettings.setValue("VAMOS/WorkbenchDir", ui->lineEdit_vamosWorkbenchDir->text().trimmed());
     mySettings.setValue("VAMOS/WorkDir", ui->lineEdit_vamosWorkDir->text().trimmed());
     mySettings.setValue("VAMOS/ExtraOpts", ui->lineEdit_vamosOpts->text().trimmed());
     mySettings.setValue("VAMOS/SascDir", ui->lineEdit_vamosSascDir->text().trimmed());
     mySettings.setValue("VAMOS/MuiDir", ui->lineEdit_vamosMuiDir->text().trimmed());

     // TAB: Misc
     mySettings.setValue("MISC/DefaultStyle", ui->comboBoxDefaultStyle->currentText());
     mySettings.setValue("MISC/ShowIndentGuide", ui->checkBoxIndentationLines->isChecked());
     mySettings.setValue("MISC/ShowDebugOutput", ui->checkBoxDebugOutput->isChecked());
     mySettings.setValue("MISC/NoLCDstatusbar", ui->checkBoxNoLCD->isChecked());
     mySettings.setValue("MISC/NoCompileButton", ui->checkBoxNoCompileButton->isChecked());
     mySettings.setValue("MISC/SimpleStatusbar", ui->checkBoxSimpleStatusbar->isChecked());
     mySettings.setValue("MISC/DefaultCrossCompiler", ui->comboBoxDefaultCompiler->currentIndex());
     mySettings.setValue("MISC/CreateIcon", ui->checkBoxCreateIcon->isChecked());
     mySettings.setValue("MISC/OpenConsoleOnFail", ui->checkBoxOpenOnFail->isChecked());
     mySettings.setValue("MISC/NoWarnRequester", ui->checkBoxWarnRequesters->isChecked());
     mySettings.setValue("MISC/DefaultGUILanguage", ui->comboBoxDefaultGuiLanguage->currentData().toString());
     mySettings.setValue("MISC/HighlightBraceBlock", ui->checkBoxHighlightBraceBlock->isChecked());
     mySettings.setValue("MISC/NoSplashScreen", ui->checkBoxNoSplashScreen->isChecked());
}

void PrefsDialog::load_mySettings()
{
    // TAB: Project
    ui->lineEdit_author->setText(mySettings.value("Project/Author").toString());
    ui->lineEdit_email->setText(mySettings.value("Project/Email").toString());
    ui->lineEdit_website->setText(mySettings.value("Project/Website").toString());
    ui->lineEdit_projectsRootDir->setText(mySettings.value("Project/ProjectRootDir").toString());
    ui->lineEdit_projectsRootAmiga->setText(mySettings.value("Project/ProjectRootAmiga").toString());   // rev.159
    ui->checkBox_saveProjectFilesAutomatically->setChecked(mySettings.value("Project/SaveFilesAutomatically", false).toBool());
    // Formerly two separate, independently-driftable UI fields (VBCC
    // tab's "Default Target OS" and Emulator tab's "Default config") for
    // what was always meant to be ONE setting - by explicit user
    // decision, now a single combo box here instead.
    ui->comboBox_defaultTargetOS->setCurrentIndex(mySettings.value("VBCC/VcDefaultTarget").toInt());

    // TAB: GCC
    ui->lineEdit_getGCCexefile->setText(mySettings.value("GCC/GccPath").toString());
    ui->lineEdit_getGPPexefile->setText(mySettings.value("GCC/GppPath").toString());
    ui->lineEdit_getMAKEexefile->setText(mySettings.value("GCC/MakePath").toString());
    ui->lineEdit_getSTRIPexefile->setText(mySettings.value("GCC/StripPath").toString());
    ui->lineEdit_getASexefile->setText(mySettings.value("GCC/AsPath").toString());
    ui->lineEdit_getLDexefile->setText(mySettings.value("GCC/LdPath").toString());
    ui->lineEdit_getASIncludeDir->setText(mySettings.value("GCC/AsIncludePath").toString());
    ui->lineEdit_GCC13CompilerOpts->setText(mySettings.value("GCC/Gcc13CompilerOpts").toString());
    ui->lineEdit_GCC30CompilerOpts->setText(mySettings.value("GCC/Gcc30CompilerOpts").toString());
    ui->lineEdit_GCC13LinkerOpts->setText(mySettings.value("GCC/Gcc13LinkerOpts").toString());
    ui->lineEdit_GCC30LinkerOpts->setText(mySettings.value("GCC/Gcc30LinkerOpts").toString());
    ui->lineEdit_GPP13CompilerOpts->setText(mySettings.value("GCC/Gpp13CompilerOpts").toString());
    ui->lineEdit_GPP30CompilerOpts->setText(mySettings.value("GCC/Gpp30CompilerOpts").toString());
    ui->lineEdit_GPP13LinkerOpts->setText(mySettings.value("GCC/Gpp13LinkerOpts").toString());
    ui->lineEdit_GPP30LinkerOpts->setText(mySettings.value("GCC/Gpp30LinkerOpts").toString());
    ui->checkBox_ShowGccOpts->setChecked(mySettings.value("GCC/ShowGccDefaultOpts").toBool());

    // TAB: VBCC
    ui->lineEdit_getVCexefile->setText(mySettings.value("VBCC/VcPath").toString());
    ui->lineEdit_getVASMexefile->setText(mySettings.value("VBCC/VasmPath").toString());
    ui->lineEdit_getVCconfigDir->setText(mySettings.value("VBCC/VcConfigPath").toString());
    ui->lineEdit_getVASMIncludeDir->setText(mySettings.value("VBCC/VasmIncludePath").toString());
    ui->lineEdit_VCdefaultOptsOS13->setText(mySettings.value("VBCC/VcDefaultOpts13").toString());
    ui->lineEdit_VCdefaultOptsOS30->setText(mySettings.value("VBCC/VcDefaultOpts30").toString());
    ui->lineEdit_VC13LinkerOpts->setText(mySettings.value("VBCC/Vc13LinkerOpts").toString());
    ui->lineEdit_VCdefaultLinkerOpts->setText(mySettings.value("VBCC/Vc30LinkerOpts").toString());
    ui->lineEdit_SASCdefaultOpts->setText(mySettings.value("SASC/DefaultOpts").toString());
    ui->checkBox_ShowVbccOpts->setChecked(mySettings.value("VBCC/ShowVbccDefaultOpts").toBool());

    // TAB: Emulator
    ui->lineEdit_getEmulatorExefile->setText(mySettings.value("UAE/UaePath").toString());
    ui->lineEdit_getOS13Configfile->setText(mySettings.value("UAE/Os13ConfigPath").toString());
    ui->lineEdit_getOS3Configfile->setText(mySettings.value("UAE/Os30ConfigPath").toString());
    ui->lineEdit_getAutodocsDir->setText(mySettings.value("NDK/AutodocsPath").toString());

    // TAB: Misc
    ui->comboBoxDefaultStyle->setCurrentText(mySettings.value("MISC/DefaultStyle").toString());
    ui->checkBoxIndentationLines->setChecked(mySettings.value("MISC/ShowIndentGuide").toBool());
    ui->checkBoxDebugOutput->setChecked(mySettings.value("MISC/ShowDebugOutput").toBool());
    ui->checkBoxNoLCD->setChecked(mySettings.value("MISC/NoLCDstatusbar").toBool());
    ui->checkBoxNoCompileButton->setChecked(mySettings.value("MISC/NoCompileButton").toBool());
    ui->checkBoxSimpleStatusbar->setChecked(mySettings.value("MISC/SimpleStatusbar").toBool());
    ui->comboBoxDefaultCompiler->setCurrentIndex(mySettings.value("MISC/DefaultCrossCompiler").toInt());
    ui->checkBoxCreateIcon->setChecked(mySettings.value("MISC/CreateIcon").toBool());
    ui->checkBoxOpenOnFail->setChecked(mySettings.value("MISC/OpenConsoleOnFail").toBool());
    ui->checkBoxWarnRequesters->setChecked(mySettings.value("MISC/NoWarnRequester").toBool());

    int guiLangIndex = ui->comboBoxDefaultGuiLanguage->findData(mySettings.value("MISC/DefaultGUILanguage", "en").toString());
    ui->comboBoxDefaultGuiLanguage->setCurrentIndex(guiLangIndex >= 0 ? guiLangIndex : 0);
    ui->checkBoxHighlightBraceBlock->setChecked(mySettings.value("MISC/HighlightBraceBlock").toBool());
    // Default unchecked (splash shown) - see main.cpp, which reads this
    // same "MISC/NoSplashScreen" key directly (rather than through
    // MainWindow's usual p_xxx readSettings() mirror) since it has to
    // decide whether to show the splash BEFORE MainWindow even exists.
    ui->checkBoxNoSplashScreen->setChecked(mySettings.value("MISC/NoSplashScreen", false).toBool());

    // TAB: Tools - deliberately loaded AFTER the Misc tab above, since
    // Tool 1's own first-run default Parameters value (built below) reuses
    // whatever GUI language/theme the Misc tab just loaded, so a freshly
    // installed AmigaED launches MuiBuilderQt session-matched to AmigaED's
    // own current View > GUI Language / View > Theme choice right out of
    // the box. QSettings::value(key, default) only substitutes "default"
    // when the key is entirely absent (never for a key the user
    // deliberately saved as empty), so this only ever applies on a truly
    // unconfigured slot - once the user has saved Prefs once (even leaving
    // a field blank), that saved value always wins from then on.
    QString defaultTool1Path;
#if defined(Q_OS_WIN)
    defaultTool1Path = QStringLiteral("C:\\MuiBuilderQt\\MUIBuilderQt.exe");
#else
    defaultTool1Path = QStringLiteral("/usr/bin/MUIBuilderQt");
#endif
    // Reuses MuiBuilderQt's own --GUI_Language/--GUI_Theme CLI options
    // (see MuiBuilderQt/gui/main.cpp) - quoted defensively since a
    // synthetic theme name can contain spaces (e.g. "Visual Studio Code
    // Dark"), same as any hand-typed value would need to be once this
    // string is split back into arguments by QProcess::splitCommand() in
    // MainWindow::launchToolAction().
    const QString defaultGuiLanguage = ui->comboBoxDefaultGuiLanguage->currentData().toString();
    const QString defaultTheme = ui->comboBoxDefaultStyle->currentText();
    const QString defaultTool1Params = QStringLiteral("--GUI_Language=\"%1\" --GUI_Theme=\"%2\"")
        .arg(defaultGuiLanguage, defaultTheme);

    ui->lineEdit_Tool1Path->setText(mySettings.value("Tools/Tool1Path", defaultTool1Path).toString());
    ui->lineEdit_Tool1Params->setText(mySettings.value("Tools/Tool1Params", defaultTool1Params).toString());
    ui->lineEdit_Tool1Name->setText(mySettings.value("Tools/Tool1Name", tr("MUI GUI Designer")).toString());
    ui->lineEdit_Tool2Path->setText(mySettings.value("Tools/Tool2Path").toString());
    ui->lineEdit_Tool2Params->setText(mySettings.value("Tools/Tool2Params").toString());
    ui->lineEdit_Tool2Name->setText(mySettings.value("Tools/Tool2Name").toString());
    ui->lineEdit_Tool3Path->setText(mySettings.value("Tools/Tool3Path").toString());
    ui->lineEdit_Tool3Params->setText(mySettings.value("Tools/Tool3Params").toString());
    ui->lineEdit_Tool3Name->setText(mySettings.value("Tools/Tool3Name").toString());
    ui->lineEdit_Tool4Path->setText(mySettings.value("Tools/Tool4Path").toString());
    ui->lineEdit_Tool4Params->setText(mySettings.value("Tools/Tool4Params").toString());
    ui->lineEdit_Tool4Name->setText(mySettings.value("Tools/Tool4Name").toString());
    ui->lineEdit_FlexCatPath->setText(mySettings.value("Tools/FlexCatPath").toString());   // rev.159

    // TAB: vamos (rev.159) - defaults must match MainWindow::readSettings()
    ui->lineEdit_vamosCommand->setText(mySettings.value("VAMOS/Command", vamosDefaultCommand()).toString());
    ui->checkBox_vamosWSL->setChecked(mySettings.value("VAMOS/UseWSL", vamosDefaultUseWSL()).toBool());
    ui->lineEdit_vamosWorkbenchDir->setText(mySettings.value("VAMOS/WorkbenchDir").toString());
    ui->lineEdit_vamosWorkDir->setText(mySettings.value("VAMOS/WorkDir").toString());
    ui->lineEdit_vamosOpts->setText(mySettings.value("VAMOS/ExtraOpts", QStringLiteral("-m 8000 -s 256")).toString());
    ui->lineEdit_vamosSascDir->setText(mySettings.value("VAMOS/SascDir", QStringLiteral("workbench:SAS-C")).toString());
    ui->lineEdit_vamosMuiDir->setText(mySettings.value("VAMOS/MuiDir", QStringLiteral("work:MUI")).toString());
}

void PrefsDialog::on_checkBoxSimpleStatusbar_clicked()
{
    if(ui->checkBoxSimpleStatusbar->isChecked())
    {
        ui->checkBoxNoCompileButton->setChecked(true);
        ui->checkBoxNoCompileButton->setDisabled(true);
        ui->checkBoxNoLCD->setChecked(true);
        ui->checkBoxNoLCD->setDisabled(true);
    }
    else
    {
        ui->checkBoxNoCompileButton->setChecked(false);
        ui->checkBoxNoCompileButton->setDisabled(false);
        ui->checkBoxNoLCD->setChecked(false);
        ui->checkBoxNoLCD->setDisabled(false);
    }
}

//
// if both statusbar comfort options are disabled:
// Select simpleStatusbar as default!
//
void PrefsDialog::simpleStatusbar()
{
    if((ui->checkBoxNoCompileButton->isChecked()) && (ui->checkBoxNoLCD->isChecked()))
    {
        ui->checkBoxNoCompileButton->setChecked(true);
        ui->checkBoxNoCompileButton->setDisabled(true);
        ui->checkBoxNoLCD->setChecked(true);
        ui->checkBoxNoLCD->setDisabled(true);
        ui->checkBoxSimpleStatusbar->setChecked(true);
    }
}

void PrefsDialog::on_checkBoxNoLCD_clicked()
{
    simpleStatusbar();
}

void PrefsDialog::on_checkBoxNoCompileButton_clicked()
{
    simpleStatusbar();
}


//
// Prefs > vamos (rev.159): platform defaults, shared with MainWindow::
// readSettings() so both sides agree on what "not configured yet" means.
// On Windows vamos only runs inside WSL (Python + machine68k), so the
// default calls it through wsl.exe from the amitools venv the setup guide
// creates; everywhere else it's simply "vamos" from the PATH.
//
QString PrefsDialog::vamosDefaultCommand()
{
#ifdef Q_OS_WIN
    return QStringLiteral("wsl ~/amitools-venv/bin/vamos");
#else
    return QStringLiteral("vamos");
#endif
}

bool PrefsDialog::vamosDefaultUseWSL()
{
#ifdef Q_OS_WIN
    return true;
#else
    return false;
#endif
}

//
// Prefs > vamos: folder pickers for the emulator's partitions. Plain host
// folders (FS-UAE/WinUAE "directory" hard drives) - vamos mounts them as
// volumes workbench:/work:, see MainWindow::runExecutableInVamos().
//
void PrefsDialog::on_btn_getVamosWorkbenchDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Host folder of the \"Workbench\" partition"),
                                                    ui->lineEdit_vamosWorkbenchDir->text());
    if (!dir.isEmpty())
        ui->lineEdit_vamosWorkbenchDir->setText(QDir::toNativeSeparators(dir));
}

void PrefsDialog::on_btn_getVamosWorkDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Host folder of the \"Work\" partition"),
                                                    ui->lineEdit_vamosWorkDir->text());
    if (!dir.isEmpty())
        ui->lineEdit_vamosWorkDir->setText(QDir::toNativeSeparators(dir));
}

//
// Prefs > Tools > "Multilingual Programs" (rev.159): FlexCat executable,
// with its "sd" folder next to it - see MainWindow::flexCatExecutable().
//
void PrefsDialog::on_btn_getFlexCatPath_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path to FlexCat"), ui->lineEdit_FlexCatPath->text(),
            tr("All Files (*);;Executable (*.exe)"));
    if (!fileName.isEmpty())
        ui->lineEdit_FlexCatPath->setText(QDir::toNativeSeparators(fileName));
}

//
// rev.159: the folder drives ("directory hard drives") of an emulator
// configuration - which host folder appears under which Amiga volume name.
//   WinUAE (.uae):  filesystem2=rw,DH1:Work:D:\WinUAE\Harddisks\Work,0
//                   (access,device:volume:path,bootpri; path may contain ':')
//   FS-UAE:         hard_drive_1 = /path/Work
//                   hard_drive_1_label = Work      (default: folder name)
//                   hard_drive_1_priority = -128
// Hardfiles (.hdf) are skipped - AmigaED can't write into those.
//
QList<PrefsDialog::EmuMount> PrefsDialog::emulatorMounts(const QString &configPath)
{
    QList<EmuMount> mounts;
    QFile f(configPath);
    if (configPath.trimmed().isEmpty() || !f.open(QIODevice::ReadOnly | QIODevice::Text))
        return mounts;
    const QString cfgDir = QFileInfo(configPath).absolutePath();
    QMap<int, EmuMount> fsuae;

    const QStringList lines = QString::fromUtf8(f.readAll()).split(QLatin1Char('\n'));
    static const QRegularExpression fsKey(QStringLiteral("^hard_drive_(\\d+)(_label|_priority)?$"));
    for (QString line : lines)
    {
        line = line.trimmed();
        const int eq = line.indexOf(QLatin1Char('='));
        if (eq <= 0 || line.startsWith(QLatin1Char('#')) || line.startsWith(QLatin1Char(';')))
            continue;
        const QString key = line.left(eq).trimmed();
        QString val = line.mid(eq + 1).trimmed();

        if (key == QLatin1String("filesystem2"))
        {
            // access , device:volume:path , bootpri
            const int c1 = val.indexOf(QLatin1Char(','));
            const int cl = val.lastIndexOf(QLatin1Char(','));
            if (c1 < 0 || cl <= c1)
                continue;
            const QString mid = val.mid(c1 + 1, cl - c1 - 1);
            const int d1 = mid.indexOf(QLatin1Char(':'));
            const int d2 = mid.indexOf(QLatin1Char(':'), d1 + 1);
            if (d1 < 0 || d2 < 0)
                continue;
            EmuMount m;
            m.volume = mid.mid(d1 + 1, d2 - d1 - 1);
            m.hostPath = mid.mid(d2 + 1);
            if (m.hostPath.startsWith(QLatin1Char('"')) && m.hostPath.endsWith(QLatin1Char('"')))
                m.hostPath = m.hostPath.mid(1, m.hostPath.length() - 2);
            m.bootPri = val.mid(cl + 1).trimmed().toInt();
            if (!m.volume.isEmpty() && QFileInfo(m.hostPath).isDir())
                mounts << m;
            continue;
        }

        QRegularExpressionMatch fm = fsKey.match(key);
        if (fm.hasMatch())
        {
            EmuMount &m = fsuae[fm.captured(1).toInt()];
            if (fm.captured(2) == QLatin1String("_label"))
                m.volume = val;
            else if (fm.captured(2) == QLatin1String("_priority"))
                m.bootPri = val.toInt();
            else
                m.hostPath = QDir::isRelativePath(val) ? QDir(cfgDir).filePath(val) : val;
        }
    }
    for (auto it = fsuae.begin(); it != fsuae.end(); ++it)
    {
        EmuMount m = it.value();
        if (m.hostPath.isEmpty() || !QFileInfo(m.hostPath).isDir())
            continue;
        if (m.volume.isEmpty())
            m.volume = QFileInfo(m.hostPath).fileName();
        mounts << m;
    }
    return mounts;
}

// "Work:Projekte" / "Projekte:" yes - "D:/x" (a Windows drive letter),
// "/home/x" or "x/y" no
bool PrefsDialog::looksLikeAmigaPath(const QString &path)
{
    const QString p = path.trimmed();
    const int colon = p.indexOf(QLatin1Char(':'));
    return colon >= 2 && !p.contains(QLatin1Char('\\')) && !p.startsWith(QLatin1Char('/'));
}

//
// rev.159: where the emulated Amiga sees a host path - shared by the
// "Set up start script" button and MainWindow's "Start in emulator".
//   1. inside the projects root and its Amiga path is set: <amigaRoot>/<rest>
//   2. inside a folder drive of the emulator configuration: <Volume>:<rest>
//      (the deepest matching drive wins)
//   3. inside the Work / Workbench folder of Prefs > vamos: Work:<rest> ...
// Returns an empty string if none applies.
//
QString PrefsDialog::amigaPathForHostPath(const QString &hostPath, const QString &projectsRootHost,
                                          const QString &projectsRootAmiga, const QString &emuConfig,
                                          const QString &workbenchHost, const QString &workHost)
{
    auto rel = [](const QString &path, const QString &root, QString *out) -> bool
    {
        if (root.trimmed().isEmpty())
            return false;
        const QString p = QDir::cleanPath(QDir::fromNativeSeparators(QFileInfo(path).absoluteFilePath()));
        const QString r = QDir::cleanPath(QDir::fromNativeSeparators(QFileInfo(root.trimmed()).absoluteFilePath()));
#ifdef Q_OS_WIN
        const Qt::CaseSensitivity cs = Qt::CaseInsensitive;
#else
        const Qt::CaseSensitivity cs = Qt::CaseSensitive;
#endif
        if (p.compare(r, cs) == 0) { *out = QString(); return true; }
        if (!p.startsWith(r.endsWith(QLatin1Char('/')) ? r : r + QLatin1Char('/'), cs))
            return false;
        *out = p.mid(r.length() + (r.endsWith(QLatin1Char('/')) ? 0 : 1));
        return true;
    };
    auto join = [](QString base, const QString &rest) -> QString
    {
        if (rest.isEmpty())
            return base;
        if (!base.endsWith(QLatin1Char(':')) && !base.endsWith(QLatin1Char('/')))
            base += QLatin1Char('/');
        return base + rest;   // never "Vol:/x" - a '/' right after the colon means "parent" on the Amiga
    };

    QString r;
    if (looksLikeAmigaPath(projectsRootAmiga) && rel(hostPath, projectsRootHost, &r))
        return join(projectsRootAmiga.trimmed(), r);

    QString best, bestRel;
    int bestLen = -1;
    for (const EmuMount &m : emulatorMounts(emuConfig))
    {
        QString mr;
        if (rel(hostPath, m.hostPath, &mr) && m.hostPath.length() > bestLen)
        {
            bestLen = m.hostPath.length();
            best = m.volume + QLatin1Char(':');
            bestRel = mr;
        }
    }
    if (bestLen >= 0)
        return join(best, bestRel);

    if (rel(hostPath, workHost, &r))
        return join(QStringLiteral("Work:"), r);
    if (rel(hostPath, workbenchHost, &r))
        return join(QStringLiteral("Workbench:"), r);
    return QString();
}

// The host path of the Amiga's S:User-Startup: in the bootable folder drive
// (highest boot priority) of the emulator configuration, else in the
// Workbench folder of Prefs > vamos. Empty if unknown or not there.
QString PrefsDialog::bootUserStartup(const QString &emuConfig, const QString &workbenchHost)
{
    QString bootDir;
    int pri = -129;
    for (const EmuMount &m : emulatorMounts(emuConfig))
        if (m.bootPri > -128 && m.bootPri > pri)
        {
            pri = m.bootPri;
            bootDir = m.hostPath;
        }
    if (bootDir.isEmpty())
        bootDir = workbenchHost.trimmed();
    if (bootDir.isEmpty())
        return QString();
    // the drawer may be "S" or "s" on a case-sensitive host file system
    for (const char *s : { "S/User-Startup", "s/User-Startup", "S/user-startup", "s/user-startup" })
    {
        const QString p = QDir(bootDir).filePath(QLatin1String(s));
        if (QFileInfo(p).isFile())
            return p;
    }
    return QString();
}

QString PrefsDialog::startScriptBlock(const QString &amigaJobDir)
{
    // The runner is COPIED to T: (RAM) and executed from there: AmigaDOS
    // reads a running script line by line from its file (and "Skip BACK"
    // seeks in it), so AmigaED rewriting <job dir>/autorun on the host while
    // the Amiga executes it would derail the runner - it then hangs right
    // after taking a job. A changed runner takes effect at the next boot.
    return QStringLiteral(";BEGIN AmigaED - starts AmigaED's job runner (see AmigaED manual)\n"
                          "If EXISTS \"%1/autorun\"\n"
                          "  Copy \"%1/autorun\" T:AmigaED-autorun QUIET\n"
                          "  Run >NIL: Execute T:AmigaED-autorun\n"
                          "EndIf\n"
                          ";END AmigaED\n").arg(amigaJobDir);
}

//
// rev.159: AmigaED's job runner <job dir>/autorun - polls for "job" files
// and executes them. Written by the "Set up start script" button AND before
// every "Start in emulator", so it always matches the current Amiga path.
// Refuses anything but an Amiga path: a host path like "D:/Projekte/..."
// would make the Amiga ask for a volume "D" at every boot.
//
bool PrefsDialog::writeJobRunner(const QString &hostJobDir, const QString &J)
{
    if (!looksLikeAmigaPath(J))
        return false;
    QDir().mkpath(hostJobDir);
    QFile f(QDir(hostJobDir).filePath(QStringLiteral("autorun")));
    const QString text = QStringLiteral(
        "; AmigaED job runner - started from S:User-Startup, do not edit\n"
        "; (written by AmigaED; executes \"job\" files AmigaED drops here)\n"
        "Lab loop\n"
        "If EXISTS \"%1/job\"\n"
        "  Delete >NIL: \"%1/job.done\" \"%1/job.log\" \"%1/job.run\" QUIET\n"
        "  Rename \"%1/job\" \"%1/job.run\"\n"
        "  Execute \"%1/job.run\" >\"%1/job.log\"\n"
        "  Echo >\"%1/job.done\" \"rc=$RC\"\n"
        "  Delete >NIL: \"%1/job.run\" QUIET\n"
        "EndIf\n"
        "Wait 1\n"
        "Skip loop BACK\n").arg(J);
    if (f.open(QIODevice::ReadOnly) && f.readAll() == text.toLatin1())
        return true;   // unchanged - leave the file alone
    f.close();
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;
    return f.write(text.toLatin1()) >= 0;   // Amiga text: ISO-8859-1, LF only
}

bool PrefsDialog::hasStartScriptBlock(const QString &userStartupPath, const QString &amigaJobDir)
{
    QFile f(userStartupPath);
    if (!f.open(QIODevice::ReadOnly))
        return false;
    return QString::fromLatin1(f.readAll()).contains(startScriptBlock(amigaJobDir));
}

// Adds AmigaED's block to the Amiga's S:User-Startup, or replaces an older
// one (e.g. for another job folder). Keeps a backup in User-Startup.bak.
bool PrefsDialog::installStartScript(const QString &file, const QString &amigaJobDir, QString *message)
{
    if (!looksLikeAmigaPath(amigaJobDir))
    {
        if (message) *message = tr("\"%1\" is not an Amiga path - nothing was changed.").arg(amigaJobDir);
        return false;
    }
    QString text;
    {
        QFile f(file);
        if (!f.open(QIODevice::ReadOnly))
        {
            if (message) *message = tr("Could not read:\n%1").arg(QDir::toNativeSeparators(file));
            return false;
        }
        text = QString::fromLatin1(f.readAll());   // Amiga text: ISO-8859-1
    }
    QFile::remove(file + QStringLiteral(".bak"));
    QFile::copy(file, file + QStringLiteral(".bak"));

    static const QRegularExpression oldBlock(QStringLiteral(";BEGIN AmigaED[^\\n]*\\n.*?;END AmigaED\\n?"),
                                             QRegularExpression::DotMatchesEverythingOption);
    const QString block = startScriptBlock(amigaJobDir);
    const bool existed = text.contains(oldBlock);
    if (existed)
        text.replace(oldBlock, block);
    else
    {
        if (!text.isEmpty() && !text.endsWith(QLatin1Char('\n')))
            text += QLatin1Char('\n');
        text += QStringLiteral("\n") + block;
    }

    QFile f(file);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate) || f.write(text.toLatin1()) < 0)
    {
        if (message) *message = tr("Could not write:\n%1\n\n%2").arg(QDir::toNativeSeparators(file), f.errorString());
        return false;
    }
    if (message)
        *message = (existed ? tr("The AmigaED block in %1 was updated:") : tr("This block was added to %1:"))
                       .arg(QDir::toNativeSeparators(file))
                   + QStringLiteral("\n\n") + block
                   + QStringLiteral("\n") + tr("A backup is in User-Startup.bak. It takes effect at the Amiga's next boot.");
    return true;
}

//
// Prefs > Emulator > "Set up start script in S:User-Startup..." (rev.159)
//
void PrefsDialog::on_btn_setupStartScript_clicked()
{
    const QString root = ui->lineEdit_projectsRootDir->text().trimmed();
    if (root.isEmpty())
    {
        QMessageBox::warning(this, tr("Set up start script"), tr("Please set the projects root on the Project tab first."));
        return;
    }
    const QString jobDir = amigaPathForHostPath(root + QStringLiteral("/AmigaED-Jobs"), root,
                                                ui->lineEdit_projectsRootAmiga->text(), ui->lineEdit_getOS3Configfile->text(),
                                                ui->lineEdit_vamosWorkbenchDir->text(), ui->lineEdit_vamosWorkDir->text());
    if (jobDir.isEmpty())
    {
        QMessageBox::warning(this, tr("Set up start script"),
                             tr("Your projects root isn't inside any folder drive of the emulator configuration:\n%1\n\n"
                                "Add the projects root (or a folder containing it) to your emulator as a folder "
                                "hard drive, or enter its Amiga path on the Project tab.")
                                 .arg(QDir::toNativeSeparators(root)));
        return;
    }

    QString file = bootUserStartup(ui->lineEdit_getOS3Configfile->text(), ui->lineEdit_vamosWorkbenchDir->text());
    if (file.isEmpty())
    {
        file = QFileDialog::getOpenFileName(this, tr("Select the Amiga's S:User-Startup"), root);
        if (file.isEmpty())
            return;
    }

    QString msg;
    if (installStartScript(file, jobDir, &msg))
    {
        writeJobRunner(root + QStringLiteral("/AmigaED-Jobs"), jobDir);   // block and runner always match
        QMessageBox::information(this, tr("Set up start script"), msg);
    }
    else
        QMessageBox::warning(this, tr("Set up start script"), msg);
}

//
// Prefs > Project > "Projects root on the Amiga" - folder dialog (rev.159):
// pick the projects root (or any folder) as it lies inside one of the
// emulator's folder drives; AmigaED turns it into the Amiga path.
//
void PrefsDialog::on_btn_getProjectsRootAmiga_clicked()
{
    const QString start = ui->lineEdit_projectsRootDir->text().trimmed();
    const QString dir = QFileDialog::getExistingDirectory(this, tr("Projects root on the Amiga"), start);
    if (dir.isEmpty())
        return;
    const QString amiga = amigaPathForHostPath(dir, QString(), QString(), ui->lineEdit_getOS3Configfile->text(),
                                               ui->lineEdit_vamosWorkbenchDir->text(), ui->lineEdit_vamosWorkDir->text());
    if (amiga.isEmpty())
    {
        QString drives;
        for (const EmuMount &m : emulatorMounts(ui->lineEdit_getOS3Configfile->text()))
            drives += QStringLiteral("\n  %1:  =  %2").arg(m.volume, QDir::toNativeSeparators(m.hostPath));
        QMessageBox::warning(this, tr("Projects root on the Amiga"),
                             tr("This folder isn't inside any folder drive of your emulator configuration (Emulator tab, OS 3.x):\n%1\n\n"
                                "Drives in that configuration:%2\n\n"
                                "Add the folder (or one containing it) to the emulator as a folder hard drive first.")
                                 .arg(QDir::toNativeSeparators(dir), drives.isEmpty() ? tr("\n  (none found)") : drives));
        return;
    }
    ui->lineEdit_projectsRootAmiga->setText(amiga);
}
