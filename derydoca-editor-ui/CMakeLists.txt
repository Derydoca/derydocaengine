project(derydoca-editor-ui)

add_executable(${PROJECT_NAME} "src/main.cpp" "src/Editor/CommandLineArgs.cpp"  )

target_include_directories(${PROJECT_NAME} PRIVATE "./include/" "../derydoca-core/include/")

target_link_libraries(${PROJECT_NAME} PUBLIC derydoca-core)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:derydoca-core> $<TARGET_FILE_DIR:derydoca-editor-ui>
)
