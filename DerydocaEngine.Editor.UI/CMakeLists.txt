project (derydoca-engine-editor-ui)

find_package(SDL2 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} "src/main.cpp" "src/Editor/CommandLineArgs.cpp" "src/Editor/EditorWindow.h" "src/Editor/EditorWindow.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE "./src/")
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)
