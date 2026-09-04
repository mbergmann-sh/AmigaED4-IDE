#include "project.h"
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QFile>

QString Project::projectDir() const
{
    return QFileInfo(projectFilePath).absolutePath();
}

//
// True if filePath starts with the AmigaOS "hunk" executable format's
// magic number (0x000003F3) - i.e. it genuinely IS a compiled Amiga
// executable, regardless of what it's named or which platform AmigaED
// is running on. Needed because an extension-less binary produced by a
// FOREIGN (non-AmigaED) Amiga project - the case typeForFile() below
// otherwise can't distinguish from an Installer script - has no other
// reliable identifying trait: it usually has no extension, and (unlike
// on a Unix host) Windows has no concept of a "+x executable" permission
// bit for it to fall back on either.
//
static bool isAmigaHunkExecutable(const QString &filePath)
{
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    unsigned char magic[4];
    if (f.read(reinterpret_cast<char *>(magic), 4) != 4)
        return false;

    return magic[0] == 0x00 && magic[1] == 0x00 && magic[2] == 0x03 && magic[3] == 0xF3;
}

ProjectFileType Project::typeForFile(const QString &filePath)
{
    QFileInfo info(filePath);
    QString ext = info.suffix().toLower();

    if (ext == "c" || ext == "cpp" || ext == "cc" || ext == "cxx" || ext == "c++")
        return ProjectFileType::CSource;

    if (ext == "h" || ext == "hpp" || ext == "hxx" || ext == "h++")
        return ProjectFileType::Header;

    if (ext == "asm" || ext == "s")
        return ProjectFileType::Assembly;

    if (ext == "guide")
        return ProjectFileType::AmigaGuide;

    if (ext.isEmpty())
    {
        // A handful of well-known extension-less SAS/C build-artifact/
        // config files (commonly seen when importing a foreign SAS/C
        // project - see MainWindow::importExistingProject()) aren't
        // Installer scripts - they belong under "Other Files" instead,
        // like anything else that isn't clearly one of the recognized
        // types.
        QString bare = info.fileName();
        if (bare.compare("debug", Qt::CaseInsensitive) == 0 ||
            bare.compare("scoptions", Qt::CaseInsensitive) == 0)
            return ProjectFileType::Other;

        // A compiled Amiga executable, extension-less like an Installer
        // script - checked BEFORE the Installer fallback below, via its
        // file content (see isAmigaHunkExecutable()) rather than its name,
        // since there's no naming convention to go on otherwise.
        if (isAmigaHunkExecutable(filePath))
            return ProjectFileType::Executable;

        // Amiga Installer scripts conventionally have no extension at all
        // either (e.g. an AmigaGuide-style "Install" script) - the
        // remaining, most common reason for a project to contain an
        // extension-less file, so it's the fallback here.
        return ProjectFileType::InstallerScript;
    }

    if (ext == "installer")
        return ProjectFileType::InstallerScript;

    return ProjectFileType::Other;
}

bool Project::contains(const QString &filePath) const
{
    QFileInfo target(filePath);
    for (const ProjectFile &f : files)
    {
        if (QFileInfo(f.path) == target)
            return true;
    }
    return false;
}

void Project::addFile(const QString &filePath)
{
    if (contains(filePath))
        return;

    ProjectFile f;
    f.path = filePath;
    f.type = typeForFile(filePath);
    files.append(f);
}

void Project::removeFile(const QString &filePath)
{
    QFileInfo target(filePath);
    for (int i = 0; i < files.count(); ++i)
    {
        if (QFileInfo(files.at(i).path) == target)
        {
            files.removeAt(i);
            if (QFileInfo(mainFile) == target)
                mainFile.clear();   // the main file was just removed from the project
            return;
        }
    }
}

bool Project::save(const QString &fileName)
{
    QSettings settings(fileName, QSettings::IniFormat);
    if (settings.status() != QSettings::NoError)
        return false;

    settings.clear();

    settings.beginGroup("Project");
    settings.setValue("Name", name);
    settings.setValue("MainFile", mainFile);
    settings.setValue("ExtraGccCompilerOptions", extraGccCompilerOptions);
    settings.setValue("ExtraGccLinkerOptions", extraGccLinkerOptions);
    settings.setValue("ExtraVbccCompilerOptions", extraVbccCompilerOptions);
    settings.setValue("ExtraVbccLinkerOptions", extraVbccLinkerOptions);
    settings.setValue("TemplateKind", templateKind);
    settings.endGroup();

    settings.beginWriteArray("Files");
    for (int i = 0; i < files.count(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", files.at(i).path);
        settings.setValue("type", static_cast<int>(files.at(i).type));
    }
    settings.endArray();

    settings.sync();
    if (settings.status() != QSettings::NoError)
        return false;

    projectFilePath = fileName;
    return true;
}

bool Project::load(const QString &fileName)
{
    if (!QFileInfo::exists(fileName))
        return false;

    QSettings settings(fileName, QSettings::IniFormat);
    if (settings.status() != QSettings::NoError)
        return false;

    QString loadedName;
    QString loadedMainFile;
    QString loadedGccCompilerOpts;
    QString loadedGccLinkerOpts;
    QString loadedVbccCompilerOpts;
    QString loadedVbccLinkerOpts;
    int loadedTemplateKind = -1;
    QList<ProjectFile> loadedFiles;

    settings.beginGroup("Project");
    loadedName = settings.value("Name").toString();
    loadedMainFile = settings.value("MainFile").toString();
    loadedGccCompilerOpts = settings.value("ExtraGccCompilerOptions").toString();
    loadedGccLinkerOpts = settings.value("ExtraGccLinkerOptions").toString();
    loadedVbccCompilerOpts = settings.value("ExtraVbccCompilerOptions").toString();
    loadedVbccLinkerOpts = settings.value("ExtraVbccLinkerOptions").toString();
    loadedTemplateKind = settings.value("TemplateKind", -1).toInt();
    settings.endGroup();

    int count = settings.beginReadArray("Files");
    for (int i = 0; i < count; ++i)
    {
        settings.setArrayIndex(i);
        ProjectFile f;
        f.path = settings.value("path").toString();
        f.type = static_cast<ProjectFileType>(settings.value("type").toInt());
        if (!f.path.isEmpty())
            loadedFiles.append(f);
    }
    settings.endArray();

    // Only commit once everything parsed without error, so a partially
    // unreadable file doesn't leave the in-memory project half-updated.
    name = loadedName;
    mainFile = loadedMainFile;
    extraGccCompilerOptions = loadedGccCompilerOpts;
    extraGccLinkerOptions = loadedGccLinkerOpts;
    extraVbccCompilerOptions = loadedVbccCompilerOpts;
    extraVbccLinkerOptions = loadedVbccLinkerOpts;
    templateKind = loadedTemplateKind;
    files = loadedFiles;
    projectFilePath = fileName;

    return true;
}
