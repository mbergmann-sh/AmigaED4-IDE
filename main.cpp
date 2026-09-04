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

#include <QApplication>
#include <QIcon>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);

    QString cmdFileName = argv[1];

    QApplication app(argc, argv);
    app.setApplicationName("Amiga Cross Editor");
    app.setApplicationVersion("4.0");
    app.setOrganizationName("MB-SoftWorX");
    app.setOrganizationDomain("http://www.mbergmann-sh.de");
    //QApplication::setStyle(p_default_style);

    // Application-wide icon: set here (before any window - including
    // MainWindow itself - is constructed/shown) so that window managers on
    // Linux pick it up right from process start for the taskbar/dock/
    // alt-tab switcher, and so every top-level window (dialogs included)
    // gets it as their default, not just MainWindow. MainWindow::
    // initializeGUI() additionally calls this->setWindowIcon(...) on
    // itself further down - harmless/redundant there, kept for clarity -
    // but THIS call is what makes the icon available at the earliest
    // possible moment, which is what Linux desktop environments look at.
    app.setWindowIcon(QIcon(":/images/amiga_classic.png"));

    // GUI Language (I18n): MainWindow's constructor calls readSettings() as
    // its very first statement (before any widget/action exists), which
    // installs the saved translator (if any) right there - so every tr()
    // call during the initial GUI build-up already picks up the right
    // language. See MainWindow::readSettings()/applyGuiLanguage().
    MainWindow mainWin(cmdFileName);   // take first cmd argument to load a file
    QGuiApplication::setQuitOnLastWindowClosed(true);

    mainWin.show();
    return app.exec();
}
