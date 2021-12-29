cmake_minimum_required(VERSION 3.21.4)
project(derydoca-engine)
set (CMAKE_CXX_STANDARD 17)

cmake_dependent_option(ENABLE_DX12 "Enable DX12" ON "WIN32" OFF)
option(ENABLE_VULKAN "Enable Vulkan" ON)

add_subdirectory("vendor")
add_subdirectory("DerydocaEngine")
add_subdirectory("DerydocaEngine.Editor.UI")
