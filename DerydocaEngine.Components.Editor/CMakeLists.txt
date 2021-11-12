project (DerydocaEngine.Components.Editor)

include_directories(
    src
    ../DerydocaEngine/src
    ../DerydocaEngine.Components/src
    ../DerydocaEngine.Editor/src
    )

file(GLOB_RECURSE derydocaengine_components_editor_src ./src/*.h ./src/*.cpp)
add_library(${PROJECT_NAME} ${derydocaengine_components_editor_src})

target_precompile_headers(${PROJECT_NAME} PRIVATE "./src/EditorComponentsPch.h")
