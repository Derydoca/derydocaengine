cmake_minimum_required(VERSION 3.21.4)
project(DerydocaEngine)
set (CMAKE_CXX_STANDARD 17)

if (MSVC)
    add_compile_options(/MP) # Multi-processor compilation
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

include_directories(
    vendor/sdl2/include
    vendor/cereal/include
    vendor/freetype/include
    "include"
)

add_compile_definitions(OPENGL)

add_subdirectory(vendor/freetype)
set_target_properties(freetype PROPERTIES FOLDER vendor/freetype)

add_subdirectory(vendor/sdl2)
set_target_properties(SDL2 PROPERTIES FOLDER vendor/sdl2)
set_target_properties(SDL2main PROPERTIES FOLDER vendor/sdl2)
set_target_properties(SDL2-static PROPERTIES FOLDER vendor/sdl2)

set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "ASSIMP_BUILD_ASSIMP_TOOLS")
set(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE BOOL "ASSIMP_BUILD_ASSIMP_VIEW")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "ASSIMP_BUILD_TESTS")
set(ASSIMP_INSTALL OFF CACHE BOOL "ASSIMP_INSTALL")
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "ASSIMP_INSTALL_PDB")
set(ASSIMP_NO_EXPORT ON CACHE BOOL "ASSIMP_NO_EXPORT")
add_subdirectory(vendor/assimp COMPONENTS ${LIBASSIMP_COMPONENT})
set_target_properties(assimp PROPERTIES FOLDER vendor/assimp)
set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER vendor/assimp)
set_target_properties(zlibstatic PROPERTIES FOLDER vendor/assimp)
set_target_properties(uninstall PROPERTIES FOLDER vendor/assimp)

add_subdirectory(DerydocaEngine)
add_subdirectory(DerydocaEngine.Components)
add_subdirectory(DerydocaEngine.Components.Editor)
add_subdirectory(DerydocaEngine.Editor)
add_subdirectory(DerydocaEngine.Editor.UI)
