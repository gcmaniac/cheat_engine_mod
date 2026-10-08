@echo off
setlocal enabledelayedexpansion

echo ======================================================================
echo    CHEAT ENGINE POINTER ADAPTER - BUILD SCRIPT
echo ======================================================================

REM Check GCC compiler in PATH or fallback to MinGW
where gcc >nul 2>nul
if %errorlevel% equ 0 (
    set BUILDER_CC=gcc
) else if exist "D:\c_projects\compilers\mingw64\bin\gcc.exe" (
    set BUILDER_CC="D:\c_projects\compilers\mingw64\bin\gcc.exe"
) else if exist "D:\msys64\mingw32\bin\gcc.exe" (
    set BUILDER_CC="D:\msys64\mingw32\bin\gcc.exe"
) else (
    echo [ERROR] Tidak ditemukan compiler GCC! Pastikan MinGW terpasang.
    pause
    exit /b 1
)

REM Compile resources (icon) for 32-bit and 64-bit
echo Mengompilasi icon resource (windres) ...
cmd /c "set PATH=D:\msys64\mingw32\bin;%%PATH%% && windres -F pe-i386 src/resource.rc -o src/resource_32.o"
cmd /c "set PATH=D:\c_projects\compilers\mingw64\bin;%%PATH%% && windres -F pe-x86-64 src/resource.rc -o src/resource_64.o"

echo Mengompilasi program compiler builder.exe ...
%BUILDER_CC% -O2 builder/builder_main.c builder/compiler_manager.c -o builder.exe
if %errorlevel% neq 0 (
    echo [ERROR] Gagal mengompilasi builder.exe!
    pause
    exit /b %errorlevel%
)

echo builder.exe siap dijalankan.
echo.
builder.exe %*

echo.
echo Selesai!
