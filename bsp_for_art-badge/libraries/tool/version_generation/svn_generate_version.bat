@ECHO OFF
SETLOCAL ENABLEEXTENSIONS

::-----------------------------
::Initialize script arguments
::-----------------------------
SET workDir=./
SET template=svn_version.template
SET target=version.h

::-----------------------------
:: Main entry
::-----------------------------

"SubWCRev.exe" %workDir% %template% %target% >NUL
type %target%

@ECHO ON
