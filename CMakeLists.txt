cmake_minimum_required(VERSION 3.21.4)
project(DerydocaEngineCmake)
set (CMAKE_CXX_STANDARD 17)

if (MSVC)
    add_compile_options(/MP) # Multi-processor compilation
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

include_directories(
    vendor/sdl2/include
    vendor/cereal/include
    vendor/freetype/include
    "include"
)

add_compile_definitions(OPENGL)

add_subdirectory(vendor/freetype)
add_subdirectory(vendor/sdl2)

add_subdirectory(DerydocaEngine)
add_subdirectory(DerydocaEngine.Components)
add_subdirectory(DerydocaEngine.Components.Editor)
add_subdirectory(DerydocaEngine.Editor)
add_subdirectory(DerydocaEngine.Editor.UI)
