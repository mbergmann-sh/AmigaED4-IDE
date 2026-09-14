#include "asmrefreader.h"
#include "version.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTreeWidget>
#include <QTextBrowser>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QCloseEvent>
#include <QSizeGrip>
#include <QUrl>
#include <QRegularExpression>
#include <QSet>

AsmRefReader::AsmRefReader(const QString &guiLanguage, QWidget *parent)
    : QDialog(parent)
    , p_lang(guiLanguage == QStringLiteral("de") ? QStringLiteral("de") : QStringLiteral("en"))
{
    // Non-modal by construction (only ever show()n, never exec()d - see
    // MainWindow::showAsmRefReaderAndJumpTo()) and WA_DeleteOnClose so the
    // "only one at a time" tracking in MainWindow (p_asmRefReader) can
    // rely on the window's destroyed() signal to know when it's gone -
    // same pattern as CppRefReader/AutodocReader/the Manual viewer.
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlag(Qt::WindowMinMaxButtonsHint, true);

    QSettings geometrySettings(AMIGAED_SETTINGS_ORG, AMIGAED_SETTINGS_APP);
    const QByteArray savedGeometry = geometrySettings.value(QStringLiteral("MISC/AsmRefReaderGeometry")).toByteArray();
    if (savedGeometry.isEmpty() || !restoreGeometry(savedGeometry))
        resize(950, 650);

    setWindowTitle((p_lang == QStringLiteral("de"))
                   ? QStringLiteral("AmigaED Assembler-Referenz")
                   : QStringLiteral("AmigaED Assembler Reference"));

    populateEntries();
    buildIdIndex();
    buildTokenIndex();

    // --- left side: filter row, entry tree, Open/Close All row ---
    // Deliberately the same three widgets in the same arrangement as
    // CppRefReader/AutodocReader - a user who already knows either of
    // those windows immediately knows how to use this one too.
    p_filterEdit = new QLineEdit(this);
    p_filterEdit->setPlaceholderText(tr("Filter..."));
    p_filterEdit->setClearButtonEnabled(true);

    p_filterPrevBtn = new QPushButton(QStringLiteral("◀"), this);   // <
    p_filterPrevBtn->setToolTip(tr("Previous match"));
    p_filterPrevBtn->setMaximumWidth(28);
    p_filterPrevBtn->setEnabled(false);

    p_filterNextBtn = new QPushButton(QStringLiteral("▶"), this);   // >
    p_filterNextBtn->setToolTip(tr("Next match"));
    p_filterNextBtn->setMaximumWidth(28);
    p_filterNextBtn->setEnabled(false);

    QHBoxLayout *filterRow = new QHBoxLayout();
    filterRow->addWidget(new QLabel(tr("Filter:"), this));
    filterRow->addWidget(p_filterEdit, 1);
    filterRow->addWidget(p_filterPrevBtn);
    filterRow->addWidget(p_filterNextBtn);

    p_tree = new QTreeWidget(this);
    p_tree->setHeaderHidden(true);
    p_tree->setColumnCount(1);
    p_tree->setUniformRowHeights(true);

    p_openAllBtn = new QPushButton(tr("Open All"), this);
    p_closeAllBtn = new QPushButton(tr("Close All"), this);
    QHBoxLayout *treeButtonsRow = new QHBoxLayout();
    treeButtonsRow->addWidget(p_openAllBtn);
    treeButtonsRow->addWidget(p_closeAllBtn);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addLayout(filterRow);
    leftLayout->addWidget(p_tree, 1);
    leftLayout->addLayout(treeButtonsRow);

    QWidget *leftWidget = new QWidget(this);
    leftWidget->setLayout(leftLayout);

    // --- right side: the selected entry's explanation ---
    p_textView = new QTextBrowser(this);
    p_textView->setReadOnly(true);
    p_textView->setOpenExternalLinks(false);
    p_textView->setOpenLinks(false);   // "asmref:<id>" links, see onCrossRefLinkClicked()

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(leftWidget);
    splitter->addWidget(p_textView);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setSizes({ 320, 630 });

    // Bottom-right resize grip - same reasoning as CppRefReader/
    // AutodocReader's own.
    QSizeGrip *sizeGrip = new QSizeGrip(this);
    QHBoxLayout *sizeGripRow = new QHBoxLayout();
    sizeGripRow->setContentsMargins(0, 0, 0, 0);
    sizeGripRow->addStretch(1);
    sizeGripRow->addWidget(sizeGrip);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(6, 6, 6, 6);
    mainLayout->addWidget(splitter);
    mainLayout->addLayout(sizeGripRow);
    setLayout(mainLayout);

    buildTree();
    applyFilter(QString());

    connect(p_filterEdit, &QLineEdit::textChanged, this, &AsmRefReader::onFilterTextChanged);
    connect(p_filterEdit, &QLineEdit::returnPressed, this, &AsmRefReader::onFilterNext);
    connect(p_tree, &QTreeWidget::currentItemChanged, this, &AsmRefReader::onTreeSelectionChanged);
    connect(p_openAllBtn, &QPushButton::clicked, this, &AsmRefReader::onExpandAll);
    connect(p_closeAllBtn, &QPushButton::clicked, this, &AsmRefReader::onCollapseAll);
    connect(p_filterPrevBtn, &QPushButton::clicked, this, &AsmRefReader::onFilterPrev);
    connect(p_filterNextBtn, &QPushButton::clicked, this, &AsmRefReader::onFilterNext);
    connect(p_textView, &QTextBrowser::anchorClicked, this, &AsmRefReader::onCrossRefLinkClicked);

    p_filterEdit->setFocus();
}

