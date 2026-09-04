# AmigaED – Qt5 → Qt6 Portierung

Build verifiziert mit **Qt 6.4.2** (qmake, `libqscintilla2-qt6-dev`) in einer
Ubuntu-24.04-Sandbox: qmake-Lauf, Kompilierung und Linking waren fehlerfrei,
das Binary wurde tatsächlich erzeugt und startet (nur mit Platzhalter-PNGs
getestet, da `images/*.png` nicht im Upload enthalten waren).

## Geänderte Stellen

### application.pro
- `CONFIG += ... c++14 ...` → `c++17`
- Kommentar ergänzt: `qscintilla2` muss selbst gegen Qt6 gebaut sein
  (eigene `qscintilla2.prf`). Auf Ubuntu/Debian reicht
  `libqscintilla2-qt6-dev`; auf anderen Systemen ggf. QScintilla aus den
  Quellen mit `qmake6` neu bauen.

### mainwindow.h
- `#include <QtGui>` entfernt (unnötiger Sammel-Header, redundant zu den
  vorhandenen Einzel-Includes)
- `#include <QPageSize>` ergänzt (für den neuen `setPageSize()`-Aufruf)
- `#include <utility>` ergänzt (für `std::as_const`)
- `Q_PID proc_pid;` → `qint64 proc_pid;`
  (Qt6 hat `Q_PID`/`QProcess::pid()` entfernt)
- `QTime timerCompile;` → `QElapsedTimer timerCompile;`
  (`QTime::start()`/`::elapsed()` existieren in Qt6 nicht mehr;
  `QElapsedTimer` war schon vorher inkludiert)

### mainwindow.cpp
- `QRegExp("[\n]")` + `QString::SkipEmptyParts`
  → `data.split(QLatin1Char('\n'), Qt::SkipEmptyParts)`
- `<< endl` → `<< Qt::endl`
- `foreach (QString item, listToFilter)` → range-based `for`-Schleife mit
  `std::as_const(...)`
- `myEmulator.pid()` (3×) → `myEmulator.processId()`
- `fontmetrics.width(...)` (3×) → `fontmetrics.horizontalAdvance(...)`
- `printer.setPageSize(QsciPrinter::A4)` →
  `printer.setPageSize(QPageSize(QPageSize::A4))`
  (`QPrinter::setPageSize()` nimmt in Qt6 ein `QPageSize`-Objekt statt des
  alten Enum-Werts)
- **Echter Logikfehler behoben** (unabhängig von Qt6, fiel erst durch
  GCCs `-Wmisleading-indentation` beim Nachbau auf): in `checkVBCC()`
  fehlten um einen mehrzeiligen `if`-Block die geschweiften Klammern.
  Dadurch lief `qDebug() << "goto line:" << QString::number(list[index]);`
  immer, auch wenn `index` außerhalb der Listengrenzen lag (potenzieller
  Out-of-Bounds-Zugriff). Jetzt korrekt in `{ }` eingeschlossen.

### aboutdialog.cpp / prefsdialog.cpp
- Fensterflags bereinigt:
  `Qt::Dialog | Qt::Desktop | Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint`
  → `Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint`
  (`Qt::Desktop` ist in Qt6 deprecated; `Dialog`, `Desktop` und `Window`
  sind eigentlich sich gegenseitig ausschließende Fenster-*Typen* – die
  Kombination war schon in der Qt5-Version fragwürdig)

## Update: alle Warnungen bereinigt

Der Build ist jetzt **komplett warnungsfrei** (0 Warnungen, 0 Fehler,
verifiziert mit `qmake6 + mingw32-make`-Testlauf).

- **5× `QMessageBox::warning()`/`::question()`** mit der alten,
  deprecated `int`-Flags-Überladung (`QMessageBox::Yes | QMessageBox::Default`,
  `QMessageBox::Escape` usw.) auf die moderne `StandardButtons`-Überladung
  umgestellt (`QMessageBox::Yes | QMessageBox::No`, Default-Button als
  eigener Parameter). Verhalten (Default-/Escape-Taste) bleibt identisch:
  Cancel/No fungiert automatisch weiter als Escape-Taste, sobald es unter
  den übergebenen Buttons ist. Betroffen: `maybeSave()`,
  `actionInsertShellAppSkeletton()`, `actionInsertCAppSkeletton()`,
  `actionInsertCppAppSkeletton()` sowie eine weitere Stelle beim Öffnen
  nicht existierender Dateien.
- Die ~105 alten `SIGNAL()/SLOT()`-Makro-Connects erzeugen **keine**
  Compiler-Warnungen (sie sind nicht deprecated, nur weniger
  typsicher als die Funktionszeiger-Syntax) und wurden daher nicht
  angetastet - eine Umstellung wäre weiterhin ein rein optionales,
  separates Refactoring, kein Bugfix.
- **`.ui`-Dateien** brauchten keine Änderung – alle verwendeten Widget-
  Klassen (`QCheckBox`, `QComboBox`, `QDialog`, `QGridLayout`, `QGroupBox`,
  `QLabel`, `QLineEdit`, `QPushButton`, `QTabWidget`, `QTextBrowser`,
  `QWidget`) existieren unverändert in Qt6.
- **`application.qrc`** referenziert einen `images/`-Ordner mit 30 PNGs,
  der nicht im Upload enthalten war – für den echten Build musst du diesen
  Ordner (aus deinem Original-Repo) neben die `.pro`-Datei legen.

## Update: Feature "Last files" (zuletzt geöffnete Dateien)

Neuer Menüpunkt **File → Last files** mit Untermenü der letzten 10 geöffneten
Dateien (vollständiger Pfad als Anzeigetext, neuste oben).

**Verhalten:**
- Beim Öffnen einer Datei (`open()`-Dialog sowie Kommandozeilen-Laden über
  `loadNonExistantFile()`) **und beim Speichern** (`saveFile()` - deckt
  sowohl "Save" als auch "Save As" ab, inkl. neu erstellter und erstmals
  gespeicherter Dateien) wird sie automatisch vorne in die Liste
  aufgenommen; ein bereits vorhandener Eintrag wird zuerst entfernt (keine
  Duplikate) und landet dann wieder ganz oben.
- Ist die Liste voll (10 Einträge), fällt der älteste Eintrag automatisch
  raus, sobald ein neuer hinzukommt.
- Klick auf einen Eintrag öffnet die Datei (fragt vorher wie gewohnt nach
  ungesicherten Änderungen). Existiert die Datei nicht mehr, erscheint eine
  Warnmeldung und der Eintrag wird automatisch aus der Liste entfernt.
- Die Liste wird über `QSettings` (`RecentFiles/List`) gespeichert – bleibt
  also über Programmstarts hinweg erhalten, und wird auch beim manuellen
  "Reload settings" (Shift+F12) neu geladen.

**Geänderte/neue Stellen:**
- `mainwindow.h`: neues Menü-Member `recentFilesMenue`, `QStringList
  p_recentFiles`, Konstante `MaxRecentFiles = 10`, neue Methoden
  `updateRecentFilesMenu()`, `addToRecentFiles()`,
  `removeFromRecentFiles()`, neuer Slot `openRecentFile()`.
- `mainwindow.cpp`: Submenü-Erzeugung in `createMenus()` direkt nach
  `openAct`; Laden der Liste in `readSettings()`; Hooks in `loadFile()`,
  `loadNonExistantFile()` und `saveFile()`; komplette Implementierung
  direkt im Anschluss an `loadFile()`.

## Update: Amiga NDK 3.2R4 Datentypen im C/C++-Syntax-Highlighting

Neue Datei **amigalexercpp.h**: eine kleine `QsciLexerCPP`-Unterklasse
(`AmigaLexerCPP`), die die Datentypen aus `exec/types.h` (NDK 3.2R4) als
eigenes Keyword-Set (4, "global classes/typedefs") erkennt und farblich
hervorhebt (Indigo, fett - deutlich abgesetzt von normalen Keywords,
Kommentaren, Strings, Zahlen und Preprocessor-Direktiven).

Erfasste Typen: `APTR`, `CONST_APTR`, `STRPTR`, `CONST_STRPTR`, `LONG`,
`ULONG`, `LONGBITS`, `WORD`, `UWORD`, `WORDBITS`, `BYTE`, `UBYTE`,
`BYTEBITS`, `RPTR`, `CPTR`, `SHORT`, `USHORT`, `COUNT`, `UCOUNT`,
`FLOAT`, `DOUBLE`, `BOOL`, `TEXT`, `VOID`.

**Technischer Hintergrund:** `QsciLexerCPP` bietet zur Laufzeit keine
`setKeywords()`-Methode - Keyword-Sets werden stattdessen über die
virtuelle Methode `keywords(int set)` bereitgestellt, die überschrieben
werden muss. Ebenso weist `QsciLexerCPP::defaultColor()` dem Style
"GlobalClass" keine eigene Farbe zu, weshalb Farbe/Fettschrift explizit
in `initializeLexerCPP()` gesetzt werden, sonst wären die Amiga-Typen
zwar erkannt, aber optisch nicht von normalem Text zu unterscheiden.

**Geänderte/neue Stellen:**
- Neu: `amigalexercpp.h` (Klasse `AmigaLexerCPP`)
- `application.pro`: `amigalexercpp.h` zu `HEADERS` hinzugefügt
- `mainwindow.cpp`: `#include "amigalexercpp.h"`; `initializeLexerCPP()`
  verwendet jetzt `AmigaLexerCPP` statt `QsciLexerCPP` und setzt Farbe/Font
  für den `GlobalClass`-Style

## Update: Amiga NDK 3.2R4 Funktionsnamen im C/C++-Syntax-Highlighting

`amigalexercpp.h` erweitert - `AmigaLexerCPP` erkennt jetzt zusätzlich
**1413 Funktionsnamen** aus dem NDK 3.2R4 (Keyword-Set 2, Style
`KeywordSet2`), automatisch extrahiert aus den `clib/*_protos.h`-Dateien
deines hochgeladenen `Include_H.zip` (80 Bibliotheks-Header geparst,
79 Bibliotheken/Devices abgedeckt: exec, dos, intuition, graphics,
utility, gadtools, asl, layers, icon, iffparse, locale, timer, ... bis
hin zu Nischenbibliotheken wie `chooser`, `sketchboard`, `datebrowser`).
Farbe: Ziegelrot (firebrick), nicht fett - Funktionen kommen pro Zeile
deutlich häufiger vor als Typen, durchgehend fett wäre zu unruhig.

**Warum aus deinen eigenen Header-Dateien statt aus Gedächtnis:** Bei
~20 Datentypen war eine Recherche aus öffentlicher Doku zuverlässig
machbar; bei >1000 Funktionsnamen über 80 Bibliotheken wäre das
Risiko fehlender oder erfundener Namen zu hoch gewesen. Die
automatisierte Extraktion aus deinen echten Headern garantiert exakte,
vollständige Namen für genau deine NDK-Version.

**Verifikation:** Neben dem üblichen Kompilier-/Link-Test wurde die
Erkennung zusätzlich mit einem eigenständigen Testprogramm zur
Laufzeit geprüft (`QsciScintilla` mit `AmigaLexerCPP`, `SCI_GETSTYLEAT`
nach `SCI_COLOURISE` abgefragt): `APTR` → Style 19 (GlobalClass),
`AllocMem` → Style 16 (KeywordSet2), ein erfundener Funktionsname
→ Style 11 (normaler, unstyled Identifier). Die Erkennung funktioniert
also nachweislich nicht nur theoretisch, sondern tatsächlich zur
Laufzeit.

**Geänderte Stellen:**
- `amigalexercpp.h`: `keywords(int set)` behandelt jetzt zusätzlich
  `set == 2` (Funktionen), `amigaFunctions()` liefert die extrahierte
  Liste
- `mainwindow.cpp`: `initializeLexerCPP()` setzt zusätzlich eine Farbe
  für `QsciLexerCPP::KeywordSet2`

## Update: MUI 5 (Stefan Stuntz) im C/C++-Syntax-Highlighting ergänzt

`amigalexercpp.h` erweitert um Inhalte aus dem hochgeladenen
`MUI5_include.zip`:

- **Keyword-Set 2 (Funktionen, ziegelrot):** um 36 echte MUI-Funktionen
  ergänzt - 34 aus `clib/muimaster_protos.h` (`MUI_NewObject`,
  `MUI_DisposeObject`, `MUI_Request`, `MUI_CreateCustomClass`, ...) plus
  die beiden Komfort-Makros `set()`/`nnset()`. Zusammen mit den
  NDK-Funktionen jetzt **1449 Funktionsnamen** gesamt (dedupliziert -
  `DoMethod`, `SetAttrs`, `GetAttr` etc. kamen schon aus dem NDK/Utility
  und wurden nicht doppelt aufgenommen).
- **Keyword-Set 4 (bislang "Typen", jetzt indigo, fett):** um **1261**
  MUI-Konstanten aus `libraries/mui.h` ergänzt - alle `MUIA_*`
  (Attribute), `MUIM_*` (Methoden-IDs), `MUIC_*` (Klassennamen) und
  `MUIV_*` (Spezialwerte). Zusammen mit den NDK-Typen jetzt **1285**
  Einträge gesamt.

**Wichtige Design-Entscheidung:** `QsciLexerCPP` stellt insgesamt nur
zwei zusätzliche, unabhängig einfärbbare Keyword-Sets bereit (2 und 4) -
mehr lässt die Lexer-Engine nicht zu. Da `MUIA_`/`MUIM_`/`MUIC_`/`MUIV_`
mit Abstand die häufigsten Bezeichner in echtem MUI-Code sind (weit
häufiger als die reinen `MUI_*`-Funktionsaufrufe), wurden sie bewusst
dem "Typen"-Set zugeordnet statt ungefärbt zu bleiben - auch wenn es sich
technisch nicht um C-Datentypen handelt.

**Verifikation:** Ein erweiterter Laufzeittest hat zusätzlich zu den
NDK-Prüfungen bestätigt: `MUI_NewObject` → Style 16 (Funktion),
`MUIC_Window`/`MUIA_Window_Title`/`MUIM_Application_Load` → Style 19
(Typ/Konstante), eine komplette Beispielzeile im MUI-Stil
(`MUI_NewObject(MUIC_Window, MUIA_Window_Title, ..., TAG_DONE)`,
`DoMethod(app, MUIM_Application_Load, MUIV_Application_Load_ENV)`) wurde
korrekt erkannt, ein erfundenes Symbol blieb korrekt unstyled.

## Update: Amiga-typische Zeichensatz-Kodierung + garantiert fixed font

**Zeichensatz beim Laden/Speichern/Editieren (der eigentliche Kern der
Anfrage):** Bislang liefen alle `QTextStream`-Objekte in `loadFile()`,
`loadNonExistantFile()`, `saveFile()` und der Fileheader-Erzeugung ohne
explizite Kodierung - Qt6s `QTextStream` verwendet dann standardmäßig
**UTF-8**. Ein mit klassischer Amiga-Software (VBCC, SAS/C, alte
Amiga-Editoren) im dortigen Standard-Zeichensatz **ISO-8859-1/Latin-1**
gespeichertes `ä`/`ö`/`ü` wäre beim Laden zu ungültigen
UTF-8-Fortsetzungsbytes geworden und durch Qt6 stillschweigend zu
Replacement-Zeichen (`�`) zerstört worden. Jetzt setzen alle vier
Stellen explizit `.setEncoding(QStringConverter::Latin1)`.

