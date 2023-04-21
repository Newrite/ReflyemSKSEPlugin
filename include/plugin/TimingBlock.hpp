#pragma once

#include "Config.hpp"

namespace Reflyem::TimingBlock
{
constexpr static auto BLOCK_DELAY = 0.65f;
auto apply_spells_from_attack(
    RE::Actor& attacker,
    RE::Actor& target,
    const RE::HitData& hit_data,
    const Config& config) -> bool;
auto on_weapon_hit(RE::Actor& target, RE::HitData& hit_data, const Config& config) -> void;
} // namespace Reflyem::TimingBlock