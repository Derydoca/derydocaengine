project (DerydocaEngine.Editor.UI)

include_directories(
    src
    ../DerydocaEngine/src
    ../DerydocaEngine.Components/src
    ../DerydocaEngine.Components.Editor/src
    ../DerydocaEngine.Editor/src
    )

file(GLOB_RECURSE derydocaengine_editor_ui_src ./src/*.h ./src/*.cpp)
add_executable(${PROJECT_NAME} ${derydocaengine_editor_ui_src})

target_precompile_headers(${PROJECT_NAME} PRIVATE "./src/EditorPch.h")

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    assimp::assimp
    ../../libs/x86_64/Debug/glew32
    SDL2
    freetype
    DerydocaEngine.Core
    DerydocaEngine.Editor
    DerydocaEngine.Components
    DerydocaEngine.Components.Editor
)

set(MY_VAR "$<TARGET_FILE_NAME:DerydocaEngine.Editor.UI>")

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:DerydocaEngine.Componentsd.lib /WHOLEARCHIVE:DerydocaEngine.Components.Editord.lib")

# HACK: Remove the glew32.dll copy line once the library is replaced
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:DerydocaEngine.Editor.UI> $<TARGET_FILE_DIR:DerydocaEngine.Editor.UI>
    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/../libs/x86_64/Debug/glew32.dll" $<TARGET_FILE_DIR:DerydocaEngine.Editor.UI>
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/../engineResources" "$<TARGET_FILE_DIR:DerydocaEngine.Editor.UI>/engineResources"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "$<TARGET_FILE_DIR:DerydocaEngine.Editor.UI>/engineResources/configs" $<TARGET_FILE_DIR:DerydocaEngine.Editor.UI>
    COMMAND_EXPAND_LISTS
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_COMMAND_ARGUMENTS "-project ${PROJECT_SOURCE_DIR}/../exampleProject"
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:DerydocaEngine.Editor.UI>
    VS_STARTUP_PROJECT ${PROJECT_NAME}
)
