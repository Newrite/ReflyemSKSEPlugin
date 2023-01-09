#pragma once

namespace Reflyem
{
  struct Config
  {
  public:

    [[nodiscard]] static const Config& get_singleton() noexcept;

    // general
    std::string mod_name;

    // magic shield
    bool magic_shield_enable;
    RE::ActorValue magic_shield_av;
    RE::BGSKeyword* magic_shield_cost_keyword;

    // petrified blood
    bool petrified_blood_enable;
    RE::ActorValue petrified_blood_av;
    RE::SpellItem* petrified_blood_spell;

    // cheat death
    bool cheat_death_enable;
    RE::BGSKeyword* cheat_death_percent_keyword;

    // vampirism
    bool vampirism_enable;
    RE::ActorValue vampirism_av;

    // vampirism magick effect
    // health
    bool vampirism_mgef_health_enable;
    RE::BGSKeyword* vampirism_mgef_health_keyword;

    // stamina
    bool vampirism_mgef_stamina_enable;
    RE::BGSKeyword* vampirism_mgef_stamina_keyword;

    // magicka
    bool vampirism_mgef_magicka_enable;
    RE::BGSKeyword* vampirism_mgef_magicka_keyword;

    // FenixSpeedCasting addon
    // speed casting
    bool speed_casting_enable;
    RE::TESGlobal* speed_casting_global;
    RE::ActorValue speed_casting_av;
    float speed_casting_high;
    float speed_casting_low;

    // weapon crit
    bool weapon_crit_enable;
    RE::ActorValue weapon_crit_chance_av;
    RE::ActorValue weapon_crit_damage_av;
    std::int32_t weapon_crit_high;

    // cast on crit
    bool cast_on_crit_enable;
    RE::BGSListForm* cast_on_crit_formlist_spells;
    RE::BGSListForm* cast_on_crit_formlist_needkw;

    // cast on hit
    bool cast_on_hit_enable;
    RE::BGSListForm* cast_on_hit_formlist_spells;
    RE::BGSListForm* cast_on_hit_formlist_needkw;

    // resource manager
    bool resource_manager_enable;
    bool resource_manager_infamy_enable;

    float resource_manager_weight_mult;
    float resource_manager_damage_mult;
    float resource_manager_global_mult;
    float resource_manager_power_attack_mult;
    float resource_manager_jump_cost;

    RE::TESObjectWEAP* resource_manager_unarmed_weapon;

    RE::ActorValue resource_manager_attack_cost_av;
    std::int32_t resource_manager_attack_cost_high;
    std::int32_t resource_manager_attack_cost_low;

    RE::ActorValue resource_manager_power_attack_cost_av;
    std::int32_t resource_manager_power_attack_cost_high;
    std::int32_t resource_manager_power_attack_cost_low;

    // RE::BGSKeyword* resource_manager_shield_health_kw;
    // RE::BGSKeyword* resource_manager_shield_stamina_kw;
    // RE::BGSKeyword* resource_manager_shield_magicka_kw;

    RE::BGSKeyword* resource_manager_health_kw;
    RE::BGSKeyword* resource_manager_stamina_kw;
    RE::BGSKeyword* resource_manager_magicka_kw;

    RE::BGSKeyword* resource_manager_convert_stamina_health_kw;
    RE::BGSKeyword* resource_manager_convert_stamina_magicka_kw;
    RE::BGSKeyword* resource_manager_convert_magicka_health_kw;
    RE::BGSKeyword* resource_manager_convert_magicka_stamina_kw;
    RE::BGSKeyword* resource_manager_convert_health_stamina_kw;
    RE::BGSKeyword* resource_manager_convert_health_magicka_kw;

  };

}