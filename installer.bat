@echo OFF
cd /D %~dp0
SET FOLDER=%ProgramFiles(x86)%\Kdbex
IF NOT EXIST %FOLDER% MKDIR %FOLDER%
XCOPY /E /Y . %FOLDER% > NUL
ECHO %FOLDER%\extension | clip
echo %PATH% | find /I "%FOLDER%" > nul
if errorlevel 1 (
    setx /M PATH "%PATH%";"%FOLDER%" > NUL
)
ECHO KDBEX Installed, use kdbex to setup the server. Path to extension file is copied into clipboard.
PAUSE
