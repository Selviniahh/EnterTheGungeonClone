cmake_minimum_required(VERSION 3.28)
if (NOT DEFINED ENV{VCPKG_CMAKE})
        message(FATAL_ERROR "VCPKG_CMAKE environment variable is not set.")
endif ()

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_CMAKE}" CACHE STRING "Vcpkg toolchain file")
project(ETG LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

# Specify only source files
add_executable(ETG main.cpp
        src/UI/UserInterface.cpp
        src/Managers/Globals.cpp
        src/Animation/Animation.cpp
        src/Managers/GameManager.cpp
        src/Characters/Hero.cpp
        src/Characters/Components/HeroAnimComp.cpp
        src/Characters/Components/InputComponent.cpp
        src/Characters/Components/HeroMoveComp.cpp
        src/Guns/Base/GunBase.cpp
        src/Guns/Base/GunBase.h
        src/Core/GameObject.h
        src/Projectile/ProjectileBase.h
        src/Projectile/ProjectileBase.cpp
        src/Guns/RogueSpecial/RogueSpecial.cpp
        src/Guns/RogueSpecial/RogueSpecial.h
        src/Managers/GameState.h
        src/Utils/DirectionUtils.cpp
        src/Utils/DirectionUtils.h
        src/Enemy/EnemyBase.cpp
        src/Enemy/EnemyBase.h
        src/Core/Components/BaseHealthComp.cpp
        src/Core/Components/BaseHealthComp.h
        src/Core/Components/BaseMoveComp.cpp
        src/Core/Components/BaseMoveComp.h
        src/Utils/Math.h
        src/Managers/SpriteBatch.cpp
        src/Core/Components/BaseAnimComp.h
        src/Engine/UI/EngineUI.cpp
        src/Engine/UI/EngineUI.h
)

# Easiest and fastest way is VCPKG 
find_package(OpenAL CONFIG REQUIRED)
find_package(SFML CONFIG REQUIRED system window graphics network audio)
find_package(benchmark CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND "${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}" STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif ()

target_compile_options(ETG PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-ftemplate-backtrace-limit=0 -ferror-limit=0>
        $<$<CXX_COMPILER_ID:Clang>:-ftemplate-backtrace-limit=0 -ferror-limit=0>
)

#Because in different builds the final path of the build directory differs, detecting Resources path from Cmake is better option.
target_compile_definitions(ETG PRIVATE RESOURCE_PATH="${CMAKE_SOURCE_DIR}/Resources")

target_link_libraries(ETG PRIVATE
        sfml-audio
        sfml-graphics
        $<$<PLATFORM_ID:Windows>:sfml-main>  # Only link sfml-main on Windows
        sfml-network
        sfml-system
        sfml-window
        benchmark::benchmark
        OpenAL::OpenAL
        ImGui-SFML::ImGui-SFML
)
