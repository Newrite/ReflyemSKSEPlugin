// ReSharper disable CppInconsistentNaming
#pragma once

#define DEBUG
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <Windows.h>
#include <Nameof.hpp>

#pragma warning(push)
#ifdef DEBUG
#	include <spdlog/sinks/basic_file_sink.h>
#else
#	include <spdlog/sinks/msvc_sink.h>
#endif
#pragma warning(pop)

namespace logger = SKSE::log;
namespace stl = SKSE::stl;
using namespace std::literals;

namespace ranges = std::ranges;
namespace views = ranges::views;

#define RELOCATION_OFFSET(SE, AE) SE
#define let const auto
#define letr const auto&
#define letp const auto*
#define let_expr constexpr auto

#define logi logger::info
#define loge logger::error
#define logd logger::debug
#define logw logger::warning

#define DLLEXPORT __declspec(dllexport)

#include "Version.h"
#ifdef USELESS_UTILS
#	include "UselessFenixUtils.h"
#endif
