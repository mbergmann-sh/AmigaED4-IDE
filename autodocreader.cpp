#include "autodocreader.h"
#include "version.h"

#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QSet>
#include <QFontDatabase>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTreeWidget>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QCloseEvent>
#include <QSizeGrip>
#include <algorithm>
#include <utility>

AutodocReader::AutodocReader(const QString &autodocsDir, QWidget *parent)
    : QDialog(parent)
{
    // Non-modal by construction (this dialog is only ever show()n, never
    // exec()d - see MainWindow::actionShowAutodocReader()) and
    // WA_DeleteOnClose so the "only one at a time" tracking in MainWindow
    // (p_autodocReader) can rely on the window's destroyed() signal to
    // know when it's gone, exactly like the existing Help > Manual viewer.
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlag(Qt::WindowMinMaxButtonsHint, true);

    // Remember the window's last size/position across sessions (rev.150).
    // Falls back to a sensible default the very first time the reader is
    // ever opened, or if nothing was saved yet.
    QSettings geometrySettings(AMIGAED_SETTINGS_ORG, AMIGAED_SETTINGS_APP);
    const QByteArray savedGeometry = geometrySettings.value(QStringLiteral("MISC/AutodocReaderGeometry")).toByteArray();
    if (savedGeometry.isEmpty() || !restoreGeometry(savedGeometry))
        resize(950, 650);

    parseAutodocsFolder(autodocsDir);

    QSet<QString> allFiles;
    for (const AutodocEntry &e : std::as_const(p_entries))
        allFiles.insert(e.sourceFile);
    p_totalFiles = allFiles.size();

    // --- left side: filter row, function tree, Open/Close All row ---
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

    // --- right side: the selected function's raw AutoDoc text ---
    p_textView = new QPlainTextEdit(this);
    p_textView->setReadOnly(true);
    p_textView->setLineWrapMode(QPlainTextEdit::NoWrap);
    // AutoDoc SYNOPSIS sections rely on column alignment (e.g. "D0" lined
    // up under the return value) - QFontDatabase::systemFont(FixedFont) is
    // Qt's own guaranteed-monospace system font, so this is correct on
    // Windows/Linux/macOS alike without any of the per-platform font-name
    // guessing initializeFont() has to do for the QScintilla editor (that
    // function's own problem - internal per-style font objects Scintilla
    // builds itself - doesn't apply here: this is one plain QPlainTextEdit
    // with a single QFont, no per-style-run font resolution involved).
    p_textView->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(leftWidget);
    splitter->addWidget(p_textView);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setSizes({ 320, 630 });

    // Bottom-right resize grip (rev.152) - the window is already
    // resizable from any edge/corner via the window manager (nothing
    // about this QDialog restricts that), but that's easy to miss at a
    // glance; the familiar diagonal-lines "size grip" in the corner
    // makes it obvious at a glance, exactly like the one every QStatusBar
    // shows by default - this dialog just has no status bar of its own
    // to host it in, hence its own dedicated bottom row here.
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
    applyFilter(QString());   // also sets the initial window title / Prev-Next enabled state

    connect(p_filterEdit, &QLineEdit::textChanged, this, &AutodocReader::onFilterTextChanged);
    connect(p_filterEdit, &QLineEdit::returnPressed, this, &AutodocReader::onFilterNext);
    connect(p_tree, &QTreeWidget::currentItemChanged, this, &AutodocReader::onTreeSelectionChanged);
    connect(p_openAllBtn, &QPushButton::clicked, this, &AutodocReader::onExpandAll);
    connect(p_closeAllBtn, &QPushButton::clicked, this, &AutodocReader::onCollapseAll);
    connect(p_filterPrevBtn, &QPushButton::clicked, this, &AutodocReader::onFilterPrev);
    connect(p_filterNextBtn, &QPushButton::clicked, this, &AutodocReader::onFilterNext);

    p_filterEdit->setFocus();
}

