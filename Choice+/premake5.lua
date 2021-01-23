project "Choice+"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    entrypoint "mainCRTStartup"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "%{wks.location}/Core",
        "%{wks.location}/Renderer/src",
        "%{Dependency.GLFW}",
        "%{Dependency.Glad}",
        "%{Dependency.ImGui}",
        "%{Dependency.glm}"
    }

    defines
    {
        "GLFW_INCLUDE_NONE"
    }

    links
    {
        "Core",
        "ImGui",
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