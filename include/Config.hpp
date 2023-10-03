#pragma once

#include <toml++/toml.h>

namespace Reflyem
{
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

    [[nodiscard]] bool physical() const { return physical_; }

    [[nodiscard]] bool magick() const { return magick_; }

    [[nodiscard]] bool must_be_or_not_be() const { return must_be_or_not_be_; }

    [[nodiscard]] RE::BGSKeyword* mgef_keyword() const { return mgef_keyword_; }

    [[nodiscard]] RE::ActorValue av() const { return av_; }

    [[nodiscard]] RE::BGSKeyword* mgef_cost_keyword() const { return mgef_cost_keyword_; }

    StaminaShieldConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    StaminaShieldConfig() = default;

private:
    bool enable_;
    bool physical_;
    bool magick_;
    bool must_be_or_not_be_;
    RE::BGSKeyword* mgef_keyword_;
    RE::ActorValue av_;
    RE::BGSKeyword* mgef_cost_keyword_;
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

    WeaponCritConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    WeaponCritConfig() = default;

private:
    bool enable_;
    RE::ActorValue chance_av_;
    RE::ActorValue damage_av_;
    std::int32_t high_;
    RE::BGSKeyword* keyword_immun_;
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
  };

  struct CastOnCritConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] bool enable_weapon() const { return enable_weapon_; }

    [[nodiscard]] bool physical() const { return physical_; }

    [[nodiscard]] bool magick() const { return magick_; }

    [[nodiscard]] float magick_cooldawn() const { return magick_cooldawn_; }

    [[nodiscard]] RE::BGSListForm* formlist_spells() const { return formlist_spells_; }

    [[nodiscard]] RE::BGSListForm* formlist_needkw() const { return formlist_needkw_; }

    CastOnCritConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnCritConfig() = default;

private:
    bool enable_;
    bool enable_weapon_;
    bool physical_;
    bool magick_;
    float magick_cooldawn_;
    RE::BGSListForm* formlist_spells_;
    RE::BGSListForm* formlist_needkw_;
  };

  struct CastOnHitConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::BGSListForm* formlist_spells() const { return formlist_spells_; }

    [[nodiscard]] RE::BGSListForm* formlist_needkw() const { return formlist_needkw_; }

    CastOnHitConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CastOnHitConfig() = default;

