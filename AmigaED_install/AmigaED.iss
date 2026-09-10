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

; Whether wizard_image.png (see the WizardImageFile comment further
; below) is available to also show inside the license disclaimer dialog
; in [Code]. Defined at compile time so both the [Files] entry that
; stages it into {tmp} and the Pascal code that loads it can be left out
; together, cleanly, if the file isn't there - same "only takes effect
; if the file actually exists" approach already used for
; WizardImageFile/WizardSmallImageFile below.
#if FileExists(SourcePath + "wizard_image.png")
  #define HasDisclaimerImage
#endif

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
OutputBaseFilename=AmigaED4_rev151_Setup
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

[CustomMessages]
; Used by the documentation Yes/No prompt in [Code] below - {cm:...}/
; CustomMessage() automatically picks whichever of these two matches the
; language chosen on Setup's language-selection page, same mechanism the
; built-in {cm:CreateDesktopIcon} etc. already rely on elsewhere in this
; script.
english.InstallDocsQuestion=Would you like to install the documentation (manual as PDF)?
german.InstallDocsQuestion=Möchten Sie die Dokumentation (Handbuch als PDF) installieren?

; Used by the custom "already installed" dialog (AskInstallChoice in
; [Code] below) instead of a plain Yes/No/Cancel MsgBox - a stock MsgBox
; can't carry custom button captions, and a real 3-way choice (Reinstall
; / Update / Uninstall) needs more than Yes/No/Cancel provides anyway.
english.AlreadyInstalledCaption=%1 is already installed
english.AlreadyInstalledText=What would you like to do?
english.BtnReinstall=&Reinstall (keep old files, install over them)
english.BtnUpdate=&Update (remove the old version first, then install fresh)
english.BtnUninstall=&Uninstall %1
english.BtnCancel=Cancel
english.UninstallFailed=The uninstallation could not be started.
german.AlreadyInstalledCaption=%1 ist bereits installiert
german.AlreadyInstalledText=Was möchten Sie tun?
german.BtnReinstall=&Neu installieren (alte Dateien behalten, drüberinstallieren)
german.BtnUpdate=&Update (alte Version zuerst entfernen, dann neu installieren)
german.BtnUninstall=%1 &deinstallieren
german.BtnCancel=Abbrechen
german.UninstallFailed=Die Deinstallation konnte nicht gestartet werden.

; Used by the license disclaimer dialog (ShowDisclaimer in [Code] below),
; shown right after Setup's language-selection page and before anything
; else - including the "already installed?" check further down - so
; declining it stops the installer in its tracks no matter what else
; might otherwise happen first.
english.DisclaimerCaption=License Disclaimer
; %n inserts a real line break here (Inno Setup's own message-file escape
; for [CustomMessages] entries, not a Pascal string) - keeps the fixed-
; width disclaimer dialog from wrapping this long text awkwardly; %n%n
; is a full blank line between paragraphs. See ShowDisclaimer() below for
; the dialog/label sizing that was enlarged to fit this longer text.
english.DisclaimerText=AmigaED 4.0 is distributed under the GNU Lesser General Public License, with one additional restriction:%nfascists, racists, AfD voters and members, as well as%nMAGA supporters and Putin sympathizers, are not permitted to use this program!%n%nA free world needs free software - and free people.%n%nFor a tolerant, peaceful and diverse world!%n%nMichael Bergmann%n%nAuthor of AmigaED 4.0
english.BtnAccept=&Accept
english.BtnDecline=&Decline
german.DisclaimerCaption=Lizenz-Hinweis
german.DisclaimerText=AmigaED 4.0 steht unter der GNU Lesser License mit einer Einschränkung:%nFaschisten, Rassisten, AfD-Wähler und -mitglieder, sowie%nMAGA-Befürworter und Putin-Fans dürfen dieses Programm nicht verwenden!%n%nEine freie Welt braucht freie Software - und freie Menschen.%n%nFür eine tolerante, friedliche und vielfältige Welt!%n%nMichael Bergmann%n%nAutor von AmigaED 4.0
german.BtnAccept=&Akzeptieren
german.BtnDecline=&Ablehnen

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; Everything staged by the build (the .exe, every DLL, and every plugin
; subfolder windeployqt created) - copied as-is, subfolder structure
; preserved. DOC (the manuals) is excluded here and handled by its own
; entry below, gated by the Yes/No prompt in [Code].
Source: "*"; DestDir: "{app}"; Excludes: "DOC\*"; Flags: recursesubdirs createallsubdirs ignoreversion
; Documentation (DE/EN PDF manuals), staged into install_src\DOC by
; AmigaED.pro's win32 build step (see install_stage.bat). Only copied to
; the chosen install directory if the user answers "Yes" to the
; documentation prompt asked from InitializeSetup below.
Source: "DOC\*"; DestDir: "{app}\DOC"; Flags: recursesubdirs createallsubdirs ignoreversion; Check: ShouldInstallDocs
; Staged into {tmp} (not {app} - never installed alongside the program)
; purely so the license disclaimer dialog in [Code] can load it at
; runtime. Only present if HasDisclaimerImage was defined above.
#ifdef HasDisclaimerImage
Source: "{#SourcePath}wizard_image.png"; DestDir: "{tmp}"; Flags: dontcopy noencryption
#endif

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

