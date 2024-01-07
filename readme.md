# Project goal

Learning 3D game development a little bit lower than start to use a game engine.

Main bullet points of the experiment:

- 3D game with input handling, animation and audio at least two platforms
- Use the least number of techs
- Go for simplicity, embrace new solutions only when a problem arose

# Specs

- **OS:** Windows 11 Pro
- **Editor:** Notepad++
- **Console:** Windows PowerShell

# Set-up

## SDL2

- [Releases · libsdl-org/SDL (github.com)](https://github.com/libsdl-org/SDL/releases) (SDL2-devel-x.y.z-mingw.zip)
- unzip "x86_64-w64-mingw32" folder
- copy SDL2.dll into projects bin folder from "x86_64-w64-mingw32\bin"
- set "sdl_path" in w_build.bat

## bgfx

- [Releases · skeeto/w64devkit (github.com)](https://github.com/skeeto/w64devkit/releases)
- unzip
- set path to "path-to-w64devkit\bin" (to be able to use `make` in the console easily)

```bash
git clone https://github.com/bkaradzic/bx.git
git clone https://github.com/bkaradzic/bimg.git
git clone https://github.com/bkaradzic/bgfx.git

cd bgfx

..\bx\tools\bin\windows\genie.exe --gcc=mingw-gcc gmake
make mingw-gcc-debug64
```

- set "bgfx_path" in w_build.bat

# Usage

## Build

Simply call `.\w_build.bat` in the root folder of the project.

> I don't use any of make/CMake/MakeFile.txt stuff, because I want to reach that point, when I feel I need something like that, then choose a solution for my needs.

#### w_build.bat

For setting debug mode on or off, change the `SET debug_mode=` line.
