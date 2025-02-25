#pragma once
#include "Config.hpp"
#include "Core.hpp"

namespace Reflyem::StaggerSystem {

namespace TrueHUD {

auto get_max_resource_special_bar(RE::Actor* actor) -> float;

auto get_current_resource_special_bar(RE::Actor* actor) -> float;

}

auto action_event_handler(const SKSE::ActionEvent& event, RE::BSTEventSource<SKSE::ActionEvent>&) -> void;

auto update(RE::Actor* actor, const float delta) -> void;

auto on_magic_hit(RE::Projectile* projectile, RE::TESObjectREFR* refr_target) -> void;

auto on_weapon_hit(const RE::HitData& hit_data, RE::Actor* target) -> void;
}
