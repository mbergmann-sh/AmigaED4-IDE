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
