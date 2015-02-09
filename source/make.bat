echo off
color 7
setlocal EnableDelayedExpansion

rem Create build directories
mkdir bin\
mkdir bin\libgdcc\
mkdir ..\acs\
mkdir inc\
mkdir src\

rem Set variables
set /a BUILT_FILES=0
set /a SKIPPED_FILES=0
set /a BUILD_ZAN=0
set OUTFILE=zkvn
set GDCC=GDCC
set PATH=%PATH%;%GDCC%
set CFLAGS=--bc-target=ZDoom
set CC=gdcc-cc
set AS=gdcc-as
set LD=gdcc-ld

rem Clear screen and begin build
cls
echo --- Begin Build ---

if ""%1 == ""-zandronum set /a BUILD_ZAN=1
if ""%2 == ""-zandronum set /a BUILD_ZAN=1

if ""%1 == ""-clean goto make.clean
if ""%2 == ""-clean goto make.clean

goto make.all

rem Build routines
:make.clean
del bin\*.ir bin\libgdcc\*.ir ..\acs\*.bin
goto make.all

:make.all
:make.libGDCC-ZDACS-asm
set CFLAGS=--bc-target=ZDoom
set TO_BUILD=bin\libgdcc\ZDACS-asm.ir
set FILES=%GDCC%\lib\src\libGDCC\ZDACS\*.asm
set EXECUTABLE=%AS%
set BUILD_RETURN=make.libGDCC-c
goto build

:make.libGDCC-c
set CFLAGS=--bc-target=ZDoom
set TO_BUILD=bin\libgdcc\c.ir
set FILES=%GDCC%\lib\src\libGDCC\*.c
set EXECUTABLE=%CC%
set BUILD_RETURN=make.libGDCC
goto build

:make.libGDCC
set CFLAGS=--bc-target=ZDoom
set TO_BUILD=bin\libgdcc\libGDCC.ir
set FILES=bin\libgdcc\*.ir
set EXECUTABLE=%LD%
set BUILD_RETURN=make.libc
set CFLAGS1=-c
goto build

:make.libc
set CFLAGS=--bc-target=ZDoom
set TO_BUILD=bin\libgdcc\libc.ir
set FILES=%GDCC%\lib\src\libc\*.c
set EXECUTABLE=%CC%
set BUILD_RETURN=make.ir
goto build

:make.ir
if %BUILD_ZAN% == 1 (
	set CFLAGS=--bc-target=ZDoom -DZAN_ACS
) else (
	set CFLAGS=--bc-target=ZDoom
)
set TO_BUILD=bin\%OUTFILE%.ir
set FILES=src\*.c
set EXECUTABLE=%CC%
set BUILD_RETURN=make.bin
set ALWAYS_REBUILD=YES
set CFLAGS1=-iinc
goto build

:make.bin
set CFLAGS=--bc-target=ZDoom
set TO_BUILD=..\acs\%OUTFILE%.bin
set FILES=bin\*.ir bin\libgdcc\*.ir
set EXECUTABLE=%LD%
set BUILD_RETURN=build.done
set ALWAYS_REBUILD=YES
goto build

:build
if ""!ALWAYS_REBUILD! == "" (
	if not exist !TO_BUILD! (
		echo --- Building !TO_BUILD! ---
		set/a BUILT_FILES=%BUILT_FILES% + 1
		!EXECUTABLE! !CFLAGS1! !CFLAGS! -o !TO_BUILD! !FILES!
		echo !EXECUTABLE! !CFLAGS1! !CFLAGS! -o !TO_BUILD! !FILES!
	) else (
		echo --- Already built !TO_BUILD!, skipping. ---
		set/a SKIPPED_FILES=%SKIPPED_FILES% + 1
	)
) else (
	echo --- Building !TO_BUILD! ---
	set/a BUILT_FILES=%BUILT_FILES% + 1
	!EXECUTABLE! !CFLAGS1! !CFLAGS! -o !TO_BUILD! !FILES!
	echo !EXECUTABLE! !CFLAGS1! !CFLAGS! -o !TO_BUILD! !FILES!
)
set CFLAGS1=
goto !BUILD_RETURN!

:build.done
echo --- Finished. %BUILT_FILES% built, and %SKIPPED_FILES% skipped. ---
echo --- Press any key to exit. ---
endlocal
pause >nul