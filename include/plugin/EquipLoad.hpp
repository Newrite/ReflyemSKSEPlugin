#pragma once

#include "Config.hpp"

namespace Reflyem::EquipLoad {
auto update_actor(RE::Actor& actor, const Config& config) -> void;
auto get_actor_value(RE::ActorValueOwner& this_, const RE::ActorValue av) -> std::optional<float>;
auto set_actor_value(RE::ActorValueOwner& this_, const RE::ActorValue av, const float value)
    -> float;
auto mod_actor_value(RE::ActorValueOwner& this_, const RE::ActorValue av, const float value)
    -> float;
} // namespace Reflyem::EquipLoad