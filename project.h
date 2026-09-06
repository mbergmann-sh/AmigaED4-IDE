#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QList>

//
// A project groups a set of source files that belong together (an Amiga
// C/C++ application), plus the metadata AmigaED needs to build them:
// which file is the "main" compile unit, and any extra compiler/linker
// options the user wants passed to each toolchain's own Makefile.
//
// Persistence format: a plain .ini-style file (suffix ".aep"), written
// via QSettings(fileName, QSettings::IniFormat) - human-readable, no
// extra dependency, consistent with how AmigaED already stores its own
// settings (QSettings elsewhere in mainwindow.cpp).
//

enum class ProjectFileType
{
    CSource = 0,          // .c / .cpp
    Header = 1,            // .h / .hpp
    InstallerScript = 2,   // Amiga Installer script
    Other = 3,               // anything else added to the project
    AmigaGuide = 4,          // .guide (AmigaGuide hypertext) - appended, not inserted before
                             // Other, so existing saved .aep files' stored "type: 3" values
                             // (Other) keep meaning Other instead of silently becoming this
    Assembly = 5,            // .asm / .s (m68k assembler) - appended for the same reason
    Executable = 6           // a compiled Amiga executable (Motorola hunk format) - appended for the same reason
};

struct ProjectFile
{
    QString path;   // absolute path
    ProjectFileType type = ProjectFileType::Other;

    bool operator==(const ProjectFile &other) const { return path == other.path; }
};

class Project
{
public:
    QString name;                    // project display name
    QString projectFilePath;         // absolute path of the .aep file itself
    QString mainFile;                // absolute path of the file to compile/link as the program's main C/C++ unit
    // Extra compiler/linker options are kept SEPARATE per toolchain -
    // Makefile.gcc and Makefile.vbcc use fundamentally different, mutually
    // incompatible option syntaxes (e.g. VBCC's "+aos68k -c99" vs GCC's
    // "-std=gnu89"), so a value meant for one would actively break a
    // build with the other if shared. Makefile.gcc always invokes gcc
    // itself (see MainWindow::regenerateProjectMakefiles()) - even when
    // G++ is the app's currently selected compiler - so there's no
    // separate G++ variant of these.
    QString extraGccCompilerOptions;    // appended to Makefile.gcc's CFLAGS
    QString extraGccLinkerOptions;      // appended to Makefile.gcc's LDFLAGS
    QString extraVbccCompilerOptions;   // appended to Makefile.vbcc's CFLAGS
    QString extraVbccLinkerOptions;     // appended to Makefile.vbcc's LDFLAGS

    // SHA-1 (hex) of each Makefile's content exactly as AmigaED itself
    // last wrote it - NOT of whatever is currently on disk. Lets
    // MainWindow::regenerateProjectMakefiles() tell "unchanged since we
    // last wrote it" (safe to regenerate/overwrite freely) apart from
    // "the user has hand-edited it since" (must NOT be silently
    // overwritten) - confirmed a real need: with no such check, ANY
    // manual edit to a generated Makefile was always undone the very
    // next time regeneration ran (every file add/remove, and - since
    // rev.128 - before every build too), with no way to make a change
    // stick. Empty until the corresponding Makefile has actually been
    // (re)written at least once.
    QString lastWrittenGccMakefileHash;
    QString lastWrittenVbccMakefileHash;
    QString lastWrittenScMakefileHash;

    QList<ProjectFile> files;

    // Which "New Project" template this was created from (see MainWindow's
    // createNewProject(): 0=Empty Amiga C, 1=Shell, 2=AmigaOS 1.3,
    // 3=AmigaOS 3.x, 4=ReAction, 5=MUI), or -1 if unknown (a hand-
    // assembled .aep, or one saved before this field existed). Some
    // toolchain specifics depend on this - e.g. an AmigaOS 1.3 project's
    // vbcc Makefile must always use '+kick13', regardless of whatever the
    // user's global VBCC preference happens to be set to - see
    // MainWindow::regenerateProjectMakefiles().
    int templateKind = -1;

    // Directory the .aep lives in - the natural base directory for the
    // project's Makefiles and relative source paths.
    QString projectDir() const;

    void addFile(const QString &filePath);          // no-op if already present
    void removeFile(const QString &filePath);
    bool contains(const QString &filePath) const;

    bool save(const QString &fileName);              // writes this->projectFilePath = fileName on success
    bool load(const QString &fileName);               // replaces all members from the file on success

    // Guess a file's project category from its extension - used both when
    // adding a file manually and when a new project template's main file
    // is created.
    static ProjectFileType typeForFile(const QString &filePath);
};

#endif // PROJECT_H
