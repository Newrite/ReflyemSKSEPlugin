#pragma once

#include "Config.hpp"
#include "Core.hpp"

namespace Reflyem::TimingBlock
{

auto place_form_at_me(RE::TESObjectREFR* node, RE::TESForm* form) -> RE::TESObjectREFR*;

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

auto spawn_sparks(
    RE::Actor& target,
    const Config& config,
    const bool weapon_block,
    const bool parry, const bool is_magic_blocker) -> void;

auto apply_spells_from_attack(
    RE::Actor& attacker,
    RE::Actor& target,
    const RE::HitData& hit_data,
    const Config& config) -> bool;
auto on_weapon_hit(RE::Actor& target, RE::HitData& hit_data, const Config& config) -> void;
} // namespace Reflyem::TimingBlock
