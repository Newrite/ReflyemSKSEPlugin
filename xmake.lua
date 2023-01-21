set_xmakever("2.7.2")

-- project
set_project("Reflyem")
set_version("1.0.0")
set_license("MIT")
set_languages("cxxlatest")
set_optimize("faster")
set_warnings("allextra", "error")

-- allowed
set_allowedarchs("windows|x64")
set_allowedmodes("debug", "releasedbg")

-- default
set_defaultarchs("windows|x64")
set_defaultmode("releasedbg")

-- rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- policies
set_policy("package.requires_lock", true)

-- packages
add_requires("fmt", "spdlog", "toml++")
add_requires("commonlibsse-ng", { configs = { skyrim_vr = false, skyrim_ae = false }})

-- includes
includes("res/package.lua")

-- targets
target("Reflyem")

    add_packages("fmt", "spdlog", "toml++", "commonlibsse-ng")

    add_rules("@commonlibsse-ng/plugin", {
        name = "Reflyem",
        author = "Newrite",
        description = "Reflyem additions"
    })

    set_targetdir("G:\\MO2Enderal\\mods\\Reflyem\\SKSE\\Plugins")

    add_files("src/plugin/**.cpp")
    add_files("src/**.cpp")
    -- add_files("src/plugin/**.ixx")
    add_headerfiles("include/plugin/**.h")
    add_headerfiles("include/**.h")
    add_includedirs("src/plugin")
    add_includedirs("src")
    add_includedirs("include/plugin")
    add_includedirs("include")
    set_pcxxheader("include/pch.h")

    add_rules("mod.package", {
        ["@{target}-@{target_ver}.zip"] = {
            { "@{target_dir}", "@{target}.dll", "Data/SKSE/Plugins/" },
            { "@{target_dir}", "@{target}.pdb", "Data/SKSE/Plugins/" },
        }
    })
