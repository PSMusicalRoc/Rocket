local rocket = {}

function rocket.populateFiles(pathtorocket)
    local abs_pathtorocket = path.getabsolute(pathtorocket)
    rocket.Files = {}
    rocket.Files["Rocket"] = {
        path.getabsolute("include/**.h", abs_pathtorocket),
        path.getabsolute("include/**.hpp", abs_pathtorocket),
        path.getabsolute("include/**.cpp", abs_pathtorocket),
        path.getabsolute("src/**.c", abs_pathtorocket),
        path.getabsolute("src/**.cpp", abs_pathtorocket)
    }
    rocket.Files["Roc_ECS"] = {
        path.getabsolute("vendor/Roc_ECS/src/**.cpp", abs_pathtorocket),
        path.getabsolute("vendor/Roc_ECS/include/**.cpp", abs_pathtorocket),
        path.getabsolute("vendor/Roc_ECS/include/**.h", abs_pathtorocket)
    }
    rocket.Files["RocLogger"] = {
        path.getabsolute(
            "vendor/Roc_ECS/vendor/RocLogger/include/RocLogger/RocLogger.cpp",
            abs_pathtorocket
        )
    }
    rocket.Files["all"] = {
        rocket.Files.Rocket,
        rocket.Files.Roc_ECS,
        rocket.Files.RocLogger
    }
end

function rocket.populateIncludes(pathtorocket)
    local abs_pathtorocket = path.getabsolute(pathtorocket)
    rocket.Includes = {}
    rocket.Includes["Rocket"] = path.getabsolute("include", abs_pathtorocket)
    rocket.Includes["Roc_ECS"] = path.getabsolute("vendor/Roc_ECS/include", abs_pathtorocket)
    rocket.Includes["RocLogger"] = path.getabsolute("vendor/Roc_ECS/vendor/RocLogger/include", abs_pathtorocket)
    rocket.Includes["all"] = {
        rocket.Includes.Rocket,
        rocket.Includes.Roc_ECS,
        rocket.Includes.RocLogger
    }
end

function rocket.populateLinks(pathtorocket)
    local abs_pathtorocket = path.getabsolute(pathtorocket)
    rocket.Links = {}
    rocket.Links["RocketGameEngine"] = "glfw"
    rocket.Links["UserProject"] = {
        rocket.Links.RocketGameEngine,
        "RocketGameEngine"
    }
end

function rocket.populateBuildCommands(pathtorocket)
    local abs_pathtorocket = path.getabsolute(pathtorocket)
    rocket.prebuildCommandsDebug = {
        string.format("premake5 --file=\"%s\" gmake2", path.getabsolute("premake5.lua", abs_pathtorocket)),
        string.format("make -C \"%s\" config=debug RocketGameEngine", abs_pathtorocket)
    }
    rocket.prebuildCommandsRelease = {
        string.format("premake5 --file=\"%s\" gmake2", path.getabsolute("premake5.lua", abs_pathtorocket)),
        string.format("make -C \"%s\" config=release RocketGameEngine", abs_pathtorocket)
    }
end

function rocket.populateBuildOutputs(pathtorocket)
    local abs_pathtorocket = path.getabsolute(pathtorocket)
    rocket.outputDebugLinux = path.getabsolute("bin/Debug/libRocketGameEngine.so", abs_pathtorocket)
    rocket.outputReleaseLinux = path.getabsolute("bin/Release/libRocketGameEngine.so", abs_pathtorocket)
    rocket.outputDebugWindows = path.getabsolute("bin/Debug/libRocketGameEngine.so", abs_pathtorocket)
    rocket.outputReleaseWindows = path.getabsolute("bin/Release/libRocketGameEngine.so", abs_pathtorocket)
end

function rocket.intialize(pathtorocket)
    rocket.populateFiles(pathtorocket)
    rocket.populateIncludes(pathtorocket)
    rocket.populateLinks(pathtorocket)
    rocket.populateBuildCommands(pathtorocket)
    rocket.populateBuildOutputs(pathtorocket)
end

function rocket.buildRocketDebug(pathtorocket)
    rocket.populateIncludes(pathtorocket)
    rocket.populateLinks(pathtorocket)
    local abs_pathtorocket = path.getabsolute(pathtorocket)
    os.executef("premake5 --file=\"%s\" gmake2", path.getabsolute("premake5.lua", abs_pathtorocket))
    os.executef("make -C \"%s\" config=debug RocketGameEngine", abs_pathtorocket)
    return path.getabsolute("bin/Debug/libRocketGameEngine.so", abs_pathtorocket)
end

function rocket.buildRocketRelease(pathtorocket)
    rocket.populateIncludes(pathtorocket)
    rocket.populateLinks(pathtorocket)
    local abs_pathtorocket = path.getabsolute(pathtorocket)
    os.executef("premake5 --file=\"%s\" gmake2", path.getabsolute("premake5.lua", abs_pathtorocket))
    os.executef("make -C \"%s\" config=release RocketGameEngine", abs_pathtorocket)
    return path.getabsolute("bin/Release/libRocketGameEngine.so", abs_pathtorocket)
end

return rocket