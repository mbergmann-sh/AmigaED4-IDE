AmigaED - Menü-Icon für den dynamisch erzeugten MuiBuilderQt-Eintrag
=======================================================================

WAS DIESER PATCH BRINGT
------------------------
Der dynamisch erzeugte Menüeintrag Tools > GUI Builders > <Name> (siehe
rev.158 "Tools"-Integration) zeigt jetzt, wenn er tatsächlich auf
MuiBuilderQt zeigt, dessen eigenes Programmicon als Menü-Icon - dasselbe
Bild, das MuiBuilderQt selbst als Fenster-/Programmicon verwendet (siehe
den separaten MuiBuilderQt_Icon_Patch).

WIE ERKANNT WIRD, DASS EIN EINTRAG "MuiBuilderQt" IST
--------------------------------------------------------
Bewusst NICHT über die Slot-Nummer (Werkzeug 1/2/3) entschieden, sondern
über den tatsächlichen Datei-Basisnamen des konfigurierten Pfads
("MUIBuilderQt", case-insensitive, ohne Dateiendung - passt also sowohl
auf "MUIBuilderQt.exe" unter Windows als auch auf "MUIBuilderQt" unter
Linux). Grund: der Nutzer kann in Prefs > Tools jedes der drei
GUI-Builder-Felder frei umbenennen/umbiegen - Werkzeug 1 ist zwar per
Default auf MuiBuilderQt vorbelegt, muss es aber nicht bleiben, und
umgekehrt könnte MuiBuilderQt später auch versehentlich in einem anderen
Slot landen. Mit dieser Erkennung bekommt IMMER genau der Eintrag das
Icon, der tatsächlich auf die MuiBuilderQt-Programmdatei zeigt - egal in
welchem der drei Slots -, und nie ein Eintrag, der (trotz evtl.
"MUI"-artigem Namen) auf etwas anderes zeigt. Die beiden anderen
GUI-Builder-Slots (GadTools/ReAction GUI Designer) bleiben bis auf
Weiteres bewusst ohne Icon, da es dafür noch keine eigenen Programme/
Icons gibt.

GEÄNDERTE / NEUE DATEIEN
--------------------------
  images/muibuilderqt.png   (NEU) - identische Kopie von MuiBuilderQts
                              eigenem gui/images/muibuilderqt.png
                              (256x256 PNG)
  application.qrc            - neuer <file>images/muibuilderqt.png</file>-
                              Eintrag in der bestehenden "/"-Ressourcengruppe
                              (selbes Muster wie alle anderen bereits
                              eingebundenen Icons dort - kein alias= nötig,
                              da application.qrc im Projektwurzelverzeichnis
                              liegt, genau wie der images/-Ordner selbst)
  mainwindow.cpp              - MainWindow::rebuildToolsMenu(): in der
                              Schleife über die drei GUI-Builder-Slots wird
                              jetzt bei jedem qualifizierten Eintrag
                              QFileInfo(tool.path).completeBaseName() gegen
                              "MUIBuilderQt" (case-insensitive) verglichen;
                              bei Treffer bekommt die neu erzeugte QAction
                              per setIcon(QIcon(":/images/muibuilderqt.png"))
                              das Icon.

VERIFIKATION
-------------
- Sauberer qmake6 + make-Regressionsbuild (0 Fehler, 0 Warnungen).
- Laufzeit-Verifikation unter Xvfb mit vorab in der QSettings-Datei
  hinterlegter Tools-Konfiguration:
    * Tool 1 -> Pfad einer echten (Dummy-)Datei namens "MUIBuilderQt",
      Name "MUI GUI Designer": erzeugte QAction meldet
      icon().isNull() == false, verfügbare Größe 256x256 - Icon kommt an.
    * Tool 2 -> Pfad einer Datei namens "SomeOtherTool": keine Übereinstimmung,
      kein Icon gesetzt - bestätigt, dass die Erkennung nicht versehentlich
      jeden Eintrag trifft.
  (Debug-Ausgaben wurden nur für diese Verifikation temporär eingefügt und
  vor der Auslieferung wieder vollständig entfernt.)
- Kein Windows-Build in dieser Sandbox möglich (kein MinGW), aber die
  Änderung ist reiner plattformunabhängiger Qt-Code (QFileInfo/QIcon/
  QAction), identisch zur bereits verifizierten Linux-Verifikation.

INSTALLATION
-------------
images/muibuilderqt.png neu ins lokale AmigaED-Projektverzeichnis unter
images/ kopieren, application.qrc und mainwindow.cpp durch die beiliegenden
Versionen ersetzen. Danach qmake6 neu laufen lassen (application.qrc hat
sich geändert) und sauber neu bauen.

Voraussetzung: die "Tools"-Integration aus rev.158 (inkl. des dortigen
toolsMenue-Absturz-Hotfixes) muss bereits eingespielt sein, da dieser
Patch auf deren rebuildToolsMenu()-Funktion aufsetzt.
