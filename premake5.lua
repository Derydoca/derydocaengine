workspace "DerydocaEngine"
    architecture "x64"
    startproject "DerydocaEngine.Editor.UI"
    rtti("On")

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
vendorincludes = {
    "%{wks.location}/vendor/cereal/include",
    "%{wks.location}/vendor/freetype/include"
}

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

    includedirs { vendorincludes }
    includedirs
    {
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

    includedirs { vendorincludes }
    includedirs
    {
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

    includedirs { vendorincludes }
    includedirs
    {
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

    includedirs { vendorincludes }
    includedirs
    {
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
            "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING",
            "IMGUI_IMPL_OPENGL_LOADER_GLEW"
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
    kind "WindowedApp"
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
        "%{wks.location}/libs/%{cfg.architecture}/%{cfg.shortname}"
    }

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs { vendorincludes }
    includedirs
    {
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
            "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
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

    filter "configurations:Debug"
        defines {
            "DD_DEBUG",
            "_WINDOWS"
        }
        symbols "On"
        staticruntime "Off"
        runtime "Debug"

    filter "configurations:Release"
        defines {
            "DD_RELEASE",
            "_WINDOWS"
        }
        symbols "On"
        runtime "Release"

    filter "configurations:Dist"
        defines {
            "DD_DIST",
            "_WINDOWS"
        }
        symbols "On"
        runtime "Release"

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
        "%{wks.location}/libs/%{cfg.architecture}/%{cfg.shortname}"
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

    includedirs { vendorincludes }
    includedirs
    {
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
            "xcopy /y /f \"%{wks.location}engineSettings.json\" \"%{cfg.buildtarget.directory}\""
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
        "%{wks.location}/libs/%{cfg.architecture}/%{cfg.shortname}"
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

    includedirs { vendorincludes }
    includedirs
    {
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
            "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
        }
        
        postbuildcommands
        {
            "@echo Copying DLLs",
            "xcopy /y /d \"%{wks.location}libs\\%{cfg.architecture}\\%{cfg.shortname}\\*.dll\" \"%{cfg.buildtarget.directory}\"",
            "@echo Copying engine settings file",
            "xcopy /y /f \"%{wks.location}engineSettings.json\" \"%{cfg.buildtarget.directory}\""
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
        "vendor/gtest/googletest/src/gtest-all.cc"
    }

    includedirs
    {
        "%{wks.location}/vendor/gtest/googletest",
        "%{wks.location}/vendor/gtest/googletest/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "On"
        staticruntime "Off"
        runtime "Debug"

    filter "configurations:Release"
        symbols "On"
        runtime "Release"

    filter "configurations:Dist"
        symbols "On"
        runtime "Release"
    
project "freetype"
    location "vendor/generatedProjects"
    kind "StaticLib"
    language "C++"
    warnings "Off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    flags
    {
        "MultiProcessorCompile"
    }

    files
    {
        "vendor/freetype/src/autofit/autofit.c",
        "vendor/freetype/src/base/ftbase.c",
        "vendor/freetype/src/base/ftbbox.c",
        "vendor/freetype/src/base/ftbdf.c",
        "vendor/freetype/src/base/ftbitmap.c",
        "vendor/freetype/src/base/ftcid.c",
        "vendor/freetype/src/base/ftdebug.c",
        "vendor/freetype/src/base/ftfstype.c",
        "vendor/freetype/src/base/ftgasp.c",
        "vendor/freetype/src/base/ftglyph.c",
        "vendor/freetype/src/base/ftgxval.c",
        "vendor/freetype/src/base/ftinit.c",
        "vendor/freetype/src/base/ftmm.c",
        "vendor/freetype/src/base/ftotval.c",
        "vendor/freetype/src/base/ftpatent.c",
        "vendor/freetype/src/base/ftpfr.c",
        "vendor/freetype/src/base/ftstroke.c",
        "vendor/freetype/src/base/ftsynth.c",
        "vendor/freetype/src/base/ftsystem.c",
        "vendor/freetype/src/base/fttype1.c",
        "vendor/freetype/src/base/ftwinfnt.c",
        "vendor/freetype/src/bdf/bdf.c",
        "vendor/freetype/src/bzip2/ftbzip2.c",
        "vendor/freetype/src/cache/ftcache.c",
        "vendor/freetype/src/cff/cff.c",
        "vendor/freetype/src/cid/type1cid.c",
        "vendor/freetype/src/gzip/ftgzip.c",
        "vendor/freetype/src/lzw/ftlzw.c",
        "vendor/freetype/src/pcf/pcf.c",
        "vendor/freetype/src/pfr/pfr.c",
        "vendor/freetype/src/psaux/psaux.c",
        "vendor/freetype/src/pshinter/pshinter.c",
        "vendor/freetype/src/psnames/psnames.c",
        "vendor/freetype/src/raster/raster.c",
        "vendor/freetype/src/sfnt/sfnt.c",
        "vendor/freetype/src/smooth/smooth.c",
        "vendor/freetype/src/truetype/truetype.c",
        "vendor/freetype/src/type1/type1.c",
        "vendor/freetype/src/type42/type42.c",
        "vendor/freetype/src/winfonts/winfnt.c"
    }

    includedirs
    {
        "%{wks.location}/vendor/freetype/src",
        "%{wks.location}/vendor/freetype/include",
        "%{wks.location}/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "WIN32",
            "_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS",
            "_CRT_NONSTDC_NO_WARNINGS",
            "FT2_BUILD_LIBRARY"
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
    