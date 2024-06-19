#pragma once

#include "Config.hpp"

namespace Reflyem::SlowTime {

auto start_slow_time(const Config& config) -> void;
auto update_actor(const RE::Character& actor, const float delta, const Config& config) -> void;
auto on_weapon_hit(const RE::Actor* target, const RE::HitData& hit_data, const Config& config) -> void;

}
