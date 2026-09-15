#include "cpprefreader.h"
#include "version.h"

#include <QCloseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QSet>
#include <QSettings>
#include <QSizeGrip>
#include <QSplitter>
#include <QTextBrowser>
#include <QTreeWidget>
#include <QUrl>
#include <QVBoxLayout>

CppRefReader::CppRefReader(const QString &guiLanguage, QWidget *parent)
    : QDialog(parent)
    , p_lang(guiLanguage == QStringLiteral("de") ? QStringLiteral("de") : QStringLiteral("en"))
{
    // Non-modal by construction (only ever show()n, never exec()d - see
    // MainWindow::showCppRefReaderAndJumpTo()) and WA_DeleteOnClose so the
    // "only one at a time" tracking in MainWindow (p_cppRefReader) can
    // rely on the window's destroyed() signal to know when it's gone -
    // same pattern as the Help > Manual viewer and the AutoDoc Reader.
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlag(Qt::WindowMinMaxButtonsHint, true);

    QSettings geometrySettings(AMIGAED_SETTINGS_ORG, AMIGAED_SETTINGS_APP);
    const QByteArray savedGeometry
        = geometrySettings.value(QStringLiteral("MISC/CppRefReaderGeometry")).toByteArray();
    if (savedGeometry.isEmpty() || !restoreGeometry(savedGeometry))
        resize(950, 650);

    setWindowTitle((p_lang == QStringLiteral("de")) ? QStringLiteral("AmigaED C/C++-Referenz")
                                                    : QStringLiteral("AmigaED C/C++ Reference"));

    populateEntries();
    buildIdIndex();
    buildTokenIndex();

    // --- left side: filter row, entry tree, Open/Close All row ---
    // Deliberately the same three widgets in the same arrangement as
    // AutodocReader (see autodocreader.cpp) - a user who already knows
    // that window immediately knows how to use this one too.
    p_filterEdit = new QLineEdit(this);
    p_filterEdit->setPlaceholderText(tr("Filter..."));
    p_filterEdit->setClearButtonEnabled(true);

    p_filterPrevBtn = new QPushButton(QStringLiteral("◀"), this); // <
    p_filterPrevBtn->setToolTip(tr("Previous match"));
    p_filterPrevBtn->setMaximumWidth(28);
    p_filterPrevBtn->setEnabled(false);

    p_filterNextBtn = new QPushButton(QStringLiteral("▶"), this); // >
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
    // Cross-reference links use the "cppref:<id>" scheme (see
    // renderEntryHtml()/onCrossRefLinkClicked()), not a real document to
    // navigate to - without this, QTextBrowser would try to "load" one
    // as a new page itself (clearing the view) before anchorClicked()
    // below even runs, exactly the same reasoning as AutodocReader's own
    // p_textView->setOpenLinks(false).
    p_textView->setOpenLinks(false);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(leftWidget);
    splitter->addWidget(p_textView);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setSizes({320, 630});

    // Bottom-right resize grip - same reasoning and pattern as
    // AutodocReader's own (rev.152): this QDialog is already resizable
    // from any edge/corner via the window manager, the grip is just a
    // visual reminder of that.
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

    connect(p_filterEdit, &QLineEdit::textChanged, this, &CppRefReader::onFilterTextChanged);
    connect(p_filterEdit, &QLineEdit::returnPressed, this, &CppRefReader::onFilterNext);
    connect(p_tree, &QTreeWidget::currentItemChanged, this, &CppRefReader::onTreeSelectionChanged);
    connect(p_openAllBtn, &QPushButton::clicked, this, &CppRefReader::onExpandAll);
    connect(p_closeAllBtn, &QPushButton::clicked, this, &CppRefReader::onCollapseAll);
    connect(p_filterPrevBtn, &QPushButton::clicked, this, &CppRefReader::onFilterPrev);
    connect(p_filterNextBtn, &QPushButton::clicked, this, &CppRefReader::onFilterNext);
    connect(p_textView, &QTextBrowser::anchorClicked, this, &CppRefReader::onCrossRefLinkClicked);

    p_filterEdit->setFocus();
}

