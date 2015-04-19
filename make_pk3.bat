echo off
title PK3 Builder
cls

echo === Compiling Source Code ===
cd .\source
set CC=.\GDCC\gdcc-cc.exe
set AS=.\GDCC\gdcc-as.exe
set LD=.\GDCC\gdcc-ld.exe
set PROJECT_NAME=zkvn
set ZANDRONUM_COMPILE=0
if exist .\bin del .\bin /s /q >nul 2>&1
if not exist .\bin mkdir .\bin
if not exist .\bin\libgdcc mkdir .\bin\libgdcc
if not exist ..\pk3\acs mkdir ..\pk3\acs
if not exist .\inc mkdir .\inc
if not exist .\src mkdir .\src
echo ZDACS-asm.ir
%AS% --bc-target=ZDoom -o .\bin\libgdcc\ZDACS-asm.ir .\GDCC\lib\src\libGDCC\ZDACS\*.asm
echo c.ir
%CC% --bc-target=ZDoom -o .\bin\libgdcc\c.ir .\GDCC\lib\src\libGDCC\*.c
echo libGDCC.ir
%LD% -c --bc-target=ZDoom -o .\bin\libgdcc\libGDCC.ir .\bin\libgdcc\*.ir
echo libc.ir
%CC% --bc-target=ZDoom -o .\bin\libgdcc\libc.ir .\GDCC\lib\src\libc\*.c
echo %PROJECT_NAME%.ir
if %ZANDRONUM_COMPILE%==1 %CC% -iinc --bc-target=ZDoom -DZAN_ACS -o .\bin\%PROJECT_NAME%.ir .\src\*.c
if not %ZANDRONUM_COMPILE%==1 %CC% -iinc --bc-target=ZDoom -o .\bin\%PROJECT_NAME%.ir .\src\*.c
echo %PROJECT_NAME%.bin
%LD% --bc-target=ZDoom -o ..\pk3\acs\%PROJECT_NAME%.bin .\bin\*.ir .\bin\libgdcc\*.ir
echo Finished.
echo.

echo === Making PK3 ===
cd..
echo Creating %PROJECT_NAME%.pk3...
.\tools\7za.exe a -tzip %PROJECT_NAME%.pk3 .\pk3\* >nul
echo Finished.
echo.

echo === Finished ===
echo You should now see a "%PROJECT_NAME%.pk3" file in %cd%
echo Press any key to exit . . .
pause >nul