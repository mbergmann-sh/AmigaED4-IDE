#include "m68kasmlexer.h"
#include <Qsci/qsciscintilla.h>

M68kAsmLexer::M68kAsmLexer(QObject *parent)
    : QsciLexerCustom(parent)
{
}

const char *M68kAsmLexer::language() const
{
    return "m68k Assembler";
}

QString M68kAsmLexer::description(int style) const
{
    switch (style)
    {
    case Default:   return QObject::tr("Default");
    case Comment:   return QObject::tr("Comment");
    case String:    return QObject::tr("String");
    case Mnemonic:  return QObject::tr("Mnemonic");
    case Directive: return QObject::tr("Directive");
    case Register:  return QObject::tr("Register");
    case Number:    return QObject::tr("Number");
    case Label:     return QObject::tr("Label");
    default:        return QString();
    }
}

QColor M68kAsmLexer::defaultColor(int style) const
{
    switch (style)
    {
    case Comment:   return QColor(0x00, 0x80, 0x00);   // dark green
    case String:    return QColor(0x80, 0x00, 0x00);   // maroon
    case Mnemonic:  return QColor(0x00, 0x00, 0xff);   // blue
    case Directive: return QColor(0x80, 0x00, 0x80);   // purple
    case Register:  return QColor(0xb2, 0x22, 0x22);   // firebrick
    case Number:    return QColor(0x00, 0x80, 0x80);   // teal
    case Label:     return QColor(0xb8, 0x86, 0x0b);   // dark goldenrod
    default:        return QsciLexerCustom::defaultColor(style);
    }
}

QFont M68kAsmLexer::defaultFont(int style) const
{
    QFont f = QsciLexerCustom::defaultFont(style);

    if (style == Mnemonic || style == Label)
        f.setBold(true);

    return f;
}

bool M68kAsmLexer::isIdentStart(QChar c)
{
    return c.isLetter() || c == '_' || c == '.' || c == '@';
}

bool M68kAsmLexer::isIdentChar(QChar c)
{
    return c.isLetterOrNumber() || c == '_' || c == '.' || c == '@' || c == '$';
}

const QSet<QString> &M68kAsmLexer::mnemonics()
{
    // Matched against the token with any trailing '.b'/'.w'/'.l'/'.s'
    // size suffix already stripped off (see styleText()).
    static const QSet<QString> kw = {
        // data movement
        "move", "movea", "movem", "movep", "moveq", "lea", "pea",
        "link", "unlk", "exg", "swap", "clr", "ext", "extb",
        // arithmetic
        "add", "adda", "addi", "addq", "addx",
        "sub", "suba", "subi", "subq", "subx",
        "neg", "negx", "cmp", "cmpa", "cmpi", "cmpm",
        "muls", "mulu", "divs", "divu", "abcd", "sbcd", "nbcd",
        // logic
        "and", "andi", "or", "ori", "eor", "eori", "not",
        // shift / rotate
        "asl", "asr", "lsl", "lsr", "rol", "ror", "roxl", "roxr",
        // bit manipulation
        "btst", "bset", "bclr", "bchg",
        // program control - branches/set-on-condition (already spelled
        // out with their condition code, not stripped/composed)
        "bra", "bsr",
        "beq", "bne", "bcc", "bcs", "bhi", "bls", "bge", "blt",
        "bgt", "ble", "bpl", "bmi", "bvc", "bvs", "bhs", "blo",
        "dbra", "dbf", "dbt",
        "dbeq", "dbne", "dbcc", "dbcs", "dbhi", "dbls", "dbge",
        "dblt", "dbgt", "dble", "dbpl", "dbmi", "dbvc", "dbvs",
        "seq", "sne", "scc", "scs", "shi", "sls", "sge", "slt",
        "sgt", "sle", "spl", "smi", "svc", "svs", "st", "sf",
        "jmp", "jsr", "rts", "rte", "rtr", "trap", "trapv",
        "chk", "tas",
        // misc
        "nop", "reset", "stop", "illegal"
    };
    return kw;
}

const QSet<QString> &M68kAsmLexer::directives()
{
    static const QSet<QString> kw = {
        "dc", "ds", "dcb", "equ", "org", "section", "even", "odd",
        "cnop", "include", "incbin", "incdir", "macro", "endm", "mexit",
        "rept", "endr", "if", "ifeq", "ifne", "ifgt", "iflt", "ifd",
        "ifnd", "else", "endif", "public", "xdef", "xref", "extern",
        "global", "list", "nolist", "module", "end", "fail", "opt"
    };
    return kw;
}

