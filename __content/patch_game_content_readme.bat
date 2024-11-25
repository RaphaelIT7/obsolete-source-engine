:: This creates symbolic links to all __content directories
:: Assumes your current directory is obsolete-source-engine/__content/, 
:: and your game folder is in ../../game

:: Requires you to run as administrator due to 'mklink' requiring it, 
:: but it's nothing fancy

@echo off

net session >NUL 2>&1
if %errorLevel% == 0 (
    echo   [SUCCESS] We are running with adequate permissions!
	echo   [  INFO ] Creating symbolic links...

	call :Link lua
	call :Link resource
	call :Link scripts

	echo   [SUCCESS] Done.
) else (
    echo   [FAILURE] ERROR: This script must be ran as administrator, due to 'mklink' requiring administrator rights.
	echo   [FAILURE] Right click on the .bat file and click 'Run as administrator'.
)

goto End

:Link
	echo   [  INFO ]     Linking %1...

	@mklink /d "../../game/garrysmod/%1" %cd%\%1 > NUL
	goto :eof
:End
pause