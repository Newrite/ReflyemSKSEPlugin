#pragma once

#include "Config.hpp"

namespace Reflyem::GameSettingsHandler {

// ReSharper disable once CppInconsistentNaming
class GMST final
{
  constexpr static auto STAMINA_BASH_BASE = "fStaminaBashBase";
  constexpr static auto STAMINA_POWER_BASH_BASE = "fStaminaPowerBashBase";
  constexpr static auto STAMINA_BLOCK_BASE = "fStaminaBlockBase";
  constexpr static auto STAMINA_ATTACK_WEAPON_BASE = "fStaminaAttackWeaponBase";
  constexpr static auto STAMINA_ATTACK_WEAPON_MULT = "fStaminaAttackWeaponMult";
  constexpr static auto STAMINA_BLOCK_DMG_MULT = "fStaminaBlockDmgMult";
  constexpr static auto OVER_ENCUMBERED = "sOverEncumbered";

public:
  GMST() = delete;
  ~GMST() = delete;
  GMST(const GMST& other) = delete;
  GMST(GMST&& other) noexcept = delete;
  GMST& operator=(const GMST& other) = delete;
  GMST& operator=(GMST&& other) noexcept = delete;

  static auto game_settings_handler(const Config& config) -> void
  {
    const auto settings = RE::GameSettingCollection::GetSingleton();
    if (!settings) { return; }
    if (config.resource_manager().block_spend_enable())
    {
      settings->GetSetting(STAMINA_BLOCK_BASE)->data.f = 0.0;
      settings->GetSetting(STAMINA_BLOCK_DMG_MULT)->data.f = 0.0;
    }
    if (config.resource_manager().bash_spend_enable() && !config.resource_manager().enable_alternate_melee_cost())
    {
      settings->GetSetting(STAMINA_BASH_BASE)->data.f = 0.0;
      settings->GetSetting(STAMINA_POWER_BASH_BASE)->data.f = 0.0;
    }
    if (config.resource_manager().weapon_spend_enable() && !config.resource_manager().enable_alternate_melee_cost())
    {
      settings->GetSetting(STAMINA_ATTACK_WEAPON_MULT)->data.f = 0.0;
      settings->GetSetting(STAMINA_ATTACK_WEAPON_BASE)->data.f = 0.0;
    }
    if (config.misc_fixes().disable_over_encumbered()) {
      static std::string zero_char = " ";
      static char* zero = zero_char.data();
      settings->GetSetting(OVER_ENCUMBERED)->data.s = zero;
    }
  }
};

}
