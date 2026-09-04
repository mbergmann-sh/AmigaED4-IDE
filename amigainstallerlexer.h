#ifndef AMIGAINSTALLERLEXER_H
#define AMIGAINSTALLERLEXER_H

#include <Qsci/qscilexercustom.h>
#include <QSet>
#include <QString>

//
// Custom QScintilla lexer for the Amiga "Installer" scripting language
// (as used by Commodore's Installer utility, first introduced in
// AmigaOS 2.1 - the interpreter behind Installer.guide/Installer scripts
// shipped with practically every Amiga software package).
//
// The language is LISP-derived: fully parenthesized prefix expressions,
// ';' end-of-line comments, double-quoted strings, '@name' predefined
// symbols (e.g. used for help texts: '@makedir-help'), and '#name'
// user-defined script variables (set via '(set #var ...)'). Example
// from the original Commodore developer documentation:
//
//   (makedir "T:fred"
//     (prompt "I will now create the directory \"T:Fred\"")
//     (help @makedir-help)
//     (infos)
//     (confirm)
//   )
//
// NOTE on the keyword list: it reflects the commonly documented core
// set of Installer script commands (abort/complete/copyfiles/copylib/
// database/delete/deltree/exists/execute/exit/getversion/if/makedir/
// message/onerror/procedure/protect/prompt/run/set/textfile/welcome/
// the various 'ask...' requesters, etc.). It is not cross-checked
// against Commodore's original Installer.guide document byte for byte -
// if you spot a missing or wrong command while using this, the keyword
// list below is the only place that needs extending.
//
class AmigaInstallerLexer : public QsciLexerCustom
{
    Q_OBJECT

public:
    // Style numbers used by styleText() - also referenced by
    // defaultColor()/defaultFont()/description() below.
    enum Style
    {
        Default  = 0,
        Comment  = 1,
        String   = 2,
        Keyword  = 3,
        Operator = 4,   // parentheses
        Number   = 5,
        Symbol   = 6,   // '@name' predefined symbols
        Variable = 7    // '#name' user script variables
    };

    explicit AmigaInstallerLexer(QObject *parent = nullptr);

    const char *language() const override;
    QString description(int style) const override;
    QColor defaultColor(int style) const override;
    QFont defaultFont(int style) const override;

    void styleText(int start, int end) override;

private:
    static bool isIdentChar(QChar c);
    static const QSet<QString> &keywords();
};

#endif // AMIGAINSTALLERLEXER_H
