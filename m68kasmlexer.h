#ifndef M68KASMLEXER_H
#define M68KASMLEXER_H

#include <Qsci/qscilexercustom.h>
#include <QSet>
#include <QString>

//
// Custom QScintilla lexer for m68k assembler source (.asm/.s), as
// accepted by vasm (used by vbcc) and the GNU assembler bundled with
// m68k-amigaos-gcc. Not QScintilla's built-in QsciLexerAsm on purpose -
// that one has no concept of 68000-family mnemonics, registers or the
// '$'/'%'/'@' number prefixes Motorola-syntax assemblers use, so its
// keyword highlighting would recognize nothing useful here.
//
// Classic Motorola-syntax assembler line shape:
//
//   Label:      move.l  #$1000,d0      ; comment
//               lea     Table(pc),a0
//   * a whole-line comment, alternative to leading ';'
//
// A LABEL is whatever starts in column zero (no leading whitespace) on
// a line; a MNEMONIC or DIRECTIVE is the first identifier token after
// leading whitespace (optionally following a label on the same line);
// everything else on the line is an operand - registers (d0-d7/a0-a7/
// sp/usp/pc/sr/ccr) get their own style, numbers ($hex/%binary/@octal/
// decimal) get theirs, and anything unrecognized is left as Default.
//
// NOTE on the mnemonic/directive lists: they cover the common
// 68000/68020 instruction set and vasm/devpac-style directives - not
// cross-checked byte for byte against a full 68020+ or every possible
// assembler's own directive dialect. If you spot a missing or wrong
// one while using this, the keyword lists below are the only place
// that needs extending.
//
class M68kAsmLexer : public QsciLexerCustom
{
    Q_OBJECT

public:
    // Style numbers used by styleText() - also referenced by
    // defaultColor()/defaultFont()/description() below.
    enum Style
    {
        Default   = 0,
        Comment   = 1,
        String    = 2,
        Mnemonic  = 3,   // instruction opcode (move.l, lea, bra, dbra, ...)
        Directive = 4,   // assembler directive (dc.l, equ, section, macro, ...)
        Register  = 5,   // d0-d7 / a0-a7 / sp / usp / pc / sr / ccr
        Number    = 6,   // decimal, $hex, %binary, @octal
        Label     = 7    // a column-zero symbol definition
    };

    explicit M68kAsmLexer(QObject *parent = nullptr);

    const char *language() const override;
    QString description(int style) const override;
    QColor defaultColor(int style) const override;
    QFont defaultFont(int style) const override;

    void styleText(int start, int end) override;

private:
    static bool isIdentStart(QChar c);
    static bool isIdentChar(QChar c);
    static const QSet<QString> &mnemonics();
    static const QSet<QString> &directives();
    static const QSet<QString> &registers();
};

#endif // M68KASMLEXER_H
