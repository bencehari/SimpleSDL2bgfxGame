@echo off
SETLOCAL enabledelayedexpansion

:: 0 disables debug mode
:: any other value sets flags and runs the app with GDB (you need to start the app
:: with `run` in gdb context)
SET debug_mode=0
:: if you don't need extra warnings, just add REM or :: at the start of the line
SET extra=-Wextra -Werror -Wpedantic

:: choosing compiler
:: for example (in this case, add `-lstdc++` doesn't needed):
:: compiler=g++ and std_opt=c++17
SET compiler=g++
SET std_opt=c++17
:: SET compiler=gcc
:: SET std_opt=c17

SET i=0
FOR /F "tokens=* delims=" %%x in (paths.txt) DO (
	SET /a i+=1
	SET "paths[!i!]=%%x"
)

:: change the paths to your SDL2 and bgfx
SET sdl_path=!paths[1]!
SET bgfx_path=!paths[2]!

:: bgfx thing (for now). If you used `make mingw-gcc-debug64`, use Debug
:: if `make mingw-gcc-release64` was used, use Release
:: or rename the lib files and remove this line
SET mode=Debug

IF %debug_mode%==0 (
	SET run_cmd=.build\output.exe
) ELSE (
	SET dbg_flags=-g -ggdb
	SET run_cmd=gdb .build\output.exe
)

:: add *.c files from src folder
SET source_files=
FOR /f "usebackq delims=" %%a IN (`DIR /s /b ".\src\*.cc" ".\src\*.c"`) DO SET source_files=!source_files! %%a

CLS

IF NOT EXIST .build MKDIR .build
IF NOT EXIST ".build\SDL2.dll" COPY "bin\SDL2.dll" ".build\SDL2.dll"

@echo on

:: -lgdi32: https://learn.microsoft.com/en-us/windows/win32/gdi/windows-gdi
:: doesn't seem right for me because of 32bit stuff, but bgfx requires it.
:: TODO: find a 64bit solution
%compiler%^
 -std=%std_opt%^
 %source_files%^
 -I%sdl_path%include^
 -I%bgfx_path%bgfx\include^
 -L%sdl_path%lib^
 -L%bgfx_path%bgfx\.build\win64_mingw-gcc\bin^
 %dbg_flags%^
 -Wall %extra%^
 -lmingw32^
 -lSDL2main^
 -lSDL2^
 -lbgfx%mode%^
 -lbimg%mode%^
 -lbx%mode%^
 -lgdi32^
 -o .build/output.exe^
 && %run_cmd%
 
 :: -lstdc++^
 
 :: -I%bgfx_path%bimg\include^
 :: -I%bgfx_path%bx\include^
 
 :: -lbimg_decode%mode%^
 