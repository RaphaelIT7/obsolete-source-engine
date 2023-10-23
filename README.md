The goal of this branch is to be able to create our own dlls that are compatible with gmod.

List of currently working dlls:
- datacache
- icudt (IDK what it even does xd)
- icudt42 (IDK what it even does xd)
- vstdlist (onyl with `-noasync`)

List of broken dlls:
- launcher
- engine
- vgui2
- ServerBrowser
- vguimatsurface

# Source engine

![CI](https://github.com/Source-Authors/obsolete-source-engine/actions/workflows/build.yml/badge.svg?branch=master)

## How to build

* [windows] Visual Studio 2022 with v143+ platform toolset.
* Use `git clone --recurse-submodules`
* Add registry keys:
  | Path                        | Name  | Type | Data |
  | ----                        | ----  | ---- | ---- |
  | `HKLM\SOFTWARE\WOW6432Node\Microsoft\VisualStudio\10.0\Projects\{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}` | `DefaultProjectExtension` | string | vcproj
* Run `creategameprojects_debug.bat` from Developer command prompt.
* Open `hl2.sln`
* Build