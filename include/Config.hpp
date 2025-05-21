#pragma once

#include "GigaWidgetAPI.hpp"
#include "PrecisionAPI.hpp"
#include "TrueHUDAPI.hpp"

#include <toml.hpp>

namespace Reflyem {

struct Config final
{

  private:
  struct MagicShieldConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] bool physical() const { return physical_; }

    [[nodiscard]] bool magick() const { return magick_; }

    [[nodiscard]] bool must_be_or_not_be() const { return must_be_or_not_be_; }

    [[nodiscard]] RE::BGSKeyword* mgef_keyword() const { return mgef_keyword_; }

    [[nodiscard]] RE::ActorValue av() const { return av_; }

    [[nodiscard]] RE::BGSKeyword* mgef_cost_keyword() const { return mgef_cost_keyword_; }

    MagicShieldConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    MagicShieldConfig() = default;

private:
    bool enable_;
    bool physical_;
    bool magick_;
    bool must_be_or_not_be_;
    RE::BGSKeyword* mgef_keyword_;
    RE::ActorValue av_;
    RE::BGSKeyword* mgef_cost_keyword_;
  };

  struct StaminaShieldConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] bool enable_av() const { return enable_av_; }

    [[nodiscard]] bool physical() const { return physical_; }

    [[nodiscard]] bool magick() const { return magick_; }

    [[nodiscard]] bool must_be_or_not_be() const { return must_be_or_not_be_; }

    [[nodiscard]] RE::BGSKeyword* mgef_keyword() const { return mgef_keyword_; }

    [[nodiscard]] RE::ActorValue av() const { return av_; }

    [[nodiscard]] RE::BGSKeyword* mgef_cost_keyword() const { return mgef_cost_keyword_; }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value() const { return keyword_shield_value_; }

    StaminaShieldConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    StaminaShieldConfig() = default;

private:
    bool enable_;
    bool enable_av_;
    bool physical_;
    bool magick_;
    bool must_be_or_not_be_;
    RE::BGSKeyword* mgef_keyword_;
    RE::ActorValue av_;
    RE::BGSKeyword* mgef_cost_keyword_;
    RE::BGSKeyword* keyword_shield_value_;
  };

  struct PetrifiedBloodConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] bool physical() const { return physical_; }

    [[nodiscard]] bool magick() const { return magick_; }

    [[nodiscard]] bool must_be_or_not_be() const { return must_be_or_not_be_; }

    [[nodiscard]] RE::BGSKeyword* mgef_keyword() const { return mgef_keyword_; }

    [[nodiscard]] RE::ActorValue av_physic() const { return av_physic_; }
    [[nodiscard]] RE::ActorValue av_magick() const { return av_magick_; }

    [[nodiscard]] RE::SpellItem* blood_spell() const { return blood_spell_; }

    PetrifiedBloodConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    PetrifiedBloodConfig() = default;

private:
    bool enable_;
    bool physical_;
    bool magick_;
    bool must_be_or_not_be_;
    RE::BGSKeyword* mgef_keyword_;
    RE::ActorValue av_physic_;
    RE::ActorValue av_magick_;
    RE::SpellItem* blood_spell_;
  };

  struct CheatDeathConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] bool physical() const { return physical_; }

    [[nodiscard]] bool magick() const { return magick_; }

    [[nodiscard]] RE::BGSKeyword* mgef_percent_keyword() const { return mgef_percent_keyword_; }

    CheatDeathConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CheatDeathConfig() = default;

private:
    bool enable_;
    bool physical_;
    bool magick_;
    RE::BGSKeyword* mgef_percent_keyword_;
  };

  struct VampirismConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::ActorValue av() const { return av_; }

    [[nodiscard]] bool mgef_health_enable() const { return mgef_health_enable_; }

    [[nodiscard]] RE::BGSKeyword* mgef_health_keyword() const { return mgef_health_keyword_; }

    [[nodiscard]] bool mgef_stamina_enable() const { return mgef_stamina_enable_; }

    [[nodiscard]] RE::BGSKeyword* mgef_stamina_keyword() const { return mgef_stamina_keyword_; }

    [[nodiscard]] bool mgef_magicka_enable() const { return mgef_magicka_enable_; }

    [[nodiscard]] RE::BGSKeyword* mgef_magicka_keyword() const { return mgef_magicka_keyword_; }

    [[nodiscard]] bool leech_vampirism() const { return leech_vampirism_; }
    [[nodiscard]] RE::SpellItem* leech_vampirism_spell() const { return leech_vampirism_spell_; }

    VampirismConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    VampirismConfig() = default;

private:
    // vampirism
    bool enable_;
    RE::ActorValue av_;

    // vampirism magick effect
    // health
    bool mgef_health_enable_;
    RE::BGSKeyword* mgef_health_keyword_;

    // stamina
    bool mgef_stamina_enable_;
    RE::BGSKeyword* mgef_stamina_keyword_;

    // magicka
    bool mgef_magicka_enable_;
    RE::BGSKeyword* mgef_magicka_keyword_;

    bool leech_vampirism_;
    RE::SpellItem* leech_vampirism_spell_;
  };

  struct MagicVampirismConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::ActorValue av() const { return av_; }

    [[nodiscard]] bool must_be_or_not_be() const { return must_be_or_not_be_; }

    [[nodiscard]] RE::BGSKeyword* mgef_keyword() const { return mgef_keyword_; }

    [[nodiscard]] bool mgef_health_enable() const { return mgef_health_enable_; }

    [[nodiscard]] RE::BGSKeyword* mgef_health_keyword() const { return mgef_health_keyword_; }

    [[nodiscard]] bool mgef_stamina_enable() const { return mgef_stamina_enable_; }

    [[nodiscard]] RE::BGSKeyword* mgef_stamina_keyword() const { return mgef_stamina_keyword_; }

    [[nodiscard]] bool mgef_magicka_enable() const { return mgef_magicka_enable_; }

    [[nodiscard]] RE::BGSKeyword* mgef_magicka_keyword() const { return mgef_magicka_keyword_; }

    [[nodiscard]] bool leech_vampirism() const { return leech_vampirism_; }
    [[nodiscard]] RE::SpellItem* leech_vampirism_spell() const { return leech_vampirism_spell_; }

    MagicVampirismConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    MagicVampirismConfig() = default;

private:
    // vampirism
    bool enable_;
    RE::ActorValue av_;
    bool must_be_or_not_be_;
    RE::BGSKeyword* mgef_keyword_;

    // vampirism magick effect
    // health
    bool mgef_health_enable_;
    RE::BGSKeyword* mgef_health_keyword_;

    // stamina
    bool mgef_stamina_enable_;
    RE::BGSKeyword* mgef_stamina_keyword_;

    // magicka
    bool mgef_magicka_enable_;
    RE::BGSKeyword* mgef_magicka_keyword_;

    bool leech_vampirism_;
    RE::SpellItem* leech_vampirism_spell_;
  };

  struct SpeedCastingConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::TESGlobal* global() const { return global_; }

    [[nodiscard]] RE::ActorValue av() const { return av_; }

    [[nodiscard]] float high() const { return high_; }

    [[nodiscard]] float low() const { return low_; }

    SpeedCastingConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    SpeedCastingConfig() = default;

private:
    bool enable_;
    RE::TESGlobal* global_;
    RE::ActorValue av_;
    float high_;
    float low_;
  };

  struct WeaponCritConfig final
  {
public:
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::ActorValue chance_av() const { return chance_av_; }

    [[nodiscard]] RE::ActorValue damage_av() const { return damage_av_; }

    [[nodiscard]] std::int32_t high() const { return high_; }

    [[nodiscard]] RE::BGSKeyword* keyword_immun() const { return keyword_immun_; }

    [[nodiscard]] RE::BGSSoundDescriptorForm* crit_sound() const { return crit_sound_; }

    WeaponCritConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    WeaponCritConfig() = default;

private:
    bool enable_;
    RE::ActorValue chance_av_;
    RE::ActorValue damage_av_;
    std::int32_t high_;
    RE::BGSKeyword* keyword_immun_;
    RE::BGSSoundDescriptorForm* crit_sound_;
  };

  struct MagickCritConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::ActorValue chance_av() const { return chance_av_; }

    [[nodiscard]] RE::ActorValue damage_av() const { return damage_av_; }

    [[nodiscard]] std::int32_t high() const { return high_; }

    [[nodiscard]] bool must_be_or_not_be() const { return must_be_or_not_be_; }

    [[nodiscard]] RE::BGSKeyword* mgef_keyword() const { return mgef_keyword_; }

    [[nodiscard]] RE::BGSKeyword* keyword_immun() const { return keyword_immun_; }

    [[nodiscard]] RE::BGSSoundDescriptorForm* crit_sound() const { return crit_sound_; }

    MagickCritConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    MagickCritConfig() = default;

private:
    bool enable_;
    RE::ActorValue chance_av_;
    RE::ActorValue damage_av_;
    std::int32_t high_;
    bool must_be_or_not_be_;
    RE::BGSKeyword* mgef_keyword_;
    RE::BGSKeyword* keyword_immun_;
    RE::BGSSoundDescriptorForm* crit_sound_;
  };

  struct CritRevisedConfig final
  {
  public:
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] bool enable_damage_av() const { return enable_damage_av_; }
    [[nodiscard]] bool enable_chance_av() const { return enable_chance_av_; }
    [[nodiscard]] bool enable_weapon_crit() const { return enable_weapon_crit_; }
    [[nodiscard]] bool enable_magick_crit() const { return enable_magick_crit_; }
    [[nodiscard]] RE::ActorValue chance_av() const { return chance_av_; }
    [[nodiscard]] RE::ActorValue damage_av() const { return damage_av_; }
    [[nodiscard]] RE::BGSKeyword* keyword_immun() const { return keyword_immun_; }
    [[nodiscard]] RE::BGSKeyword* keyword_damage() const { return keyword_damage_; }
    [[nodiscard]] RE::BGSKeyword* keyword_chance() const { return keyword_chance_; }
    [[nodiscard]] RE::BGSSoundDescriptorForm* weapon_crit_sound() const { return weapon_crit_sound_; }
    [[nodiscard]] RE::BGSSoundDescriptorForm* magick_crit_sound() const { return magick_crit_sound_; }
    [[nodiscard]] RE::BGSListForm* cast_spells() const { return cast_spells_; }
    [[nodiscard]] RE::BGSListForm* cast_keywords() const { return cast_keywords_; }

    CritRevisedConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CritRevisedConfig() = default;

  private:
    bool enable_;
    bool enable_damage_av_;
    bool enable_chance_av_;
    bool enable_weapon_crit_;
    bool enable_magick_crit_;
    RE::ActorValue chance_av_;
    RE::ActorValue damage_av_;
    RE::BGSKeyword* keyword_immun_;
    RE::BGSKeyword* keyword_damage_;
    RE::BGSKeyword* keyword_chance_;
    RE::BGSSoundDescriptorForm* weapon_crit_sound_;
    RE::BGSSoundDescriptorForm* magick_crit_sound_;
    RE::BGSListForm* cast_spells_;
    RE::BGSListForm* cast_keywords_;
  };

  struct CastOnCritConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] bool enable_weapon() const { return enable_weapon_; }

    [[nodiscard]] bool physical() const { return physical_; }

    [[nodiscard]] bool magick() const { return magick_; }

    [[nodiscard]] float magick_cooldawn() const { return magick_cooldawn_; }

    [[nodiscard]] RE::BGSListForm* weapon_formlist_spells() const { return weapon_formlist_spells_; }

    [[nodiscard]] RE::BGSListForm* weapon_formlist_needkw() const { return weapon_formlist_needkw_; }

    [[nodiscard]] RE::BGSListForm* magick_formlist_spells() const { return magick_formlist_spells_; }

    [[nodiscard]] RE::BGSListForm* magick_formlist_needkw() const { return magick_formlist_needkw_; }

    CastOnCritConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnCritConfig() = default;

