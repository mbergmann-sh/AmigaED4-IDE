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

    QStringList myPrefs;
    QStringList p_Compilers = {"VBCC (C mode only)", "GNU gcc (C mode)", "GNU g++ (C++ mode)"};    // used for building combobox entries
    QStringList p_style_items;

    QString line;
    QStringList fields;

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
    void on_btn_getVCexefile_clicked();
    void on_btn_getVASMexefile_clicked();
    void on_btn_getVCconfigDir_clicked();
    void on_btn_getEmulatorExefile_clicked();
    void on_btn_getOS13Configfile_clicked();
    void on_btn_getOS3Configfile_clicked();
    void on_btn_CancelSave_clicked();

    void on_checkBoxSimpleStatusbar_clicked();

    void on_checkBoxNoLCD_clicked();

    void on_checkBoxNoCompileButton_clicked();

    void on_btn_getDefaultIcon_clicked();

private:
    Ui::PrefsDialog *ui;
    bool p_prefs_changed;
};

#endif // PREFSDIALOG_H
