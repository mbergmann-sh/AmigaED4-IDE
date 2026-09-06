# -*- coding: utf-8 -*-
import base64

ASSET_DIR = "/home/claude/docbuild/html_assets/"

def b64(fname):
    with open(ASSET_DIR + fname, "rb") as f:
        return base64.b64encode(f.read()).decode("ascii")

IMG = {
    "logo": b64("logo-000.png"),
    "adhoc": b64("adhoc-000.png"),
    "project": b64("project-000.png"),
    "prefs": b64("prefs-000.png"),
    "ctxmenu": b64("ctxmenu-000.png"),
    "cover": b64("cover-000.png"),
    "toolbar": b64("toolbar-000.png"),
    "qs1": b64("qs1-000.png"),
    "qs2": b64("qs2-000.png"),
    "qs3": b64("qs3-000.png"),
    "qs4": b64("qs4-000.png"),
}

IMG_NATURAL_SIZE = {
    "logo": (300, 141),
    "adhoc": (1160, 760),
    "project": (1160, 800),
    "prefs": (1160, 820),
    "ctxmenu": (1100, 810),
    "cover": (1000, 900),
    "toolbar": (972, 80),
    "qs1": (1160, 560),
    "qs2": (1160, 620),
    "qs3": (1160, 560),
    "qs4": (1160, 560),
}

def img_tag(key, alt, maxw=560):
    # QTextBrowser's (QTextDocument) HTML/CSS engine does not reliably
    # reserve vertical space for an image sized via percentage width
    # (width:100%) plus max-width - it ends up under-reserving the
    # image's actual rendered height, leaving either a large blank gap
    # or the following heading overlapping the image's bottom edge.
    # Explicit pixel width/height attributes (computed here from each
    # image's real size) mostly fix that, but a block-level "display:
    # block;margin:auto" image directly after a heading still confused
    # this engine's box order (verified: the image and heading swapped
    # visual stacking order even though the heading came first in the
    # HTML). Wrapping the image in <p align="center"> instead - the
    # old-school HTML way of centering, which this engine handles far
    # more predictably than CSS margin:auto - avoids that entirely.
    # border-radius, unsupported by the same engine, is simply dropped
    # rather than left in as dead weight.
    nat_w, nat_h = IMG_NATURAL_SIZE[key]
    disp_h = round(maxw * nat_h / nat_w)
    return ('<p align="center"><img src="data:image/png;base64,' + IMG[key] + '" alt="' + alt +
            '" width="' + str(maxw) + '" height="' + str(disp_h) +
            '" style="border:1px solid #ccc;"></p>')

CSS = """
html { background-color:#ffffff; }
body { font-family: Helvetica, Arial, sans-serif; color:#222; background-color:#ffffff; margin:0; padding:24px 34px 60px 34px; }
h1 { color:#2B578F; font-size:22px; border-bottom:2px solid #eee; padding-bottom:6px; margin-top:46px; }
h1:first-of-type { margin-top:0; }
h2 { color:#D94F1A; font-size:15px; margin-top:26px; }
p { font-size:14px; }
.caption { text-align:center; font-style:italic; color:#666; font-size:12.5px; margin-top:-4px; }
table { border-collapse:collapse; width:100%; margin:14px 0; font-size:13px; }
th { background:#2B578F; color:white; text-align:left; padding:6px 10px; }
td { padding:6px 10px; border-bottom:1px solid #eee; vertical-align:top; }
tr:nth-child(even) td { background:#f2f4f7; }
code, .code { font-family: "Courier New", monospace; background:#f2f2f2; padding:1px 4px; border-radius:3px; font-size:13px; }
.note { background:#f7f7f7; border-left:4px solid #D94F1A; padding:10px 14px; font-size:13px; color:#444; margin:16px 0; }
ol.steps { padding-left:20px; }
ol.steps li { margin-bottom:10px; font-size:14px; }
.toc { background:#f7f9fb; border:1px solid #dde3ea; border-radius:6px; padding:18px 24px; margin-bottom:10px; }
.toc a { color:#2B578F; text-decoration:none; font-size:14px; }
.toc a:hover { text-decoration:underline; }
.toc-num { color:#D94F1A; font-weight:bold; display:inline-block; width:26px; }
.toc ul { list-style:none; padding-left:0; }
.toc li { margin-bottom:9px; }
.cover { text-align:center; padding: 10px 0 30px 0; border-bottom:2px solid #eee; margin-bottom:10px;}
.cover h1 { border:none; margin:6px 0 2px 0; font-size:26px; }
.cover .tagline { color:#D94F1A; font-size:14px; margin-bottom:14px; }
.cover img { max-width:260px; }
.rl-lit dt { color:#D94F1A; font-weight:bold; font-size:13.5px; margin-top:12px; }
.rl-lit dd { margin:2px 0 0 0; font-size:13px; }
"""

def wrap(title, body_html):
    return "<!DOCTYPE html>\n<html><head><meta charset=\"utf-8\"><title>" + title + "</title>\n<style>" + CSS + "</style></head>\n<body>\n" + body_html + "\n</body></html>"


