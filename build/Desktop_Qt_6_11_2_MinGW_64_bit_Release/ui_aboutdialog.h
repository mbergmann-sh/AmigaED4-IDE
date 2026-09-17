/********************************************************************************
** Form generated from reading UI file 'aboutdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_aboutDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label_logo;
    QTabWidget *tabWidget;
    QWidget *tabAbout;
    QGridLayout *gridLayout_2;
    QTextBrowser *textBrowser;
    QWidget *tabLicence;
    QGridLayout *gridLayout_3;
    QTextBrowser *textBrowser_2;
    QWidget *tabCredits;
    QGridLayout *gridLayout_4;
    QTextBrowser *textBrowser_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *aboutDialog)
    {
        if (aboutDialog->objectName().isEmpty())
            aboutDialog->setObjectName("aboutDialog");
        aboutDialog->setWindowModality(Qt::WindowModality::ApplicationModal);
        aboutDialog->resize(313, 331);
        aboutDialog->setMinimumSize(QSize(313, 331));
        aboutDialog->setMaximumSize(QSize(313, 331));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/amiga_classic.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        aboutDialog->setWindowIcon(icon);
        aboutDialog->setModal(true);
        gridLayout = new QGridLayout(aboutDialog);
        gridLayout->setObjectName("gridLayout");
        label_logo = new QLabel(aboutDialog);
        label_logo->setObjectName("label_logo");
        label_logo->setMinimumSize(QSize(301, 141));
        label_logo->setMaximumSize(QSize(301, 141));
        label_logo->setFrameShape(QFrame::Shape::NoFrame);
        label_logo->setPixmap(QPixmap(QString::fromUtf8(":/images/aced-logo.png")));

        gridLayout->addWidget(label_logo, 0, 0, 1, 2);

        tabWidget = new QTabWidget(aboutDialog);
        tabWidget->setObjectName("tabWidget");
        tabAbout = new QWidget();
        tabAbout->setObjectName("tabAbout");
        gridLayout_2 = new QGridLayout(tabAbout);
        gridLayout_2->setObjectName("gridLayout_2");
        textBrowser = new QTextBrowser(tabAbout);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setOpenLinks(false);

        gridLayout_2->addWidget(textBrowser, 0, 0, 1, 1);

        tabWidget->addTab(tabAbout, QString());
        tabLicence = new QWidget();
        tabLicence->setObjectName("tabLicence");
        gridLayout_3 = new QGridLayout(tabLicence);
        gridLayout_3->setObjectName("gridLayout_3");
        textBrowser_2 = new QTextBrowser(tabLicence);
        textBrowser_2->setObjectName("textBrowser_2");
        textBrowser_2->setOpenLinks(false);

        gridLayout_3->addWidget(textBrowser_2, 0, 0, 1, 1);

        tabWidget->addTab(tabLicence, QString());
        tabCredits = new QWidget();
        tabCredits->setObjectName("tabCredits");
        gridLayout_4 = new QGridLayout(tabCredits);
        gridLayout_4->setObjectName("gridLayout_4");
        textBrowser_3 = new QTextBrowser(tabCredits);
        textBrowser_3->setObjectName("textBrowser_3");
        textBrowser_3->setOpenLinks(false);

        gridLayout_4->addWidget(textBrowser_3, 0, 0, 1, 1);

        tabWidget->addTab(tabCredits, QString());

        gridLayout->addWidget(tabWidget, 1, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(201, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 0, 1, 1);

        pushButton = new QPushButton(aboutDialog);
        pushButton->setObjectName("pushButton");
        pushButton->setIcon(icon);

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);


        retranslateUi(aboutDialog);
        QObject::connect(pushButton, &QPushButton::clicked, aboutDialog, qOverload<>(&QDialog::accept));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(aboutDialog);
    } // setupUi

    void retranslateUi(QDialog *aboutDialog)
    {
        aboutDialog->setWindowTitle(QCoreApplication::translate("aboutDialog", "About AmigaED 4.0...", nullptr));
        label_logo->setText(QString());
        textBrowser->setHtml(QCoreApplication::translate("aboutDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Noto Sans'; font-size:10pt; font-weight:600;\">AmigaED v4.0</span><span style=\" font-family:'Noto Sans'; font-size:10pt;\"> is meant to be the missing link between advanced Amiga Cross Compiler Toolchains and the comfort of editing and compiling source files in a decent environment.<br />Originally started as an example for writing an advanced coder's editor using the </sp"
                        "an><span style=\" font-family:'Noto Sans'; font-size:10pt; font-style:italic;\">Qt5</span><span style=\" font-family:'Noto Sans'; font-size:10pt;\"> and </span><span style=\" font-family:'Noto Sans'; font-size:10pt; font-style:italic;\">QScintilla</span><span style=\" font-family:'Noto Sans'; font-size:10pt;\"> frameworks in C++, containing a menu bar, toolbars, menus, a custom context menue, an enhanced statusbar and some sophisticated editing actions, AmigaED finally grew into a full-fleshed IDE for AmigaOS application development. The source is based on Riverbank's original editor example, as provided with the </span><span style=\" font-family:'Noto Sans'; font-size:10pt; font-style:italic;\">QScintilla sources</span><span style=\" font-family:'Noto Sans'; font-size:10pt;\">. </span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabAbout), QCoreApplication::translate("aboutDialog", "About", nullptr));
        textBrowser_2->setHtml(QCoreApplication::translate("aboutDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Noto Sans'; font-size:10pt; font-style:italic;\">Copyright (C) 2018/2019 </span><span style=\" font-family:'Noto Sans'; font-size:10pt;\">by Michael Bergmann. Placed in the public domain with some restrictions!</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-famil"
                        "y:'Noto Sans'; font-size:10pt;\">You may use or enhance this piece of software anyway you want to - as long as you don't violate laws or copyright issues. I hereby </span><span style=\" font-family:'Noto Sans'; font-size:10pt; font-style:italic;\">explicitely prohibit</span><span style=\" font-family:'Noto Sans'; font-size:10pt;\"> the usage of my work to people who believe in racism, fascism and any kind of attitude against democratic lifestyle. </span><span style=\" font-family:'Noto Sans'; font-size:10pt; font-style:italic;\">It is self-explanatory that this prohibits the usage of my work to any member or fan of the german AfD party, aswell as MAGA-Supporters and Putin's minions. A free world needs free Software - and it needs free people.</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Noto Sans'; font-size:10pt;\">Further information about Qt licensing is available at: </span><a href=\"htt"
                        "p://www.trolltech.com/products/qt/licensing.html\"><span style=\" font-family:'Noto Sans'; font-size:10pt; text-decoration: underline; color:#2980b9;\">http://www.trolltech.com/products/qt/licensing.html</span></a><span style=\" font-family:'Noto Sans'; font-size:10pt;\"> or by contacting </span><a href=\"mailto:info@trolltech.com\"><span style=\" font-family:'Noto Sans'; font-size:10pt; text-decoration: underline; color:#2980b9;\">info@trolltech.com</span></a><span style=\" font-family:'Noto Sans'; font-size:10pt;\">.</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Noto Sans'; font-size:10pt;\">Further information about QScintilla licensing is available at: </span><a href=\"https://www.riverbankcomputing.com/software/qscintilla/license\"><span style=\" font-family:'Noto Sans'; font-size:10pt; text-decoration: underline; color:#2980b9;\">https://www.riverbankcomputing.com/software/qscintilla/li"
                        "cense</span></a><span style=\" font-family:'Noto Sans'; font-size:10pt;\"> or by contacting </span><a href=\"mailto:sales@riverbankcomputing.com\"><span style=\" font-family:'Noto Sans'; font-size:10pt; text-decoration: underline; color:#2980b9;\">sales@riverbankcomputing.com</span></a><span style=\" font-family:'Noto Sans'; font-size:10pt;\">.</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Noto Sans'; font-size:10pt;\">This program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.</span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabLicence), QCoreApplication::translate("aboutDialog", "License", nullptr));
        textBrowser_3->setHtml(QCoreApplication::translate("aboutDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<ul style=\"margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -qt-list-indent: 1;\">\n"
"<li style=\" font-family:'Noto Sans'; font-size:10pt;\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Matic Kucovec and Kristof Mulier for their great tutorial and website on how to use QScintilla in Python</li>\n"
"<li style=\" font-family:'Noto Sans'; font-size:10pt;\" style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margi"
                        "n-right:0px; -qt-block-indent:0; text-indent:0px;\">Keith F. Kelly for pointing me out on how to kill an existing QsciLexer</li></ul>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Noto Sans'; font-size:10pt;\">...and last, but not least all those not personally mentioned people out on stackoverflow.com for beeing patiently helpfull. Thanks for your helping hands when I got stuck, folks. You rock!</span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabCredits), QCoreApplication::translate("aboutDialog", "Credits", nullptr));
        pushButton->setText(QCoreApplication::translate("aboutDialog", "So what?", nullptr));
    } // retranslateUi

};

namespace Ui {
    class aboutDialog: public Ui_aboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
