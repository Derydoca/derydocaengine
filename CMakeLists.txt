cmake_minimum_required(VERSION 3.21.4)
project(DerydocaEngineCmake)
set (CMAKE_CXX_STANDARD 17)

if (MSVC)
    add_compile_options(/MP) # Multi-processor compilation
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

include_directories(
    vendor/cereal/include
    vendor/freetype/include
    "include"
)

add_compile_definitions(OPENGL)

add_subdirectory(DerydocaEngine)
add_subdirectory(DerydocaEngine.Components)
