/****************************************************************************
**
** CppRefReader - Help > C/C++ > Keywords / Datatypes / Variables /
** Decisions / Control Structures / Operators / Functions (rev.157)
**
** A non-modal, single-instance C/C++ language reference browser, built
** on exactly the same tree-plus-text-view pattern as AutodocReader (see
** autodocreader.h) - a QTreeWidget of entries on the left, live-filterable,
** and a QTextBrowser on the right showing the selected entry's
** explanation, with clickable cross-reference links between related
** entries. Unlike AutodocReader, nothing here is parsed from an external
** folder: every entry is authored directly in cpprefreader.cpp
** (populateEntries()), in both English and German - see buildEntries()
** for the entry format.
**
** Entries are grouped into a small, fixed set of top-level categories
** (Keywords, Datatypes, Variables, Decisions, Control Structures,
** Operators, Functions), always shown in that same pedagogical order
** rather than alphabetically - see buildTree(). Each Help > C/C++ >
** <category> menu entry opens (or raises) this same single window and
** jumps straight to that category - see showCategory() and
** MainWindow::showCppRefReaderAndJumpTo().
**
****************************************************************************/

#ifndef CPPREFREADER_H
#define CPPREFREADER_H

#include <QDialog>
#include <QHash>
#include <QList>
#include <QString>
#include <QVector>

class QTreeWidget;
class QTreeWidgetItem;
class QTextBrowser;
class QLineEdit;
class QPushButton;
class QCloseEvent;
class QUrl;

class CppRefReader : public QDialog
{
    Q_OBJECT

public:
    // guiLanguage: "de" for German, anything else (normally "en") for
    // English - MainWindow's own p_guiLanguage, passed straight through
    // at construction time. Every entry's explanation is picked in this
    // language once, here, and never re-picked afterwards even if the
    // app's own GUI language is switched while this window stays open -
    // same "fixed at open time" behaviour as the Help > Manual viewer
    // and the AutoDoc Reader (neither of which live-retranslates its own
    // content either); close and reopen the window to see the other
    // language.
    explicit CppRefReader(const QString &guiLanguage, QWidget *parent = nullptr);

    // Expands the given top-level category ("Keywords", "Datatypes",
    // "Variables", "Decisions", "Control Structures", "Operators", or
    // "Functions" - always the English key, regardless of the window's
    // own display language, so callers never have to care which
    // language is active) and selects its first entry, which also
    // populates the text view via onTreeSelectionChanged(). Clears any
    // active filter first, exactly like AutodocReader::showFunction(),
    // so the rest of the tree isn't left hidden behind an unrelated
    // leftover search term. A category name that doesn't match any
    // group, or one with no entries at all, is simply a no-op.
    void showCategory(const QString &category);

    // rev.158: editor "Lookup C/C++" support (MainWindow's
    // showCustomContextMenue()/actionLookupCppRef()). true if token
    // (case-sensitive - see buildTokenIndex()) is a literal C/C++ keyword
    // or datatype spelling this reference has an entry for, WITHOUT
    // changing anything on screen - lets MainWindow decide whether to
    // even offer the context-menu entry for the word under the click, the
    // same role isKnownAutodocFunction() plays for "Jump to Explanation".
    bool hasEntryForToken(const QString &token) const;

    // Jumps straight to the entry for token (as hasEntryForToken() would
    // find it) - same expand/select/scroll as onCrossRefLinkClicked(),
    // just keyed by a literal token instead of a "cppref:<id>" link.
    // Returns false (and leaves the current selection alone) if token
    // isn't in the index.
    bool showEntryForToken(const QString &token);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onFilterTextChanged(const QString &text);
    void onTreeSelectionChanged();
    void onExpandAll();
    void onCollapseAll();
    void onFilterNext();
    void onFilterPrev();

    // Handles a click on a "See also" cross-reference link inside an
    // entry's own explanation (href "cppref:<id>" - see renderEntryHtml()
    // and the entry authoring in populateEntries()). Jumps straight to
    // that entry, same tree expand/select/scroll, same filter-clearing
    // as showCategory() and showFunction() in AutodocReader.
    void onCrossRefLinkClicked(const QUrl &link);

private:
    // One reference entry - a single keyword, datatype, operator, and so
    // on. id is a stable, language-independent key used for cross-
    // reference links (e.g. "kw_if", "dt_long") - never shown to the
    // user. title is the entry's own name as it appears in code (e.g.
    // "if", "LONG", "switch...case") - deliberately NOT translated,
    // since a keyword or type name is spelled the same in either
    // language. category is one of the fixed keys buildTree() groups by
    // (see showCategory()). html is the complete, ready-to-display
    // explanation for that entry (example code and "See also" links
    // included) in whichever language p_lang resolved to when
    // populateEntries() built it - see the .cpp's addEntry() helper,
    // which is handed both an English and a German version and only
    // ever keeps the one matching p_lang, so nothing downstream of
    // populateEntries() needs to branch on language again.
    struct CppRefEntry
    {
        QString id;
        QString category;
        QString title;
        QString html;
    };

