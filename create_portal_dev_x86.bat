@ECHO OFF

call creategameprojects_debug.bat portal x86
if ERRORLEVEL 1 (
  ECHO Generating Portal x86 portal.sln failed.
  EXIT /B 1
)


