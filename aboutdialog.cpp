#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "version.h"

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);
    // Remove Close-, help-, size gadgets
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    // Overrides aboutdialog.ui's static "About AmigaED 4.0..." title with
    // one that includes the current revision (see version.h) - the .ui
    // file's own title is only ever seen in Designer, never at runtime.
    setWindowTitle(tr("About %1...").arg(tr(AMIGAED_VERSION_STRING)));
}

aboutDialog::~aboutDialog()
{
    delete ui;
}
