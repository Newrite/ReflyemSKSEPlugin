#pragma once

#include "AnimationEventHandler.hpp"
#include "Config.hpp"
#include "Core.hpp"

namespace Reflyem::ResourceManager {

using FormMask  = std::array<std::array<std::int16_t, 3>, 1>;
using ActorMask = std::array<std::array<std::int16_t, 3>, 3>;

struct ResourceDrain {
public:
  float stamina;
  float health;
  float magicka;

  explicit ResourceDrain(float a_stamina, float a_health, float a_magicka);

  auto drain(RE::Actor& actor) -> void;
};

auto spend_actor_value(RE::Actor& actor, const RE::ActorValue av, float value) -> void;

auto on_update_actor_regeneration(RE::Character& character, Core::ActorsCache::Data& actor_data)
    -> void;

auto calc_mask_sum(const FormMask& f_mask) -> std::int32_t;

auto handle_mask_sum_for_drain_values(std::int32_t mask_sum, float cost)
    -> std::shared_ptr<ResourceDrain>;

auto ranged_spend_handler() -> void;

auto update_actor(RE::Character& character, float delta, const Config& config) -> void;

auto animation_handler(AnimationEventHandler::AnimationEvent animation, RE::Actor& actor,
                       bool is_power_attack, const Config& config) -> void;

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void;

} // namespace Reflyem::ResourceManager
