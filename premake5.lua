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

include "Choice+"
include "Core"
include "Renderer"