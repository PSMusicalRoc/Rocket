workspace "rocket"  
    configurations { "Debug", "Release" }

IncludeDirs = {}
IncludeDirs["Rocket"] = "include"
IncludeDirs["Roc_ECS"] = "vendor/Roc_ECS/include"
IncludeDirs["RocLogger"] = "vendor/Roc_ECS/vendor/RocLogger/include"
IncludeDirs["all"] = {
    "%{IncludeDirs.Rocket}",
    "%{IncludeDirs.Roc_ECS}",
    "%{IncludeDirs.RocLogger}"
}

Links = {}
Links["Rocket"] = {"glfw"}
Links["UserProject"] = {Links.Rocket, "RocketGameEngine"}

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

links {Links.Rocket}

includedirs {IncludeDirs.all}

dofile("vendor/Roc_ECS/premake5.lua")

dofile("./includeMe.lua")

filter ""


project "TestOfEngine"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

files {
    -- insert your files here
}

includedirs {IncludeDirs.all}

links {Links.UserProject}

dofile("includeMe.lua")



filter ""

project "docs"
    kind "Makefile"

    buildcommands { "make -f make-docs" }
    cleancommands { "make -f make-docs clean" }
