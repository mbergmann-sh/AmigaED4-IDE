/****************************************************************************
**
** AutodocReader - Build > AutoDoc Reader... (rev.150)
**
** A non-modal, single-instance viewer for AmigaOS NDK AutoDocs (the
** plain-text *.doc files under the NDK's "Autodocs" drawer, e.g.
** exec.doc, dos.doc, graphics.doc, ... - one file per library, each
** holding every function of that library's own AutoDoc entry back to
** back). Lets the user browse and full-text-filter every documented
** NDK function without leaving AmigaED - inspired by (though not a
** pixel clone of) the classic Workbench tool MinAD.
**
** See the .cpp for how entries are recognized/parsed and how the
** library-name tree grouping is derived.
**
****************************************************************************/

#ifndef AUTODOCREADER_H
#define AUTODOCREADER_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QList>

class QTreeWidget;
class QTreeWidgetItem;
class QPlainTextEdit;
class QLineEdit;
class QLabel;
class QPushButton;
class QCloseEvent;

class AutodocReader : public QDialog
{
    Q_OBJECT

public:
    // autodocsDir: the folder configured in Prefs > Emulator > "AutoDocs
    // folder:" - scanned recursively for *.doc files right here in the
    // constructor, so the window is ready to browse as soon as it's shown.
    explicit AutodocReader(const QString &autodocsDir, QWidget *parent = nullptr);

    // Jumps straight to the entry for functionName - an exact,
    // case-insensitive match against the part of each entry's own
    // "library.name/FunctionName" identifier AFTER the last "/" (so
    // passing just "OpenWindow" finds "intuition.library/OpenWindow"
    // without also matching e.g. "OpenWindowTagList"). Clears any active
    // filter first, so the rest of the tree isn't left hidden behind an
    // unrelated leftover search term, then expands that entry's group
    // and selects it (which also populates the right-hand text view via
    // the existing onTreeSelectionChanged()). Used by MainWindow's
    // "Jump to Explaination" editor context-menu entry (rev.151) to jump
    // to the NDK/MUI function under the click, whether the reader was
    // just opened or was already open. Returns true if a match was
    // found and selected; false (nothing changed) if functionName is
    // empty or doesn't match any parsed entry.
    bool showFunction(const QString &functionName);

protected:
    // Persists the window's current size/position to QSettings (see
    // AMIGAED_SETTINGS_ORG/APP, "MISC/AutodocReaderGeometry") so the next
    // time the reader is opened it comes back at the size the user last
    // left it, rather than always resetting to the constructor's default.
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onFilterTextChanged(const QString &text);
    void onTreeSelectionChanged();
    void onExpandAll();
    void onCollapseAll();
    void onFilterNext();
    void onFilterPrev();

private:
    // One parsed AutoDoc function entry.
    struct AutodocEntry
    {
        QString functionName;   // e.g. "exec.library/Wait" - the entry's own marker-line identifier
        QString library;        // e.g. "exec" - the source file's own base name, used to group the tree
        QString sourceFile;     // absolute path of the *.doc file this came from
        QString fullText;       // the complete AutoDoc text (NAME/SYNOPSIS/FUNCTION/...), ready to display as-is
    };

    void parseAutodocsFolder(const QString &dir);
    void parseAutodocFile(const QString &filePath);
    void buildTree();
    void applyFilter(const QString &text);

    QVector<AutodocEntry> p_entries;
    int p_totalFiles = 0;   // distinct source *.doc files found - shown alongside the function count

    QLineEdit *p_filterEdit = nullptr;
    QPushButton *p_filterPrevBtn = nullptr;
    QPushButton *p_filterNextBtn = nullptr;
    QTreeWidget *p_tree = nullptr;
    QPlainTextEdit *p_textView = nullptr;
    QPushButton *p_openAllBtn = nullptr;
    QPushButton *p_closeAllBtn = nullptr;

    QList<QTreeWidgetItem *> p_currentMatches;   // leaf items matching the current filter, for Prev/Next
    int p_currentMatchIndex = -1;
};

#endif // AUTODOCREADER_H
