workspace "rocket"  
    configurations { "Debug", "Release" }

local rocket = require("buildRocket")
rocket.intialize("")

project "RocketGameEngine"
    kind "SharedLib"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

files {
    "include/**.h",
    "include/**.hpp",
    "include/**.cpp",
    "src/**.cpp",
    "src/**.c"
}

dofile("vendor/Roc_ECS/premake5.lua")

links {rocket.Links.RocketGameEngine}

includedirs {rocket.Includes.all}

buildoptions {
    "-Wall", "-Wextra"
}

filter "system:Windows"
    defines {"ROC_WINDOWS"}

filter "system:Linux"
    defines {"ROC_NIX"}

filter "configurations:Debug"
    defines { "ROC_DEBUG" }  
    symbols "On" 

filter "configurations:Release"
    optimize "On"



filter ""

project "docs"
    kind "Makefile"

    buildcommands { "make -f make-docs" }
    cleancommands { "make -f make-docs clean" }
