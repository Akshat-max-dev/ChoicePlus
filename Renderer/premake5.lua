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
        "src",
        "%{wks.location}/Core",
        "%{wks.location}/Choice+/src",
        "%{Dependency.Glad}",
        "%{Dependency.glm}",
        "%{Dependency.ImGui}",
        "%{Dependency.assimp}",
        "%{Dependency.tinyddsloader}"
    }

    libdirs
    {
        "%{wks.location}/vendor/assimp/bin"
    }

    links
    {
        "Glad",
        "ImGui",
        "assimp.lib"
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