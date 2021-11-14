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

target_link_libraries(${PROJECT_NAME}
    SDL2
    freetype
    DerydocaEngine.Core
    DerydocaEngine.Components
    DerydocaEngine.Components.Editor
    DerydocaEngine.Editor
)

#[[
    debugdir "$(OutputPath)"
    debugargs ("-project ../../../exampleProject")
    linkoptions {
        "/WHOLEARCHIVE:DerydocaEngine.Components.lib",
        "/WHOLEARCHIVE:DerydocaEngine.Components.Editor.lib"
    }
    links {
        "assimp",
        "glew32",
        "SDL2",
        "OpenGL32",
        "freetype"
    }
    links
    {
        "DerydocaEngine",
        "DerydocaEngine.Components",
        "DerydocaEngine.Components.Editor",
        "DerydocaEngine.Editor"
    }
    libdirs {
        "%{wks.location}/libs/%{cfg.architecture}/%{cfg.shortname}"
    }
        postbuildcommands
        {
            "@echo Copying DLLs",
            "xcopy /y /d \"%{wks.location}libs\\%{cfg.architecture}\\%{cfg.shortname}\\*.dll\" \"%{cfg.buildtarget.directory}\"",
            "@echo Copying engine settings file",
            "xcopy /y /f \"%{wks.location}engineSettings.json\" \"%{cfg.buildtarget.directory}\"",
            "@echo Copying IMGUI default layout file",
            "xcopy /y /f \"%{wks.location}imgui.ini\" \"%{cfg.buildtarget.directory}\"",
            "@echo Processing the engine resource directory",
            "\"%{prj.name}.exe\" -processDirectory \"%{wks.location}engineResources\"",
            "@echo Copying engine resources",
            "xcopy /y /d /e \"%{wks.location}engineResources\\*\" \"%{cfg.buildtarget.directory}engineResources\\\"",
        }
]]