void CppRefReader::showCategory(const QString &category)
{
    if (!p_filterEdit->text().isEmpty())
        p_filterEdit->clear(); // see showFunction()'s own comment in AutodocReader for why

    for (int gi = 0; gi < p_tree->topLevelItemCount(); ++gi) {
        QTreeWidgetItem *groupItem = p_tree->topLevelItem(gi);
        if (groupItem->data(0, Qt::UserRole + 1).toString() != category)
            continue;

        groupItem->setExpanded(true);
        p_tree->scrollToItem(groupItem);

        if (groupItem->childCount() > 0) {
            QTreeWidgetItem *firstChild = groupItem->child(0);
            p_tree->setCurrentItem(firstChild); // also triggers onTreeSelectionChanged()
            p_tree->scrollToItem(firstChild);
        }
        return;
    }
}

void CppRefReader::addEntry(const QString &id,
                            const QString &category,
                            const QString &title,
                            const QString &htmlEn,
                            const QString &htmlDe)
{
    CppRefEntry e;
    e.id = id;
    e.category = category;
    e.title = title;
    e.html = (p_lang == QStringLiteral("de")) ? htmlDe : htmlEn;
    p_entries.append(e);
}

//
// Every authored entry, in order - see cpprefdata.cpp for
// populateKeywords()/populateDatatypes()/populateVariables()/
// populateDecisions()/populateControlStructures()/populateOperators()/
// populateFunctions(), one function per top-level category (see
// buildTree()'s categoryOrder for the fixed order they're shown in).
//
void CppRefReader::populateEntries()
{
    populateKeywords();
    populateDatatypes();
    populateVariables();
    populateDecisions();
    populateControlStructures();
    populateOperators();
    populateFunctions();
}

void CppRefReader::buildIdIndex()
{
    p_idToIndex.clear();
    for (int idx = 0; idx < p_entries.size(); ++idx)
        p_idToIndex.insert(p_entries.at(idx).id, idx);
}

//
// See the .h for the overall rationale (rev.158, editor "Lookup C/C++").
// Automatic layer: any entry whose title already IS a single clean
// identifier registers that exact spelling. Explicit layer: a short,
// fixed list of additional spellings for the few entries whose DISPLAY
// title reads better as something other than a bare identifier (e.g.
// kw_do is shown as "do...while" in the tree, not just "do") - added only
// if nothing already claims that exact token, so an explicit entry can
// never shadow a more fundamental one (kw_bool's own title "bool" always
// wins the token "bool" over dt_bool's combined "bool / _Bool").
//
void CppRefReader::buildTokenIndex()
{
    p_tokenToIndex.clear();

    static const QRegularExpression singleIdentifier(QStringLiteral("^[A-Za-z_][A-Za-z0-9_]*$"));
    for (int idx = 0; idx < p_entries.size(); ++idx) {
        const QString &title = p_entries.at(idx).title;
        if (singleIdentifier.match(title).hasMatch() && !p_tokenToIndex.contains(title))
            p_tokenToIndex.insert(title, idx);
    }

    struct ExtraToken
    {
        const char *token;
        const char *id;
    };
    static const ExtraToken extraTokens[] = {
        {"do", "kw_do"},      // kw_do's own title is "do...while"
        {"_Bool", "dt_bool"}, // dt_bool's own title is "bool / _Bool"
    };
    for (const auto &extra : extraTokens) {
        const QString token = QString::fromLatin1(extra.token);
        if (p_tokenToIndex.contains(token))
            continue;
        const int entryIdx = p_idToIndex.value(QString::fromLatin1(extra.id), -1);
        if (entryIdx >= 0)
            p_tokenToIndex.insert(token, entryIdx);
    }
}

bool CppRefReader::hasEntryForToken(const QString &token) const
{
    return !token.isEmpty() && p_tokenToIndex.contains(token);
}

bool CppRefReader::showEntryForToken(const QString &token)
{
    return selectEntryByIndex(p_tokenToIndex.value(token, -1));
}

