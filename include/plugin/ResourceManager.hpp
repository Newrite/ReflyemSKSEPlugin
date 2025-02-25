#pragma once

#include "Config.hpp"
#include "Core.hpp"

namespace Reflyem::ResourceManager
{

using FormMask = std::array<std::array<std::int32_t, 3>, 1>;
using ActorMask = std::array<std::array<std::int32_t, 3>, 3>;
using WeaponOrArmor = Core::Either<RE::TESObjectWEAP*, RE::TESObjectARMO*>;

auto spend_actor_value(RE::Actor& actor, const RE::ActorValue av, float value, const bool no_delay = false) -> void;

struct ResourceDrain final
{
  public:
  float stamina;
  float health;
  float magicka;

  explicit ResourceDrain(float a_stamina, float a_health, float a_magicka)
  {
    logger::debug("create drain values: S{} H{} M{}"sv, a_stamina, a_health, a_magicka);
    stamina = a_stamina * -1.f;
    health = a_health * -1.f;
    magicka = a_magicka * -1.f;
  }

  auto drain(RE::Actor& actor, const bool no_delay = false) -> void
  {
    logger::debug("drain values: S{} H{} M{}"sv, stamina, health, magicka);
    if (health != 0.f) { spend_actor_value(actor, RE::ActorValue::kHealth, std::abs(health), no_delay); }
    if (magicka != 0.f) { spend_actor_value(actor, RE::ActorValue::kMagicka, std::abs(magicka), no_delay); }
    if (stamina != 0.f) { spend_actor_value(actor, RE::ActorValue::kStamina, std::abs(stamina), no_delay); }
  }
};

auto regeneration_actor_value(
    RE::Actor& actor,
    const RE::ActorValue regen_av,
    const RE::ActorValue regen_av_mult) -> float;

// auto regeneration(RE::Actor& actor, const RE::ActorValue av, const float delta) -> float;

auto get_form_mask(const RE::BGSKeywordForm& form, const Config& config, const std::int32_t default_stat = 0) -> std::unique_ptr<FormMask>;

auto get_actor_mask(RE::Actor& form, const Config& config, const std::int32_t default_stat = 0) -> std::unique_ptr<FormMask>;

auto calc_mask_sum(const FormMask& f_mask) -> std::int32_t;

auto handle_mask_sum_for_drain_values(std::int32_t mask_sum, float cost)
    -> std::shared_ptr<ResourceDrain>;

auto ranged_spend_handler(RE::Character& character, const Config& config) -> void;

auto get_weapon_or_shield(const RE::Actor& actor) -> std::optional<WeaponOrArmor>;

auto update_actor(RE::Character& character, float delta, const Config& config) -> void;

auto animation_handler(const RE::BSAnimationGraphEvent& event, const Config& config) -> void;

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void;

auto get_attack_cost(RE::ActorValueOwner* owner, const RE::BGSAttackData* attack_data) -> bool;

} // namespace Reflyem::ResourceManager