**Verifiziert mit einem eigenständigen Roundtrip-Test:** eine Datei mit
den rohen Latin-1-Bytes `0xE4/0xF6/0xFC` (ä/ö/ü) wurde geladen, korrekt
als Unicode-Zeichen erkannt, wieder gespeichert und die Ausgabedatei
enthielt wieder exakt dieselben Einzelbytes (keine UTF-8-Mehrfachbyte-
Kodierung). Zum Vergleich wurde dieselbe Datei zusätzlich **ohne** den
Fix geladen, um den Unterschied zu belegen - Ergebnis dort: `ä ö ü`
wurden tatsächlich zu `���` zerstört, exakt das behobene Problem.

**Auch korrigiert:** `SCI_STYLESETCHARACTERSET` stand in allen
Lexer-Init-Funktionen auf `SC_CHARSET_8859_15` (Latin-9, die *moderne*
AmigaOS-4-Kodierung mit Euro-Zeichen). Für dieses klassische
AmigaOS-3.2/68k-Projekt ist historisch **ISO-8859-1** korrekt (identisch
mit dem klassischen Amiga-Zeichensatz bis auf ein einziges Sonderzeichen
bei 0x7F). Da QScintilla keine eigene `SC_CHARSET_8859_1`-Konstante
kennt, wird dafür der Standardwert `SC_CHARSET_ANSI` verwendet (das ist
die von QScintilla vorgesehene, Latin-1-äquivalente Einstellung für
diesen Zweck). Betrifft primär die Windows-Font-Glyphenzuordnung, nicht
die eigentliche Dateikodierung - wurde der Vollständigkeit halber
trotzdem korrigiert.

**Fixed font - jetzt konsequent statt nur beim Start:** `initializeFont()`
setzte den Font zwar schon mit `setFixedPitch(true)`, aber nur **einmal**
beim App-Start. Jeder Lexer-Wechsel (`initializeLexerCPP()`,
`initializeLexerMakefile()`, `initializeLexerBatch()`,
`initializeLexerFortran()`, `initializeLexerPascal()`,
`initializeLexerNone()`) erzeugt ein **neues** Lexer-Objekt mit seinen
eigenen, potenziell nicht-monospace Style-Default-Fonts - das hätte den
fixed font beim Umschalten der Syntax stillschweigend wieder aufheben
können. Jede dieser Funktionen ruft jetzt `lexer->setFont(myfont)`
(bzw. bei `initializeLexerNone()` direkt `textEdit->setFont(myfont)`,
da dort kein Lexer-Objekt existiert) auf. Zusätzlich zu
`setFixedPitch(true)` sorgt jetzt `setStyleHint(QFont::Monospace,
QFont::PreferMatch)` dafür, dass Qt bei fehlender Font-Installation
(z.B. "Source Code Pro" nicht vorhanden) robust auf eine andere
Festbreitenschrift ausweicht statt auf eine proportionale.

## Update: Font-Größe verstellbar (Zoom)

Neue Einträge im **View**-Menü: "Increase font size" (Ctrl++),
"Decrease font size" (Ctrl+-), "Reset font size" (Ctrl+0).

**Technischer Ansatz:** Nutzt QScintillas eingebauten Zoom-Mechanismus
(`zoomIn()`/`zoomOut()`/`zoomTo()`, intern `SCI_SETZOOM`) statt den
`myfont`-Objekt selbst zu verändern. Das skaliert alle Stile (normaler
Text, Kommentare, die Amiga-Typen/-Funktionen-Farben aus
`amigalexercpp.h`, ...) gleichmäßig, ohne die einzelnen
Style-Font-Einstellungen jedes Mal neu anfassen zu müssen.

- Wird über `QSettings` (`EditorZoomLevel`) gespeichert und beim
  nächsten Start automatisch wiederhergestellt.
- Da `textEdit` im Konstruktor zum Zeitpunkt des Settings-Ladens noch
  nicht existiert, wird der geladene Wert zunächst im neuen Member
  `p_zoomLevel` zwischengespeichert und erst in `initializeGUI()`
  angewendet, nachdem Widget und Lexer bereit sind.
- Auch mit Strg+Mausrad-Zoom kompatibel (QScintilla unterstützt das
  bereits eingebaut) - der beim Beenden gespeicherte Wert spiegelt
  immer den zuletzt aktiven Zoom-Stand wider, unabhängig davon, wie er
  erreicht wurde.

**Verifiziert** mit einem eigenständigen Testprogramm: zoomIn()/
zoomOut()/zoomTo() liefern exakt die erwarteten Zoom-Stufen (0 → +2 → +1
→ Reset auf 0 → simulierte Wiederherstellung auf 5), bestätigt über
`SCI_GETZOOM`.

**Geänderte Stellen:**
- `mainwindow.h`: `zoomInAct`/`zoomOutAct`/`zoomResetAct`, Slot
  `actionResetFontSize()`, Member `p_zoomLevel`
- `mainwindow.cpp`: Actions in `createActions()` erzeugt (zoomIn/zoomOut
  direkt mit `textEdit`s eigenen Slots verbunden, Reset über eigenen
  Wrapper-Slot), ins View-Menü eingehängt, Laden in `readPosSettings()`,
  Speichern in `writeSettings()`, Anwenden in `initializeGUI()`

## Update: AmigaOS-4-Compilierungsoptionen entfernt

Alle Optionen zum **Compilieren** für AmigaOS 4 wurden entfernt. VBCC
bot bisher 3 Ziel-Profile: OS 1.3, OS 3.x, OS 4.x - jetzt nur noch
OS 1.3 und OS 3.x.

**Bewusst NICHT angefasst:** Die Emulator-Konfiguration für OS 4.1
(`UAE/Os40ConfigPath`, `btn_getOS4Configfile`, `emulator40Act` im
Tools-Menü) betrifft das **Ausführen/Testen** eines bereits gebauten
Programms unter Emulation, nicht das Compilieren - das war explizit
nicht Teil der Anfrage und bleibt daher erhalten.

**Geänderte Stellen:**
- `mainwindow.h`: `p_compiler_vc40_call` entfernt; `p_targetOS`
  von `{"OS 1.3", "OS 3.x", "OS 4.x"}` auf `{"OS 1.3", "OS 3.x"}`
  reduziert
- `mainwindow.cpp`: Laden von `VBCC/VcDefaultOpts40` entfernt; die
  beiden Switch-Anweisungen für die Ziel-OS-Auswahl (beim
  Compiler-Wechsel und in `setVbccTargetOS()`) verloren ihren
  `case 2`-Zweig; die Vollständigkeitsprüfung der Compiler-Einstellungen
  prüft `p_compiler_vc40_call` nicht mehr; das Boing-Ball-Icon
  (Index 2) der Ziel-OS-Combobox in der Statusleiste wurde entfernt
- `prefsdialog.cpp`: Laden/Speichern von `VBCC/VcDefaultOpts40` entfernt
- `prefsdialog.ui`: Eingabefeld "OS 4.x default opts" (samt Label)
  komplett entfernt; Eintrag "OS 4.x" aus der Ziel-OS-Combobox entfernt
  (Tooltip entsprechend angepasst)

**Hinweis für bestehende Installationen:** Hatte ein Nutzer zuvor
"OS 4.x" (Index 2) als Standard-Ziel ausgewählt und gespeichert, zeigt
die jetzt nur noch 2 Einträge umfassende Combobox beim ersten Start
nach dem Update ggf. keine Auswahl an einer sinnvollen Stelle - das ist
unkritisch (kein Absturz), der Nutzer muss dann einmalig neu OS 1.3
oder OS 3.x auswählen.

## Update: AmigaOS-4-Emulator-Optionen ebenfalls entfernt

Auf Nachfrage wurde zusätzlich die komplette OS-4.1-**Emulator**-Anbindung
entfernt (Ausführen/Testen unter Emulation, nicht nur Compilieren):

- **Tools-Menü:** Eintrag "Start Workbench 4.1 in UAE..." entfernt
- **Preferences → Emulator-Tab:** Eingabefeld "OS 4.x config" (samt
  Datei-Auswahl-Button) entfernt; Eintrag "AmigaOS 4.x" aus der
  "Default config"-Combobox entfernt

**Geänderte/entfernte Stellen:**
- `mainwindow.h`: `p_os40_config`, `actionEmuOS40()`, `emulator40Act`
  entfernt
- `mainwindow.cpp`: Action-Erzeugung/Verbindung, Menü-Eintrag,
  Settings-Laden, `case 3` in `actionEmulator()`'s Switch, die komplette
  `actionEmuOS40()`-Slot-Implementierung, die zugehörige Debug-Ausgabe
  sowie der Enable/Disable-Block in `setEmulatorMenu()` entfernt
- `prefsdialog.h`/`.cpp`: `on_btn_getOS4Configfile_clicked()` (Slot +
  Implementierung) sowie Laden/Speichern von `UAE/Os40ConfigPath`
  entfernt
- `prefsdialog.ui`: Eingabefeld-Zeile "OS 4.x config" samt Button und
  Label entfernt; "AmigaOS 4.x"-Eintrag aus `comboBox_defaultEmulator`
  entfernt; verwaiste Tabstops bereinigt

Damit ist AmigaOS 4 jetzt vollständig aus AmigaED entfernt - weder
Compilieren noch Emulator-Start dafür sind mehr möglich.

## Update: AmigaOS-2.0-Emulator-Option entfernt

Auf Nachfrage wurde auch die Emulator-Konfiguration für AmigaOS 2.0
entfernt (Tools-Menü "Start Workbench 2.1 in UAE..." und das
zugehörige Prefs-Feld). Verbleibende Emulator-Ziele: **nur noch
AmigaOS 1.3 und AmigaOS 3.x**.

**Wichtig, da Indizes betroffen sind:** `p_defaultEmulator` (0=OS1.3,
1=OS2.0, 2=OS3.x) wird jetzt nur noch mit 0=OS1.3, 1=OS3.x belegt -
sowohl die Switch-Anweisung in `actionEmulator()` als auch
`actionEmuOS30()` (setzte bisher Index 2, jetzt 1) wurden entsprechend
angepasst. Gleiches bei der "Default config"-Combobox in
`prefsdialog.ui` (`currentIndex` von 2 auf 1 korrigiert, damit
weiterhin "AmigaOS 3.x" vorausgewählt bleibt statt ins Leere zu
zeigen).

**Nebenbei behoben:** Zwei Lücken in der Grid-Zeilennummerierung der
`.ui`-Datei (entstanden beim vorherigen Entfernen der OS4-Felder),
die zwar keinen Fehler verursachten, aber unsauber waren - jetzt
durchgängig nummeriert.

**Geänderte/entfernte Stellen:**
- `mainwindow.h`: `p_os20_config`, `actionEmuOS20()`, `emulator20Act`
  entfernt
- `mainwindow.cpp`: Action-Erzeugung/Verbindung, Menü-Eintrag,
  Settings-Laden, `case 1` (OS2.0) aus `actionEmulator()`'s Switch,
  komplette `actionEmuOS20()`-Implementierung, zugehörige Debug-Ausgabe
  und Enable/Disable-Block in `setEmulatorMenu()` entfernt;
  `actionEmuOS30()` und die verbleibende Switch-Case auf neuen Index 1
  angepasst
- `prefsdialog.h`/`.cpp`: `on_btn_getOS20Configfile_clicked()` (Slot +
  Implementierung) sowie Laden/Speichern von `UAE/Os20ConfigPath`
  entfernt
- `prefsdialog.ui`: Eingabefeld-Zeile "OS 2.0 config" samt Button und
  Label entfernt; "AmigaOS 2.0"-Eintrag aus `comboBox_defaultEmulator`
  entfernt (`currentIndex` korrigiert); verwaiste Tabstops bereinigt;
  Grid-Zeilennummerierung durchgängig gemacht

AmigaED unterstützt jetzt ausschließlich **AmigaOS 1.3 und 3.x** - für
Compilierung wie Emulation gleichermaßen.

## Update: Prefs-Dialog aufgeräumt (Description/Purpose entfernt)

Im Reiter **Project** des Preferences-Dialogs wurden die nicht mehr
benötigten Felder **Description** und **Purpose** entfernt.

- `prefsdialog.ui`: Label + `QLineEdit` für beide Felder entfernt,
  darunterliegende Grid-Zeilen (Projects root / Default icon) von Row
  5/6 auf 3/4 nachgerückt, Tabstops bereinigt.
- `prefsdialog.cpp`: Speichern/Laden von `Project/Description` und
  `Project/Purpose` (`QSettings`) entfernt.
- `mainwindow.h`: Member `p_description` und `p_purpose` entfernt.
- `mainwindow.cpp`: alle Stellen, die diese Werte verwendet haben,
  bereinigt - Laden aus `QSettings` in `readSettings()`, die
  `Description:`/`Purpose:`-Zeilen im generierten `.h`-Fileheader
  (`insertFileHeader`-artiger Stream-Code) sowie in **vier**
  generierten Datei-Header-Kommentaren (`insertAt(...)` beim Anlegen
  neuer C/C++/Shell-Dateien), inkl. der dadurch überflüssig
  gewordenen doppelten leeren Kommentarzeile (` *`). Auch die beiden
  `qDebug()`-Ausgaben in `debugVars()` entfernt.
- **Bewusst NICHT angefasst:** die generische
  `" *\tPurpose:\tdo something usefull..."`-Zeile im
  Funktions-Kommentarskelett (Doku für eine einzelne Funktion) sowie
  die feste `"Description:\tAmigaED project main file"`-Zeile im
  Projekt-Template (`mainFileTemplateContent()`) - beide sind
  eigenständiger, nicht von `p_description`/`p_purpose` abgeleiteter
  Text und standen nicht zur Debatte.

**Verifiziert:** kompletter Build mit `qmake6 + make` (Qt 6.4.2,
libqscintilla2-qt6-dev) - 0 Fehler, Binary erzeugt und gelinkt.

## Update: Internationalisierung (I18n) - Englisch/Deutsch, Laufzeit-Umschaltung

AmigaED unterstützt jetzt zwei GUI-Sprachen: **Englisch** (weiterhin die
Quell-/Standardsprache) und **Deutsch**. Die Sprache kann sowohl zur
Laufzeit als auch als Standard für künftige Programmstarts gewählt werden.

**Menü View -> GUI Language:**
Neuer Eintrag ganz oben im View-Menü, gefolgt von einer Trennlinie.
Enthält die Untereinträge "English" und "Deutsch" als sich gegenseitig
ausschließende (checkbare) Einträge (`QActionGroup`). Ein Klick schaltet
die komplette Oberfläche sofort um - ohne Neustart.

**Prefs-Dialog, Reiter Misc, Group Box "Application Look and Feel":**
Neue ComboBox "Default GUI Language:" (Zeile mit Label, wie die anderen
Einstellungen in dieser Group Box). Legt fest, mit welcher Sprache das
Programm beim nächsten Start beginnt. Wird wie jede andere Misc-Einstellung
unter `MISC/DefaultGUILanguage` (`QSettings`, Werte `"en"`/`"de"`)
gespeichert und beim Schließen des Prefs-Dialogs sofort übernommen (über
den bestehenden `readSettings()`-Reload-Mechanismus).

