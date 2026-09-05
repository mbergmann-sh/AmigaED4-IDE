QT       += core gui widgets printsupport
CONFIG += qscintilla2 c++17 release
# HINWEIS: 'qscintilla2' Feature/Bibliothek muss selbst gegen Qt6 gebaut sein
# (eigene qscintilla2.prf fuer qmake). Eine Qt5-QScintilla-Installation
# funktioniert hier nicht - siehe README-Hinweis unten.

TARGET = AmigaED
TEMPLATE = app

macx {
    QMAKE_POST_LINK = install_name_tool -change libqscintilla2_qt$${QT_MAJOR_VERSION}.13.dylib $$[QT_INSTALL_LIBS]/libqscintilla2_qt$${QT_MAJOR_VERSION}.13.dylib $(TARGET)
}

HEADERS      = mainwindow.h \
    prefsdialog.h \
    aboutdialog.h \
    amigalexercpp.h \
    amigainstallerlexer.h \
    amigaguidelexer.h \
    m68kasmlexer.h \
    project.h \
    version.h
SOURCES      = main.cpp mainwindow.cpp \
    prefsdialog.cpp \
    aboutdialog.cpp \
    amigainstallerlexer.cpp \
    amigaguidelexer.cpp \
    m68kasmlexer.cpp \
    project.cpp
RESOURCES    += application.qrc translations.qrc

TRANSLATIONS += translations/amigaed_de.ts


