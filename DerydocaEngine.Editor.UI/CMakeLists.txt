project(derydoca-engine-editor-ui)

find_package(SDL2 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} "src/main.cpp" "src/Editor/CommandLineArgs.cpp" "src/Editor/EditorWindow.h" "src/Editor/EditorWindow.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE "./src/" "../DerydocaEngine/src/")
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(${PROJECT_NAME} PUBLIC derydoca-engine-core)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:derydoca-engine-core> $<TARGET_FILE_DIR:derydoca-engine-editor-ui>
)
