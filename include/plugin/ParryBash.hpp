#pragma once

#include "Config.hpp"
#include "PrecisionAPI.hpp"

namespace Reflyem::ParryBash
{
auto precision_pre_hit_callback(const PRECISION_API::PrecisionHitData&)
    -> PRECISION_API::PreHitCallbackReturn;
auto precision_weapons_collide_callback(const PRECISION_API::PrecisionHitData&)
    -> PRECISION_API::WeaponCollisionCallbackReturn;
auto on_melee_collision(RE::Actor& attacker, RE::Actor& victim, const Config& config) -> bool;
auto animation_handler(const RE::BSAnimationGraphEvent& event, const Config& config) -> void;
} // namespace Reflyem::ParryBash