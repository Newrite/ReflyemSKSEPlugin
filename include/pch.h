// ReSharper disable CppInconsistentNaming
#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

namespace WinAPI = SKSE::WinAPI;
namespace logger = SKSE::log;
namespace fs     = std::filesystem;
using namespace std::literals;

#define RELOCATION_OFFSET(SE, AE) REL::VariantOffset(SE, AE, 0).offset()

namespace stl {
using namespace SKSE::stl;
}