private:
    bool enable_;
    bool enable_weapon_;
    bool physical_;
    bool magick_;
    float magick_cooldawn_;
    RE::BGSListForm* weapon_formlist_spells_;
    RE::BGSListForm* weapon_formlist_needkw_;
    RE::BGSListForm* magick_formlist_spells_;
    RE::BGSListForm* magick_formlist_needkw_;
  };

  struct CastOnHitConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* formlist_melee_spells() const { return formlist_melee_spells_; }

    [[nodiscard]] RE::BGSListForm* formlist_melee_needkw() const { return formlist_melee_needkw_; }
    [[nodiscard]] RE::BGSListForm* formlist_bash_spells() const { return formlist_bash_spells_; }
    [[nodiscard]] RE::BGSListForm* formlist_bash_needkw() const { return formlist_bash_needkw_; }
    [[nodiscard]] RE::BGSListForm* formlist_ranged_spells() const { return formlist_ranged_spells_; }
    [[nodiscard]] RE::BGSListForm* formlist_ranged_needkw() const { return formlist_ranged_needkw_; }

    CastOnHitConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnHitConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* formlist_melee_spells_;
    RE::BGSListForm* formlist_melee_needkw_;
    RE::BGSListForm* formlist_bash_spells_;
    RE::BGSListForm* formlist_bash_needkw_;
    RE::BGSListForm* formlist_ranged_spells_;
    RE::BGSListForm* formlist_ranged_needkw_;
  };

  struct CastOnMagicProjectileHitConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* formlist_spells() const { return formlist_spells_; }

    [[nodiscard]] RE::BGSListForm* formlist_needkw() const { return formlist_needkw_; }

    CastOnMagicProjectileHitConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnMagicProjectileHitConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* formlist_spells_;
    RE::BGSListForm* formlist_needkw_;
  };

  struct CastOnTimingBlockStaggerConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* formlist_spells() const { return formlist_spells_; }

    [[nodiscard]] RE::BGSListForm* formlist_needkw() const { return formlist_needkw_; }

    CastOnTimingBlockStaggerConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnTimingBlockStaggerConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* formlist_spells_;
    RE::BGSListForm* formlist_needkw_;
  };

  struct CastOnMagicBlockerConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* form_list_block_spells() const { return form_list_block_spells_; }
    [[nodiscard]] RE::BGSListForm* form_list_block_keyword() const { return form_list_block_keyword_; }
    [[nodiscard]] RE::BGSListForm* form_list_timing_block_spells() const { return form_list_timing_block_spells_; }
    [[nodiscard]] RE::BGSListForm* form_list_timing_block_keyword() const { return form_list_timing_block_keyword_; }
    [[nodiscard]] RE::BGSListForm* form_list_parry_block_spells() const { return form_list_parry_block_spells_; }
    [[nodiscard]] RE::BGSListForm* form_list_parry_block_keyword() const { return form_list_parry_block_keyword_; }
    [[nodiscard]] RE::BGSListForm* form_list_projectile_block_spells_physical() const
    {
      return form_list_projectile_block_spells_physical_;
    }
    [[nodiscard]] RE::BGSListForm* form_list_projectile_block_keyword_physical() const
    {
      return form_list_projectile_block_keyword_physical_;
    }
    [[nodiscard]] RE::BGSListForm* form_list_projectile_timing_block_spells_physical() const
    {
      return form_list_projectile_timing_block_spells_physical_;
    }
    [[nodiscard]] RE::BGSListForm* form_list_projectile_timing_block_keyword_physical() const
    {
      return form_list_projectile_timing_block_keyword_physical_;
    }
    [[nodiscard]] RE::BGSListForm* form_list_projectile_block_spells_magick() const
    {
      return form_list_projectile_block_spells_magick_;
    }
    [[nodiscard]] RE::BGSListForm* form_list_projectile_block_keyword_magick() const
    {
      return form_list_projectile_block_keyword_magick_;
    }
    [[nodiscard]] RE::BGSListForm* form_list_projectile_timing_block_spells_magick() const
    {
      return form_list_projectile_timing_block_spells_magick_;
    }
    [[nodiscard]] RE::BGSListForm* form_list_projectile_timing_block_keyword_magick() const
    {
      return form_list_projectile_timing_block_keyword_magick_;
    }

    CastOnMagicBlockerConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnMagicBlockerConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* form_list_block_spells_;
    RE::BGSListForm* form_list_block_keyword_;
    RE::BGSListForm* form_list_timing_block_spells_;
    RE::BGSListForm* form_list_timing_block_keyword_;
    RE::BGSListForm* form_list_parry_block_spells_;
    RE::BGSListForm* form_list_parry_block_keyword_;
    RE::BGSListForm* form_list_projectile_block_spells_physical_;
    RE::BGSListForm* form_list_projectile_block_keyword_physical_;
    RE::BGSListForm* form_list_projectile_timing_block_spells_physical_;
    RE::BGSListForm* form_list_projectile_timing_block_keyword_physical_;
    RE::BGSListForm* form_list_projectile_block_spells_magick_;
    RE::BGSListForm* form_list_projectile_block_keyword_magick_;
    RE::BGSListForm* form_list_projectile_timing_block_spells_magick_;
    RE::BGSListForm* form_list_projectile_timing_block_keyword_magick_;
  };

  struct CastOnBlockConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* formlist_spells() const { return formlist_spells_; }

    [[nodiscard]] RE::BGSListForm* formlist_needkw() const { return formlist_needkw_; }

    CastOnBlockConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnBlockConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* formlist_spells_;
    RE::BGSListForm* formlist_needkw_;
  };

  struct CastOnBlockParryConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* formlist_spells() const { return formlist_spells_; }

    [[nodiscard]] RE::BGSListForm* formlist_needkw() const { return formlist_needkw_; }

    CastOnBlockParryConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnBlockParryConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* formlist_spells_;
    RE::BGSListForm* formlist_needkw_;
  };

  struct CastOnTimingBlockConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* formlist_spells() const { return formlist_spells_; }

    [[nodiscard]] RE::BGSListForm* formlist_needkw() const { return formlist_needkw_; }

    CastOnTimingBlockConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnTimingBlockConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* formlist_spells_;
    RE::BGSListForm* formlist_needkw_;
  };

  struct CastOnParryBashConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* formlist_spells() const { return formlist_spells_; }

    [[nodiscard]] RE::BGSListForm* formlist_needkw() const { return formlist_needkw_; }

    CastOnParryBashConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnParryBashConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* formlist_spells_;
    RE::BGSListForm* formlist_needkw_;
  };

  struct CastOnKillConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* formlist_spells() const { return formlist_spells_; }

    [[nodiscard]] RE::BGSListForm* formlist_needkw() const { return formlist_needkw_; }

    CastOnKillConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnKillConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* formlist_spells_;
    RE::BGSListForm* formlist_needkw_;
  };

  struct CastOnDrinkConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* formlist_spells() const { return formlist_spells_; }

    [[nodiscard]] RE::BGSListForm* formlist_needkw() const { return formlist_needkw_; }

    CastOnDrinkConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnDrinkConfig() = default;

  private:
    bool enable_;
    RE::BGSListForm* formlist_spells_;
    RE::BGSListForm* formlist_needkw_;
  };

  struct CastOnSoulTrapConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* formlist_spells() const { return formlist_spells_; }

    [[nodiscard]] RE::BGSListForm* formlist_needkw() const { return formlist_needkw_; }

    CastOnSoulTrapConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnSoulTrapConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* formlist_spells_;
    RE::BGSListForm* formlist_needkw_;
  };

  struct ClairvoyanceConfig final
  {

    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] RE::SpellItem* clairvoyance_spell() const { return clairvoyance_spell_; }
    [[nodiscard]] std::uint32_t key() const { return key_; }

    ClairvoyanceConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    ClairvoyanceConfig() = default;

private:
    bool enable_;
    RE::SpellItem* clairvoyance_spell_;
    std::uint32_t key_;
  };

  struct CastOnDodgeConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* formlist_spells() const { return formlist_spells_; }

    [[nodiscard]] RE::BGSListForm* formlist_needkw() const { return formlist_needkw_; }

    CastOnDodgeConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnDodgeConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* formlist_spells_;
    RE::BGSListForm* formlist_needkw_;
  };

  struct ResourceManagerConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] bool weapon_spend_enable() const { return weapon_spend_enable_; }

    [[nodiscard]] bool bash_spend_enable() const { return bash_spend_enable_; }

    [[nodiscard]] bool block_spend_enable() const { return block_spend_enable_; }

    [[nodiscard]] bool regeneration_enable() const { return regeneration_enable_; }

    [[nodiscard]] bool infamy_enable() const { return infamy_enable_; }

    [[nodiscard]] float regen_delay() const { return regen_delay_; }

    [[nodiscard]] float weight_mult() const { return weight_mult_; }

    [[nodiscard]] float damage_mult() const { return damage_mult_; }

    [[nodiscard]] float armor_mult() const { return armor_mult_; }

    [[nodiscard]] float global_mult() const { return global_mult_; }

    [[nodiscard]] float power_attack_mult() const { return power_attack_mult_; }

    [[nodiscard]] float jump_cost() const { return jump_cost_; }

    [[nodiscard]] RE::SpellItem* spell_block_attack() const { return spell_block_attack_; }

    [[nodiscard]] RE::SpellItem* spell_block_power_attack() const { return spell_block_power_attack_; }

    [[nodiscard]] RE::SpellItem* spell_block_bash() const { return spell_block_bash_; }

    [[nodiscard]] RE::TESObjectWEAP* unarmed_weapon() const { return unarmed_weapon_; }

    [[nodiscard]] RE::ActorValue attack_cost_av() const { return attack_cost_av_; }

    [[nodiscard]] std::int32_t attack_cost_high() const { return attack_cost_high_; }

    [[nodiscard]] std::int32_t attack_cost_low() const { return attack_cost_low_; }

    [[nodiscard]] RE::ActorValue power_attack_cost_av() const { return power_attack_cost_av_; }

    [[nodiscard]] std::int32_t power_attack_cost_high() const { return power_attack_cost_high_; }

    [[nodiscard]] std::int32_t power_attack_cost_low() const { return power_attack_cost_low_; }

    [[nodiscard]] RE::BGSKeyword* health_kw() const { return health_kw_; }

    [[nodiscard]] RE::BGSKeyword* stamina_kw() const { return stamina_kw_; }

    [[nodiscard]] RE::BGSKeyword* magicka_kw() const { return magicka_kw_; }

    [[nodiscard]] RE::BGSKeyword* convert_stamina_health_kw() const { return convert_stamina_health_kw_; }

    [[nodiscard]] RE::BGSKeyword* convert_stamina_magicka_kw() const { return convert_stamina_magicka_kw_; }

    [[nodiscard]] RE::BGSKeyword* convert_magicka_health_kw() const { return convert_magicka_health_kw_; }

    [[nodiscard]] RE::BGSKeyword* convert_magicka_stamina_kw() const { return convert_magicka_stamina_kw_; }

    [[nodiscard]] RE::BGSKeyword* convert_health_stamina_kw() const { return convert_health_stamina_kw_; }

    [[nodiscard]] RE::BGSKeyword* convert_health_magicka_kw() const { return convert_health_magicka_kw_; }
    [[nodiscard]] bool enable_alternate_melee_cost() const { return enable_alternate_melee_cost_; }
    [[nodiscard]] bool use_stamina_mult_from_attack_data() const { return use_stamina_mult_from_attack_data_; }
    [[nodiscard]] bool enable_overall_attack_cost_av() const { return enable_overall_attack_cost_av_; }
    [[nodiscard]] float stamina_power_attack_weapon_base() const { return stamina_power_attack_weapon_base_; }
    [[nodiscard]] float power_bash_stamina_penalty() const { return power_bash_stamina_penalty_; }
    [[nodiscard]] float stamina_bash_mult() const { return stamina_bash_mult_; }

    ResourceManagerConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    ResourceManagerConfig() = default;

private:
    bool enable_;
    bool weapon_spend_enable_;
    bool bash_spend_enable_;
    bool block_spend_enable_;
    bool regeneration_enable_;
    bool infamy_enable_;

    float regen_delay_;

    float weight_mult_;
    float damage_mult_;
    float armor_mult_;
    float global_mult_;
    float power_attack_mult_;
    float jump_cost_;

    RE::SpellItem* spell_block_attack_;
    RE::SpellItem* spell_block_power_attack_;
    RE::SpellItem* spell_block_bash_;

    RE::TESObjectWEAP* unarmed_weapon_;

    RE::ActorValue attack_cost_av_;
    std::int32_t attack_cost_high_;
    std::int32_t attack_cost_low_;

    RE::ActorValue power_attack_cost_av_;
    std::int32_t power_attack_cost_high_;
    std::int32_t power_attack_cost_low_;

    RE::BGSKeyword* health_kw_;
    RE::BGSKeyword* stamina_kw_;
    RE::BGSKeyword* magicka_kw_;

    RE::BGSKeyword* convert_stamina_health_kw_;
    RE::BGSKeyword* convert_stamina_magicka_kw_;
    RE::BGSKeyword* convert_magicka_health_kw_;
    RE::BGSKeyword* convert_magicka_stamina_kw_;
    RE::BGSKeyword* convert_health_stamina_kw_;
    RE::BGSKeyword* convert_health_magicka_kw_;

    bool enable_alternate_melee_cost_;
    bool use_stamina_mult_from_attack_data_;
    bool enable_overall_attack_cost_av_;
    float stamina_power_attack_weapon_base_;
    float power_bash_stamina_penalty_;
    float stamina_bash_mult_;
  };

  struct TkDodgeConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] bool enable_mco_recovery() const { return enable_mco_recovery_; }

    [[nodiscard]] bool enable_dodge_when_attack_start() const { return enable_dodge_when_attack_start_; }

    [[nodiscard]] bool enable_dodge_when_not_enough_resource() const { return enable_dodge_when_not_enough_resource_; }

    [[nodiscard]] std::uint32_t key() const { return key_; }

    [[nodiscard]] std::int32_t step() const { return step_; }

    [[nodiscard]] float iframe_duration() const { return iframe_duration_; }

    [[nodiscard]] RE::BGSKeyword* iframe_duration_mgef_kw() const { return iframe_duration_mgef_kw_; }

    [[nodiscard]] float gamepad_treshold() const { return gamepad_treshold_; }

    [[nodiscard]] bool sprint_tapping_dodge() const { return sprint_tapping_dodge_; }

    [[nodiscard]] bool block_dodge_when_attack() const { return block_dodge_when_attack_; }

    [[nodiscard]] RE::BGSKeyword* block_dodge_when_attack_keyword() const { return block_dodge_when_attack_keyword_; }

    [[nodiscard]] bool block_dodge_when_power_attack() const { return block_dodge_when_power_attack_; }
    [[nodiscard]] bool interrupt_cast_when_dodge() const { return interrupt_cast_when_dodge_; }

    [[nodiscard]] RE::BGSKeyword* block_dodge_when_power_attack_keyword() const
    {
      return block_dodge_when_power_attack_keyword_;
    }

    [[nodiscard]] bool block_dodge_when_casting() const { return block_dodge_when_casting_; }

    [[nodiscard]] RE::BGSKeyword* block_dodge_when_casting_keyword() const { return block_dodge_when_casting_keyword_; }

    [[nodiscard]] float key_up_delay() const { return key_up_delay_; }

    [[nodiscard]] float max_cost() const { return max_cost_; }
    [[nodiscard]] float percent_cost() const { return percent_cost_; }

    [[nodiscard]] float min_cost() const { return min_cost_; }

    [[nodiscard]] float flat_cost() const { return flat_cost_; }

    [[nodiscard]] RE::BGSKeyword* cost_from_mgef_kw() const { return cost_from_mgef_kw_; }

    [[nodiscard]] float equipped_weight_mult() const { return equipped_weight_mult_; }

    [[nodiscard]] RE::BGSKeyword* health_kw() const { return health_kw_; }

    [[nodiscard]] RE::BGSKeyword* stamina_kw() const { return stamina_kw_; }

    [[nodiscard]] RE::BGSKeyword* magicka_kw() const { return magicka_kw_; }

    [[nodiscard]] RE::BGSKeyword* enable_dodge_in_stagger_keyword() const { return enable_dodge_in_stagger_keyword_; }

    [[nodiscard]] float stagger_cost_penalty_mult() const { return stagger_cost_penalty_mult_; }

    TkDodgeConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    TkDodgeConfig() = default;