bool AutodocReader::showFunction(const QString &functionName)
{
    const QString name = functionName.trimmed();
    if (name.isEmpty())
        return false;

    for (int gi = 0; gi < p_tree->topLevelItemCount(); ++gi)
    {
        QTreeWidgetItem *groupItem = p_tree->topLevelItem(gi);

        for (int ci = 0; ci < groupItem->childCount(); ++ci)
        {
            QTreeWidgetItem *funcItem = groupItem->child(ci);
            const int idx = funcItem->data(0, Qt::UserRole).toInt();
            if (idx < 0 || idx >= p_entries.size())
                continue;

            const QString &fullName = p_entries.at(idx).functionName;   // e.g. "intuition.library/OpenWindow"
            const int slash = fullName.lastIndexOf(QLatin1Char('/'));
            const QString shortName = (slash >= 0) ? fullName.mid(slash + 1) : fullName;

            if (shortName.compare(name, Qt::CaseInsensitive) != 0)
                continue;

            // A leftover filter would otherwise leave every non-matching
            // group/leaf hidden around the one we're about to reveal -
            // clearing it re-runs applyFilter(QString()) via
            // onFilterTextChanged(), unhiding everything first.
            if (!p_filterEdit->text().isEmpty())
                p_filterEdit->clear();

            groupItem->setExpanded(true);
            p_tree->setCurrentItem(funcItem);   // also triggers onTreeSelectionChanged(), filling the text view
            p_tree->scrollToItem(funcItem);
            return true;
        }
    }

    return false;
}

//
// Recursively scans dir for *.doc files and parses every one of them.
// Deliberately recursive (QDirIterator::Subdirectories) rather than
// assuming a flat layout - the real NDK Autodocs drawer ships all
// per-library *.doc files flat in one folder, but this way a
// differently organized copy (or a partial/custom collection) still
// works without the user having to reorganize anything.
//
void AutodocReader::parseAutodocsFolder(const QString &dir)
{
    p_entries.clear();

    QDirIterator it(dir, QStringList() << QStringLiteral("*.doc"),
                     QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
        parseAutodocFile(it.next());
}

//
// Splits one *.doc file into individual function entries and appends
// them to p_entries.
//
// Verified against a real NDK 3.2R4 exec.doc: every function entry is
// delimited by a raw FORM FEED byte (0x0C - historically a "start a new
// printed page here" marker), immediately followed by a line holding
// the function's own "library.name/FunctionName" identifier (in
// exec.doc it's padded and repeated a second time further right on the
// same line, but that repeat is cosmetic, not something every AutoDoc
// relies on). Everything between the end of that line and the NEXT
// 0x0C byte (or end of file) is the entry's own body text.
//
// This mirrors exactly how the well-established Amiga AutoDoc reader
// MinAD itself finds entries (see its listtree.c CheckFile()/AddFunc(),
// which scans for the very same 0x0C byte - confusingly called
// "LINEFEED" in its own source - and takes only the first
// whitespace-delimited token of the line that follows as the entry's
// name). Earlier revision of this parser instead looked for the name
// appearing twice on one line via regex, which is what exec.doc's
// *visual* layout suggests but not what actually delimits entries - on
// a file where that assumption didn't hold, it matched arbitrary
// unrelated repeated tokens (e.g. a SYNOPSIS section's register-name
// column) instead of real entries. Matching MinAD's own, proven byte-
// level approach avoids relying on that assumption at all.
//
void AutodocReader::parseAutodocFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return;

    const QString content = QString::fromUtf8(file.readAll());
    file.close();

    static const QChar formFeed(0x0C);

    QVector<int> markerPositions;
    for (int i = 0; i < content.size(); ++i)
    {
        if (content.at(i) == formFeed)
            markerPositions << i;
    }

    static const QRegularExpression whitespaceRe(QStringLiteral("\\s+"));

    for (int i = 0; i < markerPositions.size(); ++i)
    {
        const int entryStart = markerPositions[i] + 1;   // right after the form-feed byte
        const int entryEnd = (i + 1 < markerPositions.size()) ? markerPositions[i + 1] : content.size();
        if (entryEnd <= entryStart)
            continue;   // e.g. a lone trailing form-feed right at end of file

        const QString entryBlock = content.mid(entryStart, entryEnd - entryStart);

        // First line = the marker line - take only its first token as the
        // function's name (see the comment above for why not "match the
        // whole line"). Everything after that line, trimmed, is the body.
        const int firstNewline = entryBlock.indexOf(QLatin1Char('\n'));
        const QString markerLine = (firstNewline >= 0) ? entryBlock.left(firstNewline) : entryBlock;
        const QString name = markerLine.trimmed().section(whitespaceRe, 0, 0);

        // A real function marker line always has the "library/Name" shape;
        // anything else here (most commonly a lone form-feed right at EOF,
        // or a stray 0x0C elsewhere in the file) is not a real entry.
        if (name.isEmpty() || !name.contains(QLatin1Char('/')))
            continue;

        const QString body = (firstNewline >= 0) ? entryBlock.mid(firstNewline + 1).trimmed() : QString();
        if (body.isEmpty())
            continue;

        AutodocEntry entry;
        entry.functionName = name;
        entry.library = QFileInfo(filePath).completeBaseName();   // see buildTree()
        entry.sourceFile = filePath;
        entry.fullText = body;
        p_entries << entry;
    }
}

