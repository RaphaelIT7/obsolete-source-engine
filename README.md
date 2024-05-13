# Source V1 Engine

[![Build](https://github.com/Source-Authors/Obsoletium/actions/workflows/build.yml/badge.svg)](https://github.com/Source-Authors/Obsoletium/actions/workflows/build.yml)
[![GitHub Repo Size in Bytes](https://img.shields.io/github/repo-size/Source-Authors/Obsoletium.svg)](https://github.com/Source-Authors/Obsoletium)

List of dlls that are worked on:
- `shaderapidx9.dll` (Everything was reverted.)
- `vstdlist.dll` (Everything was reverted.)
- `filesystem_stdio.dll`
Progress:  
- [ ] Update Steamworks
- [x] Implementing basic Gmod functions  
- [x] Implement Language System  
- [x] Implement Gamemode System  
- [ ] Implement GameDepot System
- [ ] Implement Addon System
- - [x] Implement GetSubscriptions  
- - [ ] Implement `cfg/addonnomount.txt` (BUG: Crashes when Saved)  
- - [ ] Implement Addon Mounting  
- - [ ] Implement Addon Download  

List of currently working dlls:
- `datacache.dll`
- `icudt.dll` (IDK what it even does xd)
- `icudt42.dll` (IDK what it even does xd)
- `unicode.dll`
- `vvis_dll.dll` (~40% faster than Gmod's version. Compile it in Release for the Performance benefit)
- `vvis.exe`
- `vrad.exe`
- `vphysics.dll` (Also, fixes ShouldCollide breaking the physics engine)

## Prerequisites

* CMake. Can be [`C++ CMake Tools`](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio#installation) component of Visual Studio.
* [windows] Visual Studio 2022 with v143+ platform toolset.
* Use `git clone --recurse-submodules`
* Run `create_<GAME_NAME>_dev_<ARCH>.bat` from `Developer Command Prompt` (Use x64 version for x64 CPU arch). See directory tree for supported games.
* Open `<GAME_NAME>_<ARCH>.sln`.
* Build.


## How to debug

* Ensure you placed hl2 / episodic / portal game into `game` folder near cloned repo.
* Open `<GAME_NAME>_<ARCH>.sln`.
* Set `launcher_main` project `Command` property to `$(SolutionDir)..\game\hl2.exe`.
* Set `launcher_main` project `Command Arguments` property to `-dxlevel 85 -windowed`.
* Set `launcher_main` project `Working Directory` property to `$(SolutionDir)..\game\`.
* Use `Set as Startup Project` menu for `launcher_main` project.
* Start debugging.


## How to release

* Ensure you placed hl2 / episodic / portal game into `game` folder near cloned repo.
* Open `<GAME_NAME>_<ARCH>.sln`.
* Choose `Release` Configuration.
* Build.
* `game` folder contains the ready game.


If you found a bug, please file it at https://github.com/Source-Authors/obsolete-source-engine/issues.


## SAST Tools

[PVS-Studio](https://pvs-studio.com/en/pvs-studio/?utm_source=website&utm_medium=github&utm_campaign=open_source) - static analyzer for C, C++, C#, and Java code.