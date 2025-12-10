@echo off
echo === SDL2 Memorama Game - Auto Build and Run ===
echo.

REM Clean build directory
if exist "build" (
    echo Cleaning previous build...
    rmdir /s /q "build"
)

REM Create build directory
mkdir build
cd build

REM Configure and build
echo Configuring project...
cmake .. -G "MinGW Makefiles"

if %errorLevel% neq 0 (
    echo.
    echo Configuration failed!
    echo Check the error messages above.
    cd ..
    pause
    exit /b 1
)

echo.
echo Building project...
cmake --build . --config Release

if %errorLevel% neq 0 (
    echo.
    echo Build failed!
    echo Check the error messages above.
    cd ..
    pause
    exit /b 1
)

REM Run the game
echo.
echo === Starting Memorama Game ===
echo.
if exist "Memorama.exe" (
    Memorama.exe
) else (
    echo Executable not found!
    dir *.exe
)

cd ..
echo.
echo Game session ended.