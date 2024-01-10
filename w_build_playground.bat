:: @echo off
CLS
gcc^
 -std=c99^
 src_playground/main.c^
 src/math/matrix4.c^
 src/core/object.c^
 -Wall -Wextra^
 -o .build/output_playground.exe^
 && .build\output_playground.exe