void AsmRefReader::showCategory(const QString &category)
{
    if (!p_filterEdit->text().isEmpty())
        p_filterEdit->clear();

    for (int gi = 0; gi < p_tree->topLevelItemCount(); ++gi)
    {
        QTreeWidgetItem *groupItem = p_tree->topLevelItem(gi);
        if (groupItem->data(0, Qt::UserRole + 1).toString() != category)
            continue;

        groupItem->setExpanded(true);
        p_tree->scrollToItem(groupItem);

        if (groupItem->childCount() > 0)
        {
            QTreeWidgetItem *firstChild = groupItem->child(0);
            p_tree->setCurrentItem(firstChild);
            p_tree->scrollToItem(firstChild);
        }
        return;
    }
}

void AsmRefReader::addEntry(const QString &id, const QString &category, const QString &title,
                             const QString &htmlEn, const QString &htmlDe)
{
    AsmRefEntry e;
    e.id = id;
    e.category = category;
    e.title = title;
    e.html = (p_lang == QStringLiteral("de")) ? htmlDe : htmlEn;
    p_entries.append(e);
}

//
// Every authored entry, in order - see asmrefdata.cpp for all seven
// populateXxx() functions (rev.159 ships full content for every
// category).
//
void AsmRefReader::populateEntries()
{
    populateRegisters();
    populateAddressingModes();
    populateMnemonics();
    populateDirectives();
    populateMacros();
    populateSubroutines();
    populateVasmVsGnuAs();
}

void AsmRefReader::buildIdIndex()
{
    p_idToIndex.clear();
    for (int idx = 0; idx < p_entries.size(); ++idx)
        p_idToIndex.insert(p_entries.at(idx).id, idx);
}

