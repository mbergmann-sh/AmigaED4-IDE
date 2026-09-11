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
#include <QSet>
#include <QHash>

class QTreeWidget;
class QTreeWidgetItem;
class QTextBrowser;
class QLineEdit;
class QLabel;
class QPushButton;
class QCloseEvent;
class QUrl;

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
    //
    // The same short function name can legitimately exist in more than
    // one library's docs (e.g. "CMD_WRITE" in audio.device,
    // carddisk.device, trackdisk.device, ...) - when more than one entry
    // matches, preferredLibrary (rev.154; e.g. "carddisk") picks the one
    // belonging to that library if there is one, falling back to
    // whichever match sorts first in the tree otherwise (the previous,
    // context-free behaviour - what a caller with no library context of
    // its own, like "Jump to Explanation", still gets by leaving this
    // empty). See onSeeAlsoLinkClicked() for the caller that actually
    // needs this: a SEE ALSO reference is, by AmigaOS AutoDoc convention,
    // read relative to the entry it appears in.
    bool showFunction(const QString &functionName, const QString &preferredLibrary = QString());

    // Lightweight counterpart to the constructor's parseAutodocsFolder():
    // scans autodocsDir for every documented function's short name (the
    // part of its "library.name/FunctionName" marker line after the last
    // "/", lower-cased for case-insensitive lookups), WITHOUT keeping any
    // entry body text or building any widget - just the name set, so
    // callers can cheaply check "is this word an actual NDK/MUI function"
    // without constructing (let alone showing) a whole AutodocReader.
    // Used by MainWindow::isKnownAutodocFunction() to decide whether the
    // editor context menu's "Jump to Explanation" entry should even
    // appear for the word under the click - see showCustomContextMenue().
    // Returns an empty set if autodocsDir doesn't exist/isn't configured;
    // callers are expected to cache the result themselves (this rescans
    // the folder from scratch on every call).
    static QSet<QString> collectFunctionNames(const QString &autodocsDir);

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

    // Handles a click on a "SEE ALSO" cross-reference link in the text
    // view (rev.153, href "adoc:<shortName>" - see renderEntryHtml()/
    // linkifySeeAlsoLine()). Jumps to that entry exactly like
    // showFunction() does for "Jump to Explanation" - same tree
    // expand/select/scroll, same filter-clearing.
    void onSeeAlsoLinkClicked(const QUrl &link);

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

    // Populates p_nameToIndex from the just-parsed p_entries (called once
    // from the constructor, right after parseAutodocsFolder()) - a
    // lower-cased "short function name" -> first-matching-entry-index map,
    // used by linkifySeeAlsoLine() to decide (cheaply, no per-click
    // re-parsing) whether a SEE ALSO token is a real cross-reference.
    void buildNameIndex();

    // Renders one entry's fullText as HTML for p_textView (a QTextBrowser),
    // wrapped in a <pre> so its column alignment survives unchanged - see
    // the .cpp for why only its "SEE ALSO" section gets linkified.
    QString renderEntryHtml(int idx) const;

    // Turns every recognized function name in one already-HTML-escaped
    // SEE ALSO line into a clickable "adoc:<shortName>" link - see the
    // .cpp for the exact matching rules (bare or "library/Name" form,
    // trailing sentence punctuation).
    QString linkifySeeAlsoLine(const QString &escapedLine) const;

    QVector<AutodocEntry> p_entries;
    int p_totalFiles = 0;   // distinct source *.doc files found - shown alongside the function count
    QHash<QString, int> p_nameToIndex;   // lower-cased short function name -> index into p_entries, see buildNameIndex()

    QLineEdit *p_filterEdit = nullptr;
    QPushButton *p_filterPrevBtn = nullptr;
    QPushButton *p_filterNextBtn = nullptr;
    QTreeWidget *p_tree = nullptr;
    QTextBrowser *p_textView = nullptr;
    QPushButton *p_openAllBtn = nullptr;
    QPushButton *p_closeAllBtn = nullptr;

    QList<QTreeWidgetItem *> p_currentMatches;   // leaf items matching the current filter, for Prev/Next
    int p_currentMatchIndex = -1;
};

#endif // AUTODOCREADER_H
