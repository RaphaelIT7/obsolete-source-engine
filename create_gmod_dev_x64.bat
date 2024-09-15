@ECHO OFF

CALL create_game_projects.bat gmod x64
IF ERRORLEVEL 1 (
  ECHO Generating Garry's Mod x64 gmod.sln failed.
  EXIT /B 1
)
