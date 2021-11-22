cmake_minimum_required(VERSION 3.21.4)
project(DerydocaEngineCmake)
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

add_subdirectory(vendor/assimp)
set_target_properties(assimp PROPERTIES FOLDER vendor/assimp)
set_target_properties(assimp_cmd PROPERTIES FOLDER vendor/assimp)
set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER vendor/assimp)
set_target_properties(zlibstatic PROPERTIES FOLDER vendor/assimp)
set_target_properties(uninstall PROPERTIES FOLDER vendor/assimp)
set_target_properties(unit PROPERTIES FOLDER vendor/assimp)

add_subdirectory(DerydocaEngine)
add_subdirectory(DerydocaEngine.Components)
add_subdirectory(DerydocaEngine.Components.Editor)
add_subdirectory(DerydocaEngine.Editor)
add_subdirectory(DerydocaEngine.Editor.UI)

#PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME
option(PACKAGE_TESTS "Build the tests" OFF)
if (PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(DerydocaEngine.Test)
    #add_subdirectory(DerydocaEngine.Editor.Test)
endif()
