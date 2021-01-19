project "Renderer"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "**.h",
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src"
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