; --- Install-or-uninstall-or-update prompt -----------------------------
; Runs before the wizard shows any page. If AmigaED is already installed
; (its uninstall registry key exists), asks the user what to do instead
; of just barrelling into a reinstall: reinstall over the top, do a
; clean update (uninstall first, then install fresh), or uninstall only.
; A plain uninstall or a cancelled dialog exits without ever showing the
; install wizard.
[Code]
var
  // Set from the Yes/No prompt in InitializeSetup below; read back by
  // ShouldInstallDocs() as the Check: for the DOC entry in [Files]. By
  // the time InitializeSetup runs, Inno has already resolved the active
  // setup language (via its language-selection page or /LANG), so
  // {cm:InstallDocsQuestion} below always comes out in that language.
  InstallDocs: Boolean;

  // Set by the button click handlers below; AskInstallChoice() reads it
  // back once the dialog closes. 1 = reinstall over the top, 2 = clean
  // update (uninstall then reinstall), 3 = uninstall only, 0 = cancel
  // (including the dialog being closed via its title bar/Esc).
  InstallChoiceResult: Integer;
  InstallChoiceForm: TSetupForm;
  // Running Y position while AddInstallChoiceButton stacks buttons onto
  // InstallChoiceForm - a plain global instead of a local var, since
  // Inno Setup's Pascal Script does not support nested functions (a
  // helper local to AskInstallChoice, as this used to be, fails to
  // compile with "'BEGIN' expected").
  InstallChoiceBtnTop: Integer;

  // Set by the button click handlers below; ShowDisclaimer() reads it
  // back once the dialog closes. True = Accept (installation continues),
  // False = Decline or the dialog closed any other way (Esc/title bar) -
  // same "closing it any other way counts as the safe/negative choice"
  // convention as InstallChoiceResult above.
  DisclaimerAccepted: Boolean;
  DisclaimerForm: TSetupForm;

function ShouldInstallDocs(): Boolean;
begin
  Result := InstallDocs;
end;

// One shared handler for all four buttons - simpler than four near-
// identical procedures, since every button already carries its own
// result value via its Tag property (set where each button is created
// in AddInstallChoiceButton below).
procedure InstallChoiceButtonClick(Sender: TObject);
begin
  InstallChoiceResult := TNewButton(Sender).Tag;
  InstallChoiceForm.Close;
end;

// Creates one button on InstallChoiceForm, stacked below the previous
// one via InstallChoiceBtnTop, wired to AResultValue through its Tag.
// A top-level function rather than nested inside AskInstallChoice below
// - see the InstallChoiceBtnTop comment above for why.
function AddInstallChoiceButton(const ACaption: String; AResultValue: Integer): TNewButton;
begin
  Result := TNewButton.Create(InstallChoiceForm);
  Result.Parent := InstallChoiceForm;
  Result.Left := ScaleX(16);
  Result.Top := InstallChoiceBtnTop;
  Result.Width := InstallChoiceForm.ClientWidth - ScaleX(32);
  Result.Height := ScaleY(23);
  Result.Caption := ACaption;
  Result.Tag := AResultValue;
  Result.OnClick := @InstallChoiceButtonClick;
  InstallChoiceBtnTop := InstallChoiceBtnTop + ScaleY(30);
