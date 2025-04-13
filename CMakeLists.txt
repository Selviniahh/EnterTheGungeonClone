cmake_minimum_required(VERSION 3.28)
if (NOT DEFINED ENV{VCPKG_CMAKE})
    message(FATAL_ERROR "VCPKG_CMAKE environment variable is not set.")
endif ()

# Check if CMAKE_TOOLCHAIN_FILE was provided via command line
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    # If not provided via command line, check for environment variable
    if(DEFINED ENV{VCPKG_CMAKE})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_CMAKE}" CACHE STRING "Vcpkg toolchain file")
        message(STATUS "Using toolchain file from VCPKG_CMAKE environment variable: ${CMAKE_TOOLCHAIN_FILE}")
    else()
        # Neither command line nor environment variable was provided
        message(FATAL_ERROR "CMAKE_TOOLCHAIN_FILE was not provided via command line and VCPKG_CMAKE environment variable is not set.")
    endif()
else()
    # Toolchain file was provided via command line
    message(STATUS "Using toolchain file provided via command line: ${CMAKE_TOOLCHAIN_FILE}")
endif()

project(ETG LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

set(CORE_SOURCES
        src/Core/GameObjectBase.cpp
        src/Core/GameObjectBase.h
        src/Core/ComponentBase.h
        src/Core/Factory.h
        src/Core/GameClass.cpp
        src/Core/GameClass.h
        src/Core/TypeID.h
        src/Core/Scene/Scene.cpp
        src/Core/Scene/Scene.h
        src/Core/Components/BaseHealthComp.cpp
        src/Core/Components/BaseHealthComp.h
        src/Core/Components/BaseMoveComp.cpp
        src/Core/Components/BaseMoveComp.h
        src/Core/Components/BaseAnimComp.h
        src/Core/Components/ArrowComp.h
        src/Core/Components/CollisionComponent.cpp
        src/Core/Components/CollisionComponent.h
        src/Core/Components/TimerComponent.cpp
        src/Core/Components/TimerComponent.h
        src/Core/Events/EventDelegate.h
)

set(ENGINE_SOURCES
        src/Engine/Engine.cpp
        src/Engine/Engine.h
        src/Engine/Reflection.h
        src/Engine/UI/EngineUI.cpp
        src/Engine/UI/EngineUI.h
        src/Engine/UI/UIUtils.cpp
        src/Engine/UI/UIUtils.h
)

set(UI_SOURCES
        src/UI/UserInterface.cpp
        src/UI/UIObjects/AmmoBarUI.cpp
        src/UI/UIObjects/AmmoBarUI.h
        src/UI/UIObjects/AmmoIndicatorsUI.cpp
        src/UI/UIObjects/AmmoIndicatorsUI.h
        src/UI/UIObjects/ReloadText.h
        src/UI/UIObjects/AmmoCounter.h
        src/UI/UIObjects/ReloadSlider.cpp
        src/UI/UIObjects/ReloadSlider.h
        src/UI/UIObjects/FrameBar.h
        src/UI/UIObjects/FrameBar.cpp
        src/UI/UIObjects/FrameLeftProgressBar.cpp
        src/UI/UIObjects/FrameLeftProgressBar.h
)

set(MANAGERS_SOURCES
        src/Managers/Globals.cpp
        src/Managers/GameManager.cpp
        src/Managers/GameState.h
        src/Managers/SpriteBatch.cpp
        src/Managers/InputManager.cpp
        src/Managers/TypeRegistry.h
        src/Managers/Enum/StateFlags.h
)

set(ANIMATION_SOURCES
        src/Animation/Animation.cpp
)

set(CHARACTERS_SOURCES
        src/Characters/Hero.cpp
        src/Characters/Components/HeroAnimComp.cpp
        src/Characters/Components/InputComponent.cpp
        src/Characters/Components/HeroMoveComp.cpp
        src/Characters/Hand/Hand.cpp
        src/Characters/Hand/Hand.h
)

set(GUNS_SOURCES
        src/Guns/Base/GunBase.cpp
        src/Guns/Base/GunBase.h
        src/Guns/RogueSpecial/RogueSpecial.cpp
        src/Guns/RogueSpecial/RogueSpecial.h
        src/Guns/VFX/MuzzleFlash.cpp
        src/Guns/VFX/MuzzleFlash.h
        src/Guns/AK-47/AK47.cpp
        src/Guns/AK-47/AK47.h
        src/Guns/SawedOff/SawedOff.cpp
        src/Guns/SawedOff/SawedOff.h
        src/Guns/Magnum/Magnum.cpp
        src/Guns/Magnum/Magnum.h
)

set(PROJECTILE_SOURCES
        src/Projectile/ProjectileBase.cpp
        src/Projectile/ProjectileBase.h
)

set(ENEMY_SOURCES
        src/Enemy/EnemyBase.cpp
        src/Enemy/EnemyBase.h
        src/Enemy/BulletMan/BulletMan.cpp
        src/Enemy/BulletMan/BulletMan.h
        src/Enemy/BulletMan/Components/BulletManAnimComp.cpp
        src/Enemy/BulletMan/Components/BulletManAnimComp.h
        src/Enemy/Components/EnemyMoveCompBase.cpp
        src/Enemy/Components/EnemyMoveCompBase.h
)

set(ITEMS_SOURCES
        src/Items/Active/ActiveItemBase.h
        src/Items/Active/ActiveItemBase.cpp
        src/Items/Active/DoubleShoot.cpp
        src/Items/Active/DoubleShoot.h
        src/Items/Passive/PassiveItemBase.h
        src/Items/Passive/PlatinumBullets.cpp
        src/Items/Passive/PlatinumBullets.h
)

set(MODIFIERS_SOURCES
        src/Modifiers/ModifierManager.h
        src/Modifiers/Gun/MultiShotModifier.h
)

set(UTILS_SOURCES
        src/Utils/DirectionUtils.cpp
        src/Utils/DirectionUtils.h
        src/Utils/Math.h
        src/Utils/TextureUtils.h
        src/Utils/StrManipulateUtil.h
        src/Utils/Interface/IAnimationComponent.h
        src/Utils/Interface/IGunModifier.h
)

find_package(SFML CONFIG REQUIRED system window graphics network audio)
find_package(benchmark CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(boost_type_index CONFIG REQUIRED)
find_package(boost_describe CONFIG REQUIRED)
find_package(boost_mpl REQUIRED CONFIG)

# Create a master STATIC library for all SFML-dependent code
add_library(etgcore STATIC
        ${CORE_SOURCES}
        ${ENGINE_SOURCES}
        ${ANIMATION_SOURCES}
        ${MANAGERS_SOURCES}
        ${CHARACTERS_SOURCES}
        ${GUNS_SOURCES}
        ${UI_SOURCES}
        ${ENEMY_SOURCES}
        ${PROJECTILE_SOURCES}
        ${ITEMS_SOURCES}
)

# Common definitions for the static library
target_compile_definitions(etgcore PRIVATE
        RESOURCE_PATH="../../Resources"
)

# Include directories
target_include_directories(etgcore PUBLIC src)

# Link everything to the static library
target_link_libraries(etgcore PUBLIC
        sfml-system
        sfml-graphics
        sfml-window
        sfml-audio
        sfml-network
        ImGui-SFML::ImGui-SFML
        Boost::type_index
        Boost::describe
        Boost::mpl
)

add_library(utils STATIC ${UTILS_SOURCES})
target_compile_definitions(utils PRIVATE RESOURCE_PATH="${CMAKE_SOURCE_DIR}/Resources")
target_include_directories(utils PUBLIC src)
target_link_libraries(utils PUBLIC Boost::type_index)

add_library(modifiers INTERFACE)
target_sources(modifiers INTERFACE ${MODIFIERS_SOURCES})
target_compile_definitions(modifiers INTERFACE RESOURCE_PATH="${CMAKE_SOURCE_DIR}/Resources")
target_include_directories(modifiers INTERFACE src)

add_executable(ETG main.cpp)

target_compile_definitions(ETG PRIVATE RESOURCE_PATH="${CMAKE_SOURCE_DIR}/Resources")
target_link_libraries(ETG PRIVATE
        etgcore  # Our consolidated static library
        utils
        modifiers
        sfml-audio
        sfml-graphics
        sfml-network
        sfml-system
        sfml-window
        benchmark::benchmark
        ImGui-SFML::ImGui-SFML
        Boost::type_index
        Boost::describe
        Boost::mpl
        $<$<PLATFORM_ID:Windows>:sfml-main>
        Threads::Threads
)