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
includedirs {"include"}

links {"glfw"}

include("vendor/Roc_ECS")
include("vendor/cereal")

filter "system:Windows"
    defines {"ROC_WINDOWS"}

filter "system:Linux"
    defines {"ROC_NIX"}

filter "configurations:Debug"
    defines { "ROC_DEBUG" }  
    symbols "On" 

filter "configurations:Release" 
    optimize "On" 