**Technische Umsetzung:**
- `mainwindow.h`/`.cpp`: neue Member `guiLanguageMenue`,
  `guiLanguageEnglishAct`, `guiLanguageGermanAct`, `guiLanguageGroup`,
  `p_guiTranslator` (der aktuell installierte `QTranslator`, `nullptr`
  während Englisch aktiv ist), `p_guiLanguage`.
- `MainWindow::applyGuiLanguage(langCode)`: zentrale Methode - (de)installiert
  den `QTranslator` sauber (keine doppelte Verwaltung), speichert die Wahl
  sofort in `QSettings`, aktualisiert die Menü-Checkmarks und ruft
  `retranslateUi()` auf. Wird sowohl vom Menü (`actionSetGuiLanguageEnglish/
  German()`) als auch von `readSettings()` aufgerufen (Prefs-Dialog
  geschlossen, "Reload settings"/Shift+F12, **und** beim allerersten
  Konstruktor-Aufruf - da `readSettings()` dort als aller erste Zeile läuft,
  noch bevor irgendein Widget existiert, ist der Translator schon
  installiert, bevor `createActions()`/`createMenus()` überhaupt die ersten
  `tr()`-Aufrufe machen - kein sichtbares Aufflackern von Englisch beim
  Programmstart mit deutscher Voreinstellung).
- `MainWindow::retranslateUi()`: automatisch aus allen `tr()`-Aufrufen in
  `createActions()`/`createMenus()`/`createToolBars()` generiert (89
  Action-Texte, 85 StatusTips, 23 Menütitel, 6 Toolbar-Titel) und manuell
  ergänzt um Project-/Functions-Panel, Statusbar-Labels, das von Hand
  gebaute Such-Formular (`centerSearchForm`) und den Fenstertitel (ruft
  dafür die bestehende `updateWindowTitle()` erneut auf).
- Verbliebene, bislang nicht übersetzbare String-Literale ergänzt: 8x
  `createStatusBarMessage(...)`, der initiale Fenstertitel, das
  "Compiler:"-Label sowie Titel/Filter der Datei-Öffnen/Speichern-Dialoge.
- `prefsdialog.ui`/`.cpp`: neue ComboBox `comboBoxDefaultGuiLanguage`
  (Einträge "English"/"Deutsch" bewusst **nicht** übersetzt - Sprachnamen
  werden hier immer in der jeweils eigenen Sprache angezeigt, unabhängig
  von der gerade aktiven Oberflächensprache), Speichern/Laden über
  `currentData()`/`findData()` (robust, unabhängig vom angezeigten Text).
- `application.pro`: `TRANSLATIONS += translations/amigaed_de.ts`,
  `RESOURCES += ... translations.qrc`.
- `translations.qrc`: bettet die kompilierte `translations/amigaed_de.qm`
  unter dem Ressourcenpfad `:/translations/amigaed_de.qm` ein - keine
  externen Dateien nötig, alles steckt im Programm selbst.
