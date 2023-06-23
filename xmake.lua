set_xmakever("2.7.9")

-- project
set_project("Reflyem")
set_version("1.0.0")
set_license("MIT")
set_languages("cxxlatest")
set_optimize("faster")
-- set_warnings("allextra", "error")
set_toolchains("msvc")
-- add_cxxflags("msvc::-Wimplicit-fallthrough=5")

-- allowed
set_allowedarchs("windows|x64")
set_allowedmodes("debug", "releasedbg")

-- default
set_defaultarchs("windows|x64")
set_defaultmode("releasedbg")

-- rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")
-- add_rules("c++")

-- policies
set_policy("package.requires_lock", true)
-- set_policy("build.c++.modules", true)

-- packages
-- add_requires("fmt", "spdlog", "toml++")
add_requires("toml++")
add_requires("commonlibsse-ng", { configs = { skyrim_vr = false, skyrim_ae = false }})

-- targets
target("Reflyem")

    add_packages("fmt", "spdlog", "toml++", "commonlibsse-ng")

    add_rules("@commonlibsse-ng/plugin", {
        name = "Reflyem",
        author = "Newrite",
        description = "Reflyem additions"
    })

    set_targetdir("G:/SteamLibrary/steamapps/common/Skyrim Special Edition/MO2/mods/ReflyemDLL/SKSE/Plugins")

    add_files("/src/**.cpp")
    -- add_files("src/**.ixx", "src/plugin/**.ixx")
    add_headerfiles("/include/**.hpp")
    add_headerfiles("/include/pch.h")
    set_pcxxheader("/include/pch.h")
    add_includedirs("/include")

    -- copy build files to MODS or SKYRIM paths
    after_build(function(target)
        local copy = function(env, ext)
            for _, env in pairs(env:split(";")) do
                if os.exists(env) then
                    local plugins = path.join(env, ext, "SKSE/Plugins")
                    os.mkdir(plugins)
                    os.trycp(target:targetfile(), plugins)
                    os.trycp(target:symbolfile(), plugins)
                end
            end
        end
        if os.getenv("SKYRIM_MODS_PATH") then
            copy(os.getenv("SKYRIM_MODS_PATH"), target:name())
        elseif os.getenv("SKYRIM_PATH") then
            copy(os.getenv("SKYRIM_PATH"), "Data")
        end
    end)