//
// Editor "Lookup Assembler" support (rev.159, see
// MainWindow::isKnownAsmRefToken()/actionLookupAsmRef()) - a
// hand-maintained table mapping every mnemonic/directive spelling and
// register name this reference covers to the entry it should open,
// lower-cased on both sides of the lookup since case carries no meaning
// in m68k assembly (unlike CppRefReader's case-sensitive C/C++ tokens,
// where "LONG" and "long" are deliberately different entries).
//
// This can't reuse CppRefReader's auto-detection-from-a-single-identifier-
// title trick: most entries here cover SEVERAL related mnemonics under
// one compound title (e.g. mn_add is titled "ADD / ADDA / ADDI / ADDQ"),
// so there's no single title string to regex-match against a clicked
// word - each lookupable spelling has to be listed explicitly instead.
// Conceptual/topic entries (Addressing Modes, Subroutines & Calling
// Conventions, vasm vs GNU-as) aren't literal source tokens a user would
// ever click on, so they're deliberately not in this table at all -
// exactly like CppRefReader doesn't register its Decisions/Control
// Structures entries by anything other than the handful of their titles
// that ARE single keywords.
//
void AsmRefReader::buildTokenIndex()
{
    p_tokenToIndex.clear();

    struct TokenAlias { const char *token; const char *id; };
    static const TokenAlias aliases[] = {
        // --- Registers ---
        { "d0", "reg_data_registers" }, { "d1", "reg_data_registers" }, { "d2", "reg_data_registers" },
        { "d3", "reg_data_registers" }, { "d4", "reg_data_registers" }, { "d5", "reg_data_registers" },
        { "d6", "reg_data_registers" }, { "d7", "reg_data_registers" },
        { "a0", "reg_address_registers" }, { "a1", "reg_address_registers" }, { "a2", "reg_address_registers" },
        { "a3", "reg_address_registers" }, { "a4", "reg_address_registers" }, { "a5", "reg_address_registers" },
        { "a6", "reg_address_registers" },
        { "a7", "reg_stack_pointer" }, { "sp", "reg_stack_pointer" }, { "usp", "reg_stack_pointer" }, { "ssp", "reg_stack_pointer" },
        { "pc", "reg_pc" },
        { "sr", "reg_sr_ccr" }, { "ccr", "reg_sr_ccr" },
        { "vbr", "reg_cpu_variants" }, { "cacr", "reg_cpu_variants" }, { "caar", "reg_cpu_variants" },
        { "fpcr", "reg_cpu_variants" }, { "fpsr", "reg_cpu_variants" }, { "fpiar", "reg_cpu_variants" },
        { "fp0", "reg_cpu_variants" }, { "fp1", "reg_cpu_variants" }, { "fp2", "reg_cpu_variants" }, { "fp3", "reg_cpu_variants" },
        { "fp4", "reg_cpu_variants" }, { "fp5", "reg_cpu_variants" }, { "fp6", "reg_cpu_variants" }, { "fp7", "reg_cpu_variants" },

        // --- Mnemonics ---
        { "move", "mn_move" }, { "movea", "mn_move" }, { "moveq", "mn_move" },
        { "lea", "mn_lea" },
        { "pea", "mn_pea" },
        { "movem", "mn_movem" },
        { "exg", "mn_exg" },
        { "swap", "mn_swap" },
        { "clr", "mn_clr" },
        { "ext", "mn_ext" }, { "extb", "mn_ext" },
        { "add", "mn_add" }, { "adda", "mn_add" }, { "addi", "mn_add" }, { "addq", "mn_add" },
        { "sub", "mn_sub" }, { "suba", "mn_sub" }, { "subi", "mn_sub" }, { "subq", "mn_sub" },
        { "addx", "mn_addx_subx" }, { "subx", "mn_addx_subx" },
        { "muls", "mn_muls_mulu" }, { "mulu", "mn_muls_mulu" },
        { "divs", "mn_divs_divu" }, { "divu", "mn_divs_divu" },
        { "neg", "mn_neg" }, { "negx", "mn_neg" },
        { "cmp", "mn_cmp" }, { "cmpa", "mn_cmp" }, { "cmpi", "mn_cmp" }, { "cmpm", "mn_cmp" },
        { "tst", "mn_tst" },
        { "and", "mn_and_or_eor" }, { "or", "mn_and_or_eor" }, { "eor", "mn_and_or_eor" },
        { "not", "mn_not" },
        { "asl", "mn_asl_asr" }, { "asr", "mn_asl_asr" },
        { "lsl", "mn_lsl_lsr" }, { "lsr", "mn_lsl_lsr" },
        { "rol", "mn_rol_ror" }, { "ror", "mn_rol_ror" },
        { "roxl", "mn_roxl_roxr" }, { "roxr", "mn_roxl_roxr" },
        { "btst", "mn_bit_manipulation" }, { "bset", "mn_bit_manipulation" }, { "bclr", "mn_bit_manipulation" }, { "bchg", "mn_bit_manipulation" },
        { "bra", "mn_bra_bcc" }, { "bcc", "mn_bra_bcc" }, { "bcs", "mn_bra_bcc" }, { "beq", "mn_bra_bcc" }, { "bne", "mn_bra_bcc" },
        { "bge", "mn_bra_bcc" }, { "bgt", "mn_bra_bcc" }, { "ble", "mn_bra_bcc" }, { "blt", "mn_bra_bcc" },
        { "bhi", "mn_bra_bcc" }, { "bls", "mn_bra_bcc" }, { "bpl", "mn_bra_bcc" }, { "bmi", "mn_bra_bcc" },
        { "bvc", "mn_bra_bcc" }, { "bvs", "mn_bra_bcc" },
        { "bsr", "mn_bsr_jsr" }, { "jsr", "mn_bsr_jsr" },
        { "jmp", "mn_jmp" },
        { "rts", "mn_rts_rte" }, { "rte", "mn_rts_rte" },
        { "dbcc", "mn_dbcc" }, { "dbra", "mn_dbcc" }, { "dbf", "mn_dbcc" }, { "dbt", "mn_dbcc" },
        { "dbeq", "mn_dbcc" }, { "dbne", "mn_dbcc" },
        { "scc", "mn_scc" }, { "seq", "mn_scc" }, { "sne", "mn_scc" }, { "st", "mn_scc" }, { "sf", "mn_scc" },
        { "link", "mn_link_unlk" }, { "unlk", "mn_link_unlk" },
        { "nop", "mn_nop" },
        { "trap", "mn_trap" }, { "trapv", "mn_trap" },
        { "chk", "mn_chk" },

        // --- Directives ---
        { "section", "dir_section" },
        { "dc", "dir_dc" },
        { "ds", "dir_ds" },
        { "equ", "dir_equ" },
        { "xdef", "dir_xdef" },
        { "xref", "dir_xref" },
        { "include", "dir_include" },
        { "even", "dir_even" },
        { "org", "dir_org" },
        { "end", "dir_end" },
        { "ifd", "dir_ifdef_endif" }, { "ifne", "dir_ifdef_endif" }, { "ifeq", "dir_ifdef_endif" }, { "endc", "dir_ifdef_endif" },
        { "rsreset", "dir_rsreset_rs" }, { "rs", "dir_rsreset_rs" },

        // --- Macros ---
        { "macro", "mac_macro_endm" }, { "endm", "mac_macro_endm" },
        { "rept", "mac_rept_endr" }, { "endr", "mac_rept_endr" },
    };

    for (const auto &alias : aliases)
    {
        const int entryIdx = p_idToIndex.value(QString::fromLatin1(alias.id), -1);
        if (entryIdx >= 0)
            p_tokenToIndex.insert(QString::fromLatin1(alias.token), entryIdx);
    }
}