end;

// Small custom dialog replacing the old three-way Yes/No/Cancel MsgBox:
// a stock MsgBox can't carry custom button captions, and a real 3-way
// choice (Reinstall / Update / Uninstall) plus Cancel needs more than
// Yes/No/Cancel provides. All captions come from [CustomMessages] above,
// so - like the documentation prompt - this always shows in whichever
// language the user picked on Setup's language-selection page.
function AskInstallChoice(): Integer;
var
  Btn: TNewButton;
  InfoLabel: TNewStaticText;
begin
  InstallChoiceResult := 0; // closing the form any other way still counts as Cancel

  // CreateCustomForm's signature changed in Inno Setup 6.6.0: ClientWidth/
  // ClientHeight are now constructor parameters (read-only afterwards)
  // instead of settable properties, plus two new KeepSizeX/KeepSizeY
  // parameters (whether the form may grow with WizardSizePercent) -
  // False/False keeps this dialog a fixed size, matching the old
  // behaviour from before 6.6.0.
  InstallChoiceForm := CreateCustomForm(ScaleX(420), ScaleY(210), False, False);
  try
    InstallChoiceForm.Caption := ExpandConstant('{cm:AlreadyInstalledCaption,{#MyAppName}}');
    InstallChoiceForm.Position := poScreenCenter;
    InstallChoiceForm.BorderStyle := bsDialog;

    InfoLabel := TNewStaticText.Create(InstallChoiceForm);
    InfoLabel.Parent := InstallChoiceForm;
    InfoLabel.Left := ScaleX(16);
    InfoLabel.Top := ScaleY(16);
    InfoLabel.Width := InstallChoiceForm.ClientWidth - ScaleX(32);
    InfoLabel.AutoSize := False;
    InfoLabel.WordWrap := True;
    InfoLabel.Caption := ExpandConstant('{cm:AlreadyInstalledText}');

    InstallChoiceBtnTop := ScaleY(60);
    AddInstallChoiceButton(ExpandConstant('{cm:BtnReinstall}'), 1);
    AddInstallChoiceButton(ExpandConstant('{cm:BtnUpdate}'), 2);
    Btn := AddInstallChoiceButton(ExpandConstant('{cm:BtnUninstall,{#MyAppName}}'), 3);
    Btn := AddInstallChoiceButton(ExpandConstant('{cm:BtnCancel}'), 0);
    Btn.Cancel := True; // Esc / closing the dialog acts as Cancel

    InstallChoiceForm.ActiveControl := Btn;
    InstallChoiceForm.ShowModal;
  finally
    InstallChoiceForm.Free;
  end;

  Result := InstallChoiceResult;
end;

// --- License disclaimer dialog -----------------------------------------
// Shown right after the language-selection page, before Setup does
// anything else. Two buttons, Accept/Decline (captions from
// [CustomMessages] above, so they follow the chosen Setup language) -
// deliberately a custom form rather than a stock MsgBox, since a plain
// MsgBox's OK/Cancel captions can't be renamed to "Accept"/"Decline".
procedure DisclaimerButtonClick(Sender: TObject);
begin
  DisclaimerAccepted := (TNewButton(Sender).Tag = 1);
  DisclaimerForm.Close;
end;

function ShowDisclaimer(): Boolean;
var
  InfoLabel: TNewStaticText;
  AcceptBtn, DeclineBtn: TNewButton;
  BtnWidth: Integer;
  TextLeft: Integer;
  ContentBottom: Integer;
#ifdef HasDisclaimerImage
  DisclaimerImage: TBitmapImage;
