workspace "Choice+"
    architecture "x64"
    startproject "Choice+"

    configurations 
    {
        "Debug",
        "Release",
        "Distribution"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

Dependency = {}
Dependency["GLFW"] = "%{wks.location}/vendor/GLFW/include"
Dependency["Glad"] = "%{wks.location}/vendor/Glad/include"
Dependency["ImGui"] = "%{wks.location}/vendor/ImGui"

group "Dependencies"
    include "vendor/GLFW"
    include "vendor/Glad"
    include "vendor/ImGui"
group ""

include "Choice+"
include "Core"
include "Renderer"