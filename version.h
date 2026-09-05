#ifndef VERSION_H
#define VERSION_H

//
// Single place to bump the revision number - every window title in the
// app (main window, About dialog, Prefs dialog, and every message box
// title) picks it up from AMIGAED_VERSION_STRING below, so there's
// nothing else to keep in sync by hand when starting a new revision.
//
#define AMIGAED_REVISION 94

#define AMIGAED_STRINGIFY2(x) #x
#define AMIGAED_STRINGIFY(x) AMIGAED_STRINGIFY2(x)

// e.g. "AmigaED 4.0 rev.38" - a plain C string literal (built entirely
// by the preprocessor), so it's safe to pass straight into tr().
#define AMIGAED_VERSION_STRING "AmigaED 4.0 rev." AMIGAED_STRINGIFY(AMIGAED_REVISION)

#endif // VERSION_H
