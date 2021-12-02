project (DerydocaEngine.Components)

include_directories(
    src
    ../DerydocaEngine/src
    )

file(GLOB_RECURSE derydocaengine_components_src ./src/*.h ./src/*.cpp)
add_library(${PROJECT_NAME} STATIC ${derydocaengine_components_src})

target_precompile_headers(${PROJECT_NAME} PRIVATE "./src/EngineComponentsPch.h")