#endif
begin
  DisclaimerAccepted := False; // closing the form any other way still counts as Decline

  // Width is the one fixed number here (kept deliberately narrow - see
  // TextLeft/InfoLabel below); height is never hand-guessed: InfoLabel
  // below computes its own exact wrapped height for whatever text/font/
  // DPI this particular system renders, and the form is resized to fit
  // that plus the button row right after - so the dialog is always
  // exactly as tall as the current DisclaimerText needs, no magic
  // number to keep re-tuning by hand every time the text changes.
  // Placeholder height here only - overwritten via ClientHeight below.
  DisclaimerForm := CreateCustomForm(ScaleX(460), ScaleY(300), False, False);
  try
    DisclaimerForm.Caption := ExpandConstant('{cm:DisclaimerCaption}');
    DisclaimerForm.Position := poScreenCenter;
    DisclaimerForm.BorderStyle := bsDialog;

#ifdef HasDisclaimerImage
    // Reuses the same wizard_image.png already configured as
    // WizardImageFile above - extracted into {tmp} here purely so this
    // dialog's own TBitmapImage can load it (TBitmapImage.PngImage
    // loads PNG directly; no BMP conversion needed). Stretched into a
    // fixed-width column on the left (Height is set below, once
    // InfoLabel's own real height is known), same idea as the Welcome
    // page's own large wizard image.
    DisclaimerImage := TBitmapImage.Create(DisclaimerForm);
    DisclaimerImage.Parent := DisclaimerForm;
    DisclaimerImage.Left := ScaleX(16);
    DisclaimerImage.Top := ScaleY(16);
    DisclaimerImage.Width := ScaleX(110);
    DisclaimerImage.Stretch := True;
    DisclaimerImage.Center := True;
    ExtractTemporaryFile('wizard_image.png');
    DisclaimerImage.PngImage.LoadFromFile(ExpandConstant('{tmp}\wizard_image.png'));
    TextLeft := DisclaimerImage.Left + DisclaimerImage.Width + ScaleX(16);
#else
    TextLeft := ScaleX(16);
#endif

    InfoLabel := TNewStaticText.Create(DisclaimerForm);
    InfoLabel.Parent := DisclaimerForm;
    InfoLabel.Left := TextLeft;
    InfoLabel.Top := ScaleY(16);
    InfoLabel.AutoSize := False;
    InfoLabel.WordWrap := True;
    // Width fixed first, Caption set next, AutoSize turned on last: with
    // WordWrap already True and a Width already in place, AutoSize now
    // only grows/shrinks Height to exactly fit the wrapped text - it
    // does NOT touch Width - the standard Delphi/Inno "fixed width,
    // auto height" label technique (TNewStaticText descends from
    // TCustomLabel, same as a plain TLabel here).
    InfoLabel.Width := DisclaimerForm.ClientWidth - TextLeft - ScaleX(16);
    InfoLabel.Caption := ExpandConstant('{cm:DisclaimerText}');
    InfoLabel.AutoSize := True;

    ContentBottom := InfoLabel.Top + InfoLabel.Height;

#ifdef HasDisclaimerImage
    // Match the image column to whichever this text needs - keeps the
    // book graphic from looking squashed (short text) or stunted (long
    // text) relative to it.
    DisclaimerImage.Height := InfoLabel.Height;
    if DisclaimerImage.Top + DisclaimerImage.Height > ContentBottom then
      ContentBottom := DisclaimerImage.Top + DisclaimerImage.Height;