private:
    bool enable_;
    bool enable_mco_recovery_;
    bool enable_dodge_when_attack_start_;
    bool enable_dodge_when_not_enough_resource_;
    RE::BGSKeyword* enable_dodge_in_stagger_keyword_;
    float stagger_cost_penalty_mult_;
    std::uint32_t key_;
    std::int32_t step_;
    float iframe_duration_;
    RE::BGSKeyword* iframe_duration_mgef_kw_;
    float gamepad_treshold_;
    bool sprint_tapping_dodge_;
    bool block_dodge_when_attack_;
    bool interrupt_cast_when_dodge_;
    RE::BGSKeyword* block_dodge_when_attack_keyword_;
    bool block_dodge_when_power_attack_;
    RE::BGSKeyword* block_dodge_when_power_attack_keyword_;
    bool block_dodge_when_casting_;
    RE::BGSKeyword* block_dodge_when_casting_keyword_;
    float key_up_delay_;
    float percent_cost_;
    float max_cost_;
    float min_cost_;
    float flat_cost_;
    RE::BGSKeyword* cost_from_mgef_kw_;
    float equipped_weight_mult_;
    RE::BGSKeyword* health_kw_;
    RE::BGSKeyword* stamina_kw_;
    RE::BGSKeyword* magicka_kw_;
  };

  struct CasterAdditionsConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::SpellItem* spell_add_when_casting() const { return spell_add_when_casting_; }

    [[nodiscard]] bool stamina_cost() const { return stamina_cost_; }

    [[nodiscard]] bool allow_cast_without_stamina() const { return allow_cast_without_stamina_; }

    [[nodiscard]] float stamina_cost_mult() const { return stamina_cost_mult_; }

    [[nodiscard]] RE::BGSSoundDescriptorForm* mag_fail_sound() const { return mag_fail_sound_; }

    [[nodiscard]] bool enable_rally_manacost() const { return enable_rally_manacost_; }
    [[nodiscard]] float rally_manacost_mult() const { return rally_manacost_mult_; }
    [[nodiscard]] float rally_magicka_mult() const { return rally_magicka_mult_; }
    [[nodiscard]] float magicka_mult_restore_per_second_from_rally() const
    {
      return magicka_mult_restore_per_second_from_rally_;
    }

    CasterAdditionsConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CasterAdditionsConfig() = default;

private:
    bool enable_;
    RE::SpellItem* spell_add_when_casting_;
    bool stamina_cost_;
    bool allow_cast_without_stamina_;
    float stamina_cost_mult_;
    RE::BGSSoundDescriptorForm* mag_fail_sound_;
    bool enable_rally_manacost_;
    float rally_manacost_mult_;
    float rally_magicka_mult_;
    float magicka_mult_restore_per_second_from_rally_;
  };

  struct MagicBlockerConfig final
  {

    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] bool enable_regen_while_casting_ward() const { return enable_regen_while_casting_ward_; }
    [[nodiscard]] bool disable_auto_rotation360_block() const { return disable_auto_rotation360_block_; }
    [[nodiscard]] RE::BGSKeyword* timing_block_integration_keyword() const { return timing_block_integration_keyword_; }
    [[nodiscard]] RE::BGSKeyword* block_parry_integration_keyword() const { return block_parry_integration_keyword_; }
    [[nodiscard]] RE::BGSKeyword* projectile_block_integration_keyword() const
    {
      return projectile_block_integration_keyword_;
    }
    [[nodiscard]] RE::BGSKeyword* projectile_timing_block_integration_keyword() const
    {
      return projectile_timing_block_integration_keyword_;
    }
    [[nodiscard]] RE::BGSKeyword* magic_blocker_keyword() const { return magic_blocker_keyword_; }
    [[nodiscard]] RE::BGSSoundDescriptorForm* timing_block_sound() const { return timing_block_sound_; }
    [[nodiscard]] RE::BGSSoundDescriptorForm* block_sound() const { return block_sound_; }
    [[nodiscard]] RE::BGSSoundDescriptorForm* parry_sound() const { return parry_sound_; }
    [[nodiscard]] RE::TESObjectARMO* resource_manager_keyword_holder() const
    {
      return resource_manager_keyword_holder_;
    }
    [[nodiscard]] RE::TESObjectACTI* blank_activator() const { return blank_activator_; }
    [[nodiscard]] RE::Explosion* spark() const { return spark_; }
    [[nodiscard]] RE::Explosion* spark_flare() const { return spark_flare_; }
    [[nodiscard]] RE::Explosion* spark_halo() const { return spark_halo_; }

    MagicBlockerConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    MagicBlockerConfig() = default;

private:
    bool enable_;
    bool enable_regen_while_casting_ward_;
    bool disable_auto_rotation360_block_;
    RE::BGSKeyword* timing_block_integration_keyword_;
    RE::BGSKeyword* block_parry_integration_keyword_;
    RE::BGSKeyword* projectile_block_integration_keyword_;
    RE::BGSKeyword* projectile_timing_block_integration_keyword_;
    RE::BGSKeyword* magic_blocker_keyword_;
    RE::BGSSoundDescriptorForm* timing_block_sound_;
    RE::BGSSoundDescriptorForm* block_sound_;
    RE::BGSSoundDescriptorForm* parry_sound_;
    RE::TESObjectACTI* blank_activator_;
    RE::TESObjectARMO* resource_manager_keyword_holder_;
    RE::Explosion* spark_;
    RE::Explosion* spark_flare_;
    RE::Explosion* spark_halo_;
  };

  struct HighlightingConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] float highlight_duration() const { return highlight_duration_; }
    [[nodiscard]] RE::TESEffectShader* power_attack_shader() const { return power_attack_shader_; }
    [[nodiscard]] RE::BGSArtObject* power_attack_art_weapon() const { return power_attack_art_weapon_; }
    [[nodiscard]] RE::BGSArtObject* power_attack_art_other() const { return power_attack_art_other_; }
    [[nodiscard]] RE::BGSArtObject* range_release_art() const { return range_release_art_; }
    [[nodiscard]] RE::BGSKeyword* power_attack_keyword() const { return power_attack_keyword_; }
    [[nodiscard]] RE::BGSKeyword* range_release_keyword() const { return range_release_keyword_; }

    HighlightingConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    HighlightingConfig() = default;

private:
    bool enable_;
    float highlight_duration_;
    RE::BGSKeyword* power_attack_keyword_;
    RE::TESEffectShader* power_attack_shader_;
    RE::BGSArtObject* power_attack_art_weapon_;
    RE::BGSArtObject* power_attack_art_other_;
    RE::BGSKeyword* range_release_keyword_;
    RE::BGSArtObject* range_release_art_;
  };

  struct MagicWeaponConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* spells() const { return spells_; }

    [[nodiscard]] RE::BGSListForm* keywords() const { return keywords_; }

    [[nodiscard]] RE::BGSListForm* globals() const { return globals_; }

    MagicWeaponConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    MagicWeaponConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* spells_;
    RE::BGSListForm* keywords_;
    RE::BGSListForm* globals_;
  };

  struct RecoupEffectConfig final
  {

    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] RE::BGSListForm* spells_melee() const { return spells_melee_; }
    [[nodiscard]] RE::BGSListForm* spells_ranged() const { return spells_ranged_; }
    [[nodiscard]] RE::BGSListForm* spells_magic() const { return spells_magic_; }
    [[nodiscard]] RE::BGSListForm* keywords_melee() const { return keywords_melee_; }
    [[nodiscard]] RE::BGSListForm* keywords_ranged() const { return keywords_ranged_; }
    [[nodiscard]] RE::BGSListForm* keywords_magic() const { return keywords_magic_; }

    RecoupEffectConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    RecoupEffectConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* spells_melee_;
    RE::BGSListForm* spells_ranged_;
    RE::BGSListForm* spells_magic_;
    RE::BGSListForm* keywords_melee_;
    RE::BGSListForm* keywords_ranged_;
    RE::BGSListForm* keywords_magic_;
  };

  struct LeechEffectConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] RE::BGSListForm* spells_melee() const { return spells_melee_; }
    [[nodiscard]] RE::BGSListForm* spells_ranged() const { return spells_ranged_; }
    [[nodiscard]] RE::BGSListForm* spells_magic() const { return spells_magic_; }
    [[nodiscard]] RE::BGSListForm* keywords_melee() const { return keywords_melee_; }
    [[nodiscard]] RE::BGSListForm* keywords_ranged() const { return keywords_ranged_; }
    [[nodiscard]] RE::BGSListForm* keywords_magic() const { return keywords_magic_; }

    LeechEffectConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    LeechEffectConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* spells_melee_;
    RE::BGSListForm* spells_ranged_;
    RE::BGSListForm* spells_magic_;
    RE::BGSListForm* keywords_melee_;
    RE::BGSListForm* keywords_ranged_;
    RE::BGSListForm* keywords_magic_;
  };

  struct SpellStrikeEffectConfig final
  {

    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] RE::BGSListForm* spells_melee() const { return spells_melee_; }
    [[nodiscard]] RE::BGSListForm* spells_ranged() const { return spells_ranged_; }
    [[nodiscard]] RE::BGSListForm* keywords_melee() const { return keywords_melee_; }
    [[nodiscard]] RE::BGSListForm* keywords_ranged() const { return keywords_ranged_; }

    SpellStrikeEffectConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    SpellStrikeEffectConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* spells_melee_;
    RE::BGSListForm* spells_ranged_;
    RE::BGSListForm* keywords_melee_;
    RE::BGSListForm* keywords_ranged_;
  };

  struct SpecialTechniquesConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] RE::BGSKeyword* enable_360_block() const { return enable_360_block_; }
    [[nodiscard]] RE::BGSKeyword* ignore_apply_spell_and_enchanting_keyword_id() const
    {
      return ignore_apply_spell_and_enchanting_keyword_id_;
    }
    [[nodiscard]] float normalize_power_attack_mult_pc() const { return normalize_power_attack_mult_pc_; }
    [[nodiscard]] float normalize_power_attack_mult_npc() const { return normalize_power_attack_mult_npc_; }
    [[nodiscard]] float normalize_normal_attack_mult_pc() const { return normalize_normal_attack_mult_pc_; }
    [[nodiscard]] float normalize_normal_attack_mult_npc() const { return normalize_normal_attack_mult_npc_; }
    [[nodiscard]] float normalize_max_regen_delay_health() const { return normalize_max_regen_delay_health_; }
    [[nodiscard]] float normalize_max_regen_delay_magicka() const { return normalize_max_regen_delay_magicka_; }
    [[nodiscard]] float normalize_max_regen_delay_stamina() const { return normalize_max_regen_delay_stamina_; }
    [[nodiscard]] float slow_after_block_hit_mult() const { return slow_after_block_hit_mult_; }
    [[nodiscard]] float slow_after_block_hit_duration() const { return slow_after_block_hit_duration_; }
    [[nodiscard]] bool dynamic_regeneration_health() const { return dynamic_regeneration_health_; }
    [[nodiscard]] bool dynamic_regeneration_magicka() const { return dynamic_regeneration_magicka_; }
    [[nodiscard]] bool dynamic_regeneration_stamina() const { return dynamic_regeneration_stamina_; }
    [[nodiscard]] RE::BGSKeyword* no_block_rotate_keyword() const { return no_block_rotate_keyword_; }
    [[nodiscard]] RE::BGSKeyword* active_block_keyword() const { return active_block_keyword_; }
    [[nodiscard]] RE::BGSKeyword* stamina_cost_spell_keyword() const { return stamina_cost_spell_keyword_; }
    [[nodiscard]] RE::BGSKeyword* keyword_actor_value_to_actor_value() const { return keyword_actor_value_to_actor_value_; }
    [[nodiscard]] RE::BGSKeyword* keyword_add_speed_mult_to_damage_resist() const
    {
      return keyword_add_speed_mult_to_damage_resist_;
    }
    [[nodiscard]] float add_speed_mult_to_damage_resist_mult() const { return add_speed_mult_to_damage_resist_mult_; }
    [[nodiscard]] RE::BGSKeyword* keyword_always_sneak() const { return keyword_always_sneak_; }

    SpecialTechniquesConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    SpecialTechniquesConfig() = default;

private:
    bool enable_;
    RE::BGSKeyword* enable_360_block_;
    RE::BGSKeyword* ignore_apply_spell_and_enchanting_keyword_id_;
    RE::BGSKeyword* no_block_rotate_keyword_;
    RE::BGSKeyword* active_block_keyword_;
    RE::BGSKeyword* stamina_cost_spell_keyword_;
    float normalize_power_attack_mult_pc_;
    float normalize_power_attack_mult_npc_;
    float normalize_normal_attack_mult_pc_;
    float normalize_normal_attack_mult_npc_;
    float normalize_max_regen_delay_health_;
    float normalize_max_regen_delay_magicka_;
    float normalize_max_regen_delay_stamina_;
    bool dynamic_regeneration_health_;
    bool dynamic_regeneration_magicka_;
    bool dynamic_regeneration_stamina_;
    float slow_after_block_hit_mult_;
    float slow_after_block_hit_duration_;
    RE::BGSKeyword* keyword_add_speed_mult_to_damage_resist_;
    RE::BGSKeyword* keyword_actor_value_to_actor_value_;
    float add_speed_mult_to_damage_resist_mult_;
    RE::BGSKeyword* keyword_always_sneak_;
  };

  struct ResistTweaksConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] bool enable_check_resistance() const { return enable_check_resistance_; }
    [[nodiscard]] RE::BGSKeyword* max_resist_keyword() const { return max_resist_keyword_; }
    [[nodiscard]] RE::BGSKeyword* negative_resist_immune_keyword() const { return negative_resist_immune_keyword_; }
    [[nodiscard]] float low() const { return low_; }

    [[nodiscard]] bool no_double_resist_check_magick() const { return no_double_resist_check_magick_; }

    [[nodiscard]] bool no_double_resist_check_poison() const { return no_double_resist_check_poison_; }

    [[nodiscard]] bool no_av_damage_resist_penetration() const { return no_av_damage_resist_penetration_; }

    [[nodiscard]] bool enable_damage_resist_tweak() const { return enable_damage_resist_tweak_; }

    [[nodiscard]] bool no_double_damage_resist_check() const { return no_double_damage_resist_check_; }

    [[nodiscard]] bool enable_resist_penetration() const { return enable_resist_penetration_; }

    [[nodiscard]] bool flat_penetration() const { return flat_penetration_; }

    [[nodiscard]] RE::ActorValue penetration_av() const { return penetration_av_; }

    [[nodiscard]] RE::BGSKeyword* penetration_kw() const { return penetration_kw_; }

    [[nodiscard]] std::int32_t penetration_high() const { return penetration_high_; }

    [[nodiscard]] bool npc_max_resistance() const { return npc_max_resistance_; }

    [[nodiscard]] float resist_weight() const { return resist_weight_; }

    [[nodiscard]] bool ench_get_no_absorb() const { return ench_get_no_absorb_; }

    [[nodiscard]] bool ench_ignore_resistance() const { return ench_ignore_resistance_; }

    [[nodiscard]] bool enable_custom_damage_resist_weight() const { return enable_custom_damage_resist_weight_; }
    [[nodiscard]] float damage_resist_weight() const { return damage_resist_weight_; }
    [[nodiscard]] float damage_resist_low() const { return damage_resist_low_; }
    [[nodiscard]] float damage_resist_high() const { return damage_resist_high_; }

    ResistTweaksConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    ResistTweaksConfig() = default;

