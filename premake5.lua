local rocket = require("buildRocket")
rocket.intialize("")

workspace "rocket"
    configurations { "Debug", "Release" }

project "RocketGameEngine"
    kind "SharedLib"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

files {rocket.Files.all}

links {rocket.Links.RocketGameEngine}

includedirs {rocket.Includes.all}

buildoptions {
    "-Wall", "-Wextra",
    rocket.cflags.RocketGameEngine
}

linkoptions {
    rocket.linkflags.RocketGameEngine
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
