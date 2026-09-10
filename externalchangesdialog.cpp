#include "externalchangesdialog.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFileInfo>
#include <QFont>

ExternalChangesDialog::ExternalChangesDialog(const QStringList &changedFiles,
                                               const QStringList &dirtyFiles,
                                               QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Files Changed on Disk"));
    setModal(true);
    resize(520, 320);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *intro = new QLabel(
        tr("The following open file(s) have been changed outside AmigaED. "
           "Choose which ones to reload from disk - reloading discards "
           "any in-editor content for that file and replaces it with "
           "what's currently on disk."),
        this);
    intro->setWordWrap(true);
    layout->addWidget(intro);

    p_list = new QListWidget(this);
    layout->addWidget(p_list, 1);

    buildList(changedFiles, dirtyFiles);

    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    QPushButton *reloadBtn = buttons->addButton(tr("Reload Selected"), QDialogButtonBox::AcceptRole);
    buttons->addButton(tr("Dismiss"), QDialogButtonBox::RejectRole);
    reloadBtn->setDefault(true);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

//
// Populates the list: one checkable row per changed file, pre-checked -
// except a file that's also listed in 'dirtyFiles' (unsaved in-editor
// changes right now), which is pre-*unchecked* and gets an extra warning
// line so reloading it can never happen without the user deliberately
// re-checking it and accepting the loss of those edits.
//
void ExternalChangesDialog::buildList(const QStringList &changedFiles, const QStringList &dirtyFiles)
{
    for (const QString &fileName : changedFiles)
    {
        bool isDirty = dirtyFiles.contains(fileName);

        QString label = QFileInfo(fileName).fileName();
        if (isDirty)
            label += tr("  (has unsaved changes - reloading will discard them)");

        QListWidgetItem *item = new QListWidgetItem(label, p_list);
        item->setToolTip(fileName);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(isDirty ? Qt::Unchecked : Qt::Checked);
        item->setData(Qt::UserRole, fileName);

        if (isDirty)
        {
            QFont f = item->font();
            f.setItalic(true);
            item->setFont(f);
        }
    }
}

QStringList ExternalChangesDialog::filesToReload() const
{
    QStringList result;

    for (int i = 0; i < p_list->count(); ++i)
    {
        QListWidgetItem *item = p_list->item(i);
        if (item->checkState() == Qt::Checked)
            result.append(item->data(Qt::UserRole).toString());
    }

    return result;
}
