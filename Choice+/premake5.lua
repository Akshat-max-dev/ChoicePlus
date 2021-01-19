project "Choice+"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src"
    }

    links
    {
        "Core",
        "Renderer"
    }

    filter "system:windows"
            systemversion "latest"

    filter "configurations:Debug"
            defines "DEBUG"
            runtime "Debug"
            symbols "on"
    
    filter "configurations:Release"
            defines "RELEASE"
            runtime "Release"
            optimize "on"

    filter "configurations:Distribution"
            defines "DISTRIBUTION"
            runtime "Release"
            optimize "on"