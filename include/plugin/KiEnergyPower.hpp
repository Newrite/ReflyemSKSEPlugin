#pragma once
#include "Config.hpp"

namespace Reflyem::KiEnergyPower {

enum UseKiResult
{
  kOk,
  kFail,
  kNoneKi
};

auto get_cap(RE::Actor* actor, const Config& config) -> int32_t;
auto get_duration(RE::Actor* actor, const Config& config) -> float;
auto use_ki_energy(RE::Actor* actor, RE::SpellItem* power) -> UseKiResult;

auto update_summon_effect(RE::SummonCreatureEffect* active_effect, float) -> void;

auto update(RE::Actor* actor) -> void;

auto summons_proc(RE::Actor* caster) -> void;

auto action_event_handler(const SKSE::ActionEvent& event, RE::BSTEventSource<SKSE::ActionEvent>&) -> void;

namespace Callbacks {
auto set_frame_for_ki_energy_callback() -> std::uint8_t;
auto set_available_ki_energy_callback() -> GIGAWIDGET_API::AvailableKiEnergyResult;
}

auto CallKiSummonsHandle(RE::StaticFunctionTag*) -> void;

auto ModifyKiEnergyCooldown(RE::StaticFunctionTag*, RE::Actor* actor, const float amount, const bool closest) -> void;

auto SpendKiEnergy(RE::StaticFunctionTag*, RE::Actor* actor, const int cost) -> void;

auto GetAvailableKiEnergy(RE::StaticFunctionTag*, RE::Actor* actor) -> int;

} // namespace Reflyem::PotionsDrinkLimit
