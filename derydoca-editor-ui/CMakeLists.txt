project(derydoca-editor-ui)

add_executable(${PROJECT_NAME} "src/main.cpp" "src/Editor/CommandLineArgs.cpp"  )

find_package(SDL2 CONFIG REQUIRED)
find_package(glfw3 REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE "./include/" "../derydoca-core/include/")

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC derydoca-core
	PRIVATE SDL2::SDL2
	PRIVATE SDL2::SDL2main
	PRIVATE glfw
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:derydoca-core> $<TARGET_FILE_DIR:derydoca-editor-ui>
)
