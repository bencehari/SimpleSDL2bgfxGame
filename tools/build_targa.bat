:: @echo off
CLS

IF NOT EXIST .build MKDIR .build

::SET debug=-g -ggdb
::SET run=gdb

gcc^
 -std=c17^
 -m64^
 %debug%^
 targa.c^
 -Wall -Wextra -Wpedantic^
 -o .build/targa_test.exe^
 && %run% .build\targa_test.exe