private:
    bool enable_;
    RE::BGSListForm* formlist_spells_;
    RE::BGSListForm* formlist_needkw_;
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

    CastOnBlockParryConfig(
        toml::table& tbl,
        RE::TESDataHandler& data_handler,
        const Config& config);
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

    CastOnTimingBlockConfig(
        toml::table& tbl,
        RE::TESDataHandler& data_handler,
        const Config& config);
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

    [[nodiscard]] RE::SpellItem* spell_block_power_attack() const
    {
      return spell_block_power_attack_;
    }

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

    [[nodiscard]] RE::BGSKeyword* convert_stamina_health_kw() const
    {
      return convert_stamina_health_kw_;
    }

    [[nodiscard]] RE::BGSKeyword* convert_stamina_magicka_kw() const
    {
      return convert_stamina_magicka_kw_;
    }

    [[nodiscard]] RE::BGSKeyword* convert_magicka_health_kw() const
    {
      return convert_magicka_health_kw_;
    }

    [[nodiscard]] RE::BGSKeyword* convert_magicka_stamina_kw() const
    {
      return convert_magicka_stamina_kw_;
    }

    [[nodiscard]] RE::BGSKeyword* convert_health_stamina_kw() const
    {
      return convert_health_stamina_kw_;
    }

    [[nodiscard]] RE::BGSKeyword* convert_health_magicka_kw() const
    {
      return convert_health_magicka_kw_;
    }

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
  };

  struct TkDodgeConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] std::uint32_t key() const { return key_; }

    [[nodiscard]] std::int32_t step() const { return step_; }

    [[nodiscard]] float iframe_duration() const { return iframe_duration_; }

    [[nodiscard]] RE::BGSKeyword* iframe_duration_mgef_kw() const
    {
      return iframe_duration_mgef_kw_;
    }

    [[nodiscard]] float gamepad_treshold() const { return gamepad_treshold_; }

    [[nodiscard]] bool sprint_tapping_dodge() const { return sprint_tapping_dodge_; }

    [[nodiscard]] bool block_dodge_when_attack() const { return block_dodge_when_attack_; }

    [[nodiscard]] RE::BGSKeyword* block_dodge_when_attack_keyword() const
    {
      return block_dodge_when_attack_keyword_;
    }

    [[nodiscard]] bool block_dodge_when_power_attack() const
    {
      return block_dodge_when_power_attack_;
    }

    [[nodiscard]] RE::BGSKeyword* block_dodge_when_power_attack_keyword() const
    {
      return block_dodge_when_power_attack_keyword_;
    }

    [[nodiscard]] bool block_dodge_when_casting() const { return block_dodge_when_casting_; }

    [[nodiscard]] RE::BGSKeyword* block_dodge_when_casting_keyword() const
    {
      return block_dodge_when_casting_keyword_;
    }

    [[nodiscard]] float key_up_delay() const { return key_up_delay_; }

    [[nodiscard]] float max_cost() const { return max_cost_; }

    [[nodiscard]] float min_cost() const { return min_cost_; }

    [[nodiscard]] float flat_cost() const { return flat_cost_; }

    [[nodiscard]] RE::BGSKeyword* cost_from_mgef_kw() const { return cost_from_mgef_kw_; }

    [[nodiscard]] float equipped_weight_mult() const { return equipped_weight_mult_; }

    [[nodiscard]] RE::BGSKeyword* health_kw() const { return health_kw_; }

    [[nodiscard]] RE::BGSKeyword* stamina_kw() const { return stamina_kw_; }

    [[nodiscard]] RE::BGSKeyword* magicka_kw() const { return magicka_kw_; }

    TkDodgeConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    TkDodgeConfig() = default;

private:
    bool enable_;
    std::uint32_t key_;
    std::int32_t step_;
    float iframe_duration_;
    RE::BGSKeyword* iframe_duration_mgef_kw_;
    float gamepad_treshold_;
    bool sprint_tapping_dodge_;
    bool block_dodge_when_attack_;
    RE::BGSKeyword* block_dodge_when_attack_keyword_;
    bool block_dodge_when_power_attack_;
    RE::BGSKeyword* block_dodge_when_power_attack_keyword_;
    bool block_dodge_when_casting_;
    RE::BGSKeyword* block_dodge_when_casting_keyword_;
    float key_up_delay_;
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

    CasterAdditionsConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    CasterAdditionsConfig() = default;