private:
    bool enable_;
    bool enable_check_resistance_;
    RE::BGSKeyword* max_resist_keyword_;
    RE::BGSKeyword* negative_resist_immune_keyword_;
    float low_;
    bool no_double_resist_check_magick_;
    bool no_double_resist_check_poison_;
    bool no_av_damage_resist_penetration_;
    bool enable_damage_resist_tweak_;
    bool no_double_damage_resist_check_;
    bool enable_resist_penetration_;
    bool flat_penetration_;
    RE::ActorValue penetration_av_;
    RE::BGSKeyword* penetration_kw_;
    std::int32_t penetration_high_;
    bool npc_max_resistance_;
    float resist_weight_;
    bool enable_custom_damage_resist_weight_;
    float damage_resist_weight_;
    float damage_resist_low_;
    float damage_resist_high_;
    bool ench_get_no_absorb_;
    bool ench_ignore_resistance_;
  };

  struct TimingBlockConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] int32_t parry_stagger_count() const { return parry_stagger_count_; }

    [[nodiscard]] float parry_stagger_count_timer() const { return parry_stagger_count_timer_; }

    [[nodiscard]] float parry_timing() const { return parry_timing_; }

    [[nodiscard]] float block_delay() const { return block_delay_; }

    [[nodiscard]] float block_timing() const { return block_timing_; }

    [[nodiscard]] bool enable_sparks() const { return enable_sparks_; }

    [[nodiscard]] RE::BGSSoundDescriptorForm* parry_sound() const { return parry_sound_; }

    [[nodiscard]] RE::BGSSoundDescriptorForm* block_sound() const { return block_sound_; }

    [[nodiscard]] RE::TESObjectACTI* blank_activator() const { return blank_activator_; }

    [[nodiscard]] RE::BGSKeyword* parry_keyword() const { return parry_keyword_; }

    [[nodiscard]] RE::BGSKeyword* parry_immun_keyword() const { return parry_immun_keyword_; }

    [[nodiscard]] RE::BGSKeyword* parry_timing_keyword() const { return parry_timing_keyword_; }

    [[nodiscard]] RE::BGSKeyword* parry_stagger_count_keyword() const { return parry_stagger_count_keyword_; }

    [[nodiscard]] RE::BGSKeyword* block_keyword() const { return block_keyword_; }

    [[nodiscard]] RE::BGSKeyword* block_immun_keyword() const { return block_immun_keyword_; }

    [[nodiscard]] RE::BGSKeyword* block_timing_keyword() const { return block_timing_keyword_; }

    [[nodiscard]] RE::Explosion* spark() const { return spark_; }

    [[nodiscard]] RE::Explosion* spark_flare() const { return spark_flare_; }

    [[nodiscard]] RE::Explosion* spark_halo() const { return spark_halo_; }

    [[nodiscard]] std::string stagger_event_npc() const { return stagger_event_npc_; }
    [[nodiscard]] float stagger_power_npc() const { return stagger_power_npc_; }
    [[nodiscard]] std::string stagger_event_pc() const { return stagger_event_pc_; }
    [[nodiscard]] float stagger_power_pc() const { return stagger_power_pc_; }

    TimingBlockConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    TimingBlockConfig() = default;

private:
    bool enable_;
    RE::BGSKeyword* parry_keyword_;
    RE::BGSKeyword* parry_immun_keyword_;
    RE::BGSKeyword* parry_timing_keyword_;
    RE::BGSKeyword* parry_stagger_count_keyword_;
    RE::BGSSoundDescriptorForm* parry_sound_;
    int32_t parry_stagger_count_;
    float parry_stagger_count_timer_;
    float parry_timing_;
    float block_delay_;
    RE::BGSKeyword* block_keyword_;
    RE::BGSKeyword* block_immun_keyword_;
    RE::BGSKeyword* block_timing_keyword_;
    RE::BGSSoundDescriptorForm* block_sound_;
    float block_timing_;
    bool enable_sparks_;
    RE::TESObjectACTI* blank_activator_;
    RE::Explosion* spark_;
    RE::Explosion* spark_flare_;
    RE::Explosion* spark_halo_;
    std::string stagger_event_npc_;
    float stagger_power_npc_;
    std::string stagger_event_pc_;
    float stagger_power_pc_;
  };

  struct EquipLoadConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] bool enable_for_npc() const { return enable_for_npc_; }

    [[nodiscard]] RE::SpellItem* low_equip_spell() const { return low_equip_spell_; }

    [[nodiscard]] RE::SpellItem* med_equip_spell() const { return med_equip_spell_; }

    [[nodiscard]] RE::SpellItem* hig_equip_spell() const { return hig_equip_spell_; }

    [[nodiscard]] float equip_weight_mult() const { return equip_weight_mult_; }

    [[nodiscard]] float weapon_weight_mult() const { return weapon_weight_mult_; }

    [[nodiscard]] float armor_weight_mult() const { return armor_weight_mult_; }

    [[nodiscard]] float low_percent() const { return low_percent_; }

    [[nodiscard]] float high_percent() const { return high_percent_; }

    EquipLoadConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    EquipLoadConfig() = default;

private:
    bool enable_;
    bool enable_for_npc_;
    RE::SpellItem* low_equip_spell_;
    RE::SpellItem* med_equip_spell_;
    RE::SpellItem* hig_equip_spell_;
    float equip_weight_mult_;
    float weapon_weight_mult_;
    float armor_weight_mult_;
    float low_percent_;
    float high_percent_;
  };

  struct ParryBashConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] bool enable_parry_bash() const { return enable_parry_bash_; }

    [[nodiscard]] bool enable_bash_block() const { return enable_bash_block_; }

    [[nodiscard]] bool enable_bash_block_with_timing_parry_block() const
    {
      return enable_bash_block_with_timing_parry_block_;
    }

    [[nodiscard]] bool enable_pre_hit_frame() const { return enable_pre_hit_frame_; }

    [[nodiscard]] bool enable_weapon_swing() const { return enable_weapon_swing_; }

    [[nodiscard]] bool enable_precision_weapon_collide() const { return enable_precision_weapon_collide_; }

    [[nodiscard]] bool enable_vanilla_bash_ranged() const { return enable_vanilla_bash_ranged_; }

    [[nodiscard]] float parry_timing() const { return parry_timing_; }

    [[nodiscard]] float pre_hit_frame_penalty() const { return pre_hit_frame_penalty_; }

    [[nodiscard]] RE::BGSKeyword* parry_keyword() const { return parry_keyword_; }

    [[nodiscard]] RE::BGSKeyword* parry_immun_keyword() const { return parry_immun_keyword_; }

    [[nodiscard]] RE::BGSKeyword* parry_timing_keyword() const { return parry_timing_keyword_; }

    [[nodiscard]] RE::BGSSoundDescriptorForm* parry_sound() const { return parry_sound_; }

    [[nodiscard]] RE::BGSKeyword* bash_block_keyword() const { return bash_block_keyword_; }

    [[nodiscard]] std::string stagger_event_npc() const { return stagger_event_npc_; }
    [[nodiscard]] float stagger_power_npc() const { return stagger_power_npc_; }
    [[nodiscard]] std::string stagger_event_pc() const { return stagger_event_pc_; }
    [[nodiscard]] float stagger_power_pc() const { return stagger_power_pc_; }

    ParryBashConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    ParryBashConfig() = default;

private:
    bool enable_;
    bool enable_parry_bash_;
    float parry_timing_;
    float pre_hit_frame_penalty_;
    bool enable_pre_hit_frame_;
    bool enable_weapon_swing_;
    bool enable_precision_weapon_collide_;
    RE::BGSKeyword* parry_keyword_;
    RE::BGSKeyword* parry_immun_keyword_;
    RE::BGSKeyword* parry_timing_keyword_;
    RE::BGSSoundDescriptorForm* parry_sound_;
    bool enable_bash_block_;
    bool enable_bash_block_with_timing_parry_block_;
    RE::BGSKeyword* bash_block_keyword_;
    bool enable_vanilla_bash_ranged_;
    std::string stagger_event_npc_;
    float stagger_power_npc_;
    std::string stagger_event_pc_;
    float stagger_power_pc_;
  };

  struct ItemLimitConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::TESObjectREFR* storage() const { return storage_; }

    [[nodiscard]] RE::BGSListForm* keywords_item() const { return keywords_item_; }

    [[nodiscard]] RE::BGSListForm* keywords_cap_mod() const { return keywords_cap_mod_; }

    [[nodiscard]] RE::BGSListForm* globals() const { return globals_; }

    [[nodiscard]] bool enable_auto_restore() const { return enable_auto_restore_; }

    [[nodiscard]] RE::BGSKeyword* auto_restore_block_keyword() const { return auto_restore_block_keyword_; }

    [[nodiscard]] RE::BGSKeyword* exclusive_restore_keyword() const { return exclusive_restore_keyword_; }

    ItemLimitConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    ItemLimitConfig() = default;

private:
    bool enable_;
    RE::TESObjectREFR* storage_;
    RE::BGSListForm* keywords_item_;
    RE::BGSListForm* keywords_cap_mod_;
    RE::BGSListForm* globals_;
    bool enable_auto_restore_;
    RE::BGSKeyword* auto_restore_block_keyword_;
    RE::BGSKeyword* exclusive_restore_keyword_;
  };

  struct DeathLootConfig final
  {
    [[nodiscard]] float gold_value_mult() const { return gold_value_mult_; }
    [[nodiscard]] float hit_timer() const { return hit_timer_; }
    [[nodiscard]] RE::BGSKeyword* exclusive_keyword() const { return exclusive_keyword_; }
    [[nodiscard]] RE::TESObjectREFR* storage() const { return storage_; }
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] bool auto_loot() const { return auto_loot_; }
    [[nodiscard]] bool loot_arrow() const { return loot_arrow_; }
    [[nodiscard]] bool loot_bosses() const { return loot_bosses_; }
    [[nodiscard]] bool loot_uniques() const { return loot_uniques_; }
    [[nodiscard]] RE::SpellItem* spell_when_loot() const { return spell_when_loot_; }

    DeathLootConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    DeathLootConfig() = default;

private:
    bool enable_;
    float gold_value_mult_;
    float hit_timer_;
    bool auto_loot_;
    bool loot_arrow_;
    bool loot_bosses_;
    bool loot_uniques_;
    RE::SpellItem* spell_when_loot_;
    RE::BGSKeyword* exclusive_keyword_;
    RE::TESObjectREFR* storage_;
  };

  struct SoulLinkConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] bool physic() const { return physic_; }
    [[nodiscard]] bool magick() const { return magick_; }
    [[nodiscard]] RE::ActorValue av() const { return av_; }
    [[nodiscard]] RE::BGSKeyword* summons_split_keyword() const { return summons_split_keyword_; }
    [[nodiscard]] RE::BGSKeyword* exclusive_keyword() const { return exclusive_keyword_; }

    SoulLinkConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    SoulLinkConfig() = default;

private:
    bool enable_;
    bool physic_;
    bool magick_;
    RE::ActorValue av_;
    RE::BGSKeyword* summons_split_keyword_;
    RE::BGSKeyword* exclusive_keyword_;
  };

  struct PotionsDrinkLimitConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] bool enable_one_cooldown() const { return enable_one_cooldown_; }
    [[nodiscard]] bool other_enable() const { return other_enable_; }
    [[nodiscard]] std::int32_t other_cap_base() const { return other_cap_base_; }
    [[nodiscard]] float other_duration_base() const { return other_duration_base_; }
    [[nodiscard]] std::string notify() const { return notify_; }
    [[nodiscard]] RE::BGSKeyword* other_exclusive_keyword() const { return other_exclusive_keyword_; }
    [[nodiscard]] RE::BGSKeyword* other_cap_keyword() const { return other_cap_keyword_; }
    [[nodiscard]] RE::BGSKeyword* other_duration_keyword() const { return other_duration_keyword_; }
    [[nodiscard]] RE::BGSKeyword* no_remove_keyword() const { return no_remove_keyword_; }
    [[nodiscard]] bool other_revert_exclusive() const { return other_revert_exclusive_; }
    [[nodiscard]] bool health_enable() const { return health_enable_; }
    [[nodiscard]] RE::BGSKeyword* health_cap_keyword() const { return health_cap_keyword_; }
    [[nodiscard]] RE::BGSKeyword* health_duration_keyword() const { return health_duration_keyword_; }
    [[nodiscard]] std::int32_t health_cap_base() const { return health_cap_base_; }
    [[nodiscard]] float health_duration_base() const { return health_duration_base_; }
    [[nodiscard]] bool stamina_enable() const { return stamina_enable_; }
    [[nodiscard]] RE::BGSKeyword* stamina_cap_keyword() const { return stamina_cap_keyword_; }
    [[nodiscard]] RE::BGSKeyword* stamina_duration_keyword() const { return stamina_duration_keyword_; }
    [[nodiscard]] std::int32_t stamina_cap_base() const { return stamina_cap_base_; }
    [[nodiscard]] float stamina_duration_base() const { return stamina_duration_base_; }
    [[nodiscard]] bool magicka_enable() const { return magicka_enable_; }
    [[nodiscard]] RE::BGSKeyword* magicka_cap_keyword() const { return magicka_cap_keyword_; }
    [[nodiscard]] RE::BGSKeyword* magicka_duration_keyword() const { return magicka_duration_keyword_; }
    [[nodiscard]] RE::BGSKeyword* health_keyword() const { return health_keyword_; }
    [[nodiscard]] RE::BGSKeyword* stamina_keyword() const { return stamina_keyword_; }
    [[nodiscard]] RE::BGSKeyword* magicka_keyword() const { return magicka_keyword_; }
    [[nodiscard]] std::int32_t magicka_cap_base() const { return magicka_cap_base_; }
    [[nodiscard]] float magicka_duration_base() const { return magicka_duration_base_; }

    PotionsDrinkLimitConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    PotionsDrinkLimitConfig() = default;

