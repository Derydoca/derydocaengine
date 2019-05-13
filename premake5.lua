workspace "DerydocaEngine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- SDL checks - Yes (/sdl): This option is missing from Premake but existed in the manually created project
project "DerydocaEngine.Components"
    location "DerydocaEngine.Components"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "EngineComponentsPch.h"
    pchsource "%{prj.location}/src/EngineComponentsPch.cpp"
    staticruntime "off"

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
        staticruntime "On"
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

-- Conformance mode - Yes (/permissive-)
-- SDL checks - Yes (/sdl): This option is missing from Premake but existed in the manually created project
project "DerydocaEngine.Editor"
    location "DerydocaEngine.Editor"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "EditorPch.h"
    pchsource "%{prj.location}/src/EditorPch.cpp"
    staticruntime "off"

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
        "%{wks.location}/DerydocaEngine.Components/src", -- Where is this????
        "%{wks.location}/DerydocaEngine.Components.Editor/src",
        "%{prj.location}/src",
        "%{wks.location}/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
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
    staticruntime "off"

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
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "OPENGL=1",
            "_CRT_SECURE_NO_WARNINGS",
            "YAML_DECLARE_STATIC",
            "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
        }

        -- postbuildcommands
        -- {
        --     ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/DerydocaEngine.Editor.UI")
        -- }

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
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "include"
    }

    links
    {
        "DerydocaEngine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "OPENGL"
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