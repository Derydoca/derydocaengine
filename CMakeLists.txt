cmake_minimum_required(VERSION 3.21.4)
project(derydoca-engine)
set (CMAKE_CXX_STANDARD 17)

add_subdirectory("vendor/nvrhi")
add_subdirectory("DerydocaEngine")
add_subdirectory("DerydocaEngine.Editor.UI")
