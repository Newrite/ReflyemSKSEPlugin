#pragma once

#include "Config.hpp"

namespace Reflyem::AbsorbShield {

auto is_enable_shield_value_from_cast_cost(RE::Actor* actor) -> bool;

auto get_shield_from_cast_cost_flat(RE::Actor* actor) -> float;

auto get_shield_from_cast_cost_percent(RE::Actor* actor) -> float;

auto get_shield_from_cast_cost_mult(RE::Actor* actor) -> float;

auto modify_actor_value_absorb(const RE::ValueModifierEffect* this_,
                        RE::Actor* actor,
                        float& value,
                        const RE::ActorValue av,
                        const Config&) -> void;
auto modify_actor_value_gettin(const RE::ValueModifierEffect* this_,
                        RE::Actor* actor,
                        float value,
                        const RE::ActorValue av,
                        const Config&) -> void;

auto on_weapon_hit_absorb(RE::Actor* target, RE::HitData* hit_data, const Config&) -> void;
auto on_weapon_hit_gettin(const RE::Actor* target, const RE::HitData* hit_data, const Config&) -> void;

auto update_actor(RE::Character* character, const float delta) -> void;

auto calculate_shield_value_from_cast_cost(const RE::TESForm* spell_form, RE::Actor* caster) -> float;

auto modify_shield_value_from_cast_cost(RE::Actor* actor, const float value_from_cast_cost) -> void;

auto action_event_handler(const SKSE::ActionEvent& event, RE::BSTEventSource<SKSE::ActionEvent>&) -> void;

namespace Callbacks {
auto set_frame_for_absorb_shield_callback() -> std::uint8_t;
}
namespace Papyrus {
auto ModifyAbsorbShieldValue(RE::StaticFunctionTag*, RE::Actor* actor, const float amount) -> void;
auto GetCurrentAbsorbShieldValue(RE::StaticFunctionTag*, const RE::Actor* actor) -> float;
auto GetMaxAbsorbShieldValue(RE::StaticFunctionTag*, RE::Actor* actor) -> float;
auto GetAbsorbShieldValueDegenDelay(RE::StaticFunctionTag*, RE::Actor* actor) -> float;
auto SetAbsorbShieldValueDegenDelay(RE::StaticFunctionTag*, RE::Actor* actor, const float amount) -> void;
}
}
