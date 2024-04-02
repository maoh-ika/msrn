@echo off

rmdir /s /q bin\Debug
rmdir /s /q dist\Debug

mkdir bin\Debug
mkdir dist\Debug


SET GBDK_HOME=.\gbdk-win64\gbdk

SET LCC_COMPILE_BASE=%GBDK_HOME%\bin\lcc -Isrc/headers -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG
SET LCC_COMPILE=%LCC_COMPILE_BASE% -debug -c -o

SETLOCAL ENABLEDELAYEDEXPANSION

SET "COMPILE_OBJECT_FILES="

:: loop for all files in the default source folder
FOR /R "src/" %%X IN (*.c) DO (
    echo Compiling %%~nX ...
    %LCC_COMPILE% bin/Debug/%%~nX.o %%X
    SET "COMPILE_OBJECT_FILES=bin/Debug/%%~nX.o !COMPILE_OBJECT_FILES!"
)

:: Compile a .gb file from the compiled .o files
%LCC_COMPILE_BASE% -Wm-yC -Wl-yt3 -debug -o dist/Debug/msrn.gb !COMPILE_OBJECT_FILES!

endlocal

"tools/external/romusage/romusage" dist\Debug\msrn.noi -a