# Windows .exe icon (shown in Explorer, taskbar, Alt-Tab, etc. - this is
# separate from the QApplication::setWindowIcon()/QWidget::setWindowIcon()
# calls in main.cpp/mainwindow.cpp, which only set the icon at runtime and
# have no effect on the .exe file itself). qmake turns RC_ICONS into an
# auto-generated Windows resource (.rc) script that embeds the .ico file
# into the binary at link time via windres/rc - no manual .rc file needed.
# (Older, incorrect attempt at this used to sit here: "RC_FILE =
# application.qrc" - a .qrc is not a valid Windows .rc script, so that
# never actually worked and has been replaced with the real mechanism.)
win32 {
    RC_ICONS = images/amiga_classic.ico

    # Without an explicit DESTDIR, this mkspec places the built .exe in
    # a "release" subfolder under OUT_PWD (confirmed by the Makefile's
    # own target: "release/AmigaED.exe"), not directly in OUT_PWD itself
    # - and that subfolder's exact name isn't something to reliably
    # guess/hardcode. Pinning DESTDIR to OUT_PWD removes that extra
    # subfolder entirely, so the .exe (and therefore windeployqt's
    # target/the DLL copy destination below) always sits at a single,
    # predictable path. Only affects the final binary's location, not
    # where intermediate object files go.
    DESTDIR = $$OUT_PWD

    # After a successful build, run windeployqt right in the .exe's own
    # output folder so it drops in the Qt DLLs (platform plugin etc.)
    # the app actually needs to run standalone there - without this, a
    # freshly built AmigaED.exe would only start on a machine that
    # already has the right Qt6*.dll files available some other way.
    # Located via QT_INSTALL_BINS - qmake's own query for "the bin/
    # folder of the Qt kit currently being used to build" (windeployqt
    # always ships right there, next to qmake itself) - rather than
    # assuming windeployqt is already on PATH. So this fires the same
    # way whether the build is triggered by Qt Creator's own build step
    # or by running qmake6/mingw32-make from a plain console, and always
    # matches whichever Qt kit is actually active for the build (switch
    # kits later and this switches with it - no hardcoded path).
    #
    # windeployqt only knows about Qt's own DLLs, not third-party ones -
    # so qscintilla2_qt6.dll still needs to be copied over separately.
    # Default assumed location: the same bin/ folder as Qt's own DLLs
    # (QT_INSTALL_BINS) - the usual place a "make install" of QScintilla
    # itself puts it, mirroring how the macx block above already assumes
    # QT_INSTALL_LIBS for the equivalent .dylib. If your own QScintilla
    # build ends up somewhere else, override it - either on the qmake
    # command line (qmake6 AmigaED.pro "QSCINTILLA_BIN_DIR=C:/path/to/dir")
    # or via Qt Creator's Project Settings > Build Steps > qmake >
    # "Additional arguments" - without touching this file.
    isEmpty(QSCINTILLA_BIN_DIR): QSCINTILLA_BIN_DIR = $$[QT_INSTALL_BINS]

    # Deliberately NOT using qmake's $$shell_path()/$$shell_quote() here:
    # both adapt their output to whichever shell qmake believes will run
    # the build - and since mingw32-make picks up the m68k-amigaos-gcc
    # toolchain's own sh.exe from PATH (see the cmd-wrapping comment
    # below), qmake rewrites D:/... into the MSYS-style /D/... form for
    # it. That form only means anything to sh - passed through to our
    # explicit "cmd /c" call below, cmd.exe can't resolve it at all
    # ("Das System kann den angegebenen Pfad nicht finden"). Plain,
    # unquoted forward-slash paths (which both cmd.exe and Windows
    # itself accept perfectly well) sidestep that entirely. No quoting
    # is added around them, so this assumes none of QT_INSTALL_BINS,
    # OUT_PWD or QSCINTILLA_BIN_DIR contain a space - true for a
    # default Qt install/project path; if yours does, this line-pair
    # would need manual quoting adjusted for it.
    WINDEPLOYQT_BIN = $$[QT_INSTALL_BINS]/windeployqt.exe
    WINDEPLOYQT_TARGET = $$DESTDIR/$${TARGET}.exe
    QSCINTILLA_DLL_DEST = $$DESTDIR

    # The DLL's exact filename is resolved here, at qmake time, via
    # $$files() - NOT left as a "qscintilla2_qt6*.dll" wildcard for the
    # build-time shell to expand. That wildcard form broke the same way
    # the paths above did: sh.exe (see above) glob-expands it itself
    # before cmd.exe ever sees the line, and if more than one file
    # happens to match (e.g. an import-library variant sitting next to
    # the DLL), copy ends up with extra, unexpected arguments and fails
    # with a plain "Syntaxfehler". Resolving to one literal filename now
    # avoids any shell needing to interpret a wildcard at all.
    QSCINTILLA_DLL_CANDIDATES = $$files($$QSCINTILLA_BIN_DIR/qscintilla2_qt6*.dll)
    isEmpty(QSCINTILLA_DLL_CANDIDATES) {
        warning("AmigaED.pro: no qscintilla2_qt6*.dll found in $$QSCINTILLA_BIN_DIR - the Windows build won't copy it automatically. Override QSCINTILLA_BIN_DIR (see comment above) to fix.")
    } else {
        QSCINTILLA_DLL_SRC = $$take_first(QSCINTILLA_DLL_CANDIDATES)
    }

    # windeployqt is run via an explicit "cmd /c ..." (rather than relying
    # on whichever shell mingw32-make happens to invoke recipe lines
    # with - it doesn't always use cmd.exe: if it finds ANY sh.exe on
    # PATH, e.g. one bundled with a cross-compiler toolchain such as the
    # m68k-amigaos-gcc one this project itself uses, it uses that
    # instead). The xcopy step below doesn't need that wrapping - see
    # its own comment. Both steps are kept on separate recipe lines (via
    # the $$RETURN line break) rather than chained with "&&", so neither
    # depends on how the other's shell would parse that operator.
    RETURN = $$escape_expand(\n\t)
    QMAKE_POST_LINK += cmd /c $$WINDEPLOYQT_BIN $$WINDEPLOYQT_TARGET
    # xcopy is old DOS-heritage tooling: unlike windeployqt.exe (a
    # modern program with normal argument parsing), it treats ANY "/"
    # anywhere in the command line as a switch introducer - not just at
    # the start of an argument - so a forward-slash path like
    # "D:/Qt/.../qscintilla2_qt6.dll" gets shredded into a pile of
    # bogus single-letter "switches" instead of being read as a
    # filename ("Unzulässige Parameteranzahl"). $$replace() converts
    # just these two paths to real backslashes as plain qmake-time text
    # substitution - deliberately not $$shell_path(), which (as seen
    # above) adapts to whichever shell qmake believes will run the
    # build and gets that wrong here.
    QSCINTILLA_DLL_SRC_WIN = $$replace(QSCINTILLA_DLL_SRC, /, \\)
    QSCINTILLA_DLL_DEST_WIN = $$replace(QSCINTILLA_DLL_DEST, /, \\)
    # /I treats the destination as a directory without prompting, even
    # though it doesn't already contain a file of that name.
    !isEmpty(QSCINTILLA_DLL_SRC): QMAKE_POST_LINK += $$RETURN xcopy /Y /I $$QSCINTILLA_DLL_SRC_WIN $$QSCINTILLA_DLL_DEST_WIN

    # --- "AmigaED_install\install_src" staging folder ------------------
    # After the DLLs above are in place, stage a clean, install-ready
    # copy of everything a distributable AmigaED needs - the .exe, every
    # DLL (Qt's own plus qscintilla2_qt6.dll), and every plugin
    # subfolder windeployqt created (platforms, styles, imageformats,
    # etc.) - into <project root>\AmigaED_install\install_src, plus the
    # DOC folder (DE/EN PDF manuals). That folder is meant to be handed
    # straight to the .iss installer script as its SourceDir (see
    # AmigaED_install\AmigaED.iss, which asks the end user whether to
    # actually install the manuals).
    #
    # The actual copy logic (clean, recreate, robocopy with its exclude
    # patterns) lives in install_stage.bat, a plain static file shipped
    # alongside AmigaED.pro - NOT built up as qmake-generated recipe
    # text. Three earlier attempts at doing this directly in
    # QMAKE_POST_LINK (an xcopy /EXCLUDE-file version, then two robocopy
    # versions, one cmd-wrapped) all hit real, reproducible corruption -
    # wildcards and/or backslashes arriving at the copy tool mangled or
    # glob-expanded, even though the exact same command typed directly
    # into cmd.exe worked perfectly every time. That points at
    # mingw32-make's own recipe-line shell (not necessarily cmd.exe -
    # see the windeployqt comment above) mangling the command text
    # in transit. Moving the wildcards/options into a static .bat file
    # sidesteps that: the recipe line below only ever has to pass two
    # wildcard-free paths through to "cmd /c install_stage.bat" -
    # everything else lives in a file only cmd.exe itself ever reads.
    INSTALL_SRC_DIR = $$PWD/AmigaED_install/install_src
    INSTALL_SRC_DIR_WIN = $$replace(INSTALL_SRC_DIR, /, \\)
    DESTDIR_WIN = $$replace(DESTDIR, /, \\)
    INSTALL_STAGE_BAT_SRC = $$PWD/install_stage.bat
    INSTALL_STAGE_BAT = $$replace(INSTALL_STAGE_BAT_SRC, /, \\)

    QMAKE_POST_LINK += $$RETURN cmd /c $$INSTALL_STAGE_BAT $$DESTDIR_WIN $$INSTALL_SRC_DIR_WIN
}

DISTFILES += \
    README.md \
    Revisions.md \
    aslrequest.txt \
    install_linux.sh \
    install_stage.bat \
    AppImage/README.md \
    AppImage/build_appimage.sh \
    AppImage/AmigaED.desktop \
    AppImage/AmigaED.png \
    Debian/build_deb.sh \
    Debian/README.md \
    syntax-examples/README.txt \
    syntax-examples/pascal_boublesort.pas \
    syntax-examples/fortran-example.f \
    syntax-examples/amiga_installer \
    syntax-examples/README.txt \
    syntax-examples/fortran-example.f \
    syntax-examples/Makefile_sample.mak \
    syntax-examples/pascal_boublesort.pas \
    abandonned_methods.txt \
    images/btn_next.png \
    images/btn_prev.png \
    images/find-and-replace.png \
    images/home.png

FORMS += \
    prefsdialog.ui \
    aboutdialog.ui
