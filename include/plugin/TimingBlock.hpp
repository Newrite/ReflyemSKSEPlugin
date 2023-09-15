#pragma once

#include "Config.hpp"
#include "Core.hpp"

namespace Reflyem::TimingBlock
{
constexpr static auto BLOCK_DELAY = 0.65f;

auto is_allow_timing_parry(
    RE::Actor& attacker,
    RE::Actor& target,
    const Core::ActorsCache::Data& actor_data,
    const Config& config) -> bool;

auto is_allow_timing_block(
    RE::Actor& attacker,
    RE::Actor& target,
    const Core::ActorsCache::Data& actor_data,
    const Config& config) -> bool;

auto apply_spells_from_attack(
    RE::Actor& attacker,
    RE::Actor& target,
    const RE::HitData& hit_data,
    const Config& config) -> bool;
auto on_weapon_hit(RE::Actor& target, RE::HitData& hit_data, const Config& config) -> void;
} // namespace Reflyem::TimingBlock