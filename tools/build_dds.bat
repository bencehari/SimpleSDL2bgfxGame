:: @echo off
CLS

IF NOT EXIST .build MKDIR .build

gcc^
 -std=c17^
 DDS_console.c DDS.c targa.c^
 -Wall -Wextra -Wpedantic^
 -o .build/dds.exe