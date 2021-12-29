project(derydoca-engine-editor-ui)

add_executable(${PROJECT_NAME} "src/main.cpp" "src/Editor/CommandLineArgs.cpp"  )

target_include_directories(${PROJECT_NAME} PRIVATE "./src/" "./include/" "../DerydocaEngine/include/")

target_link_libraries(${PROJECT_NAME} PUBLIC derydoca-engine-core)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:derydoca-engine-core> $<TARGET_FILE_DIR:derydoca-engine-editor-ui>
)
