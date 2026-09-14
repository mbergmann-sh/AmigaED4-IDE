// cpprefdata.cpp - CppRefReader::populateKeywords()/populateDatatypes()/
// populateVariables()/populateDecisions()/populateControlStructures()/
// populateOperators()/populateFunctions()
// (rev.157, Help > C/C++ - all 7 categories)
//
// Generated content - every entry is authored in
// docbuild/../../scratchpad/cppref/gen.py (kept outside the repo, not
// shipped) and emitted here as plain addEntry() calls; see cpprefreader.h
// for what each field means and cpprefreader.cpp for how these are used
// (buildTree(), renderEntryHtml(), the "cppref:<id>" cross-reference
// links onCrossRefLinkClicked() resolves).
//
// To regenerate after editing the source content: rerun gen.py and
// replace this file's contents with its output.

#include "cpprefreader.h"

void CppRefReader::populateKeywords()
{
    addEntry(QStringLiteral("kw_if"), QStringLiteral("Keywords"), QStringLiteral("if"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Executes a block only if a condition is true. See the Decisions category for the full if/else family.</p><pre style="white-space:pre;">if (count &gt; 0)
{
    printf("count is positive\n");
}</pre><p><b>See also:</b> <a href="cppref:kw_else">else</a>, <a href="cppref:dec_if_else">if / if-else</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Führt einen Block nur aus, wenn eine Bedingung wahr ist. Die vollständige if/else-Familie steht in der Kategorie Decisions.</p><pre style="white-space:pre;">if (count &gt; 0)
{
    printf("count is positive\n");
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_else">else</a>, <a href="cppref:dec_if_else">if / if-else</a></p>)HTML"));

    addEntry(QStringLiteral("kw_else"), QStringLiteral("Keywords"), QStringLiteral("else"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Pairs with a preceding if to provide the branch taken when that if's condition was false.</p><pre style="white-space:pre;">if (count &gt; 0)
{
    printf("positive\n");
}
else
{
    printf("zero or negative\n");
}</pre><p><b>See also:</b> <a href="cppref:kw_if">if</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Gehört zu einem vorangehenden if und liefert den Zweig, der ausgeführt wird, wenn dessen Bedingung falsch war.</p><pre style="white-space:pre;">if (count &gt; 0)
{
    printf("positive\n");
}
else
{
    printf("zero or negative\n");
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_if">if</a></p>)HTML"));

    addEntry(QStringLiteral("kw_for"), QStringLiteral("Keywords"), QStringLiteral("for"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>A counting loop: initializer, condition, and increment are all written on one line, then evaluated in that order once per iteration.</p><pre style="white-space:pre;">for (int i = 0; i &lt; 10; i++)
{
    printf("%d\n", i);
}</pre><p><b>See also:</b> <a href="cppref:kw_while">while</a>, <a href="cppref:kw_break">break</a>, <a href="cppref:kw_continue">continue</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Eine Zählschleife: Initialisierung, Bedingung und Inkrement stehen in einer Zeile und werden pro Durchlauf in genau dieser Reihenfolge ausgewertet.</p><pre style="white-space:pre;">for (int i = 0; i &lt; 10; i++)
{
    printf("%d\n", i);
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_while">while</a>, <a href="cppref:kw_break">break</a>, <a href="cppref:kw_continue">continue</a></p>)HTML"));

    addEntry(QStringLiteral("kw_while"), QStringLiteral("Keywords"), QStringLiteral("while"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Repeats a block for as long as its condition stays true, tested BEFORE each iteration - the block may run zero times.</p><pre style="white-space:pre;">int i = 0;
while (i &lt; 10)
{
    printf("%d\n", i);
    i++;
}</pre><p><b>See also:</b> <a href="cppref:kw_do">do</a>, <a href="cppref:kw_for">for</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Wiederholt einen Block, solange seine Bedingung wahr bleibt; die Prüfung erfolgt VOR jedem Durchlauf – der Block kann also auch null Mal laufen.</p><pre style="white-space:pre;">int i = 0;
while (i &lt; 10)
{
    printf("%d\n", i);
    i++;
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_do">do</a>, <a href="cppref:kw_for">for</a></p>)HTML"));

    addEntry(QStringLiteral("kw_do"), QStringLiteral("Keywords"), QStringLiteral("do...while"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Like while, but the condition is tested AFTER each iteration, so the block always runs at least once. Written as do { ... } while (condition);</p><pre style="white-space:pre;">int i = 0;
do
{
    printf("%d\n", i);
    i++;
} while (i &lt; 10);</pre><p><b>See also:</b> <a href="cppref:kw_while">while</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Wie while, nur wird die Bedingung NACH jedem Durchlauf geprüft – der Block läuft also immer mindestens einmal. Schreibweise: do { ... } while (Bedingung);</p><pre style="white-space:pre;">int i = 0;
do
{
    printf("%d\n", i);
    i++;
} while (i &lt; 10);</pre><p><b>Siehe auch:</b> <a href="cppref:kw_while">while</a></p>)HTML"));

    addEntry(QStringLiteral("kw_switch"), QStringLiteral("Keywords"), QStringLiteral("switch"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Selects one of several code paths by comparing one value against a list of constant case labels - see the Control Structures category.</p><pre style="white-space:pre;">switch (compiler)
{
    case COMPILER_VBCC:
        printf("vbcc\n");
        break;
    case COMPILER_GCC:
        printf("gcc\n");
        break;
    default:
        printf("unknown\n");
        break;
}</pre><p><b>See also:</b> <a href="cppref:kw_case">case</a>, <a href="cppref:kw_default">default</a>, <a href="cppref:kw_break">break</a>, <a href="cppref:cs_switch_case">switch / case</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Wählt einen von mehreren Codepfaden aus, indem ein Wert mit einer Liste konstanter case-Marken verglichen wird – siehe Kategorie Control Structures.</p><pre style="white-space:pre;">switch (compiler)
{
    case COMPILER_VBCC:
        printf("vbcc\n");
        break;
    case COMPILER_GCC:
        printf("gcc\n");
        break;
    default:
        printf("unknown\n");
        break;
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_case">case</a>, <a href="cppref:kw_default">default</a>, <a href="cppref:kw_break">break</a>, <a href="cppref:cs_switch_case">switch / case</a></p>)HTML"));

    addEntry(QStringLiteral("kw_case"), QStringLiteral("Keywords"), QStringLiteral("case"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Labels one branch inside a switch statement - execution jumps straight to the first matching case, then keeps running into the following ones unless a break stops it (see "fall-through").</p><pre style="white-space:pre;">switch (n)
{
    case 1:
    case 2:
        printf("one or two\n");
        break;
    default:
        printf("something else\n");
}</pre><p><b>See also:</b> <a href="cppref:kw_switch">switch</a>, <a href="cppref:kw_break">break</a>, <a href="cppref:kw_default">default</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Markiert einen Zweig innerhalb einer switch-Anweisung – die Ausführung springt direkt zum ersten passenden case und läuft dann, sofern kein break stoppt, in die folgenden weiter ("Fall-Through").</p><pre style="white-space:pre;">switch (n)
{
    case 1:
    case 2:
        printf("one or two\n");
        break;
    default:
        printf("something else\n");
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_switch">switch</a>, <a href="cppref:kw_break">break</a>, <a href="cppref:kw_default">default</a></p>)HTML"));

    addEntry(QStringLiteral("kw_default"), QStringLiteral("Keywords"), QStringLiteral("default"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>The catch-all branch of a switch statement, taken when no case label matches. Optional, but good practice to always include one.</p><p><b>See also:</b> <a href="cppref:kw_switch">switch</a>, <a href="cppref:kw_case">case</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Der Auffangzweig einer switch-Anweisung, der genommen wird, wenn kein case passt. Optional, aber guter Stil, ihn immer anzugeben.</p><p><b>Siehe auch:</b> <a href="cppref:kw_switch">switch</a>, <a href="cppref:kw_case">case</a></p>)HTML"));

    addEntry(QStringLiteral("kw_break"), QStringLiteral("Keywords"), QStringLiteral("break"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Immediately exits the innermost enclosing loop (for/while/do) or switch statement - execution continues right after it.</p><pre style="white-space:pre;">for (int i = 0; i &lt; 100; i++)
{
    if (i == 5)
        break;   /* stop the loop entirely */
}</pre><p><b>See also:</b> <a href="cppref:kw_continue">continue</a>, <a href="cppref:kw_switch">switch</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Verlässt sofort die innerste umschließende Schleife (for/while/do) oder switch-Anweisung – die Ausführung geht direkt danach weiter.</p><pre style="white-space:pre;">for (int i = 0; i &lt; 100; i++)
{
    if (i == 5)
        break;   /* stop the loop entirely */
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_continue">continue</a>, <a href="cppref:kw_switch">switch</a></p>)HTML"));

    addEntry(QStringLiteral("kw_continue"), QStringLiteral("Keywords"), QStringLiteral("continue"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Skips the rest of the current loop iteration and jumps straight to the next one (re-testing the loop condition first for while/do).</p><pre style="white-space:pre;">for (int i = 0; i &lt; 10; i++)
{
    if (i % 2 == 0)
        continue;   /* skip even numbers */
    printf("%d\n", i);
}</pre><p><b>See also:</b> <a href="cppref:kw_break">break</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Überspringt den Rest des aktuellen Schleifendurchlaufs und springt direkt zum nächsten (bei while/do wird die Bedingung zuvor erneut geprüft).</p><pre style="white-space:pre;">for (int i = 0; i &lt; 10; i++)
{
    if (i % 2 == 0)
        continue;   /* skip even numbers */
    printf("%d\n", i);
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_break">break</a></p>)HTML"));

    addEntry(QStringLiteral("kw_goto"), QStringLiteral("Keywords"), QStringLiteral("goto"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Jumps unconditionally to a labeled statement in the same function. Mostly avoided in modern C, but still legitimately used for a single, centralized cleanup/error-exit point.</p><pre style="white-space:pre;">if (!(handle = OpenLibrary("dos.library", 0)))
    goto cleanup;

/* ... use handle ... */

cleanup:
if (handle)
    CloseLibrary(handle);</pre><p><b>Note:</b> Amiga NDK code uses this pattern a lot: one cleanup label at the end of a function, unwinding whatever was successfully opened/allocated so far.</p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Springt bedingungslos zu einer beschrifteten Anweisung in derselben Funktion. In modernem C meist vermieden, aber für einen einzigen, zentralen Aufräum-/Fehler-Ausgang durchaus legitim.</p><pre style="white-space:pre;">if (!(handle = OpenLibrary("dos.library", 0)))
    goto cleanup;

/* ... use handle ... */

cleanup:
if (handle)
    CloseLibrary(handle);</pre><p><b>Hinweis:</b> Im Amiga-NDK-Code ist dieses Muster häufig: eine einzelne Cleanup-Marke am Funktionsende, die alles wieder freigibt, was bis dahin erfolgreich geöffnet/alloziert wurde.</p>)HTML"));

    addEntry(QStringLiteral("kw_return"), QStringLiteral("Keywords"), QStringLiteral("return"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Leaves the current function immediately, optionally handing a value back to the caller (omit the value for a void function).</p><pre style="white-space:pre;">LONG Add(LONG a, LONG b)
{
    return a + b;
}</pre>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Verlässt die aktuelle Funktion sofort und liefert optional einen Wert an den Aufrufer zurück (bei einer void-Funktion ohne Wert).</p><pre style="white-space:pre;">LONG Add(LONG a, LONG b)
{
    return a + b;
}</pre>)HTML"));

    addEntry(QStringLiteral("kw_sizeof"), QStringLiteral("Keywords"), QStringLiteral("sizeof"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>A compile-time operator (not a function, despite the parentheses) yielding the size, in bytes, of a type or expression's type.</p><pre style="white-space:pre;">printf("LONG is %lu bytes\n", (unsigned long) sizeof(LONG));
UBYTE *buf = malloc(sizeof(UBYTE) * 256);</pre><p><b>Note:</b> Its result type is size_t (an unsigned type) - cast to a signed type before doing signed arithmetic with it, or you can get surprising results.</p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Ein Compile-Zeit-Operator (trotz der Klammern keine Funktion), der die Größe eines Typs oder des Typs eines Ausdrucks in Bytes liefert.</p><pre style="white-space:pre;">printf("LONG is %lu bytes\n", (unsigned long) sizeof(LONG));
UBYTE *buf = malloc(sizeof(UBYTE) * 256);</pre><p><b>Hinweis:</b> Der Ergebnistyp ist size_t (vorzeichenlos) – vor vorzeichenbehafteter Arithmetik damit erst in einen signed Typ casten, sonst drohen überraschende Ergebnisse.</p>)HTML"));

    addEntry(QStringLiteral("kw_typedef"), QStringLiteral("Keywords"), QStringLiteral("typedef"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Introduces a new name for an existing type - does not create a new type, just an alias. This is exactly how every Amiga NDK type (LONG, BOOL, APTR, ...) is defined.</p><pre style="white-space:pre;">typedef unsigned long ULONG;
typedef struct Node
{
    struct Node *ln_Succ;
    struct Node *ln_Pred;
} NODE;</pre><p><b>See also:</b> <a href="cppref:kw_struct">struct</a>, <a href="cppref:dt_amiga_long">LONG</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Führt einen neuen Namen für einen bestehenden Typ ein – erzeugt keinen neuen Typ, nur einen Alias. Genau so ist jeder Amiga-NDK-Typ (LONG, BOOL, APTR, ...) definiert.</p><pre style="white-space:pre;">typedef unsigned long ULONG;
typedef struct Node
{
    struct Node *ln_Succ;
    struct Node *ln_Pred;
} NODE;</pre><p><b>Siehe auch:</b> <a href="cppref:kw_struct">struct</a>, <a href="cppref:dt_amiga_long">LONG</a></p>)HTML"));

    addEntry(QStringLiteral("kw_struct"), QStringLiteral("Keywords"), QStringLiteral("struct"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Groups several, possibly differently-typed, member variables together under one name - the workhorse of every AmigaOS system structure (Library, Node, Message, IntuiMessage, ...).</p><pre style="white-space:pre;">struct Point
{
    LONG x;
    LONG y;
};

struct Point p1;
p1.x = 10;
p1.y = 20;</pre><p><b>See also:</b> <a href="cppref:kw_union">union</a>, <a href="cppref:kw_typedef">typedef</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Fasst mehrere, möglicherweise unterschiedlich typisierte Member-Variablen unter einem Namen zusammen – das Arbeitspferd jeder AmigaOS-Systemstruktur (Library, Node, Message, IntuiMessage, ...).</p><pre style="white-space:pre;">struct Point
{
    LONG x;
    LONG y;
};

struct Point p1;
p1.x = 10;
p1.y = 20;</pre><p><b>Siehe auch:</b> <a href="cppref:kw_union">union</a>, <a href="cppref:kw_typedef">typedef</a></p>)HTML"));

    addEntry(QStringLiteral("kw_union"), QStringLiteral("Keywords"), QStringLiteral("union"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Like struct, but all members share the SAME memory - the union is only ever as large as its largest member, and only one member is meaningful at a time.</p><pre style="white-space:pre;">union TagData
{
    LONG  tdi_Data;
    APTR  tdi_Pointer;
};</pre><p><b>See also:</b> <a href="cppref:kw_struct">struct</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Wie struct, aber alle Member teilen sich denselben Speicher – die union ist nur so groß wie ihr größter Member, und zu jedem Zeitpunkt ist nur einer davon gültig.</p><pre style="white-space:pre;">union TagData
{
    LONG  tdi_Data;
    APTR  tdi_Pointer;
};</pre><p><b>Siehe auch:</b> <a href="cppref:kw_struct">struct</a></p>)HTML"));

    addEntry(QStringLiteral("kw_enum"), QStringLiteral("Keywords"), QStringLiteral("enum"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Defines a set of named integer constants. Handy for a fixed, closed set of choices - AmigaED itself, for instance, uses one for which compiler is selected.</p><pre style="white-space:pre;">enum Compiler
{
    COMPILER_VBCC,   /* = 0 */
    COMPILER_GCC,    /* = 1 */
    COMPILER_GPP     /* = 2 */
};</pre>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Definiert eine Menge benannter Ganzzahl-Konstanten. Nützlich für eine feste, abgeschlossene Auswahl – AmigaED selbst nutzt zum Beispiel eine für den ausgewählten Compiler.</p><pre style="white-space:pre;">enum Compiler
{
    COMPILER_VBCC,   /* = 0 */
    COMPILER_GCC,    /* = 1 */
    COMPILER_GPP     /* = 2 */
};</pre>)HTML"));

    addEntry(QStringLiteral("kw_static"), QStringLiteral("Keywords"), QStringLiteral("static"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Two unrelated meanings depending on context: at file scope, it limits a function/variable's visibility to the current source file; inside a function, it makes a local variable keep its value between calls instead of being reinitialized every time.</p><pre style="white-space:pre;">void CountCalls(void)
{
    static int calls = 0;   /* initialized once, keeps its value */
    calls++;
    printf("called %d times\n", calls);
}</pre><p><b>See also:</b> <a href="cppref:kw_extern">extern</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Zwei unabhängige Bedeutungen je nach Kontext: auf Dateiebene beschränkt es die Sichtbarkeit einer Funktion/Variable auf die aktuelle Quelldatei; innerhalb einer Funktion sorgt es dafür, dass eine lokale Variable ihren Wert zwischen Aufrufen behält, statt bei jedem Aufruf neu initialisiert zu werden.</p><pre style="white-space:pre;">void CountCalls(void)
{
    static int calls = 0;   /* initialized once, keeps its value */
    calls++;
    printf("called %d times\n", calls);
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_extern">extern</a></p>)HTML"));

    addEntry(QStringLiteral("kw_extern"), QStringLiteral("Keywords"), QStringLiteral("extern"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Declares that a variable or function is defined elsewhere (another source file) - used in a header so several .c files can share one definition without each redefining it.</p><pre style="white-space:pre;">/* in a header, e.g. globals.h */
extern struct Library *DOSBase;

/* in exactly one .c file */
struct Library *DOSBase;</pre><p><b>See also:</b> <a href="cppref:kw_static">static</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Erklärt, dass eine Variable oder Funktion anderswo (in einer anderen Quelldatei) definiert ist – wird in einem Header verwendet, damit sich mehrere .c-Dateien eine Definition teilen können, ohne sie jeweils neu zu definieren.</p><pre style="white-space:pre;">/* in a header, e.g. globals.h */
extern struct Library *DOSBase;

/* in exactly one .c file */
struct Library *DOSBase;</pre><p><b>Siehe auch:</b> <a href="cppref:kw_static">static</a></p>)HTML"));

    addEntry(QStringLiteral("kw_register"), QStringLiteral("Keywords"), QStringLiteral("register"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>A hint to the compiler that a local variable is accessed heavily and should, if possible, be kept in a CPU register rather than memory. Purely advisory in modern compilers, which usually make this decision on their own anyway.</p><pre style="white-space:pre;">register int i;
for (i = 0; i &lt; 1000000; i++)
{
    /* tight loop */
}</pre><p><b>Note:</b> On the classic 68000/68020 Amiga toolchains, register variables mattered a lot more than on a modern CPU - some older Amiga code leans on it more heavily than you'd expect from today's C style.</p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Ein Hinweis an den Compiler, dass eine lokale Variable häufig verwendet wird und wenn möglich in einem CPU-Register statt im Speicher gehalten werden soll. In modernen Compilern rein beratend – die treffen diese Entscheidung ohnehin meist selbst.</p><pre style="white-space:pre;">register int i;
for (i = 0; i &lt; 1000000; i++)
{
    /* tight loop */
}</pre><p><b>Hinweis:</b> Bei den klassischen 68000/68020-Amiga-Toolchains spielte register eine deutlich größere Rolle als auf einer modernen CPU – manch älterer Amiga-Code nutzt es stärker, als man vom heutigen C-Stil erwarten würde.</p>)HTML"));

    addEntry(QStringLiteral("kw_const"), QStringLiteral("Keywords"), QStringLiteral("const"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Marks a variable, or the thing a pointer points to, as read-only after initialization - the compiler rejects any later attempt to modify it.</p><pre style="white-space:pre;">const int maxRetries = 3;
void PrintName(const STRPTR name)   /* PrintName won't modify *name */
{
    printf("%s\n", name);
}</pre><p><b>See also:</b> <a href="cppref:kw_volatile">volatile</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Markiert eine Variable – oder das, worauf ein Zeiger zeigt – nach der Initialisierung als schreibgeschützt; der Compiler weist jeden späteren Änderungsversuch zurück.</p><pre style="white-space:pre;">const int maxRetries = 3;
void PrintName(const STRPTR name)   /* PrintName won't modify *name */
{
    printf("%s\n", name);
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_volatile">volatile</a></p>)HTML"));

    addEntry(QStringLiteral("kw_volatile"), QStringLiteral("Keywords"), QStringLiteral("volatile"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Tells the compiler a variable's value can change outside the normal flow of the program (hardware register, another task, an interrupt) - so it must always be re-read from memory, never cached in a register or optimized away.</p><pre style="white-space:pre;">volatile UWORD *custom_potgor = (volatile UWORD *) 0xdff016;
while ((*custom_potgor &amp; 1) == 0)
{
    /* wait for hardware bit to change */
}</pre><p><b>See also:</b> <a href="cppref:kw_const">const</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Teilt dem Compiler mit, dass sich der Wert einer Variable außerhalb des normalen Programmablaufs ändern kann (Hardware-Register, ein anderer Task, ein Interrupt) – sie muss daher stets neu aus dem Speicher gelesen werden, nie in einem Register gecacht oder wegoptimiert.</p><pre style="white-space:pre;">volatile UWORD *custom_potgor = (volatile UWORD *) 0xdff016;
while ((*custom_potgor &amp; 1) == 0)
{
    /* wait for hardware bit to change */
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_const">const</a></p>)HTML"));

    addEntry(QStringLiteral("kw_inline"), QStringLiteral("Keywords"), QStringLiteral("inline"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>A hint asking the compiler to substitute a small function's body directly at each call site instead of a real call, avoiding call overhead. Purely advisory - the compiler is always free to ignore it.</p><pre style="white-space:pre;">static inline LONG Max(LONG a, LONG b)
{
    return (a &gt; b) ? a : b;
}</pre><p><b>Note:</b> Support and effectiveness vary between vbcc, SAS/C, and m68k-amigaos-gcc - check your toolchain's own documentation for how aggressively it honours this.</p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Ein Hinweis an den Compiler, den Rumpf einer kleinen Funktion direkt an jeder Aufrufstelle einzusetzen statt eines echten Aufrufs, um den Aufruf-Overhead zu vermeiden. Rein beratend – der Compiler darf ihn jederzeit ignorieren.</p><pre style="white-space:pre;">static inline LONG Max(LONG a, LONG b)
{
    return (a &gt; b) ? a : b;
}</pre><p><b>Hinweis:</b> Unterstützung und Wirksamkeit unterscheiden sich zwischen vbcc, SAS/C und m68k-amigaos-gcc – die Dokumentation der jeweiligen Toolchain zeigt, wie konsequent sie dies umsetzt.</p>)HTML"));

    addEntry(QStringLiteral("kw_void"), QStringLiteral("Keywords"), QStringLiteral("void"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Three uses: as a return type, meaning "returns nothing"; as the only parameter, meaning "takes no arguments"; and as void*, a generic pointer type that can point to anything.</p><pre style="white-space:pre;">void PrintBanner(void)
{
    printf("AmigaED\n");
}

void *genericPtr = malloc(64);</pre><p><b>See also:</b> <a href="cppref:dt_amiga_aptr">APTR</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Drei Verwendungen: als Rückgabetyp bedeutet es "gibt nichts zurück"; als einziger Parameter bedeutet es "nimmt keine Argumente entgegen"; und als void* ein universeller Zeigertyp, der auf alles zeigen kann.</p><pre style="white-space:pre;">void PrintBanner(void)
{
    printf("AmigaED\n");
}

void *genericPtr = malloc(64);</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_aptr">APTR</a></p>)HTML"));

    addEntry(QStringLiteral("kw_char"), QStringLiteral("Keywords"), QStringLiteral("char"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>The smallest addressable integer type, almost always 8 bits - used both for small numbers and, far more often, as the element type of a C string.</p><pre style="white-space:pre;">char letter = 'A';
char name[] = "AmigaED";</pre><p><b>Note:</b> Whether a plain char is signed or unsigned by default is implementation-defined - the Amiga NDK sidesteps the question entirely with its own explicit BYTE/UBYTE typedefs (see the Datatypes category).</p><p><b>See also:</b> <a href="cppref:kw_short">short</a>, <a href="cppref:kw_int">int</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Der kleinste adressierbare Ganzzahltyp, fast immer 8 Bit – wird sowohl für kleine Zahlen als auch, weit häufiger, als Elementtyp eines C-Strings verwendet.</p><pre style="white-space:pre;">char letter = 'A';
char name[] = "AmigaED";</pre><p><b>Hinweis:</b> Ob ein einfaches char standardmäßig signed oder unsigned ist, ist implementierungsabhängig – das Amiga-NDK umgeht die Frage komplett mit seinen eigenen expliziten Typedefs BYTE/UBYTE (siehe Kategorie Datatypes).</p><p><b>Siehe auch:</b> <a href="cppref:kw_short">short</a>, <a href="cppref:kw_int">int</a></p>)HTML"));

    addEntry(QStringLiteral("kw_short"), QStringLiteral("Keywords"), QStringLiteral("short"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>A signed (by default) integer type, at least 16 bits wide - on every Amiga C toolchain, exactly 16 bits.</p><pre style="white-space:pre;">short delta = -12;</pre><p><b>See also:</b> <a href="cppref:kw_int">int</a>, <a href="cppref:kw_long">long</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Ein (standardmäßig) vorzeichenbehafteter Ganzzahltyp, mindestens 16 Bit breit – bei jeder Amiga-C-Toolchain genau 16 Bit.</p><pre style="white-space:pre;">short delta = -12;</pre><p><b>Siehe auch:</b> <a href="cppref:kw_int">int</a>, <a href="cppref:kw_long">long</a></p>)HTML"));

    addEntry(QStringLiteral("kw_int"), QStringLiteral("Keywords"), QStringLiteral("int"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>The "natural" signed integer type. On every m68k Amiga C toolchain (vbcc, SAS/C, m68k-amigaos-gcc) this is 32 bits - the SAME size and range as long, unlike many 16-bit-int platforms of the same era.</p><pre style="white-space:pre;">int count = -5;</pre><p><b>Note:</b> This is exactly why the AmigaOS NDK prefers its own explicit-width typedefs (LONG, WORD, ...) instead of int/short: they document the intended size directly, rather than relying on what a given platform happens to make int mean.</p><p><b>See also:</b> <a href="cppref:kw_long">long</a>, <a href="cppref:kw_short">short</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Der "natürliche" vorzeichenbehaftete Ganzzahltyp. Bei jeder m68k-Amiga-C-Toolchain (vbcc, SAS/C, m68k-amigaos-gcc) sind das 32 Bit – dieselbe Größe und derselbe Wertebereich wie long, anders als bei vielen 16-Bit-int-Plattformen derselben Ära.</p><pre style="white-space:pre;">int count = -5;</pre><p><b>Hinweis:</b> Genau deshalb bevorzugt das AmigaOS-NDK seine eigenen Typedefs mit expliziter Breite (LONG, WORD, ...) statt int/short: sie dokumentieren die beabsichtigte Größe direkt, statt sich darauf zu verlassen, was int auf einer bestimmten Plattform gerade bedeutet.</p><p><b>Siehe auch:</b> <a href="cppref:kw_long">long</a>, <a href="cppref:kw_short">short</a></p>)HTML"));

    addEntry(QStringLiteral("kw_long"), QStringLiteral("Keywords"), QStringLiteral("long"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>A signed integer type, at least 32 bits wide - exactly 32 bits on every m68k Amiga toolchain, same as int there.</p><pre style="white-space:pre;">long offset = -100000L;</pre><p><b>See also:</b> <a href="cppref:kw_int">int</a>, <a href="cppref:dt_amiga_long">LONG</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Ein vorzeichenbehafteter Ganzzahltyp, mindestens 32 Bit breit – bei jeder m68k-Amiga-Toolchain genau 32 Bit, dort also dasselbe wie int.</p><pre style="white-space:pre;">long offset = -100000L;</pre><p><b>Siehe auch:</b> <a href="cppref:kw_int">int</a>, <a href="cppref:dt_amiga_long">LONG</a></p>)HTML"));

    addEntry(QStringLiteral("kw_float"), QStringLiteral("Keywords"), QStringLiteral("float"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>A single-precision (32-bit, IEEE-754) floating-point type. AmigaED automatically adds the right math library (-lm, -lmieee, or MATH=IEEE) to your generated Makefile the moment it sees float/double anywhere in your project.</p><pre style="white-space:pre;">float ratio = 1.5f;</pre><p><b>See also:</b> <a href="cppref:kw_double">double</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Ein einfach genauer (32-Bit, IEEE-754) Gleitkommatyp. AmigaED ergänzt automatisch die passende Mathe-Bibliothek (-lm, -lmieee oder MATH=IEEE) im erzeugten Makefile, sobald irgendwo im Projekt float/double auftaucht.</p><pre style="white-space:pre;">float ratio = 1.5f;</pre><p><b>Siehe auch:</b> <a href="cppref:kw_double">double</a></p>)HTML"));

    addEntry(QStringLiteral("kw_double"), QStringLiteral("Keywords"), QStringLiteral("double"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>A double-precision (64-bit, IEEE-754) floating-point type - roughly twice float's precision, at roughly twice the storage and, on the classic 68000/68020 without an FPU, noticeably more CPU cost.</p><pre style="white-space:pre;">double precise = 3.14159265358979;</pre><p><b>See also:</b> <a href="cppref:kw_float">float</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Ein doppelt genauer (64-Bit, IEEE-754) Gleitkommatyp – etwa doppelte Genauigkeit von float, bei etwa doppeltem Speicherbedarf und, auf dem klassischen 68000/68020 ohne FPU, spürbar höheren CPU-Kosten.</p><pre style="white-space:pre;">double precise = 3.14159265358979;</pre><p><b>Siehe auch:</b> <a href="cppref:kw_float">float</a></p>)HTML"));

    addEntry(QStringLiteral("kw_signed"), QStringLiteral("Keywords"), QStringLiteral("signed"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Explicitly marks an integer type as able to hold negative values - the default for int/short/long anyway, so it's mostly written in front of char, whose own default signedness is implementation-defined.</p><pre style="white-space:pre;">signed char level = -5;</pre><p><b>See also:</b> <a href="cppref:kw_unsigned">unsigned</a>, <a href="cppref:kw_char">char</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Markiert einen Ganzzahltyp explizit als fähig, negative Werte zu halten – ohnehin der Standard bei int/short/long, daher meist vor char geschrieben, dessen eigene Standard-Vorzeichenbehaftung implementierungsabhängig ist.</p><pre style="white-space:pre;">signed char level = -5;</pre><p><b>Siehe auch:</b> <a href="cppref:kw_unsigned">unsigned</a>, <a href="cppref:kw_char">char</a></p>)HTML"));

    addEntry(QStringLiteral("kw_unsigned"), QStringLiteral("Keywords"), QStringLiteral("unsigned"),
              QStringLiteral(R"HTML(<p><i>C and C++ keyword.</i></p><p>Marks an integer type as holding only zero and positive values, doubling its positive range compared to the signed version at the same bit width - used constantly throughout the Amiga NDK (UBYTE, UWORD, ULONG).</p><pre style="white-space:pre;">unsigned long size = 4096UL;</pre><p><b>See also:</b> <a href="cppref:kw_signed">signed</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>C- und C++-Schlüsselwort.</i></p><p>Markiert einen Ganzzahltyp so, dass er nur Null und positive Werte hält – bei gleicher Bitbreite doppelt so großer positiver Wertebereich wie die signed-Version. Im Amiga-NDK ständig im Einsatz (UBYTE, UWORD, ULONG).</p><pre style="white-space:pre;">unsigned long size = 4096UL;</pre><p><b>Siehe auch:</b> <a href="cppref:kw_signed">signed</a></p>)HTML"));

    addEntry(QStringLiteral("kw_class"), QStringLiteral("Keywords"), QStringLiteral("class"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Defines a new type bundling data (member variables) and the functions that operate on it (member functions/methods) - like struct, but members are private by default.</p><pre style="white-space:pre;">class Point
{
public:
    Point(LONG x, LONG y) : m_x(x), m_y(y) {}
    LONG GetX() const { return m_x; }
private:
    LONG m_x, m_y;
};</pre><p><b>See also:</b> <a href="cppref:kw_struct">struct</a>, <a href="cppref:kw_public">public</a>, <a href="cppref:kw_private">private</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Definiert einen neuen Typ, der Daten (Member-Variablen) und die darauf arbeitenden Funktionen (Methoden) bündelt – wie struct, nur sind Member standardmäßig private.</p><pre style="white-space:pre;">class Point
{
public:
    Point(LONG x, LONG y) : m_x(x), m_y(y) {}
    LONG GetX() const { return m_x; }
private:
    LONG m_x, m_y;
};</pre><p><b>Siehe auch:</b> <a href="cppref:kw_struct">struct</a>, <a href="cppref:kw_public">public</a>, <a href="cppref:kw_private">private</a></p>)HTML"));

    addEntry(QStringLiteral("kw_public"), QStringLiteral("Keywords"), QStringLiteral("public"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>A class-member access label: everything after it, up to the next label, is reachable from outside the class.</p><pre style="white-space:pre;">class Point
{
public:
    LONG x, y;   /* freely accessible */
};</pre><p><b>See also:</b> <a href="cppref:kw_class">class</a>, <a href="cppref:kw_private">private</a>, <a href="cppref:kw_protected">protected</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Eine Zugriffs-Marke für Klassenmember: Alles danach, bis zur nächsten Marke, ist auch von außerhalb der Klasse erreichbar.</p><pre style="white-space:pre;">class Point
{
public:
    LONG x, y;   /* freely accessible */
};</pre><p><b>Siehe auch:</b> <a href="cppref:kw_class">class</a>, <a href="cppref:kw_private">private</a>, <a href="cppref:kw_protected">protected</a></p>)HTML"));

    addEntry(QStringLiteral("kw_private"), QStringLiteral("Keywords"), QStringLiteral("private"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>A class-member access label: everything after it, up to the next label, is reachable only from the class's own member functions (and its friends).</p><pre style="white-space:pre;">class Point
{
private:
    LONG m_x, m_y;   /* only Point's own methods can touch these */
};</pre><p><b>See also:</b> <a href="cppref:kw_public">public</a>, <a href="cppref:kw_friend">friend</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Eine Zugriffs-Marke für Klassenmember: Alles danach, bis zur nächsten Marke, ist nur von den eigenen Methoden der Klasse (und ihren friends) erreichbar.</p><pre style="white-space:pre;">class Point
{
private:
    LONG m_x, m_y;   /* only Point's own methods can touch these */
};</pre><p><b>Siehe auch:</b> <a href="cppref:kw_public">public</a>, <a href="cppref:kw_friend">friend</a></p>)HTML"));

    addEntry(QStringLiteral("kw_protected"), QStringLiteral("Keywords"), QStringLiteral("protected"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>A class-member access label between public and private: reachable from the class's own member functions AND from classes that derive from it, but not from unrelated outside code.</p><pre style="white-space:pre;">class Base
{
protected:
    LONG m_state;   /* visible to Base and any class deriving from it */
};</pre><p><b>See also:</b> <a href="cppref:kw_public">public</a>, <a href="cppref:kw_private">private</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Eine Zugriffs-Marke zwischen public und private: erreichbar von den eigenen Methoden der Klasse UND von Klassen, die davon ableiten, aber nicht von unbeteiligtem Code außerhalb.</p><pre style="white-space:pre;">class Base
{
protected:
    LONG m_state;   /* visible to Base and any class deriving from it */
};</pre><p><b>Siehe auch:</b> <a href="cppref:kw_public">public</a>, <a href="cppref:kw_private">private</a></p>)HTML"));

    addEntry(QStringLiteral("kw_virtual"), QStringLiteral("Keywords"), QStringLiteral("virtual"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Marks a member function as overridable by a derived class, with the call resolved at run time based on the object's actual type rather than the compile-time pointer/reference type.</p><pre style="white-space:pre;">class Shape
{
public:
    virtual LONG Area() const { return 0; }
};

class Circle : public Shape
{
public:
    LONG Area() const override { return 3 * m_r * m_r; }
private:
    LONG m_r;
};</pre><p><b>See also:</b> <a href="cppref:kw_override">override</a>, <a href="cppref:kw_class">class</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Markiert eine Methode als von einer abgeleiteten Klasse überschreibbar – der Aufruf wird zur Laufzeit anhand des tatsächlichen Objekttyps aufgelöst, nicht anhand des Zeiger-/Referenztyps zur Compile-Zeit.</p><pre style="white-space:pre;">class Shape
{
public:
    virtual LONG Area() const { return 0; }
};

class Circle : public Shape
{
public:
    LONG Area() const override { return 3 * m_r * m_r; }
private:
    LONG m_r;
};</pre><p><b>Siehe auch:</b> <a href="cppref:kw_override">override</a>, <a href="cppref:kw_class">class</a></p>)HTML"));

    addEntry(QStringLiteral("kw_friend"), QStringLiteral("Keywords"), QStringLiteral("friend"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Grants a named function or class access to another class's private/protected members - an explicit, deliberate exception to normal encapsulation.</p><pre style="white-space:pre;">class Vector
{
    friend class Matrix;   /* Matrix may reach into Vector's private data */
private:
    LONG m_data[4];
};</pre><p><b>See also:</b> <a href="cppref:kw_private">private</a>, <a href="cppref:kw_class">class</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Gewährt einer benannten Funktion oder Klasse Zugriff auf die private/protected-Member einer anderen Klasse – eine explizite, bewusste Ausnahme von der normalen Kapselung.</p><pre style="white-space:pre;">class Vector
{
    friend class Matrix;   /* Matrix may reach into Vector's private data */
private:
    LONG m_data[4];
};</pre><p><b>Siehe auch:</b> <a href="cppref:kw_private">private</a>, <a href="cppref:kw_class">class</a></p>)HTML"));

    addEntry(QStringLiteral("kw_template"), QStringLiteral("Keywords"), QStringLiteral("template"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Introduces a generic function or class parameterized over one or more types - the compiler generates a concrete version for each type it's actually used with.</p><pre style="white-space:pre;">template&lt;typename T&gt;
T Max(T a, T b)
{
    return (a &gt; b) ? a : b;
}

LONG bigger = Max&lt;LONG&gt;(3, 7);</pre><p><b>Note:</b> Older/simpler Amiga C++ toolchains may have limited template support - check what your m68k-amigaos-g++ version actually handles before relying heavily on it.</p><p><b>See also:</b> <a href="cppref:kw_typename">typename</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Führt eine generische Funktion oder Klasse ein, die über einen oder mehrere Typen parametrisiert ist – der Compiler erzeugt für jeden tatsächlich verwendeten Typ eine konkrete Version.</p><pre style="white-space:pre;">template&lt;typename T&gt;
T Max(T a, T b)
{
    return (a &gt; b) ? a : b;
}

LONG bigger = Max&lt;LONG&gt;(3, 7);</pre><p><b>Hinweis:</b> Ältere/einfachere Amiga-C++-Toolchains unterstützen Templates unter Umständen nur eingeschränkt – vor intensiver Nutzung prüfen, was die eigene m68k-amigaos-g++-Version tatsächlich beherrscht.</p><p><b>Siehe auch:</b> <a href="cppref:kw_typename">typename</a></p>)HTML"));

    addEntry(QStringLiteral("kw_typename"), QStringLiteral("Keywords"), QStringLiteral("typename"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Used inside a template to tell the compiler that a dependent name refers to a type, not a value - also increasingly used as a synonym for class in a template parameter list.</p><pre style="white-space:pre;">template&lt;typename T&gt;
class Box
{
public:
    T value;
};</pre><p><b>See also:</b> <a href="cppref:kw_template">template</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Wird innerhalb eines Templates verwendet, um dem Compiler mitzuteilen, dass ein abhängiger Name einen Typ bezeichnet, keinen Wert – zunehmend auch als Synonym für class in einer Template-Parameterliste genutzt.</p><pre style="white-space:pre;">template&lt;typename T&gt;
class Box
{
public:
    T value;
};</pre><p><b>Siehe auch:</b> <a href="cppref:kw_template">template</a></p>)HTML"));

    addEntry(QStringLiteral("kw_namespace"), QStringLiteral("Keywords"), QStringLiteral("namespace"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Groups related names (functions, classes, constants) under one prefix to avoid clashing with identically-named things elsewhere in a large project.</p><pre style="white-space:pre;">namespace Graphics
{
    void Clear();
}

Graphics::Clear();</pre><p><b>See also:</b> <a href="cppref:kw_using">using</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Fasst zusammengehörige Namen (Funktionen, Klassen, Konstanten) unter einem Präfix zusammen, um Kollisionen mit gleichnamigen Dingen anderswo in einem großen Projekt zu vermeiden.</p><pre style="white-space:pre;">namespace Graphics
{
    void Clear();
}

Graphics::Clear();</pre><p><b>Siehe auch:</b> <a href="cppref:kw_using">using</a></p>)HTML"));

    addEntry(QStringLiteral("kw_using"), QStringLiteral("Keywords"), QStringLiteral("using"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Two common uses: "using namespace X;" brings every name from namespace X into scope unqualified; "using Alias = Type;" defines a type alias (a modern alternative to typedef).</p><pre style="white-space:pre;">using namespace Graphics;
Clear();   /* no "Graphics::" prefix needed now */

using ByteBuffer = unsigned char *;</pre><p><b>See also:</b> <a href="cppref:kw_namespace">namespace</a>, <a href="cppref:kw_typedef">typedef</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Zwei häufige Verwendungen: "using namespace X;" bringt alle Namen aus dem Namensraum X unqualifiziert in den Gültigkeitsbereich; "using Alias = Type;" definiert einen Typ-Alias (eine moderne Alternative zu typedef).</p><pre style="white-space:pre;">using namespace Graphics;
Clear();   /* no "Graphics::" prefix needed now */

using ByteBuffer = unsigned char *;</pre><p><b>Siehe auch:</b> <a href="cppref:kw_namespace">namespace</a>, <a href="cppref:kw_typedef">typedef</a></p>)HTML"));

    addEntry(QStringLiteral("kw_new"), QStringLiteral("Keywords"), QStringLiteral("new"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Allocates an object on the heap and runs its constructor, returning a pointer to it - the C++ counterpart to malloc(), but type-safe and constructor-aware. Must be matched with delete.</p><pre style="white-space:pre;">Point *p = new Point(1, 2);
/* ... use p ... */
delete p;</pre><p><b>See also:</b> <a href="cppref:kw_delete">delete</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Alloziert ein Objekt auf dem Heap und ruft seinen Konstruktor auf, liefert einen Zeiger darauf – das C++-Gegenstück zu malloc(), aber typsicher und konstruktorbewusst. Muss zu delete passen.</p><pre style="white-space:pre;">Point *p = new Point(1, 2);
/* ... use p ... */
delete p;</pre><p><b>Siehe auch:</b> <a href="cppref:kw_delete">delete</a></p>)HTML"));

    addEntry(QStringLiteral("kw_delete"), QStringLiteral("Keywords"), QStringLiteral("delete"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Runs an object's destructor and frees the memory new allocated for it - use delete[] instead for an array allocated with new[].</p><pre style="white-space:pre;">delete p;
delete[] arrayOfPoints;</pre><p><b>See also:</b> <a href="cppref:kw_new">new</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Ruft den Destruktor eines Objekts auf und gibt den von new dafür allozierten Speicher frei – für ein mit new[] alloziertes Array stattdessen delete[] verwenden.</p><pre style="white-space:pre;">delete p;
delete[] arrayOfPoints;</pre><p><b>Siehe auch:</b> <a href="cppref:kw_new">new</a></p>)HTML"));

    addEntry(QStringLiteral("kw_this"), QStringLiteral("Keywords"), QStringLiteral("this"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Inside a non-static member function, a pointer to the object the function was called on - used explicitly to disambiguate a member from a same-named parameter, or to return *this for method chaining.</p><pre style="white-space:pre;">class Point
{
public:
    void SetX(LONG x) { this-&gt;m_x = x; }
private:
    LONG m_x;
};</pre><p><b>See also:</b> <a href="cppref:kw_class">class</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Innerhalb einer nicht-statischen Methode ein Zeiger auf das Objekt, für das die Methode aufgerufen wurde – wird explizit verwendet, um einen Member von einem gleichnamigen Parameter zu unterscheiden, oder um für Methodenverkettung *this zurückzugeben.</p><pre style="white-space:pre;">class Point
{
public:
    void SetX(LONG x) { this-&gt;m_x = x; }
private:
    LONG m_x;
};</pre><p><b>Siehe auch:</b> <a href="cppref:kw_class">class</a></p>)HTML"));

    addEntry(QStringLiteral("kw_try"), QStringLiteral("Keywords"), QStringLiteral("try"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Opens a block whose exceptions are caught by one or more following catch blocks, instead of propagating uncaught out of the function.</p><pre style="white-space:pre;">try
{
    RiskyOperation();
}
catch (const std::exception &amp;e)
{
    printf("failed: %s\n", e.what());
}</pre><p><b>Note:</b> C++ exceptions add real code size and, depending on your g++ configuration, may need extra runtime support - many Amiga C++ projects deliberately avoid them and stick to C-style error codes instead.</p><p><b>See also:</b> <a href="cppref:kw_catch">catch</a>, <a href="cppref:kw_throw">throw</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Eröffnet einen Block, dessen Ausnahmen von einem oder mehreren folgenden catch-Blöcken abgefangen werden, statt unabgefangen aus der Funktion zu propagieren.</p><pre style="white-space:pre;">try
{
    RiskyOperation();
}
catch (const std::exception &amp;e)
{
    printf("failed: %s\n", e.what());
}</pre><p><b>Hinweis:</b> C++-Exceptions vergrößern den Code spürbar und benötigen je nach g++-Konfiguration zusätzliche Laufzeitunterstützung – viele Amiga-C++-Projekte verzichten deshalb bewusst darauf und bleiben bei C-artigen Fehlercodes.</p><p><b>Siehe auch:</b> <a href="cppref:kw_catch">catch</a>, <a href="cppref:kw_throw">throw</a></p>)HTML"));

    addEntry(QStringLiteral("kw_catch"), QStringLiteral("Keywords"), QStringLiteral("catch"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Handles an exception thrown inside the preceding try block, if its type matches (or is a base class of) the thrown exception's type.</p><pre style="white-space:pre;">catch (const std::exception &amp;e)
{
    printf("failed: %s\n", e.what());
}</pre><p><b>See also:</b> <a href="cppref:kw_try">try</a>, <a href="cppref:kw_throw">throw</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Behandelt eine im vorangehenden try-Block geworfene Ausnahme, sofern ihr Typ passt (oder eine Basisklasse des geworfenen Ausnahmetyps ist).</p><pre style="white-space:pre;">catch (const std::exception &amp;e)
{
    printf("failed: %s\n", e.what());
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_try">try</a>, <a href="cppref:kw_throw">throw</a></p>)HTML"));

    addEntry(QStringLiteral("kw_throw"), QStringLiteral("Keywords"), QStringLiteral("throw"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Raises an exception, immediately unwinding the call stack until a matching catch block is found.</p><pre style="white-space:pre;">if (size == 0)
    throw std::invalid_argument("size must not be zero");</pre><p><b>See also:</b> <a href="cppref:kw_try">try</a>, <a href="cppref:kw_catch">catch</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Löst eine Ausnahme aus und wickelt den Aufrufstapel sofort ab, bis ein passender catch-Block gefunden wird.</p><pre style="white-space:pre;">if (size == 0)
    throw std::invalid_argument("size must not be zero");</pre><p><b>Siehe auch:</b> <a href="cppref:kw_try">try</a>, <a href="cppref:kw_catch">catch</a></p>)HTML"));

    addEntry(QStringLiteral("kw_operator"), QStringLiteral("Keywords"), QStringLiteral("operator"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Introduces an operator overload, letting a class define what +, ==, [], and other built-in operators mean for its own objects.</p><pre style="white-space:pre;">class Point
{
public:
    Point operator+(const Point &amp;o) const
    {
        return Point(m_x + o.m_x, m_y + o.m_y);
    }
private:
    LONG m_x, m_y;
};</pre><p><b>See also:</b> <a href="cppref:kw_class">class</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Öffnet eine Überladung eines Operators, wodurch eine Klasse festlegen kann, was +, ==, [] und andere eingebaute Operatoren für ihre eigenen Objekte bedeuten.</p><pre style="white-space:pre;">class Point
{
public:
    Point operator+(const Point &amp;o) const
    {
        return Point(m_x + o.m_x, m_y + o.m_y);
    }
private:
    LONG m_x, m_y;
};</pre><p><b>Siehe auch:</b> <a href="cppref:kw_class">class</a></p>)HTML"));

    addEntry(QStringLiteral("kw_explicit"), QStringLiteral("Keywords"), QStringLiteral("explicit"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Applied to a single-argument constructor (or conversion operator), it forbids the compiler from using that constructor for an implicit, unintended type conversion.</p><pre style="white-space:pre;">class Buffer
{
public:
    explicit Buffer(LONG size);   /* Buffer b = 10; is now a compile error */
};</pre><p><b>See also:</b> <a href="cppref:kw_class">class</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Auf einen Ein-Parameter-Konstruktor (oder Konversionsoperator) angewendet, verbietet es dem Compiler, diesen Konstruktor für eine implizite, unbeabsichtigte Typumwandlung zu nutzen.</p><pre style="white-space:pre;">class Buffer
{
public:
    explicit Buffer(LONG size);   /* Buffer b = 10; is now a compile error */
};</pre><p><b>Siehe auch:</b> <a href="cppref:kw_class">class</a></p>)HTML"));

    addEntry(QStringLiteral("kw_mutable"), QStringLiteral("Keywords"), QStringLiteral("mutable"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Lets one specific member of an otherwise const object still be modified - typically used for an internal cache or a lock that has to change even from a logically read-only method.</p><pre style="white-space:pre;">class Cache
{
public:
    LONG GetValue() const
    {
        m_hits++;   /* allowed - m_hits is mutable */
        return m_value;
    }
private:
    LONG m_value;
    mutable LONG m_hits = 0;
};</pre><p><b>See also:</b> <a href="cppref:kw_const">const</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Erlaubt, dass ein einzelner Member eines ansonsten const-Objekts trotzdem verändert werden darf – typischerweise für einen internen Cache oder eine Sperre, die sich selbst aus einer logisch schreibgeschützten Methode heraus ändern muss.</p><pre style="white-space:pre;">class Cache
{
public:
    LONG GetValue() const
    {
        m_hits++;   /* allowed - m_hits is mutable */
        return m_value;
    }
private:
    LONG m_value;
    mutable LONG m_hits = 0;
};</pre><p><b>Siehe auch:</b> <a href="cppref:kw_const">const</a></p>)HTML"));

    addEntry(QStringLiteral("kw_bool"), QStringLiteral("Keywords"), QStringLiteral("bool"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>C++'s built-in boolean type, holding only true or false. In C, the closest standard equivalent is _Bool (C99, via <stdbool.h>) - AmigaOS code more commonly uses its own BOOL typedef instead (see Datatypes).</p><pre style="white-space:pre;">bool found = false;
for (LONG i = 0; i &lt; count &amp;&amp; !found; i++)
{
    if (items[i] == target)
        found = true;
}</pre><p><b>See also:</b> <a href="cppref:kw_true">true</a>, <a href="cppref:kw_false">false</a>, <a href="cppref:dt_amiga_bool">BOOL</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>C++s eingebauter boolescher Typ, der nur true oder false hält. In C ist das nächste Standard-Äquivalent _Bool (C99, über <stdbool.h>) – AmigaOS-Code nutzt stattdessen üblicherweise den eigenen BOOL-Typedef (siehe Datatypes).</p><pre style="white-space:pre;">bool found = false;
for (LONG i = 0; i &lt; count &amp;&amp; !found; i++)
{
    if (items[i] == target)
        found = true;
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_true">true</a>, <a href="cppref:kw_false">false</a>, <a href="cppref:dt_amiga_bool">BOOL</a></p>)HTML"));

    addEntry(QStringLiteral("kw_true"), QStringLiteral("Keywords"), QStringLiteral("true"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>The bool value representing "yes"/logical truth.</p><p><b>See also:</b> <a href="cppref:kw_bool">bool</a>, <a href="cppref:kw_false">false</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Der bool-Wert für "ja"/logisch wahr.</p><p><b>Siehe auch:</b> <a href="cppref:kw_bool">bool</a>, <a href="cppref:kw_false">false</a></p>)HTML"));

    addEntry(QStringLiteral("kw_false"), QStringLiteral("Keywords"), QStringLiteral("false"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>The bool value representing "no"/logical falsehood.</p><p><b>See also:</b> <a href="cppref:kw_bool">bool</a>, <a href="cppref:kw_true">true</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Der bool-Wert für "nein"/logisch falsch.</p><p><b>Siehe auch:</b> <a href="cppref:kw_bool">bool</a>, <a href="cppref:kw_true">true</a></p>)HTML"));

    addEntry(QStringLiteral("kw_nullptr"), QStringLiteral("Keywords"), QStringLiteral("nullptr"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>A type-safe null pointer literal (C++11) - prefer it over the C-style NULL macro or a plain 0 in C++ code, since it can never accidentally be mistaken for an integer 0 in overload resolution.</p><pre style="white-space:pre;">APTR ptr = nullptr;
if (ptr == nullptr)
{
    printf("not allocated\n");
}</pre>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Ein typsicheres Null-Zeiger-Literal (C++11) – in C++-Code gegenüber dem C-Makro NULL oder einer blanken 0 zu bevorzugen, da es bei der Überladungsauflösung nie versehentlich mit einer ganzzahligen 0 verwechselt werden kann.</p><pre style="white-space:pre;">APTR ptr = nullptr;
if (ptr == nullptr)
{
    printf("not allocated\n");
}</pre>)HTML"));

    addEntry(QStringLiteral("kw_constexpr"), QStringLiteral("Keywords"), QStringLiteral("constexpr"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Marks a function or variable as computable at compile time, when its inputs allow it - the compiler evaluates it during compilation rather than at run time wherever possible.</p><pre style="white-space:pre;">constexpr LONG Square(LONG x)
{
    return x * x;
}

constexpr LONG nine = Square(3);   /* computed at compile time */</pre><p><b>Note:</b> A C++11 feature - support depends on your m68k-amigaos-g++ version.</p><p><b>See also:</b> <a href="cppref:kw_const">const</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Markiert eine Funktion oder Variable als zur Compile-Zeit berechenbar, sofern es die Eingaben zulassen – der Compiler wertet sie dann während der Kompilierung statt zur Laufzeit aus, wo immer möglich.</p><pre style="white-space:pre;">constexpr LONG Square(LONG x)
{
    return x * x;
}

constexpr LONG nine = Square(3);   /* computed at compile time */</pre><p><b>Hinweis:</b> Ein C++11-Feature – die Unterstützung hängt von der eigenen m68k-amigaos-g++-Version ab.</p><p><b>Siehe auch:</b> <a href="cppref:kw_const">const</a></p>)HTML"));

    addEntry(QStringLiteral("kw_override"), QStringLiteral("Keywords"), QStringLiteral("override"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Placed after a member function's signature to state it must override a virtual function from a base class - the compiler rejects the code if no such base function actually exists, catching a common typo/signature-mismatch bug.</p><pre style="white-space:pre;">class Circle : public Shape
{
public:
    LONG Area() const override;   /* must match Shape::Area() exactly */
};</pre><p><b>See also:</b> <a href="cppref:kw_virtual">virtual</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Wird nach der Signatur einer Methode angegeben, um festzulegen, dass sie eine virtuelle Funktion einer Basisklasse überschreiben MUSS – existiert keine solche Basisfunktion, lehnt der Compiler den Code ab. Fängt so einen häufigen Tippfehler-/Signatur-Bug ab.</p><pre style="white-space:pre;">class Circle : public Shape
{
public:
    LONG Area() const override;   /* must match Shape::Area() exactly */
};</pre><p><b>Siehe auch:</b> <a href="cppref:kw_virtual">virtual</a></p>)HTML"));

    addEntry(QStringLiteral("kw_static_cast"), QStringLiteral("Keywords"), QStringLiteral("static_cast"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>A compile-time-checked conversion between related types (numeric conversions, up/down a known class hierarchy, void* to a concrete pointer type) - the everyday, safer replacement for a C-style cast.</p><pre style="white-space:pre;">double d = 3.9;
LONG n = static_cast&lt;LONG&gt;(d);   /* n == 3 */</pre><p><b>See also:</b> <a href="cppref:kw_dynamic_cast">dynamic_cast</a>, <a href="cppref:kw_const_cast">const_cast</a>, <a href="cppref:kw_reinterpret_cast">reinterpret_cast</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Eine zur Compile-Zeit geprüfte Umwandlung zwischen verwandten Typen (numerische Umwandlungen, in einer bekannten Klassenhierarchie auf-/abwärts, void* in einen konkreten Zeigertyp) – der alltägliche, sicherere Ersatz für einen C-Style-Cast.</p><pre style="white-space:pre;">double d = 3.9;
LONG n = static_cast&lt;LONG&gt;(d);   /* n == 3 */</pre><p><b>Siehe auch:</b> <a href="cppref:kw_dynamic_cast">dynamic_cast</a>, <a href="cppref:kw_const_cast">const_cast</a>, <a href="cppref:kw_reinterpret_cast">reinterpret_cast</a></p>)HTML"));

    addEntry(QStringLiteral("kw_dynamic_cast"), QStringLiteral("Keywords"), QStringLiteral("dynamic_cast"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>A run-time-checked downcast within a polymorphic (virtual-function-having) class hierarchy - returns a null pointer (for pointers) if the object isn't actually of the target type, instead of silently producing garbage.</p><pre style="white-space:pre;">Shape *s = GetSomeShape();
Circle *c = dynamic_cast&lt;Circle *&gt;(s);
if (c != nullptr)
{
    /* s really was a Circle */
}</pre><p><b>See also:</b> <a href="cppref:kw_static_cast">static_cast</a>, <a href="cppref:kw_virtual">virtual</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Ein zur Laufzeit geprüftes Downcast innerhalb einer polymorphen (virtuelle Funktionen besitzenden) Klassenhierarchie – liefert bei Zeigern einen Null-Zeiger, falls das Objekt tatsächlich nicht vom Zieltyp ist, statt stillschweigend Unsinn zu erzeugen.</p><pre style="white-space:pre;">Shape *s = GetSomeShape();
Circle *c = dynamic_cast&lt;Circle *&gt;(s);
if (c != nullptr)
{
    /* s really was a Circle */
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_static_cast">static_cast</a>, <a href="cppref:kw_virtual">virtual</a></p>)HTML"));

    addEntry(QStringLiteral("kw_const_cast"), QStringLiteral("Keywords"), QStringLiteral("const_cast"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Adds or removes const (or volatile) from a type - the only standard cast able to do this. A narrow escape hatch, mostly needed when calling an older API that forgot to mark a parameter const.</p><pre style="white-space:pre;">void LegacyPrint(char *s);   /* doesn't modify s, but forgot to say const char * */

const char *msg = "hello";
LegacyPrint(const_cast&lt;char *&gt;(msg));</pre><p><b>See also:</b> <a href="cppref:kw_static_cast">static_cast</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Fügt const (oder volatile) zu einem Typ hinzu oder entfernt es – der einzige Standard-Cast, der das kann. Ein enges Schlupfloch, meist nötig beim Aufruf einer älteren API, die vergessen hat, einen Parameter als const zu markieren.</p><pre style="white-space:pre;">void LegacyPrint(char *s);   /* doesn't modify s, but forgot to say const char * */

const char *msg = "hello";
LegacyPrint(const_cast&lt;char *&gt;(msg));</pre><p><b>Siehe auch:</b> <a href="cppref:kw_static_cast">static_cast</a></p>)HTML"));

    addEntry(QStringLiteral("kw_reinterpret_cast"), QStringLiteral("Keywords"), QStringLiteral("reinterpret_cast"),
              QStringLiteral(R"HTML(<p><i>C++-only keyword — not available in standard C.</i></p><p>Reinterprets one pointer type's bit pattern as another, unrelated pointer type, with no safety checks or conversion logic at all - the most dangerous of the four named casts, used sparingly (e.g. talking to Amiga hardware registers).</p><pre style="white-space:pre;">volatile UWORD *reg = reinterpret_cast&lt;volatile UWORD *&gt;(0xdff016);</pre><p><b>See also:</b> <a href="cppref:kw_static_cast">static_cast</a>, <a href="cppref:kw_const_cast">const_cast</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++ — in Standard-C nicht verfügbar.</i></p><p>Interpretiert das Bitmuster eines Zeigertyps als einen anderen, nicht verwandten Zeigertyp um – ganz ohne Sicherheitsprüfung oder Umwandlungslogik. Der gefährlichste der vier benannten Casts, sparsam eingesetzt (z. B. beim Zugriff auf Amiga-Hardware-Register).</p><pre style="white-space:pre;">volatile UWORD *reg = reinterpret_cast&lt;volatile UWORD *&gt;(0xdff016);</pre><p><b>Siehe auch:</b> <a href="cppref:kw_static_cast">static_cast</a>, <a href="cppref:kw_const_cast">const_cast</a></p>)HTML"));

}

void CppRefReader::populateDatatypes()
{
    addEntry(QStringLiteral("dt_void"), QStringLiteral("Datatypes"), QStringLiteral("void"),
              QStringLiteral(R"HTML(<p><i>Standard C/C++ type.</i></p><p>The "no type" type - used as a function's return type to mean "returns nothing", and as void* for a generic pointer. See the void keyword entry for the full picture.</p><p><b>See also:</b> <a href="cppref:kw_void">void</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Standard-C/C++-Typ.</i></p><p>Der "kein Typ"-Typ – als Rückgabetyp einer Funktion bedeutet er "gibt nichts zurück", als void* ein universeller Zeigertyp. Das vollständige Bild liefert der Eintrag zum Schlüsselwort void.</p><p><b>Siehe auch:</b> <a href="cppref:kw_void">void</a></p>)HTML"));

    addEntry(QStringLiteral("dt_char"), QStringLiteral("Datatypes"), QStringLiteral("char"),
              QStringLiteral(R"HTML(<p><i>Standard C/C++ type.</i></p><p>The smallest addressable integer type, 8 bits on every Amiga toolchain. Whether a plain char defaults to signed or unsigned is implementation-defined - Amiga NDK code sidesteps that entirely with BYTE/UBYTE.</p><pre style="white-space:pre;">char c = 'A';</pre><p><b>See also:</b> <a href="cppref:dt_amiga_byte">BYTE</a>, <a href="cppref:dt_amiga_ubyte">UBYTE</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Standard-C/C++-Typ.</i></p><p>Der kleinste adressierbare Ganzzahltyp, bei jeder Amiga-Toolchain 8 Bit. Ob ein einfaches char standardmäßig signed oder unsigned ist, ist implementierungsabhängig – Amiga-NDK-Code umgeht das komplett mit BYTE/UBYTE.</p><pre style="white-space:pre;">char c = 'A';</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_byte">BYTE</a>, <a href="cppref:dt_amiga_ubyte">UBYTE</a></p>)HTML"));

    addEntry(QStringLiteral("dt_uchar"), QStringLiteral("Datatypes"), QStringLiteral("unsigned char"),
              QStringLiteral(R"HTML(<p><i>Standard C/C++ type.</i></p><p>An 8-bit integer type holding only 0..255 - exactly what the Amiga NDK's own UBYTE is a typedef for.</p><pre style="white-space:pre;">unsigned char flags = 0xFF;</pre><p><b>See also:</b> <a href="cppref:dt_amiga_ubyte">UBYTE</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Standard-C/C++-Typ.</i></p><p>Ein 8-Bit-Ganzzahltyp, der nur 0..255 hält – genau das, wofür UBYTE aus dem Amiga-NDK ein Typedef ist.</p><pre style="white-space:pre;">unsigned char flags = 0xFF;</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_ubyte">UBYTE</a></p>)HTML"));

    addEntry(QStringLiteral("dt_short"), QStringLiteral("Datatypes"), QStringLiteral("short"),
              QStringLiteral(R"HTML(<p><i>Standard C/C++ type.</i></p><p>16 bits wide, signed by default, on every Amiga C toolchain.</p><pre style="white-space:pre;">short delta = -1000;</pre><p><b>See also:</b> <a href="cppref:dt_amiga_word">WORD</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Standard-C/C++-Typ.</i></p><p>16 Bit breit, standardmäßig signed, bei jeder Amiga-C-Toolchain.</p><pre style="white-space:pre;">short delta = -1000;</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_word">WORD</a></p>)HTML"));

    addEntry(QStringLiteral("dt_ushort"), QStringLiteral("Datatypes"), QStringLiteral("unsigned short"),
              QStringLiteral(R"HTML(<p><i>Standard C/C++ type.</i></p><p>16 bits wide, holding only 0..65535 - exactly what the Amiga NDK's own UWORD is a typedef for.</p><pre style="white-space:pre;">unsigned short port = 6502;</pre><p><b>See also:</b> <a href="cppref:dt_amiga_uword">UWORD</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Standard-C/C++-Typ.</i></p><p>16 Bit breit, hält nur 0..65535 – genau das, wofür UWORD aus dem Amiga-NDK ein Typedef ist.</p><pre style="white-space:pre;">unsigned short port = 6502;</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_uword">UWORD</a></p>)HTML"));

    addEntry(QStringLiteral("dt_int"), QStringLiteral("Datatypes"), QStringLiteral("int"),
              QStringLiteral(R"HTML(<p><i>Standard C/C++ type.</i></p><p>32 bits wide on every m68k Amiga toolchain - the same size and range as long there, unlike many other, 16-bit-int platforms of the same era. See the int keyword entry for why the NDK avoids relying on this and uses LONG instead.</p><pre style="white-space:pre;">int n = -5;</pre><p><b>See also:</b> <a href="cppref:kw_int">int</a>, <a href="cppref:dt_amiga_long">LONG</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Standard-C/C++-Typ.</i></p><p>32 Bit breit bei jeder m68k-Amiga-Toolchain – dort dieselbe Größe und derselbe Wertebereich wie long, anders als bei vielen anderen 16-Bit-int-Plattformen derselben Ära. Warum das NDK sich darauf nicht verlässt und stattdessen LONG nutzt, steht beim Schlüsselwort-Eintrag zu int.</p><pre style="white-space:pre;">int n = -5;</pre><p><b>Siehe auch:</b> <a href="cppref:kw_int">int</a>, <a href="cppref:dt_amiga_long">LONG</a></p>)HTML"));

    addEntry(QStringLiteral("dt_uint"), QStringLiteral("Datatypes"), QStringLiteral("unsigned int"),
              QStringLiteral(R"HTML(<p><i>Standard C/C++ type.</i></p><p>32 bits wide on every m68k Amiga toolchain, holding only zero and positive values.</p><pre style="white-space:pre;">unsigned int mask = 0xFFFFFFFFU;</pre><p><b>See also:</b> <a href="cppref:dt_amiga_ulong">ULONG</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Standard-C/C++-Typ.</i></p><p>32 Bit breit bei jeder m68k-Amiga-Toolchain, hält nur Null und positive Werte.</p><pre style="white-space:pre;">unsigned int mask = 0xFFFFFFFFU;</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_ulong">ULONG</a></p>)HTML"));

    addEntry(QStringLiteral("dt_long"), QStringLiteral("Datatypes"), QStringLiteral("long"),
              QStringLiteral(R"HTML(<p><i>Standard C/C++ type.</i></p><p>32 bits wide, signed, on every m68k Amiga toolchain - the C standard type the NDK's own LONG typedef aliases.</p><pre style="white-space:pre;">long offset = -100000L;</pre><p><b>See also:</b> <a href="cppref:dt_amiga_long">LONG</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Standard-C/C++-Typ.</i></p><p>32 Bit breit, signed, bei jeder m68k-Amiga-Toolchain – der C-Standardtyp, den der NDK-eigene Typedef LONG aliasiert.</p><pre style="white-space:pre;">long offset = -100000L;</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_long">LONG</a></p>)HTML"));

    addEntry(QStringLiteral("dt_ulong"), QStringLiteral("Datatypes"), QStringLiteral("unsigned long"),
              QStringLiteral(R"HTML(<p><i>Standard C/C++ type.</i></p><p>32 bits wide on every m68k Amiga toolchain, holding only zero and positive values - the C standard type the NDK's own ULONG typedef aliases.</p><pre style="white-space:pre;">unsigned long size = 4096UL;</pre><p><b>See also:</b> <a href="cppref:dt_amiga_ulong">ULONG</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Standard-C/C++-Typ.</i></p><p>32 Bit breit bei jeder m68k-Amiga-Toolchain, hält nur Null und positive Werte – der C-Standardtyp, den der NDK-eigene Typedef ULONG aliasiert.</p><pre style="white-space:pre;">unsigned long size = 4096UL;</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_ulong">ULONG</a></p>)HTML"));

    addEntry(QStringLiteral("dt_longlong"), QStringLiteral("Datatypes"), QStringLiteral("long long"),
              QStringLiteral(R"HTML(<p><i>Standard C/C++ type.</i></p><p>A C99/C++11 addition, at least 64 bits wide. Not every classic Amiga toolchain supports it - vbcc's and SAS/C's older ISO-C90 modes typically don't; m68k-amigaos-gcc/g++ does in C99/C++11 mode.</p><pre style="white-space:pre;">long long bigCounter = 0;</pre><p><b>Note:</b> Check your specific toolchain's documentation before relying on this in portable Amiga code.</p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Standard-C/C++-Typ.</i></p><p>Eine C99/C++11-Ergänzung, mindestens 64 Bit breit. Nicht jede klassische Amiga-Toolchain unterstützt das – die älteren ISO-C90-Modi von vbcc und SAS/C typischerweise nicht; m68k-amigaos-gcc/g++ im C99/C++11-Modus schon.</p><pre style="white-space:pre;">long long bigCounter = 0;</pre><p><b>Hinweis:</b> Vor der Nutzung in portablem Amiga-Code die Dokumentation der jeweiligen Toolchain prüfen.</p>)HTML"));

    addEntry(QStringLiteral("dt_ulonglong"), QStringLiteral("Datatypes"), QStringLiteral("unsigned long long"),
              QStringLiteral(R"HTML(<p><i>Standard C/C++ type.</i></p><p>The unsigned counterpart to long long - same C99/C++11-only availability caveat applies.</p><p><b>See also:</b> <a href="cppref:dt_longlong">long long</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Standard-C/C++-Typ.</i></p><p>Das unsigned-Gegenstück zu long long – derselbe Vorbehalt (nur C99/C++11) gilt.</p><p><b>Siehe auch:</b> <a href="cppref:dt_longlong">long long</a></p>)HTML"));

    addEntry(QStringLiteral("dt_float"), QStringLiteral("Datatypes"), QStringLiteral("float"),
              QStringLiteral(R"HTML(<p><i>Standard C/C++ type.</i></p><p>Single-precision (32-bit) IEEE-754 floating point. Using float or double anywhere in a project makes AmigaED automatically add the right math library to the generated Makefile.</p><pre style="white-space:pre;">float ratio = 1.5f;</pre><p><b>See also:</b> <a href="cppref:dt_double">double</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Standard-C/C++-Typ.</i></p><p>Einfach genaues (32-Bit) IEEE-754-Gleitkomma. Sobald float oder double irgendwo im Projekt vorkommt, ergänzt AmigaED automatisch die passende Mathe-Bibliothek im erzeugten Makefile.</p><pre style="white-space:pre;">float ratio = 1.5f;</pre><p><b>Siehe auch:</b> <a href="cppref:dt_double">double</a></p>)HTML"));

    addEntry(QStringLiteral("dt_double"), QStringLiteral("Datatypes"), QStringLiteral("double"),
              QStringLiteral(R"HTML(<p><i>Standard C/C++ type.</i></p><p>Double-precision (64-bit) IEEE-754 floating point - roughly twice float's precision, and on the classic 68000/68020 without an FPU, noticeably more CPU cost.</p><pre style="white-space:pre;">double precise = 3.14159265358979;</pre><p><b>See also:</b> <a href="cppref:dt_float">float</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Standard-C/C++-Typ.</i></p><p>Doppelt genaues (64-Bit) IEEE-754-Gleitkomma – etwa doppelte Genauigkeit von float, und auf dem klassischen 68000/68020 ohne FPU spürbar höhere CPU-Kosten.</p><pre style="white-space:pre;">double precise = 3.14159265358979;</pre><p><b>Siehe auch:</b> <a href="cppref:dt_float">float</a></p>)HTML"));

    addEntry(QStringLiteral("dt_longdouble"), QStringLiteral("Datatypes"), QStringLiteral("long double"),
              QStringLiteral(R"HTML(<p><i>Standard C/C++ type.</i></p><p>An extended-precision floating-point type in the C/C++ standard. On many m68k GCC configurations it is the same size as double unless extended FPU precision is specifically configured - check your toolchain rather than assuming extra precision is actually available.</p><pre style="white-space:pre;">long double veryPrecise = 3.14159265358979L;</pre><p><b>See also:</b> <a href="cppref:dt_double">double</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Standard-C/C++-Typ.</i></p><p>Ein erweitert genauer Gleitkommatyp im C/C++-Standard. Bei vielen m68k-GCC-Konfigurationen ist er genauso groß wie double, sofern nicht gezielt erweiterte FPU-Genauigkeit konfiguriert ist – die eigene Toolchain prüfen, statt zusätzliche Genauigkeit einfach anzunehmen.</p><pre style="white-space:pre;">long double veryPrecise = 3.14159265358979L;</pre><p><b>Siehe auch:</b> <a href="cppref:dt_double">double</a></p>)HTML"));

    addEntry(QStringLiteral("dt_bool"), QStringLiteral("Datatypes"), QStringLiteral("bool / _Bool"),
              QStringLiteral(R"HTML(<p><i>Standard C/C++ type.</i></p><p>In C++, bool is a built-in type holding only true/false. In C, the standard equivalent is _Bool (C99), usually spelled bool via &lt;stdbool.h&gt;. AmigaOS C code has traditionally used its own BOOL typedef instead - see the Amiga entry below for how the two relate.</p><pre style="white-space:pre;">#include &lt;stdbool.h&gt;

bool done = false;</pre><p><b>See also:</b> <a href="cppref:kw_bool">bool</a>, <a href="cppref:dt_amiga_bool">BOOL</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Standard-C/C++-Typ.</i></p><p>In C++ ist bool ein eingebauter Typ, der nur true/false hält. In C ist das Standard-Äquivalent _Bool (C99), meist über &lt;stdbool.h&gt; als bool geschrieben. AmigaOS-C-Code nutzt traditionell stattdessen den eigenen Typedef BOOL – wie beide zusammenhängen, steht beim Amiga-Eintrag unten.</p><pre style="white-space:pre;">#include &lt;stdbool.h&gt;

bool done = false;</pre><p><b>Siehe auch:</b> <a href="cppref:kw_bool">bool</a>, <a href="cppref:dt_amiga_bool">BOOL</a></p>)HTML"));

    addEntry(QStringLiteral("dt_amiga_byte"), QStringLiteral("Datatypes"), QStringLiteral("BYTE"),
              QStringLiteral(R"HTML(<p><i>AmigaOS NDK typedef, &lt;exec/types.h&gt;.</i></p><p>An 8-bit signed integer: <span style="font-family:monospace;">typedef signed char BYTE;</span> The explicit-width counterpart to a plain char when you specifically need a signed 8-bit number, not a character.</p><pre style="white-space:pre;">BYTE level = -12;</pre><p><b>See also:</b> <a href="cppref:dt_amiga_ubyte">UBYTE</a>, <a href="cppref:dt_char">char</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>AmigaOS-NDK-Typedef, &lt;exec/types.h&gt;.</i></p><p>Eine 8-Bit-Ganzzahl mit Vorzeichen: <span style="font-family:monospace;">typedef signed char BYTE;</span> Das Gegenstück mit expliziter Breite zu einem einfachen char, wenn gezielt eine vorzeichenbehaftete 8-Bit-Zahl gebraucht wird, kein Zeichen.</p><pre style="white-space:pre;">BYTE level = -12;</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_ubyte">UBYTE</a>, <a href="cppref:dt_char">char</a></p>)HTML"));

    addEntry(QStringLiteral("dt_amiga_ubyte"), QStringLiteral("Datatypes"), QStringLiteral("UBYTE"),
              QStringLiteral(R"HTML(<p><i>AmigaOS NDK typedef, &lt;exec/types.h&gt;.</i></p><p>An 8-bit unsigned integer: <span style="font-family:monospace;">typedef unsigned char UBYTE;</span> Used constantly throughout the NDK, including as the base type behind STRPTR (a UBYTE pointer, i.e. a string).</p><pre style="white-space:pre;">UBYTE flags = 0xFF;</pre><p><b>See also:</b> <a href="cppref:dt_amiga_byte">BYTE</a>, <a href="cppref:dt_amiga_strptr">STRPTR</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>AmigaOS-NDK-Typedef, &lt;exec/types.h&gt;.</i></p><p>Eine 8-Bit-Ganzzahl ohne Vorzeichen: <span style="font-family:monospace;">typedef unsigned char UBYTE;</span> Wird im gesamten NDK ständig verwendet, unter anderem als Basistyp hinter STRPTR (ein UBYTE-Zeiger, also ein String).</p><pre style="white-space:pre;">UBYTE flags = 0xFF;</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_byte">BYTE</a>, <a href="cppref:dt_amiga_strptr">STRPTR</a></p>)HTML"));

    addEntry(QStringLiteral("dt_amiga_word"), QStringLiteral("Datatypes"), QStringLiteral("WORD"),
              QStringLiteral(R"HTML(<p><i>AmigaOS NDK typedef, &lt;exec/types.h&gt;.</i></p><p>A 16-bit signed integer: <span style="font-family:monospace;">typedef short WORD;</span></p><pre style="white-space:pre;">WORD delta = -1000;</pre><p><b>See also:</b> <a href="cppref:dt_amiga_uword">UWORD</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>AmigaOS-NDK-Typedef, &lt;exec/types.h&gt;.</i></p><p>Eine 16-Bit-Ganzzahl mit Vorzeichen: <span style="font-family:monospace;">typedef short WORD;</span></p><pre style="white-space:pre;">WORD delta = -1000;</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_uword">UWORD</a></p>)HTML"));

    addEntry(QStringLiteral("dt_amiga_uword"), QStringLiteral("Datatypes"), QStringLiteral("UWORD"),
              QStringLiteral(R"HTML(<p><i>AmigaOS NDK typedef, &lt;exec/types.h&gt;.</i></p><p>A 16-bit unsigned integer: <span style="font-family:monospace;">typedef unsigned short UWORD;</span></p><pre style="white-space:pre;">UWORD custom_reg_value;</pre><p><b>See also:</b> <a href="cppref:dt_amiga_word">WORD</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>AmigaOS-NDK-Typedef, &lt;exec/types.h&gt;.</i></p><p>Eine 16-Bit-Ganzzahl ohne Vorzeichen: <span style="font-family:monospace;">typedef unsigned short UWORD;</span></p><pre style="white-space:pre;">UWORD custom_reg_value;</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_word">WORD</a></p>)HTML"));

    addEntry(QStringLiteral("dt_amiga_long"), QStringLiteral("Datatypes"), QStringLiteral("LONG"),
              QStringLiteral(R"HTML(<p><i>AmigaOS NDK typedef, &lt;exec/types.h&gt;.</i></p><p>A 32-bit signed integer: <span style="font-family:monospace;">typedef long LONG;</span> The NDK's preferred explicit-width type wherever "int" would otherwise leave the exact size to the platform - see the int keyword entry.</p><pre style="white-space:pre;">LONG result = SomeFunction();</pre><p><b>See also:</b> <a href="cppref:dt_amiga_ulong">ULONG</a>, <a href="cppref:kw_int">int</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>AmigaOS-NDK-Typedef, &lt;exec/types.h&gt;.</i></p><p>Eine 32-Bit-Ganzzahl mit Vorzeichen: <span style="font-family:monospace;">typedef long LONG;</span> Der vom NDK bevorzugte Typ mit expliziter Breite, überall dort, wo "int" die genaue Größe sonst der Plattform überlassen würde – siehe den Eintrag zum Schlüsselwort int.</p><pre style="white-space:pre;">LONG result = SomeFunction();</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_ulong">ULONG</a>, <a href="cppref:kw_int">int</a></p>)HTML"));

    addEntry(QStringLiteral("dt_amiga_ulong"), QStringLiteral("Datatypes"), QStringLiteral("ULONG"),
              QStringLiteral(R"HTML(<p><i>AmigaOS NDK typedef, &lt;exec/types.h&gt;.</i></p><p>A 32-bit unsigned integer: <span style="font-family:monospace;">typedef unsigned long ULONG;</span> Common for sizes, flags/bitmasks, and absolute addresses treated as plain numbers.</p><pre style="white-space:pre;">ULONG signals = SetSignal(0, 0);</pre><p><b>See also:</b> <a href="cppref:dt_amiga_long">LONG</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>AmigaOS-NDK-Typedef, &lt;exec/types.h&gt;.</i></p><p>Eine 32-Bit-Ganzzahl ohne Vorzeichen: <span style="font-family:monospace;">typedef unsigned long ULONG;</span> Häufig für Größenangaben, Flags/Bitmasken und als reine Zahl behandelte absolute Adressen.</p><pre style="white-space:pre;">ULONG signals = SetSignal(0, 0);</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_long">LONG</a></p>)HTML"));

    addEntry(QStringLiteral("dt_amiga_bool"), QStringLiteral("Datatypes"), QStringLiteral("BOOL"),
              QStringLiteral(R"HTML(<p><i>AmigaOS NDK typedef, &lt;exec/types.h&gt;.</i></p><p>AmigaOS's own boolean type, holding TRUE or FALSE. Classic &lt;exec/types.h&gt; headers define it as a full LONG (32-bit) rather than a single byte - check your own NDK's header for the exact expansion, since this has been known to vary slightly by NDK revision/compiler.</p><pre style="white-space:pre;">BOOL success = DoTheThing();
if (success == TRUE)
{
    printf("worked\n");
}</pre><p><b>See also:</b> <a href="cppref:kw_bool">bool</a>, <a href="cppref:dt_bool">bool / _Bool</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>AmigaOS-NDK-Typedef, &lt;exec/types.h&gt;.</i></p><p>AmigaOS' eigener boolescher Typ, der TRUE oder FALSE hält. Klassische &lt;exec/types.h&gt;-Header definieren ihn als vollen LONG (32 Bit) statt als einzelnes Byte – die genaue Definition am besten im eigenen NDK-Header prüfen, da dies je nach NDK-Revision/Compiler leicht variieren kann.</p><pre style="white-space:pre;">BOOL success = DoTheThing();
if (success == TRUE)
{
    printf("worked\n");
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_bool">bool</a>, <a href="cppref:dt_bool">bool / _Bool</a></p>)HTML"));

    addEntry(QStringLiteral("dt_amiga_void"), QStringLiteral("Datatypes"), QStringLiteral("VOID"),
              QStringLiteral(R"HTML(<p><i>AmigaOS NDK typedef, &lt;exec/types.h&gt;.</i></p><p>Historically <span style="font-family:monospace;">#define VOID void</span> in &lt;exec/types.h&gt; - a leftover from pre-ANSI-C compilers that didn't reliably support a real void keyword. Modern code can and should just use void directly; VOID mostly still appears in older NDK headers and code that mirrors them.</p><pre style="white-space:pre;">VOID MyFunction(VOID);   /* old style - same as: void MyFunction(void); */</pre><p><b>See also:</b> <a href="cppref:kw_void">void</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>AmigaOS-NDK-Typedef, &lt;exec/types.h&gt;.</i></p><p>Historisch <span style="font-family:monospace;">#define VOID void</span> in &lt;exec/types.h&gt; – ein Überbleibsel aus Vor-ANSI-C-Compilern, die ein echtes void-Schlüsselwort nicht zuverlässig unterstützten. Moderner Code kann und sollte direkt void verwenden; VOID taucht heute noch vor allem in älteren NDK-Headern und daran angelehntem Code auf.</p><pre style="white-space:pre;">VOID MyFunction(VOID);   /* old style - same as: void MyFunction(void); */</pre><p><b>Siehe auch:</b> <a href="cppref:kw_void">void</a></p>)HTML"));

    addEntry(QStringLiteral("dt_amiga_aptr"), QStringLiteral("Datatypes"), QStringLiteral("APTR"),
              QStringLiteral(R"HTML(<p><i>AmigaOS NDK typedef, &lt;exec/types.h&gt;.</i></p><p>A generic pointer type: <span style="font-family:monospace;">typedef void *APTR;</span> Used throughout the NDK wherever a function hands back or accepts "a pointer to something", the same role void* plays in standard C.</p><pre style="white-space:pre;">APTR mem = AllocMem(1024, MEMF_PUBLIC);</pre><p><b>See also:</b> <a href="cppref:kw_void">void</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>AmigaOS-NDK-Typedef, &lt;exec/types.h&gt;.</i></p><p>Ein universeller Zeigertyp: <span style="font-family:monospace;">typedef void *APTR;</span> Wird im gesamten NDK verwendet, wo eine Funktion "einen Zeiger auf irgendetwas" zurückgibt oder entgegennimmt – dieselbe Rolle, die void* in Standard-C spielt.</p><pre style="white-space:pre;">APTR mem = AllocMem(1024, MEMF_PUBLIC);</pre><p><b>Siehe auch:</b> <a href="cppref:kw_void">void</a></p>)HTML"));

    addEntry(QStringLiteral("dt_amiga_strptr"), QStringLiteral("Datatypes"), QStringLiteral("STRPTR"),
              QStringLiteral(R"HTML(<p><i>AmigaOS NDK typedef, &lt;exec/types.h&gt;.</i></p><p>A pointer to a nul-terminated string of UBYTEs: <span style="font-family:monospace;">typedef UBYTE *STRPTR;</span> Functionally the same idea as a plain char* string, just spelled in the NDK's own explicit-width vocabulary.</p><pre style="white-space:pre;">STRPTR name = (STRPTR) "AmigaED";</pre><p><b>See also:</b> <a href="cppref:dt_amiga_conststrptr">CONST_STRPTR</a>, <a href="cppref:dt_amiga_ubyte">UBYTE</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>AmigaOS-NDK-Typedef, &lt;exec/types.h&gt;.</i></p><p>Ein Zeiger auf einen nullterminierten String aus UBYTEs: <span style="font-family:monospace;">typedef UBYTE *STRPTR;</span> Funktional dieselbe Idee wie ein normaler char*-String, nur im eigenen Vokabular des NDKs mit expliziter Breite formuliert.</p><pre style="white-space:pre;">STRPTR name = (STRPTR) "AmigaED";</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_conststrptr">CONST_STRPTR</a>, <a href="cppref:dt_amiga_ubyte">UBYTE</a></p>)HTML"));

    addEntry(QStringLiteral("dt_amiga_conststrptr"), QStringLiteral("Datatypes"), QStringLiteral("CONST_STRPTR"),
              QStringLiteral(R"HTML(<p><i>AmigaOS NDK typedef, &lt;exec/types.h&gt;.</i></p><p>A read-only string pointer: <span style="font-family:monospace;">typedef CONST UBYTE *CONST_STRPTR;</span> Added in a later NDK revision so a function that only reads a string (and never modifies it) can say so in its own signature, mirroring how const char* is used in standard C.</p><pre style="white-space:pre;">void PrintName(CONST_STRPTR name);</pre><p><b>See also:</b> <a href="cppref:dt_amiga_strptr">STRPTR</a>, <a href="cppref:kw_const">const</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>AmigaOS-NDK-Typedef, &lt;exec/types.h&gt;.</i></p><p>Ein schreibgeschützter String-Zeiger: <span style="font-family:monospace;">typedef CONST UBYTE *CONST_STRPTR;</span> In einer späteren NDK-Revision ergänzt, damit eine Funktion, die einen String nur liest (und nie ändert), dies in der eigenen Signatur ausdrücken kann – vergleichbar mit const char* in Standard-C.</p><pre style="white-space:pre;">void PrintName(CONST_STRPTR name);</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_strptr">STRPTR</a>, <a href="cppref:kw_const">const</a></p>)HTML"));

    addEntry(QStringLiteral("dt_amiga_bptr"), QStringLiteral("Datatypes"), QStringLiteral("BPTR"),
              QStringLiteral(R"HTML(<p><i>AmigaOS NDK typedef, &lt;exec/types.h&gt;.</i></p><p>AmigaDOS's own "byte pointer" type - a long-word-aligned address encoded shifted right by 2 bits rather than an ordinary C pointer, historically <span style="font-family:monospace;">typedef LONG BPTR;</span> Most often seen as the type of a DOS file handle (e.g. Open()'s return value); do not dereference it like a normal pointer without going through the proper AmigaDOS macros/functions (e.g. BADDR()).</p><pre style="white-space:pre;">BPTR fh = Open("RAM:test.txt", MODE_NEWFILE);
if (fh)
{
    Write(fh, "hello", 5);
    Close(fh);
}</pre><p><b>Note:</b> Some newer NDK/64-bit-safe headers give BPTR its own distinct, opaque type instead of a plain LONG - check your own &lt;exec/types.h&gt;/&lt;dos/dos.h&gt; rather than assuming the classic definition unconditionally.</p><p><b>See also:</b> <a href="cppref:dt_amiga_bstr">BSTR</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>AmigaOS-NDK-Typedef, &lt;exec/types.h&gt;.</i></p><p>AmigaDOS' eigener "Byte-Zeiger"-Typ – eine langwort-ausgerichtete Adresse, um 2 Bit nach rechts verschoben kodiert, kein gewöhnlicher C-Zeiger, historisch <span style="font-family:monospace;">typedef LONG BPTR;</span> Am häufigsten als Typ eines DOS-Datei-Handles zu sehen (z. B. Rückgabewert von Open()); nicht wie einen normalen Zeiger dereferenzieren, ohne die passenden AmigaDOS-Makros/Funktionen (z. B. BADDR()) zu verwenden.</p><pre style="white-space:pre;">BPTR fh = Open("RAM:test.txt", MODE_NEWFILE);
if (fh)
{
    Write(fh, "hello", 5);
    Close(fh);
}</pre><p><b>Hinweis:</b> Manche neueren NDK-/64-Bit-sicheren Header geben BPTR einen eigenen, undurchsichtigen Typ statt eines einfachen LONG – am besten im eigenen &lt;exec/types.h&gt;/&lt;dos/dos.h&gt; nachsehen, statt die klassische Definition ungeprüft anzunehmen.</p><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_bstr">BSTR</a></p>)HTML"));

    addEntry(QStringLiteral("dt_amiga_bstr"), QStringLiteral("Datatypes"), QStringLiteral("BSTR"),
              QStringLiteral(R"HTML(<p><i>AmigaOS NDK typedef, &lt;exec/types.h&gt;.</i></p><p>A BCPL-style string pointer, tracing back to AmigaDOS's BCPL heritage: a BPTR-encoded pointer to a length-prefixed string (its first byte holds the string's length, with no nul terminator), historically <span style="font-family:monospace;">typedef LONG BSTR;</span> Rare in day-to-day application code - mostly relevant when working directly with low-level AmigaDOS structures.</p><p><b>See also:</b> <a href="cppref:dt_amiga_bptr">BPTR</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>AmigaOS-NDK-Typedef, &lt;exec/types.h&gt;.</i></p><p>Ein String-Zeiger im BCPL-Stil, ein Erbe von AmigaDOS' BCPL-Ursprung: ein BPTR-kodierter Zeiger auf einen längenpräfixierten String (sein erstes Byte enthält die Länge, ohne Nullterminierung), historisch <span style="font-family:monospace;">typedef LONG BSTR;</span> Im Alltag der Anwendungsprogrammierung selten – meist relevant bei direkter Arbeit mit den systemnahen AmigaDOS-Strukturen.</p><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_bptr">BPTR</a></p>)HTML"));

    addEntry(QStringLiteral("dt_amiga_fixed"), QStringLiteral("Datatypes"), QStringLiteral("Fixed"),
              QStringLiteral(R"HTML(<p><i>AmigaOS NDK typedef, &lt;exec/types.h&gt;.</i></p><p>A 32-bit fixed-point number in 16.16 format (16 bits of integer part, 16 bits of fraction), historically <span style="font-family:monospace;">typedef LONG Fixed;</span> Used by parts of graphics.library and other places needing fractional precision without the cost (or unavailability) of real floating point on the classic hardware.</p><p><b>See also:</b> <a href="cppref:dt_amiga_ufixed">UFixed</a>, <a href="cppref:dt_amiga_long">LONG</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>AmigaOS-NDK-Typedef, &lt;exec/types.h&gt;.</i></p><p>Eine 32-Bit-Festkommazahl im Format 16.16 (16 Bit Ganzzahlanteil, 16 Bit Nachkommaanteil), historisch <span style="font-family:monospace;">typedef LONG Fixed;</span> Wird u. a. von Teilen der graphics.library verwendet, wo Nachkommagenauigkeit ohne die Kosten (oder mangels Verfügbarkeit) echter Gleitkommazahlen auf der klassischen Hardware benötigt wird.</p><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_ufixed">UFixed</a>, <a href="cppref:dt_amiga_long">LONG</a></p>)HTML"));

    addEntry(QStringLiteral("dt_amiga_ufixed"), QStringLiteral("Datatypes"), QStringLiteral("UFixed"),
              QStringLiteral(R"HTML(<p><i>AmigaOS NDK typedef, &lt;exec/types.h&gt;.</i></p><p>The unsigned counterpart to Fixed - a 32-bit unsigned 16.16 fixed-point number, historically <span style="font-family:monospace;">typedef ULONG UFixed;</span></p><p><b>See also:</b> <a href="cppref:dt_amiga_fixed">Fixed</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>AmigaOS-NDK-Typedef, &lt;exec/types.h&gt;.</i></p><p>Das unsigned-Gegenstück zu Fixed – eine vorzeichenlose 32-Bit-Festkommazahl im Format 16.16, historisch <span style="font-family:monospace;">typedef ULONG UFixed;</span></p><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_fixed">Fixed</a></p>)HTML"));

    addEntry(QStringLiteral("dt_overview"), QStringLiteral("Datatypes"), (p_lang == QStringLiteral("de")) ? QStringLiteral("Übersicht: Größen & Wertebereiche") : QStringLiteral("Overview: Sizes & Value Ranges"),
              QStringLiteral(R"HTML(<p><i>Reference table.</i></p><p>Sizes and ranges below are for the classic 32-bit m68k Amiga toolchains this project targets (vbcc, SAS/C, m68k-amigaos-gcc/g++). Always verify against your own compiler's documentation, especially for <a href="cppref:dt_longdouble">long double</a> and <a href="cppref:dt_amiga_bool">BOOL</a>/<a href="cppref:dt_amiga_bptr">BPTR</a>/<a href="cppref:dt_amiga_bstr">BSTR</a>, which have varied historically by NDK revision and compiler. Click any type name below to jump to its own full entry.</p><h4>Standard C/C++ Types</h4><table border="0" cellspacing="0" cellpadding="0" style="border-collapse:collapse;"><tr><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Type</th><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Size</th><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Range / Values</th><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Notes</th></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">void</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">&#8211;</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">(no value)</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">see the <a href="cppref:kw_void">void</a> keyword</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">char</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">1 byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">implementation-defined signedness, typically -128..127</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">Amiga NDK uses <a href="cppref:dt_amiga_byte">BYTE</a>/<a href="cppref:dt_amiga_ubyte">UBYTE</a> instead</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned char</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">1 byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">0..255</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">= <a href="cppref:dt_amiga_ubyte">UBYTE</a></td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">short</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">2 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">-32768..32767</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">= <a href="cppref:dt_amiga_word">WORD</a></td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned short</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">2 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">0..65535</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">= <a href="cppref:dt_amiga_uword">UWORD</a></td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">int</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">-2147483648..2147483647</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">same size as long on m68k Amiga</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned int</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">0..4294967295</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;"></td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">long</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">-2147483648..2147483647</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">= <a href="cppref:dt_amiga_long">LONG</a></td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned long</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">0..4294967295</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">= <a href="cppref:dt_amiga_ulong">ULONG</a></td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">long long</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">&#8805; 8 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">at least -9223372036854775808..9223372036854775807</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">C99/C++11, not every classic toolchain supports it</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned long long</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">&#8805; 8 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">at least 0..18446744073709551615</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">C99/C++11, same caveat</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">float</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">&#8776; &#177;1.2&#215;10&#8315;&#179;&#8312; to &#177;3.4&#215;10&#179;&#8312;</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">IEEE-754 single, &#8776; 6-7 significant digits</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">double</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">8 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">&#8776; &#177;2.2&#215;10&#8315;&#179;&#8304;&#8312; to &#177;1.8&#215;10&#179;&#8304;&#8312;</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">IEEE-754 double, &#8776; 15-16 significant digits</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">long double</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">toolchain-dependent</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">often same range as double</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">extended precision only if specifically configured - check your toolchain</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">bool / _Bool</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">typically 1 byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">false (0) / true (1)</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">C++ built-in / C99 (usually spelled bool via &lt;stdbool.h&gt;)</td></tr></table><h4>AmigaOS NDK Typedefs (&lt;exec/types.h&gt;)</h4><table border="0" cellspacing="0" cellpadding="0" style="border-collapse:collapse;"><tr><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Type</th><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Size</th><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Range / Values</th><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Underlying C Type</th></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_byte">BYTE</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">1 byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">-128..127</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">signed char</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_ubyte">UBYTE</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">1 byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">0..255</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned char</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_word">WORD</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">2 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">-32768..32767</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">short</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_uword">UWORD</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">2 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">0..65535</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned short</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_long">LONG</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">-2147483648..2147483647</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">long</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_ulong">ULONG</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">0..4294967295</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned long</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_bool">BOOL</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 bytes (classic)</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">FALSE (0) / TRUE (1)</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">LONG (classic - check your NDK)</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_void">VOID</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">&#8211;</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">(no value)</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">void (historical #define)</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_aptr">APTR</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">any 32-bit address</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">void*</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_strptr">STRPTR</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">any 32-bit address</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">UBYTE*</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_conststrptr">CONST_STRPTR</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">any 32-bit address</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">CONST UBYTE*</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_bptr">BPTR</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">BPTR-encoded address (shifted right 2 bits)</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">historically LONG</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_bstr">BSTR</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">BPTR-encoded, length-prefixed string pointer</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">historically LONG</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_fixed">Fixed</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">16.16 fixed-point, &#8776; &#177;32768 in 1/65536 steps</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">LONG</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_ufixed">UFixed</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 bytes</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">16.16 fixed-point, 0..&#8776;65536 in 1/65536 steps</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">ULONG</td></tr></table>)HTML"),
              QStringLiteral(R"HTML(<p><i>Referenztabelle.</i></p><p>Die unten stehenden Größen und Wertebereiche gelten für die klassischen 32-Bit-m68k-Amiga-Toolchains, die dieses Projekt anspricht (vbcc, SAS/C, m68k-amigaos-gcc/g++). Stets gegen die Dokumentation des eigenen Compilers prüfen, besonders bei <a href="cppref:dt_longdouble">long double</a> und <a href="cppref:dt_amiga_bool">BOOL</a>/<a href="cppref:dt_amiga_bptr">BPTR</a>/<a href="cppref:dt_amiga_bstr">BSTR</a>, die historisch je nach NDK-Revision und Compiler variiert haben. Auf einen Typnamen unten klicken, um zu dessen vollständigem Eintrag zu springen.</p><h4>Standard-C/C++-Typen</h4><table border="0" cellspacing="0" cellpadding="0" style="border-collapse:collapse;"><tr><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Typ</th><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Größe</th><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Wertebereich</th><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Hinweise</th></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">void</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">&#8211;</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">(kein Wert)</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">siehe Schlüsselwort <a href="cppref:kw_void">void</a></td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">char</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">1 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">Vorzeichen implementierungsabhängig, typisch -128..127</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">Amiga-NDK nutzt stattdessen <a href="cppref:dt_amiga_byte">BYTE</a>/<a href="cppref:dt_amiga_ubyte">UBYTE</a></td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned char</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">1 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">0..255</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">= <a href="cppref:dt_amiga_ubyte">UBYTE</a></td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">short</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">2 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">-32768..32767</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">= <a href="cppref:dt_amiga_word">WORD</a></td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned short</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">2 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">0..65535</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">= <a href="cppref:dt_amiga_uword">UWORD</a></td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">int</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">-2147483648..2147483647</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">auf m68k-Amiga dieselbe Größe wie long</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned int</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">0..4294967295</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;"></td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">long</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">-2147483648..2147483647</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">= <a href="cppref:dt_amiga_long">LONG</a></td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned long</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">0..4294967295</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">= <a href="cppref:dt_amiga_ulong">ULONG</a></td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">long long</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">&#8805; 8 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">mindestens -9223372036854775808..9223372036854775807</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">C99/C++11, nicht jede klassische Toolchain unterstützt es</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned long long</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">&#8805; 8 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">mindestens 0..18446744073709551615</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">C99/C++11, gleicher Vorbehalt</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">float</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">&#8776; &#177;1,2&#215;10&#8315;&#179;&#8312; bis &#177;3,4&#215;10&#179;&#8312;</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">IEEE-754 einfach genau, &#8776; 6-7 signifikante Stellen</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">double</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">8 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">&#8776; &#177;2,2&#215;10&#8315;&#179;&#8304;&#8312; bis &#177;1,8&#215;10&#179;&#8304;&#8312;</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">IEEE-754 doppelt genau, &#8776; 15-16 signifikante Stellen</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">long double</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">toolchain-abhängig</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">oft derselbe Bereich wie double</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">erweiterte Genauigkeit nur bei gezielter Konfiguration - eigene Toolchain prüfen</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;">bool / _Bool</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">typisch 1 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">false (0) / true (1)</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">C++-eingebaut / C99 (meist über &lt;stdbool.h&gt; als bool geschrieben)</td></tr></table><h4>AmigaOS-NDK-Typedefs (&lt;exec/types.h&gt;)</h4><table border="0" cellspacing="0" cellpadding="0" style="border-collapse:collapse;"><tr><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Typ</th><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Größe</th><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Wertebereich</th><th style="text-align:left;padding:3px 8px;border-bottom:2px solid #888;">Zugrundeliegender C-Typ</th></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_byte">BYTE</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">1 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">-128..127</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">signed char</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_ubyte">UBYTE</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">1 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">0..255</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned char</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_word">WORD</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">2 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">-32768..32767</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">short</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_uword">UWORD</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">2 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">0..65535</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned short</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_long">LONG</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">-2147483648..2147483647</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">long</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_ulong">ULONG</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">0..4294967295</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">unsigned long</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_bool">BOOL</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 Byte (klassisch)</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">FALSE (0) / TRUE (1)</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">LONG (klassisch - eigenes NDK prüfen)</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_void">VOID</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">&#8211;</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">(kein Wert)</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">void (historisches #define)</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_aptr">APTR</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">beliebige 32-Bit-Adresse</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">void*</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_strptr">STRPTR</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">beliebige 32-Bit-Adresse</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">UBYTE*</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_conststrptr">CONST_STRPTR</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">beliebige 32-Bit-Adresse</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">CONST UBYTE*</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_bptr">BPTR</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">BPTR-kodierte Adresse (um 2 Bit nach rechts verschoben)</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">historisch LONG</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_bstr">BSTR</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">BPTR-kodierter, längenpräfixierter String-Zeiger</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">historisch LONG</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_fixed">Fixed</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">16.16-Festkomma, &#8776; &#177;32768 in 1/65536-Schritten</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">LONG</td></tr><tr><td style="padding:3px 8px;border-bottom:1px solid #ccc;"><a href="cppref:dt_amiga_ufixed">UFixed</a></td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">4 Byte</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">16.16-Festkomma, 0..&#8776;65536 in 1/65536-Schritten</td><td style="padding:3px 8px;border-bottom:1px solid #ccc;">ULONG</td></tr></table>)HTML"));

}

void CppRefReader::populateVariables()
{
    addEntry(QStringLiteral("var_declaration"), QStringLiteral("Variables"), QStringLiteral("Declaring & Defining a Variable"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>A variable brings together a name, a type, and (for most variables) a storage location. A DEFINITION creates that storage (and may initialize it); a DECLARATION merely refers to a definition that lives elsewhere, using extern.</p><pre style="white-space:pre;">int count = 0;          /* definition: creates storage, initializes it */
extern int errorCode;   /* declaration only: defined in another file */</pre><p><b>See also:</b> <a href="cppref:var_scope_local">Local (Block) Scope</a>, <a href="cppref:var_scope_global">Global (File) Scope</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Eine Variable vereint einen Namen, einen Typ und (bei den meisten Variablen) einen Speicherort. Eine DEFINITION erzeugt diesen Speicher (und initialisiert ihn ggf.); eine DEKLARATION verweist nur auf eine Definition, die anderswo liegt - mit extern.</p><pre style="white-space:pre;">int count = 0;          /* definition: creates storage, initializes it */
extern int errorCode;   /* declaration only: defined in another file */</pre><p><b>Siehe auch:</b> <a href="cppref:var_scope_local">Local (Block) Scope</a>, <a href="cppref:var_scope_global">Global (File) Scope</a></p>)HTML"));

    addEntry(QStringLiteral("var_scope_local"), QStringLiteral("Variables"), QStringLiteral("Local (Block) Scope"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>A variable declared inside { } is visible only within that block (and any blocks nested inside it), and its storage normally exists only while that block is executing - each call/iteration gets a fresh one, unless it is declared static.</p><pre style="white-space:pre;">for (int i = 0; i &lt; 3; i++)
{
    int square = i * i;   /* square only exists inside this block */
    printf("%d\n", square);
}
/* neither i nor square exist here anymore */</pre><p><b>See also:</b> <a href="cppref:var_scope_global">Global (File) Scope</a>, <a href="cppref:var_static">static Variables & Lifetime</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Eine innerhalb von { } deklarierte Variable ist nur innerhalb dieses Blocks (und darin verschachtelter Blöcke) sichtbar, und ihr Speicher existiert normalerweise nur, solange dieser Block läuft - jeder Aufruf/Durchlauf bekommt eine frische Variable, außer sie ist static.</p><pre style="white-space:pre;">for (int i = 0; i &lt; 3; i++)
{
    int square = i * i;   /* square only exists inside this block */
    printf("%d\n", square);
}
/* neither i nor square exist here anymore */</pre><p><b>Siehe auch:</b> <a href="cppref:var_scope_global">Global (File) Scope</a>, <a href="cppref:var_static">static Variables & Lifetime</a></p>)HTML"));

    addEntry(QStringLiteral("var_scope_global"), QStringLiteral("Variables"), QStringLiteral("Global (File) Scope"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>A variable declared outside any function has file scope: by default it is visible to every function in that file, and via an extern declaration to other files too, and it exists for the entire lifetime of the program. Useful, but best kept to a minimum - each one is effectively shared, mutable state.</p><pre style="white-space:pre;">int gErrorCount = 0;   /* file scope - exists for the whole program */

void reportError(void)
{
    gErrorCount++;
}</pre><p><b>See also:</b> <a href="cppref:var_scope_local">Local (Block) Scope</a>, <a href="cppref:var_static">static Variables & Lifetime</a>, <a href="cppref:kw_extern">extern</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Eine außerhalb jeder Funktion deklarierte Variable hat Dateibereich (file scope): Sie ist standardmäßig für alle Funktionen dieser Datei sichtbar, über eine extern-Deklaration auch für andere Dateien, und existiert für die gesamte Laufzeit des Programms. Nützlich, sollte aber auf ein Minimum beschränkt bleiben - jede ist im Grunde geteilter, veränderlicher Zustand.</p><pre style="white-space:pre;">int gErrorCount = 0;   /* file scope - exists for the whole program */

void reportError(void)
{
    gErrorCount++;
}</pre><p><b>Siehe auch:</b> <a href="cppref:var_scope_local">Local (Block) Scope</a>, <a href="cppref:var_static">static Variables & Lifetime</a>, <a href="cppref:kw_extern">extern</a></p>)HTML"));

    addEntry(QStringLiteral("var_static"), QStringLiteral("Variables"), QStringLiteral("static Variables & Lifetime"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>static changes a variable's lifetime and/or visibility depending on where it is declared. Inside a function, a static local variable is initialized only once and keeps its value between calls, instead of being recreated every time. At file scope, static limits a variable's visibility to that one file ("internal linkage") instead of the whole program.</p><pre style="white-space:pre;">void countCalls(void)
{
    static int calls = 0;   /* initialized once, keeps its value */
    calls++;
    printf("called %d times\n", calls);
}</pre><p><b>See also:</b> <a href="cppref:kw_static">static</a>, <a href="cppref:var_scope_global">Global (File) Scope</a>, <a href="cppref:fn_static_functions">static Functions (Internal Linkage)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>static ändert Lebensdauer und/oder Sichtbarkeit einer Variablen, je nachdem wo sie deklariert ist. Innerhalb einer Funktion wird eine static-lokale Variable nur einmal initialisiert und behält ihren Wert zwischen Aufrufen, statt jedes Mal neu angelegt zu werden. Auf Dateiebene beschränkt static die Sichtbarkeit einer Variablen auf genau diese Datei ("interne Bindung") statt auf das ganze Programm.</p><pre style="white-space:pre;">void countCalls(void)
{
    static int calls = 0;   /* initialized once, keeps its value */
    calls++;
    printf("called %d times\n", calls);
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_static">static</a>, <a href="cppref:var_scope_global">Global (File) Scope</a>, <a href="cppref:fn_static_functions">static Functions (Internal Linkage)</a></p>)HTML"));

    addEntry(QStringLiteral("var_const"), QStringLiteral("Variables"), QStringLiteral("const Variables"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>A const variable cannot be reassigned after it is initialized. Unlike a preprocessor macro, it is a real, typed variable that participates in normal scoping and type checking, and (except when declared extern) must be initialized right where it is declared.</p><pre style="white-space:pre;">const int maxPlayers = 4;
/* maxPlayers = 5;  -- compile error: assignment of read-only variable */</pre><p><b>See also:</b> <a href="cppref:kw_const">const</a>, <a href="cppref:var_constexpr">constexpr Variables (C++)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Eine const-Variable kann nach ihrer Initialisierung nicht mehr neu zugewiesen werden. Anders als ein Präprozessor-Makro ist sie eine echte, typisierte Variable, die an normaler Sichtbarkeitsprüfung und Typprüfung teilnimmt, und muss (außer bei extern) direkt bei ihrer Deklaration initialisiert werden.</p><pre style="white-space:pre;">const int maxPlayers = 4;
/* maxPlayers = 5;  -- compile error: assignment of read-only variable */</pre><p><b>Siehe auch:</b> <a href="cppref:kw_const">const</a>, <a href="cppref:var_constexpr">constexpr Variables (C++)</a></p>)HTML"));

    addEntry(QStringLiteral("var_constexpr"), QStringLiteral("Variables"), QStringLiteral("constexpr Variables (C++)"),
              QStringLiteral(R"HTML(<p><i>C++ only.</i></p><p>constexpr (C++11) goes further than const: its value must be computable at compile time, which lets it be used anywhere a true compile-time constant is required - array bounds, template arguments, case labels - and lets the compiler evaluate it during compilation instead of at run time.</p><pre style="white-space:pre;">constexpr int bufferSize = 256;
char buffer[bufferSize];   // legal: a real compile-time constant</pre><p><b>See also:</b> <a href="cppref:var_const">const Variables</a>, <a href="cppref:kw_constexpr">constexpr</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++.</i></p><p>constexpr (C++11) geht weiter als const: Der Wert muss zur Compile-Zeit berechenbar sein, wodurch er überall dort verwendet werden kann, wo eine echte Compile-Zeit-Konstante nötig ist - Array-Größen, Template-Argumente, case-Marken - und der Compiler ihn während der Kompilierung statt zur Laufzeit auswerten kann.</p><pre style="white-space:pre;">constexpr int bufferSize = 256;
char buffer[bufferSize];   // legal: a real compile-time constant</pre><p><b>Siehe auch:</b> <a href="cppref:var_const">const Variables</a>, <a href="cppref:kw_constexpr">constexpr</a></p>)HTML"));

    addEntry(QStringLiteral("var_pointer_basics"), QStringLiteral("Variables"), QStringLiteral("Pointer Variables"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>A pointer variable holds a memory ADDRESS rather than a value directly. It is declared with *, and the address of an existing variable is obtained with &. Pointers are central to how C passes data "by reference" and to nearly the whole Amiga API, whose functions constantly hand out and expect addresses (APTR and friends).</p><pre style="white-space:pre;">int value = 42;
int *ptr = &amp;value;    /* ptr now holds the address of value */
printf("%d\n", *ptr); /* dereference: read the pointed-to value */
*ptr = 100;             /* dereference: write through the pointer */</pre><p><b>See also:</b> <a href="cppref:var_pointer_to_pointer">Pointer to Pointer</a>, <a href="cppref:var_null_pointer">NULL / nullptr</a>, <a href="cppref:op_address_dereference">Address-of (&) and Dereference (*) as Operators</a>, <a href="cppref:dt_amiga_aptr">APTR</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Eine Zeigervariable (Pointer) enthält eine Speicheradresse statt direkt einen Wert. Sie wird mit * deklariert, und die Adresse einer bestehenden Variablen erhält man mit &. Zeiger sind zentral dafür, wie C Daten "per Referenz" übergibt, und für praktisch die gesamte Amiga-API, deren Funktionen ständig Adressen entgegennehmen und zurückgeben (APTR und Verwandte).</p><pre style="white-space:pre;">int value = 42;
int *ptr = &amp;value;    /* ptr now holds the address of value */
printf("%d\n", *ptr); /* dereference: read the pointed-to value */
*ptr = 100;             /* dereference: write through the pointer */</pre><p><b>Siehe auch:</b> <a href="cppref:var_pointer_to_pointer">Pointer to Pointer</a>, <a href="cppref:var_null_pointer">NULL / nullptr</a>, <a href="cppref:op_address_dereference">Address-of (&) and Dereference (*) as Operators</a>, <a href="cppref:dt_amiga_aptr">APTR</a></p>)HTML"));

    addEntry(QStringLiteral("var_pointer_to_pointer"), QStringLiteral("Variables"), QStringLiteral("Pointer to Pointer"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>A pointer can itself point to another pointer, written with **. Most commonly used to let a function change what a CALLER's own pointer points to - for example allocating a buffer and handing it back through an "out" parameter.</p><pre style="white-space:pre;">void allocateBuffer(char **outBuffer, size_t size)
{
    *outBuffer = malloc(size);
}

char *buf = NULL;
allocateBuffer(&amp;buf, 256);</pre><p><b>See also:</b> <a href="cppref:var_pointer_basics">Pointer Variables</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Ein Zeiger kann selbst auf einen anderen Zeiger zeigen, geschrieben mit **. Meist genutzt, damit eine Funktion ändern kann, worauf der eigene Zeiger des AUFRUFERS zeigt - z. B. einen Puffer allozieren und über einen "Out"-Parameter zurückgeben.</p><pre style="white-space:pre;">void allocateBuffer(char **outBuffer, size_t size)
{
    *outBuffer = malloc(size);
}

char *buf = NULL;
allocateBuffer(&amp;buf, 256);</pre><p><b>Siehe auch:</b> <a href="cppref:var_pointer_basics">Pointer Variables</a></p>)HTML"));

    addEntry(QStringLiteral("var_null_pointer"), QStringLiteral("Variables"), QStringLiteral("NULL / nullptr"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>A pointer that points to "nothing" should be set to NULL - in C, defined as ((void*)0) or similar. C++11 adds nullptr, a real, type-safe null-pointer constant that (unlike NULL) can never be mistaken for the integer 0 during overload resolution. Always check a pointer against NULL/nullptr before dereferencing it if it might not have been assigned.</p><pre style="white-space:pre;">char *name = NULL;
if (name != NULL)
    printf("%s\n", name);</pre><p><b>Note:</b> In new C++ code, prefer nullptr over NULL or a bare 0.</p><p><b>See also:</b> <a href="cppref:var_pointer_basics">Pointer Variables</a>, <a href="cppref:var_dangling_pointer">Dangling & Uninitialized Pointers</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Ein Zeiger, der auf "nichts" zeigt, sollte auf NULL gesetzt werden - in C definiert als ((void*)0) oder ähnlich. C++11 ergänzt nullptr, eine echte, typsichere Nullzeiger-Konstante, die (anders als NULL) bei der Überladungsauflösung nie mit der Ganzzahl 0 verwechselt werden kann. Einen Zeiger, der eventuell nicht zugewiesen wurde, vor dem Dereferenzieren stets gegen NULL/nullptr prüfen.</p><pre style="white-space:pre;">char *name = NULL;
if (name != NULL)
    printf("%s\n", name);</pre><p><b>Hinweis:</b> In neuem C++-Code nullptr gegenüber NULL oder einer nackten 0 bevorzugen.</p><p><b>Siehe auch:</b> <a href="cppref:var_pointer_basics">Pointer Variables</a>, <a href="cppref:var_dangling_pointer">Dangling & Uninitialized Pointers</a></p>)HTML"));

    addEntry(QStringLiteral("var_reference"), QStringLiteral("Variables"), QStringLiteral("References (C++)"),
              QStringLiteral(R"HTML(<p><i>C++ only.</i></p><p>A reference is an alias for an existing variable. Once bound at initialization it can never be made to refer to something else, and - unlike a pointer - it never needs its own dereference syntax; it is used exactly like the variable it refers to. Most commonly seen as a pass-by-reference function parameter.</p><pre style="white-space:pre;">int value = 42;
int &amp;ref = value;   // ref IS value, not a pointer to it
ref = 100;           // changes value directly</pre><p><b>See also:</b> <a href="cppref:var_pointer_basics">Pointer Variables</a>, <a href="cppref:fn_parameters_by_reference">Parameters Passed by Reference (C++)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++.</i></p><p>Eine Referenz ist ein Alias für eine bestehende Variable. Einmal bei der Initialisierung gebunden, kann sie nie mehr auf etwas anderes verweisen, und - anders als ein Zeiger - braucht sie keine eigene Dereferenzierungs-Syntax; sie wird genau wie die Variable verwendet, auf die sie sich bezieht. Am häufigsten als Referenzparameter einer Funktion zu sehen.</p><pre style="white-space:pre;">int value = 42;
int &amp;ref = value;   // ref IS value, not a pointer to it
ref = 100;           // changes value directly</pre><p><b>Siehe auch:</b> <a href="cppref:var_pointer_basics">Pointer Variables</a>, <a href="cppref:fn_parameters_by_reference">Parameters Passed by Reference (C++)</a></p>)HTML"));

    addEntry(QStringLiteral("var_array"), QStringLiteral("Variables"), QStringLiteral("Arrays"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>An array is a fixed-size, contiguous sequence of variables of the same type, indexed starting at 0. In most expressions, the array's name decays to a pointer to its first element - the reason arrays and pointers are so closely related in C.</p><pre style="white-space:pre;">int scores[5] = { 10, 20, 30, 40, 50 };
for (int i = 0; i &lt; 5; i++)
    printf("%d\n", scores[i]);</pre><p><b>See also:</b> <a href="cppref:var_pointer_basics">Pointer Variables</a>, <a href="cppref:var_pointer_arithmetic">Pointer Arithmetic</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Ein Array ist eine feste, zusammenhängende Folge von Variablen desselben Typs, indiziert beginnend bei 0. In den meisten Ausdrücken zerfällt der Name des Arrays zu einem Zeiger auf sein erstes Element - der Grund, warum Arrays und Zeiger in C so eng verwandt sind.</p><pre style="white-space:pre;">int scores[5] = { 10, 20, 30, 40, 50 };
for (int i = 0; i &lt; 5; i++)
    printf("%d\n", scores[i]);</pre><p><b>Siehe auch:</b> <a href="cppref:var_pointer_basics">Pointer Variables</a>, <a href="cppref:var_pointer_arithmetic">Pointer Arithmetic</a></p>)HTML"));

    addEntry(QStringLiteral("var_pointer_arithmetic"), QStringLiteral("Variables"), QStringLiteral("Pointer Arithmetic"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>Adding or subtracting an integer to/from a pointer moves it by that many ELEMENTS of its pointed-to type, not bytes - the compiler automatically scales the offset by sizeof(*ptr). This is exactly what makes ptr + i and array indexing arr[i] equivalent.</p><pre style="white-space:pre;">int values[3] = { 10, 20, 30 };
int *p = values;
printf("%d\n", *(p + 1));  /* same as values[1] */</pre><p><b>See also:</b> <a href="cppref:var_array">Arrays</a>, <a href="cppref:var_pointer_basics">Pointer Variables</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Wird eine Ganzzahl zu einem Zeiger addiert oder von ihm subtrahiert, bewegt er sich um so viele ELEMENTE des Zieltyps, nicht Bytes - der Compiler skaliert den Offset automatisch mit sizeof(*ptr). Genau das macht ptr + i und die Array-Indizierung arr[i] gleichwertig.</p><pre style="white-space:pre;">int values[3] = { 10, 20, 30 };
int *p = values;
printf("%d\n", *(p + 1));  /* same as values[1] */</pre><p><b>Siehe auch:</b> <a href="cppref:var_array">Arrays</a>, <a href="cppref:var_pointer_basics">Pointer Variables</a></p>)HTML"));

    addEntry(QStringLiteral("var_dangling_pointer"), QStringLiteral("Variables"), QStringLiteral("Dangling & Uninitialized Pointers"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>A dangling pointer still holds the address of memory that has since been freed; an uninitialized ("wild") pointer was never given a value at all. Dereferencing either is undefined behavior - a classic source of crashes that are hard to reproduce. Setting a pointer to NULL once you are done with what it pointed to turns the mistake into a clean, checkable NULL instead of a silent trap.</p><pre style="white-space:pre;">char *buf = malloc(64);
free(buf);
buf = NULL;   /* now safe to check with "if (buf)" later */</pre><p><b>Note:</b> On classic AmigaOS systems without MMU-based memory protection, dereferencing a stale pointer is especially unforgiving: it can silently corrupt unrelated memory instead of crashing cleanly and immediately.</p><p><b>See also:</b> <a href="cppref:var_null_pointer">NULL / nullptr</a>, <a href="cppref:var_pointer_basics">Pointer Variables</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Ein Dangling-Zeiger enthält noch die Adresse von Speicher, der inzwischen freigegeben wurde; ein uninitialisierter ("wilder") Zeiger hat nie einen Wert erhalten. Beides zu dereferenzieren ist undefiniertes Verhalten - eine klassische Quelle schwer reproduzierbarer Abstürze. Einen Zeiger nach Gebrauch auf NULL zu setzen macht aus dem Fehler ein sauber prüfbares NULL statt einer stillen Falle.</p><pre style="white-space:pre;">char *buf = malloc(64);
free(buf);
buf = NULL;   /* now safe to check with "if (buf)" later */</pre><p><b>Hinweis:</b> Auf klassischen AmigaOS-Systemen ohne MMU-basierten Speicherschutz ist das Dereferenzieren eines veralteten Zeigers besonders tückisch: Es kann still fremden Speicher beschädigen, statt sauber und sofort abzustürzen.</p><p><b>Siehe auch:</b> <a href="cppref:var_null_pointer">NULL / nullptr</a>, <a href="cppref:var_pointer_basics">Pointer Variables</a></p>)HTML"));

}

void CppRefReader::populateDecisions()
{
    addEntry(QStringLiteral("dec_if_else"), QStringLiteral("Decisions"), QStringLiteral("if / if-else"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>The basic conditional branch: run a block only if a condition is true, optionally with an else branch for when it is false.</p><pre style="white-space:pre;">if (temperature &gt; 30)
{
    printf("hot\n");
}
else
{
    printf("not hot\n");
}</pre><p><b>See also:</b> <a href="cppref:kw_if">if</a>, <a href="cppref:kw_else">else</a>, <a href="cppref:dec_else_if_chain">else if Chains</a>, <a href="cppref:op_ternary">Ternary Conditional Operator (?:)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Der grundlegende bedingte Zweig: Ein Block wird nur ausgeführt, wenn eine Bedingung wahr ist, optional mit einem else-Zweig für den Fall, dass sie falsch ist.</p><pre style="white-space:pre;">if (temperature &gt; 30)
{
    printf("hot\n");
}
else
{
    printf("not hot\n");
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_if">if</a>, <a href="cppref:kw_else">else</a>, <a href="cppref:dec_else_if_chain">else if Chains</a>, <a href="cppref:op_ternary">Ternary Conditional Operator (?:)</a></p>)HTML"));

    addEntry(QStringLiteral("dec_else_if_chain"), QStringLiteral("Decisions"), QStringLiteral("else if Chains"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>Chaining else if lets you test a series of conditions in order, taking the first branch whose condition is true and skipping the rest. There is no dedicated "elif" keyword in C/C++ - it is simply an if used as the body of an else.</p><pre style="white-space:pre;">if (score &gt;= 90)
    grade = 'A';
else if (score &gt;= 80)
    grade = 'B';
else if (score &gt;= 70)
    grade = 'C';
else
    grade = 'F';</pre><p><b>See also:</b> <a href="cppref:dec_if_else">if / if-else</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Verkettetes else if prüft eine Reihe von Bedingungen der Reihe nach und nimmt den ersten Zweig, dessen Bedingung wahr ist; die übrigen werden übersprungen. Es gibt kein eigenes "elif"-Schlüsselwort in C/C++ - es ist schlicht ein if als Rumpf eines else.</p><pre style="white-space:pre;">if (score &gt;= 90)
    grade = 'A';
else if (score &gt;= 80)
    grade = 'B';
else if (score &gt;= 70)
    grade = 'C';
else
    grade = 'F';</pre><p><b>Siehe auch:</b> <a href="cppref:dec_if_else">if / if-else</a></p>)HTML"));

    addEntry(QStringLiteral("dec_nested_if"), QStringLiteral("Decisions"), QStringLiteral("Nested if Statements"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>An if (or if-else) can itself contain another if as its body, letting you test a second condition only once the first is already known to be true. Keep nesting shallow - more than two or three levels usually signals the logic should become a function, or be combined with &&.</p><pre style="white-space:pre;">if (fileIsOpen)
{
    if (bytesRemaining &gt; 0)
    {
        readNextChunk();
    }
}</pre><p><b>See also:</b> <a href="cppref:dec_dangling_else">The Dangling-else Ambiguity</a>, <a href="cppref:op_logical">Logical Operators (&& || !)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Ein if (oder if-else) kann selbst ein weiteres if als Rumpf enthalten, sodass eine zweite Bedingung erst geprüft wird, wenn die erste bereits als wahr feststeht. Verschachtelung flach halten - mehr als zwei oder drei Ebenen sind meist ein Zeichen, dass die Logik eine Funktion werden oder mit && kombiniert werden sollte.</p><pre style="white-space:pre;">if (fileIsOpen)
{
    if (bytesRemaining &gt; 0)
    {
        readNextChunk();
    }
}</pre><p><b>Siehe auch:</b> <a href="cppref:dec_dangling_else">The Dangling-else Ambiguity</a>, <a href="cppref:op_logical">Logical Operators (&& || !)</a></p>)HTML"));

    addEntry(QStringLiteral("dec_dangling_else"), QStringLiteral("Decisions"), QStringLiteral("The Dangling-else Ambiguity"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>When an if without braces is immediately followed by another if, and then a single else, the else always binds to the NEAREST unmatched if - not necessarily the one the indentation suggests. Braces around both branches remove the ambiguity entirely, for the compiler and for human readers alike.</p><pre style="white-space:pre;">if (a)
    if (b)
        doSomething();
else                 /* binds to "if (b)", NOT "if (a)" - despite the indentation! */
    doSomethingElse();</pre><p><b>Note:</b> Braced, the same code is unambiguous: if (a) { if (b) doSomething(); } else { doSomethingElse(); }</p><p><b>See also:</b> <a href="cppref:dec_nested_if">Nested if Statements</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Wenn auf ein if ohne geschweifte Klammern sofort ein weiteres if folgt und dann ein einzelnes else, bindet das else immer an das NÄCHSTGELEGENE unverbundene if - nicht zwingend an das, was die Einrückung nahelegt. Klammern um beide Zweige beseitigen die Mehrdeutigkeit vollständig, für den Compiler wie für menschliche Leser.</p><pre style="white-space:pre;">if (a)
    if (b)
        doSomething();
else                 /* binds to "if (b)", NOT "if (a)" - despite the indentation! */
    doSomethingElse();</pre><p><b>Hinweis:</b> Mit Klammern ist derselbe Code eindeutig: if (a) { if (b) doSomething(); } else { doSomethingElse(); }</p><p><b>Siehe auch:</b> <a href="cppref:dec_nested_if">Nested if Statements</a></p>)HTML"));

    addEntry(QStringLiteral("dec_truthiness"), QStringLiteral("Decisions"), QStringLiteral("Truthiness of Conditions in C"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>In C, any scalar expression can be used as a condition, not just something literally boolean: 0 (or a null pointer) counts as false, and any nonzero value counts as true. C99 and later have a real bool/_Bool type, but if/while/for still fall back to this same "0 is false, everything else is true" rule for any expression used as a condition.</p><pre style="white-space:pre;">int x = 5;
if (x)              /* true - x is nonzero */
    printf("nonzero\n");

char *name = getName();
if (name)           /* true if name is not NULL */
    printf("%s\n", name);</pre><p><b>See also:</b> <a href="cppref:kw_bool">bool</a>, <a href="cppref:dt_bool">bool / _Bool</a>, <a href="cppref:var_null_pointer">NULL / nullptr</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>In C kann jeder skalare Ausdruck als Bedingung verwendet werden, nicht nur etwas wörtlich Boolesches: 0 (oder ein Nullzeiger) zählt als falsch, jeder von Null verschiedene Wert als wahr. C99 und neuer haben einen echten bool/_Bool-Typ, aber if/while/for folgen für jeden als Bedingung verwendeten Ausdruck weiterhin dieser Regel "0 ist falsch, alles andere ist wahr".</p><pre style="white-space:pre;">int x = 5;
if (x)              /* true - x is nonzero */
    printf("nonzero\n");

char *name = getName();
if (name)           /* true if name is not NULL */
    printf("%s\n", name);</pre><p><b>Siehe auch:</b> <a href="cppref:kw_bool">bool</a>, <a href="cppref:dt_bool">bool / _Bool</a>, <a href="cppref:var_null_pointer">NULL / nullptr</a></p>)HTML"));

    addEntry(QStringLiteral("dec_comparison_pitfall"), QStringLiteral("Decisions"), QStringLiteral("Common Pitfall: = vs =="),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>A classic C/C++ typo: writing if (x = 5) (assignment) when if (x == 5) (comparison) was meant. Because assignment is itself an expression whose value is the assigned value, the code still compiles - and silently always takes the "true" branch unless the assigned value happens to be 0. Many developers write the constant first, if (5 == x), specifically so a dropped = becomes a compile error instead of a silent bug.</p><pre style="white-space:pre;">int x = 0;
if (x = 5)      /* BUG: assigns 5 to x, then tests 5 (always true) */
    printf("oops\n");
if (x == 5)     /* correct comparison */
    printf("x is 5\n");</pre><p><b>See also:</b> <a href="cppref:op_relational">Relational Operators (== != < > <= >=)</a>, <a href="cppref:op_assignment">Assignment & Compound Assignment</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Ein klassischer C/C++-Tippfehler: if (x = 5) (Zuweisung) schreiben, wenn if (x == 5) (Vergleich) gemeint war. Da eine Zuweisung selbst ein Ausdruck ist, dessen Wert der zugewiesene Wert ist, kompiliert der Code trotzdem - und nimmt still immer den "wahr"-Zweig, außer der zugewiesene Wert ist zufällig 0. Manche Entwickler schreiben deshalb bewusst die Konstante zuerst, if (5 == x), damit ein vergessenes = zu einem Compilerfehler statt einem stillen Bug wird.</p><pre style="white-space:pre;">int x = 0;
if (x = 5)      /* BUG: assigns 5 to x, then tests 5 (always true) */
    printf("oops\n");
if (x == 5)     /* correct comparison */
    printf("x is 5\n");</pre><p><b>Siehe auch:</b> <a href="cppref:op_relational">Relational Operators (== != < > <= >=)</a>, <a href="cppref:op_assignment">Assignment & Compound Assignment</a></p>)HTML"));

    addEntry(QStringLiteral("dec_short_circuit"), QStringLiteral("Decisions"), QStringLiteral("Short-Circuit Evaluation in Conditions"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>&& and || only evaluate their right-hand operand when the left one has not already decided the result: && stops at the first false, || stops at the first true. This is routinely relied on to guard an unsafe operation - for example, checking a pointer is not NULL before dereferencing it, in the very same expression.</p><pre style="white-space:pre;">if (ptr != NULL &amp;&amp; ptr-&gt;value &gt; 0)   /* ptr-&gt;value is only evaluated if ptr isn't NULL */
{
    printf("positive\n");
}</pre><p><b>See also:</b> <a href="cppref:op_logical">Logical Operators (&& || !)</a>, <a href="cppref:var_null_pointer">NULL / nullptr</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>&& und || werten ihren rechten Operanden nur aus, wenn der linke das Ergebnis noch nicht entschieden hat: && stoppt beim ersten falschen Wert, || beim ersten wahren. Darauf verlässt man sich regelmäßig, um eine unsichere Operation abzusichern - z. B. im selben Ausdruck zu prüfen, dass ein Zeiger nicht NULL ist, bevor er dereferenziert wird.</p><pre style="white-space:pre;">if (ptr != NULL &amp;&amp; ptr-&gt;value &gt; 0)   /* ptr-&gt;value is only evaluated if ptr isn't NULL */
{
    printf("positive\n");
}</pre><p><b>Siehe auch:</b> <a href="cppref:op_logical">Logical Operators (&& || !)</a>, <a href="cppref:var_null_pointer">NULL / nullptr</a></p>)HTML"));

}

void CppRefReader::populateControlStructures()
{
    addEntry(QStringLiteral("cs_switch_case"), QStringLiteral("Control Structures"), QStringLiteral("switch / case"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>switch dispatches to one of several labeled branches by comparing a single integer/enum expression against constant case labels. Without a break, execution "falls through" into the next case - a frequent bug when unintentional, but occasionally used deliberately to group several labels together.</p><pre style="white-space:pre;">switch (direction)
{
    case NORTH:
    case SOUTH:
        printf("vertical\n");
        break;
    case EAST:
    case WEST:
        printf("horizontal\n");
        break;
    default:
        printf("unknown\n");
        break;
}</pre><p><b>See also:</b> <a href="cppref:kw_switch">switch</a>, <a href="cppref:kw_case">case</a>, <a href="cppref:kw_default">default</a>, <a href="cppref:kw_break">break</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>switch springt zu einem von mehreren markierten Zweigen, indem ein einzelner Ganzzahl-/Enum-Ausdruck mit konstanten case-Marken verglichen wird. Ohne break "fällt" die Ausführung in den nächsten case durch ("Fall-Through") - ein häufiger Fehler, wenn ungewollt, aber gelegentlich bewusst genutzt, um mehrere Marken zu gruppieren.</p><pre style="white-space:pre;">switch (direction)
{
    case NORTH:
    case SOUTH:
        printf("vertical\n");
        break;
    case EAST:
    case WEST:
        printf("horizontal\n");
        break;
    default:
        printf("unknown\n");
        break;
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_switch">switch</a>, <a href="cppref:kw_case">case</a>, <a href="cppref:kw_default">default</a>, <a href="cppref:kw_break">break</a></p>)HTML"));

    addEntry(QStringLiteral("cs_for_loop"), QStringLiteral("Control Structures"), QStringLiteral("for Loops"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>A counting loop with initializer, condition and increment written on one line, evaluated in that order once per iteration. Any of the three parts may be omitted (for (;;) loops forever), and the comma operator lets more than one variable be initialized or updated per part.</p><pre style="white-space:pre;">for (int i = 0, j = 10; i &lt; j; i++, j--)
    printf("%d %d\n", i, j);</pre><p><b>See also:</b> <a href="cppref:kw_for">for</a>, <a href="cppref:cs_choosing_a_loop">Choosing Between for / while / do...while</a>, <a href="cppref:cs_range_based_for">Range-based for (C++11)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Eine Zählschleife mit Initialisierung, Bedingung und Inkrement in einer Zeile, ausgewertet pro Durchlauf in genau dieser Reihenfolge. Jeder der drei Teile darf weggelassen werden (for (;;) läuft endlos), und der Komma-Operator erlaubt es, mehr als eine Variable pro Teil zu initialisieren oder zu aktualisieren.</p><pre style="white-space:pre;">for (int i = 0, j = 10; i &lt; j; i++, j--)
    printf("%d %d\n", i, j);</pre><p><b>Siehe auch:</b> <a href="cppref:kw_for">for</a>, <a href="cppref:cs_choosing_a_loop">Choosing Between for / while / do...while</a>, <a href="cppref:cs_range_based_for">Range-based for (C++11)</a></p>)HTML"));

    addEntry(QStringLiteral("cs_choosing_a_loop"), QStringLiteral("Control Structures"), QStringLiteral("Choosing Between for / while / do...while"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>All three loop keywords repeat a block; they differ in when to reach for each. Use for when the number of iterations (or a counting variable) is known up front. Use while when repetition is condition-driven and might legitimately run zero times. Use do...while when the body must run at least once before the condition is even checked - e.g. redisplaying a menu until the user picks a valid option.</p><p><b>See also:</b> <a href="cppref:kw_for">for</a>, <a href="cppref:kw_while">while</a>, <a href="cppref:kw_do">do...while</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Alle drei Schleifen-Schlüsselwörter wiederholen einen Block; sie unterscheiden sich darin, wann welche passt. for, wenn die Anzahl der Durchläufe (oder eine Zählvariable) von vornherein feststeht. while, wenn die Wiederholung bedingungsgesteuert ist und legitim auch null Mal laufen darf. do...while, wenn der Rumpf mindestens einmal laufen muss, bevor die Bedingung überhaupt geprüft wird - z. B. ein Menü erneut anzeigen, bis der Benutzer eine gültige Option wählt.</p><p><b>Siehe auch:</b> <a href="cppref:kw_for">for</a>, <a href="cppref:kw_while">while</a>, <a href="cppref:kw_do">do...while</a></p>)HTML"));

    addEntry(QStringLiteral("cs_break_continue"), QStringLiteral("Control Structures"), QStringLiteral("break & continue Across Nested Constructs"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>break exits only the innermost enclosing loop or switch - a break written inside a switch that itself sits inside a loop only leaves the switch, not the loop, a frequent surprise. continue only applies to loops, never to a switch. There is no direct way to break out of several nested loops at once in C/C++; the usual workarounds are a bool flag checked by the outer loop, a goto to a label placed after the loops, or restructuring the loops into their own function and using return.</p><pre style="white-space:pre;">for (int i = 0; i &lt; 3; i++)
{
    switch (i)
    {
        case 1:
            break;      /* only exits the switch, the for loop continues! */
    }
    printf("i = %d\n", i);
}</pre><p><b>See also:</b> <a href="cppref:kw_break">break</a>, <a href="cppref:kw_continue">continue</a>, <a href="cppref:cs_goto_cleanup">The goto Error-Cleanup Idiom</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>break verlässt nur die innerste umschließende Schleife oder switch-Anweisung - ein break innerhalb eines switch, das selbst in einer Schleife steckt, verlässt nur das switch, nicht die Schleife, eine häufige Überraschung. continue gilt nur für Schleifen, niemals für switch. Es gibt in C/C++ keinen direkten Weg, mehrere verschachtelte Schleifen auf einmal zu verlassen; die üblichen Auswege sind eine bool-Flagge, die die äußere Schleife prüft, ein goto zu einer Marke nach den Schleifen, oder die Schleifen in eine eigene Funktion auszulagern und return zu verwenden.</p><pre style="white-space:pre;">for (int i = 0; i &lt; 3; i++)
{
    switch (i)
    {
        case 1:
            break;      /* only exits the switch, the for loop continues! */
    }
    printf("i = %d\n", i);
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_break">break</a>, <a href="cppref:kw_continue">continue</a>, <a href="cppref:cs_goto_cleanup">The goto Error-Cleanup Idiom</a></p>)HTML"));

    addEntry(QStringLiteral("cs_nested_loops"), QStringLiteral("Control Structures"), QStringLiteral("Nested Loops"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>A loop inside another loop: the inner loop runs to completion for each single iteration of the outer one. The classic use is iterating a 2D grid or array. The total iteration count multiplies (outer times inner), so watch performance for large sizes.</p><pre style="white-space:pre;">for (int row = 0; row &lt; 3; row++)
{
    for (int col = 0; col &lt; 3; col++)
        printf("%d,%d ", row, col);
    printf("\n");
}</pre><p><b>See also:</b> <a href="cppref:cs_for_loop">for Loops</a>, <a href="cppref:cs_break_continue">break & continue Across Nested Constructs</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Eine Schleife innerhalb einer anderen: Die innere Schleife läuft für jeden einzelnen Durchlauf der äußeren komplett durch. Der klassische Einsatz ist das Durchlaufen eines 2D-Rasters oder -Arrays. Die Gesamtzahl der Durchläufe multipliziert sich (außen mal innen) - bei großen Größen auf die Performance achten.</p><pre style="white-space:pre;">for (int row = 0; row &lt; 3; row++)
{
    for (int col = 0; col &lt; 3; col++)
        printf("%d,%d ", row, col);
    printf("\n");
}</pre><p><b>Siehe auch:</b> <a href="cppref:cs_for_loop">for Loops</a>, <a href="cppref:cs_break_continue">break & continue Across Nested Constructs</a></p>)HTML"));

    addEntry(QStringLiteral("cs_range_based_for"), QStringLiteral("Control Structures"), QStringLiteral("Range-based for (C++11)"),
              QStringLiteral(R"HTML(<p><i>C++ only.</i></p><p>Iterates directly over the elements of an array or standard-library container, with no index variable to manage at all - each pass binds the loop variable to the next element in turn.</p><pre style="white-space:pre;">int values[] = { 10, 20, 30 };
for (int v : values)
    std::cout &lt;&lt; v &lt;&lt; "\n";</pre><p><b>See also:</b> <a href="cppref:cs_for_loop">for Loops</a>, <a href="cppref:var_array">Arrays</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++.</i></p><p>Durchläuft direkt die Elemente eines Arrays oder Standardbibliothek-Containers, ganz ohne zu verwaltende Indexvariable - jeder Durchlauf bindet die Schleifenvariable an das jeweils nächste Element.</p><pre style="white-space:pre;">int values[] = { 10, 20, 30 };
for (int v : values)
    std::cout &lt;&lt; v &lt;&lt; "\n";</pre><p><b>Siehe auch:</b> <a href="cppref:cs_for_loop">for Loops</a>, <a href="cppref:var_array">Arrays</a></p>)HTML"));

    addEntry(QStringLiteral("cs_infinite_loop"), QStringLiteral("Control Structures"), QStringLiteral("Intentional Infinite Loops"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>for (;;) or while (1) (or while (true) in C++) run forever by design, relying on a break, return, or exit() somewhere inside the body to end them. This is the classic shape of an event loop - including Amiga message loops built around Exec's Wait(), dispatching on whichever signal bits come back.</p><pre style="white-space:pre;">for (;;)
{
    ULONG signals = Wait(portMask);
    if (signals &amp; SIGBREAKF_CTRL_C)
        break;
    /* handle other signals ... */
}</pre><p><b>See also:</b> <a href="cppref:kw_for">for</a>, <a href="cppref:kw_while">while</a>, <a href="cppref:cs_break_continue">break & continue Across Nested Constructs</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>for (;;) oder while (1) (bzw. while (true) in C++) laufen absichtlich endlos, verlassen auf ein break, return oder exit() irgendwo im Rumpf, um zu enden. Das ist die klassische Form einer Ereignisschleife - einschließlich Amiga-Message-Loops rund um Exec's Wait(), die auf die jeweils zurückgegebenen Signalbits reagieren.</p><pre style="white-space:pre;">for (;;)
{
    ULONG signals = Wait(portMask);
    if (signals &amp; SIGBREAKF_CTRL_C)
        break;
    /* handle other signals ... */
}</pre><p><b>Siehe auch:</b> <a href="cppref:kw_for">for</a>, <a href="cppref:kw_while">while</a>, <a href="cppref:cs_break_continue">break & continue Across Nested Constructs</a></p>)HTML"));

    addEntry(QStringLiteral("cs_goto_cleanup"), QStringLiteral("Control Structures"), QStringLiteral("The goto Error-Cleanup Idiom"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>The one place goto is still considered idiomatic in modern C: jumping forward to a single cleanup label near a function's end when an error occurs partway through a sequence of resource acquisitions, avoiding deeply nested if/else or duplicated cleanup code at every exit point. It is jumping BACKWARD, or into the middle of a block past a variable's initialization, that gives goto its bad reputation - this forward-only, single-target pattern does not share those problems.</p><pre style="white-space:pre;">int result = -1;
FILE *f = fopen("data.txt", "r");
if (!f)
    goto cleanup;
char *buf = malloc(1024);
if (!buf)
    goto cleanup;
/* ... use f and buf ... */
result = 0;
cleanup:
if (buf) free(buf);
if (f)   fclose(f);
return result;</pre><p><b>See also:</b> <a href="cppref:kw_goto">goto</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Der eine Ort, an dem goto in modernem C noch als guter Stil gilt: ein Vorwärtssprung zu einer einzigen Cleanup-Marke am Ende einer Funktion, wenn mitten in einer Folge von Ressourcen-Beschaffungen ein Fehler auftritt - so werden tief verschachteltes if/else oder doppelter Cleanup-Code an jedem Ausstiegspunkt vermieden. Erst ein Sprung RÜCKWÄRTS, oder mitten in einen Block hinter die Initialisierung einer Variablen, verschafft goto seinen schlechten Ruf - dieses reine Vorwärts-, Einzelziel-Muster hat diese Probleme nicht.</p><pre style="white-space:pre;">int result = -1;
FILE *f = fopen("data.txt", "r");
if (!f)
    goto cleanup;
char *buf = malloc(1024);
if (!buf)
    goto cleanup;
/* ... use f and buf ... */
result = 0;
cleanup:
if (buf) free(buf);
if (f)   fclose(f);
return result;</pre><p><b>Siehe auch:</b> <a href="cppref:kw_goto">goto</a></p>)HTML"));

}

void CppRefReader::populateOperators()
{
    addEntry(QStringLiteral("op_arithmetic"), QStringLiteral("Operators"), QStringLiteral("Arithmetic Operators (+ - * / %)"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>Standard arithmetic. Integer division truncates toward zero; % is the remainder (its sign follows the dividend in C99+/C++11+). Division by zero is undefined behavior for integers, but produces inf or nan for floating-point types.</p><pre style="white-space:pre;">int a = 7, b = 2;
printf("%d %d\n", a / b, a % b);   /* 3 1 */</pre><p><b>See also:</b> <a href="cppref:op_precedence">Operator Precedence & Associativity</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Übliche Arithmetik. Ganzzahldivision rundet in Richtung Null; % ist der Rest (sein Vorzeichen folgt in C99+/C++11+ dem des Dividenden). Division durch Null ist bei Ganzzahlen undefiniertes Verhalten, bei Gleitkommazahlen entsteht inf oder nan.</p><pre style="white-space:pre;">int a = 7, b = 2;
printf("%d %d\n", a / b, a % b);   /* 3 1 */</pre><p><b>Siehe auch:</b> <a href="cppref:op_precedence">Operator Precedence & Associativity</a></p>)HTML"));

    addEntry(QStringLiteral("op_relational"), QStringLiteral("Operators"), QStringLiteral("Relational Operators (== != < > <= >=)"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>Compare two values, producing 0 or 1 in C (or a real bool in C++). See the "= vs ==" pitfall - a very easy typo to make when what is meant is a comparison, not an assignment.</p><pre style="white-space:pre;">if (score == 100)
    printf("perfect\n");</pre><p><b>See also:</b> <a href="cppref:dec_comparison_pitfall">Common Pitfall: = vs ==</a>, <a href="cppref:op_logical">Logical Operators (&& || !)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Vergleichen zwei Werte und liefern in C 0 oder 1 (in C++ ein echtes bool). Siehe die Falle "= statt ==" - ein sehr leicht zu machender Tippfehler, wenn ein Vergleich statt einer Zuweisung gemeint ist.</p><pre style="white-space:pre;">if (score == 100)
    printf("perfect\n");</pre><p><b>Siehe auch:</b> <a href="cppref:dec_comparison_pitfall">Common Pitfall: = vs ==</a>, <a href="cppref:op_logical">Logical Operators (&& || !)</a></p>)HTML"));

    addEntry(QStringLiteral("op_logical"), QStringLiteral("Operators"), QStringLiteral("Logical Operators (&& || !)"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>Combine or negate boolean conditions: && (and), || (or), ! (not). && and || short-circuit - see the Decisions category for how that is routinely relied on to guard an unsafe operation.</p><pre style="white-space:pre;">if (isReady &amp;&amp; !isPaused)
    runNextStep();</pre><p><b>See also:</b> <a href="cppref:dec_short_circuit">Short-Circuit Evaluation in Conditions</a>, <a href="cppref:op_relational">Relational Operators (== != < > <= >=)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Kombinieren oder negieren boolesche Bedingungen: && (und), || (oder), ! (nicht). && und || werten kurzschlussartig aus - siehe die Kategorie Decisions dafür, wie darauf regelmäßig gebaut wird, um eine unsichere Operation abzusichern.</p><pre style="white-space:pre;">if (isReady &amp;&amp; !isPaused)
    runNextStep();</pre><p><b>Siehe auch:</b> <a href="cppref:dec_short_circuit">Short-Circuit Evaluation in Conditions</a>, <a href="cppref:op_relational">Relational Operators (== != < > <= >=)</a></p>)HTML"));

    addEntry(QStringLiteral("op_bitwise"), QStringLiteral("Operators"), QStringLiteral("Bitwise Operators (& | ^ ~ << >>)"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>Operate on the individual bits of an integer's representation: & (and), | (or), ^ (xor), ~ (complement), << and >> (shift left/right). Essential for hardware registers, custom chip programming, and flag/bitmask fields - AmigaOS uses these constantly, e.g. ULONG flag words throughout Exec and Intuition. Right-shifting a negative signed value is implementation-defined in C (an arithmetic shift on virtually every real compiler, but not standard-guaranteed).</p><pre style="white-space:pre;">ULONG flags = FLAGB_A | FLAGB_B;   /* set two flag bits */
if (flags &amp; FLAGB_A)               /* test one flag */
    printf("A is set\n");
flags &amp;= ~FLAGB_B;                 /* clear one flag */</pre><p><b>See also:</b> <a href="cppref:dt_amiga_ulong">ULONG</a>, <a href="cppref:op_assignment">Assignment & Compound Assignment</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Arbeiten auf den einzelnen Bits der Darstellung einer Ganzzahl: & (und), | (oder), ^ (xor), ~ (Komplement), << und >> (Schieben links/rechts). Essenziell für Hardware-Register, Custom-Chip-Programmierung und Flag-/Bitmasken-Felder - AmigaOS nutzt das ständig, z. B. ULONG-Flagwörter in Exec und Intuition. Das Rechtsschieben eines negativen vorzeichenbehafteten Werts ist in C implementierungsdefiniert (auf praktisch jedem realen Compiler ein arithmetisches Schieben, aber vom Standard nicht garantiert).</p><pre style="white-space:pre;">ULONG flags = FLAGB_A | FLAGB_B;   /* set two flag bits */
if (flags &amp; FLAGB_A)               /* test one flag */
    printf("A is set\n");
flags &amp;= ~FLAGB_B;                 /* clear one flag */</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_ulong">ULONG</a>, <a href="cppref:op_assignment">Assignment & Compound Assignment</a></p>)HTML"));

    addEntry(QStringLiteral("op_assignment"), QStringLiteral("Operators"), QStringLiteral("Assignment & Compound Assignment"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>= assigns a value. The compound forms (+= -= *= /= %= &= |= ^= <<= >>=) combine an operation with assignment: a += b means a = a + b, but evaluates a only once - which matters when a itself is an expression with a side effect, such as an incrementing array index.</p><pre style="white-space:pre;">int total = 0;
total += 10;   /* same as: total = total + 10; */
total *= 2;</pre><p><b>See also:</b> <a href="cppref:op_arithmetic">Arithmetic Operators (+ - * / %)</a>, <a href="cppref:dec_comparison_pitfall">Common Pitfall: = vs ==</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>= weist einen Wert zu. Die zusammengesetzten Formen (+= -= *= /= %= &= |= ^= <<= >>=) verbinden eine Operation mit einer Zuweisung: a += b bedeutet a = a + b, wertet a dabei aber nur einmal aus - wichtig, wenn a selbst ein Ausdruck mit Seiteneffekt ist, etwa ein hochzählender Array-Index.</p><pre style="white-space:pre;">int total = 0;
total += 10;   /* same as: total = total + 10; */
total *= 2;</pre><p><b>Siehe auch:</b> <a href="cppref:op_arithmetic">Arithmetic Operators (+ - * / %)</a>, <a href="cppref:dec_comparison_pitfall">Common Pitfall: = vs ==</a></p>)HTML"));

    addEntry(QStringLiteral("op_increment_decrement"), QStringLiteral("Operators"), QStringLiteral("Increment & Decrement (++ --)"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>Add or subtract 1. Prefix (++x) changes the value first and evaluates to the NEW value; postfix (x++) evaluates to the OLD value and changes it afterward. This matters when the result is used in the same expression, e.g. array[i++] versus array[++i]. Relying on more than one increment/decrement of the same variable within a single expression without an intervening sequence point is undefined behavior.</p><pre style="white-space:pre;">int i = 5;
printf("%d\n", i++);  /* prints 5, i is now 6 */
printf("%d\n", ++i);  /* i becomes 7, prints 7 */</pre><p><b>See also:</b> <a href="cppref:op_arithmetic">Arithmetic Operators (+ - * / %)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Addieren oder subtrahieren 1. Präfix (++x) ändert den Wert zuerst und liefert den NEUEN Wert; Postfix (x++) liefert den ALTEN Wert und ändert danach. Das ist wichtig, wenn das Ergebnis im selben Ausdruck verwendet wird, z. B. array[i++] gegenüber array[++i]. Sich auf mehr als eine Inkrementierung/Dekrementierung derselben Variablen innerhalb eines einzigen Ausdrucks ohne dazwischenliegenden Sequenzpunkt zu verlassen, ist undefiniertes Verhalten.</p><pre style="white-space:pre;">int i = 5;
printf("%d\n", i++);  /* prints 5, i is now 6 */
printf("%d\n", ++i);  /* i becomes 7, prints 7 */</pre><p><b>Siehe auch:</b> <a href="cppref:op_arithmetic">Arithmetic Operators (+ - * / %)</a></p>)HTML"));

    addEntry(QStringLiteral("op_ternary"), QStringLiteral("Operators"), QStringLiteral("Ternary Conditional Operator (?:)"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>The only operator in C/C++ that takes three operands: condition ? valueIfTrue : valueIfFalse. It evaluates to one of the two values depending on the condition, only evaluating the branch actually taken - a compact if/else that produces a VALUE instead of running statements.</p><pre style="white-space:pre;">int a = 5, b = 10;
int max = (a &gt; b) ? a : b;</pre><p><b>See also:</b> <a href="cppref:dec_if_else">if / if-else</a>, <a href="cppref:op_relational">Relational Operators (== != < > <= >=)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Der einzige Operator in C/C++ mit drei Operanden: Bedingung ? WertWennWahr : WertWennFalsch. Er liefert je nach Bedingung einen der beiden Werte und wertet dabei nur den tatsächlich gewählten Zweig aus - ein kompaktes if/else, das einen WERT liefert statt Anweisungen auszuführen.</p><pre style="white-space:pre;">int a = 5, b = 10;
int max = (a &gt; b) ? a : b;</pre><p><b>Siehe auch:</b> <a href="cppref:dec_if_else">if / if-else</a>, <a href="cppref:op_relational">Relational Operators (== != < > <= >=)</a></p>)HTML"));

    addEntry(QStringLiteral("op_address_dereference"), QStringLiteral("Operators"), QStringLiteral("Address-of (&) and Dereference (*) as Operators"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>Distinct from their role in a pointer DECLARATION (int *p): here, & and * are operators applied to values. &x yields the address of x; *p reads (or, on the left of an assignment, writes) the value p points to. The same * symbol meaning "this is a pointer type" in a declaration and "dereference this pointer" in an expression is a common early point of confusion.</p><pre style="white-space:pre;">int value = 42;
int *p = &amp;value;    /* &amp; = "address of" */
int copy = *p;       /* * = "dereference" (read through pointer) */</pre><p><b>See also:</b> <a href="cppref:var_pointer_basics">Pointer Variables</a>, <a href="cppref:var_pointer_arithmetic">Pointer Arithmetic</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Zu unterscheiden von ihrer Rolle in einer Zeiger-DEKLARATION (int *p): Hier sind & und * Operatoren, die auf Werte angewendet werden. &x liefert die Adresse von x; *p liest (oder schreibt, auf der linken Seite einer Zuweisung) den Wert, auf den p zeigt. Dass dasselbe Zeichen * in einer Deklaration "dies ist ein Zeigertyp" und in einem Ausdruck "diesen Zeiger dereferenzieren" bedeutet, ist ein häufiger früher Verwirrungspunkt.</p><pre style="white-space:pre;">int value = 42;
int *p = &amp;value;    /* &amp; = "address of" */
int copy = *p;       /* * = "dereference" (read through pointer) */</pre><p><b>Siehe auch:</b> <a href="cppref:var_pointer_basics">Pointer Variables</a>, <a href="cppref:var_pointer_arithmetic">Pointer Arithmetic</a></p>)HTML"));

    addEntry(QStringLiteral("op_member_access"), QStringLiteral("Operators"), QStringLiteral("Member Access: . -> ::"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>. accesses a member of a struct/class value directly. -> does the same through a pointer (equivalent to, but less noisy than, (*ptr).member). :: (C++ only) is the scope resolution operator, naming something inside a namespace or class (std::cout), or, used unqualified, the global scope.</p><pre style="white-space:pre;">struct Point { int x, y; };
struct Point p = { 1, 2 };
struct Point *pp = &amp;p;
printf("%d\n", p.x);    /* direct member access */
printf("%d\n", pp-&gt;x);  /* through a pointer */</pre><p><b>Note:</b> C++ example: std::cout is the identifier cout inside the std namespace, accessed via ::.</p><p><b>See also:</b> <a href="cppref:op_address_dereference">Address-of (&) and Dereference (*) as Operators</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>. greift direkt auf ein Member eines struct-/class-Werts zu. -> tut dasselbe über einen Zeiger (gleichwertig zu, aber weniger umständlich als, (*ptr).member). :: (nur C++) ist der Bereichsauflösungsoperator, der etwas innerhalb eines Namensraums oder einer Klasse benennt (std::cout), oder, unqualifiziert verwendet, den globalen Bereich.</p><pre style="white-space:pre;">struct Point { int x, y; };
struct Point p = { 1, 2 };
struct Point *pp = &amp;p;
printf("%d\n", p.x);    /* direct member access */
printf("%d\n", pp-&gt;x);  /* through a pointer */</pre><p><b>Hinweis:</b> C++-Beispiel: std::cout ist der Bezeichner cout innerhalb des Namensraums std, erreicht über ::.</p><p><b>Siehe auch:</b> <a href="cppref:op_address_dereference">Address-of (&) and Dereference (*) as Operators</a></p>)HTML"));

    addEntry(QStringLiteral("op_precedence"), QStringLiteral("Operators"), QStringLiteral("Operator Precedence & Associativity"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>When an expression mixes several operators, precedence fixes the order they are evaluated in - for example * binds tighter than +, so a + b * c means a + (b * c). Associativity decides the order among operators of EQUAL precedence: most binary operators go left-to-right, while assignment and the ternary operator go right-to-left. When in doubt - or when writing for a reader who might be - add explicit parentheses; they cost nothing at run time.</p><pre style="white-space:pre;">int result = 2 + 3 * 4;      /* 14, not 20 - * binds tighter than + */
int clearer = 2 + (3 * 4);   /* same result, intent obvious at a glance */</pre><p><b>See also:</b> <a href="cppref:op_arithmetic">Arithmetic Operators (+ - * / %)</a>, <a href="cppref:op_assignment">Assignment & Compound Assignment</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Mischt ein Ausdruck mehrere Operatoren, legt die Rangfolge (Precedence) die Auswertungsreihenfolge fest - z. B. bindet * stärker als +, also bedeutet a + b * c dasselbe wie a + (b * c). Die Assoziativität entscheidet die Reihenfolge unter GLEICHRANGIGEN Operatoren: die meisten binären Operatoren werten links nach rechts aus, Zuweisung und der Ternäroperator dagegen rechts nach links. Im Zweifel - oder wenn für eine Leserschaft geschrieben wird, die im Zweifel sein könnte - lieber explizite Klammern setzen; sie kosten zur Laufzeit nichts.</p><pre style="white-space:pre;">int result = 2 + 3 * 4;      /* 14, not 20 - * binds tighter than + */
int clearer = 2 + (3 * 4);   /* same result, intent obvious at a glance */</pre><p><b>Siehe auch:</b> <a href="cppref:op_arithmetic">Arithmetic Operators (+ - * / %)</a>, <a href="cppref:op_assignment">Assignment & Compound Assignment</a></p>)HTML"));

    addEntry(QStringLiteral("op_overloading"), QStringLiteral("Operators"), QStringLiteral("Operator Overloading (C++)"),
              QStringLiteral(R"HTML(<p><i>C++ only.</i></p><p>A C++ class can define its own meaning for an existing operator symbol (+, ==, <<, and so on) when applied to its own type, letting objects be used with natural-looking expression syntax instead of named member functions. The compiler picks the matching overload the same way it picks any overloaded function - by the types of the operands.</p><pre style="white-space:pre;">struct Point
{
    int x, y;
    Point operator+(const Point &amp;other) const
    {
        return Point{ x + other.x, y + other.y };
    }
};
Point a{1, 2}, b{3, 4};
Point c = a + b;   // calls Point::operator+</pre><p><b>See also:</b> <a href="cppref:fn_overloading">Function Overloading (C++)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++.</i></p><p>Eine C++-Klasse kann einem bestehenden Operatorsymbol (+, ==, <<, usw.) eine eigene Bedeutung für ihren eigenen Typ geben, sodass Objekte mit natürlich wirkender Ausdruckssyntax statt benannter Elementfunktionen verwendet werden können. Der Compiler wählt die passende Überladung genauso wie bei jeder überladenen Funktion - anhand der Typen der Operanden.</p><pre style="white-space:pre;">struct Point
{
    int x, y;
    Point operator+(const Point &amp;other) const
    {
        return Point{ x + other.x, y + other.y };
    }
};
Point a{1, 2}, b{3, 4};
Point c = a + b;   // calls Point::operator+</pre><p><b>Siehe auch:</b> <a href="cppref:fn_overloading">Function Overloading (C++)</a></p>)HTML"));

}

void CppRefReader::populateFunctions()
{
    addEntry(QStringLiteral("fn_declaration_definition"), QStringLiteral("Functions"), QStringLiteral("Declaring & Defining Functions"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>A function DECLARATION (prototype) states its name, return type and parameter types so callers - and the compiler - know its signature before it is actually implemented, usually placed in a header file. The DEFINITION provides the real body, usually in a .c/.cpp file. The same declaration/definition distinction applies to variables.</p><pre style="white-space:pre;">/* declaration (e.g. in a .h file) */
int addNumbers(int a, int b);

/* definition (e.g. in a .c file) */
int addNumbers(int a, int b)
{
    return a + b;
}</pre><p><b>See also:</b> <a href="cppref:var_declaration">Declaring & Defining a Variable</a>, <a href="cppref:fn_return_values">Return Values</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Eine Funktions-DEKLARATION (Prototyp) legt Name, Rückgabetyp und Parametertypen fest, damit Aufrufer - und der Compiler - die Signatur kennen, bevor sie tatsächlich implementiert ist, meist in einer Header-Datei. Die DEFINITION liefert den eigentlichen Rumpf, meist in einer .c-/.cpp-Datei. Dieselbe Unterscheidung Deklaration/Definition gilt auch für Variablen.</p><pre style="white-space:pre;">/* declaration (e.g. in a .h file) */
int addNumbers(int a, int b);

/* definition (e.g. in a .c file) */
int addNumbers(int a, int b)
{
    return a + b;
}</pre><p><b>Siehe auch:</b> <a href="cppref:var_declaration">Declaring & Defining a Variable</a>, <a href="cppref:fn_return_values">Return Values</a></p>)HTML"));

    addEntry(QStringLiteral("fn_parameters_by_value"), QStringLiteral("Functions"), QStringLiteral("Parameters Passed by Value"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>By default, C/C++ function parameters are passed BY VALUE: the function receives its own independent copy of each argument, and changes made to a parameter inside the function never affect the caller's original variable.</p><pre style="white-space:pre;">void tryToDouble(int n)
{
    n = n * 2;   /* only changes the local copy */
}
int x = 5;
tryToDouble(x);
/* x is still 5 here */</pre><p><b>See also:</b> <a href="cppref:fn_parameters_by_pointer">Parameters Passed by Pointer</a>, <a href="cppref:fn_parameters_by_reference">Parameters Passed by Reference (C++)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Standardmäßig werden C/C++-Funktionsparameter PER WERT übergeben: Die Funktion erhält eine eigene, unabhängige Kopie jedes Arguments, und Änderungen an einem Parameter innerhalb der Funktion wirken sich nie auf die ursprüngliche Variable des Aufrufers aus.</p><pre style="white-space:pre;">void tryToDouble(int n)
{
    n = n * 2;   /* only changes the local copy */
}
int x = 5;
tryToDouble(x);
/* x is still 5 here */</pre><p><b>Siehe auch:</b> <a href="cppref:fn_parameters_by_pointer">Parameters Passed by Pointer</a>, <a href="cppref:fn_parameters_by_reference">Parameters Passed by Reference (C++)</a></p>)HTML"));

    addEntry(QStringLiteral("fn_parameters_by_pointer"), QStringLiteral("Functions"), QStringLiteral("Parameters Passed by Pointer"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>To let a function modify a caller's variable - or avoid copying a large struct - pass a POINTER to it instead of the value itself. The function receives the address and dereferences it to reach the original.</p><pre style="white-space:pre;">void doubleIt(int *n)
{
    *n = *n * 2;   /* modifies the caller's variable through the pointer */
}
int x = 5;
doubleIt(&amp;x);
/* x is now 10 */</pre><p><b>See also:</b> <a href="cppref:fn_parameters_by_value">Parameters Passed by Value</a>, <a href="cppref:var_pointer_basics">Pointer Variables</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Damit eine Funktion die Variable eines Aufrufers ändern kann - oder das Kopieren eines großen struct vermeidet - wird stattdessen ein ZEIGER darauf übergeben. Die Funktion erhält die Adresse und dereferenziert sie, um an das Original zu gelangen.</p><pre style="white-space:pre;">void doubleIt(int *n)
{
    *n = *n * 2;   /* modifies the caller's variable through the pointer */
}
int x = 5;
doubleIt(&amp;x);
/* x is now 10 */</pre><p><b>Siehe auch:</b> <a href="cppref:fn_parameters_by_value">Parameters Passed by Value</a>, <a href="cppref:var_pointer_basics">Pointer Variables</a></p>)HTML"));

    addEntry(QStringLiteral("fn_parameters_by_reference"), QStringLiteral("Functions"), QStringLiteral("Parameters Passed by Reference (C++)"),
              QStringLiteral(R"HTML(<p><i>C++ only.</i></p><p>C++ references give the same "modify the caller's variable" ability as pointer parameters, without the caller needing to take an address, or the function needing to dereference anything - the parameter simply IS the caller's variable for the duration of the call.</p><pre style="white-space:pre;">void doubleIt(int &amp;n)
{
    n = n * 2;
}
int x = 5;
doubleIt(x);   // no &amp; needed at the call site
// x is now 10</pre><p><b>See also:</b> <a href="cppref:fn_parameters_by_pointer">Parameters Passed by Pointer</a>, <a href="cppref:var_reference">References (C++)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++.</i></p><p>C++-Referenzen bieten dieselbe Fähigkeit, "die Variable des Aufrufers zu ändern", wie Zeigerparameter, ohne dass der Aufrufer eine Adresse bilden oder die Funktion irgendetwas dereferenzieren muss - der Parameter IST für die Dauer des Aufrufs die Variable des Aufrufers.</p><pre style="white-space:pre;">void doubleIt(int &amp;n)
{
    n = n * 2;
}
int x = 5;
doubleIt(x);   // no &amp; needed at the call site
// x is now 10</pre><p><b>Siehe auch:</b> <a href="cppref:fn_parameters_by_pointer">Parameters Passed by Pointer</a>, <a href="cppref:var_reference">References (C++)</a></p>)HTML"));

    addEntry(QStringLiteral("fn_return_values"), QStringLiteral("Functions"), QStringLiteral("Return Values"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>return ends a function and, unless it returns void, hands a value back to the caller. A function may have several return statements for different code paths, but falling off the end of a non-void function without returning a value is undefined behavior. C (unlike C++) makes main a special case, allowed to fall off the end, implicitly returning 0.</p><pre style="white-space:pre;">int maxOfTwo(int a, int b)
{
    if (a &gt; b)
        return a;
    return b;
}</pre><p><b>See also:</b> <a href="cppref:fn_declaration_definition">Declaring & Defining Functions</a>, <a href="cppref:kw_return">return</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>return beendet eine Funktion und liefert, sofern sie nicht void ist, einen Wert an den Aufrufer zurück. Eine Funktion darf mehrere return-Anweisungen für verschiedene Codepfade haben, aber das Ende einer Nicht-void-Funktion ohne Rückgabewert zu erreichen ist undefiniertes Verhalten. C (anders als C++) macht main zu einem Sonderfall, der ans Ende fallen darf und dabei implizit 0 zurückgibt.</p><pre style="white-space:pre;">int maxOfTwo(int a, int b)
{
    if (a &gt; b)
        return a;
    return b;
}</pre><p><b>Siehe auch:</b> <a href="cppref:fn_declaration_definition">Declaring & Defining Functions</a>, <a href="cppref:kw_return">return</a></p>)HTML"));

    addEntry(QStringLiteral("fn_default_arguments"), QStringLiteral("Functions"), QStringLiteral("Default Arguments (C++)"),
              QStringLiteral(R"HTML(<p><i>C++ only.</i></p><p>A C++ function can give one or more of its trailing parameters a default value, letting callers omit them entirely. Defaults must be given from right to left: once one parameter has a default, every parameter after it must have one too.</p><pre style="white-space:pre;">void greet(const char *name, const char *greeting = "Hello")
{
    std::cout &lt;&lt; greeting &lt;&lt; ", " &lt;&lt; name &lt;&lt; "!\n";
}
greet("World");            // uses the default greeting
greet("World", "Hi");      // overrides it</pre><p><b>See also:</b> <a href="cppref:fn_overloading">Function Overloading (C++)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++.</i></p><p>Eine C++-Funktion kann einem oder mehreren ihrer letzten Parameter einen Standardwert geben, sodass Aufrufer sie ganz weglassen können. Standardwerte müssen von rechts nach links vergeben werden: Sobald ein Parameter einen Standardwert hat, muss jeder folgende Parameter ebenfalls einen haben.</p><pre style="white-space:pre;">void greet(const char *name, const char *greeting = "Hello")
{
    std::cout &lt;&lt; greeting &lt;&lt; ", " &lt;&lt; name &lt;&lt; "!\n";
}
greet("World");            // uses the default greeting
greet("World", "Hi");      // overrides it</pre><p><b>Siehe auch:</b> <a href="cppref:fn_overloading">Function Overloading (C++)</a></p>)HTML"));

    addEntry(QStringLiteral("fn_overloading"), QStringLiteral("Functions"), QStringLiteral("Function Overloading (C++)"),
              QStringLiteral(R"HTML(<p><i>C++ only.</i></p><p>C++ lets several functions share the same name as long as their parameter lists differ, in number and/or type - the compiler picks the matching overload at each call site based on the argument types. Plain C has no overloading at all: every function name must be unique.</p><pre style="white-space:pre;">int square(int n) { return n * n; }
double square(double n) { return n * n; }

int a = square(5);       // calls the int version
double b = square(2.5);  // calls the double version</pre><p><b>See also:</b> <a href="cppref:fn_default_arguments">Default Arguments (C++)</a>, <a href="cppref:op_overloading">Operator Overloading (C++)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++.</i></p><p>C++ erlaubt mehreren Funktionen denselben Namen, solange sich ihre Parameterlisten unterscheiden, in Anzahl und/oder Typ - der Compiler wählt an jeder Aufrufstelle anhand der Argumenttypen die passende Überladung. Reines C kennt gar keine Überladung: Jeder Funktionsname muss eindeutig sein.</p><pre style="white-space:pre;">int square(int n) { return n * n; }
double square(double n) { return n * n; }

int a = square(5);       // calls the int version
double b = square(2.5);  // calls the double version</pre><p><b>Siehe auch:</b> <a href="cppref:fn_default_arguments">Default Arguments (C++)</a>, <a href="cppref:op_overloading">Operator Overloading (C++)</a></p>)HTML"));

    addEntry(QStringLiteral("fn_recursion"), QStringLiteral("Functions"), QStringLiteral("Recursion"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>A function that calls itself, directly or indirectly. Every recursive function needs a base case that stops the recursion, or it keeps calling itself until the call stack overflows. Some problems - tree/list traversal, classic divide-and-conquer algorithms - express far more naturally as recursion than as an explicit loop.</p><pre style="white-space:pre;">unsigned long factorial(unsigned int n)
{
    if (n &lt;= 1)          /* base case */
        return 1;
    return n * factorial(n - 1);   /* recursive case */
}</pre><p><b>See also:</b> <a href="cppref:fn_return_values">Return Values</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Eine Funktion, die sich selbst aufruft, direkt oder indirekt. Jede rekursive Funktion braucht einen Basisfall, der die Rekursion stoppt, sonst ruft sie sich auf, bis der Aufrufstapel überläuft. Manche Probleme - das Durchlaufen von Bäumen/Listen, klassische Teile-und-herrsche-Algorithmen - lassen sich als Rekursion weit natürlicher ausdrücken als mit einer expliziten Schleife.</p><pre style="white-space:pre;">unsigned long factorial(unsigned int n)
{
    if (n &lt;= 1)          /* base case */
        return 1;
    return n * factorial(n - 1);   /* recursive case */
}</pre><p><b>Siehe auch:</b> <a href="cppref:fn_return_values">Return Values</a></p>)HTML"));

    addEntry(QStringLiteral("fn_pointers"), QStringLiteral("Functions"), QStringLiteral("Function Pointers"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>A variable that holds the ADDRESS of a function rather than calling it, letting the function actually run be chosen or changed at run time. Central to C-style callback APIs, including much of the Amiga's own hook/callback conventions - Intuition and BOOPSI dispatcher functions, ReAction custom-class Hook structures, and more all pass functions around this way.</p><pre style="white-space:pre;">int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }

int (*operation)(int, int) = add;
printf("%d\n", operation(3, 4));   /* calls add(3, 4) -&gt; 7 */
operation = sub;
printf("%d\n", operation(3, 4));   /* calls sub(3, 4) -&gt; -1 */</pre><p><b>See also:</b> <a href="cppref:dt_amiga_aptr">APTR</a>, <a href="cppref:fn_lambda">Lambda Expressions (C++11)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Eine Variable, die die ADRESSE einer Funktion hält statt sie aufzurufen, sodass die tatsächlich auszuführende Funktion zur Laufzeit gewählt oder geändert werden kann. Zentral für C-artige Callback-APIs, einschließlich vieler der eigenen Hook-/Callback-Konventionen des Amiga - Intuition- und BOOPSI-Dispatcher-Funktionen, ReAction-Custom-Class-Hook-Strukturen und mehr geben Funktionen genau auf diesem Weg weiter.</p><pre style="white-space:pre;">int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }

int (*operation)(int, int) = add;
printf("%d\n", operation(3, 4));   /* calls add(3, 4) -&gt; 7 */
operation = sub;
printf("%d\n", operation(3, 4));   /* calls sub(3, 4) -&gt; -1 */</pre><p><b>Siehe auch:</b> <a href="cppref:dt_amiga_aptr">APTR</a>, <a href="cppref:fn_lambda">Lambda Expressions (C++11)</a></p>)HTML"));

    addEntry(QStringLiteral("fn_variadic"), QStringLiteral("Functions"), QStringLiteral("Variadic Functions (...)"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>A function whose parameter list ends in ... accepts any number of additional arguments beyond its fixed ones - printf() is the best-known example. Reading them back inside the function needs the &lt;stdarg.h&gt; macros (va_list, va_start, va_arg, va_end); unlike normal parameters, there is no automatic type checking on the variadic arguments, so caller and callee must agree on their types by convention, typically via a format string.</p><pre style="white-space:pre;">#include &lt;stdarg.h&gt;

void logMessages(int count, ...)
{
    va_list args;
    va_start(args, count);
    for (int i = 0; i &lt; count; i++)
        printf("%s\n", va_arg(args, const char *));
    va_end(args);
}</pre><p><b>See also:</b> <a href="cppref:fn_pointers">Function Pointers</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Eine Funktion, deren Parameterliste mit ... endet, akzeptiert beliebig viele zusätzliche Argumente über ihre festen hinaus - printf() ist das bekannteste Beispiel. Sie innerhalb der Funktion wieder auszulesen braucht die Makros aus &lt;stdarg.h&gt; (va_list, va_start, va_arg, va_end); anders als bei normalen Parametern gibt es keine automatische Typprüfung der variadischen Argumente - Aufrufer und Funktion müssen sich per Konvention auf die Typen einigen, meist über einen Formatstring.</p><pre style="white-space:pre;">#include &lt;stdarg.h&gt;

void logMessages(int count, ...)
{
    va_list args;
    va_start(args, count);
    for (int i = 0; i &lt; count; i++)
        printf("%s\n", va_arg(args, const char *));
    va_end(args);
}</pre><p><b>Siehe auch:</b> <a href="cppref:fn_pointers">Function Pointers</a></p>)HTML"));

    addEntry(QStringLiteral("fn_static_functions"), QStringLiteral("Functions"), QStringLiteral("static Functions (Internal Linkage)"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>Marking a function static at file scope limits its visibility to the file (translation unit) it is defined in - the same effect static has on a global variable. Useful for helper functions that are implementation details of one .c file and should not be callable, or clash by name, from anywhere else.</p><pre style="white-space:pre;">static int helperFunction(int n)   /* only visible within this file */
{
    return n * 2;
}</pre><p><b>See also:</b> <a href="cppref:var_static">static Variables & Lifetime</a>, <a href="cppref:kw_static">static</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>Eine Funktion auf Dateiebene als static zu markieren beschränkt ihre Sichtbarkeit auf die Datei (Übersetzungseinheit), in der sie definiert ist - derselbe Effekt, den static auf eine globale Variable hat. Nützlich für Hilfsfunktionen, die Implementierungsdetails einer einzelnen .c-Datei sind und von nirgendwo sonst aufrufbar sein oder namentlich kollidieren sollen.</p><pre style="white-space:pre;">static int helperFunction(int n)   /* only visible within this file */
{
    return n * 2;
}</pre><p><b>Siehe auch:</b> <a href="cppref:var_static">static Variables & Lifetime</a>, <a href="cppref:kw_static">static</a></p>)HTML"));

    addEntry(QStringLiteral("fn_inline_practical"), QStringLiteral("Functions"), QStringLiteral("inline Functions in Practice"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>inline began life as a hint that the compiler should substitute a function's body directly at each call site instead of a real call - modern compilers mostly ignore that hint and decide for themselves. Its one guaranteed, still-relevant effect in C++ is different: it is what allows a function to be DEFINED (not just declared) in a header included by multiple .cpp files, without the linker complaining about multiple definitions.</p><p><b>See also:</b> <a href="cppref:kw_inline">inline</a>, <a href="cppref:fn_static_functions">static Functions (Internal Linkage)</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>inline begann als Hinweis, dass der Compiler den Rumpf einer Funktion direkt an jeder Aufrufstelle einsetzen soll statt eines echten Aufrufs - moderne Compiler ignorieren diesen Hinweis meist und entscheiden selbst. Der eine garantierte, weiterhin relevante Effekt in C++ ist ein anderer: Er erlaubt es, eine Funktion in einem von mehreren .cpp-Dateien eingebundenen Header zu DEFINIEREN (nicht nur zu deklarieren), ohne dass der Linker über mehrfache Definitionen klagt.</p><p><b>Siehe auch:</b> <a href="cppref:kw_inline">inline</a>, <a href="cppref:fn_static_functions">static Functions (Internal Linkage)</a></p>)HTML"));

    addEntry(QStringLiteral("fn_main"), QStringLiteral("Functions"), QStringLiteral("The main() Function"),
              QStringLiteral(R"HTML(<p><i>Available in C and C++.</i></p><p>main is the standard entry point of a hosted C/C++ program - execution begins there. It can be written with no parameters (int main(void)) or to receive the program's command-line arguments (int main(int argc, char *argv[])); its return value becomes the process's exit status, with 0 conventionally meaning success.</p><pre style="white-space:pre;">int main(int argc, char *argv[])
{
    for (int i = 1; i &lt; argc; i++)
        printf("arg %d: %s\n", i, argv[i]);
    return 0;
}</pre><p><b>Note:</b> On AmigaOS, a program launched from the CLI/Shell gets the same argc/argv as any hosted C program - but one started from Workbench (double-clicked as an icon) receives no command line at all, traditionally detected via argc == 0, with any startup arguments instead read from the WBStartup message.</p><p><b>See also:</b> <a href="cppref:fn_return_values">Return Values</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Verfügbar in C und C++.</i></p><p>main ist der übliche Einstiegspunkt eines gehosteten C/C++-Programms - dort beginnt die Ausführung. Sie kann ohne Parameter geschrieben werden (int main(void)) oder die Kommandozeilenargumente des Programms entgegennehmen (int main(int argc, char *argv[])); ihr Rückgabewert wird zum Exit-Status des Prozesses, wobei 0 üblicherweise Erfolg bedeutet.</p><pre style="white-space:pre;">int main(int argc, char *argv[])
{
    for (int i = 1; i &lt; argc; i++)
        printf("arg %d: %s\n", i, argv[i]);
    return 0;
}</pre><p><b>Hinweis:</b> Unter AmigaOS erhält ein von der CLI/Shell gestartetes Programm dieselben argc/argv wie jedes gehostete C-Programm - eines, das vom Workbench aus gestartet wird (per Doppelklick auf ein Icon), bekommt dagegen gar keine Kommandozeile, traditionell erkannt an argc == 0, wobei etwaige Startparameter stattdessen aus der WBStartup-Message gelesen werden.</p><p><b>Siehe auch:</b> <a href="cppref:fn_return_values">Return Values</a></p>)HTML"));

    addEntry(QStringLiteral("fn_lambda"), QStringLiteral("Functions"), QStringLiteral("Lambda Expressions (C++11)"),
              QStringLiteral(R"HTML(<p><i>C++ only.</i></p><p>A lambda is an unnamed function written inline at the point it is used - handy for short callbacks (e.g. a Qt signal/slot connect(), or a standard-library algorithm) without writing a separate named function. Written as [captures](parameters) { body }; the capture list controls which surrounding variables the lambda can see, by value ([x]) or by reference ([&x]).</p><pre style="white-space:pre;">int threshold = 10;
auto isAboveThreshold = [threshold](int value) {
    return value &gt; threshold;
};
bool result = isAboveThreshold(15);   // true</pre><p><b>See also:</b> <a href="cppref:fn_pointers">Function Pointers</a></p>)HTML"),
              QStringLiteral(R"HTML(<p><i>Nur C++.</i></p><p>Eine Lambda ist eine unbenannte, inline an ihrer Verwendungsstelle geschriebene Funktion - praktisch für kurze Callbacks (z. B. ein Qt-Signal/Slot-connect(), oder einen Standardbibliothek-Algorithmus), ohne eine eigene benannte Funktion zu schreiben. Geschrieben als [Captures](Parameter) { Rumpf }; die Capture-Liste legt fest, welche umgebenden Variablen die Lambda sehen kann, per Wert ([x]) oder per Referenz ([&x]).</p><pre style="white-space:pre;">int threshold = 10;
auto isAboveThreshold = [threshold](int value) {
    return value &gt; threshold;
};
bool result = isAboveThreshold(15);   // true</pre><p><b>Siehe auch:</b> <a href="cppref:fn_pointers">Function Pointers</a></p>)HTML"));

}
