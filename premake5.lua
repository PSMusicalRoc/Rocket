workspace "rocket"  
    configurations { "Debug", "Release" }

project "app"  
    kind "ConsoleApp"   
    language "C++"   

targetdir "bin/%{cfg.buildcfg}" 
files {
    "include/**.h",
    "include/**.hpp",
    "include/**.cpp",
    "src/**.cpp",
    "src/**.c"
}
removefiles {
    "src/tests.cpp"
}
includedirs {"include"}

links {"glfw"}

dofile("vendor/Roc_ECS/premake5.lua")
-- include("vendor/cereal")

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

filter "configurations:Release"     optimize "On"




filter ""

project "tests"
    kind "ConsoleApp"
    language "C++"

    targetdir "bin/%{cfg.buildcfg}"

files {
    "include/**.h",
    "include/**.hpp",
    "include/**.cpp",
    "src/**.cpp",
    "src/**.c"
}
removefiles{
    "src/main.cpp"
}
includedirs {"include"}

links {"glfw"}

dofile("vendor/Roc_ECS/premake5.lua")
dofile("vendor/boost/premake5.lua")
-- include("vendor/cereal")

symbols "On"
defines {"ROC_DEBUG"}

buildoptions {
    "-Wall", "-Wextra"
}

filter "system:Windows"
    defines {"ROC_WINDOWS"}

filter "system:Linux"
    defines {"ROC_NIX"}





filter ""

project "docs"
    kind "Makefile"

    buildcommands { "make -f make-docs" }

project "remove-docs"
    kind "Makefile"

    buildcommands { "rm -rf html/" }
