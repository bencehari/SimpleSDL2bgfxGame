:: https://drmemory.org/
@echo off

IF NOT EXIST .build\logs MKDIR .build\logs

SET drmemory_path=C:\dev\C\Tools\DrMemory-Windows-2.6.0\bin64\
SET block_list=""

@echo on

%drmemory_path%drmemory.exe -logdir .build\logs -lib_blocklist %block_list% .build\output.exe
