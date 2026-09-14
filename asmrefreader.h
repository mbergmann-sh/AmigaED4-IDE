/****************************************************************************
**
** AsmRefReader - Help > Assembler > Registers / Addressing Modes /
** Mnemonics / Directives / Macros / Subroutines & Calling Conventions /
** vasm vs GNU-as (rev.159)
**
** The m68k Assembler counterpart to CppRefReader (see cpprefreader.h) -
** deliberately built on the exact same tree-plus-text-view pattern (and,
** one level further back, the same pattern AutodocReader established) so
** a user who already knows either of those windows immediately knows how
** to use this one too. Non-modal, single instance, every entry authored
** directly in asmrefreader.cpp/asmrefdata.cpp in both English and German.
**
** Content is written against vasm in its Motorola-syntax mode
** (vasmm68k_mot) as the reference assembler - matching AmigaED's own
** Prefs > Build > "vasm (Assembler mode)" - with a dedicated "vasm vs
** GNU-as" category covering where AmigaED's other supported assembler,
** GNU as (m68k-amigaos-as, part of the m68k-amigaos-gcc/g++ toolchain),
** differs.
**
** rev.159 ships full content for all seven categories, and adds the same
** editor "Lookup Assembler" token-lookup support (hasEntryForToken()/
** showEntryForToken(), see MainWindow::isKnownAsmRefToken()/
** actionLookupAsmRef()) that CppRefReader's "Lookup C/C++" already has -
** but see buildTokenIndex() in the .cpp for why the actual lookup table
** there is hand-maintained rather than auto-detected the way
** CppRefReader's is.
**
****************************************************************************/

#ifndef ASMREFREADER_H
#define ASMREFREADER_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QList>
#include <QHash>

class QTreeWidget;
class QTreeWidgetItem;
class QTextBrowser;
class QLineEdit;
class QPushButton;
class QCloseEvent;
class QUrl;

class AsmRefReader : public QDialog
{
    Q_OBJECT

public:
    // guiLanguage: "de" for German, anything else (normally "en") for
    // English - same "fixed at open time, never re-picked while the
    // window stays open" behaviour as CppRefReader/AutodocReader/the
    // Manual viewer; close and reopen the window to see the other
    // language.
    explicit AsmRefReader(const QString &guiLanguage, QWidget *parent = nullptr);

    // Expands the given top-level category ("Registers", "Addressing
    // Modes", "Mnemonics", "Directives", "Macros", "Subroutines &
    // Calling Conventions", or "vasm vs GNU-as" - always the English
    // key, regardless of the window's own display language) and selects
    // its first entry. Clears any active filter first. A category name
    // that doesn't match any group is simply a no-op.
    void showCategory(const QString &category);

    // true if 'token' (case-insensitive - see buildTokenIndex(); unlike
    // CppRefReader's case-sensitive C/C++ token lookup, case carries no
    // meaning for an m68k mnemonic, directive, or register name) is a
    // token this reference has an entry for.
    bool hasEntryForToken(const QString &token) const;

    // Expands and selects the entry for 'token' (case-insensitive), same
    // "clears any active filter first" behaviour as showCategory(). False
    // and a no-op if 'token' isn't a known token.
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
    // entry's own explanation (href "asmref:<id>" - see renderEntryHtml()
    // and the entry authoring in populateEntries()/asmrefdata.cpp).
    void onCrossRefLinkClicked(const QUrl &link);

private:
    // One reference entry - see CppRefEntry in cpprefreader.h for the
    // full rationale of each field (id/category/title/html carry exactly
    // the same meaning here).
    struct AsmRefEntry
    {
        QString id;
        QString category;
        QString title;
        QString html;
    };

    void populateEntries();

    void addEntry(const QString &id, const QString &category, const QString &title,
                  const QString &htmlEn, const QString &htmlDe);

    // One function per category, each appending its own entries via
    // addEntry() above - see asmrefdata.cpp.
    void populateRegisters();
    void populateAddressingModes();
    void populateMnemonics();
    void populateDirectives();
    void populateMacros();
    void populateSubroutines();
    void populateVasmVsGnuAs();

    void buildIdIndex();

    // Builds p_tokenToIndex from a hand-maintained table (see the .cpp) -
    // every mnemonic/directive spelling and register name this reference
    // covers, mapped (lower-cased) to the entry it should open. Unlike
    // CppRefReader::buildTokenIndex(), there's no auto-detection from a
    // single-identifier title regex: most Assembler entries here cover
    // SEVERAL related mnemonics under one compound title (e.g. "ADD /
    // ADDA / ADDI / ADDQ"), so which literal source tokens should resolve
    // to which entry has to be spelled out explicitly rather than derived
    // from the title text.
    void buildTokenIndex();

    // Builds the tree: one top-level group per category, always in the
    // fixed pedagogical order (Registers, Addressing Modes, Mnemonics,
    // Directives, Macros, Subroutines & Calling Conventions, vasm vs
    // GNU-as - see the .cpp), each containing its own entries sorted
    // alphabetically by title, except any entry listed in the .cpp's
    // pinnedFirstIds (e.g. the Addressing Modes overview table), which is
    // moved to the front of its group after that sort.
    void buildTree();

    bool selectEntryByIndex(int idx);

    void applyFilter(const QString &text);

    QString renderEntryHtml(int idx) const;

    QVector<AsmRefEntry> p_entries;
    QHash<QString, int> p_idToIndex;
    QHash<QString, int> p_tokenToIndex;   // lower-cased source token -> index into p_entries, see buildTokenIndex()
    QString p_lang;

    QLineEdit *p_filterEdit = nullptr;
    QPushButton *p_filterPrevBtn = nullptr;
    QPushButton *p_filterNextBtn = nullptr;
    QTreeWidget *p_tree = nullptr;
    QTextBrowser *p_textView = nullptr;
    QPushButton *p_openAllBtn = nullptr;
    QPushButton *p_closeAllBtn = nullptr;

    QList<QTreeWidgetItem *> p_currentMatches;
    int p_currentMatchIndex = -1;
};

#endif // ASMREFREADER_H
