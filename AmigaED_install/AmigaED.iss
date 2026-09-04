; AmigaED Setup script (Inno Setup)
;
; Expected layout when this file is compiled:
;   AmigaED_install\AmigaED.iss      <- this file
;   AmigaED_install\install_src\     <- AmigaED.exe, all DLLs and plugin
;                                        subfolders, staged automatically
;                                        by AmigaED.pro's win32 build step
;                                        after every successful build
;
; Compile with the Inno Setup Compiler (ISCC.exe) or by opening this file
; in the Inno Setup IDE and choosing Build > Compile. The result is a
; single AmigaED_Setup.exe in AmigaED_install\Output\.
;
; IMPORTANT: AppId below is a fixed GUID that identifies "AmigaED" to
; Windows' installed-programs registry across versions. Do NOT regenerate
; it for future releases - a changed AppId would make Windows (and this
; script's own "already installed?" check) treat a new build as a
; completely different, unrelated program, breaking upgrades/uninstalls
; of anything installed with an earlier AppId.

#define MyAppName "AmigaED"
#define MyAppVersion "4.0"
#define MyAppPublisher "MB-SoftWorX"
#define MyAppExeName "AmigaED.exe"
#define MyAppId "B3B6B6C1-6C8E-4B2C-9C36-6E9C6E5B7F3A"

[Setup]
AppId={{{#MyAppId}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
; C:\AmigaED, as asked - not the usual "{autopf}\..." location, so this
; intentionally does NOT use the standard Program Files suggestion.
DefaultDirName=C:\AmigaED
; Let the user change the path on the wizard's directory-selection page
; instead of just accepting the default silently.
DisableDirPage=no
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
; The staged output is the sole source tree for [Files] below - every
; relative Source: path is resolved against this folder.
SourceDir=install_src
; Relative directives (this one included) resolve against SourceDir,
; not against this .iss file's own folder - so "..\Output" lands at
; AmigaED_install\Output, a sibling of install_src. Deliberately NOT
; "Output" (i.e. install_src\Output): install_stage.bat wipes install_src
; completely on every build (see AmigaED.pro), which would delete any
; previously compiled Setup.exe sitting inside it right along with the
; rest.
OutputDir=..\Output
OutputBaseFilename=AmigaED_Setup
Compression=lzma
SolidCompression=yes
WizardStyle=modern
; AmigaED.pro's win32 build targets 64-bit MinGW; matches windeployqt's
; own output, which is 64-bit-only (no 32-bit DLLs are staged).
ArchitecturesInstallIn64BitMode=x64compatible
; Writing to C:\AmigaED (outside the user's own profile) needs elevation
; on a standard Windows setup, same as most installers that don't target
; {localappdata}/{userappdata}.
PrivilegesRequired=admin
UninstallDisplayIcon={app}\{#MyAppExeName}
; Shown on the left of the Welcome and Finished pages (164x314 px) and,
; scaled down, top-right on every other page (55x58 px). Place your own
; wizard_image.png / wizard_image_small.png directly in this folder
; (AmigaED_install, next to this .iss file) - install_src itself gets
; wiped and rebuilt on every compile, so an image dropped in there
; wouldn't survive the next build. Each directive only takes effect if
; its file actually exists, so compiling still works even if you've
; only supplied one of the two (or neither yet).
#if FileExists(SourcePath + "wizard_image.png")
WizardImageFile={#SourcePath}wizard_image.png
#endif
#if FileExists(SourcePath + "wizard_image_small.png")
WizardSmallImageFile={#SourcePath}wizard_image_small.png
#endif
; The Welcome page (shown first, before any file copying) already shows
; the large WizardImageFile above and offers exactly "Next" (proceed
; with the install) or "Cancel" (abort, nothing changed) - Inno Setup's
; built-in behaviour, not something that needs building by hand. Made
; explicit here rather than left as an implicit default, so it's clear
; this satisfies "ein Willkommensfenster mit Bild, Installieren oder
; Abbrechen" on its own.
DisableWelcomePage=no

[Languages]
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; Everything staged by the build (the .exe, every DLL, and every plugin
; subfolder windeployqt created) - copied as-is, subfolder structure
; preserved.
Source: "*"; DestDir: "{app}"; Flags: recursesubdirs createallsubdirs ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#MyAppName}}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
; Belt-and-braces cleanup: removes the install folder even if it still
; contains files [Files] above didn't track (e.g. left over from a
; future manual copy) so a reinstall always starts from a clean folder.
Type: filesandordirs; Name: "{app}"

; --- Install-or-uninstall prompt -------------------------------------
; Runs before the wizard shows any page. If AmigaED is already
; installed (its uninstall registry key exists), asks the user whether
; they want to (re)install or uninstall instead of just barrelling into
; a reinstall - and if they choose uninstall, hands off to the existing
; uninstaller and exits without ever showing the install wizard.
[Code]
function InitializeSetup(): Boolean;
var
  UninstallString: String;
  UninstallRegKey: String;
  ResultCode: Integer;
  UserChoice: Integer;
begin
  Result := True;

  UninstallRegKey := 'Software\Microsoft\Windows\CurrentVersion\Uninstall\{{#MyAppId}}_is1';

  if RegQueryStringValue(HKLM, UninstallRegKey, 'UninstallString', UninstallString) or
     RegQueryStringValue(HKCU, UninstallRegKey, 'UninstallString', UninstallString) then
  begin
    UserChoice := MsgBox(
      '{#MyAppName} ist bereits installiert.' + #13#10 + #13#10 +
      'Möchten Sie {#MyAppName} erneut installieren (Ja) oder deinstallieren (Nein)?' + #13#10 +
      '(Abbrechen beendet das Setup, ohne etwas zu ändern.)',
      mbConfirmation, MB_YESNOCANCEL);

    if UserChoice = IDNO then
    begin
      // Strip any surrounding quotes Inno wrote around the path itself.
      UninstallString := RemoveQuotes(UninstallString);
      if Exec(UninstallString, '', '', SW_SHOW, ewWaitUntilTerminated, ResultCode) then
      begin
        // The uninstaller runs standalone; nothing more for this setup
        // to do either way, whether it succeeded or the user cancelled
        // it partway through.
      end
      else
      begin
        MsgBox('Die Deinstallation konnte nicht gestartet werden.', mbError, MB_OK);
      end;
      Result := False; // never show the install wizard in this run
    end
    else if UserChoice = IDCANCEL then
    begin
      Result := False;
    end;
    // IDYES: Result stays True, falls through into the normal install
    // wizard (acts as a reinstall/repair/upgrade).
  end;
end;
