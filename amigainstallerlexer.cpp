#include "amigainstallerlexer.h"
#include <Qsci/qsciscintilla.h>

AmigaInstallerLexer::AmigaInstallerLexer(QObject *parent)
    : QsciLexerCustom(parent)
{
}

const char *AmigaInstallerLexer::language() const
{
    return "Amiga Installer";
}

QString AmigaInstallerLexer::description(int style) const
{
    switch (style)
    {
    case Default:  return QObject::tr("Default");
    case Comment:  return QObject::tr("Comment");
    case String:   return QObject::tr("String");
    case Keyword:  return QObject::tr("Keyword");
    case Operator: return QObject::tr("Operator");
    case Number:   return QObject::tr("Number");
    case Symbol:   return QObject::tr("Predefined symbol (@...)");
    case Variable: return QObject::tr("Variable (#...)");
    default:       return QString();
    }
}

QColor AmigaInstallerLexer::defaultColor(int style) const
{
    switch (style)
    {
    case Comment:  return QColor(0x00, 0x80, 0x00);   // dark green
    case String:   return QColor(0x80, 0x00, 0x00);   // maroon
    case Keyword:  return QColor(0x00, 0x00, 0xff);   // blue
    case Operator: return QColor(0x40, 0x40, 0x40);   // dark grey
    case Number:   return QColor(0x00, 0x80, 0x80);   // teal
    case Symbol:   return QColor(0x80, 0x00, 0x80);   // purple - @help/predefined symbols
    case Variable: return QColor(0xb8, 0x86, 0x0b);   // dark goldenrod - #user variables
    default:       return QsciLexerCustom::defaultColor(style);
    }
}

QFont AmigaInstallerLexer::defaultFont(int style) const
{
    QFont f = QsciLexerCustom::defaultFont(style);

    if (style == Keyword)
        f.setBold(true);

    return f;
}

bool AmigaInstallerLexer::isIdentChar(QChar c)
{
    return c.isLetterOrNumber() || c == '_' || c == '-';
}

const QSet<QString> &AmigaInstallerLexer::keywords()
{
    // All lower-case - matching against the lower-cased token in
    // styleText(), since Installer scripts are conventionally written
    // in lower case but the interpreter itself is case-insensitive.
    static const QSet<QString> kw = {
        // control flow / structure
        "if", "while", "procedure", "onerror", "abort", "exit", "complete",
        "skip", "quiet",
        // requesters ("ask...")
        "askbool", "askchoice", "askdir", "askdisk", "askfile", "asknumber",
        "askoptions", "askstring", "welcome", "message", "prompt", "help",
        // file/directory operations
        "copyfiles", "copylib", "delete", "deltree", "makedir", "makeassign",
        "rename", "protect", "textfile", "patch", "tooltype",
        // system / environment queries
        "exists", "getversion", "getassign", "getdevice", "getdiskspace",
        "getenv", "getsize", "run", "execute", "startup", "user", "working",
        "database",
        // string / path / value helpers
        "cat", "expandpath", "pathonly", "tackon", "strequal", "string",
        "substr", "select", "includes", "in", "and", "or", "not",
        // variable / value handling
        "set", "put", "parse", "orlist", "midfile", "transcript",
        "showlog", "delopts", "all", "confirm", "infos"
    };
    return kw;
}

void AmigaInstallerLexer::styleText(int start, int end)
{
    if (!editor())
        return;

    // Read the raw source once. AmigaED's editor treats source files as
    // 8-bit (classic Amiga Latin-1 text, see loadFile()), i.e. Scintilla
    // is not running in UTF-8 mode here - so byte offsets from Scintilla
    // (start/end) map 1:1 onto QString character indices, and we can
    // safely use plain QString/int index arithmetic below instead of
    // going through SCI_GETTEXTRANGE byte-offset juggling.
    const QString source = editor()->text().mid(start, end - start);
    const int n = source.length();

    startStyling(start);

    int i = 0;
    while (i < n)
    {
        const QChar c = source.at(i);

        if (c == ';')
        {
            // Comment: ';' to end of line.
            int j = i;
            while (j < n && source.at(j) != QLatin1Char('\n'))
                ++j;
            setStyling(j - i, Comment);
            i = j;
        }
        else if (c == QLatin1Char('"'))
        {
            // String: double-quoted, backslash-escaped (e.g. \" \n).
            int j = i + 1;
            while (j < n && source.at(j) != QLatin1Char('"'))
            {
                if (source.at(j) == QLatin1Char('\\') && j + 1 < n)
                    ++j; // skip the escaped character
                ++j;
            }
            if (j < n)
                ++j; // include the closing quote
            setStyling(j - i, String);
            i = j;
        }
        else if (c == QLatin1Char('(') || c == QLatin1Char(')'))
        {
            setStyling(1, Operator);
            ++i;
        }
        else if (c == QLatin1Char('@'))
        {
            // Predefined symbol, e.g. @makedir-help, @app-name.
            int j = i + 1;
            while (j < n && isIdentChar(source.at(j)))
                ++j;
            setStyling(j - i, Symbol);
            i = j;
        }
        else if (c == QLatin1Char('#'))
        {
            // User-defined script variable, e.g. #dest, #copy-files.
            int j = i + 1;
            while (j < n && isIdentChar(source.at(j)))
                ++j;
            setStyling(j - i, Variable);
            i = j;
        }
        else if (c.isDigit())
        {
            int j = i;
            while (j < n && source.at(j).isDigit())
                ++j;
            setStyling(j - i, Number);
            i = j;
        }
        else if (c.isLetter() || c == QLatin1Char('_'))
        {
            int j = i;
            while (j < n && isIdentChar(source.at(j)))
                ++j;
            const QString word = source.mid(i, j - i);
            setStyling(j - i, keywords().contains(word.toLower()) ? Keyword : Default);
            i = j;
        }
        else
        {
            // whitespace and anything else not specially handled
            setStyling(1, Default);
            ++i;
        }
    }
}
