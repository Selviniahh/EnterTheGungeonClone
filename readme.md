# Enter the Gungeon Clone

A clone of the game Enter the Gungeon built with SFML. It aims to replicate the original game, featuring modern C++ design, clean modular code, and a reflection system that lets you easily modify any initialized game object's variables. 

[![Watch the video](https://img.youtube.com/vi/lgvuDcSot1w/0.jpg)](https://youtu.be/lgvuDcSot1w)

## Download the game 
1.  Download at  [release tab](https://github.com/Selviniahh/CapstoneGameProject/releases/download/release/Game.zip). Unzip and open executable file.
2. Also it's possible to download at [itch.io](https://selviniah.itch.io/enter-the-gungeon-clone). Unzip and open the executable file.

# How to build
## Prerequisites
- CMake
- Ninja Build System
- A C++ compiler
- vcpkg (for dependency management)

Please follow each step carefully to ensure a successful build.

```
git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
git clone https://github.com/Selviniahh/CapstoneGameProject.git
cd CapstoneGameProject
mkdir build
set VCPKG_CMAKE=..\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake -G Ninja -B build
cmake --build build --config Release
.\build\bin\Release\ETG.exe
```

### Other Platforms
The build process is similar on Linux and macOS. Adjust the environment variables and toolchain settings as necessary.

# Dependencies

- SFML
- ImGui
- ImGui-SFML
- boost-type-index
- boost-describe
- boost-mpl

# Contributing
Please read the docs folder to understand the project structure.