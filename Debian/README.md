# AmigaED als .deb-Paket bauen

Alternative zum AppImage-Rezept (`../AppImage/`): baut AmigaED als
gewöhnliches, dynamisch gelinktes `.deb`-Paket für genau die
Debian-Version, auf der das Skript läuft.

## Warum .deb statt AppImage?

AmigaED startet selbst wieder externe Compiler-Toolchains
(m68k-amigaos-gcc, vbcc) als Kindprozesse. Ein AppImage setzt dafür
intern `LD_LIBRARY_PATH`/`PATH` um, damit AmigaED seine eigenen,
mitgelieferten Qt6-/QScintilla-Bibliotheken findet - genau diese
Umgebung vererbt sich aber an jeden von AmigaED gestarteten
Kindprozess weiter, was einen extern installierten Compiler
durcheinanderbringen kann, wenn der (unter demselben Bibliotheksnamen)
eine andere Version derselben Bibliothek erwartet.

Ein per `.deb` installiertes Programm läuft dagegen mit ganz normaler
Systemumgebung - dieses Problem entfällt komplett.

**Der Kompromiss:** Anders als das AppImage bringt das `.deb` Qt6 und
QScintilla NICHT selbst mit - die müssen auf dem Zielsystem bereits
installiert sein (das Paket trägt das als ganz normale `Depends:`-
Abhängigkeit ein, `apt` installiert sie beim Einrichten automatisch
mit, falls noch nicht vorhanden).

## Wichtig: separat für Debian 12 UND Debian 13 bauen

Anders als beim AppImage **muss** dieses Skript auf jeder Ziel-Version
einzeln laufen - die Namen der Qt6-Laufzeitpakete unterscheiden sich
zwischen Debian 12 und Debian 13 (die "t64"-Umbenennung wegen der
Umstellung auf 64-Bit-`time_t`, die zwischen beiden Versionen
stattfand). Das Skript ermittelt die korrekten Paketnamen automatisch
über `dpkg-shlibdeps` (siehe unten) - das funktioniert aber naturgemäß
nur richtig für genau das System, auf dem gerade gebaut wird.

## Voraussetzungen

```bash
sudo apt install build-essential qt6-base-dev qt6-base-dev-tools libqscintilla2-qt6-dev dpkg-dev
```

## Bauen

```bash
cd Debian
./build_deb.sh
```

Ergebnis z. B.: `Debian/AmigaED_4.0.51_bookworm_amd64.deb` (Debian 12)
bzw. `..._trixie_amd64.deb` (Debian 13) - der Codename wird automatisch
aus `/etc/os-release` gelesen.

Das Skript:
1. baut AmigaED in einem eigenen `Debian/build/`-Ordner (rührt deinen
   normalen Qt-Creator-Build-Ordner nicht an),
2. stellt einen Paketbaum unter `Debian/pkgroot/` zusammen (Programm,
   `.desktop`-Datei, Icon - letztere zwei identisch zu denen im
   AppImage-Rezept),
3. lässt `dpkg-shlibdeps` die tatsächlich benötigten Laufzeitpakete
   (Qt6, QScintilla) korrekt für dieses System ermitteln,
4. packt alles mit `dpkg-deb` zum fertigen `.deb`.

## Installieren / Deinstallieren

```bash
sudo apt install ./AmigaED_4.0.51_bookworm_amd64.deb
```

(`apt install ./datei.deb` statt `dpkg -i` verwenden - dann installiert
`apt` fehlende Abhängigkeiten wie Qt6/QScintilla gleich automatisch
mit, statt nur eine Fehlermeldung auszugeben.)

Deinstallieren:

```bash
sudo apt remove amigaed
```

## Maintainer-Zeile

`build_deb.sh` trägt aktuell `MB-SoftWorX <noreply@example.com>` als
Paket-Maintainer ein. Falls du das Paket über deine eigenen Rechner
hinaus weitergeben möchtest, solltest du das im Skript (Abschnitt
"DEBIAN/control") auf eine echte Kontaktadresse anpassen.
