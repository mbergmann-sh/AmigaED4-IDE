#ifndef PREFSDIALOG_H
#define PREFSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QStandardPaths>
#include <QStringList>
#include <QDir>
#include <QStyle>
#include <QStyleFactory>
#include <QComboBox>
#include <QSettings>
#include <QCoreApplication>
#include <QProcess>
#include <QMessageBox>
#include <QRegularExpression>

class MainWindow;

namespace Ui {
class PrefsDialog;
}

class PrefsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrefsDialog(QWidget *parent = 0, int tabindex = 0);
    //explicit PrefsDialog(int tabindex);
    ~PrefsDialog();

    QStringList p_Compilers = {"VBCC (C mode only)", "GNU gcc (C mode)", "GNU g++ (C++ mode)"};    // used for building combobox entries
    QStringList p_style_items;

    QSettings mySettings;

    // Prefs > vamos (rev.159): platform-dependent defaults, also used by
    // MainWindow::readSettings() - see prefsdialog.cpp
    static QString vamosDefaultCommand();
    static bool vamosDefaultUseWSL();

    // rev.159: start programs in the emulator - see prefsdialog.cpp
    struct EmuMount { QString volume; QString hostPath; int bootPri = 0; };
    static QList<EmuMount> emulatorMounts(const QString &configPath);   // folder drives of a WinUAE/FS-UAE config
    static QString amigaPathForHostPath(const QString &hostPath, const QString &projectsRootHost,
                                        const QString &projectsRootAmiga, const QString &emuConfig,
                                        const QString &workbenchHost, const QString &workHost);
    static bool looksLikeAmigaPath(const QString &path);                // "Work:x" yes, "D:/x", "/x" no
    static QString bootUserStartup(const QString &emuConfig, const QString &workbenchHost);   // host path of S:User-Startup
    static QString startScriptBlock(const QString &amigaJobDir);
    static bool hasStartScriptBlock(const QString &userStartupPath, const QString &amigaJobDir);
    static bool installStartScript(const QString &userStartupPath, const QString &amigaJobDir, QString *message);
    static bool writeJobRunner(const QString &hostJobDir, const QString &amigaJobDir);   // <job dir>/autorun, see prefsdialog.cpp

public slots:
    void save_mySettings();
    void load_mySettings();
    void simpleStatusbar();

private slots:
    void on_btn_SavePrefs_clicked();
    void on_btn_getProjectRootDir_clicked();
    void on_btn_getGCCexefile_clicked();
    void on_btn_getGPPexefile_clicked();
    void on_btn_getMAKEexefile_clicked();
    void on_btn_getSTRIPexefile_clicked();
    void on_btn_getASexefile_clicked();
    void on_btn_getLDexefile_clicked();
    void on_btn_getASIncludeDir_clicked();
    void on_btn_getVCexefile_clicked();
    void on_btn_getVASMexefile_clicked();
    void on_btn_getVCconfigDir_clicked();
    void on_btn_getVASMIncludeDir_clicked();
    void on_btn_getEmulatorExefile_clicked();
    void on_btn_getOS13Configfile_clicked();
    void on_btn_getOS3Configfile_clicked();
    void on_btn_editOS13Configfile_clicked();  // Prefs > Emulator: "Edit" next to OS 1.3 config - opens the file in the system's own text editor
    void on_btn_editOS3Configfile_clicked();   // Prefs > Emulator: "Edit" next to OS 3.x config - opens the file in the system's own text editor
    void on_btn_getAutodocsDir_clicked();      // Prefs > Emulator: "AutoDocs folder:" - selects the NDK AutoDocs folder used by Build > AutoDoc Reader...

    // Prefs > Tools: file-selection buttons for the 4 external GUI-builder
    // tool slots - each opens a file requester and writes the chosen path
    // into that slot's own "Path" field. See MainWindow::rebuildToolsMenu()
    // for what consumes these once saved.
    void on_btn_getTool1Path_clicked();   // MUI GUI Designer
    void on_btn_getTool2Path_clicked();   // GadTools GUI Designer
    void on_btn_getTool3Path_clicked();   // ReAction GUI Designer
    void on_btn_getTool4Path_clicked();   // User Tool

    void on_btn_setupStartScript_clicked();
    void on_btn_getProjectsRootAmiga_clicked();    // Prefs > Project: Amiga path of the projects root via folder dialog (rev.159)        // Prefs > Emulator: AmigaED block in S:User-Startup (rev.159)
    void on_btn_getFlexCatPath_clicked();          // Prefs > Tools > "Multilingual Programs": FlexCat executable (rev.159)
    void on_btn_getVamosWorkbenchDir_clicked();   // Prefs > vamos: host folder of the "Workbench" partition (rev.159)
    void on_btn_getVamosWorkDir_clicked();        // Prefs > vamos: host folder of the "Work" partition (rev.159)

    void on_btn_CancelSave_clicked();

    void on_checkBoxSimpleStatusbar_clicked();

    void on_checkBoxNoLCD_clicked();

    void on_checkBoxNoCompileButton_clicked();

private:
    Ui::PrefsDialog *ui;

    // Shared by on_btn_editOS13Configfile_clicked()/on_btn_editOS3Configfile_clicked() -
    // launches the platform's own text editor on filePath. See the .cpp
    // for why a concrete editor is launched per platform instead of just
    // going through QDesktopServices::openUrl()'s file-association lookup.
    void openFileInSystemEditor(const QString &filePath);
};

#endif // PREFSDIALOG_H
