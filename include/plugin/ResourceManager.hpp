#pragma once

#include "Config.hpp"
#include "Core.hpp"

namespace Reflyem::ResourceManager {

// ReSharper disable once CppInconsistentNaming
class GMST {
  constexpr static auto STAMINA_BASH_BASE                = "fStaminaBashBase";
  constexpr static auto STAMINA_POWER_BASH_BASE          = "fStaminaPowerBashBase";
  constexpr static auto STAMINA_BLOCK_BASE               = "fStaminaBlockBase";
  constexpr static auto STAMINA_ATTACK_WEAPON_BASE       = "fStaminaAttackWeaponBase";
  constexpr static auto STAMINA_ATTACK_WEAPON_MULT       = "fStaminaAttackWeaponMult";
  constexpr static auto STAMINA_BLOCK_DMG_MULT           = "fStaminaBlockDmgMult";
public:
  GMST()                                                 = delete;
  ~GMST()                                                = delete;
  GMST(const GMST& other)                                = delete;
  GMST(GMST&& other) noexcept                            = delete;
  GMST&                 operator=(const GMST& other)     = delete;
  GMST&                 operator=(GMST&& other) noexcept = delete;

  static auto game_settings_handler(const Config& config) -> void {

    const auto settings = RE::GameSettingCollection::GetSingleton();
    if (!settings) {
      return;
    }
    if (config.resource_manager().block_spend_enable()) {
      settings->GetSetting(STAMINA_BLOCK_BASE)->data.f     = 0.0;
      settings->GetSetting(STAMINA_BLOCK_DMG_MULT)->data.f = 0.0;
    }
    if (config.resource_manager().bash_spend_enable()) {
      settings->GetSetting(STAMINA_BASH_BASE)->data.f       = 0.0;
      settings->GetSetting(STAMINA_POWER_BASH_BASE)->data.f = 0.0;
    }
    if (config.resource_manager().weapon_spend_enable()) {
      settings->GetSetting(STAMINA_ATTACK_WEAPON_MULT)->data.f = 0.0;
      settings->GetSetting(STAMINA_ATTACK_WEAPON_BASE)->data.f = 0.0;
    }
  }
};

using FormMask  = std::array<std::array<std::int32_t, 3>, 1>;
using ActorMask = std::array<std::array<std::int32_t, 3>, 3>;

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

auto ranged_spend_handler(RE::Character& character, const Config& config) -> void;

auto update_actor(RE::Character& character, float delta, const Config& config) -> void;

auto animation_handler(const RE::BSAnimationGraphEvent& event, const Config& config) -> void;

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void;

} // namespace Reflyem::ResourceManager
