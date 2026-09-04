# AmigaED als AppImage bauen

Dieses Verzeichnis enthält alles, um AmigaED für Linux als einzelne,
selbstständig lauffähige `.AppImage`-Datei zu bauen - inklusive Qt6,
QScintilla und deren Plugins. Auf dem Zielsystem muss dafür **nichts**
installiert sein außer den paar Basisbibliotheken, die praktisch jede
Linux-Distribution ohnehin schon mitbringt (glibc, X11/Wayland-Client-
Bibliotheken, Fontconfig etc.).

## Auf welcher Maschine bauen?

**Das ist der wichtigste Punkt.** Ein AppImage bündelt zwar Qt6 und
QScintilla mit hinein - die AmigaED-Programmdatei selbst ist aber immer
noch gegen die glibc der Baumaschine gelinkt. glibc-Kompatibilität gilt
nur in eine Richtung: Ein auf einer neueren glibc gebautes Programm
läuft auf einer älteren glibc meist **nicht** ("GLIBC_2.XX not found"),
umgekehrt (älter gebaut → neueres System) funktioniert es dagegen.

Für "läuft auf Debian 12 und Debian 13" bedeutet das: **Bau es auf
einer echten Debian-12-Maschine** (oder einem Debian-12-Container/einer
VM) - das Ergebnis läuft dann auch auf Debian 13 (und meist auf noch
neueren Distros) mit. Ein auf Debian 13 gebautes AppImage ist dagegen
**nicht** garantiert auf Debian 12 lauffähig.

## Voraussetzungen (auf der Baumaschine, z. B. Debian 12)

```bash
sudo apt install build-essential qt6-base-dev qt6-base-dev-tools libqscintilla2-qt6-dev curl
```

Falls `libqscintilla2-qt6-dev` in deiner Distribution/Version nicht
verfügbar ist (QScintilla für Qt6 wurde erst nach und nach in die
Distributions-Paketquellen aufgenommen - in Debian 12/bookworm ggf.
noch nicht enthalten), muss QScintilla einmalig selbst aus dem
Quellcode gebaut werden (siehe `MIGRATION_NOTES.md` im Projekt-
Hauptverzeichnis für Hintergründe, bzw. https://www.riverbankcomputing.com/software/qscintilla/).

`curl` wird nur gebraucht, um `linuxdeploy` und dessen Qt-Plugin einmalig
herunterzuladen (landen dann in `AppImage/tools/`, werden bei erneuten
Läufen wiederverwendet).

## Bauen

```bash
cd AppImage
./build_appimage.sh
```

Ergebnis: `AppImage/AmigaED-x86_64.AppImage`

Das Skript:
1. baut AmigaED in einem eigenen `AppImage/build/`-Ordner (rührt deinen
   normalen Qt-Creator-Build-Ordner nicht an),
2. lädt `linuxdeploy` und `linuxdeploy-plugin-qt` herunter (einmalig),
3. baut daraus ein `AppDir` mit der Programmdatei, `AmigaED.desktop`
   und einem Icon,
4. lässt `linuxdeploy` automatisch alle benötigten Bibliotheken (Qt6,
   QScintilla, Anzeige-Plugins wie `platforms/libqxcb.so` usw.)
   hineinkopieren und packt das Ganze zur fertigen `.AppImage`.

## Falls "exit code 127" beim Qt-Plugin auftritt

`linuxdeploy` und sein Qt-Plugin sind selbst AppImages und brauchen zum
Ausführen normalerweise FUSE. Das Skript setzt dafür bereits
`APPIMAGE_EXTRACT_AND_RUN=1`, was auch für das intern von `linuxdeploy`
gestartete Qt-Plugin gilt (der reine `--appimage-extract-and-run`-
Kommandozeilenschalter allein reicht dafür nicht, da er sich nicht auf
das Plugin vererbt). Tritt der Fehler trotzdem auf, hilft alternativ:

```bash
sudo apt install fuse3   # oder: fuse, je nach Distributionsversion
```

## Testen

```bash
chmod +x AmigaED-x86_64.AppImage
./AmigaED-x86_64.AppImage
```

Ein AppImage braucht zum direkten Doppelklick-Start normalerweise FUSE
(auf den meisten Desktop-Systemen schon vorhanden). Falls nicht
vorhanden, funktioniert stattdessen:

```bash
./AmigaED-x86_64.AppImage --appimage-extract-and-run
```

## Icon

`AppImage/AmigaED.png` ist eine quadratische, transparent aufgefüllte
256×256-Version von `images/amiga_classic.png` (demselben Icon, das
schon für die Windows-`.exe` genutzt wird - siehe `AmigaED.pro`,
`RC_ICONS`). `linuxdeploy` verlangt zwingend quadratische Icons
("Failed to deploy icon" bei nicht-quadratischen Bildern) - daher die
eigene, vorbereitete Datei statt des Originals direkt. Falls gewünscht,
kannst du später einfach ein anderes quadratisches PNG unter demselben
Namen (`AppImage/AmigaED.png`) hinterlegen.

## Für Debian 12 UND 13 - zwei Dateien oder eine?

Da (wie oben erklärt) ein auf Debian 12 gebautes AppImage auch unter
Debian 13 laufen sollte, reicht in der Praxis **ein** Build (auf
Debian 12) für beide. Willst du es dennoch getrennt testen/ausliefern,
lass das Skript einfach zusätzlich auf einer echten Debian-13-Maschine
laufen und benenne die beiden Ergebnisse entsprechend um, z. B.
`AmigaED-debian12-x86_64.AppImage` / `AmigaED-debian13-x86_64.AppImage`.
