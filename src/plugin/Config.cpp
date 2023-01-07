#include "Config.h"

#include <latch>
#include <toml++/toml.h>

namespace reflyem
{
  constexpr auto Reflyem = "Reflyem";
  constexpr auto ModName = "ModName";
  constexpr auto Enable = "Enable";
  constexpr auto ActorValueIndex = "ActorValueIndex";
  constexpr auto MagicShield = "MagicShield";
  constexpr auto Vampirism = "Vampirism";
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

  const config& config::get_singleton() noexcept
  {
    static config instance;

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

      logger::info("config init: petrified blood...");
      instance.petrified_blood_enable = tbl[PetrifiedBlood][Enable].value_or(false);
      if (instance.petrified_blood_enable)
      {
        instance.petrified_blood_av =
          static_cast<RE::ActorValue>(tbl[PetrifiedBlood][Enable].value_or(120));

        auto pb_form_id = tbl[PetrifiedBlood][SpellId].value<RE::FormID>();
        instance.petrified_blood_spell =
          data_handler->LookupForm<RE::SpellItem>(pb_form_id.value(), instance.mod_name);
      }

      logger::info("config init: cheat death");
      instance.cheat_death_enable = tbl[CheatDeath][Enable].value_or(false);
      if (instance.cheat_death_enable)
      {
        auto cd_form_id = tbl[CheatDeath][CheatDeathPercentKeywordId].value<RE::FormID>();
        instance.cheat_death_percent_keyword =
          data_handler->LookupForm<RE::BGSKeyword>(cd_form_id.value(), instance.mod_name);
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

      logger::info("finish init config");
      latch.count_down();
    }
    latch.wait();

    return instance;
  }
}