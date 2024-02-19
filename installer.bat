@echo OFF
cd /D %~dp0
::In case it is running, we stop it to do the modifications
CALL nssm status KDBEX > NUL
if errorlevel 0 (
    CALL nssm stop kdbex > NUL
)
SET FOLDER=%ProgramFiles(x86)%\Kdbex
IF NOT EXIST %FOLDER% MKDIR %FOLDER% > NUL
XCOPY /E /Y . %FOLDER% > NUL
ECHO %FOLDER%\extension | clip
echo %PATH% | find /I "%FOLDER%" > NUL
if errorlevel 1 (
    setx /M PATH "%PATH%";"%FOLDER%" > NUL
)
:: In case the server was already running, we restart it
CALL nssm status kdbex > NUL
if errorlevel 0 (
    CALL nssm start kdbex > NUL
)
ECHO KDBEX Installed, use kdbex to setup the server. Path to extension file is copied into clipboard.
PAUSE
