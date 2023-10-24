filter ""

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