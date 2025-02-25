#pragma once

#include "Config.hpp"
#include "Core.hpp"

namespace Reflyem::SpellStrikeEffect
{

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config&) -> void;

} 
