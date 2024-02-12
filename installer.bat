@echo OFF
SET FOLDER=%ProgramFiles(x86)%\Kdbex
IF NOT EXIST %FOLDER% MKDIR %FOLDER%
COPY * %FOLDER%
ECHO %FOLDER%\extension | clip
setx /M PATH %PATH%;%FOLDER%
ECHO KDBEX Installed, use kdbex to setup the server. Path to extension file is copied into clipboard.