private:
    bool enable_;
    RE::SpellItem* spell_add_when_casting_;
    bool stamina_cost_;
    bool allow_cast_without_stamina_;
    float stamina_cost_mult_;
    RE::BGSSoundDescriptorForm* mag_fail_sound_;
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

  struct ResistTweaksConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] bool enable_check_resistance() const { return enable_check_resistance_; }
    [[nodiscard]] RE::BGSKeyword* max_resist_keyword() const { return max_resist_keyword_; }
    [[nodiscard]] RE::BGSKeyword* negative_resist_immune_keyword() const
    {
      return negative_resist_immune_keyword_;
    }
    [[nodiscard]] float low() const { return low_; }

    [[nodiscard]] bool no_double_resist_check_magick() const
    {
      return no_double_resist_check_magick_;
    }

    [[nodiscard]] bool no_double_resist_check_poison() const
    {
      return no_double_resist_check_poison_;
    }

    [[nodiscard]] bool no_av_damage_resist_penetration() const
    {
      return no_av_damage_resist_penetration_;
    }

    [[nodiscard]] bool enable_damage_resist_tweak() const { return enable_damage_resist_tweak_; }

    [[nodiscard]] bool no_double_damage_resist_check() const
    {
      return no_double_damage_resist_check_;
    }

    [[nodiscard]] bool enable_resist_penetration() const { return enable_resist_penetration_; }

    [[nodiscard]] bool flat_penetration() const { return flat_penetration_; }

    [[nodiscard]] RE::ActorValue penetration_av() const { return penetration_av_; }

    [[nodiscard]] RE::BGSKeyword* penetration_kw() const { return penetration_kw_; }

    [[nodiscard]] std::int32_t penetration_high() const { return penetration_high_; }

    [[nodiscard]] bool npc_max_resistance() const { return npc_max_resistance_; }

    [[nodiscard]] float resist_weight() const { return resist_weight_; }

    [[nodiscard]] bool ench_get_no_absorb() const { return ench_get_no_absorb_; }

    [[nodiscard]] bool ench_ignore_resistance() const { return ench_ignore_resistance_; }

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
    bool ench_get_no_absorb_;
    bool ench_ignore_resistance_;
  };

  struct TimingBlockConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] int32_t parry_stagger_count() const { return parry_stagger_count_; }

    [[nodiscard]] float parry_stagger_count_timer() const { return parry_stagger_count_timer_; }

    [[nodiscard]] float parry_timing() const { return parry_timing_; }

    [[nodiscard]] float block_timing() const { return block_timing_; }

    [[nodiscard]] bool enable_sparks() const { return enable_sparks_; }

    [[nodiscard]] RE::BGSSoundDescriptorForm* parry_sound() const { return parry_sound_; }

    [[nodiscard]] RE::BGSSoundDescriptorForm* block_sound() const { return block_sound_; }

    [[nodiscard]] RE::TESObjectACTI* blank_activator() const { return blank_activator_; }

    [[nodiscard]] RE::BGSKeyword* parry_keyword() const { return parry_keyword_; }

    [[nodiscard]] RE::BGSKeyword* parry_immun_keyword() const { return parry_immun_keyword_; }

    [[nodiscard]] RE::BGSKeyword* parry_timing_keyword() const { return parry_timing_keyword_; }

    [[nodiscard]] RE::BGSKeyword* parry_stagger_count_keyword() const
    {
      return parry_stagger_count_keyword_;
    }

    [[nodiscard]] RE::BGSKeyword* block_keyword() const { return block_keyword_; }

    [[nodiscard]] RE::BGSKeyword* block_immun_keyword() const { return block_immun_keyword_; }

    [[nodiscard]] RE::BGSKeyword* block_timing_keyword() const { return block_timing_keyword_; }

    [[nodiscard]] RE::Explosion* spark() const { return spark_; }

    [[nodiscard]] RE::Explosion* spark_flare() const { return spark_flare_; }

    [[nodiscard]] RE::Explosion* spark_halo() const { return spark_halo_; }

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
  };

  struct EquipLoadConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

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

    [[nodiscard]] bool enable_pre_hit_frame() const { return enable_pre_hit_frame_; }

    [[nodiscard]] bool enable_weapon_swing() const { return enable_weapon_swing_; }

    [[nodiscard]] float parry_timing() const { return parry_timing_; }

    [[nodiscard]] float pre_hit_frame_penalty() const { return pre_hit_frame_penalty_; }

    [[nodiscard]] RE::BGSKeyword* parry_keyword() const { return parry_keyword_; }

    [[nodiscard]] RE::BGSKeyword* parry_immun_keyword() const { return parry_immun_keyword_; }

    [[nodiscard]] RE::BGSKeyword* parry_timing_keyword() const { return parry_timing_keyword_; }

    [[nodiscard]] RE::BGSSoundDescriptorForm* parry_sound() const { return parry_sound_; }

    ParryBashConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    ParryBashConfig() = default;

