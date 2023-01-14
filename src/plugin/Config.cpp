#include "Config.h"

#include <latch>
#include <toml++/toml.h>

namespace Reflyem
{
  constexpr auto Reflyem = "Reflyem";
  constexpr auto ModName = "ModName";
  constexpr auto Enable = "Enable";
  constexpr auto ActorValueIndex = "ActorValueIndex";
  constexpr auto MagicShield = "MagicShield";
  constexpr auto Vampirism = "Vampirism";
  constexpr auto MagicVampirism = "MagicVampirism";
  constexpr auto PetrifiedBlood = "PetrifiedBlood";
  constexpr auto SpellId = "SpellId";
  constexpr auto CheatDeath = "CheatDeath";
  constexpr auto CheatDeathPercentKeywordId = "CheatDeathPercentKeywordId";
  constexpr auto SpeedCasting = "SpeedCasting";
  constexpr auto GlobalId = "GlobalId";
  constexpr auto Low = "Low";
  constexpr auto High = "High";
  constexpr auto CostEffectKeywordId = "CostEffectKeywordId";
  constexpr auto EnableMGEFHealth = "EnableMGEFHealth";
  constexpr auto MGEFHealthKeywordId = "MGEFHealthKeywordId";
  constexpr auto EnableMGEFStamina = "EnableMGEFStamina";
  constexpr auto MGEFStaminaKeywordId = "MGEFStaminaKeywordId";
  constexpr auto EnableMGEFMagicka = "EnableMGEFMagicka";
  constexpr auto MGEFMagickaKeywordId = "MGEFMagickaKeywordId";
  constexpr auto WeaponCrit = "WeaponCrit";
  constexpr auto ActorValueIndexCritChance = "ActorValueIndexCritChance";
  constexpr auto ActorValueIndexCritDamage = "ActorValueIndexCritDamage";
  constexpr auto CastOnCrit = "CastOnCrit"; 
  constexpr auto FormListSpellsId = "FormListSpellsId";
  constexpr auto FormListKeywordId = "FormListKeywordId";
  constexpr auto CastOnHit = "CastOnHit";

  constexpr auto ResourceManager = "ResourceManager";
  constexpr auto EnableInfamy = "EnableInfamy";
  constexpr auto EnableWeaponSpend = "EnableWeaponSpend";
  constexpr auto EnableBashSpend = "EnableBashSpend";
  constexpr auto EnableBlock = "EnableBlock";
  constexpr auto WeightMult = "WeightMult";
  constexpr auto GlobalMult = "GlobalMult";
  constexpr auto DamageMult = "DamageMult";
  constexpr auto ArmorMult = "ArmorMult";
  constexpr auto JumpCost = "JumpCost";
  constexpr auto PowerAttackMult = "PowerAttackMult";
  constexpr auto ActorValueAttackCostIndex = "ActorValueAttackCostIndex";
  constexpr auto AttackCostHigh = "AttackCostHigh";
  constexpr auto AttackCostLow = "AttackCostLow";
  constexpr auto ActorValuePowerAttackCostIndex = "ActorValuePowerAttackCostIndex";
  constexpr auto PowerAttackCostHigh = "PowerAttackCostHigh";
  constexpr auto PowerAttackCostLow = "PowerAttackCostLow";
  constexpr auto UnarmedWeaponId = "UnarmedWeaponId";
  constexpr auto KeywordHealthId = "KeywordHealthId";
  constexpr auto KeywordMagickaId = "KeywordMagickaId";
  constexpr auto KeywordStaminaId = "KeywordStaminaId";
  constexpr auto ConvertionStaminaToHealthId = "ConvertionStaminaToHealthId";
  constexpr auto ConvertionMagickaToHealthId = "ConvertionMagickaToHealthId";
  constexpr auto ConvertionStaminaToMagickaId = "ConvertionStaminaToMagickaId";
  constexpr auto ConvertionMagickaToStaminaId = "ConvertionMagickaToStaminaId";
  constexpr auto ConvertionHealthToStaminaId = "ConvertionHealthToStaminaId";
  constexpr auto ConvertionHealthToMagickaId = "ConvertionHealthToMagickaId";