//
// Lightweight counterpart to parseAutodocsFolder()/parseAutodocFile():
// same form-feed-delimited entry scan (see parseAutodocFile()'s own
// comment for why that, rather than the visually-repeated-name regex an
// earlier revision used, is the correct delimiter), but only ever keeps
// each entry's short function name - never its body text, and never
// touches p_entries or builds anything - so a caller can cheaply ask "is
// this word a real NDK/MUI function" without constructing a whole
// AutodocReader (which would also mean building its entire tree/text-view
// UI just to throw it away again). See the header for the intended use.
//
QSet<QString> AutodocReader::collectFunctionNames(const QString &autodocsDir)
{
    QSet<QString> names;

    static const QChar formFeed(0x0C);
    static const QRegularExpression whitespaceRe(QStringLiteral("\\s+"));

    QDirIterator it(autodocsDir, QStringList() << QStringLiteral("*.doc"),
                     QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QFile file(it.next());
        if (!file.open(QIODevice::ReadOnly))
            continue;

        const QString content = QString::fromUtf8(file.readAll());
        file.close();

        QVector<int> markerPositions;
        for (int i = 0; i < content.size(); ++i)
        {
            if (content.at(i) == formFeed)
                markerPositions << i;
        }

        for (int i = 0; i < markerPositions.size(); ++i)
        {
            const int entryStart = markerPositions[i] + 1;
            const int entryEnd = (i + 1 < markerPositions.size()) ? markerPositions[i + 1] : content.size();
            if (entryEnd <= entryStart)
                continue;

            const QString entryBlock = content.mid(entryStart, entryEnd - entryStart);
            const int firstNewline = entryBlock.indexOf(QLatin1Char('\n'));
            const QString markerLine = (firstNewline >= 0) ? entryBlock.left(firstNewline) : entryBlock;
            const QString fullName = markerLine.trimmed().section(whitespaceRe, 0, 0);

            if (fullName.isEmpty() || !fullName.contains(QLatin1Char('/')))
                continue;   // not a real marker line - see parseAutodocFile()

            const int slash = fullName.lastIndexOf(QLatin1Char('/'));
            const QString shortName = (slash >= 0) ? fullName.mid(slash + 1) : fullName;
            if (!shortName.isEmpty())
                names.insert(shortName.toLower());
        }
    }

    return names;
}

//
// Groups every parsed entry under a top-level tree node per SOURCE FILE
// (exec, dos, graphics, ... - one group per *.doc file, using that
// file's own base name - see parseAutodocFile()). This, not grouping by
// the function name's own "library.name/" prefix, is what the reference
// screenshots' "Autogroup" mode actually does - confirmed against
// MinAD's own listtree.c CheckGroup(), which classifies each file by
// its filename, and directly visible in the second screenshot's
// "amiga_lib" group (an underscore, which can only come from a real
// file named amiga_lib.doc - no function name ever contains one).
// Final alphabetical ordering (both groups and the functions within
// them) is left to the sortItems() call at the end rather than
// pre-sorting here.
//
void AutodocReader::buildTree()
{
    p_tree->clear();

    QHash<QString, QTreeWidgetItem *> groupItems;

    for (int idx = 0; idx < p_entries.size(); ++idx)
    {
        const AutodocEntry &e = p_entries.at(idx);

        QTreeWidgetItem *groupItem = groupItems.value(e.library, nullptr);
        if (!groupItem)
        {
            groupItem = new QTreeWidgetItem(p_tree, QStringList{ e.library });
            // Group headers are just organisational - only the function
            // leaves underneath carry an actual AutoDoc entry to show.
            groupItem->setFlags(groupItem->flags() & ~Qt::ItemIsSelectable);
            groupItems.insert(e.library, groupItem);
        }

        QTreeWidgetItem *funcItem = new QTreeWidgetItem(groupItem, QStringList{ e.functionName });
        funcItem->setData(0, Qt::UserRole, idx);
    }

    p_tree->sortItems(0, Qt::AscendingOrder);
}