bool AsmRefReader::hasEntryForToken(const QString &token) const
{
    return !token.isEmpty() && p_tokenToIndex.contains(token.toLower());
}

bool AsmRefReader::showEntryForToken(const QString &token)
{
    return selectEntryByIndex(p_tokenToIndex.value(token.toLower(), -1));
}

//
// Fixed, pedagogical top-level order (NOT alphabetical).
//
void AsmRefReader::buildTree()
{
    p_tree->clear();

    static const QStringList categoryOrder = {
        QStringLiteral("Registers"),
        QStringLiteral("Addressing Modes"),
        QStringLiteral("Mnemonics"),
        QStringLiteral("Directives"),
        QStringLiteral("Macros"),
        QStringLiteral("Subroutines & Calling Conventions"),
        QStringLiteral("vasm vs GNU-as"),
    };

    // tr() here deliberately uses the same source strings as the Help >
    // Assembler submenu actions in mainwindow.cpp, so translating one
    // translates the other too - with one unavoidable exception:
    // "Subroutines & Calling Conventions" needs a literal "&&" over in
    // mainwindow.cpp's QAction::setText() (Qt reads a single "&" there as
    // an accelerator marker), so that one category has its own separate,
    // separately-translated .ts entry.
    QHash<QString, QString> displayName;
    displayName.insert(QStringLiteral("Registers"), tr("Registers"));
    displayName.insert(QStringLiteral("Addressing Modes"), tr("Addressing Modes"));
    displayName.insert(QStringLiteral("Mnemonics"), tr("Mnemonics"));
    displayName.insert(QStringLiteral("Directives"), tr("Directives"));
    displayName.insert(QStringLiteral("Macros"), tr("Macros"));
    displayName.insert(QStringLiteral("Subroutines & Calling Conventions"), tr("Subroutines & Calling Conventions"));
    displayName.insert(QStringLiteral("vasm vs GNU-as"), tr("vasm vs GNU-as"));

    QHash<QString, QTreeWidgetItem *> groupItems;
    for (const QString &category : categoryOrder)
    {
        QTreeWidgetItem *groupItem = new QTreeWidgetItem(p_tree, QStringList{ displayName.value(category, category) });
        groupItem->setFlags(groupItem->flags() & ~Qt::ItemIsSelectable);
        groupItem->setData(0, Qt::UserRole + 1, category);
        groupItems.insert(category, groupItem);
    }

    for (int idx = 0; idx < p_entries.size(); ++idx)
    {
        const AsmRefEntry &e = p_entries.at(idx);
        QTreeWidgetItem *groupItem = groupItems.value(e.category, nullptr);
        if (!groupItem)
            continue;

        QTreeWidgetItem *entryItem = new QTreeWidgetItem(groupItem, QStringList{ e.title });
        entryItem->setData(0, Qt::UserRole, idx);
    }

    // Sort only WITHIN each group - see CppRefReader::buildTree()'s own
    // comment for why p_tree->sortItems() itself is never used here.
    for (int gi = 0; gi < p_tree->topLevelItemCount(); ++gi)
        p_tree->topLevelItem(gi)->sortChildren(0, Qt::AscendingOrder);

    // Pin the Addressing Modes and Mnemonics overview tables to the front
    // of their groups, same mechanism/rationale as CppRefReader's own
    // dt_overview - see that class's buildTree() comment.
    static const QSet<QString> pinnedFirstIds = {
        QStringLiteral("am_overview"),
        QStringLiteral("mn_overview"),
    };
    for (int gi = 0; gi < p_tree->topLevelItemCount(); ++gi)
    {
        QTreeWidgetItem *groupItem = p_tree->topLevelItem(gi);
        for (int ci = 0; ci < groupItem->childCount(); ++ci)
        {
            QTreeWidgetItem *entryItem = groupItem->child(ci);
            const int idx = entryItem->data(0, Qt::UserRole).toInt();
            if (idx < 0 || idx >= p_entries.size())
                continue;
            if (pinnedFirstIds.contains(p_entries.at(idx).id) && ci != 0)
            {
                groupItem->takeChild(ci);
                groupItem->insertChild(0, entryItem);
            }
        }
    }
}