private:
    bool enable_;
    bool enable_one_cooldown_;
    bool other_enable_;
    std::int32_t other_cap_base_;
    float other_duration_base_;
    std::string notify_;
    RE::BGSKeyword* other_exclusive_keyword_;
    RE::BGSKeyword* other_cap_keyword_;
    RE::BGSKeyword* other_duration_keyword_;
    RE::BGSKeyword* no_remove_keyword_;
    bool other_revert_exclusive_;
    bool health_enable_;
    std::int32_t health_cap_base_;
    float health_duration_base_;
    RE::BGSKeyword* health_cap_keyword_;
    RE::BGSKeyword* health_duration_keyword_;
    RE::BGSKeyword* health_keyword_;
    bool stamina_enable_;
    std::int32_t stamina_cap_base_;
    float stamina_duration_base_;
    RE::BGSKeyword* stamina_cap_keyword_;
    RE::BGSKeyword* stamina_duration_keyword_;
    RE::BGSKeyword* stamina_keyword_;
    bool magicka_enable_;
    std::int32_t magicka_cap_base_;
    float magicka_duration_base_;
    RE::BGSKeyword* magicka_cap_keyword_;
    RE::BGSKeyword* magicka_duration_keyword_;
    RE::BGSKeyword* magicka_keyword_;

    // RE::TESGlobal* max_potions_;
    // RE::TESGlobal* used_potions_;
  };

  struct UnblockableAttackConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] RE::BGSKeyword* absolute_unblock() const { return absolute_unblock_; }
    [[nodiscard]] RE::BGSKeyword* just_block_unblock() const { return just_block_unblock_; }
    [[nodiscard]] RE::BGSKeyword* timing_block_unblock() const { return timing_block_unblock_; }

    UnblockableAttackConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    UnblockableAttackConfig() = default;

private:
    bool enable_;
    RE::BGSKeyword* absolute_unblock_;
    RE::BGSKeyword* just_block_unblock_;
    RE::BGSKeyword* timing_block_unblock_;
  };

  struct DamageReductionSystemConfig final
  {
    DamageReductionSystemConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    DamageReductionSystemConfig() = default;

public:
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] RE::ActorValue general_damage_reduction() const { return general_damage_reduction_; }
    [[nodiscard]] RE::ActorValue general_damage_reduction_bypass() const { return general_damage_reduction_bypass_; }
    [[nodiscard]] RE::BGSKeyword* damage_reduction_effect() const { return damage_reduction_effect_; }
    [[nodiscard]] RE::BGSKeyword* damage_reduction_bypass_effect() const { return damage_reduction_bypass_effect_; }
    [[nodiscard]] std::string damage_reduction_keyword() const { return damage_reduction_keyword_; }
    [[nodiscard]] std::string damage_reduction_bypass_keyword() const { return damage_reduction_bypass_keyword_; }

private:
    bool enable_;
    RE::ActorValue general_damage_reduction_;
    RE::ActorValue general_damage_reduction_bypass_;
    RE::BGSKeyword* damage_reduction_effect_;
    RE::BGSKeyword* damage_reduction_bypass_effect_;
    std::string damage_reduction_keyword_;
    std::string damage_reduction_bypass_keyword_;
  };

  struct SpeedMultCapConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] float cap_base() const { return cap_base_; }
    [[nodiscard]] RE::BGSKeyword* effect_allow_overcap() const { return effect_allow_overcap_; }
    [[nodiscard]] RE::BGSKeyword* effect_mutate_cap() const { return effect_mutate_cap_; }
    [[nodiscard]] RE::BGSKeyword* exclusive_keyword() const { return exclusive_keyword_; }

    SpeedMultCapConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    SpeedMultCapConfig() = default;

private:
    bool enable_;
    float cap_base_;
    RE::BGSKeyword* effect_allow_overcap_;
    RE::BGSKeyword* effect_mutate_cap_;
    RE::BGSKeyword* exclusive_keyword_;
  };

  struct KiEnergyPowerConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] bool enable_one_cooldown() const { return enable_one_cooldown_; }
    [[nodiscard]] std::int32_t cap_base() const { return cap_base_; }
    [[nodiscard]] float duration_base() const { return duration_base_; }
    [[nodiscard]] std::string notify() const { return notify_; }
    [[nodiscard]] RE::BGSKeyword* cap_keyword() const { return cap_keyword_; }
    [[nodiscard]] RE::BGSKeyword* duration_keyword() const { return duration_keyword_; }
    [[nodiscard]] RE::BGSKeyword* ki_power_keyword() const { return ki_power_keyword_; }
    [[nodiscard]] bool enable_reservation() const { return enable_reservation_; }
    [[nodiscard]] bool enable_reservation_adjust_cost() const { return enable_reservation_adjust_cost_; }
    [[nodiscard]] RE::BGSListForm* ki_reservation_keywords() const { return ki_reservation_keywords_; }
    [[nodiscard]] RE::BGSListForm* ki_reservation_abilities() const { return ki_reservation_abilities_; }
    [[nodiscard]] bool enable_ki_summons() const { return enable_ki_summons_; }
    [[nodiscard]] RE::BGSListForm* ki_summons_list() const { return ki_summons_list_; }
    [[nodiscard]] RE::BGSListForm* ki_reservation_abilities_no_condition() const
    {
      return ki_reservation_abilities_no_condition_;
    }

    KiEnergyPowerConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    KiEnergyPowerConfig() = default;

private:
    bool enable_;
    bool enable_one_cooldown_;
    std::int32_t cap_base_;
    float duration_base_;
    std::string notify_;
    RE::BGSKeyword* cap_keyword_;
    RE::BGSKeyword* duration_keyword_;
    RE::BGSKeyword* ki_power_keyword_;
    bool enable_reservation_;
    bool enable_reservation_adjust_cost_;
    RE::BGSListForm* ki_reservation_keywords_;
    RE::BGSListForm* ki_reservation_abilities_;
    RE::BGSListForm* ki_reservation_abilities_no_condition_;
    bool enable_ki_summons_;
    RE::BGSListForm* ki_summons_list_;
  };

  struct ProjectileBlockConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] bool enable_block() const { return enable_block_; }
    [[nodiscard]] bool enable_timing_block() const { return enable_timing_block_; }
    [[nodiscard]] float block_timing() const { return block_timing_; }
    [[nodiscard]] float base_flat_cost() const { return base_flat_cost_; }
    [[nodiscard]] float base_percent_cost() const { return base_percent_cost_; }
    [[nodiscard]] bool enable_block_magick_for_magick() const { return enable_block_magick_for_magick_; }
    [[nodiscard]] bool enable_arrow() const { return enable_arrow_; }
    [[nodiscard]] bool enable_missile() const { return enable_missile_; }
    [[nodiscard]] bool enable_cone() const { return enable_cone_; }
    [[nodiscard]] bool enable_flame() const { return enable_flame_; }
    [[nodiscard]] bool enable_beam() const { return enable_beam_; }
    [[nodiscard]] bool enable_sparks() const { return enable_sparks_; }
    [[nodiscard]] bool enable_sound() const { return enable_sound_; }
    [[nodiscard]] RE::BGSKeyword* enable_block_keyword() const { return enable_block_keyword_; }
    [[nodiscard]] RE::BGSKeyword* enable_timing_block_keyword() const { return enable_timing_block_keyword_; }
    [[nodiscard]] RE::BGSKeyword* timing_block_timer_keyword() const { return timing_block_timer_keyword_; }
    [[nodiscard]] RE::BGSKeyword* block_cost_keyword() const { return block_cost_keyword_; }
    [[nodiscard]] RE::BGSKeyword* enable_arrow_block_keyword() const { return enable_arrow_block_keyword_; }
    [[nodiscard]] RE::BGSKeyword* enable_magick_block_keyword() const { return enable_magick_block_keyword_; }
    [[nodiscard]] RE::BGSKeyword* enable_arrow_timing_block_keyword() const
    {
      return enable_arrow_timing_block_keyword_;
    }
    [[nodiscard]] RE::BGSKeyword* enable_magick_timing_block_keyword() const
    {
      return enable_magick_timing_block_keyword_;
    }
    [[nodiscard]] RE::BGSKeyword* exclusive_keyword() const { return exclusive_keyword_; }
    [[nodiscard]] RE::BGSKeyword* exclusive_timing_keyword() const { return exclusive_timing_keyword_; }
    [[nodiscard]] RE::BGSSoundDescriptorForm* block_sound() const { return block_sound_; }
    [[nodiscard]] RE::TESObjectACTI* blank_activator() const { return blank_activator_; }
    [[nodiscard]] RE::Explosion* spark() const { return spark_; }
    [[nodiscard]] RE::Explosion* spark_flare() const { return spark_flare_; }
    [[nodiscard]] RE::Explosion* spark_halo() const { return spark_halo_; }
    [[nodiscard]] bool enable_correct_cost_calculate() const { return enable_correct_cost_calculate_; }
    [[nodiscard]] bool enable_add_spell_damage_cost() const { return enable_add_spell_damage_cost_; }
    [[nodiscard]] RE::BGSKeyword* spell_cost_keyword() const { return spell_cost_keyword_; }
    [[nodiscard]] float spell_cost_magicka_mult() const { return spell_cost_magicka_mult_; }
    [[nodiscard]] float spell_damage_novice() const { return spell_damage_novice_; }
    [[nodiscard]] float spell_damage_apprentice() const { return spell_damage_apprentice_; }
    [[nodiscard]] float spell_damage_adept() const { return spell_damage_adept_; }
    [[nodiscard]] float spell_damage_expert() const { return spell_damage_expert_; }
    [[nodiscard]] float spell_damage_master() const { return spell_damage_master_; }
    [[nodiscard]] float spell_cost_base() const { return spell_cost_base_; }
    [[nodiscard]] RE::Explosion* arrow_spark() const { return arrow_spark_; }
    [[nodiscard]] RE::Explosion* arrow_spark_flare() const { return arrow_spark_flare_; }
    [[nodiscard]] RE::Explosion* arrow_spark_halo() const { return arrow_spark_halo_; }
    [[nodiscard]] RE::BGSSoundDescriptorForm* arrow_block_sound() const { return arrow_block_sound_; }

    ProjectileBlockConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    ProjectileBlockConfig() = default;

private:
    bool enable_;
    bool enable_block_;
    bool enable_timing_block_;
    float block_timing_;
    float base_flat_cost_;
    float base_percent_cost_;
    bool enable_block_magick_for_magick_;
    bool enable_arrow_;
    bool enable_missile_;
    bool enable_cone_;
    bool enable_flame_;
    bool enable_beam_;
    bool enable_sparks_;
    bool enable_sound_;
    RE::BGSKeyword* enable_block_keyword_;
    RE::BGSKeyword* enable_timing_block_keyword_;
    RE::BGSKeyword* timing_block_timer_keyword_;         // модификатор тайминга
    RE::BGSKeyword* block_cost_keyword_;                 // мультипликатор стоимости
    RE::BGSKeyword* enable_arrow_block_keyword_;         // только если enable_arrow_ = true
    RE::BGSKeyword* enable_magick_block_keyword_;        // missile + cone + flame + beam, смотря что
                                                         // включено в конфиге
    RE::BGSKeyword* enable_arrow_timing_block_keyword_;  // только если enable_arrow_ = true
    RE::BGSKeyword* enable_magick_timing_block_keyword_; // missile + cone + flame + beam, смотря что
    // включено в конфиге
    RE::BGSKeyword* exclusive_keyword_;        // Если на акторе - его проджектайлы не блочатся, если на
                                               // заклинании то с него тоже не блочатся
    RE::BGSKeyword* exclusive_timing_keyword_; // Если на акторе - его проджектайлы не блочатся таймингово, если
                                               // на заклинании то с него тоже не блочатся таймингово
    RE::BGSSoundDescriptorForm* block_sound_;
    RE::TESObjectACTI* blank_activator_;
    RE::Explosion* spark_;
    RE::Explosion* spark_flare_;
    RE::Explosion* spark_halo_;
    RE::Explosion* arrow_spark_;
    RE::Explosion* arrow_spark_flare_;
    RE::Explosion* arrow_spark_halo_;
    RE::BGSSoundDescriptorForm* arrow_block_sound_;
    bool enable_correct_cost_calculate_;
    RE::BGSKeyword* spell_cost_keyword_;
    float spell_cost_base_;
    bool enable_add_spell_damage_cost_;
    float spell_cost_magicka_mult_;
    float spell_damage_novice_;
    float spell_damage_apprentice_;
    float spell_damage_adept_;
    float spell_damage_expert_;
    float spell_damage_master_;
  };

  struct BoundSpellSwapConfig final
  {

    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] RE::BGSListForm* bound_spells() const { return bound_spells_; }
    [[nodiscard]] RE::BGSListForm* bound_weapons() const { return bound_weapons_; }

    BoundSpellSwapConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    BoundSpellSwapConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* bound_spells_;
    RE::BGSListForm* bound_weapons_;
  };

  struct CastOnProjectileBlockConfig final
  {

    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] bool physical() const { return physical_; }
    [[nodiscard]] bool magick() const { return magick_; }
    [[nodiscard]] RE::BGSListForm* weapon_formlist_spells() const { return weapon_formlist_spells_; }
    [[nodiscard]] RE::BGSListForm* weapon_formlist_needkw() const { return weapon_formlist_needkw_; }
    [[nodiscard]] RE::BGSListForm* magick_formlist_spells() const { return magick_formlist_spells_; }
    [[nodiscard]] RE::BGSListForm* magick_formlist_needkw() const { return magick_formlist_needkw_; }

    CastOnProjectileBlockConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnProjectileBlockConfig() = default;

private:
    bool enable_;
    bool physical_;
    bool magick_;
    RE::BGSListForm* weapon_formlist_spells_;
    RE::BGSListForm* weapon_formlist_needkw_;
    RE::BGSListForm* magick_formlist_spells_;
    RE::BGSListForm* magick_formlist_needkw_;
  };

  struct CastOnProjectileTimingBlockConfig final
  {

    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] bool physical() const { return physical_; }
    [[nodiscard]] bool magick() const { return magick_; }
    [[nodiscard]] RE::BGSListForm* weapon_formlist_spells() const { return weapon_formlist_spells_; }
    [[nodiscard]] RE::BGSListForm* weapon_formlist_needkw() const { return weapon_formlist_needkw_; }
    [[nodiscard]] RE::BGSListForm* magick_formlist_spells() const { return magick_formlist_spells_; }
    [[nodiscard]] RE::BGSListForm* magick_formlist_needkw() const { return magick_formlist_needkw_; }

    CastOnProjectileTimingBlockConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnProjectileTimingBlockConfig() = default;

