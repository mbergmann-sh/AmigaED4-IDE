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
#include <QTextBrowser>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QCloseEvent>
#include <QSizeGrip>
#include <QUrl>
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
    buildNameIndex();   // rev.153 - see its own comment; needed before the first renderEntryHtml() call

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
    // QTextBrowser, not QPlainTextEdit (changed in rev.153): SEE ALSO
    // cross-references need to render as real, clickable <a> links (see
    // renderEntryHtml()/onSeeAlsoLinkClicked() below) - a plain text
    // widget has no notion of a link at all. setHtml() wraps the content
    // in its own <pre>, which keeps everything else about the display
    // (monospace column alignment, no line wrapping) exactly as before.
    p_textView = new QTextBrowser(this);
    p_textView->setReadOnly(true);
    p_textView->setLineWrapMode(QTextEdit::NoWrap);
    // AutoDoc SYNOPSIS sections rely on column alignment (e.g. "D0" lined
    // up under the return value) - QFontDatabase::systemFont(FixedFont) is
    // Qt's own guaranteed-monospace system font, so this is correct on
    // Windows/Linux/macOS alike without any of the per-platform font-name
    // guessing initializeFont() has to do for the QScintilla editor (that
    // function's own problem - internal per-style font objects Scintilla
    // builds itself - doesn't apply here: this is one rich-text view with
    // a single QFont, no per-style-run font resolution involved).
    p_textView->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    // The SEE ALSO links aren't real documents to navigate to - without
    // this, QTextBrowser would try to "load" e.g. "adoc:CMD_WRITE" as a
    // new page itself (clearing the view) before anchorClicked() below
    // even runs. onSeeAlsoLinkClicked() is what actually acts on a click.
    p_textView->setOpenLinks(false);

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
    connect(p_textView, &QTextBrowser::anchorClicked, this, &AutodocReader::onSeeAlsoLinkClicked);

    p_filterEdit->setFocus();
}

