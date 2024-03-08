# Project goal

Learning 3D game development a little bit lower than start to use a game engine.

Main bullet points of the experiment:

- 3D game with input handling, animation and audio at least two platforms
- Use the least number of techs
- Go for simplicity, embrace new solutions only when a problem arose

> UPDATE: looks like I love to know how things work, so when I got to texturing, I had to dive into DDS (it's WiP), and to validate some steps, a simple TGA lib also has been written.

# Specs

- **OS:** Windows 11 Pro
- **Editor:** Notepad++
- **Console:** Windows PowerShell/Command Prompt

# Set-up

Create a paths.txt in the root of the project (it is .gitignore'd to avoid overriding different paths on different computers with VC).

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

## Editors

### Visual Studio Code

Open File/Preferences/Settings (CTRL+,), select Workspace and search for 'include', then add these items to C_Cpp > Default: Include Path:

- path/to/SDL2/x86_64-mingw32/include/**
- path/to/bgfx/include/**
- path/to/bimg/inlcude/**
- path/to/bx/include/**

Or add them to the .vscode/settings.json file like this:

```json
{
    "C_Cpp.default.includePath": [
        "path/to/SDL2/x86_64-w64-mingw32/include/**",
        "path/to/bgfx/include/**",
        "path/to/bimg/include/**",
        "path/to/bx/include/**"
    ]
}
```



# Usage

## Build

Simply call `.\w_build.bat` in the root folder of the project.

> I don't use any of make/CMake/MakeFile.txt stuff, because I want to reach that point, when I feel I need something like that, then choose a solution for my needs.

#### w_build.bat

For setting debug mode on or off, change the `SET debug_mode=` line.
