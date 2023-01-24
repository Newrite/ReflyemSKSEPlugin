#pragma once

#include "Config.hpp"

namespace Reflyem {
namespace PetrifiedBlood {

auto modify_actor_value(RE::ValueModifierEffect* a_this, RE::Actor* a_actor, float& a_value, RE::ActorValue av,
                        const Reflyem::Config& config) -> void;

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Reflyem::Config& config) -> void;

auto character_update(RE::Character& character, float delta, const Reflyem::Config& config) -> void;
} // namespace PetrifiedBlood
} // namespace Reflyem