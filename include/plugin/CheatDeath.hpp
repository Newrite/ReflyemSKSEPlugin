#pragma once

#include "Config.hpp"

namespace Reflyem::CheatDeath
{

auto modify_actor_value(
    const RE::ValueModifierEffect* this_,
    RE::Actor* actor,
    float& value,
    const RE::ActorValue av,
    const Config& config) -> void;

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void;
} // namespace Reflyem::CheatDeath
