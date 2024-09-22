The goal of this branch is to be able to create our own dlls that are compatible with gmod.  
There is also the [gmod-improvements](https://github.com/RaphaelIT7/obsolete-source-engine/tree/gmod-improvements) branch, which will contain bug fixes or improvements.

[![Build](https://github.com/Source-Authors/Obsoletium/actions/workflows/build.yml/badge.svg)](https://github.com/Source-Authors/Obsoletium/actions/workflows/build.yml)
[![GitHub Repo Size in Bytes](https://img.shields.io/github/repo-size/Source-Authors/Obsoletium.svg)](https://github.com/Source-Authors/Obsoletium)

# NOTE
I just rebased this entire branch to have the 64x stuff :D  
So stuff might be broken.  

List of dlls that are worked on:
- `vstdlist.dll` (Everything was reverted.)
- `studiorender.dll` (Only works with `studio_queue_mode`, BUG: pCallQueue->QueueCall causes a crash! `QUEUE_STUDIORENDER_CALL`)
- `filesystem_stdio.dll` (Something broke and idk what)
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

List of currently (probably)working dlls:
- `datacache.dll`
- `icudt.dll` (IDK what it even does xd)
- `icudt42.dll` (IDK what it even does xd)
- `shaderapidx9.dll` (Needs more testing but seems to work.)
- `unicode.dll`
- `vvis_dll.dll` (~40% faster than Gmod's version. Compile it in Release for the Performance benefit)
- `vvis.exe`
- `vrad.exe`

List of currently verified working dlls:
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

## NOTES
Hammer could work but it wasn't tested.


# Source engine

If you found a bug, please file it at https://github.com/Source-Authors/obsolete-source-engine/issues.


## SAST Tools

[PVS-Studio](https://pvs-studio.com/en/pvs-studio/?utm_source=website&utm_medium=github&utm_campaign=open_source) - static analyzer for C, C++, C#, and Java code.