//
// Live-filters the tree as the user types: a function leaf stays visible
// only if its name contains the filter text (case-insensitive); a group
// header stays visible only if at least one of its children does, and is
// auto-expanded while a filter is active so matches are immediately
// visible without the user having to expand each group by hand. Also
// rebuilds p_currentMatches for the Prev/Next buttons and refreshes the
// window title's running "showing X/Y Funcs in A/B Files" count, the
// same information the reference screenshot shows in its own title bar.
//
void AutodocReader::applyFilter(const QString &textRaw)
{
    const QString text = textRaw.trimmed();

    int visibleFuncs = 0;
    QSet<QString> visibleFiles;
    p_currentMatches.clear();

    for (int gi = 0; gi < p_tree->topLevelItemCount(); ++gi)
    {
        QTreeWidgetItem *groupItem = p_tree->topLevelItem(gi);
        bool anyVisibleChild = false;

        for (int ci = 0; ci < groupItem->childCount(); ++ci)
        {
            QTreeWidgetItem *funcItem = groupItem->child(ci);
            const bool matches = text.isEmpty() || funcItem->text(0).contains(text, Qt::CaseInsensitive);
            funcItem->setHidden(!matches);

            if (matches)
            {
                anyVisibleChild = true;
                ++visibleFuncs;
                const int idx = funcItem->data(0, Qt::UserRole).toInt();
                visibleFiles.insert(p_entries.at(idx).sourceFile);
                p_currentMatches << funcItem;
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

    if (haveFilter)
    {
        setWindowTitle(tr("AutoDoc Reader - Filter showing %1/%2 Funcs in %3/%4 Files")
                        .arg(visibleFuncs).arg(p_entries.size())
                        .arg(visibleFiles.size()).arg(p_totalFiles));
    }
    else
    {
        setWindowTitle(tr("AutoDoc Reader - %1 Funcs in %2 Files")
                        .arg(p_entries.size()).arg(p_totalFiles));
    }
}

void AutodocReader::onFilterTextChanged(const QString &text)
{
    applyFilter(text);
}

void AutodocReader::onTreeSelectionChanged()
{
    QTreeWidgetItem *item = p_tree->currentItem();
    // No selection, or a group header (group items have no parent and
    // carry no Qt::UserRole entry index) - nothing to show.
    if (!item || !item->parent())
    {
        p_textView->clear();
        return;
    }

    const int idx = item->data(0, Qt::UserRole).toInt();
    if (idx >= 0 && idx < p_entries.size())
        p_textView->setPlainText(p_entries.at(idx).fullText);
}

void AutodocReader::onExpandAll()
{
    p_tree->expandAll();
}

void AutodocReader::onCollapseAll()
{
    p_tree->collapseAll();
}

void AutodocReader::onFilterNext()
{
    if (p_currentMatches.isEmpty())
        return;

    p_currentMatchIndex = (p_currentMatchIndex + 1) % p_currentMatches.size();
    QTreeWidgetItem *item = p_currentMatches.at(p_currentMatchIndex);
    p_tree->setCurrentItem(item);
    p_tree->scrollToItem(item);
}

void AutodocReader::onFilterPrev()
{
    if (p_currentMatches.isEmpty())
        return;

    p_currentMatchIndex = (p_currentMatchIndex - 1 + p_currentMatches.size()) % p_currentMatches.size();
    QTreeWidgetItem *item = p_currentMatches.at(p_currentMatchIndex);
    p_tree->setCurrentItem(item);
    p_tree->scrollToItem(item);
}

void AutodocReader::closeEvent(QCloseEvent *event)
{
    QSettings geometrySettings(AMIGAED_SETTINGS_ORG, AMIGAED_SETTINGS_APP);
    geometrySettings.setValue(QStringLiteral("MISC/AutodocReaderGeometry"), saveGeometry());

    QDialog::closeEvent(event);
}
