# SDL2 Memorama Game - Installation Guide

## Quick Installation (Windows)

### Option 1: Automatic Installation (Recommended)

1. Download the project files
2. Right-click on `install_dependencies.bat` and select "Run as administrator"
3. Wait for the installation to complete
4. Run `C:\Users\[YourUsername]\dev-tools\setup_environment.bat`
5. Build the project:
   ```cmd
   mkdir build
   cd build
   cmake .. -DSDL2_DIR=%SDL2_PATH%
   cmake --build .
   ```

### Option 2: PowerShell Installation

1. Open PowerShell as administrator
2. Run: `PowerShell -ExecutionPolicy Bypass -File install_dependencies.ps1`
3. Follow the same build steps as above

## What Gets Installed

The installation script will download and install:

- **CMake** (build system) - Latest version
- **SDL2 Development Libraries** - For graphics and input
- **SDL2_ttf** - For text rendering
- **Visual C++ Redistributable** - Runtime libraries

Installation location: `C:\Users\[YourUsername]\dev-tools\`

## Manual Installation

If the automatic installation fails, you can install manually:

### 1. Install CMake

- Download from: https://cmake.org/download/
- Add to PATH during installation

### 2. Install SDL2

- Download SDL2 Development Libraries: https://github.com/libsdl-org/SDL/releases
- Download SDL2_ttf: https://github.com/libsdl-org/SDL_ttf/releases
- Extract both to a folder (e.g., `C:\SDL2`)

### 3. Install Visual C++ Redistributable

- Download from: https://aka.ms/vs/17/release/vc_redist.x64.exe
- Run the installer

### 4. Build the Project

```cmd
mkdir build
cd build
cmake .. -DSDL2_DIR=C:\path\to\SDL2
cmake --build .
```

## Running the Game

After building:

1. The executable will be in the `build` directory
2. Make sure all DLL files are in the same directory as the executable
3. The `data` folder should be in the same directory as the executable
4. Run `Memorama.exe`

## Troubleshooting

### "DLL not found" errors

- Ensure all SDL2 DLLs are in the same folder as the executable
- Install Visual C++ Redistributable if not already installed

### Font loading errors

- The game tries to load Arial font from Windows system fonts
- Make sure Windows fonts are accessible

### Build errors

- Ensure CMake can find SDL2: `cmake .. -DSDL2_DIR=path\to\SDL2`
- Check that all development libraries are properly installed

### Permission errors

- Run installation scripts as administrator
- Ensure you have write permissions to the installation directory

## System Requirements

- Windows 10 or later
- Visual C++ Redistributable 2015-2022
- At least 100MB free disk space for dependencies
- Graphics card with basic OpenGL support

## File Structure After Installation

```
C:\Users\[Username]\dev-tools\
├── cmake\
│   └── bin\
│       └── cmake.exe
├── SDL2\
│   ├── include\
│   ├── lib\
│   └── ...
└── setup_environment.bat

Your Project\
├── build\
│   ├── Memorama.exe
│   ├── SDL2.dll
│   ├── SDL2_ttf.dll
│   ├── data\
│   └── ...
├── src\
├── include\
├── data\
└── CMakeLists.txt
```