//
// Shared by onCrossRefLinkClicked() and showEntryForToken() - expands the
// entry's group, selects it and scrolls it into view, clearing any active
// filter first exactly like showCategory() does.
//
bool CppRefReader::selectEntryByIndex(int idx)
{
    if (idx < 0 || idx >= p_entries.size())
        return false;

    if (!p_filterEdit->text().isEmpty())
        p_filterEdit->clear();

    for (int gi = 0; gi < p_tree->topLevelItemCount(); ++gi) {
        QTreeWidgetItem *groupItem = p_tree->topLevelItem(gi);
        for (int ci = 0; ci < groupItem->childCount(); ++ci) {
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

//
// Fixed, pedagogical top-level order (NOT alphabetical - see the header)
// - every category listed here gets its own group, populated from the
// matching populateXxx() call in populateEntries().
//
void CppRefReader::buildTree()
{
    p_tree->clear();

    static const QStringList categoryOrder = {
        QStringLiteral("Keywords"),
        QStringLiteral("Datatypes"),
        QStringLiteral("Variables"),
        QStringLiteral("Decisions"),
        QStringLiteral("Control Structures"),
        QStringLiteral("Operators"),
        QStringLiteral("Functions"),
    };

    // tr() here deliberately uses the exact same source strings as the
    // Help > C/C++ submenu actions in mainwindow.cpp, so lupdate/lrelease
    // only ever need one German translation per category name, reused
    // for both the menu entry and this group header.
    QHash<QString, QString> displayName;
    displayName.insert(QStringLiteral("Keywords"), tr("Keywords"));
    displayName.insert(QStringLiteral("Datatypes"), tr("Datatypes"));
    displayName.insert(QStringLiteral("Variables"), tr("Variables"));
    displayName.insert(QStringLiteral("Decisions"), tr("Decisions"));
    displayName.insert(QStringLiteral("Control Structures"), tr("Control Structures"));
    displayName.insert(QStringLiteral("Operators"), tr("Operators"));
    displayName.insert(QStringLiteral("Functions"), tr("Functions"));

    QHash<QString, QTreeWidgetItem *> groupItems;
    for (const QString &category : categoryOrder) {
        QTreeWidgetItem *groupItem = new QTreeWidgetItem(p_tree,
                                                         QStringList{displayName.value(category,
                                                                                       category)});
        groupItem->setFlags(groupItem->flags() & ~Qt::ItemIsSelectable);
        groupItem->setData(0,
                           Qt::UserRole + 1,
                           category); // language-independent key, see showCategory()
        groupItems.insert(category, groupItem);
    }

    for (int idx = 0; idx < p_entries.size(); ++idx) {
        const CppRefEntry &e = p_entries.at(idx);
        QTreeWidgetItem *groupItem = groupItems.value(e.category, nullptr);
        if (!groupItem)
            continue; // an entry whose category isn't in categoryOrder above - shouldn't happen, silently skip rather than crash

        QTreeWidgetItem *entryItem = new QTreeWidgetItem(groupItem, QStringList{e.title});
        entryItem->setData(0, Qt::UserRole, idx);
    }

    // Sort only WITHIN each group (alphabetically, by entry title) -
    // p_tree->sortItems() would also reorder the top-level groups
    // themselves back to alphabetical, undoing categoryOrder above, so
    // each group is sorted individually instead.
    for (int gi = 0; gi < p_tree->topLevelItemCount(); ++gi)
        p_tree->topLevelItem(gi)->sortChildren(0, Qt::AscendingOrder);

    // A handful of entries are reference material a user wants to see
    // FIRST in their group regardless of where their title happens to
    // land alphabetically (rev.158: Datatypes' "Overview: Sizes & Value
    // Ranges" table) - pin them to index 0 of their own group, after the
    // alphabetical sort above so this always wins.
    static const QSet<QString> pinnedFirstIds = {
        QStringLiteral("dt_overview"),
    };
    for (int gi = 0; gi < p_tree->topLevelItemCount(); ++gi) {
        QTreeWidgetItem *groupItem = p_tree->topLevelItem(gi);
        for (int ci = 0; ci < groupItem->childCount(); ++ci) {
            QTreeWidgetItem *entryItem = groupItem->child(ci);
            const int idx = entryItem->data(0, Qt::UserRole).toInt();
            if (idx < 0 || idx >= p_entries.size())
                continue;
            if (pinnedFirstIds.contains(p_entries.at(idx).id) && ci != 0) {
                groupItem->takeChild(ci);
                groupItem->insertChild(0, entryItem);
            }
        }
    }
}

void CppRefReader::applyFilter(const QString &textRaw)
{
    const QString text = textRaw.trimmed();

    int visibleEntries = 0;
    p_currentMatches.clear();

    for (int gi = 0; gi < p_tree->topLevelItemCount(); ++gi) {
        QTreeWidgetItem *groupItem = p_tree->topLevelItem(gi);
        bool anyVisibleChild = false;

        for (int ci = 0; ci < groupItem->childCount(); ++ci) {
            QTreeWidgetItem *entryItem = groupItem->child(ci);
            const bool matches = text.isEmpty()
                                 || entryItem->text(0).contains(text, Qt::CaseInsensitive);
            entryItem->setHidden(!matches);

            if (matches) {
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
                                  ? QStringLiteral("AmigaED C/C++-Referenz")
                                  : QStringLiteral("AmigaED C/C++ Reference");

    if (haveFilter) {
        setWindowTitle(
            baseTitle + QStringLiteral(" - ")
            + tr("Filter showing %1/%2 entries").arg(visibleEntries).arg(p_entries.size()));
    } else {
        setWindowTitle(baseTitle + QStringLiteral(" - ") + tr("%1 entries").arg(p_entries.size()));
    }
}

void CppRefReader::onFilterTextChanged(const QString &text)
{
    applyFilter(text);
}

void CppRefReader::onTreeSelectionChanged()
{
    QTreeWidgetItem *item = p_tree->currentItem();
    if (!item || !item->parent()) {
        p_textView->clear();
        return;
    }

    const int idx = item->data(0, Qt::UserRole).toInt();
    if (idx >= 0 && idx < p_entries.size())
        p_textView->setHtml(renderEntryHtml(idx));
}

QString CppRefReader::renderEntryHtml(int idx) const
{
    if (idx < 0 || idx >= p_entries.size())
        return QString();

    const CppRefEntry &e = p_entries.at(idx);

    QString html;
    html.reserve(e.html.size() + 128);
    html += QStringLiteral("<h2 style=\"margin-top:0;\">") + e.title.toHtmlEscaped()
            + QStringLiteral("</h2>");
    html += e.html;
    return html;
}

//
// Handles a click on one of the "See also" cross-reference links an
// entry's own HTML embeds directly (href "cppref:<id>", written by hand
// in populateEntries() - unlike AutodocReader's SEE ALSO lines, there is
// no plain-text section to scan and linkify automatically here, since
// every entry's explanation is already hand-authored HTML).
// setOpenLinks(false) on p_textView (see constructor) is what stops
// QTextBrowser from trying to navigate to that URL itself first.
//
void CppRefReader::onCrossRefLinkClicked(const QUrl &link)
{
    static const QString scheme = QStringLiteral("cppref:");

    const QString url = link.toString();
    if (!url.startsWith(scheme))
        return;

    const QString id = url.mid(scheme.length());
    selectEntryByIndex(p_idToIndex.value(id, -1));
}

void CppRefReader::onExpandAll()
{
    p_tree->expandAll();
}

void CppRefReader::onCollapseAll()
{
    p_tree->collapseAll();
}

void CppRefReader::onFilterNext()
{
    if (p_currentMatches.isEmpty())
        return;

    p_currentMatchIndex = (p_currentMatchIndex + 1) % p_currentMatches.size();
    QTreeWidgetItem *item = p_currentMatches.at(p_currentMatchIndex);
    p_tree->setCurrentItem(item);
    p_tree->scrollToItem(item);
}

void CppRefReader::onFilterPrev()
{
    if (p_currentMatches.isEmpty())
        return;

    p_currentMatchIndex = (p_currentMatchIndex - 1 + p_currentMatches.size())
                          % p_currentMatches.size();
    QTreeWidgetItem *item = p_currentMatches.at(p_currentMatchIndex);
    p_tree->setCurrentItem(item);
    p_tree->scrollToItem(item);
}

void CppRefReader::closeEvent(QCloseEvent *event)
{
    QSettings geometrySettings(AMIGAED_SETTINGS_ORG, AMIGAED_SETTINGS_APP);
    geometrySettings.setValue(QStringLiteral("MISC/CppRefReaderGeometry"), saveGeometry());

    QDialog::closeEvent(event);
}
