#pragma once

#include "Config.hpp"

namespace Reflyem::ComboSeries {

auto update(RE::Actor* actor, const float) -> void;

auto on_magic_hit(const RE::Projectile* projectile, RE::TESObjectREFR* refr_target) -> void;

auto on_weapon_hit(const RE::HitData& hit_data, RE::Actor* target) -> void;

}
