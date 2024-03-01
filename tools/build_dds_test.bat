:: @echo off
CLS

IF NOT EXIST .build MKDIR .build

gcc^
 -std=c++17^
 dds_test.cc targa.c^
 -Wall -Wextra -Wpedantic^
 -o .build/dds_test.exe