#endif

    // Resize the form to exactly fit what's above plus one button row -
    // this is the only place the dialog's final height is decided.
    DisclaimerForm.ClientHeight := ContentBottom + ScaleY(16) + ScaleY(23) + ScaleY(20);

    // Fixed, compact width (not "half the dialog each") - was stretching
    // both buttons across almost the entire dialog once the dialog got
    // wide enough for the disclaimer text. Right-aligned as a pair,
    // Windows-dialog-style, instead of starting flush at the left edge.
    BtnWidth := ScaleX(130);

    DeclineBtn := TNewButton.Create(DisclaimerForm);
    DeclineBtn.Parent := DisclaimerForm;
    DeclineBtn.Left := DisclaimerForm.ClientWidth - ScaleX(16) - BtnWidth;
    DeclineBtn.Top := ContentBottom + ScaleY(16);
    DeclineBtn.Width := BtnWidth;
    DeclineBtn.Height := ScaleY(23);
    DeclineBtn.Caption := ExpandConstant('{cm:BtnDecline}');
    DeclineBtn.Tag := 0;
    DeclineBtn.OnClick := @DisclaimerButtonClick;
    DeclineBtn.Cancel := True; // Esc / closing the dialog acts as Decline

    AcceptBtn := TNewButton.Create(DisclaimerForm);
    AcceptBtn.Parent := DisclaimerForm;
    AcceptBtn.Left := DeclineBtn.Left - ScaleX(10) - BtnWidth;
    AcceptBtn.Top := DeclineBtn.Top;
    AcceptBtn.Width := BtnWidth;
    AcceptBtn.Height := ScaleY(23);
    AcceptBtn.Caption := ExpandConstant('{cm:BtnAccept}');
    AcceptBtn.Tag := 1;
    AcceptBtn.OnClick := @DisclaimerButtonClick;

    // Decline is the active/default control - closing the dialog any
    // way other than an explicit Accept click is the safe outcome.
    DisclaimerForm.ActiveControl := DeclineBtn;
    DisclaimerForm.ShowModal;
  finally
    DisclaimerForm.Free;
  end;

  Result := DisclaimerAccepted;
end;

function InitializeSetup(): Boolean;
var
  UninstallString: String;
  UninstallRegKey: String;
  ResultCode: Integer;
  Choice: Integer;
begin
  Result := True;

  // License disclaimer - shown first, right after the language-selection
  // page (Inno Setup has already resolved the active language by the
  // time InitializeSetup runs, so {cm:DisclaimerText} etc. below come
  // out in that language). Declining exits Setup immediately, before
  // the "already installed?" check or anything else gets a chance to
  // run.
  if not ShowDisclaimer() then
  begin
    Result := False;
    Exit;
  end;

  UninstallRegKey := 'Software\Microsoft\Windows\CurrentVersion\Uninstall\{{#MyAppId}}_is1';

  if RegQueryStringValue(HKLM, UninstallRegKey, 'UninstallString', UninstallString) or
     RegQueryStringValue(HKCU, UninstallRegKey, 'UninstallString', UninstallString) then
  begin
    Choice := AskInstallChoice();
    UninstallString := RemoveQuotes(UninstallString);

    case Choice of
      1:
        begin
          // Reinstall over the top: Result stays True, falls through
          // into the normal install wizard, same as before.
        end;

      2:
        begin
          // Clean update: run the existing uninstaller silently first
          // (/VERYSILENT hides its UI so this reads as one continuous
          // operation, not two separate installer windows), then fall
          // through into the normal install wizard exactly like case 1.
          // [UninstallDelete] on the OLD installation already removes
          // anything [Files] didn't track, so the install directory is
          // clean by the time the wizard starts copying files again.
          if not Exec(UninstallString, '/VERYSILENT', '', SW_HIDE, ewWaitUntilTerminated, ResultCode) then
          begin
            MsgBox(ExpandConstant('{cm:UninstallFailed}'), mbError, MB_OK);
            Result := False;
          end;
        end;

      3:
        begin
          // Uninstall only - same as the old "No" branch: hand off to
          // the existing (visible) uninstaller and exit without ever
          // showing the install wizard.
          if not Exec(UninstallString, '', '', SW_SHOW, ewWaitUntilTerminated, ResultCode) then
          begin
            MsgBox(ExpandConstant('{cm:UninstallFailed}'), mbError, MB_OK);
          end;
          Result := False;
        end;

    else // 0 = cancel
      Result := False;
    end;
  end;

  // Documentation prompt - only asked when the wizard is actually going
  // to run (Result still True here: reinstall, a successful clean
  // update, or no prior installation at all). Answer is read back later
  // by ShouldInstallDocs() as the [Files] Check: for the DOC entry.
  if Result then
  begin
    InstallDocs := (MsgBox(ExpandConstant('{cm:InstallDocsQuestion}'), mbConfirmation, MB_YESNO) = IDYES);
  end;
end;
