#include "amigaguidelexer.h"
#include <Qsci/qsciscintilla.h>

AmigaGuideLexer::AmigaGuideLexer(QObject *parent)
    : QsciLexerCustom(parent)
{
}

const char *AmigaGuideLexer::language() const
{
    return "AmigaGuide";
}

QString AmigaGuideLexer::description(int style) const
{
    switch (style)
    {
    case Default: return QObject::tr("Default");
    case Command: return QObject::tr("Command (@node, @title, ...)");
    case String:  return QObject::tr("String");
    case Link:    return QObject::tr("Inline escape / link (@{...})");
    case Comment: return QObject::tr("Comment (@remark)");
    default:      return QString();
    }
}

QColor AmigaGuideLexer::defaultColor(int style) const
{
    switch (style)
    {
    case Command: return QColor(0x00, 0x00, 0xff);   // blue
    case String:  return QColor(0x80, 0x00, 0x00);   // maroon
    case Link:    return QColor(0x80, 0x00, 0x80);   // purple - hyperlinks/inline styling
    case Comment: return QColor(0x00, 0x80, 0x00);   // dark green
    default:      return QsciLexerCustom::defaultColor(style);
    }
}

QFont AmigaGuideLexer::defaultFont(int style) const
{
    QFont f = QsciLexerCustom::defaultFont(style);

    if (style == Command)
        f.setBold(true);

    return f;
}

bool AmigaGuideLexer::isCommandChar(QChar c)
{
    return c.isLetter();
}

const QSet<QString> &AmigaGuideLexer::commands()
{
    // All lower-case - matched against the lower-cased command word in
    // styleText(), since AmigaGuide commands are conventionally written
    // upper-case (@NODE) or lower-case (@node) interchangeably.
    static const QSet<QString> cmds = {
        // database / node structure
        "database", "node", "endnode", "master", "deftab", "defext",
        "defhelp",
        // node/database metadata
        "title", "author", "copyright", "index", "toc", "next", "prev",
        "keywords", "help", "xref",
        // layout
        "width", "height", "font", "tab", "wordwrap", "smartwrap",
        // comment
        "remark",
        // event hooks
        "onopen", "onclose", "embed", "image"
    };
    return cmds;
}

void AmigaGuideLexer::styleText(int start, int end)
{
    if (!editor())
        return;

    // See AmigaInstallerLexer::styleText() for why plain QString/int
    // index arithmetic is safe here (AmigaED's editor treats source
    // files as 8-bit text, not UTF-8 - byte offsets from Scintilla map
    // 1:1 onto QString character indices).
    const QString source = editor()->text().mid(start, end - start);
    const int n = source.length();

    startStyling(start);

    int i = 0;
    while (i < n)
    {
        const QChar c = source.at(i);

        if (c == QLatin1Char('@') && i + 1 < n && source.at(i + 1) == QLatin1Char('{'))
        {
            // Inline escape sequence: '@{' up to the matching '}' (no
            // nesting in AmigaGuide, so the first '}' always closes it).
            // A quoted link title inside it (e.g. @{"Intro" LINK Intro})
            // is sub-styled as String for readability; everything else
            // in the sequence (including the leading '@{'/trailing '}')
            // is styled as Link.
            int j = i + 2;
            setStyling(2, Link);   // the '@{' itself
            while (j < n && source.at(j) != QLatin1Char('}'))
            {
                if (source.at(j) == QLatin1Char('"'))
                {
                    int k = j + 1;
                    while (k < n && source.at(k) != QLatin1Char('"') && source.at(k) != QLatin1Char('\n'))
                    {
                        if (source.at(k) == QLatin1Char('\\') && k + 1 < n)
                            ++k; // skip the escaped character
                        ++k;
                    }
                    if (k < n && source.at(k) == QLatin1Char('"'))
                        ++k; // include the closing quote
                    setStyling(k - j, String);
                    j = k;
                }
                else
                {
                    setStyling(1, Link);
                    ++j;
                }
            }
            if (j < n)
            {
                setStyling(1, Link);   // the closing '}'
                ++j;
            }
            i = j;
        }
        else if (c == QLatin1Char('@') && i + 1 < n && isCommandChar(source.at(i + 1)))
        {
            // Line-level command, e.g. @node, @endnode, @remark, @title.
            int j = i + 1;
            while (j < n && isCommandChar(source.at(j)))
                ++j;
            const QString word = source.mid(i + 1, j - i - 1);
            bool isRemark = (word.compare(QLatin1String("remark"), Qt::CaseInsensitive) == 0);
            setStyling(j - i, (isRemark || commands().contains(word.toLower())) ? Command : Default);

            if (isRemark)
            {
                // @remark's own argument is a plain comment: the rest of
                // the line, not further command/string parsing.
                int k = j;
                while (k < n && source.at(k) != QLatin1Char('\n'))
                    ++k;
                setStyling(k - j, Comment);
                j = k;
            }
            i = j;
        }
        else if (c == QLatin1Char('"'))
        {
            // A quoted string outside an @{...} escape, e.g. the node
            // name/title arguments on an @node line.
            int j = i + 1;
            while (j < n && source.at(j) != QLatin1Char('"') && source.at(j) != QLatin1Char('\n'))
            {
                if (source.at(j) == QLatin1Char('\\') && j + 1 < n)
                    ++j; // skip the escaped character
                ++j;
            }
            if (j < n && source.at(j) == QLatin1Char('"'))
                ++j; // include the closing quote
            setStyling(j - i, String);
            i = j;
        }
        else
        {
            // Plain browsable body text (and anything else not specially
            // handled) - batch consecutive characters up to the next
            // '@' or '"' into one Default-styled run.
            int j = i;
            while (j < n && source.at(j) != QLatin1Char('@') && source.at(j) != QLatin1Char('"'))
                ++j;
            if (j == i)
                ++j; // a lone stray '@'/'"' that didn't match the branches above
            setStyling(j - i, Default);
            i = j;
        }
    }
}
