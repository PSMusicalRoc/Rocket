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

project "tests"
    kind "ConsoleApp"
    language "C++"

    targetdir "bin/%{cfg.buildcfg}"

files {
    "include/**.h",
    "include/**.hpp",
    "include/**.cpp",
    "src/**.cpp",
    "src/**.c",

}
removefiles{
    "src/main.cpp"
}
includedirs {"include"}

links {"glfw"}

symbols "On"
defines {"ROC_DEBUG"}

filter "system:Windows"
    defines {"ROC_WINDOWS"}

filter "system:Linux"
    defines {"ROC_NIX"}