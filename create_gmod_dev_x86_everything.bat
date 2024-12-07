@echo OFF
MSBuild.exe /?
if ERRORLEVEL 1 (
  echo MsBuild not found in PATH. Please, start from Developer Command Prompt or add MSVC MsBuild directory to the PATH.
  exit /B 1
)

SET "WIN_X64= "
MSBuild.exe /m /p:Platform=x64 /p:Configuration=Release external/vpc/vpc.sln
devtools\bin\vpc.exe /2022 %WIN_X64% /define:WORKSHOP_IMPORT_DISABLE /define:SIXENSE_DISABLE /define:NO_X360_XDK /define:RAD_TELEMETRY_DISABLED /define:DISABLE_ETW /define:NO_STEAM /define:NO_ATI_COMPRESS /define:NO_NVTC /define:LTCG /no_ceg /nofpo /gmod +everything /mksln everything.sln