bool AsmRefReader::selectEntryByIndex(int idx)
{
    if (idx < 0 || idx >= p_entries.size())
        return false;

    if (!p_filterEdit->text().isEmpty())
        p_filterEdit->clear();

    for (int gi = 0; gi < p_tree->topLevelItemCount(); ++gi)
    {
        QTreeWidgetItem *groupItem = p_tree->topLevelItem(gi);
        for (int ci = 0; ci < groupItem->childCount(); ++ci)
        {
            QTreeWidgetItem *entryItem = groupItem->child(ci);
            if (entryItem->data(0, Qt::UserRole).toInt() != idx)
                continue;

            groupItem->setExpanded(true);
            p_tree->setCurrentItem(entryItem);
            p_tree->scrollToItem(entryItem);
            return true;
        }
    }
    return false;
}

void AsmRefReader::applyFilter(const QString &textRaw)
{
    const QString text = textRaw.trimmed();

    int visibleEntries = 0;
    p_currentMatches.clear();

    for (int gi = 0; gi < p_tree->topLevelItemCount(); ++gi)
    {
        QTreeWidgetItem *groupItem = p_tree->topLevelItem(gi);
        bool anyVisibleChild = false;

        for (int ci = 0; ci < groupItem->childCount(); ++ci)
        {
            QTreeWidgetItem *entryItem = groupItem->child(ci);
            const bool matches = text.isEmpty() || entryItem->text(0).contains(text, Qt::CaseInsensitive);
            entryItem->setHidden(!matches);

            if (matches)
            {
                anyVisibleChild = true;
                ++visibleEntries;
                p_currentMatches << entryItem;
            }
        }

        groupItem->setHidden(!anyVisibleChild);
        if (!text.isEmpty() && anyVisibleChild)
            groupItem->setExpanded(true);
    }

    p_currentMatchIndex = -1;

    const bool haveFilter = !text.isEmpty();
    p_filterPrevBtn->setEnabled(haveFilter && !p_currentMatches.isEmpty());
    p_filterNextBtn->setEnabled(haveFilter && !p_currentMatches.isEmpty());

    const QString baseTitle = (p_lang == QStringLiteral("de"))
            ? QStringLiteral("AmigaED Assembler-Referenz")
            : QStringLiteral("AmigaED Assembler Reference");

    if (haveFilter)
    {
        setWindowTitle(baseTitle + QStringLiteral(" - ")
                        + tr("Filter showing %1/%2 entries").arg(visibleEntries).arg(p_entries.size()));
    }
    else
    {
        setWindowTitle(baseTitle + QStringLiteral(" - ") + tr("%1 entries").arg(p_entries.size()));
    }
}

