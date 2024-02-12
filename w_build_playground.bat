:: @echo off
CLS

SET MATH_INCLUDES=src/math/math.c src/math/HandmadeMath.c

gcc^
 -std=c17^
 src_playground/main.c^
 %MATH_INCLUDES%^
 -Wall -Wextra -Wpedantic^
 -o .build/output_playground.exe^
 && .build\output_playground.exe