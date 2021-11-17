project (DerydocaEngine.Editor)

include_directories(
    src
    ../DerydocaEngine/src
    ../DerydocaEngine.Components/src
    ../DerydocaEngine.Components.Editor/src
    )

file(GLOB_RECURSE derydocaengine_editor_src ./src/*.h ./src/*.cpp)
add_library(${PROJECT_NAME} STATIC ${derydocaengine_editor_src})

target_precompile_headers(${PROJECT_NAME} PRIVATE "./src/EditorPch.h")
