:: @echo off
CLS

IF NOT EXIST .build MKDIR .build

gcc^
 -std=c17^
 targa.c^
 -Wall -Wextra -Wpedantic^
 -o .build/targa_test.exe^
 && .build\targa_test.exe