private:
    bool enable_;
    bool physical_;
    bool magick_;
    RE::BGSListForm* weapon_formlist_spells_;
    RE::BGSListForm* weapon_formlist_needkw_;
    RE::BGSListForm* magick_formlist_spells_;
    RE::BGSListForm* magick_formlist_needkw_;
  };

  struct JoshuaSawyerArmorSystemConfig final
  {
private:
    bool enable_;
    RE::BGSKeyword* reduction_keyword_;
    RE::BGSKeyword* penetration_keyword_;
    std::map<RE::ActorValue, RE::ActorValue> reduction_map_;
    std::map<RE::ActorValue, RE::ActorValue> penetration_map_;
  };

  struct MiscFixesConfig final
  {
    [[nodiscard]] bool regeneration_fix() const { return regeneration_fix_; }
    [[nodiscard]] bool equip_bound_fix() const { return equip_bound_fix_; }
    [[nodiscard]] bool disable_sprint_cost_in_combat() const { return disable_sprint_cost_in_combat_; }
    [[nodiscard]] bool disable_over_encumbered() const { return disable_over_encumbered_; }
    [[nodiscard]] bool negative_speed_mult_fix() const { return negative_speed_mult_fix_; }
    [[nodiscard]] bool no_sneak_requirement_for_sneak_attack() const { return no_sneak_requirement_for_sneak_attack_; }
    [[nodiscard]] bool equip_spell_with_two_handed() const { return equip_spell_with_two_handed_; }
    [[nodiscard]] bool cast_ability_on_equip() const { return cast_ability_on_equip_; }
    [[nodiscard]] bool cast_ability_on_equip_instant() const { return cast_ability_on_equip_instant_; }
    [[nodiscard]] bool cast_ability_magicka_cost() const { return cast_ability_magicka_cost_; }
    [[nodiscard]] bool cast_shout_on_equip() const { return cast_shout_on_equip_; }
    [[nodiscard]] bool display_key_name() const { return display_key_name_; }
    [[nodiscard]] bool enable_instant_restore_stamina_npc() const { return enable_instant_restore_stamina_npc_; }
    [[nodiscard]] bool enable_unlimited_merchant_gold() const { return enable_unlimited_merchant_gold_; }
    [[nodiscard]] bool enable_enchants_ignore_absorb() const { return enable_enchants_ignore_absorb_; }
    [[nodiscard]] bool enable_npc_always_spend_stamina_on_attack() const
    {
      return enable_npc_always_spend_stamina_on_attack_;
    }
    [[nodiscard]] bool enable_blade_and_blunt_true_hud_special_bar() const
    {
      return enable_blade_and_blunt_true_hud_special_bar_;
    }

    MiscFixesConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    MiscFixesConfig() = default;

private:
    bool regeneration_fix_;
    bool equip_bound_fix_;
    bool disable_sprint_cost_in_combat_;
    bool disable_over_encumbered_;
    bool negative_speed_mult_fix_;
    bool no_sneak_requirement_for_sneak_attack_;
    bool equip_spell_with_two_handed_;
    bool cast_ability_on_equip_;
    bool cast_ability_magicka_cost_;
    bool cast_ability_on_equip_instant_;
    bool cast_shout_on_equip_;
    bool display_key_name_;
    bool enable_instant_restore_stamina_npc_;
    bool enable_unlimited_merchant_gold_;
    bool enable_blade_and_blunt_true_hud_special_bar_;
    bool enable_enchants_ignore_absorb_;
    bool enable_npc_always_spend_stamina_on_attack_;
  };

  struct FollowersSummonsApplySpellConfig final
  {

    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] RE::BGSListForm* keywords_to_player_from_summons() const { return keywords_to_player_from_summons_; }
    [[nodiscard]] RE::BGSListForm* keywords_to_player_from_followers() const
    {
      return keywords_to_player_from_followers_;
    }
    [[nodiscard]] RE::BGSListForm* spells_to_player_from_summons() const { return spells_to_player_from_summons_; }
    [[nodiscard]] RE::BGSListForm* spells_to_player_from_followers() const { return spells_to_player_from_followers_; }
    [[nodiscard]] RE::BGSListForm* keywords_from_player_to_summons() const { return keywords_from_player_to_summons_; }
    [[nodiscard]] RE::BGSListForm* keywords_from_player_to_followers() const
    {
      return keywords_from_player_to_followers_;
    }
    [[nodiscard]] RE::BGSListForm* spells_from_player_to_summons() const { return spells_from_player_to_summons_; }
    [[nodiscard]] RE::BGSListForm* spells_from_player_to_followers() const { return spells_from_player_to_followers_; }

    FollowersSummonsApplySpellConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    FollowersSummonsApplySpellConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* keywords_to_player_from_summons_;
    RE::BGSListForm* keywords_to_player_from_followers_;
    RE::BGSListForm* spells_to_player_from_summons_;
    RE::BGSListForm* spells_to_player_from_followers_;
    RE::BGSListForm* keywords_from_player_to_summons_;
    RE::BGSListForm* keywords_from_player_to_followers_;
    RE::BGSListForm* spells_from_player_to_summons_;
    RE::BGSListForm* spells_from_player_to_followers_;
  };

  struct PoisonReworkConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] std::string notify() const { return notify_; }
    [[nodiscard]] std::int32_t cap_base() const { return cap_base_; }
    [[nodiscard]] float duration_base() const { return duration_base_; }
    [[nodiscard]] bool revert_exclusive() const { return revert_exclusive_; }
    [[nodiscard]] bool apply_on_both_weapon() const { return apply_on_both_weapon_; }
    [[nodiscard]] RE::BGSKeyword* cap_keyword() const { return cap_keyword_; }
    [[nodiscard]] RE::BGSKeyword* duration_keyword() const { return duration_keyword_; }
    [[nodiscard]] RE::BGSKeyword* exclusive_keyword() const { return exclusive_keyword_; }
    [[nodiscard]] RE::BGSKeyword* no_remove_keyword() const { return no_remove_keyword_; }

    PoisonReworkConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    PoisonReworkConfig() = default;

private:
    bool enable_;
    std::string notify_;
    std::int32_t cap_base_;
    float duration_base_;
    bool revert_exclusive_;
    bool apply_on_both_weapon_;
    RE::BGSKeyword* cap_keyword_;
    RE::BGSKeyword* duration_keyword_;
    RE::BGSKeyword* exclusive_keyword_;
    RE::BGSKeyword* no_remove_keyword_;
  };

  struct MagickToStaminaConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    MagickToStaminaConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    MagickToStaminaConfig() = default;

private:
    bool enable_;
  };

  struct DeclutteredLootConfig final
  {

    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] std::uint32_t switch_key() const { return switch_key_; }
    [[nodiscard]] float hold_switch_key() const { return hold_switch_key_; }
    [[nodiscard]] std::uint32_t highlight_key() const { return highlight_key_; }
    [[nodiscard]] float hold_highlight_key() const { return hold_highlight_key_; }
    [[nodiscard]] float highlight_duration() const { return highlight_duration_; }
    [[nodiscard]] bool hide_food() const { return hide_food_; }
    [[nodiscard]] bool hide_misc() const { return hide_misc_; }
    [[nodiscard]] bool hide_ingredient() const { return hide_ingredient_; }
    [[nodiscard]] bool enable_gold_weight_threshold() const { return enable_gold_weight_threshold_; }
    [[nodiscard]] float gold_weight_threshold() const { return gold_weight_threshold_; }
    [[nodiscard]] bool enable_gold_threshold() const { return enable_gold_threshold_; }
    [[nodiscard]] std::int32_t gold_threshold() const { return gold_threshold_; }
    [[nodiscard]] RE::BGSKeyword* exclusive_keyword() const { return exclusive_keyword_; }
    [[nodiscard]] RE::TESEffectShader* shader() const { return shader_; }
    [[nodiscard]] bool enable_sound() const { return enable_sound_; }
    [[nodiscard]] RE::BGSSoundDescriptorForm* highlight_activation_sound() const { return highlight_activation_sound_; }

    DeclutteredLootConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    DeclutteredLootConfig() = default;

private:
    bool enable_;
    std::uint32_t switch_key_;
    float hold_switch_key_;
    std::uint32_t highlight_key_;
    float hold_highlight_key_;
    float highlight_duration_;
    bool hide_food_;
    bool hide_misc_;
    bool hide_ingredient_;
    bool enable_gold_weight_threshold_;
    float gold_weight_threshold_;
    bool enable_gold_threshold_;
    std::int32_t gold_threshold_;
    RE::BGSKeyword* exclusive_keyword_;
    RE::TESEffectShader* shader_;
    bool enable_sound_;
    RE::BGSSoundDescriptorForm* highlight_activation_sound_;
  };

  struct SlowTimeConfig final
  {

    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] bool enable_on_block() const { return enable_on_block_; }
    [[nodiscard]] bool enable_on_timing_block() const { return enable_on_timing_block_; }
    [[nodiscard]] bool enable_on_parry_timing_block() const { return enable_on_parry_timing_block_; }
    [[nodiscard]] bool enable_on_parry_bash() const { return enable_on_parry_bash_; }
    [[nodiscard]] bool enable_on_kill() const { return enable_on_kill_; }
    [[nodiscard]] bool enable_on_projectile_block() const { return enable_on_projectile_block_; }
    [[nodiscard]] bool enable_on_projectile_timing_block() const { return enable_on_projectile_timing_block_; }
    [[nodiscard]] bool enable_on_weapon_crit() const { return enable_on_weapon_crit_; }
    [[nodiscard]] bool enable_on_magick_crit() const { return enable_on_magick_crit_; }
    [[nodiscard]] bool enable_on_weapon_vanilla_crit() const { return enable_on_weapon_vanilla_crit_; }
    [[nodiscard]] bool enable_on_sneak_attack() const { return enable_on_sneak_attack_; }
    [[nodiscard]] float slow_duration() const { return slow_duration_; }
    [[nodiscard]] float slow_power() const { return slow_power_; }

    SlowTimeConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    SlowTimeConfig() = default;

