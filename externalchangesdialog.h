/****************************************************************************
**
** ExternalChangesDialog - shown when AmigaED regains application focus
** and finds that one or more currently open files have been modified on
** disk by something other than AmigaED itself (another editor, a build
** tool, version control, ...). Lists every affected file as a checkbox,
** pre-checked, so the user can pick which of them to reload from disk in
** one go - see MainWindow::checkForExternallyModifiedFiles().
**
** A file that also has unsaved in-editor changes is listed too, but
** pre-*unchecked* and flagged with a warning, since reloading it would
** silently throw away those edits - the user has to check it deliberately.
**
****************************************************************************/

#ifndef EXTERNALCHANGESDIALOG_H
#define EXTERNALCHANGESDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>

class QListWidget;

class ExternalChangesDialog : public QDialog
{
    Q_OBJECT

public:
    // 'changedFiles': every open file whose on-disk mtime no longer
    // matches AmigaED's stored baseline. 'dirtyFiles': the subset of
    // those that also have unsaved in-editor changes right now (used to
    // pre-uncheck + flag them - see buildList()).
    explicit ExternalChangesDialog(const QStringList &changedFiles,
                                    const QStringList &dirtyFiles,
                                    QWidget *parent = nullptr);

    // Files the user left checked when they closed the dialog with
    // "Reload Selected" - empty if they cancelled/dismissed instead.
    QStringList filesToReload() const;

private:
    void buildList(const QStringList &changedFiles, const QStringList &dirtyFiles);

    QListWidget *p_list = nullptr;
};

#endif // EXTERNALCHANGESDIALOG_H
