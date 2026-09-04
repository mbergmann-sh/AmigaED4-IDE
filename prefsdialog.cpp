#include "prefsdialog.h"
#include "version.h"
#include "ui_prefsdialog.h"
#include "mainwindow.h"

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
    mySettings.setValue("Project/DefaultIcon", ui->lineEdit_getDefaultIcon->text());

    // TAB: GCC
    mySettings.setValue("GCC/GccPath", ui->lineEdit_getGCCexefile->text());
    mySettings.setValue("GCC/GppPath", ui->lineEdit_getGPPexefile->text());
    mySettings.setValue("GCC/MakePath", ui->lineEdit_getMAKEexefile->text());
    mySettings.setValue("GCC/StripPath", ui->lineEdit_getSTRIPexefile->text());
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
    mySettings.setValue("VBCC/VcDefaultOpts13", ui->lineEdit_VCdefaultOptsOS13->text());
    mySettings.setValue("VBCC/VcDefaultOpts30", ui->lineEdit_VCdefaultOptsOS30->text());
    mySettings.setValue("VBCC/Vc13LinkerOpts", ui->lineEdit_VC13LinkerOpts->text());
    mySettings.setValue("VBCC/Vc30LinkerOpts", ui->lineEdit_VCdefaultLinkerOpts->text());
    mySettings.setValue("SASC/DefaultOpts", ui->lineEdit_SASCdefaultOpts->text());
    mySettings.setValue("VBCC/VcDefaultTarget", ui->comboBoxVbccDefaultTargetOS->currentIndex());
    mySettings.setValue("VBCC/ShowVbccDefaultOpts", ui->checkBox_ShowVbccOpts->isChecked());

    // TAB: Emulator
     mySettings.setValue("UAE/UaePath", ui->lineEdit_getEmulatorExefile->text());
     mySettings.setValue("UAE/Os13ConfigPath", ui->lineEdit_getOS13Configfile->text());
     mySettings.setValue("UAE/Os30ConfigPath", ui->lineEdit_getOS3Configfile->text());
     mySettings.setValue("UAE/DefaultConfig", ui->comboBox_defaultEmulator->currentIndex());

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
}

void PrefsDialog::load_mySettings()
{
    // TAB: Project
    ui->lineEdit_author->setText(mySettings.value("Project/Author").toString());
    ui->lineEdit_email->setText(mySettings.value("Project/Email").toString());
    ui->lineEdit_website->setText(mySettings.value("Project/Website").toString());
    ui->lineEdit_projectsRootDir->setText(mySettings.value("Project/ProjectRootDir").toString());
    ui->lineEdit_getDefaultIcon->setText(mySettings.value("Project/DefaultIcon").toString());

    // TAB: GCC
    ui->lineEdit_getGCCexefile->setText(mySettings.value("GCC/GccPath").toString());
    ui->lineEdit_getGPPexefile->setText(mySettings.value("GCC/GppPath").toString());
    ui->lineEdit_getMAKEexefile->setText(mySettings.value("GCC/MakePath").toString());
    ui->lineEdit_getSTRIPexefile->setText(mySettings.value("GCC/StripPath").toString());
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
    ui->lineEdit_VCdefaultOptsOS13->setText(mySettings.value("VBCC/VcDefaultOpts13").toString());
    ui->lineEdit_VCdefaultOptsOS30->setText(mySettings.value("VBCC/VcDefaultOpts30").toString());
    ui->lineEdit_VC13LinkerOpts->setText(mySettings.value("VBCC/Vc13LinkerOpts").toString());
    ui->lineEdit_VCdefaultLinkerOpts->setText(mySettings.value("VBCC/Vc30LinkerOpts").toString());
    ui->lineEdit_SASCdefaultOpts->setText(mySettings.value("SASC/DefaultOpts").toString());
    ui->checkBox_ShowVbccOpts->setChecked(mySettings.value("VBCC/ShowVbccDefaultOpts").toBool());
    ui->comboBoxVbccDefaultTargetOS->setCurrentIndex(mySettings.value("VBCC/VcDefaultTarget").toInt());

    // TAB: Emulator
    ui->lineEdit_getEmulatorExefile->setText(mySettings.value("UAE/UaePath").toString());
    ui->lineEdit_getOS13Configfile->setText(mySettings.value("UAE/Os13ConfigPath").toString());
    ui->lineEdit_getOS3Configfile->setText(mySettings.value("UAE/Os30ConfigPath").toString());
    ui->comboBox_defaultEmulator->setCurrentIndex(mySettings.value("UAE/DefaultConfig").toInt());

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

void PrefsDialog::on_btn_getDefaultIcon_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Path Amiga icon file"), QDir::homePath(),
            tr("Amiga Icon Files (*.info);;All Files (*)"));
    ui->lineEdit_getDefaultIcon->setText(fileName);
}

