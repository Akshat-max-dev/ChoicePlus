project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs
    {
        "include"
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