#ifndef AMIGAGUIDELEXER_H
#define AMIGAGUIDELEXER_H

#include <Qsci/qscilexercustom.h>
#include <QSet>
#include <QString>

//
// Custom QScintilla lexer for AmigaGuide files (Commodore's native
// hypertext help format, suffix ".guide" - the format behind AmigaOS's
// own Help system, Installer.guide, and countless third-party program
// manuals).
//
// The format is plain text interspersed with '@'-prefixed commands:
//
//   @DATABASE MyProgram.guide
//   @(NODE) Main "MyProgram Help"
//   @{B}Welcome@{UB} to MyProgram!
//
//   See also: @{"Introduction" LINK Intro}
//   @ENDNODE
//
// Two different '@' forms exist:
//  - line-level commands (@node/@endnode/@title/@author/@remark/@width/
//    ...) - these configure the node/database as a whole and normally
//    start a line
//  - inline escape sequences '@{...}' - text styling (@{b}/@{ub}/@{i}/
//    @{u}/...) and hyperlinks (@{"link text" LINK NodeName}) embedded
//    directly in the browsable body text
//
// NOTE on the keyword list: it reflects the commonly documented core
// set of AmigaGuide commands (per Commodore's original AmigaGuide
// developer documentation and the amigaguide.library autodocs) - not
// cross-checked byte for byte against every possible variant. If you
// spot a missing or wrong command while using this, the keyword list
// below is the only place that needs extending.
//
class AmigaGuideLexer : public QsciLexerCustom
{
    Q_OBJECT

public:
    // Style numbers used by styleText() - also referenced by
    // defaultColor()/defaultFont()/description() below.
    enum Style
    {
        Default  = 0,
        Command  = 1,   // line-level '@command' (e.g. @node, @width, @author)
        String   = 2,   // "quoted text" - node names, link titles, titles
        Link     = 3,   // an entire '@{...}' inline escape sequence
        Comment  = 4    // '@remark' and the rest of its line
    };

    explicit AmigaGuideLexer(QObject *parent = nullptr);

    const char *language() const override;
    QString description(int style) const override;
    QColor defaultColor(int style) const override;
    QFont defaultFont(int style) const override;

    void styleText(int start, int end) override;

private:
    static bool isCommandChar(QChar c);
    static const QSet<QString> &commands();
};

#endif // AMIGAGUIDELEXER_H
