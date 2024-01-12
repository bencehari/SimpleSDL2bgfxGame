# Project goal

Learning 3D game development a little bit lower than start to use a game engine.

Main bullet points of the experiment:

- 3D game with input handling, animation and audio at least two platforms
- Use the least number of techs
- Go for simplicity, embrace new solutions only when a problem arose

# Specs

- **OS:** Windows 11 Pro
- **Editor:** Notepad++
- **Console:** Windows PowerShell/Command Prompt

# Set-up

Create a paths.txt in the root of the project.

## SDL2

- download SDL2: [Releases · libsdl-org/SDL (github.com)](https://github.com/libsdl-org/SDL/releases) (SDL2-devel-x.y.z-mingw.zip)
- unzip "x86_64-w64-mingw32" folder
- copy SDL2.dll into projects bin folder from "x86_64-w64-mingw32\bin"
- add the path "to\the\SDL2\x86_64-w64-mingw32\" to paths.txt (must be first line and don't forget the \ at the end)

## bgfx

- download MinGW-W64: [Releases · skeeto/w64devkit (github.com)](https://github.com/skeeto/w64devkit/releases)
- unzip
- set path to "path-to-w64devkit\bin" (to be able to use `make` in the console easily)
- [Building — bgfx documentation (bkaradzic.github.io)](https://bkaradzic.github.io/bgfx/build.html)

```bash
git clone https://github.com/bkaradzic/bx.git
git clone https://github.com/bkaradzic/bimg.git
git clone https://github.com/bkaradzic/bgfx.git

cd bx
make mingw-gcc-debug64

cd ..
cd bimg
make mingw-gcc-debug64

cd ..
cd bgfx
make mingw-gcc-debug64
```

- add the path "to\the\bgfx\" (where the bgfx, bimg and bx folders are) to paths.txt (must be second line and don't forget the \ at the end)

# Usage

## Build

Simply call `.\w_build.bat` in the root folder of the project.

> I don't use any of make/CMake/MakeFile.txt stuff, because I want to reach that point, when I feel I need something like that, then choose a solution for my needs.

#### w_build.bat

For setting debug mode on or off, change the `SET debug_mode=` line.
