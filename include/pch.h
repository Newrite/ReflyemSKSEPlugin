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

auto actor_dispel_effect(RE::ActiveEffect* effect, RE::Actor* actor) -> bool
{
  if (!effect || !actor) { return false; }

  RE::ActorHandle& actor_handle = effect->caster;
  RE::MagicItem* spell = effect->spell;

  if (!spell || !actor_handle) { return false; }

  return actor->DispelEffect(spell, actor_handle, effect);
}

#define logi logger::info
#define loge logger::error
#define logd logger::debug
#define logw logger::warning