  constexpr auto Physical = "Physical";
  constexpr auto Magick = "Magick";

  constexpr auto TKDodge = "TKDodge";
  constexpr auto DodgeKey = "DodgeKey";
  constexpr auto EnableTappingDodge = "EnableTappingDodge";
  constexpr auto StepDodge = "StepDodge";
  constexpr auto iFrameDuration = "iFrameDuration";
  constexpr auto KeyUpDelay = "KeyUpDelay";
  constexpr auto MaxCost = "MaxCost";
  constexpr auto MinCost = "MinCost";


  const Config& Config::get_singleton() noexcept
  {
    static Config instance;

    static std::atomic_bool initialized;
    static std::latch latch(1);
    if (!initialized.exchange(true))
    {
      logger::info("start init config toml");
      toml::table tbl = toml::parse_file("Data\\SKSE\\Plugins\\Reflyem.toml");
      auto data_handler = RE::TESDataHandler::GetSingleton();

      instance.mod_name = tbl[Reflyem][ModName].value_or("Skyrim.esm");

      logger::info("config init: magic shield...");
      instance.magic_shield_enable = tbl[MagicShield][Enable].value_or(false);
      if (instance.magic_shield_enable)
      {
        auto ms_form_id = tbl[MagicShield][CostEffectKeywordId].value<RE::FormID>();
        instance.magic_shield_cost_keyword =
          data_handler->LookupForm<RE::BGSKeyword>(ms_form_id.value(), instance.mod_name);

        instance.magic_shield_av =
          static_cast<RE::ActorValue>(tbl[MagicShield][ActorValueIndex].value_or(120));

        instance.magic_shield_magick = tbl[MagicShield][Magick].value_or(false);
        instance.magic_shield_physical = tbl[MagicShield][Physical].value_or(false);
      }

      logger::info("config init: vampirism...");
      instance.vampirism_enable = tbl[Vampirism][Enable].value_or(false);
      if (instance.vampirism_enable)
      {
        instance.vampirism_av =
          static_cast<RE::ActorValue>(tbl[Vampirism][ActorValueIndex].value_or(120));
      }

      logger::info("config init: vampirism mgef health...");
      instance.vampirism_mgef_health_enable = tbl[Vampirism][EnableMGEFHealth].value_or(false);
      if (instance.vampirism_mgef_health_enable)
      {
        auto v_form_id = tbl[Vampirism][MGEFHealthKeywordId].value<RE::FormID>();
        instance.vampirism_mgef_health_keyword =
          data_handler->LookupForm<RE::BGSKeyword>(v_form_id.value(), instance.mod_name);
      }

      logger::info("config init: vampirism mgef stamina...");
      instance.vampirism_mgef_stamina_enable = tbl[Vampirism][EnableMGEFStamina].value_or(false);
      if (instance.vampirism_mgef_stamina_enable)
      {
        auto v_form_id = tbl[Vampirism][MGEFStaminaKeywordId].value<RE::FormID>();
        instance.vampirism_mgef_stamina_keyword =
          data_handler->LookupForm<RE::BGSKeyword>(v_form_id.value(), instance.mod_name);
      }

      logger::info("config init: vampirism mgef magicka...");
      instance.vampirism_mgef_magicka_enable = tbl[Vampirism][EnableMGEFMagicka].value_or(false);
      if (instance.vampirism_mgef_magicka_enable)
      {
        auto v_form_id = tbl[Vampirism][MGEFMagickaKeywordId].value<RE::FormID>();
        instance.vampirism_mgef_magicka_keyword =
          data_handler->LookupForm<RE::BGSKeyword>(v_form_id.value(), instance.mod_name);
      }

      logger::info("config init: magic vampirism...");
      instance.magic_vampirism_enable = tbl[MagicVampirism][Enable].value_or(false);
      if (instance.magic_vampirism_enable)
      {
        instance.magic_vampirism_av =
          static_cast<RE::ActorValue>(tbl[MagicVampirism][ActorValueIndex].value_or(120));
      }

      logger::info("config init: magic vampirism mgef health...");
      instance.magic_vampirism_mgef_health_enable = tbl[MagicVampirism][EnableMGEFHealth].value_or(false);
      if (instance.magic_vampirism_mgef_health_enable)
      {
        auto v_form_id = tbl[MagicVampirism][MGEFHealthKeywordId].value<RE::FormID>();
        instance.magic_vampirism_mgef_health_keyword =
          data_handler->LookupForm<RE::BGSKeyword>(v_form_id.value(), instance.mod_name);
      }

      logger::info("config init: magic vampirism mgef stamina...");
      instance.magic_vampirism_mgef_stamina_enable = tbl[MagicVampirism][EnableMGEFStamina].value_or(false);
      if (instance.magic_vampirism_mgef_stamina_enable)
      {
        auto v_form_id = tbl[MagicVampirism][MGEFStaminaKeywordId].value<RE::FormID>();
        instance.magic_vampirism_mgef_stamina_keyword =
          data_handler->LookupForm<RE::BGSKeyword>(v_form_id.value(), instance.mod_name);
      }

      logger::info("config init: magic vampirism mgef magicka...");
      instance.magic_vampirism_mgef_magicka_enable = tbl[MagicVampirism][EnableMGEFMagicka].value_or(false);
      if (instance.magic_vampirism_mgef_magicka_enable)
      {
        auto v_form_id = tbl[MagicVampirism][MGEFMagickaKeywordId].value<RE::FormID>();
        instance.magic_vampirism_mgef_magicka_keyword =
          data_handler->LookupForm<RE::BGSKeyword>(v_form_id.value(), instance.mod_name);
      }

      logger::info("config init: petrified blood...");
      instance.petrified_blood_enable = tbl[PetrifiedBlood][Enable].value_or(false);
      if (instance.petrified_blood_enable)
      {
        instance.petrified_blood_av =
          static_cast<RE::ActorValue>(tbl[PetrifiedBlood][Enable].value_or(120));

        auto pb_form_id = tbl[PetrifiedBlood][SpellId].value<RE::FormID>();
        instance.petrified_blood_spell =
          data_handler->LookupForm<RE::SpellItem>(pb_form_id.value(), instance.mod_name);

        instance.petrified_blood_magick = tbl[PetrifiedBlood][Magick].value_or(false);
        instance.petrified_blood_physical = tbl[PetrifiedBlood][Physical].value_or(false);
      }

      logger::info("config init: cheat death");
      instance.cheat_death_enable = tbl[CheatDeath][Enable].value_or(false);
      if (instance.cheat_death_enable)
      {
        auto cd_form_id = tbl[CheatDeath][CheatDeathPercentKeywordId].value<RE::FormID>();
        instance.cheat_death_percent_keyword =
          data_handler->LookupForm<RE::BGSKeyword>(cd_form_id.value(), instance.mod_name);

        instance.cheat_death_magick = tbl[CheatDeath][Magick].value_or(false);
        instance.cheat_death_physical = tbl[CheatDeath][Physical].value_or(false);
      }

      logger::info("config init: speed casting");
      instance.speed_casting_enable = tbl[SpeedCasting][Enable].value_or(false);
      if (instance.speed_casting_enable)
      {
        auto sc_form_id = tbl[SpeedCasting][GlobalId].value<RE::FormID>();
        instance.speed_casting_global =
          data_handler->LookupForm<RE::TESGlobal>(sc_form_id.value(), instance.mod_name);

        instance.speed_casting_av =
          static_cast<RE::ActorValue>(tbl[SpeedCasting][ActorValueIndex].value_or(120));
        instance.speed_casting_low = tbl[SpeedCasting][Low].value_or(0.20f);
        instance.speed_casting_high = tbl[SpeedCasting][High].value_or(2.0f);
      }

      logger::info("config init: weapon crit");
      instance.weapon_crit_enable = tbl[WeaponCrit][Enable].value_or(false);
      if (instance.weapon_crit_enable)
      {
        instance.weapon_crit_chance_av =
          static_cast<RE::ActorValue>(tbl[WeaponCrit][ActorValueIndexCritChance].value_or(120));
        instance.weapon_crit_damage_av =
          static_cast<RE::ActorValue>(tbl[WeaponCrit][ActorValueIndexCritDamage].value_or(120));
        instance.weapon_crit_high = tbl[WeaponCrit][High].value_or(200);
      }

      logger::info("config init: cast on crit");
      instance.cast_on_crit_enable = tbl[CastOnCrit][Enable].value_or(false);
      if (instance.weapon_crit_enable && instance.cast_on_crit_enable)
      {
        auto cnc_form_idkw = tbl[CastOnCrit][FormListKeywordId].value<RE::FormID>();
        auto cnc_form_idsp = tbl[CastOnCrit][FormListSpellsId].value<RE::FormID>();
        instance.cast_on_crit_formlist_needkw =
          data_handler->LookupForm<RE::BGSListForm>(cnc_form_idkw.value(), instance.mod_name);
        instance.cast_on_crit_formlist_spells =
          data_handler->LookupForm<RE::BGSListForm>(cnc_form_idsp.value(), instance.mod_name);
      }

      logger::info("config init: cast on hit");
      instance.cast_on_hit_enable = tbl[CastOnHit][Enable].value_or(false);
      if (instance.cast_on_hit_enable)
      {
        auto cnh_form_idkw = tbl[CastOnHit][FormListKeywordId].value<RE::FormID>();
        auto cnh_form_idsp = tbl[CastOnHit][FormListSpellsId].value<RE::FormID>();
        instance.cast_on_hit_formlist_needkw =
          data_handler->LookupForm<RE::BGSListForm>(cnh_form_idkw.value(), instance.mod_name);
        instance.cast_on_hit_formlist_spells =
          data_handler->LookupForm<RE::BGSListForm>(cnh_form_idsp.value(), instance.mod_name);
      }

      logger::info("config init: resource manager");
      instance.resource_manager_enable = tbl[ResourceManager][Enable].value_or(false);
      if (instance.resource_manager_enable)
      {
        instance.resource_manager_infamy_enable = tbl[ResourceManager][EnableInfamy].value_or(false);
        instance.resource_manager_weapon_spend_enable = tbl[ResourceManager][EnableWeaponSpend].value_or(false);
        instance.resource_manager_block_spend_enable = tbl[ResourceManager][EnableBlock].value_or(false);
        instance.resource_manager_bash_spend_enable = tbl[ResourceManager][EnableBashSpend].value_or(false);

        instance.resource_manager_attack_cost_av =
          static_cast<RE::ActorValue>(tbl[ResourceManager][ActorValueAttackCostIndex].value_or(120));
        instance.resource_manager_power_attack_cost_av =
          static_cast<RE::ActorValue>(tbl[ResourceManager][ActorValuePowerAttackCostIndex].value_or(120));
        instance.resource_manager_attack_cost_high = tbl[ResourceManager][AttackCostHigh].value_or(100);
        instance.resource_manager_attack_cost_low = tbl[ResourceManager][AttackCostLow].value_or(-100);
        instance.resource_manager_power_attack_cost_high = tbl[ResourceManager][PowerAttackCostHigh].value_or(100);
        instance.resource_manager_power_attack_cost_low = tbl[ResourceManager][PowerAttackCostLow].value_or(-100);
        instance.resource_manager_weight_mult = tbl[ResourceManager][WeightMult].value_or(1.0f);
        instance.resource_manager_global_mult = tbl[ResourceManager][GlobalMult].value_or(1.0f);
        instance.resource_manager_jump_cost = tbl[ResourceManager][JumpCost].value_or(15.f);
        instance.resource_manager_damage_mult = tbl[ResourceManager][DamageMult].value_or(1.f);
        instance.resource_manager_armor_mult = tbl[ResourceManager][ArmorMult].value_or(1.f);
        instance.resource_manager_power_attack_mult = tbl[ResourceManager][PowerAttackMult].value_or(2.f);

        auto rm_unarmed_weapon = tbl[ResourceManager][UnarmedWeaponId].value<RE::FormID>();

        auto rm_health = tbl[ResourceManager][KeywordHealthId].value<RE::FormID>();
        auto rm_stamina = tbl[ResourceManager][KeywordStaminaId].value<RE::FormID>();
        auto rm_magicka = tbl[ResourceManager][KeywordMagickaId].value<RE::FormID>();

        auto rm_stamina_health = tbl[ResourceManager][ConvertionStaminaToHealthId].value<RE::FormID>();
        auto rm_stamina_magicka = tbl[ResourceManager][ConvertionStaminaToMagickaId].value<RE::FormID>();
        auto rm_health_stamina = tbl[ResourceManager][ConvertionHealthToStaminaId].value<RE::FormID>();
        auto rm_health_magicka = tbl[ResourceManager][ConvertionHealthToMagickaId].value<RE::FormID>();
        auto rm_magicka_stamina = tbl[ResourceManager][ConvertionMagickaToStaminaId].value<RE::FormID>();
        auto rm_magicka_health = tbl[ResourceManager][ConvertionMagickaToHealthId].value<RE::FormID>();

        instance.resource_manager_unarmed_weapon =
          data_handler->LookupForm<RE::TESObjectWEAP>(rm_unarmed_weapon.value(), instance.mod_name);

        instance.resource_manager_health_kw =
          data_handler->LookupForm<RE::BGSKeyword>(rm_health.value(), instance.mod_name);
        instance.resource_manager_stamina_kw =
          data_handler->LookupForm<RE::BGSKeyword>(rm_stamina.value(), instance.mod_name);
        instance.resource_manager_magicka_kw =
          data_handler->LookupForm<RE::BGSKeyword>(rm_magicka.value(), instance.mod_name);

        instance.resource_manager_convert_stamina_health_kw =
          data_handler->LookupForm<RE::BGSKeyword>(rm_stamina_health.value(), instance.mod_name);
        instance.resource_manager_convert_stamina_magicka_kw =
          data_handler->LookupForm<RE::BGSKeyword>(rm_stamina_magicka.value(), instance.mod_name);
        instance.resource_manager_convert_health_stamina_kw =
          data_handler->LookupForm<RE::BGSKeyword>(rm_health_stamina.value(), instance.mod_name);
        instance.resource_manager_convert_health_magicka_kw =
          data_handler->LookupForm<RE::BGSKeyword>(rm_health_magicka.value(), instance.mod_name);
        instance.resource_manager_convert_magicka_stamina_kw =
          data_handler->LookupForm<RE::BGSKeyword>(rm_magicka_stamina.value(), instance.mod_name);
        instance.resource_manager_convert_magicka_health_kw =
          data_handler->LookupForm<RE::BGSKeyword>(rm_magicka_health.value(), instance.mod_name);

      }

      logger::info("config init: tk dodge");
      instance.tk_dodge_enable = tbl[TKDodge][Enable].value_or(false);
      if (instance.tk_dodge_enable)
      {
        instance.tk_dodge_gamepad_treshold = 0.15f;
        instance.tk_dodge_iframe_duration = tbl[TKDodge][iFrameDuration].value_or(0.5f);
        instance.tk_dodge_step = tbl[TKDodge][StepDodge].value_or(false);
        instance.tk_dodge_key = tbl[TKDodge][DodgeKey].value_or(277);
        instance.tk_dodge_sprint_tapping_dodge = tbl[TKDodge][EnableTappingDodge].value_or(false);
        instance.tk_dodge_key_up_delay = tbl[TKDodge][KeyUpDelay].value_or(0.2f);
        instance.tk_dodge_max_cost = tbl[TKDodge][MaxCost].value_or(40.f);
        instance.tk_dodge_min_cost = tbl[TKDodge][MinCost].value_or(10.f);

        auto tk_health = tbl[TKDodge][KeywordHealthId].value<RE::FormID>();
        auto tk_stamina = tbl[TKDodge][KeywordStaminaId].value<RE::FormID>();
        auto tk_magicka = tbl[TKDodge][KeywordMagickaId].value<RE::FormID>();

        instance.tk_dodge_health_kw =
          data_handler->LookupForm<RE::BGSKeyword>(tk_health.value(), instance.mod_name);
        instance.tk_dodge_stamina_kw =
          data_handler->LookupForm<RE::BGSKeyword>(tk_stamina.value(), instance.mod_name);
        instance.tk_dodge_magicka_kw =
          data_handler->LookupForm<RE::BGSKeyword>(tk_magicka.value(), instance.mod_name);
      }

      logger::info("finish init config");
      latch.count_down();
    }
    latch.wait();

    return instance;
  }
}