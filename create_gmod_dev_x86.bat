@ECHO OFF

CALL create_game_projects.bat gmod x86
IF ERRORLEVEL 1 (
  ECHO Generating Garry's Mod x86 gmod.sln failed.
  EXIT /B 1
)