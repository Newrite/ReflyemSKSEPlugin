#pragma once

#include "Config.hpp"
#include "Core.hpp"

namespace Reflyem::LeechEffect
{

auto character_update(RE::Character& character, float, const Config&, Core::ActorsCache::Data& actor_data) -> void;

auto modify_actor_value(const RE::ValueModifierEffect* this_,
                        RE::Actor* actor,
                        float& value,
                        const RE::ActorValue av,
                        const Config&) -> void;

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config&) -> void;

} 
