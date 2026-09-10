#ifndef VERSION_H
#define VERSION_H

//
// Single place to bump the revision number - every window title in the
// app (main window, About dialog, Prefs dialog, and every message box
// title) picks it up from AMIGAED_VERSION_STRING below, so there's
// nothing else to keep in sync by hand when starting a new revision.
//
#define AMIGAED_REVISION 151

#define AMIGAED_STRINGIFY2(x) #x
#define AMIGAED_STRINGIFY(x) AMIGAED_STRINGIFY2(x)

// e.g. "AmigaED 4.0 rev.38" - a plain C string literal (built entirely
// by the preprocessor), so it's safe to pass straight into tr().
#define AMIGAED_VERSION_STRING "AmigaED 4.0 rev." AMIGAED_STRINGIFY(AMIGAED_REVISION)

//
// QSettings identity - single place to bump if this ever needs to
// change again. Every QSettings(...) call in the app (explicit ones in
// mainwindow.cpp, and PrefsDialog's own default-constructed
// "mySettings", which resolves via QApplication::setOrganizationName()/
// setApplicationName() in main.cpp) must use these same two strings -
// otherwise prefs silently split across two different files/registry
// keys. AMIGAED_SETTINGS_APP was renamed from "Amiga Cross Editor" (the
// app's original working title) to "AmigaED4" in rev.149, since the app
// itself had long since been renamed to AmigaED - see
// MainWindow::migrateLegacySettingsIfNeeded() for the one-time startup
// migration that carries existing users' settings over to the new file
// without making them re-enter everything.
//
#define AMIGAED_SETTINGS_ORG "MB-SoftWorX"
#define AMIGAED_SETTINGS_APP "AmigaED4"

// The pre-rev.149 application name, kept ONLY so
// migrateLegacySettingsIfNeeded() can still find and read an existing
// user's old settings file/registry key on their first startup after
// upgrading. Never used for anything else - do not read/write current
// settings through this name.
#define AMIGAED_LEGACY_SETTINGS_APP "Amiga Cross Editor"

#endif // VERSION_H