void AsmRefReader::onFilterTextChanged(const QString &text)
{
    applyFilter(text);
}

void AsmRefReader::onTreeSelectionChanged()
{
    QTreeWidgetItem *item = p_tree->currentItem();
    if (!item || !item->parent())
    {
        p_textView->clear();
        return;
    }

    const int idx = item->data(0, Qt::UserRole).toInt();
    if (idx >= 0 && idx < p_entries.size())
        p_textView->setHtml(renderEntryHtml(idx));
}

QString AsmRefReader::renderEntryHtml(int idx) const
{
    if (idx < 0 || idx >= p_entries.size())
        return QString();

    const AsmRefEntry &e = p_entries.at(idx);

    QString html;
    html.reserve(e.html.size() + 128);
    html += QStringLiteral("<h2 style=\"margin-top:0;\">") + e.title.toHtmlEscaped() + QStringLiteral("</h2>");
    html += e.html;
    return html;
}

void AsmRefReader::onCrossRefLinkClicked(const QUrl &link)
{
    static const QString scheme = QStringLiteral("asmref:");

    const QString url = link.toString();
    if (!url.startsWith(scheme))
        return;

    const QString id = url.mid(scheme.length());
    selectEntryByIndex(p_idToIndex.value(id, -1));
}

void AsmRefReader::onExpandAll()
{
    p_tree->expandAll();
}

void AsmRefReader::onCollapseAll()
{
    p_tree->collapseAll();
}

void AsmRefReader::onFilterNext()
{
    if (p_currentMatches.isEmpty())
        return;

    p_currentMatchIndex = (p_currentMatchIndex + 1) % p_currentMatches.size();
    QTreeWidgetItem *item = p_currentMatches.at(p_currentMatchIndex);
    p_tree->setCurrentItem(item);
    p_tree->scrollToItem(item);
}

void AsmRefReader::onFilterPrev()
{
    if (p_currentMatches.isEmpty())
        return;

    p_currentMatchIndex = (p_currentMatchIndex - 1 + p_currentMatches.size()) % p_currentMatches.size();
    QTreeWidgetItem *item = p_currentMatches.at(p_currentMatchIndex);
    p_tree->setCurrentItem(item);
    p_tree->scrollToItem(item);
}

void AsmRefReader::closeEvent(QCloseEvent *event)
{
    QSettings geometrySettings(AMIGAED_SETTINGS_ORG, AMIGAED_SETTINGS_APP);
    geometrySettings.setValue(QStringLiteral("MISC/AsmRefReaderGeometry"), saveGeometry());

    QDialog::closeEvent(event);
}
