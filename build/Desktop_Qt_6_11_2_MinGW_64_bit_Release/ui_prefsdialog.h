/********************************************************************************
** Form generated from reading UI file 'prefsdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFSDIALOG_H
#define UI_PREFSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrefsDialog
{
public:
    QGridLayout *gridLayout_10;
    QTabWidget *tabWidget;
    QWidget *tabProject;
    QGridLayout *gridLayout_23;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit_author;
    QLabel *label_2;
    QLineEdit *lineEdit_email;
    QLabel *label_3;
    QLineEdit *lineEdit_website;
    QLabel *label_4;
    QLineEdit *lineEdit_projectsRootDir;
    QPushButton *btn_getProjectRootDir;
    QCheckBox *checkBox_saveProjectFilesAutomatically;
    QLabel *label_defaultTargetOS;
    QComboBox *comboBox_defaultTargetOS;
    QSpacerItem *verticalSpacer_3;
    QWidget *tabGCC;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_6;
    QLineEdit *lineEdit_getGCCexefile;
    QPushButton *btn_getGCCexefile;
    QLabel *label_5;
    QGridLayout *gridLayout_7;
    QPushButton *btn_getGPPexefile;
    QLineEdit *lineEdit_getGPPexefile;
    QLabel *label_6;
    QGridLayout *gridLayout_8;
    QLineEdit *lineEdit_getMAKEexefile;
    QPushButton *btn_getMAKEexefile;
    QLabel *label_7;
    QGridLayout *gridLayout_28;
    QLineEdit *lineEdit_getSTRIPexefile;
    QPushButton *btn_getSTRIPexefile;
    QLabel *label_27;
    QGridLayout *gridLayout_33;
    QLineEdit *lineEdit_getASexefile;
    QPushButton *btn_getASexefile;
    QLabel *label_29;
    QGridLayout *gridLayout_34;
    QLineEdit *lineEdit_getLDexefile;
    QPushButton *btn_getLDexefile;
    QLabel *label_30;
    QGridLayout *gridLayout_3;
    QLabel *label_8;
    QLineEdit *lineEdit_GCC13CompilerOpts;
    QLabel *label_37;
    QLineEdit *lineEdit_GCC30CompilerOpts;
    QLabel *label_38;
    QLineEdit *lineEdit_GCC13LinkerOpts;
    QLabel *label_39;
    QLineEdit *lineEdit_GCC30LinkerOpts;
    QLabel *label_9;
    QLineEdit *lineEdit_GPP13CompilerOpts;
    QLabel *label_40;
    QLineEdit *lineEdit_GPP30CompilerOpts;
    QLabel *label_41;
    QLineEdit *lineEdit_GPP13LinkerOpts;
    QLabel *label_42;
    QLineEdit *lineEdit_GPP30LinkerOpts;
    QGridLayout *gridLayout_ASInclude;
    QLabel *label_ASIncludeDir;
    QLineEdit *lineEdit_getASIncludeDir;
    QPushButton *btn_getASIncludeDir;
    QCheckBox *checkBox_ShowGccOpts;
    QWidget *tabVBCC;
    QGridLayout *gridLayout_13;
    QGridLayout *gridLayout_9;
    QGridLayout *gridLayout_18;
    QPushButton *btn_getVCexefile;
    QLabel *label_19;
    QLineEdit *lineEdit_getVCexefile;
    QGridLayout *gridLayout_16;
    QLabel *label_17;
    QPushButton *btn_getVASMexefile;
    QLineEdit *lineEdit_getVASMexefile;
    QGridLayout *gridLayout_17;
    QLabel *label_18;
    QPushButton *btn_getVCconfigDir;
    QLineEdit *lineEdit_getVCconfigDir;
    QGridLayout *gridLayout_19;
    QLineEdit *lineEdit_VCdefaultOptsOS13;
    QLabel *label_20;
    QGridLayout *gridLayout_20;
    QLineEdit *lineEdit_VCdefaultOptsOS30;
    QLabel *label_32;
    QGridLayout *gridLayout_30;
    QLineEdit *lineEdit_VC13LinkerOpts;
    QLabel *label_43;
    QGridLayout *gridLayout_29;
    QLabel *label_36;
    QLineEdit *lineEdit_VCdefaultLinkerOpts;
    QGridLayout *gridLayout_VasmInclude;
    QLabel *label_VasmIncludeDir;
    QLineEdit *lineEdit_getVASMIncludeDir;
    QPushButton *btn_getVASMIncludeDir;
    QCheckBox *checkBox_ShowVbccOpts;
    QSpacerItem *verticalSpacer;
    QWidget *tabSASC;
    QGridLayout *gridLayout_31;
    QGridLayout *gridLayout_32;
    QLabel *label_44;
    QLineEdit *lineEdit_SASCdefaultOpts;
    QSpacerItem *verticalSpacer_2;
    QWidget *tabEmulator;
    QGridLayout *gridLayout_14;
    QGridLayout *gridLayout_12;
    QLabel *label_22;
    QLineEdit *lineEdit_getEmulatorExefile;
    QPushButton *btn_getEmulatorExefile;
    QLabel *label_21;
    QLineEdit *lineEdit_getOS13Configfile;
    QPushButton *btn_getOS13Configfile;
    QPushButton *btn_editOS13Configfile;
    QLabel *label_24;
    QLineEdit *lineEdit_getOS3Configfile;
    QPushButton *btn_getOS3Configfile;
    QPushButton *btn_editOS3Configfile;
    QLabel *label_25;
    QLineEdit *lineEdit_getAutodocsDir;
    QPushButton *btn_getAutodocsDir;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_4;
    QWidget *tabTools;
    QVBoxLayout *verticalLayout_Tools;
    QGroupBox *groupBox_ToolMui;
    QGridLayout *gridLayout_ToolMui;
    QLabel *label_ToolMuiPath;
    QLineEdit *lineEdit_Tool1Path;
    QPushButton *btn_getTool1Path;
    QLabel *label_ToolMuiParams;
    QLineEdit *lineEdit_Tool1Params;
    QLabel *label_ToolMuiName;
    QLineEdit *lineEdit_Tool1Name;
    QGroupBox *groupBox_ToolGadtools;
    QGridLayout *gridLayout_ToolGadtools;
    QLabel *label_ToolGadtoolsPath;
    QLineEdit *lineEdit_Tool2Path;
    QPushButton *btn_getTool2Path;
    QLabel *label_ToolGadtoolsParams;
    QLineEdit *lineEdit_Tool2Params;
    QLabel *label_ToolGadtoolsName;
    QLineEdit *lineEdit_Tool2Name;
    QGroupBox *groupBox_ToolReaction;
    QGridLayout *gridLayout_ToolReaction;
    QLabel *label_ToolReactionPath;
    QLineEdit *lineEdit_Tool3Path;
    QPushButton *btn_getTool3Path;
    QLabel *label_ToolReactionParams;
    QLineEdit *lineEdit_Tool3Params;
    QLabel *label_ToolReactionName;
    QLineEdit *lineEdit_Tool3Name;
    QGroupBox *groupBox_ToolUser;
    QGridLayout *gridLayout_ToolUser;
    QLabel *label_ToolUserPath;
    QLineEdit *lineEdit_Tool4Path;
    QPushButton *btn_getTool4Path;
    QLabel *label_ToolUserParams;
    QLineEdit *lineEdit_Tool4Params;
    QLabel *label_ToolUserName;
    QLineEdit *lineEdit_Tool4Name;
    QSpacerItem *verticalSpacer_Tools;
    QWidget *tabMisc;
    QGridLayout *gridLayout_22;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_21;
    QGridLayout *gridLayout_2;
    QLabel *label_28;
    QComboBox *comboBoxDefaultStyle;
    QCheckBox *checkBoxNoLCD;
    QCheckBox *checkBoxIndentationLines;
    QCheckBox *checkBoxNoCompileButton;
    QCheckBox *checkBoxDebugOutput;
    QCheckBox *checkBoxSimpleStatusbar;
    QLabel *label_33;
    QComboBox *comboBoxDefaultGuiLanguage;
    QCheckBox *checkBoxHighlightBraceBlock;
    QCheckBox *checkBoxNoSplashScreen;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_15;
    QGridLayout *gridLayout_11;
    QLabel *label_31;
    QComboBox *comboBoxDefaultCompiler;
    QCheckBox *checkBoxCreateIcon;
    QCheckBox *checkBoxOpenOnFail;
    QCheckBox *checkBoxWarnRequesters;
    QSpacerItem *verticalSpacer_6;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_CancelSave;
    QPushButton *btn_SavePrefs;

    void setupUi(QDialog *PrefsDialog)
    {
        if (PrefsDialog->objectName().isEmpty())
            PrefsDialog->setObjectName("PrefsDialog");
        PrefsDialog->resize(590, 656);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/prefs.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        PrefsDialog->setWindowIcon(icon);
        PrefsDialog->setSizeGripEnabled(false);
        PrefsDialog->setModal(true);
        gridLayout_10 = new QGridLayout(PrefsDialog);
        gridLayout_10->setObjectName("gridLayout_10");
        tabWidget = new QTabWidget(PrefsDialog);
        tabWidget->setObjectName("tabWidget");
        tabProject = new QWidget();
        tabProject->setObjectName("tabProject");
        gridLayout_23 = new QGridLayout(tabProject);
        gridLayout_23->setObjectName("gridLayout_23");
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName("gridLayout_5");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(tabProject);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit_author = new QLineEdit(tabProject);
        lineEdit_author->setObjectName("lineEdit_author");
        lineEdit_author->setMinimumSize(QSize(142, 0));

        gridLayout->addWidget(lineEdit_author, 0, 1, 1, 2);

        label_2 = new QLabel(tabProject);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEdit_email = new QLineEdit(tabProject);
        lineEdit_email->setObjectName("lineEdit_email");
        lineEdit_email->setMinimumSize(QSize(142, 0));

        gridLayout->addWidget(lineEdit_email, 1, 1, 1, 2);

        label_3 = new QLabel(tabProject);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        lineEdit_website = new QLineEdit(tabProject);
        lineEdit_website->setObjectName("lineEdit_website");
        lineEdit_website->setMinimumSize(QSize(142, 0));

        gridLayout->addWidget(lineEdit_website, 2, 1, 1, 2);

        label_4 = new QLabel(tabProject);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 3, 0, 1, 2);

        lineEdit_projectsRootDir = new QLineEdit(tabProject);
        lineEdit_projectsRootDir->setObjectName("lineEdit_projectsRootDir");

        gridLayout->addWidget(lineEdit_projectsRootDir, 3, 2, 1, 1);

        btn_getProjectRootDir = new QPushButton(tabProject);
        btn_getProjectRootDir->setObjectName("btn_getProjectRootDir");
        btn_getProjectRootDir->setMaximumSize(QSize(31, 25));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/open.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_getProjectRootDir->setIcon(icon1);

        gridLayout->addWidget(btn_getProjectRootDir, 3, 3, 1, 1);


        gridLayout_5->addLayout(gridLayout, 0, 0, 1, 2);

        checkBox_saveProjectFilesAutomatically = new QCheckBox(tabProject);
        checkBox_saveProjectFilesAutomatically->setObjectName("checkBox_saveProjectFilesAutomatically");

        gridLayout_5->addWidget(checkBox_saveProjectFilesAutomatically, 1, 0, 1, 2);

        label_defaultTargetOS = new QLabel(tabProject);
        label_defaultTargetOS->setObjectName("label_defaultTargetOS");

        gridLayout_5->addWidget(label_defaultTargetOS, 2, 0, 1, 1);

        comboBox_defaultTargetOS = new QComboBox(tabProject);
        comboBox_defaultTargetOS->addItem(QString());
        comboBox_defaultTargetOS->addItem(QString());
        comboBox_defaultTargetOS->setObjectName("comboBox_defaultTargetOS");

        gridLayout_5->addWidget(comboBox_defaultTargetOS, 2, 1, 1, 1);


        gridLayout_23->addLayout(gridLayout_5, 0, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 294, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_23->addItem(verticalSpacer_3, 1, 0, 1, 1);

        tabWidget->addTab(tabProject, QString());
        tabGCC = new QWidget();
        tabGCC->setObjectName("tabGCC");
        gridLayout_4 = new QGridLayout(tabGCC);
        gridLayout_4->setObjectName("gridLayout_4");
        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName("gridLayout_6");
        lineEdit_getGCCexefile = new QLineEdit(tabGCC);
        lineEdit_getGCCexefile->setObjectName("lineEdit_getGCCexefile");

        gridLayout_6->addWidget(lineEdit_getGCCexefile, 0, 1, 1, 1);

        btn_getGCCexefile = new QPushButton(tabGCC);
        btn_getGCCexefile->setObjectName("btn_getGCCexefile");
        btn_getGCCexefile->setMaximumSize(QSize(31, 25));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/new.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_getGCCexefile->setIcon(icon2);

        gridLayout_6->addWidget(btn_getGCCexefile, 0, 2, 1, 1);

        label_5 = new QLabel(tabGCC);
        label_5->setObjectName("label_5");

        gridLayout_6->addWidget(label_5, 0, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_6, 0, 0, 1, 1);

        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName("gridLayout_7");
        btn_getGPPexefile = new QPushButton(tabGCC);
        btn_getGPPexefile->setObjectName("btn_getGPPexefile");
        btn_getGPPexefile->setMaximumSize(QSize(31, 25));
        btn_getGPPexefile->setIcon(icon2);

        gridLayout_7->addWidget(btn_getGPPexefile, 0, 2, 1, 1);

        lineEdit_getGPPexefile = new QLineEdit(tabGCC);
        lineEdit_getGPPexefile->setObjectName("lineEdit_getGPPexefile");

        gridLayout_7->addWidget(lineEdit_getGPPexefile, 0, 1, 1, 1);

        label_6 = new QLabel(tabGCC);
        label_6->setObjectName("label_6");

        gridLayout_7->addWidget(label_6, 0, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_7, 1, 0, 1, 1);

        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName("gridLayout_8");
        lineEdit_getMAKEexefile = new QLineEdit(tabGCC);
        lineEdit_getMAKEexefile->setObjectName("lineEdit_getMAKEexefile");

        gridLayout_8->addWidget(lineEdit_getMAKEexefile, 0, 1, 1, 1);

        btn_getMAKEexefile = new QPushButton(tabGCC);
        btn_getMAKEexefile->setObjectName("btn_getMAKEexefile");
        btn_getMAKEexefile->setMaximumSize(QSize(31, 25));
        btn_getMAKEexefile->setIcon(icon2);

        gridLayout_8->addWidget(btn_getMAKEexefile, 0, 2, 1, 1);

        label_7 = new QLabel(tabGCC);
        label_7->setObjectName("label_7");

        gridLayout_8->addWidget(label_7, 0, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_8, 2, 0, 1, 1);

        gridLayout_28 = new QGridLayout();
        gridLayout_28->setObjectName("gridLayout_28");
        lineEdit_getSTRIPexefile = new QLineEdit(tabGCC);
        lineEdit_getSTRIPexefile->setObjectName("lineEdit_getSTRIPexefile");

        gridLayout_28->addWidget(lineEdit_getSTRIPexefile, 0, 1, 1, 1);

        btn_getSTRIPexefile = new QPushButton(tabGCC);
        btn_getSTRIPexefile->setObjectName("btn_getSTRIPexefile");
        btn_getSTRIPexefile->setMaximumSize(QSize(31, 25));
        btn_getSTRIPexefile->setIcon(icon2);

        gridLayout_28->addWidget(btn_getSTRIPexefile, 0, 2, 1, 1);

        label_27 = new QLabel(tabGCC);
        label_27->setObjectName("label_27");

        gridLayout_28->addWidget(label_27, 0, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_28, 3, 0, 1, 1);

        gridLayout_33 = new QGridLayout();
        gridLayout_33->setObjectName("gridLayout_33");
        lineEdit_getASexefile = new QLineEdit(tabGCC);
        lineEdit_getASexefile->setObjectName("lineEdit_getASexefile");

        gridLayout_33->addWidget(lineEdit_getASexefile, 0, 1, 1, 1);

        btn_getASexefile = new QPushButton(tabGCC);
        btn_getASexefile->setObjectName("btn_getASexefile");
        btn_getASexefile->setMaximumSize(QSize(31, 25));
        btn_getASexefile->setIcon(icon2);

        gridLayout_33->addWidget(btn_getASexefile, 0, 2, 1, 1);

        label_29 = new QLabel(tabGCC);
        label_29->setObjectName("label_29");

        gridLayout_33->addWidget(label_29, 0, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_33, 4, 0, 1, 1);

        gridLayout_34 = new QGridLayout();
        gridLayout_34->setObjectName("gridLayout_34");
        lineEdit_getLDexefile = new QLineEdit(tabGCC);
        lineEdit_getLDexefile->setObjectName("lineEdit_getLDexefile");

        gridLayout_34->addWidget(lineEdit_getLDexefile, 0, 1, 1, 1);

        btn_getLDexefile = new QPushButton(tabGCC);
        btn_getLDexefile->setObjectName("btn_getLDexefile");
        btn_getLDexefile->setMaximumSize(QSize(31, 25));
        btn_getLDexefile->setIcon(icon2);

        gridLayout_34->addWidget(btn_getLDexefile, 0, 2, 1, 1);

        label_30 = new QLabel(tabGCC);
        label_30->setObjectName("label_30");

        gridLayout_34->addWidget(label_30, 0, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_34, 5, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName("gridLayout_3");
        label_8 = new QLabel(tabGCC);
        label_8->setObjectName("label_8");

        gridLayout_3->addWidget(label_8, 0, 0, 1, 1);

        lineEdit_GCC13CompilerOpts = new QLineEdit(tabGCC);
        lineEdit_GCC13CompilerOpts->setObjectName("lineEdit_GCC13CompilerOpts");

        gridLayout_3->addWidget(lineEdit_GCC13CompilerOpts, 0, 1, 1, 1);

        label_37 = new QLabel(tabGCC);
        label_37->setObjectName("label_37");

        gridLayout_3->addWidget(label_37, 1, 0, 1, 1);

        lineEdit_GCC30CompilerOpts = new QLineEdit(tabGCC);
        lineEdit_GCC30CompilerOpts->setObjectName("lineEdit_GCC30CompilerOpts");

        gridLayout_3->addWidget(lineEdit_GCC30CompilerOpts, 1, 1, 1, 1);

        label_38 = new QLabel(tabGCC);
        label_38->setObjectName("label_38");

        gridLayout_3->addWidget(label_38, 2, 0, 1, 1);

        lineEdit_GCC13LinkerOpts = new QLineEdit(tabGCC);
        lineEdit_GCC13LinkerOpts->setObjectName("lineEdit_GCC13LinkerOpts");

        gridLayout_3->addWidget(lineEdit_GCC13LinkerOpts, 2, 1, 1, 1);

        label_39 = new QLabel(tabGCC);
        label_39->setObjectName("label_39");

        gridLayout_3->addWidget(label_39, 3, 0, 1, 1);

        lineEdit_GCC30LinkerOpts = new QLineEdit(tabGCC);
        lineEdit_GCC30LinkerOpts->setObjectName("lineEdit_GCC30LinkerOpts");

        gridLayout_3->addWidget(lineEdit_GCC30LinkerOpts, 3, 1, 1, 1);

        label_9 = new QLabel(tabGCC);
        label_9->setObjectName("label_9");

        gridLayout_3->addWidget(label_9, 4, 0, 1, 1);

        lineEdit_GPP13CompilerOpts = new QLineEdit(tabGCC);
        lineEdit_GPP13CompilerOpts->setObjectName("lineEdit_GPP13CompilerOpts");

        gridLayout_3->addWidget(lineEdit_GPP13CompilerOpts, 4, 1, 1, 1);

        label_40 = new QLabel(tabGCC);
        label_40->setObjectName("label_40");

        gridLayout_3->addWidget(label_40, 5, 0, 1, 1);

        lineEdit_GPP30CompilerOpts = new QLineEdit(tabGCC);
        lineEdit_GPP30CompilerOpts->setObjectName("lineEdit_GPP30CompilerOpts");

        gridLayout_3->addWidget(lineEdit_GPP30CompilerOpts, 5, 1, 1, 1);

        label_41 = new QLabel(tabGCC);
        label_41->setObjectName("label_41");

        gridLayout_3->addWidget(label_41, 6, 0, 1, 1);

        lineEdit_GPP13LinkerOpts = new QLineEdit(tabGCC);
        lineEdit_GPP13LinkerOpts->setObjectName("lineEdit_GPP13LinkerOpts");

        gridLayout_3->addWidget(lineEdit_GPP13LinkerOpts, 6, 1, 1, 1);

        label_42 = new QLabel(tabGCC);
        label_42->setObjectName("label_42");

        gridLayout_3->addWidget(label_42, 7, 0, 1, 1);

        lineEdit_GPP30LinkerOpts = new QLineEdit(tabGCC);
        lineEdit_GPP30LinkerOpts->setObjectName("lineEdit_GPP30LinkerOpts");

        gridLayout_3->addWidget(lineEdit_GPP30LinkerOpts, 7, 1, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 6, 0, 1, 1);

        gridLayout_ASInclude = new QGridLayout();
        gridLayout_ASInclude->setObjectName("gridLayout_ASInclude");
        label_ASIncludeDir = new QLabel(tabGCC);
        label_ASIncludeDir->setObjectName("label_ASIncludeDir");

        gridLayout_ASInclude->addWidget(label_ASIncludeDir, 0, 0, 1, 1);

        lineEdit_getASIncludeDir = new QLineEdit(tabGCC);
        lineEdit_getASIncludeDir->setObjectName("lineEdit_getASIncludeDir");

        gridLayout_ASInclude->addWidget(lineEdit_getASIncludeDir, 0, 1, 1, 1);

        btn_getASIncludeDir = new QPushButton(tabGCC);
        btn_getASIncludeDir->setObjectName("btn_getASIncludeDir");
        btn_getASIncludeDir->setMaximumSize(QSize(31, 25));
        btn_getASIncludeDir->setIcon(icon1);

        gridLayout_ASInclude->addWidget(btn_getASIncludeDir, 0, 2, 1, 1);


        gridLayout_4->addLayout(gridLayout_ASInclude, 7, 0, 1, 1);

        checkBox_ShowGccOpts = new QCheckBox(tabGCC);
        checkBox_ShowGccOpts->setObjectName("checkBox_ShowGccOpts");
        checkBox_ShowGccOpts->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        checkBox_ShowGccOpts->setChecked(true);

        gridLayout_4->addWidget(checkBox_ShowGccOpts, 8, 0, 1, 1);

        tabWidget->addTab(tabGCC, QString());
        tabVBCC = new QWidget();
        tabVBCC->setObjectName("tabVBCC");
        gridLayout_13 = new QGridLayout(tabVBCC);
        gridLayout_13->setObjectName("gridLayout_13");
        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName("gridLayout_9");
        gridLayout_18 = new QGridLayout();
        gridLayout_18->setObjectName("gridLayout_18");
        btn_getVCexefile = new QPushButton(tabVBCC);
        btn_getVCexefile->setObjectName("btn_getVCexefile");
        btn_getVCexefile->setMaximumSize(QSize(31, 25));
        btn_getVCexefile->setIcon(icon2);

        gridLayout_18->addWidget(btn_getVCexefile, 0, 2, 1, 1);

        label_19 = new QLabel(tabVBCC);
        label_19->setObjectName("label_19");

        gridLayout_18->addWidget(label_19, 0, 0, 1, 1);

        lineEdit_getVCexefile = new QLineEdit(tabVBCC);
        lineEdit_getVCexefile->setObjectName("lineEdit_getVCexefile");

        gridLayout_18->addWidget(lineEdit_getVCexefile, 0, 1, 1, 1);


        gridLayout_9->addLayout(gridLayout_18, 0, 0, 1, 1);

        gridLayout_16 = new QGridLayout();
        gridLayout_16->setObjectName("gridLayout_16");
        label_17 = new QLabel(tabVBCC);
        label_17->setObjectName("label_17");

        gridLayout_16->addWidget(label_17, 0, 0, 1, 1);

        btn_getVASMexefile = new QPushButton(tabVBCC);
        btn_getVASMexefile->setObjectName("btn_getVASMexefile");
        btn_getVASMexefile->setMaximumSize(QSize(31, 25));
        btn_getVASMexefile->setIcon(icon2);

        gridLayout_16->addWidget(btn_getVASMexefile, 0, 2, 1, 1);

        lineEdit_getVASMexefile = new QLineEdit(tabVBCC);
        lineEdit_getVASMexefile->setObjectName("lineEdit_getVASMexefile");

        gridLayout_16->addWidget(lineEdit_getVASMexefile, 0, 1, 1, 1);


        gridLayout_9->addLayout(gridLayout_16, 1, 0, 1, 1);

        gridLayout_17 = new QGridLayout();
        gridLayout_17->setObjectName("gridLayout_17");
        label_18 = new QLabel(tabVBCC);
        label_18->setObjectName("label_18");

        gridLayout_17->addWidget(label_18, 0, 0, 1, 1);

        btn_getVCconfigDir = new QPushButton(tabVBCC);
        btn_getVCconfigDir->setObjectName("btn_getVCconfigDir");
        btn_getVCconfigDir->setMaximumSize(QSize(31, 25));
        btn_getVCconfigDir->setIcon(icon1);

        gridLayout_17->addWidget(btn_getVCconfigDir, 0, 2, 1, 1);

        lineEdit_getVCconfigDir = new QLineEdit(tabVBCC);
        lineEdit_getVCconfigDir->setObjectName("lineEdit_getVCconfigDir");

        gridLayout_17->addWidget(lineEdit_getVCconfigDir, 0, 1, 1, 1);


        gridLayout_9->addLayout(gridLayout_17, 2, 0, 1, 1);

        gridLayout_19 = new QGridLayout();
        gridLayout_19->setObjectName("gridLayout_19");
        lineEdit_VCdefaultOptsOS13 = new QLineEdit(tabVBCC);
        lineEdit_VCdefaultOptsOS13->setObjectName("lineEdit_VCdefaultOptsOS13");

        gridLayout_19->addWidget(lineEdit_VCdefaultOptsOS13, 0, 1, 1, 1);

        label_20 = new QLabel(tabVBCC);
        label_20->setObjectName("label_20");

        gridLayout_19->addWidget(label_20, 0, 0, 1, 1);


        gridLayout_9->addLayout(gridLayout_19, 3, 0, 1, 1);

        gridLayout_20 = new QGridLayout();
        gridLayout_20->setObjectName("gridLayout_20");
        lineEdit_VCdefaultOptsOS30 = new QLineEdit(tabVBCC);
        lineEdit_VCdefaultOptsOS30->setObjectName("lineEdit_VCdefaultOptsOS30");

        gridLayout_20->addWidget(lineEdit_VCdefaultOptsOS30, 0, 1, 1, 1);

        label_32 = new QLabel(tabVBCC);
        label_32->setObjectName("label_32");

        gridLayout_20->addWidget(label_32, 0, 0, 1, 1);


        gridLayout_9->addLayout(gridLayout_20, 4, 0, 1, 1);

        gridLayout_30 = new QGridLayout();
        gridLayout_30->setObjectName("gridLayout_30");
        lineEdit_VC13LinkerOpts = new QLineEdit(tabVBCC);
        lineEdit_VC13LinkerOpts->setObjectName("lineEdit_VC13LinkerOpts");

        gridLayout_30->addWidget(lineEdit_VC13LinkerOpts, 0, 1, 1, 1);

        label_43 = new QLabel(tabVBCC);
        label_43->setObjectName("label_43");

        gridLayout_30->addWidget(label_43, 0, 0, 1, 1);


        gridLayout_9->addLayout(gridLayout_30, 5, 0, 1, 1);

        gridLayout_29 = new QGridLayout();
        gridLayout_29->setObjectName("gridLayout_29");
        label_36 = new QLabel(tabVBCC);
        label_36->setObjectName("label_36");

        gridLayout_29->addWidget(label_36, 0, 0, 1, 1);

        lineEdit_VCdefaultLinkerOpts = new QLineEdit(tabVBCC);
        lineEdit_VCdefaultLinkerOpts->setObjectName("lineEdit_VCdefaultLinkerOpts");

        gridLayout_29->addWidget(lineEdit_VCdefaultLinkerOpts, 0, 1, 1, 1);


        gridLayout_9->addLayout(gridLayout_29, 6, 0, 1, 1);

        gridLayout_VasmInclude = new QGridLayout();
        gridLayout_VasmInclude->setObjectName("gridLayout_VasmInclude");
        label_VasmIncludeDir = new QLabel(tabVBCC);
        label_VasmIncludeDir->setObjectName("label_VasmIncludeDir");

        gridLayout_VasmInclude->addWidget(label_VasmIncludeDir, 0, 0, 1, 1);

        lineEdit_getVASMIncludeDir = new QLineEdit(tabVBCC);
        lineEdit_getVASMIncludeDir->setObjectName("lineEdit_getVASMIncludeDir");

        gridLayout_VasmInclude->addWidget(lineEdit_getVASMIncludeDir, 0, 1, 1, 1);

        btn_getVASMIncludeDir = new QPushButton(tabVBCC);
        btn_getVASMIncludeDir->setObjectName("btn_getVASMIncludeDir");
        btn_getVASMIncludeDir->setMaximumSize(QSize(31, 25));
        btn_getVASMIncludeDir->setIcon(icon1);

        gridLayout_VasmInclude->addWidget(btn_getVASMIncludeDir, 0, 2, 1, 1);


        gridLayout_9->addLayout(gridLayout_VasmInclude, 7, 0, 1, 1);


        gridLayout_13->addLayout(gridLayout_9, 0, 0, 1, 1);

        checkBox_ShowVbccOpts = new QCheckBox(tabVBCC);
        checkBox_ShowVbccOpts->setObjectName("checkBox_ShowVbccOpts");
        checkBox_ShowVbccOpts->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        checkBox_ShowVbccOpts->setChecked(true);

        gridLayout_13->addWidget(checkBox_ShowVbccOpts, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 32, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_13->addItem(verticalSpacer, 2, 0, 1, 1);

        tabWidget->addTab(tabVBCC, QString());
        tabSASC = new QWidget();
        tabSASC->setObjectName("tabSASC");
        gridLayout_31 = new QGridLayout(tabSASC);
        gridLayout_31->setObjectName("gridLayout_31");
        gridLayout_32 = new QGridLayout();
        gridLayout_32->setObjectName("gridLayout_32");
        label_44 = new QLabel(tabSASC);
        label_44->setObjectName("label_44");

        gridLayout_32->addWidget(label_44, 0, 0, 1, 1);

        lineEdit_SASCdefaultOpts = new QLineEdit(tabSASC);
        lineEdit_SASCdefaultOpts->setObjectName("lineEdit_SASCdefaultOpts");

        gridLayout_32->addWidget(lineEdit_SASCdefaultOpts, 0, 1, 1, 1);


        gridLayout_31->addLayout(gridLayout_32, 0, 0, 1, 2);

        verticalSpacer_2 = new QSpacerItem(20, 32, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_31->addItem(verticalSpacer_2, 1, 0, 1, 1);

        tabWidget->addTab(tabSASC, QString());
        tabEmulator = new QWidget();
        tabEmulator->setObjectName("tabEmulator");
        gridLayout_14 = new QGridLayout(tabEmulator);
        gridLayout_14->setObjectName("gridLayout_14");
        gridLayout_12 = new QGridLayout();
        gridLayout_12->setObjectName("gridLayout_12");
        label_22 = new QLabel(tabEmulator);
        label_22->setObjectName("label_22");

        gridLayout_12->addWidget(label_22, 0, 0, 1, 1);

        lineEdit_getEmulatorExefile = new QLineEdit(tabEmulator);
        lineEdit_getEmulatorExefile->setObjectName("lineEdit_getEmulatorExefile");

        gridLayout_12->addWidget(lineEdit_getEmulatorExefile, 0, 1, 1, 2);

        btn_getEmulatorExefile = new QPushButton(tabEmulator);
        btn_getEmulatorExefile->setObjectName("btn_getEmulatorExefile");
        btn_getEmulatorExefile->setMaximumSize(QSize(31, 25));
        btn_getEmulatorExefile->setIcon(icon2);

        gridLayout_12->addWidget(btn_getEmulatorExefile, 0, 3, 1, 1);

        label_21 = new QLabel(tabEmulator);
        label_21->setObjectName("label_21");

        gridLayout_12->addWidget(label_21, 1, 0, 1, 2);

        lineEdit_getOS13Configfile = new QLineEdit(tabEmulator);
        lineEdit_getOS13Configfile->setObjectName("lineEdit_getOS13Configfile");

        gridLayout_12->addWidget(lineEdit_getOS13Configfile, 1, 2, 1, 1);

        btn_getOS13Configfile = new QPushButton(tabEmulator);
        btn_getOS13Configfile->setObjectName("btn_getOS13Configfile");
        btn_getOS13Configfile->setMaximumSize(QSize(31, 25));
        btn_getOS13Configfile->setIcon(icon2);

        gridLayout_12->addWidget(btn_getOS13Configfile, 1, 3, 1, 1);

        btn_editOS13Configfile = new QPushButton(tabEmulator);
        btn_editOS13Configfile->setObjectName("btn_editOS13Configfile");
        btn_editOS13Configfile->setMaximumSize(QSize(16777215, 25));

        gridLayout_12->addWidget(btn_editOS13Configfile, 1, 4, 1, 1);

        label_24 = new QLabel(tabEmulator);
        label_24->setObjectName("label_24");

        gridLayout_12->addWidget(label_24, 2, 0, 1, 2);

        lineEdit_getOS3Configfile = new QLineEdit(tabEmulator);
        lineEdit_getOS3Configfile->setObjectName("lineEdit_getOS3Configfile");

        gridLayout_12->addWidget(lineEdit_getOS3Configfile, 2, 2, 1, 1);

        btn_getOS3Configfile = new QPushButton(tabEmulator);
        btn_getOS3Configfile->setObjectName("btn_getOS3Configfile");
        btn_getOS3Configfile->setMaximumSize(QSize(31, 25));
        btn_getOS3Configfile->setIcon(icon2);

        gridLayout_12->addWidget(btn_getOS3Configfile, 2, 3, 1, 1);

        btn_editOS3Configfile = new QPushButton(tabEmulator);
        btn_editOS3Configfile->setObjectName("btn_editOS3Configfile");
        btn_editOS3Configfile->setMaximumSize(QSize(16777215, 25));

        gridLayout_12->addWidget(btn_editOS3Configfile, 2, 4, 1, 1);

        label_25 = new QLabel(tabEmulator);
        label_25->setObjectName("label_25");

        gridLayout_12->addWidget(label_25, 3, 0, 1, 2);

        lineEdit_getAutodocsDir = new QLineEdit(tabEmulator);
        lineEdit_getAutodocsDir->setObjectName("lineEdit_getAutodocsDir");

        gridLayout_12->addWidget(lineEdit_getAutodocsDir, 3, 2, 1, 1);

        btn_getAutodocsDir = new QPushButton(tabEmulator);
        btn_getAutodocsDir->setObjectName("btn_getAutodocsDir");
        btn_getAutodocsDir->setMaximumSize(QSize(31, 25));
        btn_getAutodocsDir->setIcon(icon2);

        gridLayout_12->addWidget(btn_getAutodocsDir, 3, 3, 1, 1);


        gridLayout_14->addLayout(gridLayout_12, 0, 0, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(508, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_14->addItem(horizontalSpacer_2, 1, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 294, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_14->addItem(verticalSpacer_4, 2, 1, 1, 1);

        tabWidget->addTab(tabEmulator, QString());
        tabTools = new QWidget();
        tabTools->setObjectName("tabTools");
        verticalLayout_Tools = new QVBoxLayout(tabTools);
        verticalLayout_Tools->setObjectName("verticalLayout_Tools");
        groupBox_ToolMui = new QGroupBox(tabTools);
        groupBox_ToolMui->setObjectName("groupBox_ToolMui");
        gridLayout_ToolMui = new QGridLayout(groupBox_ToolMui);
        gridLayout_ToolMui->setObjectName("gridLayout_ToolMui");
        label_ToolMuiPath = new QLabel(groupBox_ToolMui);
        label_ToolMuiPath->setObjectName("label_ToolMuiPath");

        gridLayout_ToolMui->addWidget(label_ToolMuiPath, 0, 0, 1, 1);

        lineEdit_Tool1Path = new QLineEdit(groupBox_ToolMui);
        lineEdit_Tool1Path->setObjectName("lineEdit_Tool1Path");

        gridLayout_ToolMui->addWidget(lineEdit_Tool1Path, 0, 1, 1, 3);

        btn_getTool1Path = new QPushButton(groupBox_ToolMui);
        btn_getTool1Path->setObjectName("btn_getTool1Path");
        btn_getTool1Path->setMaximumSize(QSize(31, 25));
        btn_getTool1Path->setIcon(icon2);

        gridLayout_ToolMui->addWidget(btn_getTool1Path, 0, 4, 1, 1);

        label_ToolMuiParams = new QLabel(groupBox_ToolMui);
        label_ToolMuiParams->setObjectName("label_ToolMuiParams");

        gridLayout_ToolMui->addWidget(label_ToolMuiParams, 1, 0, 1, 1);

        lineEdit_Tool1Params = new QLineEdit(groupBox_ToolMui);
        lineEdit_Tool1Params->setObjectName("lineEdit_Tool1Params");

        gridLayout_ToolMui->addWidget(lineEdit_Tool1Params, 1, 1, 1, 1);

        label_ToolMuiName = new QLabel(groupBox_ToolMui);
        label_ToolMuiName->setObjectName("label_ToolMuiName");

        gridLayout_ToolMui->addWidget(label_ToolMuiName, 1, 2, 1, 1);

        lineEdit_Tool1Name = new QLineEdit(groupBox_ToolMui);
        lineEdit_Tool1Name->setObjectName("lineEdit_Tool1Name");

        gridLayout_ToolMui->addWidget(lineEdit_Tool1Name, 1, 3, 1, 2);


        verticalLayout_Tools->addWidget(groupBox_ToolMui);

        groupBox_ToolGadtools = new QGroupBox(tabTools);
        groupBox_ToolGadtools->setObjectName("groupBox_ToolGadtools");
        gridLayout_ToolGadtools = new QGridLayout(groupBox_ToolGadtools);
        gridLayout_ToolGadtools->setObjectName("gridLayout_ToolGadtools");
        label_ToolGadtoolsPath = new QLabel(groupBox_ToolGadtools);
        label_ToolGadtoolsPath->setObjectName("label_ToolGadtoolsPath");

        gridLayout_ToolGadtools->addWidget(label_ToolGadtoolsPath, 0, 0, 1, 1);

        lineEdit_Tool2Path = new QLineEdit(groupBox_ToolGadtools);
        lineEdit_Tool2Path->setObjectName("lineEdit_Tool2Path");

        gridLayout_ToolGadtools->addWidget(lineEdit_Tool2Path, 0, 1, 1, 3);

        btn_getTool2Path = new QPushButton(groupBox_ToolGadtools);
        btn_getTool2Path->setObjectName("btn_getTool2Path");
        btn_getTool2Path->setMaximumSize(QSize(31, 25));
        btn_getTool2Path->setIcon(icon2);

        gridLayout_ToolGadtools->addWidget(btn_getTool2Path, 0, 4, 1, 1);

        label_ToolGadtoolsParams = new QLabel(groupBox_ToolGadtools);
        label_ToolGadtoolsParams->setObjectName("label_ToolGadtoolsParams");

        gridLayout_ToolGadtools->addWidget(label_ToolGadtoolsParams, 1, 0, 1, 1);

        lineEdit_Tool2Params = new QLineEdit(groupBox_ToolGadtools);
        lineEdit_Tool2Params->setObjectName("lineEdit_Tool2Params");

        gridLayout_ToolGadtools->addWidget(lineEdit_Tool2Params, 1, 1, 1, 1);

        label_ToolGadtoolsName = new QLabel(groupBox_ToolGadtools);
        label_ToolGadtoolsName->setObjectName("label_ToolGadtoolsName");

        gridLayout_ToolGadtools->addWidget(label_ToolGadtoolsName, 1, 2, 1, 1);

        lineEdit_Tool2Name = new QLineEdit(groupBox_ToolGadtools);
        lineEdit_Tool2Name->setObjectName("lineEdit_Tool2Name");

        gridLayout_ToolGadtools->addWidget(lineEdit_Tool2Name, 1, 3, 1, 2);


        verticalLayout_Tools->addWidget(groupBox_ToolGadtools);

        groupBox_ToolReaction = new QGroupBox(tabTools);
        groupBox_ToolReaction->setObjectName("groupBox_ToolReaction");
        gridLayout_ToolReaction = new QGridLayout(groupBox_ToolReaction);
        gridLayout_ToolReaction->setObjectName("gridLayout_ToolReaction");
        label_ToolReactionPath = new QLabel(groupBox_ToolReaction);
        label_ToolReactionPath->setObjectName("label_ToolReactionPath");

        gridLayout_ToolReaction->addWidget(label_ToolReactionPath, 0, 0, 1, 1);

        lineEdit_Tool3Path = new QLineEdit(groupBox_ToolReaction);
        lineEdit_Tool3Path->setObjectName("lineEdit_Tool3Path");

        gridLayout_ToolReaction->addWidget(lineEdit_Tool3Path, 0, 1, 1, 3);

        btn_getTool3Path = new QPushButton(groupBox_ToolReaction);
        btn_getTool3Path->setObjectName("btn_getTool3Path");
        btn_getTool3Path->setMaximumSize(QSize(31, 25));
        btn_getTool3Path->setIcon(icon2);

        gridLayout_ToolReaction->addWidget(btn_getTool3Path, 0, 4, 1, 1);

        label_ToolReactionParams = new QLabel(groupBox_ToolReaction);
        label_ToolReactionParams->setObjectName("label_ToolReactionParams");

        gridLayout_ToolReaction->addWidget(label_ToolReactionParams, 1, 0, 1, 1);

        lineEdit_Tool3Params = new QLineEdit(groupBox_ToolReaction);
        lineEdit_Tool3Params->setObjectName("lineEdit_Tool3Params");

        gridLayout_ToolReaction->addWidget(lineEdit_Tool3Params, 1, 1, 1, 1);

        label_ToolReactionName = new QLabel(groupBox_ToolReaction);
        label_ToolReactionName->setObjectName("label_ToolReactionName");

        gridLayout_ToolReaction->addWidget(label_ToolReactionName, 1, 2, 1, 1);

        lineEdit_Tool3Name = new QLineEdit(groupBox_ToolReaction);
        lineEdit_Tool3Name->setObjectName("lineEdit_Tool3Name");

        gridLayout_ToolReaction->addWidget(lineEdit_Tool3Name, 1, 3, 1, 2);


        verticalLayout_Tools->addWidget(groupBox_ToolReaction);

        groupBox_ToolUser = new QGroupBox(tabTools);
        groupBox_ToolUser->setObjectName("groupBox_ToolUser");
        gridLayout_ToolUser = new QGridLayout(groupBox_ToolUser);
        gridLayout_ToolUser->setObjectName("gridLayout_ToolUser");
        label_ToolUserPath = new QLabel(groupBox_ToolUser);
        label_ToolUserPath->setObjectName("label_ToolUserPath");

        gridLayout_ToolUser->addWidget(label_ToolUserPath, 0, 0, 1, 1);

        lineEdit_Tool4Path = new QLineEdit(groupBox_ToolUser);
        lineEdit_Tool4Path->setObjectName("lineEdit_Tool4Path");

        gridLayout_ToolUser->addWidget(lineEdit_Tool4Path, 0, 1, 1, 3);

        btn_getTool4Path = new QPushButton(groupBox_ToolUser);
        btn_getTool4Path->setObjectName("btn_getTool4Path");
        btn_getTool4Path->setMaximumSize(QSize(31, 25));
        btn_getTool4Path->setIcon(icon2);

        gridLayout_ToolUser->addWidget(btn_getTool4Path, 0, 4, 1, 1);

        label_ToolUserParams = new QLabel(groupBox_ToolUser);
        label_ToolUserParams->setObjectName("label_ToolUserParams");

        gridLayout_ToolUser->addWidget(label_ToolUserParams, 1, 0, 1, 1);

        lineEdit_Tool4Params = new QLineEdit(groupBox_ToolUser);
        lineEdit_Tool4Params->setObjectName("lineEdit_Tool4Params");

        gridLayout_ToolUser->addWidget(lineEdit_Tool4Params, 1, 1, 1, 1);

        label_ToolUserName = new QLabel(groupBox_ToolUser);
        label_ToolUserName->setObjectName("label_ToolUserName");

        gridLayout_ToolUser->addWidget(label_ToolUserName, 1, 2, 1, 1);

        lineEdit_Tool4Name = new QLineEdit(groupBox_ToolUser);
        lineEdit_Tool4Name->setObjectName("lineEdit_Tool4Name");

        gridLayout_ToolUser->addWidget(lineEdit_Tool4Name, 1, 3, 1, 2);


        verticalLayout_Tools->addWidget(groupBox_ToolUser);

        verticalSpacer_Tools = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_Tools->addItem(verticalSpacer_Tools);

        tabWidget->addTab(tabTools, QString());
        tabMisc = new QWidget();
        tabMisc->setObjectName("tabMisc");
        gridLayout_22 = new QGridLayout(tabMisc);
        gridLayout_22->setObjectName("gridLayout_22");
        groupBox = new QGroupBox(tabMisc);
        groupBox->setObjectName("groupBox");
        gridLayout_21 = new QGridLayout(groupBox);
        gridLayout_21->setObjectName("gridLayout_21");
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        label_28 = new QLabel(groupBox);
        label_28->setObjectName("label_28");

        gridLayout_2->addWidget(label_28, 0, 0, 1, 1);

        comboBoxDefaultStyle = new QComboBox(groupBox);
        comboBoxDefaultStyle->setObjectName("comboBoxDefaultStyle");

        gridLayout_2->addWidget(comboBoxDefaultStyle, 0, 1, 1, 1);

        checkBoxNoLCD = new QCheckBox(groupBox);
        checkBoxNoLCD->setObjectName("checkBoxNoLCD");
        checkBoxNoLCD->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        checkBoxNoLCD->setCheckable(true);

        gridLayout_2->addWidget(checkBoxNoLCD, 1, 1, 1, 1);

        checkBoxIndentationLines = new QCheckBox(groupBox);
        checkBoxIndentationLines->setObjectName("checkBoxIndentationLines");
        checkBoxIndentationLines->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        checkBoxIndentationLines->setCheckable(true);

        gridLayout_2->addWidget(checkBoxIndentationLines, 2, 0, 1, 1);

        checkBoxNoCompileButton = new QCheckBox(groupBox);
        checkBoxNoCompileButton->setObjectName("checkBoxNoCompileButton");
        checkBoxNoCompileButton->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        checkBoxNoCompileButton->setCheckable(true);

        gridLayout_2->addWidget(checkBoxNoCompileButton, 2, 1, 1, 1);

        checkBoxDebugOutput = new QCheckBox(groupBox);
        checkBoxDebugOutput->setObjectName("checkBoxDebugOutput");
        checkBoxDebugOutput->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        checkBoxDebugOutput->setCheckable(true);

        gridLayout_2->addWidget(checkBoxDebugOutput, 3, 0, 1, 1);

        checkBoxSimpleStatusbar = new QCheckBox(groupBox);
        checkBoxSimpleStatusbar->setObjectName("checkBoxSimpleStatusbar");
        checkBoxSimpleStatusbar->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        checkBoxSimpleStatusbar->setCheckable(true);

        gridLayout_2->addWidget(checkBoxSimpleStatusbar, 3, 1, 1, 1);

        label_33 = new QLabel(groupBox);
        label_33->setObjectName("label_33");

        gridLayout_2->addWidget(label_33, 4, 0, 1, 1);

        comboBoxDefaultGuiLanguage = new QComboBox(groupBox);
        comboBoxDefaultGuiLanguage->setObjectName("comboBoxDefaultGuiLanguage");

        gridLayout_2->addWidget(comboBoxDefaultGuiLanguage, 4, 1, 1, 1);

        checkBoxHighlightBraceBlock = new QCheckBox(groupBox);
        checkBoxHighlightBraceBlock->setObjectName("checkBoxHighlightBraceBlock");
        checkBoxHighlightBraceBlock->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        checkBoxHighlightBraceBlock->setCheckable(true);

        gridLayout_2->addWidget(checkBoxHighlightBraceBlock, 5, 1, 1, 1);

        checkBoxNoSplashScreen = new QCheckBox(groupBox);
        checkBoxNoSplashScreen->setObjectName("checkBoxNoSplashScreen");
        checkBoxNoSplashScreen->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        checkBoxNoSplashScreen->setCheckable(true);

        gridLayout_2->addWidget(checkBoxNoSplashScreen, 6, 1, 1, 1);


        gridLayout_21->addLayout(gridLayout_2, 0, 0, 1, 1);


        gridLayout_22->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(tabMisc);
        groupBox_2->setObjectName("groupBox_2");
        gridLayout_15 = new QGridLayout(groupBox_2);
        gridLayout_15->setObjectName("gridLayout_15");
        gridLayout_11 = new QGridLayout();
        gridLayout_11->setObjectName("gridLayout_11");
        label_31 = new QLabel(groupBox_2);
        label_31->setObjectName("label_31");

        gridLayout_11->addWidget(label_31, 0, 0, 1, 1);

        comboBoxDefaultCompiler = new QComboBox(groupBox_2);
        comboBoxDefaultCompiler->setObjectName("comboBoxDefaultCompiler");

        gridLayout_11->addWidget(comboBoxDefaultCompiler, 0, 1, 1, 1);

        checkBoxCreateIcon = new QCheckBox(groupBox_2);
        checkBoxCreateIcon->setObjectName("checkBoxCreateIcon");
        checkBoxCreateIcon->setEnabled(true);

        gridLayout_11->addWidget(checkBoxCreateIcon, 0, 2, 1, 1);

        checkBoxOpenOnFail = new QCheckBox(groupBox_2);
        checkBoxOpenOnFail->setObjectName("checkBoxOpenOnFail");

        gridLayout_11->addWidget(checkBoxOpenOnFail, 1, 0, 1, 1);

        checkBoxWarnRequesters = new QCheckBox(groupBox_2);
        checkBoxWarnRequesters->setObjectName("checkBoxWarnRequesters");

        gridLayout_11->addWidget(checkBoxWarnRequesters, 1, 1, 1, 1);


        gridLayout_15->addLayout(gridLayout_11, 0, 0, 1, 1);


        gridLayout_22->addWidget(groupBox_2, 1, 0, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 218, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_22->addItem(verticalSpacer_6, 2, 0, 1, 1);

        tabWidget->addTab(tabMisc, QString());

        gridLayout_10->addWidget(tabWidget, 0, 0, 1, 4);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_10->addItem(verticalSpacer_5, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(211, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_10->addItem(horizontalSpacer, 2, 0, 1, 1);

        btn_CancelSave = new QPushButton(PrefsDialog);
        btn_CancelSave->setObjectName("btn_CancelSave");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/fileexit.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_CancelSave->setIcon(icon3);

        gridLayout_10->addWidget(btn_CancelSave, 2, 2, 1, 1);

        btn_SavePrefs = new QPushButton(PrefsDialog);
        btn_SavePrefs->setObjectName("btn_SavePrefs");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/save.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btn_SavePrefs->setIcon(icon4);

        gridLayout_10->addWidget(btn_SavePrefs, 2, 3, 1, 1);

#if QT_CONFIG(shortcut)
        label->setBuddy(lineEdit_author);
        label_2->setBuddy(lineEdit_email);
        label_3->setBuddy(lineEdit_website);
        label_4->setBuddy(lineEdit_projectsRootDir);
        label_defaultTargetOS->setBuddy(comboBox_defaultTargetOS);
        label_5->setBuddy(lineEdit_projectsRootDir);
        label_6->setBuddy(lineEdit_projectsRootDir);
        label_7->setBuddy(lineEdit_projectsRootDir);
        label_27->setBuddy(lineEdit_projectsRootDir);
        label_29->setBuddy(lineEdit_projectsRootDir);
        label_30->setBuddy(lineEdit_projectsRootDir);
        label_8->setBuddy(lineEdit_projectsRootDir);
        label_37->setBuddy(lineEdit_projectsRootDir);
        label_38->setBuddy(lineEdit_projectsRootDir);
        label_39->setBuddy(lineEdit_projectsRootDir);
        label_9->setBuddy(lineEdit_projectsRootDir);
        label_40->setBuddy(lineEdit_projectsRootDir);
        label_41->setBuddy(lineEdit_projectsRootDir);
        label_42->setBuddy(lineEdit_projectsRootDir);
        label_ASIncludeDir->setBuddy(lineEdit_projectsRootDir);
        label_19->setBuddy(lineEdit_projectsRootDir);
        label_17->setBuddy(lineEdit_projectsRootDir);
        label_18->setBuddy(lineEdit_projectsRootDir);
        label_20->setBuddy(lineEdit_projectsRootDir);
        label_32->setBuddy(lineEdit_projectsRootDir);
        label_43->setBuddy(lineEdit_projectsRootDir);
        label_36->setBuddy(lineEdit_projectsRootDir);
        label_VasmIncludeDir->setBuddy(lineEdit_projectsRootDir);
        label_44->setBuddy(lineEdit_projectsRootDir);
        label_22->setBuddy(lineEdit_projectsRootDir);
        label_21->setBuddy(lineEdit_projectsRootDir);
        label_24->setBuddy(lineEdit_projectsRootDir);
        label_25->setBuddy(lineEdit_getAutodocsDir);
        label_ToolMuiPath->setBuddy(lineEdit_Tool1Path);
        label_ToolMuiParams->setBuddy(lineEdit_Tool1Params);
        label_ToolMuiName->setBuddy(lineEdit_Tool1Name);
        label_ToolGadtoolsPath->setBuddy(lineEdit_Tool2Path);
        label_ToolGadtoolsParams->setBuddy(lineEdit_Tool2Params);
        label_ToolGadtoolsName->setBuddy(lineEdit_Tool2Name);
        label_ToolReactionPath->setBuddy(lineEdit_Tool3Path);
        label_ToolReactionParams->setBuddy(lineEdit_Tool3Params);
        label_ToolReactionName->setBuddy(lineEdit_Tool3Name);
        label_ToolUserPath->setBuddy(lineEdit_Tool4Path);
        label_ToolUserParams->setBuddy(lineEdit_Tool4Params);
        label_ToolUserName->setBuddy(lineEdit_Tool4Name);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(lineEdit_author, lineEdit_email);
        QWidget::setTabOrder(lineEdit_email, lineEdit_website);
        QWidget::setTabOrder(lineEdit_website, lineEdit_projectsRootDir);
        QWidget::setTabOrder(lineEdit_projectsRootDir, btn_getProjectRootDir);
        QWidget::setTabOrder(btn_getProjectRootDir, btn_SavePrefs);
        QWidget::setTabOrder(btn_SavePrefs, btn_CancelSave);
        QWidget::setTabOrder(btn_CancelSave, lineEdit_getEmulatorExefile);
        QWidget::setTabOrder(lineEdit_getEmulatorExefile, tabWidget);
        QWidget::setTabOrder(tabWidget, lineEdit_getGCCexefile);
        QWidget::setTabOrder(lineEdit_getGCCexefile, btn_getGCCexefile);
        QWidget::setTabOrder(btn_getGCCexefile, lineEdit_getGPPexefile);
        QWidget::setTabOrder(lineEdit_getGPPexefile, btn_getGPPexefile);
        QWidget::setTabOrder(btn_getGPPexefile, lineEdit_getMAKEexefile);
        QWidget::setTabOrder(lineEdit_getMAKEexefile, btn_getMAKEexefile);
        QWidget::setTabOrder(btn_getMAKEexefile, lineEdit_getSTRIPexefile);
        QWidget::setTabOrder(lineEdit_getSTRIPexefile, btn_getSTRIPexefile);
        QWidget::setTabOrder(btn_getSTRIPexefile, lineEdit_getASexefile);
        QWidget::setTabOrder(lineEdit_getASexefile, btn_getASexefile);
        QWidget::setTabOrder(btn_getASexefile, lineEdit_getLDexefile);
        QWidget::setTabOrder(lineEdit_getLDexefile, btn_getLDexefile);
        QWidget::setTabOrder(btn_getLDexefile, lineEdit_getASIncludeDir);
        QWidget::setTabOrder(lineEdit_getASIncludeDir, btn_getASIncludeDir);
        QWidget::setTabOrder(btn_getASIncludeDir, lineEdit_GCC13CompilerOpts);
        QWidget::setTabOrder(lineEdit_GCC13CompilerOpts, lineEdit_GCC30CompilerOpts);
        QWidget::setTabOrder(lineEdit_GCC30CompilerOpts, lineEdit_GCC13LinkerOpts);
        QWidget::setTabOrder(lineEdit_GCC13LinkerOpts, lineEdit_GCC30LinkerOpts);
        QWidget::setTabOrder(lineEdit_GCC30LinkerOpts, lineEdit_GPP13CompilerOpts);
        QWidget::setTabOrder(lineEdit_GPP13CompilerOpts, lineEdit_GPP30CompilerOpts);
        QWidget::setTabOrder(lineEdit_GPP30CompilerOpts, lineEdit_GPP13LinkerOpts);
        QWidget::setTabOrder(lineEdit_GPP13LinkerOpts, lineEdit_GPP30LinkerOpts);
        QWidget::setTabOrder(lineEdit_GPP30LinkerOpts, lineEdit_getVCexefile);
        QWidget::setTabOrder(lineEdit_getVCexefile, btn_getVCexefile);
        QWidget::setTabOrder(btn_getVCexefile, lineEdit_getVASMexefile);
        QWidget::setTabOrder(lineEdit_getVASMexefile, btn_getVASMexefile);
        QWidget::setTabOrder(btn_getVASMexefile, lineEdit_getVCconfigDir);
        QWidget::setTabOrder(lineEdit_getVCconfigDir, btn_getVCconfigDir);
        QWidget::setTabOrder(btn_getVCconfigDir, lineEdit_getVASMIncludeDir);
        QWidget::setTabOrder(lineEdit_getVASMIncludeDir, btn_getVASMIncludeDir);
        QWidget::setTabOrder(btn_getVASMIncludeDir, lineEdit_VCdefaultOptsOS13);
        QWidget::setTabOrder(lineEdit_VCdefaultOptsOS13, lineEdit_VCdefaultOptsOS30);
        QWidget::setTabOrder(lineEdit_VCdefaultOptsOS30, lineEdit_VC13LinkerOpts);
        QWidget::setTabOrder(lineEdit_VC13LinkerOpts, lineEdit_VCdefaultLinkerOpts);
        QWidget::setTabOrder(lineEdit_VCdefaultLinkerOpts, lineEdit_SASCdefaultOpts);
        QWidget::setTabOrder(lineEdit_SASCdefaultOpts, btn_getEmulatorExefile);
        QWidget::setTabOrder(btn_getEmulatorExefile, lineEdit_getOS13Configfile);
        QWidget::setTabOrder(lineEdit_getOS13Configfile, btn_getOS13Configfile);
        QWidget::setTabOrder(btn_getOS13Configfile, btn_editOS13Configfile);
        QWidget::setTabOrder(btn_editOS13Configfile, lineEdit_getOS3Configfile);
        QWidget::setTabOrder(lineEdit_getOS3Configfile, btn_getOS3Configfile);
        QWidget::setTabOrder(btn_getOS3Configfile, btn_editOS3Configfile);
        QWidget::setTabOrder(btn_editOS3Configfile, lineEdit_getAutodocsDir);
        QWidget::setTabOrder(lineEdit_getAutodocsDir, btn_getAutodocsDir);
        QWidget::setTabOrder(btn_getAutodocsDir, checkBoxIndentationLines);
        QWidget::setTabOrder(checkBoxIndentationLines, checkBoxNoLCD);
        QWidget::setTabOrder(checkBoxNoLCD, checkBoxDebugOutput);
        QWidget::setTabOrder(checkBoxDebugOutput, comboBoxDefaultStyle);
        QWidget::setTabOrder(comboBoxDefaultStyle, comboBoxDefaultCompiler);
        QWidget::setTabOrder(comboBoxDefaultCompiler, checkBoxNoCompileButton);
        QWidget::setTabOrder(checkBoxNoCompileButton, checkBoxSimpleStatusbar);
        QWidget::setTabOrder(checkBoxSimpleStatusbar, comboBoxDefaultGuiLanguage);
        QWidget::setTabOrder(comboBoxDefaultGuiLanguage, checkBoxHighlightBraceBlock);
        QWidget::setTabOrder(checkBoxHighlightBraceBlock, checkBoxOpenOnFail);
        QWidget::setTabOrder(checkBoxOpenOnFail, checkBoxWarnRequesters);

        retranslateUi(PrefsDialog);

        tabWidget->setCurrentIndex(0);
        comboBox_defaultTargetOS->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(PrefsDialog);
    } // setupUi

    void retranslateUi(QDialog *PrefsDialog)
    {
        PrefsDialog->setWindowTitle(QCoreApplication::translate("PrefsDialog", "Prefs - AmigaED 4.0 ", nullptr));
        label->setText(QCoreApplication::translate("PrefsDialog", "Au&thor:", nullptr));
        lineEdit_author->setText(QCoreApplication::translate("PrefsDialog", "author_name", nullptr));
        label_2->setText(QCoreApplication::translate("PrefsDialog", "Email:", nullptr));
        lineEdit_email->setText(QCoreApplication::translate("PrefsDialog", "someone@somewhere.com", nullptr));
        label_3->setText(QCoreApplication::translate("PrefsDialog", "Website:", nullptr));
        lineEdit_website->setText(QCoreApplication::translate("PrefsDialog", "http://www.somewhere.com", nullptr));
        label_4->setText(QCoreApplication::translate("PrefsDialog", "Pro&jects root:", nullptr));
        lineEdit_projectsRootDir->setText(QString());
#if QT_CONFIG(tooltip)
        btn_getProjectRootDir->setToolTip(QCoreApplication::translate("PrefsDialog", "Select default Root Folder for your Projects<br><b>Tipp:</b> Should be accessiable by one of your emulated Amiga systems!", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_getProjectRootDir->setText(QString());
#if QT_CONFIG(tooltip)
        checkBox_saveProjectFilesAutomatically->setToolTip(QCoreApplication::translate("PrefsDialog", "If checked, any open project file (including Makefiles) with unsaved changes is saved automatically before a project build, with no prompt. If unchecked, you are asked each time instead.", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_saveProjectFilesAutomatically->setText(QCoreApplication::translate("PrefsDialog", "Save Project Files Automatically", nullptr));
        label_defaultTargetOS->setText(QCoreApplication::translate("PrefsDialog", "Default Target OS:", nullptr));
        comboBox_defaultTargetOS->setItemText(0, QCoreApplication::translate("PrefsDialog", "OS 1.3", nullptr));
        comboBox_defaultTargetOS->setItemText(1, QCoreApplication::translate("PrefsDialog", "OS 3.x", nullptr));

#if QT_CONFIG(tooltip)
        comboBox_defaultTargetOS->setToolTip(QCoreApplication::translate("PrefsDialog", "Select whether to compile, generate Makefiles, and start the emulator for OS 1.3 or OS 2.x/3.x by default. Used at startup, and whenever no open project (or an imported one - see File > Import existing Project...) says otherwise.", nullptr));
#endif // QT_CONFIG(tooltip)
        tabWidget->setTabText(tabWidget->indexOf(tabProject), QCoreApplication::translate("PrefsDialog", "Project", nullptr));
#if QT_CONFIG(tooltip)
        tabWidget->setTabToolTip(tabWidget->indexOf(tabProject), QCoreApplication::translate("PrefsDialog", "Basic settings for macros and pathes...", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_getGCCexefile->setText(QString());
        btn_getGCCexefile->setText(QString());
        label_5->setText(QCoreApplication::translate("PrefsDialog", "GNU gcc:", nullptr));
        btn_getGPPexefile->setText(QString());
        lineEdit_getGPPexefile->setText(QString());
        label_6->setText(QCoreApplication::translate("PrefsDialog", "GNU g++:", nullptr));
        lineEdit_getMAKEexefile->setText(QString());
        btn_getMAKEexefile->setText(QString());
        label_7->setText(QCoreApplication::translate("PrefsDialog", "GNU make:", nullptr));
        lineEdit_getSTRIPexefile->setText(QString());
        btn_getSTRIPexefile->setText(QString());
        label_27->setText(QCoreApplication::translate("PrefsDialog", "GNU s&trip:", nullptr));
        lineEdit_getASexefile->setText(QString());
        btn_getASexefile->setText(QString());
        label_29->setText(QCoreApplication::translate("PrefsDialog", "GNU as:", nullptr));
        lineEdit_getLDexefile->setText(QString());
        btn_getLDexefile->setText(QString());
        label_30->setText(QCoreApplication::translate("PrefsDialog", "GNU ld:", nullptr));
        label_8->setText(QCoreApplication::translate("PrefsDialog", "GCC OS 1.3 compiler opts:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_GCC13CompilerOpts->setToolTip(QCoreApplication::translate("PrefsDialog", "Compiler options for m68k-amigaos-gcc when targetting AmigaOS 1.3 (used for single-file compiles, the \"Compiler Options\" prompt when creating a new project, and the generated Makefile's CCARGS). Includes -mcrt=nix13, since it affects code generation and must be present at compile time too, not just when linking.", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_GCC13CompilerOpts->setText(QCoreApplication::translate("PrefsDialog", "-std=gnu89 -O2 -Wall -mcrt=nix13", nullptr));
        label_37->setText(QCoreApplication::translate("PrefsDialog", "GCC OS 3.x compiler opts:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_GCC30CompilerOpts->setToolTip(QCoreApplication::translate("PrefsDialog", "Compiler options for m68k-amigaos-gcc when targetting AmigaOS 3.x (used for single-file compiles, the \"Compiler Options\" prompt when creating a new project, and the generated Makefile's CCARGS)", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_GCC30CompilerOpts->setText(QCoreApplication::translate("PrefsDialog", "-std=gnu89 -O2 -Wall", nullptr));
        label_38->setText(QCoreApplication::translate("PrefsDialog", "GCC OS 1.3 linker opts:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_GCC13LinkerOpts->setToolTip(QCoreApplication::translate("PrefsDialog", "Linker options for m68k-amigaos-gcc when targetting AmigaOS 1.3 (used for single-file compiles, the \"Linker Options\" prompt when creating a new project, and the generated Makefile's LDFLAGS) Note: -lamiga/-lauto are VBCC library names (amiga.lib/auto.lib) and do not apply to m68k-amigaos-gcc.", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_GCC13LinkerOpts->setText(QCoreApplication::translate("PrefsDialog", "-noixemul", nullptr));
        label_39->setText(QCoreApplication::translate("PrefsDialog", "GCC OS 3.x linker opts:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_GCC30LinkerOpts->setToolTip(QCoreApplication::translate("PrefsDialog", "Linker options for m68k-amigaos-gcc when targetting AmigaOS 3.x (used for single-file compiles, the \"Linker Options\" prompt when creating a new project, and the generated Makefile's LDFLAGS) Note: -lamiga/-lauto are VBCC library names (amiga.lib/auto.lib) and do not apply to m68k-amigaos-gcc.", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_GCC30LinkerOpts->setText(QCoreApplication::translate("PrefsDialog", "-noixemul", nullptr));
        label_9->setText(QCoreApplication::translate("PrefsDialog", "G++ OS 1.3 compiler opts:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_GPP13CompilerOpts->setToolTip(QCoreApplication::translate("PrefsDialog", "Compiler options for m68k-amigaos-g++ when targetting AmigaOS 1.3 (used for single-file compiles, the \"Compiler Options\" prompt when creating a new project, and the generated Makefile's CCARGS). Includes -mcrt=nix13, since it affects code generation and must be present at compile time too, not just when linking.", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_GPP13CompilerOpts->setText(QCoreApplication::translate("PrefsDialog", "-std=c++11 -O2 -Wall -mcrt=nix13", nullptr));
        label_40->setText(QCoreApplication::translate("PrefsDialog", "G++ OS 3.x compiler opts:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_GPP30CompilerOpts->setToolTip(QCoreApplication::translate("PrefsDialog", "Compiler options for m68k-amigaos-g++ when targetting AmigaOS 3.x (used for single-file compiles, the \"Compiler Options\" prompt when creating a new project, and the generated Makefile's CCARGS)", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_GPP30CompilerOpts->setText(QCoreApplication::translate("PrefsDialog", "-std=c++11 -O2 -Wall", nullptr));
        label_41->setText(QCoreApplication::translate("PrefsDialog", "G++ OS 1.3 linker opts:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_GPP13LinkerOpts->setToolTip(QCoreApplication::translate("PrefsDialog", "Linker options for m68k-amigaos-g++ when targetting AmigaOS 1.3 (used for single-file compiles, the \"Linker Options\" prompt when creating a new project, and the generated Makefile's LDFLAGS) Note: -lamiga/-lauto are VBCC library names (amiga.lib/auto.lib) and do not apply to m68k-amigaos-gcc.", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_GPP13LinkerOpts->setText(QCoreApplication::translate("PrefsDialog", "-noixemul", nullptr));
        label_42->setText(QCoreApplication::translate("PrefsDialog", "G++ OS 3.x linker opts:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_GPP30LinkerOpts->setToolTip(QCoreApplication::translate("PrefsDialog", "Linker options for m68k-amigaos-g++ when targetting AmigaOS 3.x (used for single-file compiles, the \"Linker Options\" prompt when creating a new project, and the generated Makefile's LDFLAGS) Note: -lamiga/-lauto are VBCC library names (amiga.lib/auto.lib) and do not apply to m68k-amigaos-gcc.", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_GPP30LinkerOpts->setText(QCoreApplication::translate("PrefsDialog", "-noixemul", nullptr));
        label_ASIncludeDir->setText(QCoreApplication::translate("PrefsDialog", "Assembler Include Path:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_getASIncludeDir->setToolTip(QCoreApplication::translate("PrefsDialog", "Search directory for GNU as's \".include\" directive, added as -I<path> to the generated Makefile's assemble rule (only when this field isn't empty). Only affects hand-written .asm/.s sources assembled with GNU as, not C/C++ compiles. Avoid paths containing spaces - the value is inserted unquoted (quoting it breaks mingw32-make on Windows).", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_getASIncludeDir->setText(QString());
        btn_getASIncludeDir->setText(QString());
#if QT_CONFIG(tooltip)
        checkBox_ShowGccOpts->setToolTip(QCoreApplication::translate("PrefsDialog", "<html><head/><body><p>If set, you will be presented with an options field where you might change options for recent compilation every time compilation is triggered</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_ShowGccOpts->setText(QCoreApplication::translate("PrefsDialog", "Show/Change Opts at compiler start", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabGCC), QCoreApplication::translate("PrefsDialog", "GCC", nullptr));
#if QT_CONFIG(tooltip)
        tabWidget->setTabToolTip(tabWidget->indexOf(tabGCC), QCoreApplication::translate("PrefsDialog", "Basic settings for gcc and g++", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_getVCexefile->setText(QString());
        label_19->setText(QCoreApplication::translate("PrefsDialog", "vc: ", nullptr));
        lineEdit_getVCexefile->setText(QString());
        label_17->setText(QCoreApplication::translate("PrefsDialog", "vasm: ", nullptr));
        btn_getVASMexefile->setText(QString());
        lineEdit_getVASMexefile->setText(QString());
        label_18->setText(QCoreApplication::translate("PrefsDialog", "config dir:", nullptr));
        btn_getVCconfigDir->setText(QString());
        lineEdit_getVCconfigDir->setText(QString());
#if QT_CONFIG(tooltip)
        lineEdit_VCdefaultOptsOS13->setToolTip(QCoreApplication::translate("PrefsDialog", "Compiler options for vbcc when targetting AmigaOS 1.3 (used for single-file compiles, the \"Compiler Options\" prompt when creating a new project, and the generated Makefile's CCARGS). '+kick13' must stay the first token.", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_VCdefaultOptsOS13->setText(QCoreApplication::translate("PrefsDialog", "+kick13 -cpp-comments -v -O2 -size -cpu=68020 -I$NDK13_INC", nullptr));
        label_20->setText(QCoreApplication::translate("PrefsDialog", "VBCC OS &1.3 compiler opts:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_VCdefaultOptsOS30->setToolTip(QCoreApplication::translate("PrefsDialog", "Compiler options for vbcc when targetting AmigaOS 3.x (used for single-file compiles, the \"Compiler Options\" prompt when creating a new project, and the generated Makefile's CCARGS). '+aos68k' must stay the first token.", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_VCdefaultOptsOS30->setText(QCoreApplication::translate("PrefsDialog", "+aos68k -cpp-comments  -v -O2 -size -cpu=68030 -I$NDK39_INC", nullptr));
        label_32->setText(QCoreApplication::translate("PrefsDialog", "VBCC OS &3.x compiler opts:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_VC13LinkerOpts->setToolTip(QCoreApplication::translate("PrefsDialog", "Linker options for vbcc when targetting AmigaOS 1.3 (used for single-file compiles, the \"Linker Options\" prompt when creating a new project, and the generated Makefile's LDFLAGS)", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_VC13LinkerOpts->setText(QCoreApplication::translate("PrefsDialog", "-noixemul -L$NDK13_LIB -lauto", nullptr));
        label_43->setText(QCoreApplication::translate("PrefsDialog", "VBCC OS 1.3 linker opts:", nullptr));
        label_36->setText(QCoreApplication::translate("PrefsDialog", "VBCC OS 3.x linker opts:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_VCdefaultLinkerOpts->setToolTip(QCoreApplication::translate("PrefsDialog", "Linker options for vbcc when targetting AmigaOS 3.x (used for single-file compiles, the \"Linker Options\" prompt when creating a new project, and the generated Makefile's LDFLAGS)", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_VCdefaultLinkerOpts->setText(QCoreApplication::translate("PrefsDialog", "-noixemul -L$NDK39_LIB -lmieee -lauto -lamiga", nullptr));
        label_VasmIncludeDir->setText(QCoreApplication::translate("PrefsDialog", "Assembler Include Path:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_getVASMIncludeDir->setToolTip(QCoreApplication::translate("PrefsDialog", "Search directory for vasm's \"include\" directive, added as -I<path> to the generated Makefile's assemble rule (only when this field isn't empty). Only affects hand-written .asm/.s sources assembled with vasm, not C/C++ compiles. Avoid paths containing spaces - the value is inserted unquoted (quoting it breaks mingw32-make on Windows).", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_getVASMIncludeDir->setText(QString());
        btn_getVASMIncludeDir->setText(QString());
#if QT_CONFIG(tooltip)
        checkBox_ShowVbccOpts->setToolTip(QCoreApplication::translate("PrefsDialog", "<html><head/><body><p>If set, you will be presented with an options field where you might change options for recent compilation every time compilation is triggered</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_ShowVbccOpts->setText(QCoreApplication::translate("PrefsDialog", "Show/Change Opts at compiler start", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabVBCC), QCoreApplication::translate("PrefsDialog", "VBCC", nullptr));
#if QT_CONFIG(tooltip)
        tabWidget->setTabToolTip(tabWidget->indexOf(tabVBCC), QCoreApplication::translate("PrefsDialog", "Basic settings for vc", nullptr));
#endif // QT_CONFIG(tooltip)
        label_44->setText(QCoreApplication::translate("PrefsDialog", "SAS/C default opts:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_SASCdefaultOpts->setToolTip(QCoreApplication::translate("PrefsDialog", "Options passed to \"sc\" in the generated Makefile.sc (e.g. \"sc ICONS ERRORREXX file.c LINK TO target\"). No OS 1.3/3.x distinction is made for SAS/C.", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_SASCdefaultOpts->setText(QCoreApplication::translate("PrefsDialog", "ICONS ERRORREXX", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabSASC), QCoreApplication::translate("PrefsDialog", "SAS/C", nullptr));
#if QT_CONFIG(tooltip)
        tabWidget->setTabToolTip(tabWidget->indexOf(tabSASC), QCoreApplication::translate("PrefsDialog", "Basic settings for SAS/C - used only for generating Makefile.sc, never invoked automatically (SAS/C only runs on a real Amiga or emulator)", nullptr));
#endif // QT_CONFIG(tooltip)
        label_22->setText(QCoreApplication::translate("PrefsDialog", "UAE:", nullptr));
        lineEdit_getEmulatorExefile->setText(QString());
        btn_getEmulatorExefile->setText(QString());
        label_21->setText(QCoreApplication::translate("PrefsDialog", "OS &1.3 config:", nullptr));
        lineEdit_getOS13Configfile->setText(QString());
        btn_getOS13Configfile->setText(QString());
#if QT_CONFIG(tooltip)
        btn_editOS13Configfile->setToolTip(QCoreApplication::translate("PrefsDialog", "Open this config file in the system's text editor", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_editOS13Configfile->setText(QCoreApplication::translate("PrefsDialog", "Edit", nullptr));
        label_24->setText(QCoreApplication::translate("PrefsDialog", "OS &3.x config:", nullptr));
        lineEdit_getOS3Configfile->setText(QString());
        btn_getOS3Configfile->setText(QString());
#if QT_CONFIG(tooltip)
        btn_editOS3Configfile->setToolTip(QCoreApplication::translate("PrefsDialog", "Open this config file in the system's text editor", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_editOS3Configfile->setText(QCoreApplication::translate("PrefsDialog", "Edit", nullptr));
        label_25->setText(QCoreApplication::translate("PrefsDialog", "&AutoDocs folder:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_getAutodocsDir->setToolTip(QCoreApplication::translate("PrefsDialog", "NDK AutoDocs folder, e.g. Include_I's \"Autodocs\" drawer - used by Build > AutoDoc Reader...", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_getAutodocsDir->setText(QString());
        btn_getAutodocsDir->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tabEmulator), QCoreApplication::translate("PrefsDialog", "Emulator", nullptr));
#if QT_CONFIG(tooltip)
        tabWidget->setTabToolTip(tabWidget->indexOf(tabEmulator), QCoreApplication::translate("PrefsDialog", "Basic settings for UAE emulation startup", nullptr));
#endif // QT_CONFIG(tooltip)
        groupBox_ToolMui->setTitle(QCoreApplication::translate("PrefsDialog", "MUI GUI Designer", nullptr));
        label_ToolMuiPath->setText(QCoreApplication::translate("PrefsDialog", "Path:", nullptr));
        lineEdit_Tool1Path->setText(QString());
        btn_getTool1Path->setText(QString());
        label_ToolMuiParams->setText(QCoreApplication::translate("PrefsDialog", "Parameters:", nullptr));
        lineEdit_Tool1Params->setText(QString());
        label_ToolMuiName->setText(QCoreApplication::translate("PrefsDialog", "Name:", nullptr));
        lineEdit_Tool1Name->setText(QString());
        groupBox_ToolGadtools->setTitle(QCoreApplication::translate("PrefsDialog", "GadTools GUI Designer", nullptr));
        label_ToolGadtoolsPath->setText(QCoreApplication::translate("PrefsDialog", "Path:", nullptr));
        lineEdit_Tool2Path->setText(QString());
        btn_getTool2Path->setText(QString());
        label_ToolGadtoolsParams->setText(QCoreApplication::translate("PrefsDialog", "Parameters:", nullptr));
        lineEdit_Tool2Params->setText(QString());
        label_ToolGadtoolsName->setText(QCoreApplication::translate("PrefsDialog", "Name:", nullptr));
        lineEdit_Tool2Name->setText(QString());
        groupBox_ToolReaction->setTitle(QCoreApplication::translate("PrefsDialog", "ReAction GUI Designer", nullptr));
        label_ToolReactionPath->setText(QCoreApplication::translate("PrefsDialog", "Path:", nullptr));
        lineEdit_Tool3Path->setText(QString());
        btn_getTool3Path->setText(QString());
        label_ToolReactionParams->setText(QCoreApplication::translate("PrefsDialog", "Parameters:", nullptr));
        lineEdit_Tool3Params->setText(QString());
        label_ToolReactionName->setText(QCoreApplication::translate("PrefsDialog", "Name:", nullptr));
        lineEdit_Tool3Name->setText(QString());
        groupBox_ToolUser->setTitle(QCoreApplication::translate("PrefsDialog", "User Tool", nullptr));
        label_ToolUserPath->setText(QCoreApplication::translate("PrefsDialog", "Path:", nullptr));
        lineEdit_Tool4Path->setText(QString());
        btn_getTool4Path->setText(QString());
        label_ToolUserParams->setText(QCoreApplication::translate("PrefsDialog", "Parameters:", nullptr));
        lineEdit_Tool4Params->setText(QString());
        label_ToolUserName->setText(QCoreApplication::translate("PrefsDialog", "Name:", nullptr));
        lineEdit_Tool4Name->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tabTools), QCoreApplication::translate("PrefsDialog", "Tools", nullptr));
#if QT_CONFIG(tooltip)
        tabWidget->setTabToolTip(tabWidget->indexOf(tabTools), QCoreApplication::translate("PrefsDialog", "External GUI builder tools, launchable from the Tools menu", nullptr));
#endif // QT_CONFIG(tooltip)
        groupBox->setTitle(QCoreApplication::translate("PrefsDialog", "Application Look and Feel", nullptr));
        label_28->setText(QCoreApplication::translate("PrefsDialog", "Default application style:", nullptr));
#if QT_CONFIG(tooltip)
        comboBoxDefaultStyle->setToolTip(QCoreApplication::translate("PrefsDialog", "Select default Application style (only styles that are available on your system will be shown!). \"Dark\" gives the whole application - including the editor - a dark theme.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        checkBoxNoLCD->setToolTip(QCoreApplication::translate("PrefsDialog", "Use standard text widgets for cursor position", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBoxNoLCD->setText(QCoreApplication::translate("PrefsDialog", "Don't use LCD for cursor position", nullptr));
#if QT_CONFIG(tooltip)
        checkBoxIndentationLines->setToolTip(QCoreApplication::translate("PrefsDialog", "Show indentation guides by default?", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBoxIndentationLines->setText(QCoreApplication::translate("PrefsDialog", "Show indentation lines:", nullptr));
#if QT_CONFIG(tooltip)
        checkBoxNoCompileButton->setToolTip(QCoreApplication::translate("PrefsDialog", "Hide compiler selection and compile buttons from statusbar", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBoxNoCompileButton->setText(QCoreApplication::translate("PrefsDialog", "Don't show compiler buttons in statusbar", nullptr));
#if QT_CONFIG(tooltip)
        checkBoxDebugOutput->setToolTip(QCoreApplication::translate("PrefsDialog", "Show debug output (only usefull for developers in order to enhange Amiga Cross Editor!)", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBoxDebugOutput->setText(QCoreApplication::translate("PrefsDialog", "Show debug output:", nullptr));
#if QT_CONFIG(tooltip)
        checkBoxSimpleStatusbar->setToolTip(QCoreApplication::translate("PrefsDialog", "use a simple statusbar without any gimmicks", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBoxSimpleStatusbar->setText(QCoreApplication::translate("PrefsDialog", "Use simple statusbar", nullptr));
        label_33->setText(QCoreApplication::translate("PrefsDialog", "Default GUI Language:", nullptr));
#if QT_CONFIG(tooltip)
        comboBoxDefaultGuiLanguage->setToolTip(QCoreApplication::translate("PrefsDialog", "Select the GUI language to use the next time the program starts (can also be switched at runtime via View -> GUI Language)", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        checkBoxHighlightBraceBlock->setToolTip(QCoreApplication::translate("PrefsDialog", "When using Goto matching bracket, highlight the text between the matched bracket pair with a readability-preserving background", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBoxHighlightBraceBlock->setText(QCoreApplication::translate("PrefsDialog", "Highlight block between braces", nullptr));
#if QT_CONFIG(tooltip)
        checkBoxNoSplashScreen->setToolTip(QCoreApplication::translate("PrefsDialog", "Skip the AmigaED logo/progress splash window normally shown while the program starts up", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBoxNoSplashScreen->setText(QCoreApplication::translate("PrefsDialog", "Don't show Splash Screen at Startup", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("PrefsDialog", "Build System", nullptr));
        label_31->setText(QCoreApplication::translate("PrefsDialog", "Default Cross Compiler:", nullptr));
#if QT_CONFIG(tooltip)
        comboBoxDefaultCompiler->setToolTip(QCoreApplication::translate("PrefsDialog", "Select default cross compiler to use", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        checkBoxCreateIcon->setToolTip(QCoreApplication::translate("PrefsDialog", "Create an icon for your successfully compiled Amiga program", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBoxCreateIcon->setText(QCoreApplication::translate("PrefsDialog", "create icon", nullptr));
#if QT_CONFIG(tooltip)
        checkBoxOpenOnFail->setToolTip(QCoreApplication::translate("PrefsDialog", "<html><head/><body><p>Open output console on compiler failure only</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBoxOpenOnFail->setText(QCoreApplication::translate("PrefsDialog", "Open panel on failure only", nullptr));
#if QT_CONFIG(tooltip)
        checkBoxWarnRequesters->setToolTip(QCoreApplication::translate("PrefsDialog", "<html><head/><body><p>Dont' show messageboxes on compilation finished or failed</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBoxWarnRequesters->setText(QCoreApplication::translate("PrefsDialog", "Don't use warn requesters", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabMisc), QCoreApplication::translate("PrefsDialog", "Misc", nullptr));
#if QT_CONFIG(tooltip)
        tabWidget->setTabToolTip(tabWidget->indexOf(tabMisc), QCoreApplication::translate("PrefsDialog", "Misc settings for GUI look and feel, compiler to use, compiler behaviour...", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_CancelSave->setText(QCoreApplication::translate("PrefsDialog", "Cancel", nullptr));
        btn_SavePrefs->setText(QCoreApplication::translate("PrefsDialog", "Save Prefs", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrefsDialog: public Ui_PrefsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFSDIALOG_H
