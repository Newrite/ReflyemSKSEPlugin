﻿#pragma once
#include "Config.hpp"

namespace Reflyem::SpecialTechniques {

static constexpr auto TDM_DODGE = "TDM_Dodge";

auto get_actor_value_map(RE::ActorValueOwner* owner) -> std::map<RE::ActorValue, std::map<RE::ActorValue, float>>;
auto rotate_blocker(RE::Actor* target, const RE::TESObjectREFR* attacker, const Config& config)-> void;
auto normalize_attack(RE::HitData& hit_data, const Config& config, bool is_player) -> void;
auto update(RE::Character* character, const float delta) -> void;

}
