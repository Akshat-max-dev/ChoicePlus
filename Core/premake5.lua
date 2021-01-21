project "Core"
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
		"src/**.cpp",

		"%{wks.location}/vendor/glm/**.hpp",
		"%{wks.location}/vendor/glm/**.inl"
	}

	includedirs
	{
		"%{wks.location}/Core",
		"%{wks.location}/Choice+/src",
		"%{wks.location}/Renderer/src",
		"src",
		"%{Dependency.GLFW}",
		"%{Dependency.Glad}",
		"%{Dependency.ImGui}",
		"%{Dependency.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"Renderer"
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"_CRT_SECURE_NO_WARNINGS"
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