private:
    bool enable_;
    bool enable_on_block_;
    bool enable_on_timing_block_;
    bool enable_on_parry_timing_block_;
    bool enable_on_parry_bash_;
    bool enable_on_kill_;
    bool enable_on_projectile_block_;
    bool enable_on_projectile_timing_block_;
    bool enable_on_weapon_crit_;
    bool enable_on_magick_crit_;
    bool enable_on_weapon_vanilla_crit_;
    bool enable_on_sneak_attack_;
    float slow_duration_;
    float slow_power_;
  };

  struct CastOnGetHitConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* formlist_melee_spells() const { return formlist_melee_spells_; }

    [[nodiscard]] RE::BGSListForm* formlist_melee_needkw() const { return formlist_melee_needkw_; }
    [[nodiscard]] RE::BGSListForm* formlist_bash_spells() const { return formlist_bash_spells_; }
    [[nodiscard]] RE::BGSListForm* formlist_bash_needkw() const { return formlist_bash_needkw_; }
    [[nodiscard]] RE::BGSListForm* formlist_ranged_spells() const { return formlist_ranged_spells_; }
    [[nodiscard]] RE::BGSListForm* formlist_ranged_needkw() const { return formlist_ranged_needkw_; }

    CastOnGetHitConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnGetHitConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* formlist_melee_spells_;
    RE::BGSListForm* formlist_melee_needkw_;
    RE::BGSListForm* formlist_bash_spells_;
    RE::BGSListForm* formlist_bash_needkw_;
    RE::BGSListForm* formlist_ranged_spells_;
    RE::BGSListForm* formlist_ranged_needkw_;
  };

  struct CastOnMagicProjectileGetHitConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* formlist_spells() const { return formlist_spells_; }

    [[nodiscard]] RE::BGSListForm* formlist_needkw() const { return formlist_needkw_; }

    CastOnMagicProjectileGetHitConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnMagicProjectileGetHitConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* formlist_spells_;
    RE::BGSListForm* formlist_needkw_;
  };

  struct ComboSeriesConfig final
  {

    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] RE::BGSListForm* list_keywords_weapon() const { return list_keywords_weapon_; }
    [[nodiscard]] RE::BGSListForm* list_keywords_point() const { return list_keywords_point_; }
    [[nodiscard]] RE::BGSKeyword* keyword_no_source_point() const { return keyword_no_source_point_; }
    [[nodiscard]] RE::BGSKeyword* keyword_magic_projectile_point() const { return keyword_magic_projectile_point_; }
    [[nodiscard]] RE::BGSKeyword* keyword_magic_projectile_dual_cast_point() const
    {
      return keyword_magic_projectile_dual_cast_point_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_combo_point_timer() const { return keyword_combo_point_timer_; }
    [[nodiscard]] RE::BGSKeyword* keyword_combo_points() const { return keyword_combo_points_; }
    [[nodiscard]] RE::BGSListForm* list_cast_keywords() const { return list_cast_keywords_; }
    [[nodiscard]] RE::BGSListForm* list_cast_spells() const { return list_cast_spells_; }

    ComboSeriesConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    ComboSeriesConfig() = default;
  private:
    bool enable_;
    RE::BGSListForm* list_keywords_weapon_;
    RE::BGSListForm* list_keywords_point_;
    RE::BGSListForm* list_cast_keywords_;
    RE::BGSListForm* list_cast_spells_;
    RE::BGSKeyword* keyword_no_source_point_;
    RE::BGSKeyword* keyword_magic_projectile_point_;
    RE::BGSKeyword* keyword_magic_projectile_dual_cast_point_;
    RE::BGSKeyword* keyword_combo_point_timer_;
    RE::BGSKeyword* keyword_combo_points_;
  };

  struct AbsorbShieldConfig final
  {
public:
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] float absolute_max_shield_value() const { return absolute_max_shield_value_; }
    [[nodiscard]] float base_shield_value() const { return base_shield_value_; }
    [[nodiscard]] float base_mult_shield_value_from_max_health() const
    {
      return base_mult_shield_value_from_max_health_;
    }
    [[nodiscard]] float base_mult_shield_value_from_max_stamina() const
    {
      return base_mult_shield_value_from_max_stamina_;
    }
    [[nodiscard]] float base_mult_shield_value_from_max_magicka() const
    {
      return base_mult_shield_value_from_max_magicka_;
    }
    [[nodiscard]] float base_mult_shield_value_from_armor() const { return base_mult_shield_value_from_armor_; }
    [[nodiscard]] float absolute_max_shield_value_drain() const { return absolute_max_shield_value_drain_; }
    [[nodiscard]] float base_drain_shield_value_percent() const { return base_drain_shield_value_percent_; }
    [[nodiscard]] float base_drain_shield_value_flat() const { return base_drain_shield_value_flat_; }
    [[nodiscard]] float base_drain_shield_value_current_percent() const
    {
      return base_drain_shield_value_current_percent_;
    }
    [[nodiscard]] float base_shield_value_gettin_weapon_damage() const
    {
      return base_shield_value_gettin_weapon_damage_;
    }
    [[nodiscard]] float base_shield_value_gettin_magick_damage() const
    {
      return base_shield_value_gettin_magick_damage_;
    }
    [[nodiscard]] float base_from_cast_cost_flat() const { return base_from_cast_cost_flat_; }
    [[nodiscard]] float base_from_cast_cost_percent() const { return base_from_cast_cost_percent_; }
    [[nodiscard]] float absolute_max_shield_value_drain_threshold() const
    {
      return absolute_max_shield_value_drain_threshold_;
    }
    [[nodiscard]] float base_shield_value_drain_threshold_flat() const
    {
      return base_shield_value_drain_threshold_flat_;
    }
    [[nodiscard]] float base_shield_value_drain_threshold_percent() const
    {
      return base_shield_value_drain_threshold_percent_;
    }
    [[nodiscard]] float base_mult_shield_value_drain_threshold_from_max_health() const
    {
      return base_mult_shield_value_drain_threshold_from_max_health_;
    }
    [[nodiscard]] float base_mult_shield_value_drain_threshold_from_max_stamina() const
    {
      return base_mult_shield_value_drain_threshold_from_max_stamina_;
    }
    [[nodiscard]] float base_mult_shield_value_drain_threshold_from_max_magicka() const
    {
      return base_mult_shield_value_drain_threshold_from_max_magicka_;
    }
    [[nodiscard]] float base_mult_shield_value_drain_threshold_from_armor() const
    {
      return base_mult_shield_value_drain_threshold_from_armor_;
    }
    [[nodiscard]] float absolute_max_shield_value_degen_delay() const { return absolute_max_shield_value_degen_delay_; }
    [[nodiscard]] float base_shield_value_degen_delay_after_weapon_hit_flat() const
    {
      return base_shield_value_degen_delay_after_weapon_hit_flat_;
    }
    [[nodiscard]] float base_shield_value_degen_delay_after_magick_hit_flat() const
    {
      return base_shield_value_degen_delay_after_magick_hit_flat_;
    }
    [[nodiscard]] float base_shield_value_degen_delay_after_from_cast_cost_hit_flat() const
    {
      return base_shield_value_degen_delay_after_from_cast_cost_hit_flat_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_degen_delay_after_weapon_hit_enable() const
    {
      return keyword_shield_value_degen_delay_after_weapon_hit_enable_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_degen_delay_after_magick_hit_enable() const
    {
      return keyword_shield_value_degen_delay_after_magick_hit_enable_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_degen_delay_after_from_cast_cost_hit_enable() const
    {
      return keyword_shield_value_degen_delay_after_from_cast_cost_hit_enable_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_degen_delay_after_weapon_hit_flat() const
    {
      return keyword_shield_value_degen_delay_after_weapon_hit_flat_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_degen_delay_after_weapon_hit_mult() const
    {
      return keyword_shield_value_degen_delay_after_weapon_hit_mult_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_degen_delay_after_magick_hit_flat() const
    {
      return keyword_shield_value_degen_delay_after_magick_hit_flat_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_degen_delay_after_magick_hit_mult() const
    {
      return keyword_shield_value_degen_delay_after_magick_hit_mult_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_degen_delay_after_from_cast_cost_hit_flat() const
    {
      return keyword_shield_value_degen_delay_after_from_cast_cost_hit_flat_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_degen_delay_after_from_cast_cost_hit_mult() const
    {
      return keyword_shield_value_degen_delay_after_from_cast_cost_hit_mult_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_drain_threshold_enable() const
    {
      return keyword_shield_value_drain_threshold_enable_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_drain_threshold_flat() const
    {
      return keyword_shield_value_drain_threshold_flat_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_drain_threshold_percent() const
    {
      return keyword_shield_value_drain_threshold_percent_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_drain_threshold_mult() const
    {
      return keyword_shield_value_drain_threshold_mult_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_mult_shield_value_drain_threshold_from_max_health() const
    {
      return keyword_mult_shield_value_drain_threshold_from_max_health_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_mult_shield_value_drain_threshold_from_max_stamina() const
    {
      return keyword_mult_shield_value_drain_threshold_from_max_stamina_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_mult_shield_value_drain_threshold_from_max_magicka() const
    {
      return keyword_mult_shield_value_drain_threshold_from_max_magicka_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_mult_shield_value_drain_threshold_from_armor() const
    {
      return keyword_mult_shield_value_drain_threshold_from_armor_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_from_cast_cost_enable() const
    {
      return keyword_shield_value_from_cast_cost_enable_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_from_cast_cost_flat() const { return keyword_from_cast_cost_flat_; }
    [[nodiscard]] RE::BGSKeyword* keyword_from_cast_cost_percent() const { return keyword_from_cast_cost_percent_; }
    [[nodiscard]] RE::BGSKeyword* keyword_from_cast_cost_mult() const { return keyword_from_cast_cost_mult_; }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_enable_magick() const { return keyword_shield_enable_magick_; }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_enable_weapon() const { return keyword_shield_enable_weapon_; }
    [[nodiscard]] RE::BGSKeyword* keyword_mult_shield_value_from_max_health() const
    {
      return keyword_mult_shield_value_from_max_health_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_mult_shield_value_from_max_stamina() const
    {
      return keyword_mult_shield_value_from_max_stamina_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_mult_shield_value_from_max_magicka() const
    {
      return keyword_mult_shield_value_from_max_magicka_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_mult_shield_value_from_armor() const
    {
      return keyword_mult_shield_value_from_armor_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_flat() const { return keyword_shield_value_flat_; }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_mult() const { return keyword_shield_value_mult_; }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_drain_mult() const { return keyword_shield_value_drain_mult_; }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_drain_flat() const { return keyword_shield_value_drain_flat_; }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_drain_percent() const
    {
      return keyword_shield_value_drain_percent_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_drain_current_percent() const
    {
      return keyword_shield_value_drain_current_percent_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_gettin_weapon_damage() const
    {
      return keyword_shield_value_gettin_weapon_damage_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_gettin_magick_damage() const
    {
      return keyword_shield_value_gettin_magick_damage_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_magic_inject() const
    {
      return keyword_shield_value_magic_inject_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_shield_value_weapon_inject() const
    {
      return keyword_shield_value_weapon_inject_;
    }
    [[nodiscard]] RE::BGSKeyword* keyword_actor_ignore_shield() const { return keyword_actor_ignore_shield_; }
    [[nodiscard]] RE::BGSKeyword* keyword_form_ignore_shield() const { return keyword_form_ignore_shield_; }
    [[nodiscard]] RE::BGSKeyword* keyword_damage_to_shield_mult() const { return keyword_damage_to_shield_mult_; }
    [[nodiscard]] bool enable_sound() const { return enable_sound_; }
    [[nodiscard]] float sound_cooldown() const { return sound_cooldown_; }
    [[nodiscard]] RE::BGSSoundDescriptorForm* hit_sound() const { return hit_sound_; }
    [[nodiscard]] bool enable_effect() const { return enable_effect_; }
    [[nodiscard]] float effect_cooldown() const { return effect_cooldown_; }
    [[nodiscard]] RE::Explosion* hit_effect() const { return hit_effect_; }

    AbsorbShieldConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    AbsorbShieldConfig() = default;

private:
    bool enable_;
    float absolute_max_shield_value_;
    float base_shield_value_;
    float base_mult_shield_value_from_max_health_;
    float base_mult_shield_value_from_max_stamina_;
    float base_mult_shield_value_from_max_magicka_;
    float base_mult_shield_value_from_armor_;
    float absolute_max_shield_value_drain_;
    float base_drain_shield_value_percent_;
    float base_drain_shield_value_flat_;
    float base_drain_shield_value_current_percent_;
    float base_shield_value_gettin_weapon_damage_;
    float base_shield_value_gettin_magick_damage_;
    float base_from_cast_cost_flat_;
    float base_from_cast_cost_percent_;
    float absolute_max_shield_value_drain_threshold_;
    float base_shield_value_drain_threshold_flat_;
    float base_shield_value_drain_threshold_percent_;
    float base_mult_shield_value_drain_threshold_from_max_health_;
    float base_mult_shield_value_drain_threshold_from_max_stamina_;
    float base_mult_shield_value_drain_threshold_from_max_magicka_;
    float base_mult_shield_value_drain_threshold_from_armor_;
    float absolute_max_shield_value_degen_delay_;
    float base_shield_value_degen_delay_after_weapon_hit_flat_;
    float base_shield_value_degen_delay_after_magick_hit_flat_;
    float base_shield_value_degen_delay_after_from_cast_cost_hit_flat_;

    RE::BGSKeyword* keyword_shield_value_degen_delay_after_weapon_hit_enable_;
    RE::BGSKeyword* keyword_shield_value_degen_delay_after_magick_hit_enable_;
    RE::BGSKeyword* keyword_shield_value_degen_delay_after_from_cast_cost_hit_enable_;
    RE::BGSKeyword* keyword_shield_value_degen_delay_after_weapon_hit_flat_;
    RE::BGSKeyword* keyword_shield_value_degen_delay_after_weapon_hit_mult_;
    RE::BGSKeyword* keyword_shield_value_degen_delay_after_magick_hit_flat_;
    RE::BGSKeyword* keyword_shield_value_degen_delay_after_magick_hit_mult_;
    RE::BGSKeyword* keyword_shield_value_degen_delay_after_from_cast_cost_hit_flat_;
    RE::BGSKeyword* keyword_shield_value_degen_delay_after_from_cast_cost_hit_mult_;

    RE::BGSKeyword* keyword_shield_value_drain_threshold_enable_;
    RE::BGSKeyword* keyword_shield_value_drain_threshold_flat_;
    RE::BGSKeyword* keyword_shield_value_drain_threshold_percent_;
    RE::BGSKeyword* keyword_shield_value_drain_threshold_mult_;
    RE::BGSKeyword* keyword_mult_shield_value_drain_threshold_from_max_health_;
    RE::BGSKeyword* keyword_mult_shield_value_drain_threshold_from_max_stamina_;
    RE::BGSKeyword* keyword_mult_shield_value_drain_threshold_from_max_magicka_;
    RE::BGSKeyword* keyword_mult_shield_value_drain_threshold_from_armor_;

    RE::BGSKeyword* keyword_shield_value_from_cast_cost_enable_;
    RE::BGSKeyword* keyword_from_cast_cost_flat_;
    RE::BGSKeyword* keyword_from_cast_cost_percent_;
    RE::BGSKeyword* keyword_from_cast_cost_mult_;

    RE::BGSKeyword* keyword_shield_enable_magick_;
    RE::BGSKeyword* keyword_shield_enable_weapon_;
    RE::BGSKeyword* keyword_mult_shield_value_from_max_health_;
    RE::BGSKeyword* keyword_mult_shield_value_from_max_stamina_;
    RE::BGSKeyword* keyword_mult_shield_value_from_max_magicka_;
    RE::BGSKeyword* keyword_mult_shield_value_from_armor_;
    RE::BGSKeyword* keyword_shield_value_flat_;
    RE::BGSKeyword* keyword_shield_value_mult_;
    RE::BGSKeyword* keyword_shield_value_drain_mult_;
    RE::BGSKeyword* keyword_shield_value_drain_flat_;
    RE::BGSKeyword* keyword_shield_value_drain_percent_;
    RE::BGSKeyword* keyword_shield_value_drain_current_percent_;
    RE::BGSKeyword* keyword_shield_value_gettin_weapon_damage_;
    RE::BGSKeyword* keyword_shield_value_gettin_magick_damage_;

    RE::BGSKeyword* keyword_shield_value_magic_inject_;
    RE::BGSKeyword* keyword_shield_value_weapon_inject_;

    RE::BGSKeyword* keyword_actor_ignore_shield_;
    RE::BGSKeyword* keyword_form_ignore_shield_;

    RE::BGSKeyword* keyword_damage_to_shield_mult_;

    bool enable_sound_;
    float sound_cooldown_;
    RE::BGSSoundDescriptorForm* hit_sound_;

    bool enable_effect_;
    float effect_cooldown_;
    RE::Explosion* hit_effect_;
  };

  struct StaggerSystemConfig final
  {

    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] RE::BGSKeyword* poise_recovery_time() const { return poise_recovery_time_; }
    [[nodiscard]] RE::BGSKeyword* poise_recovery_actions() const { return poise_recovery_actions_; }
    [[nodiscard]] RE::BGSKeyword* poise_value() const { return poise_value_; }
    [[nodiscard]] RE::BGSKeyword* poise_small() const { return poise_small_; }
    [[nodiscard]] RE::BGSKeyword* poise_medium() const { return poise_medium_; }
    [[nodiscard]] RE::BGSKeyword* poise_large() const { return poise_large_; }
    [[nodiscard]] RE::BGSKeyword* poise_largest() const { return poise_largest_; }
    [[nodiscard]] RE::BGSKeyword* poise_damage_cooldown() const { return poise_damage_cooldown_; }
    [[nodiscard]] RE::BGSKeyword* poise_damage_immunity() const { return poise_damage_immunity_; }
    [[nodiscard]] RE::BGSKeyword* poise_regeneration() const { return poise_regeneration_; }
    [[nodiscard]] RE::BGSListForm* list_small_spells_magick() const { return list_small_spells_magick_; }
    [[nodiscard]] RE::BGSListForm* list_small_spells_melee() const { return list_small_spells_melee_; }
    [[nodiscard]] RE::BGSListForm* list_small_spells_ranged() const { return list_small_spells_ranged_; }
    [[nodiscard]] RE::BGSListForm* list_small_keywords_magick() const { return list_small_keywords_magick_; }
    [[nodiscard]] RE::BGSListForm* list_small_keywords_melee() const { return list_small_keywords_melee_; }
    [[nodiscard]] RE::BGSListForm* list_small_keywords_ranged() const { return list_small_keywords_ranged_; }
    [[nodiscard]] RE::BGSListForm* list_medium_spells_magick() const { return list_medium_spells_magick_; }
    [[nodiscard]] RE::BGSListForm* list_medium_spells_melee() const { return list_medium_spells_melee_; }
    [[nodiscard]] RE::BGSListForm* list_medium_spells_ranged() const { return list_medium_spells_ranged_; }
    [[nodiscard]] RE::BGSListForm* list_medium_keywords_magick() const { return list_medium_keywords_magick_; }
    [[nodiscard]] RE::BGSListForm* list_medium_keywords_melee() const { return list_medium_keywords_melee_; }
    [[nodiscard]] RE::BGSListForm* list_medium_keywords_ranged() const { return list_medium_keywords_ranged_; }
    [[nodiscard]] RE::BGSListForm* list_large_spells_magick() const { return list_large_spells_magick_; }
    [[nodiscard]] RE::BGSListForm* list_large_spells_melee() const { return list_large_spells_melee_; }
    [[nodiscard]] RE::BGSListForm* list_large_spells_ranged() const { return list_large_spells_ranged_; }
    [[nodiscard]] RE::BGSListForm* list_large_keywords_magick() const { return list_large_keywords_magick_; }
    [[nodiscard]] RE::BGSListForm* list_large_keywords_melee() const { return list_large_keywords_melee_; }
    [[nodiscard]] RE::BGSListForm* list_large_keywords_ranged() const { return list_large_keywords_ranged_; }
    [[nodiscard]] RE::BGSListForm* list_largest_spells_magick() const { return list_largest_spells_magick_; }
    [[nodiscard]] RE::BGSListForm* list_largest_spells_melee() const { return list_largest_spells_melee_; }
    [[nodiscard]] RE::BGSListForm* list_largest_spells_ranged() const { return list_largest_spells_ranged_; }
    [[nodiscard]] RE::BGSListForm* list_largest_keywords_magick() const { return list_largest_keywords_magick_; }
    [[nodiscard]] RE::BGSListForm* list_largest_keywords_melee() const { return list_largest_keywords_melee_; }
    [[nodiscard]] RE::BGSListForm* list_largest_keywords_ranged() const { return list_largest_keywords_ranged_; }

    StaggerSystemConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    StaggerSystemConfig() = default;

private:
    bool enable_;

    RE::BGSKeyword* poise_recovery_time_;
    RE::BGSKeyword* poise_recovery_actions_;
    RE::BGSKeyword* poise_value_;
    RE::BGSKeyword* poise_small_;
    RE::BGSKeyword* poise_medium_;
    RE::BGSKeyword* poise_large_;
    RE::BGSKeyword* poise_largest_;
    RE::BGSKeyword* poise_damage_cooldown_;
    RE::BGSKeyword* poise_damage_immunity_;
    RE::BGSKeyword* poise_regeneration_;

    RE::BGSListForm* list_small_spells_magick_;
    RE::BGSListForm* list_small_spells_melee_;
    RE::BGSListForm* list_small_spells_ranged_;
    RE::BGSListForm* list_small_keywords_magick_;
    RE::BGSListForm* list_small_keywords_melee_;
    RE::BGSListForm* list_small_keywords_ranged_;

    RE::BGSListForm* list_medium_spells_magick_;
    RE::BGSListForm* list_medium_spells_melee_;
    RE::BGSListForm* list_medium_spells_ranged_;
    RE::BGSListForm* list_medium_keywords_magick_;
    RE::BGSListForm* list_medium_keywords_melee_;
    RE::BGSListForm* list_medium_keywords_ranged_;

    RE::BGSListForm* list_large_spells_magick_;
    RE::BGSListForm* list_large_spells_melee_;
    RE::BGSListForm* list_large_spells_ranged_;
    RE::BGSListForm* list_large_keywords_magick_;
    RE::BGSListForm* list_large_keywords_melee_;
    RE::BGSListForm* list_large_keywords_ranged_;

    RE::BGSListForm* list_largest_spells_magick_;
    RE::BGSListForm* list_largest_spells_melee_;
    RE::BGSListForm* list_largest_spells_ranged_;
    RE::BGSListForm* list_largest_keywords_magick_;
    RE::BGSListForm* list_largest_keywords_melee_;
    RE::BGSListForm* list_largest_keywords_ranged_;
  };

  private:
  Config();

  // general
  std::string_view mod_name_;
  RE::BGSKeyword* cast_on_is_cost_;
  RE::BGSKeyword* cast_on_caster_swap_;
  RE::BGSKeyword* restrict_equip_shield_;
  RE::BGSKeyword* restrict_equip_offhand_;
  float start_attack_duration_;
  float volume_override_;

  // magic shield
  MagicShieldConfig magic_shield_;

  StaggerSystemConfig stagger_system_;

  AbsorbShieldConfig absorb_shield_;

  // stamina shield
  StaminaShieldConfig stamina_shield_;

  // petrified blood
  PetrifiedBloodConfig petrified_blood_;

  // cheat death
  CheatDeathConfig cheat_death_;
  CastOnMagicBlockerConfig cast_on_magic_blocker_;

  // vampirism
  VampirismConfig vampirism_;

  // magic vampirism
  MagicVampirismConfig magic_vampirism_;

  // FenixSpeedCasting addon
  // speed casting
  SpeedCastingConfig speed_casting_;

  // weapon crit
  WeaponCritConfig weapon_crit_;
  ClairvoyanceConfig clairvoyance_;

  // magic crit
  MagickCritConfig magick_crit_;

  // magic blocker
  MagicBlockerConfig magic_blocker_config_;

  // cast on crit
  CastOnCritConfig cast_on_crit_;

  // cast on hit
  CastOnHitConfig cast_on_hit_;

  // cast on block
  CastOnBlockConfig cast_on_block_;

  // cast on block parry
  CastOnBlockParryConfig cast_on_block_parry_;

  // cast on timing block
  CastOnTimingBlockConfig cast_on_timing_block_;

  // cast on parry bash
  CastOnParryBashConfig cast_on_parry_bash_;

  // cast on kill bash
  CastOnKillConfig cast_on_kill_;
  CastOnDrinkConfig cast_on_drink_;

  CastOnMagicProjectileHitConfig cast_on_magic_projectile_hit_;

  CastOnDodgeConfig cast_on_dodge_;

  // cast on kill bash
  CastOnSoulTrapConfig cast_on_soul_trap_;

  // resource manager
  ResourceManagerConfig resource_manager_;

  // TK Dodge RE reimplement
  TkDodgeConfig tk_dodge_;

  // caster additions
  CasterAdditionsConfig caster_additions_;

  // magic weapon
  MagicWeaponConfig magic_weapon_;

  // resist tweaks
  ResistTweaksConfig resist_tweaks_;

  // timing block
  TimingBlockConfig timing_block_;

  // equip load
  EquipLoadConfig equip_load_;

  HighlightingConfig highlighting_;

  // parry bash
  ParryBashConfig parry_bash_;

  // item limit
  ItemLimitConfig item_limit_;

  // death loot
  DeathLootConfig death_loot_;

  // soul link
  SoulLinkConfig soul_link_;

  // ublockable attack
  UnblockableAttackConfig unblockable_attack_;

  // misc fixes
  MiscFixesConfig misc_fixes_;

  // potions drink limit
  PotionsDrinkLimitConfig potions_drink_limit_;

  // potions drink limit
  ProjectileBlockConfig projectile_block_;

  // potions drink limit
  DamageReductionSystemConfig damage_reduction_system_;

  PoisonReworkConfig poison_rework_;

  MagickToStaminaConfig magicka_to_stamina_;

  SlowTimeConfig slow_time_;

  SpeedMultCapConfig speed_mult_cap_;
  DeclutteredLootConfig decluttered_loot_;
  CastOnProjectileBlockConfig cast_on_projectile_block_;
  CastOnProjectileTimingBlockConfig cast_on_projectile_timing_block_;
  CastOnTimingBlockStaggerConfig cast_on_timing_block_stagger_;
  SpecialTechniquesConfig special_techniques_;
  KiEnergyPowerConfig ki_energy_power_;
  CastOnGetHitConfig cast_on_get_hit_;
  CastOnMagicProjectileGetHitConfig cast_on_magic_projectile_get_hit_;
  RecoupEffectConfig recoup_effect_;
  LeechEffectConfig leech_effect_;
  SpellStrikeEffectConfig spell_strike_effect_;
  BoundSpellSwapConfig bound_spell_swap_;
  FollowersSummonsApplySpellConfig followers_summons_apply_spell_;
  ComboSeriesConfig combo_series_;
  CritRevisedConfig crit_revised_;

  public:
  [[nodiscard]] static auto get_singleton() noexcept -> Config&;

  auto load() -> void;

  [[nodiscard]] auto mod_name() const -> const std::string_view& { return mod_name_; }

  [[nodiscard]] auto cast_on_is_cost() const -> RE::BGSKeyword* { return cast_on_is_cost_; }
  [[nodiscard]] auto cast_on_caster_swap() const -> RE::BGSKeyword* { return cast_on_caster_swap_; }

  [[nodiscard]] auto start_attack_duration() const -> float { return start_attack_duration_; }

  [[nodiscard]] auto volume_override() const -> float { return volume_override_; }

  [[nodiscard]] RE::BGSKeyword* restrict_equip_shield() const { return restrict_equip_shield_; }
  [[nodiscard]] RE::BGSKeyword* restrict_equip_offhand() const { return restrict_equip_offhand_; }

  [[nodiscard]] const MagicShieldConfig& magic_shield() const { return magic_shield_; }
  [[nodiscard]] const SpellStrikeEffectConfig& spell_strike_effect() const { return spell_strike_effect_; }
  [[nodiscard]] const BoundSpellSwapConfig& bound_spell_swap() const { return bound_spell_swap_; }
  [[nodiscard]] const RecoupEffectConfig& recoup_effect() const { return recoup_effect_; }
  [[nodiscard]] const LeechEffectConfig& leech_effect() const { return leech_effect_; }
  [[nodiscard]] const AbsorbShieldConfig& absorb_shield() const { return absorb_shield_; }
  [[nodiscard]] const ComboSeriesConfig& combo_series() const { return combo_series_; }
  [[nodiscard]] const FollowersSummonsApplySpellConfig& followers_summons_apply_spell() const
  {
    return followers_summons_apply_spell_;
  }
  [[nodiscard]] const CastOnMagicProjectileGetHitConfig& cast_on_magic_projectile_get_hit() const
  {
    return cast_on_magic_projectile_get_hit_;
  }
  [[nodiscard]] const CastOnGetHitConfig& cast_on_get_hit() const { return cast_on_get_hit_; }
  [[nodiscard]] const StaggerSystemConfig& stagger_system() const { return stagger_system_; }

  [[nodiscard]] const SpecialTechniquesConfig& special_techniques() const { return special_techniques_; }
  [[nodiscard]] const KiEnergyPowerConfig& ki_energy_power() const { return ki_energy_power_; }

  [[nodiscard]] const DamageReductionSystemConfig& damage_reduction_system() const { return damage_reduction_system_; }
  [[nodiscard]] const CastOnTimingBlockStaggerConfig& cast_on_timing_block_stagger() const
  {
    return cast_on_timing_block_stagger_;
  }

  [[nodiscard]] const StaminaShieldConfig& stamina_shield() const { return stamina_shield_; }

  [[nodiscard]] const PetrifiedBloodConfig& petrified_blood() const { return petrified_blood_; }

  [[nodiscard]] const CheatDeathConfig& cheat_death() const { return cheat_death_; }

  [[nodiscard]] const VampirismConfig& vampirism() const { return vampirism_; }

  [[nodiscard]] const MagicVampirismConfig& magic_vampirism() const { return magic_vampirism_; }

  [[nodiscard]] const SpeedCastingConfig& speed_casting() const { return speed_casting_; }

  [[nodiscard]] const WeaponCritConfig& weapon_crit() const { return weapon_crit_; }
  
  [[nodiscard]] const CritRevisedConfig& crit_revised() const { return crit_revised_; }

  [[nodiscard]] const MagicBlockerConfig& magic_blocker() const { return magic_blocker_config_; }

  [[nodiscard]] const MagickCritConfig& magick_crit() const { return magick_crit_; }

  [[nodiscard]] const CastOnCritConfig& cast_on_crit() const { return cast_on_crit_; }
  [[nodiscard]] const CastOnMagicProjectileHitConfig& cast_on_magic_projectile_hit() const
  {
    return cast_on_magic_projectile_hit_;
  }
  [[nodiscard]] const CastOnProjectileBlockConfig& cast_on_projectile_block() const
  {
    return cast_on_projectile_block_;
  }
  [[nodiscard]] const CastOnProjectileTimingBlockConfig& cast_on_projectile_timing_block() const
  {
    return cast_on_projectile_timing_block_;
  }

  [[nodiscard]] const CastOnHitConfig& cast_on_hit() const { return cast_on_hit_; }

  [[nodiscard]] const CastOnBlockConfig& cast_on_block() const { return cast_on_block_; }

  [[nodiscard]] const CastOnBlockParryConfig& cast_on_block_parry() const { return cast_on_block_parry_; }

  [[nodiscard]] const CastOnTimingBlockConfig& cast_on_timing_block() const { return cast_on_timing_block_; }

  [[nodiscard]] const CastOnParryBashConfig& cast_on_parry_bash() const { return cast_on_parry_bash_; }
  [[nodiscard]] const ClairvoyanceConfig& clairvoyance() const { return clairvoyance_; }

  [[nodiscard]] const CastOnKillConfig& cast_on_kill() const { return cast_on_kill_; }
  [[nodiscard]] const CastOnDrinkConfig& cast_on_drink() const { return cast_on_drink_; }

  [[nodiscard]] const CastOnDodgeConfig& cast_on_dodge() const { return cast_on_dodge_; }

  [[nodiscard]] const CastOnSoulTrapConfig& cast_on_soul_trap() const { return cast_on_soul_trap_; }

  [[nodiscard]] const ResourceManagerConfig& resource_manager() const { return resource_manager_; }

  [[nodiscard]] const TkDodgeConfig& tk_dodge() const { return tk_dodge_; }

  [[nodiscard]] const CasterAdditionsConfig& caster_additions() const { return caster_additions_; }

  [[nodiscard]] const MagicWeaponConfig& magic_weapon() const { return magic_weapon_; }

  [[nodiscard]] const ResistTweaksConfig& resist_tweaks() const { return resist_tweaks_; }

  [[nodiscard]] const HighlightingConfig& highlighting() const { return highlighting_; }

  [[nodiscard]] const TimingBlockConfig& timing_block() const { return timing_block_; }

  [[nodiscard]] const EquipLoadConfig& equip_load() const { return equip_load_; }

  [[nodiscard]] const ParryBashConfig& parry_bash() const { return parry_bash_; }

  [[nodiscard]] const ItemLimitConfig& item_limit() const { return item_limit_; }

  [[nodiscard]] const DeathLootConfig& death_loot() const { return death_loot_; }

  [[nodiscard]] const SoulLinkConfig& soul_link() const { return soul_link_; }

  [[nodiscard]] const UnblockableAttackConfig& unblockable_attack() const { return unblockable_attack_; }

  [[nodiscard]] const MiscFixesConfig& misc_fixes() const { return misc_fixes_; }

  [[nodiscard]] const PoisonReworkConfig& poison_rework() const { return poison_rework_; }

  [[nodiscard]] const PotionsDrinkLimitConfig& potions_drink_limit() const { return potions_drink_limit_; }

  [[nodiscard]] const ProjectileBlockConfig& projectile_block() const { return projectile_block_; }
  [[nodiscard]] const CastOnMagicBlockerConfig& cast_on_magic_blocker() const { return cast_on_magic_blocker_; }

  [[nodiscard]] const SpeedMultCapConfig& speed_mult_cap() const { return speed_mult_cap_; }

  [[nodiscard]] const DeclutteredLootConfig& decluttered_loot() const { return decluttered_loot_; }

  [[nodiscard]] const MagickToStaminaConfig& magicka_to_stamina() const { return magicka_to_stamina_; }

  [[nodiscard]] const SlowTimeConfig& slow_time() const { return slow_time_; }
};

} // namespace Reflyem
