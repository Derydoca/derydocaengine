workspace "DerydocaEngine"
    architecture "x64"
    startproject "DerydocaEngine.Editor.UI"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "DerydocaEngine.Components"
    location "DerydocaEngine.Components"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "EngineComponentsPch.h"
    pchsource "%{prj.location}/src/EngineComponentsPch.cpp"
    staticruntime "Off"

    flags
    {
        "MultiProcessorCompile"
    }

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "C:/local/boost_1_68_0",
        "D:/local/boost_1_68_0",
        "%{wks.location}/DerydocaEngine/src",
        "%{prj.location}/src",
        "%{wks.location}/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "OPENGL=1",
            "_CRT_SECURE_NO_WARNINGS",
            "YAML_DECLARE_STATIC",
            "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
        }

    filter "configurations:Debug"
        defines "DD_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "DD_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "DD_DIST"
        symbols "On"

project "DerydocaEngine.Editor"
    location "DerydocaEngine.Editor"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "EditorPch.h"
    pchsource "%{prj.location}/src/EditorPch.cpp"
    staticruntime "Off"

    flags
    {
        "MultiProcessorCompile"
    }

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "C:/local/boost_1_68_0",
        "D:/local/boost_1_68_0",
        "%{wks.location}/DerydocaEngine/src",
        "%{wks.location}/DerydocaEngine.Components/src",
        "%{wks.location}/DerydocaEngine.Components.Editor/src",
        "%{prj.location}/src",
        "%{wks.location}/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "OPENGL=1",
            "_CRT_SECURE_NO_WARNINGS",
            "YAML_DECLARE_STATIC",
            "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
        }

    filter "configurations:Debug"
        defines "DD_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "DD_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "DD_DIST"
        symbols "On"

project "DerydocaEngine.Components.Editor"
    location "DerydocaEngine.Components.Editor"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "EditorComponentsPch.h"
    pchsource "%{prj.location}/src/EditorComponentsPch.cpp"
    staticruntime "Off"

    flags
    {
        "MultiProcessorCompile"
    }

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "C:/local/boost_1_68_0",
        "D:/local/boost_1_68_0",
        "%{wks.location}/DerydocaEngine/src",
        "%{wks.location}/DerydocaEngine.Components/src",
        "%{wks.location}/DerydocaEngine.Editor/src",
        "%{prj.location}/src",
        "%{wks.location}/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "OPENGL=1",
            "_CRT_SECURE_NO_WARNINGS",
            "YAML_DECLARE_STATIC",
            "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
        }

    filter "configurations:Debug"
        defines "DD_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "DD_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "DD_DIST"
        symbols "On"

project "DerydocaEngine"
    location "DerydocaEngine"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "EnginePch.h"
    pchsource "%{prj.location}/src/EnginePch.cpp"
    staticruntime "Off"

    flags
    {
        "MultiProcessorCompile"
    }

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "C:/local/boost_1_68_0",
        "D:/local/boost_1_68_0",
        "%{wks.location}/%{wks.name}.Components/src",
        "%{prj.location}/src",
        "%{wks.location}/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "OPENGL=1",
            "_CRT_SECURE_NO_WARNINGS",
            "YAML_DECLARE_STATIC",
            "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
        }

    filter "configurations:Debug"
        defines "DD_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "DD_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "DD_DIST"
        symbols "On"

project "DerydocaEngine.Editor.UI"
    location "DerydocaEngine.Editor.UI"
    kind "ConsoleApp"
    language "C++"
    debugdir "$(OutputPath)"
    debugargs ("-project ../../../exampleProject")
    pchheader "EditorPch.h"
    pchsource "%{prj.location}/src/EditorPch.cpp"

    linkoptions {
        "/WHOLEARCHIVE:DerydocaEngine.Components.lib",
        "/WHOLEARCHIVE:DerydocaEngine.Components.Editor.lib"
    }
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    links {
        "assimp",
        "glew32",
        "libyaml-cppmdd",
        "SDL2",
        "OpenGL32",
        "freetype"
    }

    libdirs {
        "%{wks.location}/libs/%{cfg.architecture}/%{cfg.shortname}",
        "C:/local/boost_1_68_0/lib64-msvc-14.1",
        "D:/local/boost_1_68_0/lib64-msvc-14.1"
    }

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "C:/local/boost_1_68_0",
        "D:/local/boost_1_68_0",
        "%{wks.location}/include",
        "%{wks.location}/DerydocaEngine/src",
        "%{wks.location}/DerydocaEngine.Components/src",
        "%{wks.location}/DerydocaEngine.Components.Editor/src",
        "%{wks.location}/DerydocaEngine.Editor/src"
    }

    links
    {
        "DerydocaEngine",
        "DerydocaEngine.Components",
        "DerydocaEngine.Components.Editor",
        "DerydocaEngine.Editor"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "OPENGL=1",
            "_CRT_SECURE_NO_WARNINGS",
            "YAML_DECLARE_STATIC",
            "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
        }

        postbuildcommands
        {
            "@echo Copying DLLs",
            "xcopy /y /d \"%{wks.location}libs\\%{cfg.architecture}\\%{cfg.shortname}\\*.dll\" \"%{cfg.buildtarget.directory}\"",
            "@echo Copying engine settings file",
            "xcopy /y /f \"%{wks.location}engineSettings.yaml\" \"%{cfg.buildtarget.directory}\"",
            "@echo Processing the engine resource directory",
            "\"%{cfg.buildtarget.abspath}\" -processDirectory \"%{wks.location}engineResources\"",
            "@echo Copying engine resources",
            "xcopy /y /d /e \"%{wks.location}engineResources\\*\" \"%{cfg.buildtarget.directory}engineResources\\\"",
        }

    filter "configurations:Debug"
        defines "DD_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "DD_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "DD_DIST"
        symbols "On"