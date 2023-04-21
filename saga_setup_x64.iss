; To build installer (for example, saga-9.0.2_setup.exe).
; - Unzip the SAGA binary files in their own folder (for example, saga-9.0.2.zip).
; - Place saga.iss and saga_readme.rtf in the SAGA folder (contains saga_gui.exe, etc.).
; - Open saga.iss in Inno Setup.
; - Choose Build | Compile.
; Contributed by Philipp J. Hess

[Setup]
AppName=SAGA - System for Automated Geoscientific Analyses
AppVerName=SAGA 9.0.2

ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

DefaultDirName={commonpf}\SAGA
DefaultGroupName=SAGA
UninstallDisplayIcon={app}\saga_gui.exe

InfoBeforeFile=saga_setup_readme.rtf

Compression=lzma
SolidCompression=yes

OutputDir=.
OutputBaseFilename=saga-9.0.2_x64_setup

VersionInfoVersion=9.0.2.0
VersionInfoCompany=SAGA User Group Association
VersionInfoCopyright=(c) 2005-2018 by O. Conrad
VersionInfoDescription=SAGA single-file installer

PrivilegesRequired=admin

[Files]
; Add .exe's separately so can exclude any other .exe's (e.g., old installer).
Source: "saga_gui.exe"; DestDir: "{app}"
Source: "saga_cmd.exe"; DestDir: "{app}"
Source: "*.*"; Excludes: "*.iss,*.ini,*.cfg,*.bak,*.exe,*.bat,*.rtf"; DestDir: "{app}"
Source: "tools\*.*"; DestDir: "{app}\tools"
Source: "tools\toolchains\*.*"; DestDir: "{app}\tools\toolchains"
Source: "dll\*.*"; DestDir: "{app}\dll"; Flags: recursesubdirs
Source: "ArcSAGA Toolboxes\*.*"; DestDir: "{app}\ArcSAGA Toolboxes"

[Icons]
; Start up in tools folder so SAGA will load all tools automatically.
Name: "{commonprograms}\SAGA"; Filename: "{app}\saga_gui.exe"; WorkingDir: "{app}"
Name: "{commondesktop}\SAGA"; Filename: "{app}\saga_gui.exe"; WorkingDir: "{app}"

[Registry]
Root: HKLM; Subkey: "SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\saga_gui.exe"; ValueType: string; ValueName: ""; ValueData: "{app}\saga_gui.exe"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\saga_gui.exe"; ValueType: string; ValueName: "Path"; ValueData: "{app}"

Root: HKLM; Subkey: "SOFTWARE\SAGA User Group Association\SAGA\9.0.2"; ValueType: string; ValueName: "InstallDir"; ValueData: "{app}"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\SAGA User Group Association\SAGA\9.0.2"; ValueType: string; ValueName: "Version"; ValueData: "9.0.2.0"