private:
    bool enable_;
    float parry_timing_;
    float pre_hit_frame_penalty_;
    bool enable_pre_hit_frame_;
    bool enable_weapon_swing_;
    RE::BGSKeyword* parry_keyword_;
    RE::BGSKeyword* parry_immun_keyword_;
    RE::BGSKeyword* parry_timing_keyword_;
    RE::BGSSoundDescriptorForm* parry_sound_;
  };

  struct ItemLimitConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }

    [[nodiscard]] RE::TESObjectREFR* storage() const { return storage_; }

    [[nodiscard]] RE::BGSListForm* keywords_item() const { return keywords_item_; }

    [[nodiscard]] RE::BGSListForm* keywords_cap_mod() const { return keywords_cap_mod_; }

    [[nodiscard]] RE::BGSListForm* globals() const { return globals_; }

    [[nodiscard]] bool enable_auto_restore() const { return enable_auto_restore_; }

    [[nodiscard]] RE::BGSKeyword* auto_restore_block_keyword() const
    {
      return auto_restore_block_keyword_;
    }

    [[nodiscard]] RE::BGSKeyword* exclusive_restore_keyword() const
    {
      return exclusive_restore_keyword_;
    }

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
    [[nodiscard]] RE::BGSKeyword* exclusive_keyword() const { return exclusive_keyword_; }
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] bool auto_loot() const { return auto_loot_; }

    DeathLootConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    DeathLootConfig() = default;

private:
    bool enable_;
    float gold_value_mult_;
    bool auto_loot_;
    RE::BGSKeyword* exclusive_keyword_;
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
    [[nodiscard]] std::int32_t cap_base() const { return cap_base_; }
    [[nodiscard]] float duration_base() const { return duration_base_; }
    [[nodiscard]] std::string notify() const { return notify_; }
    [[nodiscard]] RE::BGSKeyword* exclusive_keyword() const { return exclusive_keyword_; }
    [[nodiscard]] RE::BGSKeyword* cap_keyword() const { return cap_keyword_; }
    [[nodiscard]] RE::BGSKeyword* duration_keyword() const { return duration_keyword_; }

    PotionsDrinkLimitConfig(
        toml::table& tbl,
        RE::TESDataHandler& data_handler,
        const Config& config);
    PotionsDrinkLimitConfig() = default;

private:
    bool enable_;
    std::int32_t cap_base_;
    float duration_base_;
    std::string notify_;
    RE::BGSKeyword* exclusive_keyword_;
    RE::BGSKeyword* cap_keyword_;
    RE::BGSKeyword* duration_keyword_;

    // RE::TESGlobal* max_potions_;
    // RE::TESGlobal* used_potions_;
  };

  struct UnblockableAttackConfig final
  {
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] RE::BGSKeyword* absolute_unblock() const { return absolute_unblock_; }
    [[nodiscard]] RE::BGSKeyword* just_block_unblock() const { return just_block_unblock_; }
    [[nodiscard]] RE::BGSKeyword* timing_block_unblock() const { return timing_block_unblock_; }

    UnblockableAttackConfig(
        toml::table& tbl,
        RE::TESDataHandler& data_handler,
        const Config& config);
    UnblockableAttackConfig() = default;