    // Fills p_entries with every authored entry (Keywords, Datatypes,
    // ... - see cpprefdata.cpp for the current coverage) by calling
    // populateKeywords()/populateDatatypes()/... in turn, each of which
    // calls addEntry() below for every entry in its own category.
    void populateEntries();

    // Appends one entry to p_entries, keeping only htmlEn or htmlDe -
    // whichever p_lang resolved to - see the CppRefEntry comment above.
    // The one and only place that ever reads p_lang to pick a language;
    // every populateXxx() function in cpprefdata.cpp just calls this
    // once per entry and never branches on language itself.
    void addEntry(const QString &id,
                  const QString &category,
                  const QString &title,
                  const QString &htmlEn,
                  const QString &htmlDe);

    // One function per category, each appending its own entries via
    // addEntry() above - see cpprefdata.cpp. Split out one function per
    // category (rather than one giant populateEntries()) purely to keep
    // each one a manageable size to read/maintain as more categories are
    // authored in future revisions.
    void populateKeywords();
    void populateDatatypes();
    void populateVariables();
    void populateDecisions();
    void populateControlStructures();
    void populateOperators();
    void populateFunctions();

    // Populates p_idToIndex from the just-built p_entries (called once
    // from the constructor, right after populateEntries()) - a
    // language-independent id -> entry-index map used by
    // renderEntryHtml()/linkifyCrossRefs() to turn a "See also" mention
    // into a real link, and by onCrossRefLinkClicked() to resolve one
    // back to an entry, without a linear search either way.
    void buildIdIndex();

    // Populates p_tokenToIndex from the just-built p_entries (called once
    // from the constructor, right after buildIdIndex()) - backs
    // hasEntryForToken()/showEntryForToken() (rev.158, the editor's
    // "Lookup C/C++" context-menu entry). Keyed by the literal, CASE-
    // SENSITIVE spelling of a code token (so "long" and "LONG" resolve to
    // their own distinct entries - the C keyword vs. the Amiga NDK
    // typedef), built from every entry whose own title already IS a
    // single clean identifier (covers every Keywords/Datatypes entry that
    // could plausibly be the word under an editor cursor), plus a small
    // fixed list of extra spellings for the few entries whose display
    // title isn't literally one identifier (see the .cpp). Entries in
    // Variables/Decisions/Control Structures/Operators/Functions mostly
    // have descriptive, multi-word titles rather than literal code
    // tokens, so they're simply not reachable this way - browsing the
    // tree, or a "See also" link, still gets you there.
    void buildTokenIndex();

    // Shared by onCrossRefLinkClicked() and showEntryForToken(): expands
    // the entry's group, selects it and scrolls it into view. Clears any
    // active filter first, exactly like showCategory(). Returns false
    // (a no-op) for an out-of-range index or one that isn't actually in
    // the tree for some reason - shouldn't happen, but this is called
    // from user-triggered code paths (a clicked link, an editor word),
    // so it fails quietly rather than crashing.
    bool selectEntryByIndex(int idx);

    // Builds the tree: one top-level group per category, always in the
    // fixed pedagogical order (Keywords, Datatypes, Variables,
    // Decisions, Control Structures, Operators, Functions - see the
    // .cpp), each containing its own entries sorted alphabetically by
    // title.
    void buildTree();

    void applyFilter(const QString &text);

    // Renders p_entries[idx]'s already-language-picked HTML for
    // p_textView, prefixed with a small title heading - the stored HTML
    // itself is trusted, hand-authored markup (not user input), so no
    // escaping happens here; only the heading built from title/category
    // needs it.
    QString renderEntryHtml(int idx) const;

    QVector<CppRefEntry> p_entries;
    QHash<QString, int> p_idToIndex; // id -> index into p_entries, see buildIdIndex()
    QHash<QString, int>
        p_tokenToIndex; // literal code token -> index into p_entries, see buildTokenIndex()
    QString p_lang;     // "de" or "en" - see the constructor

    QLineEdit *p_filterEdit = nullptr;
    QPushButton *p_filterPrevBtn = nullptr;
    QPushButton *p_filterNextBtn = nullptr;
    QTreeWidget *p_tree = nullptr;
    QTextBrowser *p_textView = nullptr;
    QPushButton *p_openAllBtn = nullptr;
    QPushButton *p_closeAllBtn = nullptr;

    QList<QTreeWidgetItem *> p_currentMatches; // leaf items matching the current filter, for Prev/Next
    int p_currentMatchIndex = -1;
};

#endif // CPPREFREADER_H
