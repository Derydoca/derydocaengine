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
        systemversion "latest"

        defines
        {
            "OPENGL=1",
            "_CRT_SECURE_NO_WARNINGS",
            "YAML_DECLARE_STATIC",
            "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
        }

    filter "configurations:Debug"
        staticruntime "Off"
        defines "DD_DEBUG"
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines "DD_RELEASE"
        symbols "On"
        runtime "Release"

    filter "configurations:Dist"
        defines "DD_DIST"
        symbols "On"
        runtime "Release"

project "DerydocaEngine.Editor"
    location "DerydocaEngine.Editor"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "EditorPch.h"
    pchsource "%{prj.location}/src/EditorPch.cpp"

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
        staticruntime "Off"
        runtime "Debug"

    filter "configurations:Release"
        defines "DD_RELEASE"
        symbols "On"
        runtime "Release"

    filter "configurations:Dist"
        defines "DD_DIST"
        symbols "On"
        runtime "Release"

project "DerydocaEngine.Components.Editor"
    location "DerydocaEngine.Components.Editor"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "EditorComponentsPch.h"
    pchsource "%{prj.location}/src/EditorComponentsPch.cpp"

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
        staticruntime "Off"
        runtime "Debug"

    filter "configurations:Release"
        defines "DD_RELEASE"
        symbols "On"
        runtime "Release"

    filter "configurations:Dist"
        defines "DD_DIST"
        symbols "On"
        runtime "Release"

project "DerydocaEngine"
    location "DerydocaEngine"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "EnginePch.h"
    pchsource "%{prj.location}/src/EnginePch.cpp"

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
        staticruntime "Off"
        runtime "Debug"

    filter "configurations:Release"
        defines "DD_RELEASE"
        symbols "On"
        runtime "Release"

    filter "configurations:Dist"
        defines "DD_DIST"
        symbols "On"
        runtime "Release"

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
        staticruntime "Off"
        runtime "Debug"

        links {
            "libyaml-cppmdd"
        }

    filter "configurations:Release"
        defines "DD_RELEASE"
        symbols "On"
        runtime "Release"

        links {
            "libyaml-cppmd"
        }

    filter "configurations:Dist"
        defines "DD_DIST"
        symbols "On"
        runtime "Release"

        links {
            "libyaml-cppmd"
        }

project "DerydocaEngine.Test"
    location "DerydocaEngine.Test"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    links {
        "assimp",
        "glew32",
        "SDL2",
        "OpenGL32",
        "freetype"
    }

    libdirs {
        "%{wks.location}/libs/%{cfg.architecture}/%{cfg.shortname}",
        "C:/local/boost_1_68_0/lib64-msvc-14.1",
        "D:/local/boost_1_68_0/lib64-msvc-14.1"
    }

    pchheader "EngineTestPch.h"
    pchsource "%{prj.location}/src/EngineTestPch.cpp"

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
        "%{prj.location}/src",
        "%{wks.location}/include",
        "%{wks.location}/vendor/gtest/googletest/include"
    }

    links
    {
        "DerydocaEngine",
        "DerydocaEngine.Components",
        "GoogleTest"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING",
            "_HAS_TR1_NAMESPACE"
        }
        
        postbuildcommands
        {
            "@echo Copying DLLs",
            "xcopy /y /d \"%{wks.location}libs\\%{cfg.architecture}\\%{cfg.shortname}\\*.dll\" \"%{cfg.buildtarget.directory}\"",
            "@echo Copying engine settings file",
            "xcopy /y /f \"%{wks.location}engineSettings.yaml\" \"%{cfg.buildtarget.directory}\""
        }

    filter "configurations:Debug"
        defines "DD_DEBUG"
        symbols "On"
        staticruntime "Off"
        runtime "Debug"

        links {
            "libyaml-cppmdd"
        }

    filter "configurations:Release"
        defines "DD_RELEASE"
        symbols "On"
        runtime "Release"

        links {
            "libyaml-cppmd"
        }

    filter "configurations:Dist"
        defines "DD_DIST"
        symbols "On"
        runtime "Release"

        links {
            "libyaml-cppmd"
        }

project "DerydocaEngine.Editor.Test"
    location "DerydocaEngine.Editor.Test"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

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

    libdirs {
        "%{wks.location}/libs/%{cfg.architecture}/%{cfg.shortname}",
        "C:/local/boost_1_68_0/lib64-msvc-14.1",
        "D:/local/boost_1_68_0/lib64-msvc-14.1"
    }

    pchheader "EditorTestPch.h"
    pchsource "%{prj.location}/src/EditorTestPch.cpp"

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
        "%{wks.location}/include",
        "%{wks.location}/vendor/gtest/googletest/include"
    }

    links
    {
        "DerydocaEngine",
        "DerydocaEngine.Components",
        "DerydocaEngine.Components.Editor",
        "DerydocaEngine.Editor",
        "GoogleTest"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "OPENGL=1",
            "_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING",
            "_HAS_TR1_NAMESPACE",
            "_CRT_SECURE_NO_WARNINGS",
            "YAML_DECLARE_STATIC",
            "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
        }
        
        postbuildcommands
        {
            "@echo Copying DLLs",
            "xcopy /y /d \"%{wks.location}libs\\%{cfg.architecture}\\%{cfg.shortname}\\*.dll\" \"%{cfg.buildtarget.directory}\"",
            "@echo Copying engine settings file",
            "xcopy /y /f \"%{wks.location}engineSettings.yaml\" \"%{cfg.buildtarget.directory}\""
        }

    filter "configurations:Debug"
        defines "DD_DEBUG"
        symbols "On"
        staticruntime "Off"
        runtime "Debug"

        links {
            "libyaml-cppmdd"
        }

    filter "configurations:Release"
        defines "DD_RELEASE"
        symbols "On"
        runtime "Release"

        links {
            "libyaml-cppmd"
        }

    filter "configurations:Dist"
        defines "DD_DIST"
        symbols "On"
        runtime "Release"

        links {
            "libyaml-cppmd"
        }
    
project "GoogleTest"
    location "vendor/generatedProjects"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    flags
    {
        "MultiProcessorCompile"
    }

    files
    {
        "vendor/gtest/googletest/src/**.h",
        "vendor/gtest/googletest/src/**.cc"
    }

    includedirs
    {
        "%{wks.location}/vendor/gtest/googletest",
        "%{wks.location}/vendor/gtest/googletest/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "OPENGL=1"
        }

    filter "configurations:Debug"
        defines "DD_DEBUG"
        symbols "On"
        staticruntime "Off"
        runtime "Debug"

    filter "configurations:Release"
        defines "DD_RELEASE"
        symbols "On"
        runtime "Release"

    filter "configurations:Dist"
        defines "DD_DIST"
        symbols "On"
        runtime "Release"
    