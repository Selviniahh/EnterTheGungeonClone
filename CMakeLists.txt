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
        src/Managers/Misc.cpp
        src/Characters/Components/AnimationComponent.h
        src/Characters/Components/AnimationComponent.cpp
        src/Characters/Components/InputComponent.cpp
        src/Characters/Components/InputComponent.h
        src/Characters/Components/MoveComponent.cpp
        src/Characters/Components/MoveComponent.h
        src/Managers/SpriteBatch.h
)

# Easiest and fastest way is VCPKG 
find_package(OpenAL CONFIG REQUIRED)
find_package(SFML CONFIG REQUIRED system window graphics network audio)
find_package(benchmark CONFIG REQUIRED)
find_package(Threads REQUIRED)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND "${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}" STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif ()

target_compile_options(ETG PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-ftemplate-backtrace-limit=0 -ferror-limit=0>
        $<$<CXX_COMPILER_ID:Clang>:-ftemplate-backtrace-limit=0 -ferror-limit=0>
)

target_link_libraries(ETG PRIVATE
        sfml-audio
        sfml-graphics
        $<$<PLATFORM_ID:Windows>:sfml-main>  # Only link sfml-main on Windows
        sfml-network
        sfml-system
        sfml-window
        benchmark::benchmark
        OpenAL::OpenAL
)
