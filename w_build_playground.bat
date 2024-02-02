:: @echo off
CLS
gcc^
 -std=c99^
 src_playground/main.c^
 -Wall -Wextra -Wpedantic^
 -o .build/output_playground.exe^
 && .build\output_playground.exe