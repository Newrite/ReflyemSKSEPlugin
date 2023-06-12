// ReSharper disable CppInconsistentNaming
#pragma once

#undef UNICODE

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <Windows.h>

namespace WinAPI = SKSE::WinAPI;
namespace logger = SKSE::log;
namespace stl = SKSE::stl;
using namespace std::literals;

namespace ranges = std::ranges;
namespace views = ranges::views;

#define RELOCATION_OFFSET(SE, AE) REL::VariantOffset(SE, AE, 0).offset()
#define let const auto
#define letr const auto&
#define letp const auto*
#define let_expr constexpr auto