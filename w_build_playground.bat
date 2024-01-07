:: @echo off
CLS
gcc^
 -std=c99^
 src_playground/main.c^
 src/bx_math.c^
 -Wall -Wextra^
 -o .build/output_playground.exe^
 && .build\output_playground.exe