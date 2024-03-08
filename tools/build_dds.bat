:: @echo off
CLS

IF NOT EXIST .build MKDIR .build

gcc^
 -std=c17^
 DDS.c targa.c^
 -Wall -Wextra -Wpedantic^
 -o .build/dds.exe