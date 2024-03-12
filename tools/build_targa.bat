:: @echo off
CLS

IF NOT EXIST .build MKDIR .build

::SET debug=-g -ggdb
::SET run=gdb

gcc^
 -std=c17^
 -m64^
 %debug%^
 targa_console.c targa.c^
 -Wall -Wextra -Wpedantic^
 -o .build/targa.exe^
 && %run% .build\targa.exe