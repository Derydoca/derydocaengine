cmake_minimum_required(VERSION 3.21.4)
project(derydoca-engine)
set (CMAKE_CXX_STANDARD 17)

if (WIN32)
	cmake_dependent_option(ENABLE_DX12 "Enable DX12" ON "WIN32" OFF)
endif()
option(ENABLE_VULKAN "Enable Vulkan" ON)

if (WIN32)
	add_compile_definitions(PLATFORM_WINDOWS=1)
elseif (UNIX)
	add_compile_definitions(PLATFORM_LINUX=1)
endif()

add_subdirectory("derydoca-core")
add_subdirectory("derydoca-editor-ui")
