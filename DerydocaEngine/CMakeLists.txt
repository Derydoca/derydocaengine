project (DerydocaEngine.Core)

include_directories(
    src
    ../DerydocaEngine.Components/src
    )

#[[ Other defines that were in the premake file
_CRT_SECURE_NO_WARNINGS
_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
]]
add_compile_definitions(
    IMGUI_IMPL_OPENGL_LOADER_GLEW
    )

file(GLOB_RECURSE derydocaengine_src ./src/*.h ./src/*.cpp)
add_library(${PROJECT_NAME} STATIC ${derydocaengine_src})

target_precompile_headers(${PROJECT_NAME} PRIVATE "./src/EnginePch.h")