def build(lang):
    de = (lang == "de")

    def T(en, de_):
        return de_ if de else en

    cover = ("""
    <div class="cover">
      """ + img_tag("cover", "AmigaED", 220) + """
      <h1>AmigaED 4.0 Quick Manual</h1>
      <div class="tagline">""" + T("Quick Reference &amp; Getting Started Guide", "Kurzanleitung &amp; Schnellreferenz") + """</div>
      <p style="font-size:12.5px;color:#666;">""" + T("AmigaED is a cross-platform C/C++ and m68k Assembler IDE for classic Amiga development.",
          "AmigaED ist eine plattformübergreifende C/C++- und m68k-Assembler-IDE für die klassische Amiga-Entwicklung.") + """</p>
    </div>
    """)

    toc_items = [
        ("introduction", T("Introduction", "Einführung")),
        ("two-ways", T("Two Ways to Build Your Code", "Zwei Wege, deinen Code zu bauen")),
        ("toolbar", T("Toolbar Reference", "Die Toolbar im Überblick")),
        ("mode1", T("Mode 1: Ad-hoc One File Compilation", "Modus 1: Ad-hoc-Einzeldatei-Kompilierung")),
        ("mode2", T("Mode 2: Project Driven Compilation", "Modus 2: Projektgesteuerte Kompilierung")),
        ("quickstart", T("Quick Start: From Template to Clean Build", "Schnellstart: Von der Vorlage zum sauberen Build")),
        ("prefs", T("The Preferences Editor", "Der Einstellungs-Editor (Prefs)")),
        ("ctxmenu", T("The Editor&rsquo;s Context Menu", "Das Kontextmenü des Editors")),
        ("outputpane", T("The Compiler Output Pane", "Das Compiler-Output-Fenster")),
        ("switches", T("Suggested Compiler &amp; Linker Switches", "Empfohlene Compiler- &amp; Linker-Schalter")),
        ("recommendations", T("Recommendations for Amiga C Programmers", "Empfehlungen für Amiga-C-Programmierer")),
    ]
    toc_html = '<div class="toc"><h1 style="margin-top:0;">' + T("Table of Contents", "Inhaltsverzeichnis") + '</h1><ul>'
    for i, (anchor, label) in enumerate(toc_items, 1):
        toc_html += '<li><span class="toc-num">' + str(i) + '.</span><a href="#' + anchor + '">' + label + '</a></li>'
    toc_html += '</ul></div>'

    de_note_screens = ''
    if de:
        de_note_screens = '<p style="font-size:12.5px;color:#666;"><i>Hinweis: Die Bildschirm-Beschriftungen in dieser Anleitung sind auf Englisch (AmigaEDs Standardsprache) abgebildet. Die Anwendung selbst besitzt aber auch eine deutsche Oberfläche (View \u2192 GUI Language).</i></p>'

    introduction = ("""
    <a name="introduction"></a>
    <h1 id="introduction">""" + T("Introduction", "Einführung") + """</h1>
    """ + img_tag("logo", "AmigaED logo", 300) + """
    <p style="text-align:center;font-size:20px;color:#2B578F;font-weight:bold;margin-bottom:0;">AmigaED</p>
    <p style="text-align:center;font-size:14px;color:#D94F1A;margin-top:2px;">""" + T("Two Ways to Compile", "Zwei Wege zum Compilieren") + """</p>
    <p>""" + T("A short, illustrated guide to Ad-hoc One File Compilation, Project Driven Compilation, and the Preferences Editor.",
        "Eine kurze, bebilderte Anleitung zu Ad-hoc-Einzeldatei-Kompilierung, projektgesteuerter Kompilierung und dem Einstellungs-Editor.") + """</p>
    <p>""" + T("AmigaED is a cross-platform C/C++ and m68k Assembler IDE for classic Amiga development.",
          "AmigaED ist eine plattformübergreifende C/C++- und m68k-Assembler-IDE für die klassische Amiga-Entwicklung.") + """</p>
    """ + de_note_screens + """
    """)

    two_ways = ("""
    <a name="two-ways"></a>
    <h1 id="two-ways">""" + T("Two Ways to Build Your Code", "Zwei Wege, deinen Code zu bauen") + """</h1>
    <p>""" + T("AmigaED supports two independent ways to turn your source code into an AmigaOS-runnable program. Both are always available side by side, so use whichever fits what you are doing right now.",
          "AmigaED unterstützt zwei unabhängige Wege, um aus deinem Quellcode ein lauffähiges AmigaOS-Programm zu machen. Beide stehen immer gleichzeitig zur Verfügung \u2013 nutze einfach den, der gerade zu deiner Aufgabe passt.") + """</p>
    <p><b>1. """ + T("Ad-hoc One File Compilation", "Ad-hoc-Einzeldatei-Kompilierung") + """</b> &ndash; """ + T(
        "open a single source file and compile it directly, with no project setup at all. Best for a quick test, a one-off tool, or trying out an idea.",
        "eine einzelne Quelldatei öffnen und direkt compilieren, ganz ohne Projekt-Einrichtung. Ideal für einen schnellen Test, ein Einzelwerkzeug oder zum Ausprobieren einer Idee.") + """</p>
    <p><b>2. """ + T("Project Driven Compilation", "Projektgesteuerte Kompilierung") + """</b> &ndash; """ + T(
        "group multiple files (sources, headers, assembler, icons, and so on) into a Project. AmigaED tracks them, generates Makefiles for you, and builds the whole thing with one click. Best for anything with more than one file, a GUI (ReAction/MUI), or a program you will come back to later.",
        "mehrere Dateien (Quellcode, Header, Assembler, Icons, ...) in einem Projekt zusammenfassen. AmigaED verfolgt sie, erzeugt automatisch Makefiles und baut alles mit einem Klick. Ideal für alles mit mehr als einer Datei, eine GUI (ReAction/MUI), oder Programme, an denen du später weiterarbeiten willst.") + """</p>
    <table>
      <tr><th></th><th>Ad-hoc</th><th>""" + T("Project", "Projekt") + """</th></tr>
      <tr><td>""" + T("Best for", "Geeignet für") + """</td><td>""" + T("Quick single-file tests", "Schnelle Einzeldatei-Tests") + """</td><td>""" + T("Multi-file / GUI programs", "Mehrdatei-/GUI-Programme") + """</td></tr>
      <tr><td>""" + T("Setup needed", "Einrichtung nötig") + """</td><td>""" + T("None, just open a file", "Keine \u2013 Datei einfach öffnen") + """</td><td>""" + T("Create or import a project", "Projekt anlegen oder importieren") + """</td></tr>
      <tr><td>""" + T("Multiple files linked together", "Mehrere Dateien verknüpft") + """</td><td>""" + T("No", "Nein") + """</td><td>""" + T("Yes", "Ja") + """</td></tr>
      <tr><td>Makefiles</td><td>""" + T("Not generated", "Werden nicht erzeugt") + """</td><td>""" + T("Auto-generated (gcc, vbcc, SAS/C)", "Automatisch (gcc, vbcc, SAS/C)") + """</td></tr>
      <tr><td>""" + T("Persists between sessions", "Bleibt zwischen Sitzungen erhalten") + """</td><td>""" + T("No", "Nein") + """</td><td>""" + T("Yes (.aep project file)", "Ja (.aep-Projektdatei)") + """</td></tr>
    </table>
    """)

    toolbar = ("""
    <a name="toolbar"></a>
    <h1 id="toolbar">""" + T("Toolbar Reference", "Die Toolbar im Überblick") + """</h1>
    <p>""" + T("Every toolbar button mirrors a menu entry exactly - the toolbar is purely a shortcut for the entries you reach for most often, never a separate feature of its own.",
          "Jede Toolbar-Schaltfläche entspricht exakt einem Menüeintrag \u2013 die Toolbar ist reine Abkürzung für die am häufigsten gebrauchten Einträge, keine eigenständige Funktion.") + """</p>
    """ + img_tag("toolbar", "Toolbar reference", 560) + """
    <table>
      <tr><th>#</th><th>""" + T("Button", "Schaltfläche") + """</th><th>""" + T("Menu equivalent", "Entspricht Menü") + """</th><th>""" + T("What it does", "Funktion") + """</th></tr>
      <tr><td>1</td><td>New</td><td>File &rarr; New</td><td>""" + T("Opens a fresh, empty editor tab.", "Öffnet einen neuen, leeren Editor-Tab.") + """</td></tr>
      <tr><td>2</td><td>Open</td><td>File &rarr; Open...</td><td>""" + T("Opens an existing file in a new tab.", "Öffnet eine bestehende Datei in einem neuen Tab.") + """</td></tr>
      <tr><td>3</td><td>Save</td><td>File &rarr; Save</td><td>""" + T("Saves the current tab.", "Speichert den aktuellen Tab.") + """</td></tr>
      <tr><td>4</td><td>Save As</td><td>File &rarr; Save As...</td><td>""" + T("Saves the current tab under a new name/location.", "Speichert den aktuellen Tab unter neuem Namen/Ort.") + """</td></tr>
      <tr><td>5</td><td>Print</td><td>File &rarr; Print file...</td><td>""" + T("Prints the current file.", "Druckt die aktuelle Datei.") + """</td></tr>
      <tr><td>6</td><td>Undo</td><td>Edit &rarr; Undo</td><td>""" + T("Undoes the last edit.", "Macht die letzte Änderung rückgängig.") + """</td></tr>
      <tr><td>7</td><td>Redo</td><td>Edit &rarr; Redo</td><td>""" + T("Redoes the last undone edit.", "Stellt die zuletzt rückgängig gemachte Änderung wieder her.") + """</td></tr>
      <tr><td>8</td><td>Cut</td><td>Edit &rarr; Cut</td><td>""" + T("Cuts the current selection.", "Schneidet die aktuelle Auswahl aus.") + """</td></tr>
      <tr><td>9</td><td>Copy</td><td>Edit &rarr; Copy</td><td>""" + T("Copies the current selection.", "Kopiert die aktuelle Auswahl.") + """</td></tr>
      <tr><td>10</td><td>Paste</td><td>Edit &rarr; Paste</td><td>""" + T("Pastes the clipboard contents.", "Fügt den Zwischenablage-Inhalt ein.") + """</td></tr>
      <tr><td>11</td><td>""" + T("Search", "Suchen") + """</td><td>""" + T("(context menu) Search and Replace...", "(Kontextmenü) Search and Replace...") + """</td><td>""" + T("Opens the Find/Replace panel for the current tab.", "Öffnet das Suchen/Ersetzen-Panel für den aktuellen Tab.") + """</td></tr>
      <tr><td>12</td><td>""" + T("Goto Line", "Goto Line") + """</td><td>Navigation &rarr; Goto Line...</td><td>""" + T("Jumps straight to a given line number.", "Springt direkt zu einer angegebenen Zeilennummer.") + """</td></tr>
      <tr><td>13</td><td>""" + T("Matching bracket", "Matching bracket") + """</td><td>Navigation &rarr; Goto matching bracket</td><td>""" + T("Jumps to the bracket/brace/parenthesis matching the one under the caret.", "Springt zur zur Cursor-Position passenden Klammer.") + """</td></tr>
      <tr><td>14</td><td>Compile</td><td>Build &rarr; Compile...</td><td>""" + T("Ad-hoc single-file compile (Mode 1) - see below.", "Ad-hoc-Einzeldatei-Kompilierung (Modus 1) \u2013 siehe unten.") + """</td></tr>
      <tr><td>15</td><td>Build Project</td><td>Build &rarr; Build Project</td><td>""" + T("Project build (Mode 2) - see below.", "Projekt-Build (Modus 2) \u2013 siehe unten.") + """</td></tr>
      <tr><td>16</td><td>Clean Project</td><td>Build &rarr; Clean Project</td><td>""" + T("Removes the current project's build artifacts (object files, executable, icon).", "Entfernt die Build-Artefakte des aktuellen Projekts (Objektdateien, Executable, Icon).") + """</td></tr>
      <tr><td>17</td><td>""" + T("Start Emulator", "Start Emulator") + """</td><td>Tools &rarr; Emulator &rarr; Start default Workbench in UAE...</td><td>""" + T("Launches your configured UAE emulator.", "Startet den konfigurierten UAE-Emulator.") + """</td></tr>
      <tr><td>18</td><td>""" + T("Stop Emulator", "Stop Emulator") + """</td><td>Tools &rarr; Stop running Emulation...</td><td>""" + T("Stops the running emulator instance.", "Beendet die laufende Emulator-Instanz.") + """</td></tr>
      <tr><td>19</td><td>Exit</td><td>File &rarr; Exit</td><td>""" + T("Closes AmigaED.", "Beendet AmigaED.") + """</td></tr>
    </table>
    <div class="note"><b>""" + T("Note", "Hinweis") + """:</b> """ + T(
        "Start/Stop Emulator automatically grey out and enable each other depending on whether an emulator instance is currently running - including one AmigaED itself didn't start (e.g. left open on purpose from a previous session, or launched outside AmigaED entirely).",
        "Start/Stop Emulator grauen sich automatisch gegenseitig ein bzw. aus, je nachdem, ob gerade eine Emulator-Instanz läuft \u2013 auch eine, die AmigaED selbst nicht gestartet hat (z. B. bewusst aus einer früheren Sitzung offen gelassen, oder ganz unabhängig von AmigaED gestartet).") + """</div>
    """)

    mode1 = ("""
    <a name="mode1"></a>
    <h1 id="mode1">""" + T("Mode 1: Ad-hoc One File Compilation", "Modus 1: Ad-hoc-Einzeldatei-Kompilierung") + """</h1>
    <p>""" + T("Use this when you just want to write and test a single source file, with no project, no linking multiple files, and nothing to set up first.",
          "Nutze diesen Modus, wenn du nur eine einzelne Quelldatei schreiben und testen willst \u2013 ohne Projekt, ohne mehrere verknüpfte Dateien, ohne vorherige Einrichtung.") + """</p>
    """ + img_tag("adhoc", "Ad-hoc mode screenshot") + """
    <p class="caption">""" + T("The editor in Ad-hoc mode: numbers match the steps below.", "Der Editor im Ad-hoc-Modus: Die Nummern entsprechen den Schritten unten.") + """</p>
    <ol class="steps">
      <li>""" + T("File &rarr; New (or Open...) to create or open a single .c/.cpp file. No project is involved.",
             "File &rarr; New (oder Open...), um eine einzelne .c-/.cpp-Datei zu erstellen oder zu öffnen. Es ist kein Projekt beteiligt.") + """</li>
      <li>""" + T("Write or edit your code in the editor tab, with full syntax highlighting.",
             "Code im Editor-Tab schreiben oder bearbeiten \u2013 mit vollständigem Syntax-Highlighting.") + """</li>
      <li>""" + T("Pick your target toolchain and OS from the compiler dropdown in the toolbar (e.g. vbcc / OS 3.x, or m68k-amigaos-gcc / OS 1.3).",
             "In der Toolbar über das Compiler-Dropdown die gewünschte Toolchain und Ziel-OS auswählen (z. B. vbcc / OS 3.x, oder m68k-amigaos-gcc / OS 1.3).") + """</li>
      <li>""" + T("Click the Compile toolbar icon (or Compiler menu) to build just this file.",
             "Auf das Compile-Symbol in der Toolbar klicken (oder das Compiler-Menü nutzen), um nur diese Datei zu bauen.") + """</li>
      <li>""" + T("Check the Compiler Output panel at the bottom, which shows the exact compiler/linker commands used and whether the build succeeded.",
             "Im Compiler-Output-Panel unten nachsehen \u2013 dort stehen die genauen Compiler-/Linker-Befehle und ob der Build erfolgreich war.") + """</li>
      <li>""" + T("Optionally, click Start Emulator to launch UAE and try your freshly built program right away.",
             "Optional auf Start Emulator klicken, um UAE zu starten und das frisch gebaute Programm gleich auszuprobieren.") + """</li>
    </ol>
    <div class="note"><b>""" + T("Note", "Hinweis") + """:</b> """ + T(
        "Ad-hoc compilation always builds and links just the one open file. If your program needs more than one source file, switch to Project Driven Compilation instead (see next chapter).",
        "Die Ad-hoc-Kompilierung baut und linkt immer nur die eine geöffnete Datei. Falls dein Programm mehr als eine Quelldatei braucht, wechsle stattdessen zur projektgesteuerten Kompilierung (siehe nächstes Kapitel).") + """</div>
    """)

    mode2 = ("""
    <a name="mode2"></a>
    <h1 id="mode2">""" + T("Mode 2: Project Driven Compilation", "Modus 2: Projektgesteuerte Kompilierung") + """</h1>
    <p>""" + T("Use this for anything with more than one source file, a GUI (ReAction or MUI), or a program you will want to rebuild and maintain over time.",
          "Nutze diesen Modus für alles mit mehr als einer Quelldatei, für eine GUI (ReAction oder MUI), oder für Programme, die du über längere Zeit weiterbauen und pflegen willst.") + """</p>
    """ + img_tag("project", "Project mode screenshot") + """
    <p class="caption">""" + T("The Project panel and toolbar: numbers match the steps below.", "Das Projekt-Panel und die Toolbar: Die Nummern entsprechen den Schritten unten.") + """</p>
    <ol class="steps">
      <li>""" + T("File &rarr; New Project... to start from a template (Empty C, AmigaShell, AmigaOS 1.3, AmigaOS 3.x, ReAction, or MUI), or File &rarr; Import existing Project... to bring in a folder of sources AmigaED does not know yet.",
             "File &rarr; New Project..., um mit einer Vorlage zu starten (Empty C, AmigaShell, AmigaOS 1.3, AmigaOS 3.x, ReAction oder MUI), oder File &rarr; Import existing Project..., um einen Ordner mit Quelldateien einzubinden, den AmigaED noch nicht kennt.") + """</li>
      <li>""" + T("The Project panel on the left shows all your files, automatically sorted into categories: C/C++ Sources, Headers, Assembler Sources, AmigaGuide, Installer Scripts, Executables, Other Files, and the auto-generated Makefiles.",
             "Das Projekt-Panel links zeigt alle Dateien, automatisch in Kategorien sortiert \u2013 C/C++-Quellen, Header, Assembler-Quellen, AmigaGuide, Installer-Skripte, Executables, Sonstige Dateien, sowie die automatisch erzeugten Makefiles.") + """</li>
      <li>""" + T("Add further files any time with Add files to Project..., or by dragging them straight onto the Project panel.",
             "Jederzeit weitere Dateien über Add files to Project... hinzufügen, oder sie einfach per Drag &amp; Drop auf das Projekt-Panel ziehen.") + """</li>
      <li>""" + T("Click Build Project. AmigaED (re)generates the Makefiles for every configured toolchain and runs the build for you.",
             "Auf Build Project klicken. AmigaED erzeugt die Makefiles für jede konfigurierte Toolchain (neu) und startet den Build.") + """</li>
      <li>""" + T("Watch the Build Output panel. On success, the compiled program appears in the Project panel Executable category, together with its file size.",
             "Das Build-Output-Panel beobachten. Bei Erfolg erscheint das gebaute Programm im Projekt-Panel in der Kategorie Executable, zusammen mit seiner Dateigröße.") + """</li>
      <li>""" + T("Click Start Emulator to launch UAE and test your program.", "Auf Start Emulator klicken, um UAE zu starten und das Programm zu testen.") + """</li>
    </ol>
    <div class="note"><b>""" + T("Note", "Hinweis") + """:</b> """ + T(
        "A project's Makefiles are regenerated automatically every time you add or remove a file, and also right before every Build Project - so a plain content change (e.g. adding floating-point usage to an already-tracked file) is always picked up too, not just file-list changes. Both vbcc and gcc/g++ Makefiles are generated side by side, plus a SAS/C Makefile for building later on a real Amiga.",
        "Die Makefiles eines Projekts werden automatisch neu erzeugt, sobald eine Datei hinzugefügt oder entfernt wird \u2013 und zusätzlich unmittelbar vor jedem Build Project, sodass auch reine Inhalts-Änderungen (z. B. nachträglich ergänzte Gleitkommazahlen in einer bereits erfassten Datei) zuverlässig erkannt werden, nicht nur Änderungen an der Dateiliste. Es werden gleichzeitig sowohl vbcc- als auch gcc/g++-Makefiles erzeugt, dazu ein SAS/C-Makefile für den späteren Bau auf einem echten Amiga.") + """</div>
    <h2>""" + T("Closing a project", "Ein Projekt schließen") + """</h2>
    <p>""" + T("File &rarr; Close Project closes every tab belonging to the current project (prompting to save any with unsaved changes first, exactly like switching to a different project), then returns to the \u201cno project loaded\u201d state. Cancelling a save prompt leaves the project and all its tabs open untouched.",
          "File &rarr; Close Project schließt alle zum aktuellen Projekt gehörenden Tabs (mit Speicherabfrage bei ungespeicherten Änderungen \u2013 genau wie beim Wechsel zu einem anderen Projekt) und kehrt danach in den Zustand \u201ckein Projekt geladen\u201c zurück. Wird eine Speicherabfrage abgebrochen, bleiben Projekt und alle Tabs unverändert offen.") + """</p>
    <h2>""" + T("Automatic program icons", "Automatische Programm-Icons") + """</h2>
    <p>""" + T("If Prefs &rarr; Misc &rarr; \u201ccreate icon\u201d is checked, AmigaED writes its own built-in, multi-coloured Amiga tool icon next to your compiled program - both for a project build and for an ad-hoc single-file compile. No default icon file needs to be configured anywhere. The icon's stack size is set automatically for the target: 4096 bytes for AmigaOS 1.3, 8192 for AmigaOS 3.x, 16000 for a ReAction project, and 34000 for a MUI project.",
          "Ist Prefs &rarr; Misc &rarr; \u201ccreate icon\u201c aktiviert, schreibt AmigaED sein eigenes, eingebautes, mehrfarbiges Amiga-Tool-Icon direkt neben das kompilierte Programm \u2013 sowohl beim Projekt-Build als auch bei der Ad-hoc-Einzeldatei-Kompilierung. Es muss nirgends eine Standard-Icon-Datei konfiguriert werden. Die Stack-Größe des Icons wird automatisch passend zum Ziel gesetzt: 4096 Bytes für AmigaOS 1.3, 8192 für AmigaOS 3.x, 16000 für ein ReAction-Projekt und 34000 für ein MUI-Projekt.") + """</p>
    <h2>""" + T("Cleaning a project", "Ein Projekt bereinigen") + """</h2>
    <p>""" + T("Build &rarr; Clean Project removes the current project's build artifacts - every object file, the compiled executable, and its icon - by deleting each one directly and individually, listing every file it removes in the Compiler Output panel. It does not invoke \u201cmake clean\u201d through a shell.",
          "Build &rarr; Clean Project entfernt die Build-Artefakte des aktuellen Projekts \u2013 jede Objektdatei, das kompilierte Programm sowie dessen Icon \u2013 indem jede Datei einzeln und direkt gelöscht wird; jede entfernte Datei wird im Compiler-Output-Fenster aufgelistet. Dabei wird kein \u201cmake clean\u201c über eine Shell aufgerufen.") + """</p>
    """)

    quickstart = ("""
    <a name="quickstart"></a>
    <h1 id="quickstart">""" + T("Quick Start: From Template to Clean Build", "Schnellstart: Von der Vorlage zum sauberen Build") + """</h1>
    <p>""" + T("A concrete, illustrated walk-through of Mode 2 above, start to finish: creating a new project from a template, building it, and cleaning it up again.",
          "Ein konkreter, bebilderter Durchlauf des oben beschriebenen Modus 2, von Anfang bis Ende: ein neues Projekt aus einer Vorlage erstellen, bauen, und wieder bereinigen.") + """</p>
    <h2>""" + T("1. Create the project", "1. Projekt anlegen") + """</h2>
    <p>""" + T("File &rarr; New Project offers six templates: Empty C Project, Shell Project, AmigaOS 1.3 Project, AmigaOS 3.x Project, ReAction Project, and MUI Project. The ReAction and MUI templates already build a small working window with a File menu (About/Quit) - a genuine starting point, not just an empty shell.",
          "File &rarr; New Project bietet sechs Vorlagen: Empty C Project, Shell Project, AmigaOS 1.3 Project, AmigaOS 3.x Project, ReAction Project und MUI Project. Die ReAction- und MUI-Vorlagen bauen bereits ein kleines, funktionierendes Fenster mit File-Menü (About/Quit) \u2013 ein echter Startpunkt, keine leere Hülle.") + """</p>
    """ + img_tag("qs1", "Choosing a project template", 560) + """
    <p class="caption">""" + T("Picking \u201cAmigaOS 3.x Project\u201d from the New Project submenu.", "Auswahl von \u201cAmigaOS 3.x Project\u201d im New-Project-Untermenü.") + """</p>
    <p>""" + T("After picking a template, AmigaED asks for a target directory and a project name, then (for AmigaOS 1.3/3.x, ReAction, and MUI templates) confirms the default compiler/linker switches for that target before creating anything - the same switches listed in the table further below, pre-filled and ready to adjust.",
          "Nach der Vorlagenauswahl fragt AmigaED nach einem Zielverzeichnis und einem Projektnamen, und bestätigt anschließend (bei den Vorlagen AmigaOS 1.3/3.x, ReAction und MUI) die Standard-Compiler-/Linker-Schalter für dieses Ziel, bevor irgendetwas angelegt wird \u2013 dieselben Schalter, die weiter unten in der Tabelle stehen, bereits vorausgefüllt und direkt anpassbar.") + """</p>
    <h2>""" + T("2. Write your code", "2. Code schreiben") + """</h2>
    <p>""" + T("The generated main file opens automatically, already including the right Amiga NDK headers for the chosen template. The Project panel on the left shows the file, the auto-generated Makefiles, and the (still empty) Executable category.",
          "Die erzeugte Hauptdatei öffnet sich automatisch, bereits mit den passenden Amiga-NDK-Headern für die gewählte Vorlage. Das Projekt-Panel links zeigt die Datei, die automatisch erzeugten Makefiles, sowie die (noch leere) Executable-Kategorie.") + """</p>
    """ + img_tag("qs2", "Freshly created project with editor and project tree", 560) + """
    <p class="caption">""" + T("A freshly created AmigaOS 3.x project, ready to edit.", "Ein frisch erstelltes AmigaOS-3.x-Projekt, bereit zur Bearbeitung.") + """</p>
    <h2>""" + T("3. Build it", "3. Bauen") + """</h2>
    <p>""" + T("Click the Build Project toolbar icon (or Build &rarr; Build Project). AmigaED regenerates the Makefiles fresh, then runs the currently selected compiler (VBCC or GCC/G++, picked in the status bar) against them. If your code uses <span class=\"code\">float</span>/<span class=\"code\">double</span> - or the Amiga-typical uppercase <span class=\"code\">FLOAT</span>/<span class=\"code\">DOUBLE</span> typedefs, both are recognized - the right math library is linked in automatically; nothing to configure by hand.",
          "Auf das Build-Project-Symbol in der Toolbar klicken (oder Build &rarr; Build Project). AmigaED erzeugt die Makefiles frisch neu und ruft dann den aktuell gewählten Compiler auf (VBCC oder GCC/G++, wählbar in der Statusleiste). Verwendet der Code <span class=\"code\">float</span>/<span class=\"code\">double</span> \u2013 oder die auf Amiga typischen großgeschriebenen Typedefs <span class=\"code\">FLOAT</span>/<span class=\"code\">DOUBLE</span>, beide werden erkannt \u2013 wird die passende Mathe-Bibliothek automatisch mitgelinkt; nichts muss von Hand konfiguriert werden.") + """</p>
    """ + img_tag("qs3", "Build Project toolbar button and successful compiler output", 560) + """
    <p class="caption">""" + T("A successful build, reported in the Compiler Output panel.", "Ein erfolgreicher Build, gemeldet im Compiler-Output-Fenster.") + """</p>
    <p>""" + T("See &ldquo;The Compiler Output Pane&rdquo; below for how to jump straight from an error or warning line to the offending source line.",
          "Siehe &bdquo;Das Compiler-Output-Fenster&ldquo; weiter unten, um direkt von einer Fehler- oder Warnzeile zur betroffenen Quelltextzeile zu springen.") + """</p>
    <h2>""" + T("4. Clean up", "4. Aufräumen") + """</h2>
    <p>""" + T("Click the Clean Project toolbar icon (or Build &rarr; Clean Project) to remove the build artifacts again - every object file, the compiled executable, and its icon - each one deleted directly and listed individually in the Compiler Output panel.",
          "Auf das Clean-Project-Symbol in der Toolbar klicken (oder Build &rarr; Clean Project), um die Build-Artefakte wieder zu entfernen \u2013 jede Objektdatei, das kompilierte Programm sowie dessen Icon \u2013 jede Datei einzeln gelöscht und im Compiler-Output-Fenster aufgelistet.") + """</p>
    """ + img_tag("qs4", "Clean Project toolbar button and removed-files output", 560) + """
    <p class="caption">""" + T("Clean Project's output: every removed file is listed by name.", "Ausgabe von Clean Project: Jede entfernte Datei wird namentlich aufgeführt.") + """</p>
    <div class="note"><b>""" + T("Note", "Hinweis") + """:</b> """ + T(
        "Your source files, the .aep project file, and the Makefiles themselves are never touched by Clean Project - only the files a build actually produces.",
        "Deine Quelldateien, die .aep-Projektdatei und die Makefiles selbst werden von Clean Project nie angefasst \u2013 nur die Dateien, die ein Build tatsächlich erzeugt.") + """</div>
    """)

    prefs = ("""
    <a name="prefs"></a>
    <h1 id="prefs">""" + T("The Preferences Editor", "Der Einstellungs-Editor (Prefs)") + """</h1>
    <p>""" + T("File &rarr; Prefs... opens AmigaED settings, organized into tabs: Project (author/email/website, default projects folder), GCC, VBCC, and SAS/C (paths to each toolchain and their compiler/linker options, with separate fields for AmigaOS 1.3 and AmigaOS 3.x targets), Emulator (path to your UAE executable, and separate OS 1.3/3.x configs), and Misc (application style/theme, whether to create a program icon, and other editor behaviour).",
          "File &rarr; Prefs... öffnet AmigaEDs Einstellungen, gegliedert in Reiter: Project (Autor/E-Mail/Website, Standard-Projektordner), GCC, VBCC und SAS/C (Pfade zu jeder Toolchain sowie deren Compiler-/Linker-Optionen \u2013 jeweils getrennte Felder für AmigaOS-1.3- und AmigaOS-3.x-Ziele), Emulator (Pfad zu deiner UAE-Programmdatei, sowie getrennte OS-1.3-/3.x-Konfigurationen) und Misc (Anwendungsstil/Theme, ob ein Programm-Icon erzeugt werden soll, sowie weiteres Editor-Verhalten).") + """</p>
    """ + img_tag("prefs", "Preferences dialog screenshot") + """
    <p class="caption">""" + T("The VBCC tab, as an example: GCC and SAS/C follow the same pattern.", "Der VBCC-Reiter als Beispiel \u2013 GCC und SAS/C folgen demselben Muster.") + """</p>
    <ol class="steps">
      <li>""" + T("Tabs across the top switch between Project, GCC, VBCC, SAS/C, Emulator, and Misc settings.",
             "Die Reiter oben wechseln zwischen Project-, GCC-, VBCC-, SAS/C-, Emulator- und Misc-Einstellungen.") + """</li>
      <li>""" + T("Compiler options are kept separate for AmigaOS 1.3 and AmigaOS 3.x, since the two targets typically need different flags (see the tables below).",
             "Compiler-Optionen werden getrennt für AmigaOS 1.3 und AmigaOS 3.x vorgehalten \u2013 die beiden Ziele brauchen üblicherweise unterschiedliche Schalter (siehe Tabellen unten).") + """</li>
      <li>""" + T("Linker options are likewise separate per target OS. This is where a math library gets added automatically when your project uses floating point (see note below).",
             "Linker-Optionen sind ebenso pro Ziel-OS getrennt \u2013 hier wird automatisch eine Mathe-Bibliothek ergänzt, wenn dein Projekt Gleitkommazahlen verwendet (siehe Hinweis unten).") + """</li>
      <li>""" + T("Default Target OS sets which of the two option sets above is used when you do not explicitly pick one.",
             "Default Target OS legt fest, welcher der beiden obigen Optionssätze verwendet wird, wenn du nicht ausdrücklich einen auswählst.") + """</li>
    </ol>
    <div class="note"><b>""" + T("Automatic floating-point handling", "Automatische Gleitkommazahlen-Behandlung") + """:</b> """ + T(
        "if AmigaED detects float or double - or the Amiga-typical uppercase FLOAT/DOUBLE typedefs, both are recognized - anywhere in your project's own C/C++ sources, it automatically adds the right math library to the generated Makefiles for you: -lm for gcc/g++, -lmieee for vbcc, and MATH=IEEE for SAS/C (only if no MATH= mode is already set). You do not need to add these yourself.",
        "Erkennt AmigaED irgendwo in den eigenen C/C++-Quellen deines Projekts float oder double \u2013 oder die auf Amiga typischen großgeschriebenen Typedefs FLOAT/DOUBLE, beide werden erkannt \u2013 wird automatisch die passende Mathe-Bibliothek in den erzeugten Makefiles ergänzt \u2013 -lm für gcc/g++, -lmieee für vbcc, und MATH=IEEE für SAS/C (nur falls noch kein MATH=-Modus gesetzt ist). Das musst du nicht selbst ergänzen.") + """</div>
    <h2>""" + T("Themes", "Designs") + """</h2>
    <p>""" + T("Prefs &rarr; Misc &rarr; \u201cDefault application style\u201d (or the View &rarr; Theme menu, which changes it immediately without opening Prefs at all) lists every native style available on your platform, plus three synthetic ones: <b>Dark</b>, <b>Workbench 1.3</b> (the classic blue Kickstart 1.3 desktop look, sampled from a real screenshot), and <b>Workbench 3.1</b> (the plainer, neutral grey AmigaOS 3.x look, also sampled from a real screenshot). View &rarr; Theme shows all of them as a single, mutually exclusive list - picking one applies it immediately, everywhere: the application chrome, every open editor tab, and the Compiler Output pane's error/warning colours all update together.",
          "Prefs &rarr; Misc &rarr; \u201cDefault application style\u201c (oder das Menü View &rarr; Theme, das die Änderung sofort anwendet, ganz ohne Prefs zu öffnen) listet jeden auf deiner Plattform verfügbaren nativen Stil, plus drei synthetische: <b>Dark</b>, <b>Workbench 1.3</b> (der klassische blaue Kickstart-1.3-Desktop-Look, direkt aus einem echten Screenshot abgetastet) und <b>Workbench 3.1</b> (der schlichtere, neutral-graue AmigaOS-3.x-Look, ebenfalls aus einem echten Screenshot abgetastet). View &rarr; Theme zeigt alle als eine einzige, sich gegenseitig ausschließende Liste \u2013 die Auswahl wirkt sofort und überall: die Anwendungsoberfläche, jeder offene Editor-Tab, sowie die Fehler-/Warnfarben im Compiler-Output-Fenster ziehen gemeinsam mit.") + """</p>
    <h2>""" + T("The emulator and running instances", "Der Emulator und laufende Instanzen") + """</h2>
    <p>""" + T("If you close AmigaED while its emulator is still running, it asks whether to leave the emulator open or shut it down too - handy since starting an emulator takes real time you can skip on the next session. Starting the emulator also checks, via the operating system's own process list, whether a matching emulator process is already running - whether AmigaED itself started it (e.g. one left open like this) or not - and asks before launching a second instance rather than doing so unconditionally. Start/Stop Emulator (toolbar and Tools menu) automatically reflect whichever of these is currently true.",
          "Wird AmigaED beendet, während der Emulator noch läuft, fragt es, ob der Emulator geöffnet bleiben oder mitbeendet werden soll \u2013 praktisch, da ein Emulator-Start echte Zeit kostet, die man sich für die nächste Sitzung sparen kann. Der Start des Emulators prüft außerdem über die eigene Prozessliste des Betriebssystems, ob bereits ein passender Emulator-Prozess läuft \u2013 egal ob von AmigaED selbst gestartet (z. B. so offen gelassen) oder nicht \u2013 und fragt nach, bevor eine zweite Instanz gestartet wird, statt das bedingungslos zu tun. Start/Stop Emulator (Toolbar und Tools-Menü) spiegeln automatisch wider, welcher dieser beiden Zustände gerade zutrifft.") + """</p>
    """)

    ctxmenu = ("""
    <a name="ctxmenu"></a>
    <h1 id="ctxmenu">""" + T("The Editor&rsquo;s Context Menu", "Das Kontextmenü des Editors") + """</h1>
    <p>""" + T("Right-click anywhere inside the code editor to open a context menu of quick code-insertion shortcuts. It mirrors the main Inserts menu (menu bar) exactly: anything listed here is also available there, under the same name.",
          "Ein Rechtsklick irgendwo im Code-Editor öffnet ein Kontextmenü mit schnellen Code-Einfüge-Hilfen. Es spiegelt exakt das Hauptmenü Inserts (Menüleiste) wider \u2013 alles, was hier aufgeführt ist, findest du dort unter demselben Namen wieder.") + """</p>
    """ + img_tag("ctxmenu", "Context menu screenshot", 480) + """
    <p class="caption">""" + T("The editor context menu, opened over a source file: numbers match the entries explained below.",
                           "Das Kontextmenü des Editors, geöffnet über einer Quelldatei: Die Nummern entsprechen den unten erklärten Einträgen.") + """</p>
    <ol class="steps">
      <li><b>What to insert?</b> &ndash; """ + T("a disabled heading line, not a clickable entry. It is only there to label the menu.",
             "eine deaktivierte Kopfzeile, kein anklickbarer Eintrag. Sie dient nur als Beschriftung des Menüs.") + """</li>
      <li><b>Preprocessor...</b> &ndash; """ + T(
          'a submenu of preprocessor-related inserts: <span class="code">#include</span>, <span class="code">#define</span>, <span class="code">#ifdef</span>, <span class="code">#ifndef</span>, <span class="code">#if defined(...)</span>, <b>Amiga #include files</b> (a ready-made block of the most common Amiga NDK headers), <b>Identify Amiga compiler</b> (a chain of <span class="code">#if defined(...)</span> checks generating a <span class="code">compiler_string</span> constant naming whichever compiler is in use), and <b>Amiga C version string</b> (a <span class="code">$VER:</span> tag).',
          'ein Untermenü mit Präprozessor-Einfügungen: <span class="code">#include</span>, <span class="code">#define</span>, <span class="code">#ifdef</span>, <span class="code">#ifndef</span>, <span class="code">#if defined(...)</span>, <b>Amiga #include files</b> (ein fertiger Block der gängigsten Amiga-NDK-Header), <b>Identify Amiga compiler</b> (eine Kette aus <span class="code">#if defined(...)</span>-Prüfungen, die eine Konstante <span class="code">compiler_string</span> mit dem Namen des jeweils verwendeten Compilers erzeugt), sowie <b>Amiga C version string</b> (ein <span class="code">$VER:</span>-Versionsstring).') + """</li>
      <li><b>Libraries...</b> &ndash; """ + T(
          '<span class="code">OpenLibrary()</span> and <span class="code">CloseLibrary()</span>, each inserting a ready-to-edit <span class="code">dummy.library</span> Open/Close template with error handling.',
          '<span class="code">OpenLibrary()</span> und <span class="code">CloseLibrary()</span> fügen je eine bearbeitbare <span class="code">dummy.library</span>-Open/Close-Vorlage mit Fehlerbehandlung ein.') + """</li>
      <li><b>Condition...</b> &ndash; """ + T('<span class="code">if(..) {...}</span> and <span class="code">if(..) {...} else {...}</span> skeletons.',
             'Vorlagen für <span class="code">if(..) {...}</span> und <span class="code">if(..) {...} else {...}</span>.') + """</li>
      <li><b>Loops...</b> &ndash; """ + T(
          '<span class="code">while(...) {...}</span>, <span class="code">for(...) {...}</span>, <span class="code">do...{...}while(...)</span>, and <span class="code">switch(...)</span> (already including a <span class="code">case dummy: break;</span>).',
          '<span class="code">while(...) {...}</span>, <span class="code">for(...) {...}</span>, <span class="code">do...{...}while(...)</span> sowie <span class="code">switch(...)</span> (bereits mit einem <span class="code">case dummy: break;</span>).') + """</li>
      <li><b>int main {...}</b> &ndash; """ + T('inserts a complete <span class="code">main()</span> function skeleton; disabled once the file already contains one.',
             'fügt eine vollständige <span class="code">main()</span>-Funktion ein; deaktiviert, sobald die Datei bereits eine enthält.') + """</li>
      <li><b>int function {...}</b> &ndash; """ + T("inserts a function prototype plus a matching function skeleton, ready to cut and paste into place.",
             "fügt einen Funktionsprototyp plus passende Funktionsvorlage ein, bereit zum Ausschneiden und Einfügen an die richtige Stelle.") + """</li>
      <li><b>enum {...}</b> &ndash; """ + T('inserts a complete C enumeration skeleton (<span class="code">SomeEnum</span> with three example values).',
             'fügt eine vollständige C-Enumeration ein (<span class="code">SomeEnum</span> mit drei Beispielwerten).') + """</li>
      <li><b>Console Debugging Message</b> &ndash; """ + T(
          'inserts an <span class="code">if (myDebug) {...}</span> debugging block; the caret lands right inside it, ready to type. Needs <span class="code">#define myDebug TRUE</span> earlier in the file, already included near the top of every "New Project" template.',
          'fügt einen <span class="code">if (myDebug) {...}</span>-Debug-Block ein; der Cursor landet direkt darin, bereit zum Tippen. Benötigt ein zuvor im Quelltext definiertes <span class="code">#define myDebug TRUE</span> \u2013 das ist bereits am Anfang jeder "New Project"-Vorlage enthalten.') + """</li>
      <li><b>Comments...</b> &ndash; """ + T('<b>Fileheader comment...</b>, C-style single/multi line comments, a C++ style single line comment, and a C-style line-dividing comment.',
             '<b>Fileheader comment...</b>, C-style Einzeil-/Mehrzeilkommentare, ein C++-Einzeilkommentar sowie ein C-artiger Trennlinien-Kommentar.') + """</li>
    </ol>
    <div class="note"><b>""" + T("Note", "Hinweis") + """:</b> """ + T(
        "Most entries insert their skeleton right at the caret position and leave the caret ready to keep typing, either on a new line after the inserted block, or (for Console Debugging Message) right inside it. This context menu is built from the exact same actions as the main Inserts menu, so the two are always in sync.",
        "Die meisten Einträge fügen ihre Vorlage direkt an der Cursor-Position ein und positionieren den Cursor anschließend so, dass direkt weitergetippt werden kann \u2013 entweder in einer neuen Zeile nach dem eingefügten Block, oder (bei Console Debugging Message) direkt darin. Dieses Kontextmenü nutzt exakt dieselben Aktionen wie das Hauptmenü Inserts \u2013 beide sind daher immer synchron.") + """</div>
    """)

    outputpane = ("""
    <a name="outputpane"></a>
    <h1 id="outputpane">""" + T("The Compiler Output Pane", "Das Compiler-Output-Fenster") + """</h1>
    <p>""" + T("Every compile - ad-hoc (Mode 1) or project (Mode 2), VBCC or GCC/G++ - prints its output into the Compiler Output pane at the bottom, with two things making it easier to work through: colour-coded diagnostics, and click-to-jump.",
          "Jede Kompilierung \u2013 Ad-hoc (Modus 1) oder Projekt (Modus 2), VBCC oder GCC/G++ \u2013 gibt ihre Ausgabe im Compiler-Output-Fenster unten aus, mit zwei Dingen, die das Durcharbeiten erleichtern: farblich markierte Diagnosen und Klick-zum-Springen.") + """</p>
    <h2>""" + T("Colour-coded errors and warnings", "Farblich markierte Fehler und Warnungen") + """</h2>
    <p>""" + T("Every recognized error line is highlighted in red, every warning in yellow/orange - bold text on a tinted background, using a colour scheme matched to the current theme (see Themes above) so it stays readable whichever one is active. Purely informational lines (compiler invocation echoes, \u201cIn file included from...\u201d, GCC's own source-snippet/caret lines) are deliberately left uncoloured.",
          "Jede erkannte Fehlerzeile wird rot hervorgehoben, jede Warnung gelb/orange \u2013 fetter Text auf getöntem Hintergrund, mit einem an das aktuelle Theme angepassten Farbschema (siehe Designs oben), damit es unabhängig vom gewählten Theme lesbar bleibt. Rein informative Zeilen (Echo des Compiler-Aufrufs, \u201cIn file included from...\u201c, GCCs eigene Quelltext-Ausschnitt-/Zeigerzeilen) bleiben bewusst unmarkiert.") + """</p>
    <h2>""" + T("Click to jump", "Klick zum Springen") + """</h2>
    <p>""" + T("Click any error or warning line to jump straight to the offending file and line: the file opens in a tab (or is simply activated, if already open) and the caret moves to the exact reported line and column.",
          "Ein Klick auf eine Fehler- oder Warnzeile springt direkt zur betroffenen Datei und Zeile: Die Datei öffnet sich in einem Tab (oder wird einfach aktiviert, falls bereits offen), und der Cursor springt zur exakt gemeldeten Zeile und Spalte.") + """</p>
    <table>
      <tr><th>""" + T("Compiler", "Compiler") + """</th><th>""" + T("Recognized format", "Erkanntes Format") + """</th></tr>
      <tr><td>VBCC</td><td><span class="code">error 9 in line 24 of "file.c": message</span></td></tr>
      <tr><td>GCC/G++</td><td><span class="code">file.c:24:5: error: message</span></td></tr>
    </table>
    <div class="note"><b>""" + T("Note", "Hinweis") + """:</b> """ + T(
        "An error reported inside a generated intermediate file (e.g. VBCC reporting an assembler-level problem in the .asm file it produced from your .c source) still jumps correctly - to that intermediate file, exactly as the compiler itself reported it.",
        "Ein Fehler, der innerhalb einer erzeugten Zwischendatei gemeldet wird (z. B. wenn VBCC ein Problem auf Assembler-Ebene in der aus deiner .c-Datei erzeugten .asm-Datei meldet), springt weiterhin korrekt dorthin \u2013 zu genau der Zwischendatei, wie vom Compiler selbst gemeldet.") + """</div>
    """)

    switches = ("""
    <a name="switches"></a>
    <h1 id="switches">""" + T("Suggested Compiler &amp; Linker Switches", "Empfohlene Compiler- &amp; Linker-Schalter") + """</h1>
    <p>""" + T("These are AmigaED own built-in defaults: a solid, well-tested starting point for each toolchain and target. You are always free to add your own project-specific flags in the same Prefs fields alongside them.",
          "Das sind AmigaEDs eigene, eingebaute Standardwerte \u2013 ein solider, gut erprobter Ausgangspunkt für jede Toolchain und jedes Ziel. Du kannst jederzeit eigene, projektspezifische Schalter in denselben Prefs-Feldern ergänzen.") + """</p>
    <h2>m68k-amigaos-gcc (GCC/G++)</h2>
    <table>
      <tr><th>""" + T("OS 1.3 compiler", "OS-1.3-Compiler") + """</th><td class="code">-Wall -O2 -mcrt=nix13</td><td>""" + T("Common warnings, optimize, link against libnix built for Kickstart 1.3.", "Übliche Warnungen, Optimierung, Bindung gegen libnix, gebaut für Kickstart 1.3.") + """</td></tr>
      <tr><th>""" + T("OS 3.x compiler", "OS-3.x-Compiler") + """</th><td class="code">-Wall -O2 -noixemul</td><td>""" + T("Common warnings, optimize, use the AmigaOS-native C runtime instead of ixemul.library.", "Übliche Warnungen, Optimierung, nutzt die AmigaOS-native C-Laufzeitumgebung statt ixemul.library.") + """</td></tr>
      <tr><th>""" + T("OS 1.3 linker", "OS-1.3-Linker") + """</th><td class="code">""" + T("(none)", "(keine)") + """</td><td>""" + T("The OS 1.3 template is console-only and needs no extra libraries by default.", "Die OS-1.3-Vorlage ist reine Konsolenanwendung und braucht standardmäßig keine Zusatzbibliotheken.") + """</td></tr>
      <tr><th>""" + T("OS 3.x linker", "OS-3.x-Linker") + """</th><td class="code">-lamiga</td><td>""" + T("Needed for Workbench-capable programs (ReAction/MUI), added automatically by AmigaED OS 3.x/ReAction/MUI templates.", "Für Workbench-fähige Programme (ReAction/MUI) nötig \u2013 wird von AmigaEDs OS-3.x-/ReAction-/MUI-Vorlagen automatisch ergänzt.") + """</td></tr>
    </table>
    <h2>vbcc (vc)</h2>
    <table>
      <tr><th>""" + T("OS 1.3 compiler", "OS-1.3-Compiler") + """</th><td class="code">+kick13 -c99</td><td>""" + T("Named vbcc config for Kickstart 1.3, C99 language mode.", "Benannte vbcc-Konfiguration für Kickstart 1.3, C99-Sprachmodus.") + """</td></tr>
      <tr><th>""" + T("OS 3.x compiler", "OS-3.x-Compiler") + """</th><td class="code">+aos68k -c99</td><td>""" + T("Named vbcc config for AmigaOS 2.0+/3.x, C99 language mode.", "Benannte vbcc-Konfiguration für AmigaOS 2.0+/3.x, C99-Sprachmodus.") + """</td></tr>
      <tr><th>""" + T("OS 1.3 linker", "OS-1.3-Linker") + """</th><td class="code">""" + T("(none)", "(keine)") + """</td><td>""" + T("Console-only OS 1.3 template needs no extra libraries by default.", "Die reine Konsolen-OS-1.3-Vorlage braucht standardmäßig keine Zusatzbibliotheken.") + """</td></tr>
      <tr><th>""" + T("OS 3.x linker", "OS-3.x-Linker") + """</th><td class="code">-lauto -lamiga</td><td>""" + T("vbcc own amiga.lib equivalent, needed for Workbench-capable (ReAction/MUI) programs.", "vbccs eigenes Äquivalent zu amiga.lib, nötig für Workbench-fähige (ReAction/MUI) Programme.") + """</td></tr>
    </table>
    <div class="note">""" + T(
        "Optional additions many users like to add for a release build once things compile cleanly: -cpu=68020 (or higher), -O2/-O3 (optimize), -size (optimize for code size), -final (disable runtime checks). These are not part of AmigaED own defaults since they trade off compatibility or debuggability for speed/size.",
        "Optionale Ergänzungen, die viele Nutzer für einen Release-Build hinzufügen, sobald der Code sauber compiliert: -cpu=68020 (oder höher), -O2/-O3 (Optimierung), -size (Codegröße), -final (Laufzeitprüfungen abschalten). Diese sind bewusst nicht Teil von AmigaEDs eigenen Standardwerten, da sie Kompatibilität bzw. Debugbarkeit gegen Geschwindigkeit/Größe eintauschen.") + """</div>
    <h2>SAS/C</h2>
    <p>""" + T("SAS/C only runs on a real Amiga or emulator, so AmigaED never invokes it itself: it just generates a Makefile.sc alongside the others for later, manual use there. The SCOPTS field in Prefs starts empty; AmigaED still adds MATH=IEEE automatically if your project needs it.",
          "SAS/C läuft nur auf einem echten Amiga oder Emulator, AmigaED ruft es daher selbst nie auf \u2013 es erzeugt lediglich ein Makefile.sc zusätzlich zu den anderen, für die spätere, manuelle Nutzung dort. Das SCOPTS-Feld in den Prefs ist zunächst leer; AmigaED ergänzt trotzdem automatisch MATH=IEEE, falls dein Projekt das braucht.") + """</p>
    """)

    recommendations = ("""
    <a name="recommendations"></a>
    <h1 id="recommendations">""" + T("Recommendations for Amiga C Programmers", "Empfehlungen für Amiga-C-Programmierer") + """</h1>
    <p>""" + T("A short, curated list of literature and tools the Amiga C/ReAction/MUI development community particularly recommends. None of this is bundled with AmigaED, but all of it pairs well with it.",
          "Eine kurze, handverlesene Liste an Literatur und Werkzeugen, die in der Amiga-C-/ReAction-/MUI-Entwicklergemeinde besonders empfohlen werden \u2013 nichts davon liegt AmigaED bei, aber alles passt gut dazu.") + """</p>
    <h2>""" + T("Literature", "Literatur") + """</h2>
    <dl class="rl-lit">
      <dt>Amiga ROM Kernel Reference Manual: Exec / Libraries and Devices / Includes and Autodocs</dt>
      <dd>""" + T("The original Commodore-Amiga reference series, still the authoritative source for Exec, Intuition, and the rest of the classic system libraries. Out of print; free scanned copies are archived at archive.org.",
             "Die ursprüngliche Commodore-Amiga-Referenzreihe \u2013 nach wie vor die maßgebliche Quelle für Exec, Intuition und den Rest der klassischen System-Bibliotheken. Vergriffen; freie Scans sind auf archive.org archiviert.") + """</dd>
      <dt>Amiga ROM Kernel Reference Manual: AmigaDOS &ndash; Thomas Richter</dt>
      <dd>""" + T("A modern, RKRM-style reference specifically for AmigaDOS/dos.library. Free download from Aminet; a printed edition has also been available via lookbehindyou.de.",
             "Eine moderne, im RKRM-Stil gehaltene Referenz speziell für AmigaDOS/dos.library. Kostenloser Download bei Aminet; eine gedruckte Ausgabe war zeitweise auch über lookbehindyou.de erhältlich.") + """</dd>
      <dt>ROM Kernel Reference Manual: Changes &amp; Additions &ndash; Camilla Boemann &amp; Jason Stead</dt>
      <dd>""" + T("An ongoing (WIP) volume covering everything added to the Amiga OS from Release 2 through 3.2. Free PDF from developer.amigaos3.net.",
             "Ein fortlaufendes (WIP-)Werk, das alles abdeckt, was dem AmigaOS von Release 2 bis 3.2 hinzugefügt wurde. Kostenloses PDF von developer.amigaos3.net.") + """</dd>
      <dt>Erik Bartmanns Amiga Programmierbuch</dt>
      <dd>""" + T("C programming, cross-compiling with vbcc and ReAction, on an Amiga 4000 (or Amiga Forever). Book: 32 EUR, e-book: 19.99 EUR, via bombini-verlag.de.",
             "C-Programmierung, Crosscompiling mit vbcc und ReAction, auf einem Amiga 4000 (bzw. Amiga Forever). Buch: 32 EUR, E-Book: 19,99 EUR, über bombini-verlag.de.") + """</dd>
      <dt>Erik Bartmanns Amiga 1200 Buch</dt>
      <dd>""" + T("Machine language, C programming, and Shell: 717 pages across 28 chapters, focused on the Amiga 1200. Book: 64 EUR, e-book: 29.99 EUR, via bombini-verlag.de.",
             "Maschinensprache, C-Programmierung und Shell \u2013 717 Seiten in 28 Kapiteln, mit Fokus auf den Amiga 1200. Buch: 64 EUR, E-Book: 29,99 EUR, über bombini-verlag.de.") + """</dd>
    </dl>
    <h2>""" + T("Tools", "Werkzeuge") + """</h2>
    <dl class="rl-lit">
      <dt>Rebuild &ndash; Darren Coles</dt>
      <dd>""" + T("A ReAction GUI builder (a from-scratch remake of the classic ClassMate), generating C or Amiga E code. Public domain. Source: github.com/dmcoles/ReBuild",
             "Ein ReAction-GUI-Builder (eine komplette Neuentwicklung des klassischen ClassMate), der C- oder Amiga-E-Code erzeugt. Public Domain. Quelle: github.com/dmcoles/ReBuild") + """</dd>
      <dt>Codecraft &ndash; Camilla Boemann</dt>
      <dd>""" + T("Codecraft is a powerful IDE for developing software natively on the Amiga. Codecraft makes it easy for you as a developer to write code, then build, execute and debug your resulting program. And everything is at your fingertips in one unified user interface. A native IDE for AmigaOS 3.2.3 and later. Source: gitlab.com/boemann/codecraft, http://boemann.dk/codecraft/",
             "Codecraft ist eine leistungsstarke IDE für die native Softwareentwicklung auf dem Amiga. Codecraft macht es für dich als Entwickler einfach, Code zu schreiben und das daraus entstehende Programm zu bauen, auszuführen und zu debuggen. Und das alles ist in einer einzigen, einheitlichen Benutzeroberfläche griffbereit. Eine native IDE für AmigaOS 3.2.3 und neuer. Quelle: gitlab.com/boemann/codecraft, http://boemann.dk/codecraft/") + """</dd>
      <dt>MUIBuilder</dt>
      <dd>""" + T("A user interface designer for the MUI and Zune toolkits. GPLv3/LGPLv3. Source: sourceforge.net/projects/muibuilder",
             "Ein Oberflächen-Designer für die MUI- und Zune-Toolkits. GPLv3/LGPLv3. Quelle: sourceforge.net/projects/muibuilder") + """</dd>
    </dl>
    <div class="note">""" + T("Prices, links, and availability change over time. If something above is no longer reachable, a web search for the title and author is usually the fastest way to find its current home.",
          "Preise, Links und Verfügbarkeit ändern sich mit der Zeit \u2013 sollte oben etwas nicht mehr erreichbar sein, findet eine Websuche nach Titel und Autor meist am schnellsten die aktuelle Quelle.") + """</div>
    """)

    body = cover + toc_html + introduction + two_ways + toolbar + mode1 + mode2 + quickstart + prefs + ctxmenu + outputpane + switches + recommendations
    title = "AmigaED 4.0 Quick Manual" if not de else "AmigaED 4.0 Kurzanleitung"
    return wrap(title, body)


with open("/home/claude/docbuild/manual_en.html", "w", encoding="utf-8") as f:
    f.write(build("en"))
with open("/home/claude/docbuild/manual_de.html", "w", encoding="utf-8") as f:
    f.write(build("de"))

print("done")