- `translations/amigaed_de.ts`: **476 Strings** vollständig ins Deutsche
  übersetzt (`lupdate` fand 476, `lrelease` meldet "476 finished and 0
  unfinished"). Tastenkombinationen (z.B. "Ctrl+N") und
  Compiler-Optionsvorgaben in Eingabefeldern (z.B. "author_name",
  "-std=gnu89 -O2 ...") bleiben unverändert, da es sich um Beispieldaten
  bzw. tatsächliche Tastenkürzel handelt, nicht um GUI-Text.

**Verifikation:** Neben dem üblichen Kompilier-/Link-Test (0 Warnungen,
0 Fehler) wurde die Übersetzung zusätzlich mit zwei eigenständigen
Testprogrammen zur Laufzeit geprüft: (1) `QTranslator::load()` gegen die
kompilierte `.qm` mit stichprobenartigen `QCoreApplication::translate()`-
Aufrufen über alle fünf Kontexte (MainWindow, PrefsDialog, QObject,
aboutDialog, centerSearchForm) sowie ein unbekannter Schlüssel als
Fallback-Test; (2) eine vollständige Simulation des Umschalt-Zyklus
Englisch -> Deutsch -> Englisch, die exakt die in `applyGuiLanguage()`
verwendete Install-/Remove-Logik nachbildet - keine Rückstände nach dem
Zurückschalten.

**Um später weitere `tr()`-Strings zu übersetzen** (z.B. nach neuen
Features): `lupdate application.pro` aktualisiert `amigaed_de.ts` um neue
Strings (bestehende Übersetzungen bleiben erhalten), danach die neuen
`<translation type="unfinished"></translation>`-Einträge ausfüllen und
`lrelease application.pro` ausführen, um `amigaed_de.qm` neu zu erzeugen.

## Update: Functions Browser ein-/ausblendbar

Neues Menü-Paar **View -> Show Functions Browser / Hide Functions Browser**,
direkt nach der Trennlinie, die auf das GUI-Language-Untermenü folgt -
gefolgt von einer weiteren Trennlinie. Beide Einträge sind wie das
GUI-Language-Paar mutual exclusive (`QActionGroup`, checkbar); "Show..."
ist beim Programmstart aktiv, da das Functions-Panel standardmäßig
sichtbar ist.

**Technisch:** `functionsGroupBox` steckt als normales Widget in
`mainSplitter` (siehe `createProjectPanel()`/Konstruktor) - ein
`QSplitter` behandelt ein verstecktes Kind-Widget problemlos und gibt
dessen Platz automatisch frei. Die beiden neuen Slots
`actionShowFunctionsBrowser()`/`actionHideFunctionsBrowser()` rufen
daher schlicht `functionsGroupBox->setVisible(true/false)` auf - kein
Neuaufbau des Layouts nötig. Die Sichtbarkeit wird bewusst **nicht**
über `QSettings` persistiert (wie schon bei den bestehenden "Show/Hide
output pane"-Einträgen im Build-Menü) - es ist ein reiner
Sitzungs-Umschalter.

`retranslateUi()` wurde um die vier neuen Strings ergänzt, damit ein
Sprachwechsel zur Laufzeit auch diese beiden Menüeinträge korrekt
aktualisiert.

**Sprachdateien aktualisiert:** `lupdate` fand 4 neue Strings ("Show
Functions Browser", "Hide Functions Browser" sowie deren StatusTips),
alle 476 bestehenden Übersetzungen blieben unverändert erhalten. Nach
Übersetzung ins Deutsche meldet `lrelease`: **480 finished, 0
unfinished**.

**Verifiziert:** Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler.
Zusätzlich per eigenständigem Testprogramm gegen die neu kompilierte
`amigaed_de.qm` geprüft, dass alle vier neuen Strings korrekt ins
Deutsche übersetzt werden.

## Update: Functions-Browser-Shortcuts repariert (F10 funktionierte nicht) + Projekt-Umbenennung zu AmigaED_4

**Problem:** Die in der vorigen Iteration vergebenen Shortcuts F10 /
Shift+F10 (später Ctrl+F10 / Shift+Ctrl+F10) lösten beim Drücken nichts
aus - nur die Auswahl über das Menü per Mausklick funktionierte.

**Diagnose (empirisch, nicht geraten):** Ich habe ein eigenständiges
Testprogramm gebaut, das exakt das reale Szenario nachstellt - ein
`QMainWindow` mit einem `QsciScintilla`-Editor als Zentral-Widget (mit
Fokus, wie beim echten Arbeiten in AmigaED), eine `QAction` mit
`Ctrl+F10`-Shortcut in einem Menü, und `QTest::keyClick()`, um den
Tastendruck offscreen zu simulieren. Ergebnis: Der Shortcut-Mechanismus
selbst (QAction + QActionGroup + Fokus im Scintilla-Editor) funktioniert
einwandfrei - auch `Ctrl+F10` löste im Test zuverlässig aus. Das
schließt einen Fehler im Qt-/QScintilla-Zusammenspiel aus und deutet
stark auf eine **Windows-/Umgebungs-spezifische Blockade von
F-Tasten-Kombinationen** hin (z.B. Fn-Lock auf Notebook-Tastaturen oder
eine andere laufende Software, die F-Tasten-Kombinationen global
abfängt).

**Fix:** Die Shortcuts wurden auf reine Buchstaben-Kombinationen
umgestellt, die kein F-Taste-bezogenes Risiko mehr tragen:
- Show Functions Browser: **Ctrl+Shift+B**
- Hide Functions Browser: **Ctrl+Shift+H**

Zusätzlich wurde `setShortcutContext(Qt::ApplicationShortcut)` auf
beiden Actions gesetzt (statt des Standard-`Qt::WindowShortcut`) - eine
zusätzliche Absicherung, die Aktivierungs-Eigenheiten rund um das aktive
Fenster ausschließt, auch wenn sie im Test nicht die eigentliche Ursache
war. Beide neuen Kombinationen wurden ebenfalls per `QTest::keyClick()`
mit Editor-Fokus verifiziert - inklusive korrektem Umschalten des
Checked-Zustands in der `QActionGroup`.

**Projekt umbenannt:** Der Projektordner wurde von `AmigaED-v3.3` zu
`AmigaED_4` überführt, die Anwendungsversion in `main.cpp`
(`app.setApplicationVersion(...)`) von "3.0" auf "4.0" angehoben.
Historische Code-Kommentare, die auf frühere Versionsnummern verweisen
(z.B. "AmigaED v3.2"/"AmigaED v3.3" als Datierung, wann ein Feature
eingeführt wurde), wurden bewusst NICHT verändert - das sind
Änderungs-Historie, kein aktueller Versionsstand.

**Sprachdateien aktualisiert:** `lupdate` fand 2 neue Strings
("Ctrl+Shift+B", "Ctrl+Shift+H"), die alten "Ctrl+F10"/"Shift+Ctrl+F10"-
Einträge wurden als nicht mehr referenziert entfernt (nicht nur als
"vanished" markiert liegen gelassen). `lrelease` meldet: **482 finished,
0 unfinished**.

**Verifiziert:** Build im neuen Verzeichnis `AmigaED_4` mit `qmake6 +
make` - 0 Warnungen, 0 Fehler.

## Update: m68k-amigaos-gcc/g++ können jetzt AmigaOS 1.3 ansteuern (-mcrt=nix13)

**Hintergrund:** Das Ziel-OS-Gadget in der Statusleiste (`osCombo`, "OS
1.3" / "OS 3.x") war für GCC und G++ dauerhaft ausgegraut - es galt
bisher nur für VBCC. m68k-amigaos-gcc kann jedoch mit dem Schalter
`-mcrt=nix13` ebenfalls für AmigaOS 1.3 übersetzen.

**Prefs-Dialog, Reiter GCC:** Neues Feld "default OS 1.3 opts:" direkt
nach "default g++ opts", Vorgabewert `-Wall -mcrt=nix13`. Ein einzelnes,
gemeinsames Feld für gcc UND g++ (beide nutzen denselben m68k-amigaos-gcc-
Unterbau und denselben `-mcrt`-Mechanismus). Wird über `QSettings`
(`GCC/DefaultOptsOS13`) gespeichert/geladen wie die anderen GCC-
Einstellungen.

**Ziel-OS-Gadget freigeschaltet:** `SelectCompiler()` aktiviert `osCombo`
jetzt für alle drei Compiler (VBCC, GCC, G++), nicht mehr nur für VBCC.

**Compiler-Umbenennungen für Klarheit (funktional erweitert, nicht nur
umbenannt):**
- `setVbccTargetOS(int)` → `setDefaultTargetOS(int)`: reagiert jetzt je
  nach aktuell gewähltem Compiler unterschiedlich - bei VBCC wie bisher
  (`p_compiler_vc13_call`/`vc30_call`), bei GCC/G++ über die neue
  Hilfsmethode `composeGccArgsForCurrentTarget()`. Nebenbei einen
  bestehenden kleinen Inkonsistenz-Bug behoben: die alte Funktion
  aktualisierte `p_compiler_vc_default_target` nicht bei manueller
  Gadget-Bedienung, was bei GCC/G++-Erweiterung zu veralteten Werten in
  der Makefile-Generierung geführt hätte.
- `applyProjectVbccTargetIfNeeded()` → `applyProjectTargetOSIfNeeded()`:
  die VBCC-only-Einschränkung wurde entfernt - ein neues/geladenes
  "AmigaOS 1.3 Project" setzt das Gadget jetzt unabhängig vom aktuell
  gewählten Compiler korrekt auf "OS 1.3".
- Neue Hilfsmethode `composeGccArgsForCurrentTarget(baseArgs)`: hängt
  `p_compiler_gcc13_call` an `baseArgs` an, wenn das Ziel-OS-Gadget auf
  "OS 1.3" steht und das Feld nicht leer ist - sonst wird `baseArgs`
  unverändert zurückgegeben.

**Makefile-Generierung:** `regenerateProjectMakefiles()` befüllt jetzt
auch für `Makefile.gcc` bei einem "AmigaOS 1.3 Project" (`templateKind
== 2`) die Variable `CCARGS` mit den konfigurierten OS-1.3-Optionen -
exakt nach demselben, bereits bewährten Muster wie das feste `+kick13`
bei VBCC (`CCARGS` erscheint sowohl in der Link- als auch in der
Compile-Regel, da `-mcrt=nix13` an beiden Stellen nötig ist).

**Verifiziert:**
- Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler.
- `composeGccArgsForCurrentTarget()` mit einem eigenständigen
  Testprogramm geprüft: OS 1.3 hängt die Zusatzoptionen korrekt an, OS
  3.x lässt die Basisoptionen unverändert, ein leeres OS-1.3-Feld ändert
  nichts.
- Die komplette Makefile-Schreiblogik (identischer Code-Auszug aus
  `regenerateProjectMakefiles()`) mit einem eigenständigen Testprogramm
  für ein simuliertes "AmigaOS 1.3 Project" ausgeführt: das erzeugte
  `Makefile.gcc` enthält `CCARGS = -Wall -mcrt=nix13` korrekt sowohl in
  der Link- als auch in der Compile-Regel, inklusive erklärendem
  Kommentar.

**Sprachdateien aktualisiert:** `lupdate` fand 4 neue Strings, 1 alter
Eintrag ("Change VBCC default target OS") wurde entfernt statt nur als
"vanished" liegen zu lassen. `lrelease` meldet: **485 finished, 0
unfinished**.

## Update: Vorbelegte Compiler-/Linker-Flags beim Erstellen neuer Projekte

Die beim Anlegen eines neuen Projekts abgefragten Dialoge "Compiler
Options" und "Linker Options" (`promptCompilerLinkerOptions()`) sind
jetzt für bestimmte Template-/Compiler-Kombinationen vorbelegt - der
Nutzer kann den Vorschlag weiterhin frei bearbeiten oder löschen, bevor
er bestätigt:

- **"AmigaOS 1.3 Project" + GCC aktiv:** Compiler-Flags werden mit den
  VBCC-"OS 1.3 default opts" aus den Einstellungen (`p_compiler_vc13_call`,
  z.B. `+kick13 -cpp-comments -v -O2 -size -cpu=68020 -I$NDK13_INC
  -L$NDK13_LIB -lauto`) vorbelegt - als praktische, bereits korrekte
  Include-/Lib-Pfad-Basis, die sich der Nutzer zurechtstutzen kann.
  Linker-Flags werden mit `-mcrt=nix13` vorbelegt (der Schalter, der
  m68k-amigaos-gcc erst für OS 1.3 tauglich macht - siehe letztes
  Update).
- **"AmigaOS 1.3 Project" oder "AmigaOS 3.x Project" + VBCC aktiv:**
  Linker-Flags werden mit `-noixemul` vorbelegt.

**Bewusste Einschränkungen (auf Nachfrage bestätigt):**
- Gilt nur für G++ **nicht**, nur für GCC - `-mcrt=nix13`/die
  VBCC-Opts-Vorbelegung greifen ausschließlich bei GCC.
- Gilt wörtlich nur für die beiden Templates "AmigaOS 1.3 Project" und
  "AmigaOS 3.x Project" - **nicht** für "ReAction Project" oder "MUI
  Project", obwohl auch diese AmigaOS-Ziele sind.

**Geänderte Stellen:**
- `promptCompilerLinkerOptions()`: neuer Parameter `int templateKind`,
  berechnet die beiden Vorbelegungen und übergibt sie als Default-Text
  an die beiden `QInputDialog::getText()`-Aufrufe (vierter Parameter,
  vorher jeweils `QString()`).
- `createNewProject()`: übergibt sein eigenes `templateKind` an den
  neuen Parameter.

**Verifiziert:** Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler. Die
Entscheidungslogik wurde zusätzlich mit einem eigenständigen
Testprogramm gegen alle relevanten Template-/Compiler-Kombinationen
geprüft (inkl. der Fälle, die bewusst NICHT vorbelegt werden sollen:
G++, andere Templates, ReAction/MUI) - jede Kombination verhält sich
exakt wie spezifiziert.

Da es sich bei den vorbelegten Werten um reine Compiler-/Linker-Flag-
Literale handelt (keine UI-Beschriftungen), wurden keine neuen
übersetzbaren Strings eingeführt - die Sprachdateien sind unverändert
gültig (weiterhin 485/485 finished).

## Update: Neue Architektur für Default-Compiler-/Linker-Optionen + Dubletten-Eliminierung

**Problem der letzten Iteration:** Die Vorbelegung der "Compiler
Options"-Abfrage bei GCC+AmigaOS1.3-Projekten zeigte fälschlich den
Inhalt der VBCC-OS-1.3-Voreinstellungen an, statt eigener GCC-Werte -
Resultat einer zu ad-hoc geratenen Wiederverwendung fremder Felder.

**Neue Architektur (grundlegend überarbeitet):**

Neue Prefs-Felder:
- **Reiter GCC**, nach "default OS 1.3 opts": "default linker opts:"
  (`lineEdit_GCCdefaultLinkerOpts`, Settings `GCC/DefaultLinkerOpts`,
  Member `p_compiler_gcc_linker_call`) - gemeinsam für gcc/g++, leer per
  Vorgabe.
- **Reiter VBCC**, nach "OS 3.x default opts": "default linker opts:"
  (`lineEdit_VCdefaultLinkerOpts`, Settings `VBCC/DefaultLinkerOpts`,
  Member `p_compiler_vc_linker_call`), Vorgabewert `-noixemul`.

`promptCompilerLinkerOptions()` befüllt beide Dialoge jetzt ausschließlich
aus den Feldern des **aktuell gewählten** Compilers (nie mehr aus einem
fremden Compiler-Feld):
- VBCC: Compiler-Opts = OS-1.3/3.x-Opts (je nach Ziel), Linker-Opts =
  neues VBCC-Linker-Feld.
- GCC: Compiler-Opts = "default gcc opts" (+ OS-1.3-Extras via
  `composeGccArgsForTarget()`), Linker-Opts = neues GCC-Linker-Feld
  (ebenso + OS-1.3-Extras).
- G++: Compiler-Opts = "default g++ opts", Linker-Opts = GCC-Linker-Feld
  (die OS-1.3-Extras gelten laut Spezifikation weiterhin nur für GCC,
  nicht G++).

Das zu verwendende Ziel-OS wird dabei **aus dem gewählten Projekt-Template
abgeleitet** (`templateKind == 2` -> OS 1.3, `== 3` -> OS 3.x), nicht aus
dem gerade in der Statusleiste angezeigten Gadget-Wert - so bleibt die
Vorbelegung auch dann korrekt, wenn Statusleiste und gewähltes
"New Project"-Template zufällig auseinanderlaufen.

**Makefile-Generierung** (`regenerateProjectMakefiles()`) verwendet
dieselben Prefs-Felder jetzt unabhängig von der Dialog-Eingabe als
CCARGS/LDFLAGS-Baseline (für GCC neu: `p_compiler_gcc_call`
wird jetzt - wie bei VBCC seit dem letzten Update bereits üblich -
automatisch in CCARGS geschrieben, nicht mehr nur bei OS1.3-Projekten).

**Dubletten-Eliminierung (neue zentrale Hilfsfunktionen):**
- `dedupTokens(args)`: entfernt doppelte Leerzeichen-getrennte Tokens
  innerhalb eines Strings (erste Vorkommen bleiben erhalten).
- `removeTokensAlsoIn(text, reference)`: entfernt aus `text` alle Tokens,
  die auch in `reference` vorkommen - ohne die Strings zu verschmelzen.

Damit werden Schalter, die sowohl in der (jetzt automatisch injizierten)
Baseline (CCARGS) als auch in den vom Nutzer bestätigten
Projekt-Extra-Optionen (CFLAGS/LDFLAGS) stehen - was durch die neue
Vorbelegung sehr häufig der Fall ist, wenn der Nutzer den Vorschlag
einfach übernimmt - zuverlässig auf ein einziges Vorkommen reduziert.
Gilt für beide Toolchains (GCC und VBCC) sowie für Dubletten *innerhalb*
einer einzelnen zusammengesetzten Optionszeile (z.B. wenn sowohl die
Basis-Opts als auch die OS-1.3-Extras unabhängig voneinander "-Wall"
enthalten).

**Verifiziert:**
- Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler.
- Kompletter End-to-End-Workflow (Dialog-Vorbelegung -> Nutzer bestätigt
  unverändert -> Makefile-Generierung) für GCC+OS1.3 UND VBCC+OS1.3 mit
  eigenständigen Testprogrammen simuliert: finale CFLAGS/LDFLAGS sind in
  beiden Fällen korrekt leer (da alles bereits über CCARGS abgedeckt
  ist), keine Dubletten in den resultierenden Kommandozeilen.
- Grenzfälle geprüft: G++ (OS-1.3-Extras greifen bewusst nicht), GCC+OS
  3.x (Extras greifen bewusst nicht), sowie der Fall, dass der Nutzer
  eine echte projektspezifische Zusatzoption zur Vorbelegung hinzufügt
  (bleibt korrekt als einziger Eintrag in CFLAGS erhalten).

**Sprachdateien aktualisiert:** `lupdate` fand 4 neue Strings, `lrelease`
meldet: **489 finished, 0 unfinished**.

## Kritischer Bugfix: Absturz beim Programmstart (Speicherzugriffsfehler)

**Symptom:** Das Programm stürzte unter Windows und Linux (Qt Creator
Release-Build) unmittelbar beim Start ab, ohne dass ein Fenster
erschien.

**Ursache:** Mehrere `QAction`/`QMenu`/`QActionGroup`-Zeiger, die in den
letzten Updates (GUI-Language-Menü, Functions-Browser-Sichtbarkeit)
als Member zu `MainWindow` hinzugefügt wurden
(`guiLanguageMenue`, `guiLanguageEnglishAct`, `guiLanguageGermanAct`,
`guiLanguageGroup`, `showFunctionsBrowserAct`, `hideFunctionsBrowserAct`,
`functionsBrowserGroup`), waren **ohne `= nullptr`-Initialisierer**
deklariert - anders als das bereits etablierte Muster im Projekt
(`recentFilesMenue`/`recentProjectsMenue`, die genau aus diesem Grund
bereits `= nullptr` hatten).

Da `MainWindow::readSettings()` als **allererste Zeile** im Konstruktor
läuft - noch bevor `createActions()` diese Zeiger überhaupt zuweist -
prüft `readSettings()`/`applyGuiLanguage()` dort `if (guiLanguageEnglishAct)`,
um zu erkennen, ob das Menü schon existiert. Ohne Initialisierer enthält
dieser Zeiger zu diesem Zeitpunkt **Zufallsspeicher** (undefined
behaviour) statt zuverlässig `nullptr`. Ist die Oberflächensprache
bereits einmal auf Deutsch umgestellt und gespeichert worden (`"de"` in
den Einstellungen), wird `applyGuiLanguage("de")` unbedingt aufgerufen
und dereferenziert dabei diesen Zufallszeiger (`guiLanguageEnglishAct->
setChecked(...)`) sowie - über den anschließenden Aufruf von
`retranslateUi()` - Dutzende weiterer, zu diesem Zeitpunkt noch gar
nicht existierender Widget-Zeiger. Das erklärt auch, warum der Fehler
erst jetzt auftrat: er tritt nur auf, wenn zuvor mindestens einmal auf
Deutsch umgeschaltet wurde (wodurch `"de"` dauerhaft gespeichert ist),
und äußert sich - typisch für undefined behaviour durch
uninitialisierten Stack-Speicher - nichtdeterministisch, in Release-
Builds tendenziell häufiger als in Debug-Builds.

**Fix:** Alle betroffenen Zeiger-Member erhalten jetzt konsequent
`= nullptr` als Initialisierer, exakt nach dem bereits im Projekt
etablierten Muster. Zusätzlich `projectGroupBox` und `functionsGroupBox`
(ebenfalls kürzlich zu Membern gemacht) vorsorglich mit `= nullptr`
abgesichert.

**Verifiziert (empirisch, nicht nur Code-Review):**
- Den Bug gezielt reproduziert: mit einer gespeicherten Einstellung
  `MISC/DefaultGUILanguage=de` (exakt das reale Szenario) stürzte die
  *unreparierte* Version bei 10 Testläufen offscreen in 2 von 5 Fällen
  mit Exit-Code 139 (SIGSEGV) ab - nichtdeterministisch, wie für
  undefined behaviour typisch.
- Nach dem Fix: **0 von 10 Läufen** abgestürzt, mit derselben
  gespeicherten deutschen Spracheinstellung.
- Zusätzlich das Erststart-Szenario (keine gespeicherten Einstellungen
  vorhanden) mit 3 Läufen geprüft - ebenfalls stabil.
- Build mit `qmake6 + make` - weiterhin 0 Warnungen, 0 Fehler.

## Update: Compiler-/Linker-Optionen für GCC und VBCC getrennt (großes Redesign)

**Problem der letzten Iteration:** Die Vorbelegung bei GCC+AmigaOS1.3
zeigte zwar nach der Reparatur die richtigen GCC-eigenen Werte an, aber
das zugrundeliegende Datenmodell (je EIN gemeinsames Feld pro Compiler,
plus ein separates "OS 1.3 Extra"-Feld) war unnötig kompliziert und
unterschied nicht zwischen Compiler- und Linker-Belangen. Auf Wunsch
komplett neu aufgebaut.

**Neue Prefs-Struktur - 12 Felder statt bisher 6:**

Reiter GCC (8 Felder, für GCC und G++ je einzeln):
- GCC OS 1.3 compiler opts / GCC OS 3.x compiler opts
- GCC OS 1.3 linker opts / GCC OS 3.x linker opts
- G++ OS 1.3 compiler opts / G++ OS 3.x compiler opts
- G++ OS 1.3 linker opts / G++ OS 3.x linker opts

Reiter VBCC (4 Felder; VBCC hat nur einen Compiler-Frontend, daher keine
GCC/G++-Aufteilung nötig):
- VBCC OS 1.3 compiler opts / VBCC OS 3.x compiler opts (bestehende
  Felder, aber um die reinen Linker-Anteile wie `-L...`/`-lauto`
  bereinigt)
- VBCC OS 1.3 linker opts / VBCC OS 3.x linker opts (neu)

Die alten 6 Widgets wurden wo möglich recycelt (Objektname/Label
geändert, Settings-Key neu), 6 weitere neu ergänzt - alte Settings-Werte
gehen dabei verloren (Architekturwechsel), die Vorgabewerte sind aber
sinnvoll vorbelegt.

**Wichtige Detailentscheidung:** `-mcrt=nix13` landet in den
**Compiler**-Opts (nicht Linker-Opts), da dieser gcc-Schalter auch die
Codegenerierung beeinflusst und daher beim Compilieren präsent sein
muss, nicht nur beim Linken - über CCARGS wirkt er dadurch korrekt bei
beiden Schritten. `-noixemul`/`-lamiga` (reine Linker-Belange) liegen
dagegen in den Linker-Opts.

**Zentrale Lookup-Funktion:** `getCompilerAndLinkerOptsForTarget(compiler,
targetOS, compilerOpts, linkerOpts)` ist jetzt die EINZIGE Quelle der
Wahrheit für alle drei Verwendungsstellen:
- `SelectCompiler()`/`setDefaultTargetOS()`: Einzeldatei-Compilierung
  kombiniert jetzt Compiler- UND Linker-Opts (vorher fehlten bei VBCC
  die Linker-Opts hier - stiller Lücke, die durch die Neustrukturierung
  mit behoben wurde).
- `promptCompilerLinkerOptions()`: befüllt "Compiler Options"/"Linker
  Options" beim Projekterstellen getrennt vor.
- `regenerateProjectMakefiles()`: Compiler-Opts -> CCARGS, Linker-Opts
  -> LDFLAGS, für beide Toolchains (GCC UND VBCC jetzt symmetrisch,
  vorher hatte nur VBCC eine funktionierende CCARGS-Befüllung).

Das Ziel-OS wird dabei konsistent aus dem gewählten Projekt-Template
abgeleitet (`templateKind == 2` -> OS 1.3, `== 3` -> OS 3.x, sonst
aktueller Gadget-Wert) - identische Logik in `promptCompilerLinkerOptions()`
und `regenerateProjectMakefiles()`, sodass das, was der Nutzer im Dialog
bestätigt, exakt dem entspricht, was im Makefile landet.

Die bereits bestehende Dubletten-Eliminierung (`dedupTokens()`,
`removeTokensAlsoIn()`) greift unverändert weiter.

**Verifiziert:**
- Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler.
- Alle 6 Compiler×Ziel-OS-Kombinationen (VBCC/GCC/G++ × OS1.3/OS3.x) per
  eigenständigem Testprogramm geprüft: Dialog-Vorbelegung und
  Einzeldatei-Compile-Argumente sind für jede Kombination korrekt und
  sauber nach Compiler/Linker getrennt.
- Vollständige Makefile-Simulation für GCC+OS1.3 und VBCC+OS1.3: CCARGS
  enthält korrekt nur Compiler-Opts (inkl. `-mcrt=nix13`/`+kick13`),
  LDFLAGS korrekt nur Linker-Opts, keine Dubletten zwischen den beiden.
- Absturz-Regressionstest wiederholt (siehe vorheriger Bugfix-Eintrag):
  10 Programmstarts mit gespeicherter deutscher Spracheinstellung - 0
  Abstürze.

**Sprachdateien aktualisiert:** `lupdate` fand 33 neue Strings, 15 alte
Einträge (aus den 6 abgelösten Feldern) wurden vollständig entfernt statt
nur als "vanished" liegen zu lassen. `lrelease` meldet: **507 finished,
0 unfinished**.

## Update: SAS/C-Unterstützung (Makefile.sc)

**Neuer Prefs-Tab "SAS/C"**, direkt nach dem VBCC-Tab eingefügt: ein
Label "SAS/C default opts:" + Eingabefeld (`lineEdit_SASCdefaultOpts`,
Settings `SASC/DefaultOpts`, Member `p_compiler_sc_call`), Vorgabewert
`ICONS ERRORREXX`. Anders als GCC/VBCC gibt es **keinen** Pfad zu einem
SAS/C-Compiler-Executable in den Einstellungen - SAS/C wird von AmigaED
nie selbst aufgerufen (siehe unten).

**Makefile.sc:** `regenerateProjectMakefiles()` erzeugt jetzt zusätzlich
zu `Makefile.gcc`/`Makefile.vbcc` ein drittes Makefile für SAS/C, sofern
das Projekt mindestens eine reine `.c`-Datei enthält (SAS/C unterstützt
kein C++, daher werden `.cpp`/`.cc`/`.cxx`-Dateien für dieses Makefile
konsequent ausgeschlossen - ein reines C++-Projekt bekommt entsprechend
kein `Makefile.sc`). Format orientiert sich am vom Nutzer vorgegebenen
Beispielaufruf `sc ICONS ERRORREXX programm.c LINK TO programm.`:

```makefile
SCOPTS = ICONS ERRORREXX
TARGET = ProjektName
SRCS   = datei1.c datei2.c

all: $(TARGET)

$(TARGET): $(SRCS)
	sc $(SCOPTS) $(SRCS) LINK TO $(TARGET)

clean:
	delete $(TARGET)
```

Bewusste Vereinfachungen (wie vom Nutzer vorgegeben):
- **Keine Trennung zwischen Compiler- und Linker-Optionen** und **keine
  OS-1.3/3.x-Unterscheidung** - anders als bei GCC/VBCC nur EIN
  gemeinsames Optionsfeld, da `sc` Compilieren und Linken in einem
  einzigen Aufruf erledigt.
- **`clean` nutzt AmigaDOS-Syntax** (`delete` statt `rm -f`), da dieses
  Makefile ausschließlich für die manuelle Nutzung auf einem echten
  Amiga oder Emulator gedacht ist (z.B. via `smake`).
- **AmigaED ruft `sc` nie selbst auf** - weder "Build Project" noch
  "Clean Project" fassen `Makefile.sc` an; es wird ausschließlich
  generiert, nicht ausgeführt. SAS/C erscheint dementsprechend auch
  nicht in der Compiler-Auswahl (`SelectCompiler`/Ziel-OS-Gadget) und
  fließt nicht in die Vollständigkeitsprüfung der Compiler-Einstellungen
  ein.

**Projektbaum:** `refreshProjectTree()` zeigt `Makefile.sc` jetzt
ebenfalls in der "Makefiles"-Kategorie an, sofern es auf der Festplatte
existiert - dieselbe generische, dateisystembasierte Logik wie schon für
`Makefile.gcc`/`Makefile.vbcc`, lediglich um einen dritten Namen
erweitert.

**Verifiziert:**
- Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler.
- Die komplette Makefile.sc-Schreiblogik (identischer Code-Auszug) mit
  einem eigenständigen Testprogramm ausgeführt: erzeugtes Makefile
  entspricht exakt dem vom Nutzer vorgegebenen Format und Beispielaufruf.
- Die C++-Ausschlusslogik separat geprüft: aus einem gemischten
  C/C++-Projekt werden nur die `.c`-Dateien übernommen; ein reines
  C++-Projekt liefert eine leere Liste (kein `Makefile.sc`).

**Sprachdateien aktualisiert:** `lupdate` fand 5 neue Strings, `lrelease`
meldet: **512 finished, 0 unfinished**.

## Update: About-Dialog Text aktualisiert (Reiter "About")

Der englische Text im About-Dialog wurde durch den vom Nutzer
vorgegebenen HTML-Inhalt ersetzt - u.a. Umbenennung von "Amiga Cross
Editor (AcED)" zu "AmigaED v4.0", "Amiga/Aros application development"
zu "AmigaOS application development", sowie aktualisiertes HTML-Markup
(Zeichensatz-Meta-Tag, `hr`/Listen-Marker-Stile im `<style>`-Block,
Schriftart "Segoe UI" für den Fließtext). Direkt in
`aboutdialog.ui` (`textBrowser`, Reiter "About") ersetzt.

**Deutsche Übersetzung ergänzt:** vollständig neu erstellt, mit
identischer HTML-Struktur/Markup wie das englische Original (gleiche
`<span>`-Aufteilung, gleiche Inline-Styles) - nur der Fließtext ist
übersetzt, "AmigaED v4.0", "Qt5" und "QScintilla" bleiben unverändert
(Produktnamen/Eigennamen).

**Verifiziert:**
- Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler.
- Laufzeit-Übersetzung gegen die kompilierte `.qm` per eigenständigem
  Testprogramm geprüft: die deutsche Übersetzung wird korrekt gefunden
  und enthält die erwarteten übersetzten Textbausteine ("fehlende
  Verbindung", "AmigaOS-Anwendungsentwicklung", "QScintilla-Quellen"),
  während "AmigaED v4.0" unverändert bleibt.

**Sprachdateien aktualisiert:** `lupdate` fand den geänderten Text als 1
neuen String, der alte wurde vollständig entfernt statt nur als
"vanished" liegen zu lassen. `lrelease` meldet: **512 finished, 0
unfinished**.

## Bugfix: Bereits geöffnetes Makefile.sc (und Makefile.gcc/.vbcc) wurde nach Regenerierung nicht aktualisiert

**Gemeldetes Problem:** Nach dem Hinzufügen einer weiteren C-Datei zu
einem bestehenden Projekt wurde `Makefile.sc` "nicht erneuert".

**Diagnose:** Die eigentliche Regenerierungs-Logik selbst funktionierte
bereits korrekt - per eigenständigem Testprogramm mit der echten
`Project`-Klasse verifiziert: `Project::addFile()` + die
`regenerateProjectMakefiles()`-Logik für SAS/C erkennen eine neu
hinzugefügte `.c`-Datei zuverlässig und schreiben die aktualisierte
Datei-auf-Festplatte korrekt neu. Der tatsächliche Fehler lag woanders:
`openFileInTab()` liest beim Öffnen einer bereits in einem Tab
geöffneten Datei **nicht neu von der Festplatte** - es wechselt nur zum
bestehenden Tab (Duplikat-Vermeidung). Wer `Makefile.sc` also schon
einmal geöffnet hatte (z.B. um es sich anzusehen), sah nach dem
Hinzufügen einer Datei weiterhin den alten Tab-Inhalt, obwohl die Datei
auf der Festplatte bereits korrekt aktualisiert war - exakt das
gemeldete Symptom.

**Fix:** Neue Methode `reloadEditorFromDiskIfOpen(fileName)` - prüft,
ob die angegebene Datei gerade in einem Tab offen ist, und aktualisiert
in diesem Fall gezielt NUR diesen einen Tab-Inhalt von der Festplatte
(ohne Tab-Wechsel, ohne andere Tabs zu berühren). `regenerateProjectMakefiles()`
ruft das jetzt nach jedem erfolgreichen Schreibvorgang auf - für
`Makefile.gcc`, `Makefile.vbcc` UND `Makefile.sc` gleichermaßen (der
Bug betraf potenziell alle drei, nicht nur SAS/C). Da diese Makefiles
laut eigenem Dateikopf-Kommentar "regenerated automatically" sind und
nicht zum Von-Hand-Bearbeiten gedacht sind, erfolgt die Aktualisierung
bewusst ohne Rückfrage.

**Nebenbei behobene Reihenfolge-Inkonsistenz:** In 5 der 7 Aufrufstellen
lief `refreshProjectTree()` vor `regenerateProjectMakefiles()` - obwohl
ein bereits bestehender Code-Kommentar an anderer Stelle explizit
"must run BEFORE refreshProjectTree() - it scans disk for which
Makefiles exist" dokumentiert. Alle Aufrufstellen (Datei hinzufügen,
Datei entfernen, als Hauptdatei setzen, Kontextmenü "Remove from
project") wurden auf die dokumentierte, korrekte Reihenfolge
vereinheitlicht.

**Verifiziert:**
- Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler.
- Die reine Regenerierungs-Logik (ohne Tab-Reload) mit der echten
  `Project`-Klasse getestet: Hinzufügen einer zweiten `.c`-Datei wird
  korrekt in `Makefile.sc` übernommen (`SRCS = main.c utils.c`).
- Die neue `reloadEditorFromDiskIfOpen()`-Logik separat mit einem
  echten `QsciScintilla`-Editor getestet: Tab-Inhalt bleibt vor dem
  Reload-Aufruf wie erwartet veraltet, wird durch den Aufruf korrekt
  aktualisiert, `isModified()` ist danach `false`.
- Keine neuen übersetzbaren Strings eingeführt (reine Bugfix-/interne
  Änderungen) - Sprachdateien unverändert gültig (weiterhin 512/512
  finished).

## Bugfix: "Erfolgreicher Build" trotz fehlender ausführbarer Datei

**Gemeldetes Problem:** Weder das Makefile für GCC noch für VBCC
erzeugt eine ausführbare Datei, obwohl die Statusleiste einen
erfolgreichen Build meldet.

**Root Cause gefunden:** Der Code enthielt bereits einen expliziten
Kommentar, der das Problem bestätigt - der Build-Erfolg für "Build
Project"/"Clean Project" wurde **ausschließlich** anhand des Exit-Codes
von `make` bestimmt, nie anhand der tatsächlichen Existenz der
erzeugten Datei. Läuft `make` durch (Exit-Code 0), meldet AmigaED
"erfolgreich" - selbst wenn der zugrunde liegende Compiler-Aufruf aus
irgendeinem Grund (fehlerhafter Compiler-Pfad, stille
Fehler/Warnungen, veraltete Zeitstempel, die `make` zur Annahme
verleiten, das Ziel sei bereits aktuell, o.ä.) keine Datei erzeugt hat.

**Verifiziert per echtem `make`-Lauf:** Mit einem absichtlich
"kaputten" Fake-Compiler (gibt Exit-Code 0 zurück, schreibt aber
nichts) reproduziert: `make -f Makefile.gcc all` meldet Exit-Code 0,
obwohl keine `TestProj`-Datei entsteht - exakt das gemeldete Verhalten.
Die eigentliche Makefile-Struktur selbst wurde separat mit einem
funktionierenden Fake-Compiler verifiziert und arbeitet korrekt.

**Fix:** Neues Member `p_expectedProjectBuildTarget` - `actionBuildProject()`
berechnet vor dem Start den erwarteten Pfad der ausführbaren Datei
(identische Namensableitung wie in `regenerateProjectMakefiles()`).
`stopCommand()` prüft nach einem project build mit Exit-Code 0
zusätzlich, ob diese Datei tatsächlich existiert - falls nicht, wird
jetzt eine ehrliche Warnung angezeigt ("make" reported success, but no
executable was created ...) statt der irreführenden Erfolgsmeldung, und
die Compiler-Ausgabekonsole wird automatisch eingeblendet. Für "Clean
Project" bleibt die Prüfung deaktiviert (`p_expectedProjectBuildTarget`
wird dort geleert), da nach dem Aufräumen ja gerade *keine* Datei mehr
existieren soll.

**SAS/C-Hinweis:** `Makefile.sc` wird von AmigaED nie selbst ausgeführt
(siehe vorheriges Update) - für SAS/C gibt es daher keine "Statusleiste
zeigt Erfolg"-Meldung, die geprüft werden könnte. Die generierte
`Makefile.sc`-Syntax wurde bereits gegen das vom Nutzer vorgegebene
Beispiel verifiziert; sollte beim manuellen Bauen mit SAS/C auf einem
echten Amiga/Emulator weiterhin keine ausführbare Datei entstehen,
wäre das ein separates, eigenständiges Problem (vermutlich in der
tatsächlichen `sc`-Umgebung, nicht im generierten Makefile selbst).

**Verifiziert:**
- Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler.
- Die neue Verifikationslogik mit 5 Szenarien per eigenständigem
  Testprogramm geprüft: erfolgreicher Build mit echter Datei, Exit 0
  ohne Datei (der gemeldete Bug), Absturz, Exit ungleich 0, sowie Clean
  (wo die Prüfung korrekt deaktiviert bleibt) - alle fünf Fälle liefern
  die erwartete Meldung.
- End-to-End mit echtem `make` + defektem Fake-Compiler reproduziert:
  bestätigt exakt das gemeldete Symptom vor dem Fix.

**Sprachdateien aktualisiert:** `lupdate` fand 1 neuen String,
`lrelease` meldet: **513 finished, 0 unfinished**.

## Bugfix: VBCC-Flags landeten in Makefile.gcc (und umgekehrt)

**Gemeldetes Problem:** Beim Bau eines Testprojekts mit gcc erschien
`collect2: error: ld returned 1 exit status`. Das generierte
`Makefile.gcc` enthielt `CFLAGS = +aos68k -v -c99 -cpu=68030 -O3 -size
-final` - eindeutig VBCC-Syntax, keine gültigen gcc-Optionen.

**Root Cause:** `Project::extraCompilerOptions`/`extraLinkerOptions`
waren **einzelne, projektweite Felder** - ursprünglich aus einer
früheren, einfacheren Designphase, bevor Compiler-/Ziel-OS-spezifische
Baselines eingeführt wurden. Die "New Project"-Dialoge (Compiler
Options/Linker Options) sind zwar seit einigen Updates
Compiler-bewusst (befüllen sich abhängig vom aktuell gewählten
Compiler vor) - aber das Ergebnis landete in genau EINEM gemeinsamen
Feld, das anschließend **blind auf beide Makefiles** (`Makefile.gcc`
UND `Makefile.vbcc`) angewendet wurde. Wurde ein Projekt bei aktivem
VBCC erstellt, bestätigte der Nutzer die VBCC-Vorbelegung im Dialog -
und dieser VBCC-spezifische Text wurde anschließend auch unverändert
in `Makefile.gcc`s `CFLAGS` übernommen, da die Dedup-Logik nur
Dubletten mit der GCC-eigenen Baseline erkennt, nicht aber "gehört
das hier überhaupt zu gcc?".

**Fix:** `Project` besitzt jetzt vier statt zwei Felder - konsequent
pro Toolchain getrennt:
- `extraGccCompilerOptions` / `extraGccLinkerOptions`
- `extraVbccCompilerOptions` / `extraVbccLinkerOptions`

(G++ nutzt dieselben "Gcc"-Felder wie GCC, da `Makefile.gcc` ohnehin
immer gcc selbst aufruft, nie g++.) `createNewProject()` schreibt die
Dialog-Antworten jetzt in genau das zum aktuell gewählten Compiler
passende Feldpaar - das jeweils andere Feldpaar bleibt leer, da der
Nutzer dazu nie befragt wurde. `regenerateProjectMakefiles()` liest für
`Makefile.gcc` ausschließlich die Gcc-Felder, für `Makefile.vbcc`
ausschließlich die Vbcc-Felder - keine Vermischung mehr möglich.

**Verifiziert:**
- Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler.
- Das exakt gemeldete Szenario (Projekt bei VBCC erstellt, dann
  Makefile.gcc generiert) per eigenständigem Testprogramm nachgestellt:
  `CFLAGS` in `Makefile.gcc` ist jetzt leer/sauber, enthält weder
  `+aos68k` noch `-c99` oder andere VBCC-Flags.
- Persistenz der vier getrennten Felder mit der echten `Project`-Klasse
  geprüft: Speichern und Laden eines `.aep` erhält alle vier Werte
  korrekt und unvermischt.
- Ein komplettes, realistisches `Makefile.gcc` (inkl. `TARGET = sctest`,
  passend zum Namen aus der Fehlermeldung) generiert und inspiziert -
  `CFLAGS` ist leer, keine VBCC-Syntax mehr enthalten.

**Hinweis für bereits existierende Projekte:** Da sich die
Settings-Keys in der `.aep`-Datei geändert haben
(`ExtraCompilerOptions`/`ExtraLinkerOptions` → vier neue,
toolchain-spezifische Keys), gehen bei älteren, bereits gespeicherten
Projekten die bisherigen Extra-Optionen beim nächsten Laden verloren
(genau wie bei den vorherigen Architektur-Updates) - sie müssten über
"Add..."/die Projekteinstellungen einmalig neu vergeben werden, sollte
das relevant sein.

**Keine neuen übersetzbaren Strings** - Sprachdateien unverändert
gültig (weiterhin 513/513 finished).

## Bugfix: "-lamiga"/"-lauto" in GCC-Linker-Opts verursachten Link-Fehler

**Gemeldetes Problem:** `collect2: error: ld returned 1 exit status` bei
einem einfachen GCC-Testprojekt (1 C-Datei). Das generierte
`Makefile.gcc` enthielt `LDFLAGS = -lamiga -lauto -noixemul`.

**Root Cause recherchiert (nicht geraten):** `-lamiga` und `-lauto`
sind **VBCC-spezifische Bibliotheksnamen** (`amiga.lib`/`auto.lib`,
klassische VBCC/SAS-C-Konvention - vgl. vbcc-Handbuch: "Specify
'-lamiga' to link with 'amiga.lib'... '-lauto' or '-lautos' ... to
link with 'auto.lib'"). Für bebbos m68k-amigaos-gcc-Toolchain
(erkennbar am Pfad `D:/amiga-gcc/`) existieren diese Bibliotheken in
dieser Form nicht - die offizielle Dokumentation zeigt selbst für
Programme mit echten AmigaOS-Bibliotheksaufrufen
(`OpenLibrary()`/`Write()`/`CloseLibrary()`) nur `-noixemul` als
nötige Linker-Option, ganz ohne `-lamiga`/`-lauto`.

**Das war mein eigener Fehler:** Der von mir gesetzte **Vorgabewert**
für "GCC OS 1.3/3.x linker opts" (und die G++-Pendants) lautete
fälschlich `-noixemul -lamiga` - ich hatte "-lamiga" versehentlich mit
VBCC-Konventionen vermischt, statt mich auf die tatsächliche
bebbo-gcc-Dokumentation zu stützen.

**Fix:** Alle vier GCC/G++-Linker-Opts-Vorgabewerte
(`lineEdit_GCC13LinkerOpts`, `lineEdit_GCC30LinkerOpts`,
`lineEdit_GPP13LinkerOpts`, `lineEdit_GPP30LinkerOpts`) auf schlicht
`-noixemul` korrigiert. Die Tooltips wurden um einen erklärenden
Hinweis ergänzt: *"-lamiga/-lauto are VBCC library names
(amiga.lib/auto.lib) and do not apply to m68k-amigaos-gcc."*

**Für bereits konfigurierte Installationen:** Da dies nur die
**Vorgabewerte** in der `.ui`-Datei betrifft, wirkt sich der Fix nur
auf neu erstellte Prefs-Dateien (bzw. leere Felder) aus - wer die
GCC-Linker-Opts-Felder bereits selbst mit `-lamiga`/`-lauto` befüllt
hat (wie im gemeldeten Fall), muss dies einmalig manuell in
Einstellungen > GCC auf `-noixemul` korrigieren.

**Sprachdateien aktualisiert:** `lupdate` fand 5 geänderte Tooltip-
Strings (neuer Hinweistext), die 5 alten Varianten wurden vollständig
entfernt statt nur als "vanished" liegen zu lassen. `lrelease` meldet:
**513 finished, 0 unfinished**.

**Verifiziert:** Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler.

## Bugfix: Makefiles landeten in "Installer Scripts" beim versehentlichen Hinzufügen zum Projekt

**Gemeldetes Problem:** Beim Bearbeiten und Speichern eines Makefiles im
Editor fragt AmigaED, ob es zum Projekt hinzugefügt werden soll (obwohl
es ja schon "drin" ist, als auto-generierte Datei). Bei Bestätigung
landet es fälschlich unter "Installer Scripts", und `make -f
Makefile.gcc` schlägt danach mit `missing separator. Stop.` fehl.

**Zwei getrennte Ursachen gefunden:**

1. **`maybeOfferAddToProject()`** (nach jedem Speichern aufgerufen)
   fragte für **jede** nicht in `currentProject->files` getrackte
   Datei nach - auch für die drei auto-generierten Makefiles
   (`Makefile.gcc`/`Makefile.vbcc`/`Makefile.sc`), obwohl diese laut
   eigenem Dateikopf-Kommentar "regenerated automatically" sind und nie
   als reguläre Projektdatei gedacht waren (sie werden bereits über die
   eigene "Makefiles"-Kategorie im Baum angezeigt, unabhängig von
   `Project::files`).

2. **Kategorisierungs-Bug in `refreshProjectTree()`:** Jeder Dateityp
   außer `CSource` und `Header` fiel als **Standardfall** in den
   "Installer Scripts"-Zweig - auch `ProjectFileType::Other`, dem
   `Makefile.gcc` zugeordnet wird (die Dateiendung ".gcc" passt zu
   keinem bekannten Muster). Es gab schlicht keine eigene Kategorie für
   "alles andere".

**Zur gemeldeten Tab-Korruption:** `saveFile()` schreibt nachweislich
nur den rohen Editor-Inhalt 1:1 (`out << textEdit->text();`), keine
Tab-zu-Leerzeichen-Umwandlung. Die vermutliche Ursache: Beim Bearbeiten
im Editor wurde eine neue/geänderte Zeile eingefügt, deren Einrückung
nicht dem für `make` zwingend erforderlichen echten Tab-Zeichen
entsprach. Da `Makefile.gcc` durch Fix 1 nie mehr als reguläre
Projektdatei getrackt wird, bleibt es außerdem "nur" eine temporäre
Override-Situation bis zur nächsten Regenerierung (z.B. durch
Hinzufügen/Entfernen einer Datei) - `regenerateProjectMakefiles()`
schreibt den Inhalt dabei ohnehin komplett neu und stellt korrekte
Tabs automatisch wieder her.

**Fix:**
- `maybeOfferAddToProject()`: prüft jetzt zusätzlich, ob die
  gespeicherte Datei im Projektverzeichnis liegt UND einer der drei
  bekannten Makefile-Namen ist - falls ja, wird die Abfrage komplett
  übersprungen.
- Neue Kategorie **"Other Files"** im Projektbaum
  (`projectOtherGroupItem`) - `ProjectFileType::Other` fällt jetzt
  dorthin statt in "Installer Scripts". `ProjectFileType::InstallerScript`
  bekam einen expliziten eigenen Zweig in der Zuordnungslogik (vorher
  implizit der "sonst alles"-Fall).

**Verifiziert:**
- Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler.
- `Project::typeForFile()` für alle relevanten Namen geprüft (`main.c`,
  `header.h`, `Install`, `Install.installer`, `Makefile.gcc/.vbcc/.sc`,
  `README.txt`) - Makefiles klassifizieren korrekt als `Other`.
- Die neue Ausschlusslogik in `maybeOfferAddToProject()` mit 6
  Testfällen geprüft (inkl. eines Makefiles AUSSERHALB des
  Projektverzeichnisses, das weiterhin korrekt nachgefragt wird) - alle
  Fälle korrekt.
- Programmstart 5x offscreen wiederholt - 0 Abstürze (Regressionscheck
  nach dem neuen `projectOtherGroupItem`-Member).

**Sprachdateien aktualisiert:** `lupdate` fand 1 neuen String ("Other
Files"), `lrelease` meldet: **514 finished, 0 unfinished**.

## Update: Compiler-Name in Build-Statusmeldungen

Alle build-/compile-bezogenen Meldungen in der Statusleiste nennen jetzt
den gerade verwendeten Compiler als Präfix, z.B. `gcc: Building project
"sctest"...` oder `vbcc: Project build finished successfully.`

**Neue Hilfsfunktion `compilerDisplayLabel(int compiler)`** liefert den
kurzen Anzeigenamen ("vbcc"/"gcc"/"g++") für die Statusleiste - separat
von den ausführlicheren Namen in der Compiler-Auswahl-ComboBox
("VBCC - C" usw.).

**Neues Member `p_lastRunCompilerLabel`:** wird beim **Start** eines
Build-/Clean-/Einzeldatei-Compile-Vorgangs (`actionBuildProject()`,
`actionCleanProject()`, `startCompiler()`) einmalig festgehalten und
für alle zugehörigen Meldungen bis zum Abschluss desselben Laufs
verwendet - so bleibt die Anzeige auch dann korrekt, falls die
Compiler-Auswahl sich (theoretisch) ändert, während ein Lauf noch im
Hintergrund läuft.

**Betroffene Meldungen** (Präfix `%1: ` ergänzt):
- "Building project ..." / "Cleaning project ..."
- "Compiler run has been started."
- "Project build failed (exit code %1) ..."
- "\"make\" reported success, but no executable was created ..."
- "Project build finished successfully."
- "File compiled." / "There where errors..." / "Compiler error!"

**Bewusst nicht angefasst:** die (bereits vor diesem Update bestehende,
irreführend benannte) Meldung "Compiler run has been started." in
`startEmulator()` - die startet tatsächlich den UAE-Emulator, nicht den
Compiler, und liegt außerhalb des Aufgabenbereichs dieses Updates.
Ebenso unangetastet: die als "Convenience method - will never really be
executed..." dokumentierte, nicht erreichbare `finished()`-Funktion.

**Verifiziert:**
- Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler.
- Alle Nachrichtenformate für alle drei Compiler (vbcc/gcc/g++) per
  eigenständigem Testprogramm geprüft - liefern exakt das gewünschte
  Format.
- Deutsche Übersetzung zur Laufzeit gegen die kompilierte `.qm`
  getestet - Platzhalter (`%1`/`%2`) werden korrekt ersetzt.

**Sprachdateien aktualisiert:** `lupdate` fand 9 neue Strings, 7 alte
Varianten wurden vollständig entfernt statt nur als "vanished" liegen
zu lassen. `lrelease` meldet: **516 finished, 0 unfinished**.

## Update: "Save Project"-Menüpunkt + Drag'n'Drop für die Projektverwaltung

### 1. Neuer Menüpunkt "Save Project"

File-Menü, direkt nach "Load Project...". Speichert die Dateiliste und
Einstellungen des aktuellen Projekts explizit in dessen `.aep` - ein
Sicherheitsnetz zusätzlich zum bereits bestehenden Auto-Save (das nach
jeder projektverändernden Aktion sofort greift).

**Echtes Dirty-Tracking:** Neues Member `p_projectModified`, gesetzt
über `markProjectModified()` und zurückgesetzt über die neue zentrale
`saveCurrentProject()`-Methode (Speicher-Engpass für ALLE
Projekt-Speichervorgänge, ob automatisch oder explizit). Alle fünf
bereits bestehenden projektverändernden Stellen (Datei hinzufügen,
entfernen, als Hauptdatei setzen, Kontextmenü "Remove from project",
sowie die Nachfrage beim Speichern einer noch nicht getrackten Datei)
wurden darauf umgestellt. Beim Erstellen oder Laden eines Projekts wird
der Dirty-Status korrekt zurückgesetzt (frisch erstellt/geladen = nichts
ungespeichert). Klick auf "Save Project" ohne Änderungen zeigt "Projekt
ist bereits aktuell" statt unnötig neu zu schreiben.

### 2. Drag'n'Drop für die Projektverwaltung

Der Projektbaum (`projectTree`) akzeptiert jetzt Dateien, die von außen
(z.B. aus dem Windows-Explorer) hineingezogen werden:
- **Akzeptiert:** `.c`/`.cpp`/`.cc`/`.cxx`/`.h`/`.hpp`/`.hxx` (dieselben
  Endungen, die `Project::typeForFile()` bereits als C/C++ erkennt) sowie
  jede Datei, deren Name mit "Makefile" beginnt (auch mit zusätzlicher
  Endung, z.B. ein mitgebrachtes `Makefile.mine`) - **außer** einem der
  drei auto-generierten Makefiles des *eigenen* Projekts
  (`Makefile.gcc`/`.vbcc`/`.sc`), die weiterhin von der Aufnahme
  ausgeschlossen bleiben (gleiche Ausschlusslogik wie beim kürzlich
  behobenen Bug in `maybeOfferAddToProject()`).
- Nach einer Rückfrage (Einzeldatei oder Liste bei mehreren) werden die
  akzeptierten Dateien über dieselbe `addFilesToProject()`-Logik
  hinzugefügt, die auch die "Add..."-Schaltfläche verwendet - inklusive
  Speichern, Makefile-Regenerierung und Aktualisierung von Baum und
  Funktions-Browser.
- **C-Funktionen erscheinen automatisch im Funktions-Browser:**
  `refreshFunctionsList()` scannt ohnehin alle Projektdateien direkt von
  der Festplatte (nicht nur die gerade geöffnete) - eine per Drag'n'Drop
  hinzugefügte Datei mit C-Funktionen taucht daher ohne zusätzliche
  Arbeit dort auf.

**Technische Umsetzung:** Der Event-Filter wird auf
`projectTree->viewport()` installiert, nicht auf `projectTree` selbst -
das ist die korrekte Stelle für Drag/Drop-Events bei
QAbstractItemView-abgeleiteten Widgets wie `QTreeWidget`.

**Nebenbei behoben:** `projectOtherGroupItem` (aus dem vorherigen
Update) fehlte in der Fett-Formatierungs-Schleife für die
Kategorie-Überschriften im Projektbaum - jetzt ergänzt.

**Verifiziert:**
- Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler.
- `isDragDropAcceptableProjectFile()` mit 13 Testfällen geprüft (alle
  akzeptierten Endungen, externe Makefiles, gezielt die drei
  eigenen auto-generierten Makefiles als Ausschluss, sowie
  zurückgewiesene Dateitypen) - alle korrekt.
- Die komplette Dirty-Tracking-Zustandsmaschine mit der echten
  `Project`-Klasse End-to-End durchgespielt (erstellen → Datei
  hinzufügen → speichern → erneuter Klick ohne Änderungen) - Verhalten
  an jedem Schritt korrekt.
- Programmstart 5x offscreen wiederholt - 0 Abstürze.

**Sprachdateien aktualisiert:** `lupdate` fand 6 neue Strings,
`lrelease` meldet: **522 finished, 0 unfinished**.

## Bugfix: Hinzugefügte Dateien wurden nicht ins Projektverzeichnis kopiert

**Gemeldetes Problem:** Eine per Drag'n'Drop zum Projekt hinzugefügte
Datei wurde nicht ins Projektverzeichnis kopiert, sondern nur an ihrem
ursprünglichen Speicherort referenziert.

**Warum das ein echter Bug war:** Alle drei generierten Makefiles
(`Makefile.gcc`/`.vbcc`/`.sc`) listen `SRCS` als reine Dateinamen ohne
Pfadangabe (siehe `regenerateProjectMakefiles()`), und ihre
Compile-Regeln suchen die Quelldateien direkt neben sich selbst. Eine
Datei außerhalb des Projektverzeichnisses hätte `make` also gar nicht
gefunden - der Build wäre fehlgeschlagen.

**Fix:** Neue Methode `ensureFileInProjectDir(sourcePath)` - kopiert
eine Datei ins Projektverzeichnis, falls sie sich nicht bereits direkt
dort befindet, und liefert den zu trackenden Pfad zurück (die Kopie,
falls eine erstellt wurde, sonst den Originalpfad unverändert). Bei
einem Namenskonflikt mit einer bereits vorhandenen Datei wird vor dem
Überschreiben nachgefragt; schlägt der Kopiervorgang fehl, wird die
Datei übersprungen statt einen kaputten Verweis zu tracken.

`addFilesToProject()` - die gemeinsame Logik hinter **sowohl** der
"Add..."-Schaltfläche **als auch** Drag'n'Drop - ruft dies jetzt für
jede hinzuzufügende Datei auf, bevor sie getrackt wird. Damit verhalten
sich beide Wege konsistent (nicht nur Drag'n'Drop): Eine über "Add..."
aus einem anderen Ordner ausgewählte Datei wird jetzt ebenfalls korrekt
ins Projektverzeichnis kopiert, statt am ursprünglichen Ort zu
verbleiben.

**Bewusst unverändert:** `maybeOfferAddToProject()` (die Nachfrage beim
Speichern einer noch nicht getrackten, aber bereits in einem Editor-Tab
offenen Datei) kopiert weiterhin nicht - dort würde ein Kopieren dazu
führen, dass der offene Tab (der weiterhin die Originaldatei
referenziert) und das Projekt (das dann die Kopie trackt) auseinander-
laufen, sobald erneut gespeichert wird. Dieser Fall betrifft eine
bereits aktiv bearbeitete Datei, kein neues Hinzufügen von außen.

**Verifiziert:**
- Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler.
- `ensureFileInProjectDir()` mit echten Dateien getestet: eine bereits
  im Projektverzeichnis liegende Datei bleibt unverändert (kein
  unnötiges Kopieren), eine externe Datei wird mit identischem Inhalt
  hineinkopiert (Original bleibt unangetastet), ein erneutes
  Hinzufügen derselben externen Datei erkennt korrekt den
  Namenskonflikt und würde nach Bestätigung fragen.

**Sprachdateien aktualisiert:** `lupdate` fand 2 neue Strings,
`lrelease` meldet: **524 finished, 0 unfinished**.

## Bugfix: Fehler-/Warnungs-Parsing für GCC und VBCC grundlegend repariert

**Gemeldetes Problem:** Weder für gcc noch für vbcc werden alle
Warnungen/Fehler in der Ausgabekonsole angezeigt; angezeigte Meldungen
sind bei VBCC anklickbar, bei GCC jedoch nicht.

### Ursache 1: Zu aggressiver Zeilenfilter (betraf BEIDE Compiler)

`readCommand()` (reagiert auf laufend eintreffende Prozess-Ausgabe)
filterte JEDE Zeile heraus, die nicht wörtlich "error" oder "warning"
enthielt. Das killt genau die Zeilen, die zu einer mehrzeiligen
GCC-Diagnose gehören - der Quellcode-Ausschnitt und die `^~~~~`-Marker-
Zeile nach einer "warning:"-Zeile enthalten selbst weder "error" noch
"warning" und wurden verworfen. Noch gravierender: waschechte
Linker-Fehler wie `undefined reference to \`foo'` enthalten ebenfalls
keines der beiden Wörter und wurden komplett unterschlagen - nur eine
etwaige abschließende Sammelzeile wie `collect2: error: ld returned 1
exit status` kam überhaupt durch. `stopCommand()` filterte an anderer
Stelle bereits gar nicht (`output->appendPlainText(cmd->readAll())`) -
diese Inkonsistenz erklärt, warum ein Teil der Meldungen (was am Ende
noch im Puffer stand) sichtbar war, der Rest aber nicht.

**Fix:** Filter komplett entfernt - `readCommand()` zeigt jetzt die
komplette Rohausgabe an, konsistent mit `stopCommand()`.

### Ursache 2: `checkGCC()`s Zeilen-/Spalten-Erkennung war grundlegend fragil

Die alte Logik sammelte **alle** Ziffernfolgen irgendwo in der Zeile
und verlangte, dass es exakt zwei gibt (`list.count() == 2`) - bricht
sofort, sobald der Pfad oder die Meldung selbst eine weitere Zahl
enthält. Genau das war der Fall: Die Projektordner in den Tests hießen
`gcctest3`, `shellprog2`, `sc_test2` - jede Ziffer darin zählte mit und
verfälschte die Erkennung. Auch eine Meldung wie "expected 3 arguments,
have 4" hätte dasselbe Problem ausgelöst. VBCCs `checkVBCC()` nutzte
dagegen bereits eine verankerte Regex mit gezielten Capture-Gruppen -
deshalb funktionierte dort das Anklicken zuverlässig, bei GCC nicht.

**Fix:** `checkGCC()` komplett neu geschrieben, nach demselben Prinzip
wie `checkVBCC()` - eine verankerte Regex `^(.+?):(\d+):(\d+):\s*(\w+):\s*(.*)$`
für die übliche `Datei:Zeile:Spalte: Typ: Meldung`-Form, mit
spaltenloser Fallback-Form `^(.+?):(\d+):\s*(\w+):\s*(.*)$` für ältere/
seltenere Diagnosen - extrahiert exakt die richtigen Werte, unabhängig
davon, wie viele weitere Zahlen sonst noch in der Zeile stehen.

### Ursache 3: Klick auf eine Nicht-Diagnose-Zeile sprang zu veralteten Daten

`checkVBCC()`/`checkGCC()` gaben vorher `void` zurück und
`jumpToError()` wurde in `on_output_cursorPositionChanged()`
**bedingungslos** aufgerufen - auch wenn die angeklickte Zeile gar
keine Diagnose war. In diesem Fall blieben `line_nr`/`column_nr` auf
ihren Werten vom letzten ERFOLGREICHEN Treffer stehen, und ein Klick
auf eine harmlose Ausgabezeile sprang im Editor zu einer völlig
unabhängigen, veralteten Zeile.

**Fix:** Beide Funktionen geben jetzt `bool` zurück (Treffer ja/nein);
`jumpToError()` wird nur noch aufgerufen, wenn tatsächlich ein Treffer
vorlag. `debugfilename` wird bei Nicht-Treffer konsequent geleert (galt
für GCC schon vorher, jetzt auch für VBCC).

**Nebenbei aufgeräumt:** Die tote, explizit als "to be deleted ASAP"
markierte Funktion `testGCCregEx()` (nutzte dieselbe fehlerhafte
Ziffern-Zähl-Logik) wurde entfernt. Die inzwischen ungenutzte
Statusleisten-Meldung "no valid line and column data!" fiel damit
ebenfalls weg (führte vorher bei jedem Klick auf eine harmlose Zeile zu
einer irreführenden Spam-Meldung).

**Verifiziert:**
- Build mit `qmake6 + make` - 0 Warnungen, 0 Fehler.
- Neue `checkGCC()`-Logik mit 8 Testfällen geprüft: der genau
  gemeldete Bruch-Fall (Ziffer im Ordnernamen `gcctest3`) funktioniert
  jetzt korrekt, mehrzeilige Diagnosen (Quellcode-Ausschnitt,
  `^~~~~`-Marker) werden sauber als "kein Diagnose-Format" erkannt statt
  falsch zu matchen, Linker-Zeilen ohne "error:"-Präfix ebenso, eine
  Meldung mit zusätzlichen Zahlen im Text verwirrt die Erkennung nicht
  mehr, und die spaltenlose Fallback-Form funktioniert.
- `checkVBCC()` per Regressionstest erneut geprüft (Ziffer im Pfad,
  Zusammenfassungszeile ohne Treffer) - unverändert korrekt.
- Programmstart 5x offscreen wiederholt - 0 Abstürze.

**Sprachdateien aktualisiert:** keine neuen Strings, 1 veralteter
Eintrag ("no valid line and column data!") vollständig entfernt.
`lrelease` meldet: **523 finished, 0 unfinished**.

## Update: Dark Theme (Prefs > Misc > Default application style)

**Anfrage:** Das Programm soll um ein optisch ansprechendes Dark Theme
erweitert werden, wählbar in den Voreinstellungen (Reiter "Misc", Chooser
"Default application style").

**Umsetzung:** Neuer synthetischer Eintrag **"Dark"** in der bestehenden
ComboBox `comboBoxDefaultStyle` (nach den echten `QStyleFactory`-Stilen,
durch einen Separator abgesetzt) - kein echter Qt-Stilname, sondern wird
in `MainWindow::applyApplicationStyle()` speziell behandelt:
- erzwingt den plattformübergreifenden **"Fusion"**-Stil plus eine eigene
  dunkle `QPalette` (`darkApplicationPalette()`) - native Stile wie
  "windowsvista" ignorieren eine eigene Palette größtenteils für ihr
  eigenes Chrome, Fusion ist der einzige Stil, der sie zuverlässig auf
  jeder Plattform respektiert.
- färbt zusätzlich den **QScintilla-Editor selbst** passend dunkel ein
  (nicht nur die Anwendungs-Chrome) - Hintergrund/Text pro Lexer
  (`applyLexerDarkColors()`), Zeilennummern-Rand, Caret-Line, Selektion,
  Klammer-Matching, Einrückungslinien, Whitespace-Marker
  (`initializeMargin()`/`initializeCaretLine()`, beide jetzt mit
  optionalem `QsciScintilla *editor`-Parameter statt fest `textEdit`).

**Editor-Farbschema:** dunkler Hintergrund (#1e1e1e), heller Standardtext
(#d4d4d4), Kommentare grün, Keywords blau, Strings lachsfarben, Zahlen
mintgrün, Preprocessor magenta - sowie die beiden Amiga-spezifischen
Keyword-Sets aus `amigalexercpp.h` neu abgestimmt: Amiga-NDK/MUI-**Typen**
(`GlobalClass`) von Indigo auf helles Violett aufgehellt,
Amiga-NDK/MUI-**Funktionsnamen** (`KeywordSet2`) von Firebrick auf warmes
Amber umgestellt (beide Original-Farben waren für einen hellen
Hintergrund gedacht und hätten auf dunklem Grund kaum Kontrast gehabt).
Hand-abgestimmte Farben gibt es außerdem für `QsciLexerMakefile`,
`QsciLexerBatch`, `QsciLexerPascal` und den eigenen `AmigaInstallerLexer`
(Installer-Skripte) - jeweils über deren echte Style-Enums
(`qscilexermakefile.h` etc.), keine geratenen Werte.

**Sofortige Wirkung ohne Neustart:** Anders als bisher (der alte
`QApplication::setStyle(p_default_style)`-Aufruf lief nur einmal im
Konstruktor) wird ein Stilwechsel jetzt auch beim Schließen des
Prefs-Dialogs bzw. per Shift+F12 ("Reload settings") sofort angewendet:
- `readSettings()` erkennt eine tatsächliche Änderung von
  `MISC/DefaultStyle` und ruft dann `applyApplicationStyle()` sowie die
  neue `reapplyEditorTheme()` auf.
- `reapplyEditorTheme()` durchläuft alle offenen Tabs und färbt
  Margin/Caret-Line/Lexer jedes einzelnen Editors neu ein, **ohne**
  dessen Lexer neu zu erzeugen (das würde den Fold-Status verwerfen und -
  schlimmer - einen Tab, den der Nutzer bewusst auf einen anderen Lexer
  umgestellt hat, z.B. Makefile, stillschweigend auf C/C++
  zurückschalten).
- Absicherung analog zum früheren Absturz-Bugfix (siehe oben): das
  Live-Anwenden wird über ein eigenes, sauber mit `= false`
  initialisiertes `p_styleInitialized`-Flag getorgt, **nicht** über einen
  Zeiger wie `tabWidget` - der hat (anders als z.B. `recentFilesMenue`)
  keinen `= nullptr`-Initialisierer und wäre beim allerersten
  `readSettings()`-Aufruf (erste Zeile im Konstruktor) noch
  Zufallsspeicher.

**Zusammenspiel mit der alten "Use blackish stylesheet"-Checkbox:** Die
bisherige, rein grau eingefärbte MainWindow-Stylesheet-Option (Prefs >
Misc) würde die neue, anwendungsweite dunkle Palette nur stören - sie
wird jetzt zentral in `applyApplicationStyle()` (statt wie bisher
einmalig in `initializeGUI()`) verwaltet und **nur** angewendet, solange
"Dark" nicht aktiv ist; beim Umschalten auf "Dark" wird ein evtl. bereits
gesetztes Stylesheet automatisch entfernt. Deren Tooltip sowie der
Tooltip der Stil-ComboBox wurden entsprechend präzisiert.

**Nebenbei behoben:** `initializeMargin()` verband bei jedem Aufruf (z.B.
bei jedem Lexer-Wechsel über das Syntax-Menü, jetzt zusätzlich bei jedem
Theme-Wechsel) erneut `textChanged()` mit `fitMarginLines()` - ohne
`Qt::UniqueConnection` hätte sich das über viele Aufrufe hinweg zu
mehrfachen, redundanten Verbindungen aufsummiert. Jetzt mit
`Qt::UniqueConnection` abgesichert.

**Verifiziert:**
- Build mit `qmake6 + make` (Qt 6.4.2, libqscintilla2-qt6-dev) - 0
  Warnungen, 0 Fehler.
- 5x Programmstart offscreen mit vorbelegtem `MISC/DefaultStyle=Dark` - 0
  Abstürze, keine Qt-Warnungen (z.B. kein "invalid style override").
- Je 3x offscreen wiederholt für: Default-Stil ganz ohne gespeicherte
  Einstellung (Erststart-Fall), `Fusion` + aktivierter
  "blackish"-Checkbox (Interaktions-Test), sowie `Dark` kombiniert mit
  deutscher GUI-Sprache - alle 0 Abstürze.
- Visuell per Screenshot (eigenständiges, instrumentiertes
  Testprogramm, `QWidget::grab()` unter `-platform offscreen`) gegen eine
  Demo-C-Datei mit NDK-Typen/-Funktionen geprüft, im Vergleich Light- vs.
  Dark-Theme: Anwendungs-Chrome (Menü, Toolbar, Projekt-/Functions-Panel,
  Statusleiste) UND Editor (Hintergrund, Kommentare, Keywords, Strings,
  Amiga-Typen/-Funktionen) wechseln konsistent und lesbar zwischen beiden
  Modi.

**Sprachdateien aktualisiert:** Tooltip von `comboBoxDefaultStyle` sowie
von `checkBoxStylesheet` geändert (neuer erklärender Text zu "Dark" bzw.
dessen fehlender Wirkung während "Dark" aktiv ist). `lupdate` fand 2
neue/geänderte Strings, die 2 alten Varianten wurden vollständig entfernt
statt nur als "vanished" liegen zu lassen. `lrelease` meldet: **523
finished, 0 unfinished**. Der neue ComboBox-Eintrag "Dark" selbst bleibt
bewusst unübersetzt - wie die echten Qt-Stilnamen daneben (Qt-Stilschlüssel
werden ebenfalls nicht übersetzt), und weil `MISC/DefaultStyle` weiterhin
über `currentText()`/`setCurrentText()` gespeichert/geladen wird, was bei
einer übersetzten Bezeichnung sprachabhängig brechen würde.

## Update: Bugfix - "zerbröselter" Editor-Text unter Linux

**Gemeldetes Problem:** Auf einem echten Linux-System (nicht nur im
Offscreen-Test hier) überlappten bzw. verschmolzen Zeichen im
QScintilla-Editor-Gadget miteinander - z.B. "return NULL;" erschien als
"returnNULL;", "int main(void)" als "intmain(void)", einzelne Buchstaben
direkt vor einem Farbwechsel (Klammern, Kommas) fehlten oder wurden vom
nächsten Style überzeichnet. Betraf **beide** Themes (Light wie Dark) gleich
stark - das Dark Theme selbst war also nicht die Ursache, sondern hat das
längst vorhandene Problem nur sichtbar gemacht, weil der Nutzer es beim
Testen des neuen Themes zuerst bemerkt hat.

**Ursache:** `initializeFont()` setzt für Linux `QFont("Source Code Pro", 9)`
und macht daraus per `setFixedPitch(true)` +
`setStyleHint(QFont::Monospace, QFont::PreferMatch)` unser eigenes,
garantiert dickten-festes `myfont`-Objekt. Das Problem: QScintilla übernimmt
dieses `QFont`-Objekt intern **nicht** unverändert. Für jeden einzelnen
Syntax-Highlighting-Style (Kommentar, Keyword, String, Amiga-Typ, ...) baut
QScintillas Qt-Plattformschicht sich selbst ein **neues, ungehintetes**
`QFont` nur aus Fontname + Punktgröße - unsere `setFixedPitch()`/
`setStyleHint()`-Flags erreichen diese internen Style-Fonts nie (im
QScintilla-Quellcode nachvollzogen: `Font::Create()` in `PlatQt.cpp` setzt
nur Familie, Größe, Kursiv-Flag und Gewicht).

Ist "Source Code Pro" auf dem jeweiligen Linux-System nicht installiert -
was sehr häufig der Fall ist, da diese Schriftart ein separates Paket
braucht und auf vielen Distributionen nicht vorinstalliert ist -, weiß Qt
für **unser eigenes** `myfont` dank der Hints trotzdem eine passende
dicktenfeste Ersatzschrift zu finden. Für die internen, ungehinteten
Style-Fonts von QScintilla dagegen greift Fontconfig ohne jeden Hinweis auf
"monospace" und ersetzt die Schrift durch eine ganz gewöhnliche
**Proportionalschrift** (in den eigenen Tests: statt "Source Code Pro" kam
schlicht "DejaVu Sans" heraus - keine Mono-Variante). QScintilla positioniert
Zeichen aber weiterhin so, als stünden sie auf einem festen Zeichenraster;
mit einer Proportionalschrift (und deren Unterschneidung/Kerning) landen
benachbarte Style-Abschnitte dann direkt aufeinander oder verschlucken den
Zwischenraum - am stärksten sichtbar genau an Farbwechseln, exakt wie
gemeldet.

**Fix:** `initializeFont()` prüft jetzt selbst per `QFontInfo`, ob die
gewünschte Linux-Schrift auf dem laufenden System tatsächlich zu einer
dicktenfesten Schrift auflöst. Falls nicht, wird auf die
Fontconfig-Sammelfamilie `"monospace"` ausgewichen. Weil das kein echter
Schriftname, sondern ein generischer Alias ist, löst Fontconfig ihn auf
jedem korrekt eingerichteten Linux-System **immer** zu einer echten
dicktenfesten Schrift auf - auch ganz ohne Hints, und damit auch für
QScintillas eigene interne Style-Fonts korrekt.

**Verworfene Zwischenversuche** (zur eigenen Doku, falls das Problem später
in anderer Form wieder auftaucht):
- `SCI_SETFONTQUALITY` auf `SC_EFF_QUALITY_NON_ANTIALIASED` - keine
  Wirkung, das Problem lag nicht am Antialiasing.
- `SCI_SETPHASESDRAW` auf `SC_PHASES_ONE` - hat das Symptom nur leicht
  verändert (abgeschnittene statt verschmolzene Zeichen), aber nicht
  behoben, da die eigentliche Ursache eine falsche Schriftauflösung war,
  keine Zeichenreihenfolge beim Zeichnen.

**Verifiziert:**
- Sauberer Neu-Build (`qmake6` + `make`) unter Linux/Qt6/QScintilla 2.14.1:
  0 Warnungen, 0 Fehler.
- Offscreen-Regressionstest (gleiche Methode wie beim Dark-Theme-Update
  oben) für: Erststart ohne gespeicherten Stil, `Dark`-Theme, `Fusion` +
  aktivierter "blackish"-Checkbox, sowie `Dark` + deutsche GUI-Sprache -
  alle 0 Abstürze.
- Visuell per Screenshot gegen eine erweiterte Demo-C-Datei (structs,
  Amiga-Typen/-Funktionen, Zahlen, Strings, Preprocessor, verschachtelte
  Blöcke) geprüft: In allen vier Szenarien oben rendert jede Zeile jetzt
  sauber, ohne verschmolzene oder abgeschnittene Zeichen an
  Farbwechsel-Grenzen - vorher/nachher direkt verglichen.
- Da der Fix rein auf dem bereits überall wiederverwendeten `myfont`-Objekt
  aufsetzt (jeder `lexer->setFont(myfont)`-Aufruf in den
  `initializeLexerXxx()`-Funktionen profitiert automatisch mit), war keine
  Änderung an anderer Stelle nötig.

## Update: Programm-Icon unter Linux beim Start

**Anliegen:** Das Projekt soll beim Programmstart auch unter Linux ein
eigenes Programm-Icon zeigen (`images/amiga_classic.png`), und - falls auf
einem Mac compiliert - `setUnifiedTitleAndToolBarOnMac` verwenden.

**Bestandsaufnahme:** `setUnifiedTitleAndToolBarOnMac(true)` war (hinter
`#if defined(__APPLE__)`) bereits in `MainWindow::initializeGUI()`
vorhanden - hier war nichts zu tun. Ebenso setzte
`MainWindow::initializeGUI()` bereits `this->setWindowIcon(QIcon(
":/images/amiga_classic.png"))` auf das MainWindow selbst.

**Ergänzt:** In `main.cpp` wird das Icon jetzt zusätzlich ganz am Anfang,
direkt nach dem Erzeugen des `QApplication`-Objekts (noch bevor
`MainWindow` überhaupt konstruiert wird), per
`app.setWindowIcon(QIcon(":/images/amiga_classic.png"))` gesetzt. Das ist
unter Linux relevant: Fenstermanager/Taskleiste/Alt-Tab-Umschalter lesen
das Programm-Icon direkt beim Start des Prozesses aus den Fenster-Hints
aus - wird das Icon (wie bisher) erst später in `MainWindow`s Konstruktor
gesetzt, kann es je nach Desktop-Umgebung zu spät kommen oder nur für das
Hauptfenster selbst gelten, nicht aber für zwischenzeitlich angezeigte
Dialoge. Der App-weite Aufruf sorgt dafür, dass von der ersten Sekunde an
und für jedes Fenster (inkl. Prefs-/About-Dialog) das Amiga-Icon verwendet
wird. Der bereits vorhandene `this->setWindowIcon(...)`-Aufruf in
`initializeGUI()` bleibt unverändert bestehen (redundant, aber unschädlich).

**Verifiziert:**
- Sauberer Neu-Build (`qmake6` + `make`) unter Linux/Qt6: 0 Warnungen, 0
  Fehler.
- Offscreen-Testlauf: sowohl `QApplication::windowIcon()` als auch
  `MainWindow::windowIcon()` liefern nach dem Start ein nicht-leeres Icon
  mit korrekt geladener Pixmap-Größe (92x71, aus `amiga_classic.png`) -
  das Icon aus den Qt-Ressourcen (`application.qrc`) wird also
  zuverlässig gefunden und angewendet.
- Kein Einfluss auf Windows/Mac-Build: `app.setWindowIcon(...)` ist
  plattformunabhängiger Qt-Code und wirkt dort genauso (schadet nicht,
  Windows/Mac haben ohnehin meist ein eigenes Icon-Handling über die
  ausführbare Datei selbst).

## Update: Toolbar-Button für "Build/Build Project"

**Anliegen:** Der Projekt-Build-Vorgang (Menüpunkt "Build/Build Project",
führt das Makefile des Projekts mit Target "all" aus) soll auch über einen
eigenen, mehrfarbigen Button in der Werkzeugleiste startbar sein.

**Umsetzung:**
- Neues Icon `images/build_project.png` (64x64, selbst erstellt): ein
  blaues, glänzendes Zahnrad (steht für den Build-/Projekt-Prozess) mit
  einem diagonal aufliegenden Hammer (Holzstiel + Metallkopf, mit
  Glanzlicht und weichem Schlagschatten) - im selben glänzenden,
  mehrfarbigen Stil wie die übrigen Toolbar-Icons (z.B. `dice.png` für
  "Compile"). Motiv Hammer+Zahnrad ist die klassische, sofort erkennbare
  Bildsprache für "Build" (vergleichbar mit dem Hammer-Icon in Eclipse
  o.ä.).
- In `application.qrc` als Ressource eingetragen (`images/build_project.png`).
- `buildProjectAct` (bereits vorhandene Action für "Build Project", bisher
  ohne Icon) bekommt in `createActions()` jetzt
  `QIcon(":/images/build_project.png")` mit auf den Weg.
- In `createToolBars()` wird `buildProjectAct` direkt neben `compileAct` in
  die bereits vorhandene `buildToolBar` eingehängt (`compileAct`, dann
  `buildProjectAct`, dann Trenner, dann die Emulator-Buttons) - spiegelt
  damit exakt die Reihenfolge im Menü "Build" wider
  (compileAct/buildProjectAct/cleanProjectAct).

**Verifiziert:**
- Sauberer Neu-Build (`qmake6` + `make`) unter Linux/Qt6: 0 Warnungen, 0
  Fehler.
- Offscreen-Screenshot-Vergleich Light- und Dark-Theme: Der neue Button
  erscheint direkt neben dem Compile-Button (Würfel-Icon), ist bei
  Standard-Toolbar-Größe klar als Hammer+Zahnrad erkennbar und hebt sich
  farblich gut sowohl vom hellen als auch vom dunklen Toolbar-Hintergrund
  ab.
- Kein Absturz beim Start (Icon wird zuverlässig aus den Qt-Ressourcen
  geladen).

## Update: Eigenes Programm-Icon für die .exe unter Windows

**Anliegen:** Die ausführbare Datei `AmigaED.exe` soll unter Windows ein
eigenes Programm-Icon bekommen (in Explorer, Taskleiste, Alt-Tab usw.).

**Wichtige Klarstellung:** Das ist etwas anderes als das bereits vorhandene
`app.setWindowIcon(...)`/`this->setWindowIcon(...)` (siehe Update weiter
oben zum Linux-Programm-Icon) - diese Aufrufe setzen das Icon nur zur
Laufzeit für die Fenster der laufenden Anwendung. Das Icon der `.exe`-Datei
selbst (das, was man schon im Explorer sieht, bevor das Programm überhaupt
läuft) muss dagegen als Windows-Ressource in die ausführbare Datei
eincompiliert werden - dafür gab es bisher nichts.

**Umsetzung:**
- Neue Datei `images/amiga_classic.ico` - eine mehrstufige Windows-Icon-Datei
  (16, 24, 32, 48, 64, 128, 256 px, jeweils 32-Bit RGBA) mit dem bekannten
  Amiga-Regenbogen-Häkchen-Logo (gleiches Motiv wie `amiga_classic.png`,
  das für das Windows-Icon jedoch in hoher Auflösung neu nachgezeichnet
  wurde, damit auch die großen 128px/256px-Stufen gestochen scharf sind -
  die vorhandene PNG-Quelle war mit 92x71px dafür zu klein und hätte
  unscharf hochskaliert werden müssen).
- In `application.pro` wurde die (nie funktionierende) Alt-Zeile
  `#RC_FILE = application.qrc` durch einen echten `win32 { RC_ICONS =
  images/amiga_classic.ico }`-Block ersetzt. `RC_ICONS` ist die reguläre
  qmake-Variable dafür: qmake erzeugt daraus automatisch ein
  Windows-Ressourcenskript, das der Compiler (MinGW/`windres` bzw. MSVC/
  `rc`) beim Linken mit einbindet - alle Icon-Stufen aus der .ico-Datei
  landen automatisch in der .exe, kein manuelles .rc-Skript nötig.
- Per `win32 { ... }` bewusst auf Windows beschränkt - unter Linux/Mac
  bleibt alles unverändert (dort greift stattdessen weiterhin
  `setWindowIcon()`/`setUnifiedTitleAndToolBarOnMac()`).

**Verifiziert (im Rahmen dessen, was hier ohne echtes Windows/MinGW-System
möglich ist):**
- `qmake6` parst `application.pro` inklusive des neuen `win32 { ... }`-Blocks
  fehlerfrei; der anschließende Linux-Build bleibt bei 0 Warnungen/Fehlern -
  der Windows-spezifische Block hat also keinerlei Nebenwirkung auf
  Linux/Mac.
- Die erzeugte `amiga_classic.ico` wurde als gültige Windows-Icon-Datei
  geprüft (Header + alle 7 Auflösungsstufen einzeln eingelesen und
  dargestellt, auch bei 16x16/24x24 noch klar erkennbar).
- Die eigentliche Einbettung in die `.exe` selbst (der `windres`/`rc`-Schritt)
  kann in dieser Linux-Umgebung nicht kompiliert werden, da hier kein
  MinGW-Windows-Toolchain installiert ist - das ist der eine Teil, der sich
  nur auf deinem Windows/Qt-6.11.2/MinGW-13.1.0-System selbst endgültig
  bestätigen lässt.

**Zum Nachziehen unter Windows wichtig:** Da `RC_ICONS` eine neue
Variable in `application.pro` ist, reicht ein einfaches `make`/erneutes
Bauen in Qt Creator NICHT - qmake muss das Makefile neu erzeugen, damit die
Änderung überhaupt berücksichtigt wird (in Qt Creator z.B. über
"Build > Run qmake", oder per sauberem Neu-Konfigurieren des Projekts).

## Zum Selbst-Nachbauen

```bash
# Debian/Ubuntu:
sudo apt install qt6-base-dev qt6-base-dev-tools libqscintilla2-qt6-dev
qmake6 AmigaED.pro
make -j$(nproc)
```

Die deutsche Übersetzung (`translations/amigaed_de.qm`) liegt bereits fertig
kompiliert im Projekt und wird via `translations.qrc` mit eingebettet - für
einen normalen Build ist nichts weiter nötig. Nur wer die Übersetzung selbst
ändern oder erweitern möchte, braucht zusätzlich `qt6-l10n-tools` (liefert
`lupdate`/`lrelease`) - siehe Abschnitt "Internationalisierung" oben.