private:
    bool enable_;
    RE::BGSKeyword* absolute_unblock_;
    RE::BGSKeyword* just_block_unblock_;
    RE::BGSKeyword* timing_block_unblock_;
  };

  struct DamageReductionSystemConfig final
  {
    DamageReductionSystemConfig(
        toml::table& tbl,
        RE::TESDataHandler& data_handler,
        const Config& config);
    DamageReductionSystemConfig() = default;

public:
    [[nodiscard]] bool enable() const { return enable_; }
    [[nodiscard]] RE::ActorValue general_damage_reduction() const
    {
      return general_damage_reduction_;
    }
    [[nodiscard]] RE::ActorValue general_damage_reduction_bypass() const
    {
      return general_damage_reduction_bypass_;
    }
    [[nodiscard]] RE::BGSKeyword* damage_reduction_effect() const
    {
      return damage_reduction_effect_;
    }
    [[nodiscard]] RE::BGSKeyword* damage_reduction_bypass_effect() const
    {
      return damage_reduction_bypass_effect_;
    }
    [[nodiscard]] std::string damage_reduction_keyword() const { return damage_reduction_keyword_; }
    [[nodiscard]] std::string damage_reduction_bypass_keyword() const
    {
      return damage_reduction_bypass_keyword_;
    }

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

  struct MiscFixesConfig final
  {
    [[nodiscard]] bool regeneration_fix() const { return regeneration_fix_; }
    [[nodiscard]] bool equip_bound_fix() const { return equip_bound_fix_; }
    [[nodiscard]] bool disable_sprint_cost_in_combat() const
    {
      return disable_sprint_cost_in_combat_;
    }
    [[nodiscard]] bool disable_over_encumbered() const { return disable_over_encumbered_; }
    [[nodiscard]] bool negative_speed_mult_fix() const { return negative_speed_mult_fix_; }

    MiscFixesConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config);
    MiscFixesConfig() = default;

private:
    bool regeneration_fix_;
    bool equip_bound_fix_;
    bool disable_sprint_cost_in_combat_;
    bool disable_over_encumbered_;
    bool negative_speed_mult_fix_;
  };

  struct AbsorbShieldConfig final
  {
private:
    bool enable_;
    bool enbale_base_shield_;
    bool enable_from_physick_attack_shield_;
    bool enable_from_magick_attack_shield_;
    bool enable_delay_degeneration_after_damage_;
    bool enable_delay_regeneration_after_damage_;

    float base_regeneration_base_shield_physick_;
    float base_regeneration_base_shield_magick_;
    float base_regeneration_base_shield_all_;

    float base_regeneration_from_attack_shield_physick_;
    float base_regeneration_from_attack_shield_magick_;
    float base_regeneration_from_attack_shield_all_;

    float base_delay_base_shield_physick_;
    float base_delay_base_shield_magick_;
    float base_delay_base_shield_all_;

    float base_delay_from_attack_shield_physick_;
    float base_delay_from_attack_shield_magick_;
    float base_delay_from_attack_shield_all_;

    float cap_from_attack_shield_physick_;
    float cap_from_attack_shield_magick_;
    float cap_from_attack_shield_all_;

    RE::BGSKeyword* keyword_base_shield_value_physick_;
    RE::BGSKeyword* keyword_base_shield_value_magick_;
    RE::BGSKeyword* keyword_base_shield_value_all_;

    RE::BGSKeyword* keyword_mult_shield_value_physick_;
    RE::BGSKeyword* keyword_mult_shield_value_magick_;
    RE::BGSKeyword* keyword_mult_shield_value_all_;

    RE::BGSKeyword* keyword_base_delay_base_shield_physick_;
    RE::BGSKeyword* keyword_base_delay_base_shield_magick_;
    RE::BGSKeyword* keyword_base_delay_base_shield_all_;

    RE::BGSKeyword* keyword_percent_from_attack_physick_shield_value_physick_;
    RE::BGSKeyword* keyword_percent_from_attack_physick_shield_value_magick_;
    RE::BGSKeyword* keyword_percent_from_attack_physick_shield_value_all_;

    RE::BGSKeyword* keyword_percent_from_attack_magick_shield_value_physick_;
    RE::BGSKeyword* keyword_percent_from_attack_magick_shield_value_magick_;
    RE::BGSKeyword* keyword_percent_from_attack_magick_shield_value_all_;

    RE::BGSKeyword* keyword_base_delay_from_attack_shield_physick_;
    RE::BGSKeyword* keyword_base_delay_from_attack_shield_magick_;
    RE::BGSKeyword* keyword_base_delay_from_attack_shield_all_;

    RE::BGSKeyword* keyword_cap_from_attack_shield_physick_;
    RE::BGSKeyword* keyword_cap_from_attack_shield_magick_;
    RE::BGSKeyword* keyword_cap_from_attack_shield_all_;

    RE::BGSKeyword* exclusive_keyword_;
  };

  Config();

  // general
  std::string_view mod_name_;

  // magic shield
  MagicShieldConfig magic_shield_;

  // stamina shield
  StaminaShieldConfig stamina_shield_;

  // petrified blood
  PetrifiedBloodConfig petrified_blood_;

  // cheat death
  CheatDeathConfig cheat_death_;

  // vampirism
  VampirismConfig vampirism_;

  // magic vampirism
  MagicVampirismConfig magic_vampirism_;

  // FenixSpeedCasting addon
  // speed casting
  SpeedCastingConfig speed_casting_;

  // weapon crit
  WeaponCritConfig weapon_crit_;

  // magic crit
  MagickCritConfig magick_crit_;

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
  DamageReductionSystemConfig damage_reduction_system_;
  
  SpeedMultCapConfig speed_mult_cap_config_;

  public:
  [[nodiscard]] static auto get_singleton() noexcept -> const Config&;

  [[nodiscard]] auto mod_name() const -> const std::string_view& { return mod_name_; }

  [[nodiscard]] const MagicShieldConfig& magic_shield() const { return magic_shield_; }

  [[nodiscard]] const DamageReductionSystemConfig& damage_reduction_system() const
  {
    return damage_reduction_system_;
  }

  [[nodiscard]] const StaminaShieldConfig& stamina_shield() const { return stamina_shield_; }

  [[nodiscard]] const PetrifiedBloodConfig& petrified_blood() const { return petrified_blood_; }

  [[nodiscard]] const CheatDeathConfig& cheat_death() const { return cheat_death_; }

  [[nodiscard]] const VampirismConfig& vampirism() const { return vampirism_; }

  [[nodiscard]] const MagicVampirismConfig& magic_vampirism() const { return magic_vampirism_; }

  [[nodiscard]] const SpeedCastingConfig& speed_casting() const { return speed_casting_; }

  [[nodiscard]] const WeaponCritConfig& weapon_crit() const { return weapon_crit_; }

  [[nodiscard]] const MagickCritConfig& magick_crit() const { return magick_crit_; }

  [[nodiscard]] const CastOnCritConfig& cast_on_crit() const { return cast_on_crit_; }

  [[nodiscard]] const CastOnHitConfig& cast_on_hit() const { return cast_on_hit_; }

  [[nodiscard]] const CastOnBlockConfig& cast_on_block() const { return cast_on_block_; }

  [[nodiscard]] const CastOnBlockParryConfig& cast_on_block_parry() const
  {
    return cast_on_block_parry_;
  }

  [[nodiscard]] const CastOnTimingBlockConfig& cast_on_timing_block() const
  {
    return cast_on_timing_block_;
  }

  [[nodiscard]] const CastOnParryBashConfig& cast_on_parry_bash() const
  {
    return cast_on_parry_bash_;
  }

  [[nodiscard]] const CastOnKillConfig& cast_on_kill() const { return cast_on_kill_; }

  [[nodiscard]] const ResourceManagerConfig& resource_manager() const { return resource_manager_; }

  [[nodiscard]] const TkDodgeConfig& tk_dodge() const { return tk_dodge_; }

  [[nodiscard]] const CasterAdditionsConfig& caster_additions() const { return caster_additions_; }

  [[nodiscard]] const MagicWeaponConfig& magic_weapon() const { return magic_weapon_; }

  [[nodiscard]] const ResistTweaksConfig& resist_tweaks() const { return resist_tweaks_; }

  [[nodiscard]] const TimingBlockConfig& timing_block() const { return timing_block_; }

  [[nodiscard]] const EquipLoadConfig& equip_load() const { return equip_load_; }

  [[nodiscard]] const ParryBashConfig& parry_bash() const { return parry_bash_; }

  [[nodiscard]] const ItemLimitConfig& item_limit() const { return item_limit_; }

  [[nodiscard]] const DeathLootConfig& death_loot() const { return death_loot_; }

  [[nodiscard]] const SoulLinkConfig& soul_link() const { return soul_link_; }

  [[nodiscard]] const UnblockableAttackConfig& unblockable_attack() const
  {
    return unblockable_attack_;
  }

  [[nodiscard]] const MiscFixesConfig& misc_fixes() const { return misc_fixes_; }

  [[nodiscard]] const PotionsDrinkLimitConfig& potions_drink_limit() const
  {
    return potions_drink_limit_;
  }

  [[nodiscard]] const SpeedMultCapConfig& speed_mult_cap_config() const
  {
    return speed_mult_cap_config_;
  }
};

} // namespace Reflyem