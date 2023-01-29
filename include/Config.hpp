#pragma once

namespace Reflyem {
struct Config {
private:
  Config();
public:
  [[nodiscard]] static const Config& get_singleton() noexcept;

  // general
  std::string mod_name;

  // magic shield
  bool            magic_shield_enable{};
  bool            magic_shield_physical{};
  bool            magic_shield_magick{};
  bool            magic_shield_must_be_or_not_be{};
  RE::BGSKeyword* magic_shield_mgef_keyword_id{};
  RE::ActorValue  magic_shield_av;
  RE::BGSKeyword* magic_shield_cost_keyword{};

  // petrified blood
  bool            petrified_blood_enable{};
  bool            petrified_blood_physical{};
  bool            petrified_blood_magick{};
  bool            petrified_blood_must_be_or_not_be{};
  RE::BGSKeyword* petrified_blood_mgef_keyword_id{};
  RE::BGSKeyword* petrified_blood_acc_mgef_kw{};
  RE::ActorValue  petrified_blood_av;
  RE::SpellItem*  petrified_blood_spell{};

  // cheat death
  bool            cheat_death_enable{};
  bool            cheat_death_physical{};
  bool            cheat_death_magick{};
  RE::BGSKeyword* cheat_death_percent_keyword{};

  // vampirism
  bool           vampirism_enable{};
  RE::ActorValue vampirism_av;

  // vampirism magick effect
  // health
  bool            vampirism_mgef_health_enable{};
  RE::BGSKeyword* vampirism_mgef_health_keyword{};

  // stamina
  bool            vampirism_mgef_stamina_enable{};
  RE::BGSKeyword* vampirism_mgef_stamina_keyword{};

  // magicka
  bool            vampirism_mgef_magicka_enable{};
  RE::BGSKeyword* vampirism_mgef_magicka_keyword{};

  // magic vampirism
  bool            magic_vampirism_enable{};
  bool            magic_vampirism_must_be_or_not_be{};
  RE::BGSKeyword* magic_vampirism_mgef_keyword_id{};
  RE::ActorValue  magic_vampirism_av;

  // magic vampirism magick effect
  // health
  bool            magic_vampirism_mgef_health_enable{};
  RE::BGSKeyword* magic_vampirism_mgef_health_keyword{};

  // stamina
  bool            magic_vampirism_mgef_stamina_enable{};
  RE::BGSKeyword* magic_vampirism_mgef_stamina_keyword{};

  // magicka
  bool            magic_vampirism_mgef_magicka_enable{};
  RE::BGSKeyword* magic_vampirism_mgef_magicka_keyword{};

  // FenixSpeedCasting addon
  // speed casting
  bool           speed_casting_enable{};
  RE::TESGlobal* speed_casting_global{};
  RE::ActorValue speed_casting_av;
  float          speed_casting_high{};
  float          speed_casting_low{};

  // weapon crit
  bool           weapon_crit_enable{};
  RE::ActorValue weapon_crit_chance_av;
  RE::ActorValue weapon_crit_damage_av;
  std::int32_t   weapon_crit_high{};

  // cast on crit
  bool             cast_on_crit_enable{};
  RE::BGSListForm* cast_on_crit_formlist_spells{};
  RE::BGSListForm* cast_on_crit_formlist_needkw{};

  // cast on hit
  bool             cast_on_hit_enable{};
  RE::BGSListForm* cast_on_hit_formlist_spells{};
  RE::BGSListForm* cast_on_hit_formlist_needkw{};

  // resource manager
  bool resource_manager_enable{};
  bool resource_manager_weapon_spend_enable{};
  bool resource_manager_bash_spend_enable{};
  bool resource_manager_block_spend_enable{};
  bool resource_manager_regeneration_enable{};
  bool resource_manager_infamy_enable{};

  float resource_manager_weight_mult{};
  float resource_manager_damage_mult{};
  float resource_manager_armor_mult{};
  float resource_manager_global_mult{};
  float resource_manager_power_attack_mult{};
  float resource_manager_jump_cost{};

  RE::SpellItem* resource_manage_spell_block_attack{};
  RE::SpellItem* resource_manage_spell_block_power_attack{};
  RE::SpellItem* resource_manage_spell_block_bash{};

  RE::TESObjectWEAP* resource_manager_unarmed_weapon{};

  RE::ActorValue resource_manager_attack_cost_av;
  std::int32_t   resource_manager_attack_cost_high{};
  std::int32_t   resource_manager_attack_cost_low{};

  RE::ActorValue resource_manager_power_attack_cost_av;
  std::int32_t   resource_manager_power_attack_cost_high{};
  std::int32_t   resource_manager_power_attack_cost_low{};

  RE::BGSKeyword* resource_manager_health_kw{};
  RE::BGSKeyword* resource_manager_stamina_kw{};
  RE::BGSKeyword* resource_manager_magicka_kw{};

  RE::BGSKeyword* resource_manager_convert_stamina_health_kw{};
  RE::BGSKeyword* resource_manager_convert_stamina_magicka_kw{};
  RE::BGSKeyword* resource_manager_convert_magicka_health_kw{};
  RE::BGSKeyword* resource_manager_convert_magicka_stamina_kw{};
  RE::BGSKeyword* resource_manager_convert_health_stamina_kw{};
  RE::BGSKeyword* resource_manager_convert_health_magicka_kw{};

  // magic crit
  bool           magic_crit_enable{};
  RE::ActorValue magic_crit_chance_av;
  RE::ActorValue magic_crit_damage_av;
  std::int32_t   magic_crit_high{};

  // TK Dodge RE reimplement
  bool            tk_dodge_enable{};
  std::uint32_t   tk_dodge_key{};
  std::int32_t    tk_dodge_step{};
  float           tk_dodge_iframe_duration{};
  RE::BGSKeyword* tk_dodge_iframe_duration_mgef_kw{};
  float           tk_dodge_gamepad_treshold{};
  bool            tk_dodge_sprint_tapping_dodge{};
  bool            tk_dodge_block_dodge_when_attack{};
  RE::BGSPerk*    tk_dodge_block_dodge_when_attack_perk{};
  bool            tk_dodge_block_dodge_when_power_attack{};
  RE::BGSPerk*    tk_dodge_block_dodge_when_power_attack_perk{};
  bool            tk_dodge_block_dodge_when_casting{};
  RE::BGSPerk*    tk_dodge_block_dodge_when_casting_perk{};
  float           tk_dodge_key_up_delay{};
  float           tk_dodge_max_cost{};
  float           tk_dodge_min_cost{};
  float           tk_dodge_flat_cost{};
  RE::BGSKeyword* tk_dodge_cost_from_mgef_kw{};
  float           tk_dodge_equipped_weight_mult{};
  RE::BGSKeyword* tk_dodge_health_kw{};
  RE::BGSKeyword* tk_dodge_stamina_kw{};
  RE::BGSKeyword* tk_dodge_magicka_kw{};

  // caster debuff movement
  bool           caster_debuff_enable{};
  RE::SpellItem* caster_debuff_spell{};

  // magic weapon
  bool             magic_weapon_enable{};
  RE::BGSListForm* magic_weapon_spells{};
  RE::BGSListForm* magic_weapon_keywords{};
  RE::BGSListForm* magic_weapon_globals{};
};

} // namespace Reflyem