bool AutodocReader::showFunction(const QString &functionName, const QString &preferredLibrary)
{
    const QString name = functionName.trimmed();
    if (name.isEmpty())
        return false;

    // A short function name isn't always unique across libraries (e.g.
    // "CMD_WRITE" exists in audio.device, carddisk.device, trackdisk.device,
    // ...) - collect the FIRST match found (tree/alphabetical order, same
    // behaviour as before rev.154) as a fallback, but prefer one whose
    // library equals preferredLibrary, if given and found. Callers that
    // don't have a library context (e.g. MainWindow's "Jump to Explanation",
    // which only knows the clicked word, not which library it belongs to)
    // pass an empty preferredLibrary and get exactly the old behaviour.
    QTreeWidgetItem *firstMatch = nullptr;
    QTreeWidgetItem *preferredMatch = nullptr;

    for (int gi = 0; gi < p_tree->topLevelItemCount() && !preferredMatch; ++gi)
    {
        QTreeWidgetItem *groupItem = p_tree->topLevelItem(gi);

        for (int ci = 0; ci < groupItem->childCount(); ++ci)
        {
            QTreeWidgetItem *funcItem = groupItem->child(ci);
            const int idx = funcItem->data(0, Qt::UserRole).toInt();
            if (idx < 0 || idx >= p_entries.size())
                continue;

            const AutodocEntry &entry = p_entries.at(idx);
            const QString &fullName = entry.functionName;   // e.g. "intuition.library/OpenWindow"
            const int slash = fullName.lastIndexOf(QLatin1Char('/'));
            const QString shortName = (slash >= 0) ? fullName.mid(slash + 1) : fullName;

            if (shortName.compare(name, Qt::CaseInsensitive) != 0)
                continue;

            if (!firstMatch)
                firstMatch = funcItem;

            if (!preferredLibrary.isEmpty()
                && entry.library.compare(preferredLibrary, Qt::CaseInsensitive) == 0)
            {
                preferredMatch = funcItem;
                break;
            }
        }
    }

    QTreeWidgetItem *funcItem = preferredMatch ? preferredMatch : firstMatch;
    if (!funcItem)
        return false;

    // A leftover filter would otherwise leave every non-matching
    // group/leaf hidden around the one we're about to reveal -
    // clearing it re-runs applyFilter(QString()) via
    // onFilterTextChanged(), unhiding everything first.
    if (!p_filterEdit->text().isEmpty())
        p_filterEdit->clear();

    if (QTreeWidgetItem *groupItem = funcItem->parent())
        groupItem->setExpanded(true);
    p_tree->setCurrentItem(funcItem);   // also triggers onTreeSelectionChanged(), filling the text view
    p_tree->scrollToItem(funcItem);
    return true;
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
        setWindowTitle(tr("AmigaED AutoDoc Reader - Filter showing %1/%2 Funcs in %3/%4 Files")
                        .arg(visibleFuncs).arg(p_entries.size())
                        .arg(visibleFiles.size()).arg(p_totalFiles));
    }
    else
    {
        setWindowTitle(tr("AmigaED AutoDoc Reader - %1 Funcs in %2 Files")
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
        p_textView->setHtml(renderEntryHtml(idx));
}

//
// Populates p_nameToIndex from p_entries - see the header for what it's
// for. Where the same short name genuinely occurs in more than one
// library's docs (rare, but not impossible), the first one encountered
// while parsing wins; SEE ALSO linkification only needs "does a match
// exist at all", not which one - actually jumping to it (showFunction())
// does its own independent, alphabetical-tree-order search anyway.
//
void AutodocReader::buildNameIndex()
{
    p_nameToIndex.clear();

    for (int idx = 0; idx < p_entries.size(); ++idx)
    {
        const QString &fullName = p_entries.at(idx).functionName;
        const int slash = fullName.lastIndexOf(QLatin1Char('/'));
        const QString shortName = (slash >= 0) ? fullName.mid(slash + 1) : fullName;

        if (shortName.isEmpty())
            continue;

        const QString key = shortName.toLower();
        if (!p_nameToIndex.contains(key))
            p_nameToIndex.insert(key, idx);
    }
}

//
// Renders p_entries[idx].fullText as HTML for p_textView, HTML-escaped
// and wrapped in a <pre> so its original column alignment survives
// completely unchanged from the old setPlainText() behaviour - except
// that within the entry's own "SEE ALSO" section specifically, a token
// matching another parsed entry's short function name becomes a
// clickable link (see linkifySeeAlsoLine()).
//
// A real AutoDoc section header (NAME, FUNCTION, SYNOPSIS, SEE ALSO, ...)
// is in upper case and - this is the part an earlier revision got wrong -
// is NOT necessarily flush against column 0: real NDK AutoDoc files
// indent section headers with a few literal spaces (three, in every
// sample seen so far), while body text under a header is indented with a
// literal TAB instead. That's what actually distinguishes the two, not
// "any leading whitespace at all" - a naive "must start in column 0" check
// misses every header except an entry's very first one (whose leading
// spaces happen to get stripped by parseAutodocFile()'s call to
// QString::trimmed() on the whole entry body, since trimmed() only trims
// the outer edges of the full block, not each line). Matching on "doesn't
// start with a tab" instead handles both the unindented first header and
// every space-indented one after it, while still correctly excluding
// every tab-indented body line - including a body line that happens to be
// all upper case itself, e.g. a "<devices/trackdisk.h>" style reference.
//
// A section is "SEE ALSO" from that header line up to (not including) the
// next such header line, or the end of the entry.
//
// Deliberately scoped to just SEE ALSO rather than linking every
// occurrence of a known function name anywhere in the text: ordinary
// prose in FUNCTION/NOTES/etc. can easily contain a plain English word
// ("Wait", "Read", "Open", ...) that also happens to be some unrelated
// library's function name, and linkifying those would be surprising
// rather than helpful.
//
QString AutodocReader::renderEntryHtml(int idx) const
{
    if (idx < 0 || idx >= p_entries.size())
        return QString();

    static const QRegularExpression headerRe(QStringLiteral("^[A-Z][A-Z0-9 /]*$"));

    const QString &fullText = p_entries.at(idx).fullText;
    const QStringList lines = fullText.split(QLatin1Char('\n'));

    QString html;
    html.reserve(fullText.size() + 64);
    html += QStringLiteral("<pre style=\"white-space:pre; margin:0;\">");

    bool inSeeAlso = false;
    for (const QString &rawLine : lines)
    {
        const QString trimmed = rawLine.trimmed();
        const bool startsWithTab = !rawLine.isEmpty() && rawLine.at(0) == QLatin1Char('\t');
        const bool isHeaderLine = !trimmed.isEmpty()
                                    && !startsWithTab
                                    && headerRe.match(trimmed).hasMatch();

        const QString escapedLine = rawLine.toHtmlEscaped();

        if (isHeaderLine)
        {
            // The header word itself is never linkified, only what
            // follows it - see linkifySeeAlsoLine().
            inSeeAlso = (trimmed.compare(QStringLiteral("SEE ALSO"), Qt::CaseInsensitive) == 0);
            html += escapedLine;
        }
        else
        {
            html += inSeeAlso ? linkifySeeAlsoLine(escapedLine) : escapedLine;
        }

        html += QLatin1Char('\n');
    }

    html += QStringLiteral("</pre>");
    return html;
}

//
// Turns every recognized function-name token in one already-HTML-escaped
// SEE ALSO line into a clickable link (href "adoc:<shortName>", handled
// by onSeeAlsoLinkClicked()). A token may be a bare short name
// ("CMD_WRITE") or a qualified one ("exec.library/Wait") - either way,
// only its short name (the part after the last "/", if any) needs to
// match a parsed entry for the WHOLE token to become the link's text.
//
// Matching directly against the already-escaped line (rather than
// escaping after matching) is safe: HTML-escaping only ever touches &,
// <, >, " - none of which are valid characters in an Amiga function or
// library identifier - so it can never shift or split a token match.
//
QString AutodocReader::linkifySeeAlsoLine(const QString &escapedLine) const
{
    static const QRegularExpression tokenRe(QStringLiteral("[A-Za-z_][A-Za-z0-9_./]*"));

    QString result;
    result.reserve(escapedLine.size() + 32);
    int lastEnd = 0;

    QRegularExpressionMatchIterator it = tokenRe.globalMatch(escapedLine);
    while (it.hasNext())
    {
        const QRegularExpressionMatch m = it.next();
        result += escapedLine.mid(lastEnd, m.capturedStart() - lastEnd);

        QString candidate = m.captured(0);
        // A lone trailing "." is virtually always sentence punctuation
        // ("...see CMD_WRITE.") rather than part of the name - strip it
        // before looking the name up, and put it back outside the link
        // (if there was one) either way.
        const bool hadTrailingDot = candidate.endsWith(QLatin1Char('.'));
        if (hadTrailingDot)
            candidate.chop(1);

        const int slash = candidate.lastIndexOf(QLatin1Char('/'));
        const QString shortName = (slash >= 0) ? candidate.mid(slash + 1) : candidate;

        if (!shortName.isEmpty() && p_nameToIndex.contains(shortName.toLower()))
        {
            result += QStringLiteral("<a href=\"adoc:") + shortName.toHtmlEscaped() + QStringLiteral("\">")
                     + candidate + QStringLiteral("</a>");
            if (hadTrailingDot)
                result += QLatin1Char('.');
        }
        else
        {
            result += m.captured(0);   // no match - leave the token exactly as found
        }

        lastEnd = m.capturedEnd();
    }

    result += escapedLine.mid(lastEnd);
    return result;
}

//
// Handles a click on one of the "SEE ALSO" links renderEntryHtml()/
// linkifySeeAlsoLine() add to the text view (href "adoc:<shortName>") -
// setOpenLinks(false) on p_textView (see constructor) is what stops
// QTextBrowser from trying to navigate to that URL itself first, since
// it isn't a real document source, just a name to jump to.
//
// Reuses showFunction() itself rather than duplicating any of its
// lookup/expand/select/scroll/filter-clearing logic, so a SEE ALSO click
// behaves exactly like "Jump to Explanation" finding the same name from
// the editor - except it also passes the CURRENTLY shown entry's own
// library as showFunction()'s preferredLibrary: an unqualified SEE ALSO
// reference like "CMD_WRITE" is, by AmigaOS AutoDoc convention, the
// CMD_WRITE of that very same library, not just whichever library's
// CMD_WRITE happens to sort first in the tree (e.g. carddisk.device's own
// SEE ALSO "CMD_WRITE" must jump to carddisk.device/CMD_WRITE, not
// audio.device/CMD_WRITE just because "audio" sorts before "carddisk").
//
void AutodocReader::onSeeAlsoLinkClicked(const QUrl &link)
{
    static const QString scheme = QStringLiteral("adoc:");

    const QString url = link.toString();
    if (!url.startsWith(scheme))
        return;

    QString preferredLibrary;
    if (QTreeWidgetItem *current = p_tree->currentItem())
    {
        if (current->parent())   // a group header carries no entry index - see onTreeSelectionChanged()
        {
            const int idx = current->data(0, Qt::UserRole).toInt();
            if (idx >= 0 && idx < p_entries.size())
                preferredLibrary = p_entries.at(idx).library;
        }
    }

    showFunction(url.mid(scheme.length()), preferredLibrary);
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
