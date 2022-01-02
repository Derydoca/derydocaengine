cmake_minimum_required(VERSION 3.21.4)
project(derydoca-engine)
set (CMAKE_CXX_STANDARD 17)

cmake_dependent_option(ENABLE_DX12 "Enable DX12" ON "WIN32" OFF)
option(ENABLE_VULKAN "Enable Vulkan" ON)

if (PLATFORM_WINDOWS)
	add_compile_definitions(PLATFORM_WINDOWS=1)
endif()

if (PLATFORM_LINUX)
	add_compile_definitions(PLATFORM_LINUX=1)
endif()

add_subdirectory("vendor")
add_subdirectory("DerydocaEngine")
add_subdirectory("DerydocaEngine.Editor.UI")