const QSet<QString> &M68kAsmLexer::registers()
{
    static const QSet<QString> kw = {
        "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
        "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
        "sp", "usp", "ssp", "pc", "sr", "ccr"
    };
    return kw;
}

void M68kAsmLexer::styleText(int start, int end)
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
    // True as long as nothing (not even whitespace) has been consumed
    // yet on the current line - only a token starting here counts as a
    // column-zero LABEL.
    bool columnZero = true;
    // True once this line's first real token (label aside) has been
    // classified as mnemonic/directive/unrecognized - every token after
    // that is an operand, not another opcode.
    bool haveMnemonic = false;

    while (i < n)
    {
        const QChar c = source.at(i);

        if (c == QLatin1Char('\n'))
        {
            setStyling(1, Default);
            ++i;
            columnZero = true;
            haveMnemonic = false;
        }
        else if (c == QLatin1Char(';') || (c == QLatin1Char('*') && columnZero))
        {
            // ';' starts an end-of-line comment anywhere; a '*' as the
            // very first character of a line is a whole-line comment -
            // a traditional Motorola-assembler convention (NOT '*' used
            // as a multiply operator, which never appears in column
            // zero anyway).
            int j = i;
            while (j < n && source.at(j) != QLatin1Char('\n'))
                ++j;
            setStyling(j - i, Comment);
            i = j;
            columnZero = false;
        }
        else if (c == QLatin1Char('"') || c == QLatin1Char('\''))
        {
            QChar quote = c;
            int j = i + 1;
            while (j < n && source.at(j) != quote && source.at(j) != QLatin1Char('\n'))
            {
                if (source.at(j) == QLatin1Char('\\') && j + 1 < n)
                    ++j; // skip the escaped character
                ++j;
            }
            if (j < n && source.at(j) == quote)
                ++j; // include the closing quote
            setStyling(j - i, String);
            i = j;
            columnZero = false;
        }
        else if (c.isSpace())
        {
            setStyling(1, Default);
            ++i;
            columnZero = false;   // a token following whitespace is never a label
        }
        else if (c == QLatin1Char('$') || c == QLatin1Char('%') || c.isDigit())
        {
            // $hex / %binary / decimal (a leading '@' - octal - is
            // handled below as part of the general identifier branch,
            // since '@' also starts local-label names in some
            // assemblers - only style it as Number if what follows are
            // octal digits).
            int j = i + 1;
            while (j < n && (source.at(j).isLetterOrNumber()))
                ++j;
            setStyling(j - i, Number);
            i = j;
            columnZero = false;
        }
        else if (isIdentStart(c))
        {
            int j = i + 1;
            while (j < n && isIdentChar(source.at(j)))
                ++j;
            QString word = source.mid(i, j - i);

            if (columnZero && !haveMnemonic)
            {
                // Column-zero token: a label definition, whether or not
                // it happens to also spell a mnemonic/directive name.
                int end2 = j;
                if (end2 < n && source.at(end2) == QLatin1Char(':'))
                    ++end2;   // include an explicit "Label:" colon
                setStyling(end2 - i, Label);
                i = end2;
            }
            else if (!haveMnemonic)
            {
                QString bare = word;
                // Strip a trailing size suffix (.b/.w/.l/.s) before
                // matching against the mnemonic list, e.g. "move.l".
                static const QSet<QString> sizes = { "b", "w", "l", "s" };
                int dot = bare.lastIndexOf(QLatin1Char('.'));
                if (dot > 0 && sizes.contains(bare.mid(dot + 1).toLower()))
                    bare = bare.left(dot);

                QString lower = bare.toLower();
                if (mnemonics().contains(lower))
                    setStyling(j - i, Mnemonic);
                else if (directives().contains(lower))
                    setStyling(j - i, Directive);
                else
                    setStyling(j - i, Default);
                haveMnemonic = true;
                i = j;
            }
            else
            {
                // Operand position.
                if (registers().contains(word.toLower()))
                    setStyling(j - i, Register);
                else
                    setStyling(j - i, Default);
                i = j;
            }
            columnZero = false;
        }
        else
        {
            setStyling(1, Default);
            ++i;
            columnZero = false;
        }
    }
}
