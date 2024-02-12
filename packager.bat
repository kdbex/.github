@ECHO OFF
SET BUILD_DIR=.\build
SET EXTENSION_TARGET=%BUILD_DIR%\extension
SET SERVER_TARGET=%BUILD_DIR%\server
IF EXIST %BUILD_DIR% rmdir /S /Q %BUILD_DIR%
MKDIR %BUILD_DIR%
MKDIR %EXTENSION_TARGET%
MKDIR %SERVER_TARGET%
COPY *.exe %BUILD_DIR% > NUL
CD ..\extension
CALL npm run build > NUL
ECHO Extension built
for /R %%G in (*.zip) do (
    SET BEX_ZIP=%%G
    goto :found
)
:found
CD ..\server
CALL npm run build > NUL
ECHO Server built
CD ..\.github
CALL tar -xf %BEX_ZIP% -C %EXTENSION_TARGET%
COPY ..\server\dist\server %SERVER_TARGET% > NUL
CALL gcc kdbex.c -o %BUILD_DIR%\kdbex.exe
CALL tar -cf .\build.tar -C %BUILD_DIR% .
ECHO Build complete