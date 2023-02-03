// ReSharper disable CppInconsistentNaming
#include "Config.hpp"

namespace Reflyem {
constexpr inline std::string_view PathToConfig = "Data\\SKSE\\Plugins\\Reflyem.toml";
constexpr inline std::string_view Reflyem = "Reflyem";
constexpr inline std::string_view ModName = "ModName";
constexpr inline std::string_view Enable = "Enable";
constexpr inline std::string_view ActorValueIndex = "ActorValueIndex";
constexpr inline std::string_view MagicShield = "MagicShield";
constexpr inline std::string_view Vampirism = "Vampirism";
constexpr inline std::string_view MagicVampirism = "MagicVampirism";
constexpr inline std::string_view PetrifiedBlood = "PetrifiedBlood";
constexpr inline std::string_view SpellId = "SpellId";
constexpr inline std::string_view CheatDeath = "CheatDeath";
constexpr inline std::string_view CheatDeathPercentKeywordId = "CheatDeathPercentKeywordId";
constexpr inline std::string_view SpeedCasting = "SpeedCasting";
constexpr inline std::string_view GlobalId = "GlobalId";
constexpr inline std::string_view Low = "Low";
constexpr inline std::string_view High = "High";
constexpr inline std::string_view CostEffectKeywordId = "CostEffectKeywordId";
constexpr inline std::string_view EnableMGEFHealth = "EnableMGEFHealth";
constexpr inline std::string_view MGEFHealthKeywordId = "MGEFHealthKeywordId";
constexpr inline std::string_view EnableMGEFStamina = "EnableMGEFStamina";
constexpr inline std::string_view MGEFStaminaKeywordId = "MGEFStaminaKeywordId";
constexpr inline std::string_view EnableMGEFMagicka = "EnableMGEFMagicka";
constexpr inline std::string_view MGEFMagickaKeywordId = "MGEFMagickaKeywordId";
constexpr inline std::string_view WeaponCrit = "WeaponCrit";
constexpr inline std::string_view ActorValueIndexCritChance = "ActorValueIndexCritChance";
constexpr inline std::string_view ActorValueIndexCritDamage = "ActorValueIndexCritDamage";
constexpr inline std::string_view CastOnCrit = "CastOnCrit";
constexpr inline std::string_view FormListSpellsId = "FormListSpellsId";
constexpr inline std::string_view FormListKeywordId = "FormListKeywordId";
constexpr inline std::string_view CastOnHit = "CastOnHit";
constexpr inline std::string_view ResourceManager = "ResourceManager";
constexpr inline std::string_view EnableInfamy = "EnableInfamy";
constexpr inline std::string_view EnableRegeneration = "EnableRegeneration";
constexpr inline std::string_view EnableWeaponSpend = "EnableWeaponSpend";
constexpr inline std::string_view EnableBashSpend = "EnableBashSpend";
constexpr inline std::string_view EnableBlock = "EnableBlock";
constexpr inline std::string_view WeightMult = "WeightMult";
constexpr inline std::string_view GlobalMult = "GlobalMult";
constexpr inline std::string_view DamageMult = "DamageMult";
constexpr inline std::string_view ArmorMult = "ArmorMult";
constexpr inline std::string_view JumpCost = "JumpCost";
constexpr inline std::string_view PowerAttackMult = "PowerAttackMult";
constexpr inline std::string_view ActorValueAttackCostIndex = "ActorValueAttackCostIndex";
constexpr inline std::string_view AttackCostHigh = "AttackCostHigh";
constexpr inline std::string_view AttackCostLow = "AttackCostLow";
constexpr inline std::string_view ActorValuePowerAttackCostIndex = "ActorValuePowerAttackCostIndex";
constexpr inline std::string_view PowerAttackCostHigh = "PowerAttackCostHigh";
constexpr inline std::string_view PowerAttackCostLow = "PowerAttackCostLow";
constexpr inline std::string_view UnarmedWeaponId = "UnarmedWeaponId";
constexpr inline std::string_view KeywordHealthId = "KeywordHealthId";
constexpr inline std::string_view KeywordMagickaId = "KeywordMagickaId";
constexpr inline std::string_view KeywordStaminaId = "KeywordStaminaId";
constexpr inline std::string_view ConvertionStaminaToHealthId = "ConvertionStaminaToHealthId";
constexpr inline std::string_view ConvertionMagickaToHealthId = "ConvertionMagickaToHealthId";
constexpr inline std::string_view ConvertionStaminaToMagickaId = "ConvertionStaminaToMagickaId";
constexpr inline std::string_view ConvertionMagickaToStaminaId = "ConvertionMagickaToStaminaId";
constexpr inline std::string_view ConvertionHealthToStaminaId = "ConvertionHealthToStaminaId";
constexpr inline std::string_view ConvertionHealthToMagickaId = "ConvertionHealthToMagickaId";
constexpr inline std::string_view Physical = "Physical";
constexpr inline std::string_view Magick = "Magick";
constexpr inline std::string_view TKDodge = "TKDodge";
constexpr inline std::string_view DodgeKey = "DodgeKey";
constexpr inline std::string_view EnableTappingDodge = "EnableTappingDodge";
constexpr inline std::string_view StepDodge = "StepDodge";
constexpr inline std::string_view iFrameDuration = "iFrameDuration";
constexpr inline std::string_view iFrameDurationMGEFKeywordId = "iFrameDurationMGEFKeywordId";
constexpr inline std::string_view KeyUpDelay = "KeyUpDelay";
constexpr inline std::string_view MaxCost = "MaxCost";
constexpr inline std::string_view MinCost = "MinCost";
constexpr inline std::string_view AccumulateEffectKeywordId = "AccumulateEffectKeywordId";
constexpr inline std::string_view Timer100KeywordId = "Timer100KeywordId";
constexpr inline std::string_view FlatCost = "FlatCost";
constexpr inline std::string_view FlatCostMGEFKeywordId = "FlatCostMGEFKeywordId";
constexpr inline std::string_view BlockDodgeWhenAttack = "BlockDodgeWhenAttack";
constexpr inline std::string_view BlockDodgeWhenAttackPerkId = "BlockDodgeWhenAttackPerkId";
constexpr inline std::string_view BlockDodgeWhenPowerAttack = "BlockDodgeWhenPowerAttack";
constexpr inline std::string_view BlockDodgeWhenPowerAttackPerkId =
    "BlockDodgeWhenPowerAttackPerkId";
constexpr inline std::string_view BlockDodgeWhenCasting       = "BlockDodgeWhenCasting";
constexpr inline std::string_view BlockDodgeWhenCastingPerkId = "BlockDodgeWhenCastingPerkId";
constexpr inline std::string_view SpellBlockAttackId          = "SpellBlockAttackId";
constexpr inline std::string_view SpellBlockPowerAttackId     = "SpellBlockPowerAttackId";
constexpr inline std::string_view SpellBlockBashId            = "SpellBlockBashId";
constexpr inline std::string_view SpellAddWhileCastingId      = "SpellAddWhileCastingId";
constexpr inline std::string_view EnableStaminaCost           = "EnableStaminaCost";
constexpr inline std::string_view CasterAdditions             = "CasterAdditions";
constexpr inline std::string_view FormListGlobalsId           = "FormListGlobalsId";
constexpr inline std::string_view MagickCritCooldawn          = "MagickCritCooldawn";
constexpr inline std::string_view MagicWeapon                 = "MagicWeapon";
constexpr inline std::string_view KeywordId                   = "KeywordId";
constexpr inline std::string_view MustBeOrNotBe               = "MustBeOrNotBe";
constexpr inline std::string_view MagickCrit                  = "MagickCrit";
constexpr inline std::string_view CastOnBlock                 = "CastOnBlock";
constexpr inline std::string_view RegenDelay                  = "RegenDelay";
constexpr inline std::string_view ResistTweaks                = "ResistTweaks";
constexpr inline std::string_view CheckResistance             = "CheckResistance";
constexpr inline std::string_view EnchGetNoAbsorb             = "EnchGetNoAbsorb";
constexpr inline std::string_view EnchIgnoreResistance        = "EnchIgnoreResistance";

Config::MagicShieldConfig::MagicShieldConfig(toml::table&  tbl, RE::TESDataHandler& data_handler,
                                             const Config& config) {

  logger::info("config init: magic shield..."sv);
  enable_ = tbl[MagicShield][Enable].value_or(false);
  if (enable_) {
    const auto effect_keyword_cost_form_id =
        tbl[MagicShield][CostEffectKeywordId].value<RE::FormID>();
    mgef_cost_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(
        effect_keyword_cost_form_id.value(), config.mod_name());

    av_ = static_cast<RE::ActorValue>(tbl[MagicShield][ActorValueIndex].value_or(120));

    magick_   = tbl[MagicShield][Magick].value_or(false);
    physical_ = tbl[MagicShield][Physical].value_or(false);

    const auto must_or_not_effect_keyword_form_id = tbl[MagicShield][KeywordId].value<RE::FormID>();

    mgef_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(
        must_or_not_effect_keyword_form_id.value(), config.mod_name());

    must_be_or_not_be_ = tbl[MagicShield][MustBeOrNotBe].value_or(false);
  }
}

Config::PetrifiedBloodConfig::PetrifiedBloodConfig(toml::table&        tbl,
                                                   RE::TESDataHandler& data_handler,
                                                   const Config&       config) {
  logger::info("config init: petrified blood..."sv);
  enable_ = tbl[PetrifiedBlood][Enable].value_or(false);
  if (enable_) {
    av_ = static_cast<RE::ActorValue>(tbl[PetrifiedBlood][Enable].value_or(120));

    const auto blood_spell_form_id = tbl[PetrifiedBlood][SpellId].value<RE::FormID>();
    blood_spell_                   =
        data_handler.LookupForm<RE::SpellItem>(blood_spell_form_id.value(), config.mod_name());

    const auto effect_accumulate_keyword_form_id =
        tbl[PetrifiedBlood][AccumulateEffectKeywordId].value<RE::FormID>();
    mgef_accumulate_keyword_damage_ = data_handler.LookupForm<RE::BGSKeyword>(
        effect_accumulate_keyword_form_id.value(), config.mod_name());

    magick_   = tbl[PetrifiedBlood][Magick].value_or(false);
    physical_ = tbl[PetrifiedBlood][Physical].value_or(false);

    const auto must_or_not_effect_keyword_form_id =
        tbl[PetrifiedBlood][KeywordId].value<RE::FormID>();

    mgef_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(
        must_or_not_effect_keyword_form_id.value(), config.mod_name());

    must_be_or_not_be_ = tbl[PetrifiedBlood][MustBeOrNotBe].value_or(false);
  }
}

Config::CheatDeathConfig::CheatDeathConfig(toml::table&  tbl, RE::TESDataHandler& data_handler,
                                           const Config& config) {
  logger::info("config init: cheat death"sv);
  enable_ = tbl[CheatDeath][Enable].value_or(false);
  if (enable_) {
    const auto effect_percent_keyword_form_id =
        tbl[CheatDeath][CheatDeathPercentKeywordId].value<RE::FormID>();
    mgef_percent_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(
        effect_percent_keyword_form_id.value(), config.mod_name());

    magick_   = tbl[CheatDeath][Magick].value_or(false);
    physical_ = tbl[CheatDeath][Physical].value_or(false);
  }
}

Config::VampirismConfig::VampirismConfig(toml::table&  tbl, RE::TESDataHandler& data_handler,
                                         const Config& config) {
  logger::info("config init: vampirism..."sv);
  enable_ = tbl[Vampirism][Enable].value_or(false);
  if (enable_) {
    av_ = static_cast<RE::ActorValue>(tbl[Vampirism][ActorValueIndex].value_or(120));
  }

  logger::info("config init: vampirism mgef health..."sv);
  mgef_health_enable_ = tbl[Vampirism][EnableMGEFHealth].value_or(false);
  if (mgef_health_enable_) {
    const auto effect_vampirism_health_keyword_form_id =
        tbl[Vampirism][MGEFHealthKeywordId].value<RE::FormID>();
    mgef_health_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(
        effect_vampirism_health_keyword_form_id.value(), config.mod_name());
  }

  logger::info("config init: vampirism mgef stamina..."sv);
  mgef_stamina_enable_ = tbl[Vampirism][EnableMGEFStamina].value_or(false);
  if (mgef_stamina_enable_) {
    const auto effect_vampirism_stamina_keyword_form_id =
        tbl[Vampirism][MGEFStaminaKeywordId].value<RE::FormID>();
    mgef_stamina_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(
        effect_vampirism_stamina_keyword_form_id.value(), config.mod_name());
  }

  logger::info("config init: vampirism mgef magicka..."sv);
  mgef_magicka_enable_ = tbl[Vampirism][EnableMGEFMagicka].value_or(false);
  if (mgef_magicka_enable_) {
    const auto effect_vampirism_magicka_keyword_form_id =
        tbl[Vampirism][MGEFMagickaKeywordId].value<RE::FormID>();
    mgef_magicka_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(
        effect_vampirism_magicka_keyword_form_id.value(), config.mod_name());
  }
}

Config::MagicVampirismConfig::MagicVampirismConfig(toml::table&        tbl,
                                                   RE::TESDataHandler& data_handler,
                                                   const Config&       config) {

  logger::info("config init: magic vampirism..."sv);
  enable_ = tbl[MagicVampirism][Enable].value_or(false);
  if (enable_) {
    av_ = static_cast<RE::ActorValue>(tbl[MagicVampirism][ActorValueIndex].value_or(120));
  }

  logger::info("config init: magic vampirism mgef health..."sv);
  mgef_health_enable_ = tbl[MagicVampirism][EnableMGEFHealth].value_or(false);
  if (mgef_health_enable_) {
    const auto effect_vampirism_health_keyword_form_id =
        tbl[MagicVampirism][MGEFHealthKeywordId].value<RE::FormID>();
    mgef_health_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(
        effect_vampirism_health_keyword_form_id.value(), config.mod_name());
  }

  logger::info("config init: magic vampirism mgef stamina..."sv);
  mgef_stamina_enable_ = tbl[MagicVampirism][EnableMGEFStamina].value_or(false);
  if (mgef_stamina_enable_) {
    const auto effect_vampirism_stamina_keyword_form_id =
        tbl[MagicVampirism][MGEFStaminaKeywordId].value<RE::FormID>();
    mgef_stamina_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(
        effect_vampirism_stamina_keyword_form_id.value(), config.mod_name());
  }

  logger::info("config init: magic vampirism mgef magicka..."sv);
  mgef_magicka_enable_ = tbl[MagicVampirism][EnableMGEFMagicka].value_or(false);
  if (mgef_magicka_enable_) {
    const auto effect_vampirism_magicka_keyword_form_id =
        tbl[MagicVampirism][MGEFMagickaKeywordId].value<RE::FormID>();
    mgef_magicka_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(
        effect_vampirism_magicka_keyword_form_id.value(), config.mod_name());
  }

  if (enable_ || mgef_health_enable_ || mgef_stamina_enable_ || mgef_magicka_enable_) {
    const auto must_or_not_effect_keyword_form_id =
        tbl[MagicVampirism][KeywordId].value<RE::FormID>();

    mgef_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(
        must_or_not_effect_keyword_form_id.value(), config.mod_name());

    must_be_or_not_be_ = tbl[MagicVampirism][MustBeOrNotBe].value_or(false);
  }
}

Config::SpeedCastingConfig::SpeedCastingConfig(toml::table&  tbl, RE::TESDataHandler& data_handler,
                                               const Config& config) {
  logger::info("config init: speed casting"sv);
  enable_ = tbl[SpeedCasting][Enable].value_or(false);
  if (enable_) {
    const auto speed_casting_global_form_id = tbl[SpeedCasting][GlobalId].value<RE::FormID>();
    global_ = data_handler.LookupForm<RE::TESGlobal>(speed_casting_global_form_id.value(),
                                                     config.mod_name());

    av_   = static_cast<RE::ActorValue>(tbl[SpeedCasting][ActorValueIndex].value_or(120));
    low_  = tbl[SpeedCasting][Low].value_or(0.20f);
    high_ = tbl[SpeedCasting][High].value_or(2.0f);
  }
}

Config::WeaponCritConfig::WeaponCritConfig(toml::table& tbl, RE::TESDataHandler&, const Config&) {
  logger::info("config init: weapon crit"sv);
  enable_ = tbl[WeaponCrit][Enable].value_or(false);
  if (enable_) {
    chance_av_ =
        static_cast<RE::ActorValue>(tbl[WeaponCrit][ActorValueIndexCritChance].value_or(120));
    damage_av_ =
        static_cast<RE::ActorValue>(tbl[WeaponCrit][ActorValueIndexCritDamage].value_or(120));
    high_ = tbl[WeaponCrit][High].value_or(200);
  }
}

Config::MagickCritConfig::MagickCritConfig(toml::table&  tbl, RE::TESDataHandler& data_handler,
                                           const Config& config) {
  {
    logger::info("config init: magick crit"sv);
    enable_ = tbl[MagickCrit][Enable].value_or(false);
    if (enable_) {

      chance_av_ =
          static_cast<RE::ActorValue>(tbl[MagickCrit][ActorValueIndexCritChance].value_or(120));
      damage_av_ =
          static_cast<RE::ActorValue>(tbl[MagickCrit][ActorValueIndexCritDamage].value_or(120));

      high_ = tbl[MagickCrit][High].value_or(200);

      const auto must_or_not_effect_keyword_form_id =
          tbl[MagickCrit][KeywordId].value<RE::FormID>();

      mgef_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(
          must_or_not_effect_keyword_form_id.value(), config.mod_name());

      must_be_or_not_be_ = tbl[MagickCrit][MustBeOrNotBe].value_or(false);
    }
  }
}

Config::CastOnCritConfig::CastOnCritConfig(toml::table&  tbl, RE::TESDataHandler& data_handler,
                                           const Config& config) {
  logger::info("config init: cast on crit"sv);
  enable_ = tbl[CastOnCrit][Enable].value_or(false);
  if ((config.weapon_crit().enable() || config.magick_crit().enable()) && enable_) {

    physical_        = tbl[CastOnCrit][Physical].value_or(false);
    magick_          = tbl[CastOnCrit][Magick].value_or(false);
    magick_cooldawn_ = tbl[CastOnCrit][MagickCritCooldawn].value_or(1.0f);

    const auto form_keywords_form_id = tbl[CastOnCrit][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id   = tbl[CastOnCrit][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_                 =
        data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}

Config::CastOnHitConfig::CastOnHitConfig(toml::table&  tbl, RE::TESDataHandler& data_handler,
                                         const Config& config) {
  logger::info("config init: cast on hit"sv);
  enable_ = tbl[CastOnHit][Enable].value_or(false);
  if (enable_) {
    const auto form_keywords_form_id = tbl[CastOnHit][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id   = tbl[CastOnHit][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_                 =
        data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}

Config::CastOnBlockConfig::CastOnBlockConfig(toml::table&  tbl, RE::TESDataHandler& data_handler,
                                             const Config& config) {
  logger::info("config init: cast on hit"sv);
  enable_ = tbl[CastOnBlock][Enable].value_or(false);
  if (enable_) {
    const auto form_keywords_form_id = tbl[CastOnBlock][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id   = tbl[CastOnBlock][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_                 =
        data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}

Config::ResourceManagerConfig::ResourceManagerConfig(toml::table&        tbl,
                                                     RE::TESDataHandler& data_handler,
                                                     const Config&       config) {
  logger::info("config init: resource manager"sv);
  enable_ = tbl[ResourceManager][Enable].value_or(false);
  if (enable_) {
    infamy_enable_       = tbl[ResourceManager][EnableInfamy].value_or(false);
    regeneration_enable_ = tbl[ResourceManager][EnableRegeneration].value_or(false);
    weapon_spend_enable_ = tbl[ResourceManager][EnableWeaponSpend].value_or(false);
    block_spend_enable_  = tbl[ResourceManager][EnableBlock].value_or(false);
    bash_spend_enable_   = tbl[ResourceManager][EnableBashSpend].value_or(false);

    regen_delay_ = tbl[ResourceManager][RegenDelay].value_or(2.5f);

    auto spell_for_block_attack = tbl[ResourceManager][SpellBlockAttackId].value<RE::FormID>();
    auto spell_for_block_power_attack =
        tbl[ResourceManager][SpellBlockPowerAttackId].value<RE::FormID>();
    auto spell_for_block_bash = tbl[ResourceManager][SpellBlockBashId].value<RE::FormID>();

    spell_block_attack_ =
        data_handler.LookupForm<RE::SpellItem>(spell_for_block_attack.value(), config.mod_name());
    spell_block_power_attack_ = data_handler.LookupForm<RE::SpellItem>(
        spell_for_block_power_attack.value(), config.mod_name());
    spell_block_bash_ =
        data_handler.LookupForm<RE::SpellItem>(spell_for_block_bash.value(), config.mod_name());

    attack_cost_av_ =
        static_cast<RE::ActorValue>(tbl[ResourceManager][ActorValueAttackCostIndex].value_or(120));
    power_attack_cost_av_ = static_cast<RE::ActorValue>(
      tbl[ResourceManager][ActorValuePowerAttackCostIndex].value_or(120));
    attack_cost_high_       = tbl[ResourceManager][AttackCostHigh].value_or(100);
    attack_cost_low_        = tbl[ResourceManager][AttackCostLow].value_or(-100);
    power_attack_cost_high_ = tbl[ResourceManager][PowerAttackCostHigh].value_or(100);
    power_attack_cost_low_  = tbl[ResourceManager][PowerAttackCostLow].value_or(-100);
    weight_mult_            = tbl[ResourceManager][WeightMult].value_or(1.0f);
    global_mult_            = tbl[ResourceManager][GlobalMult].value_or(1.0f);
    jump_cost_              = tbl[ResourceManager][JumpCost].value_or(15.f);
    damage_mult_            = tbl[ResourceManager][DamageMult].value_or(1.f);
    armor_mult_             = tbl[ResourceManager][ArmorMult].value_or(1.f);
    power_attack_mult_      = tbl[ResourceManager][PowerAttackMult].value_or(2.f);

    auto unarmed_weapon_form_id = tbl[ResourceManager][UnarmedWeaponId].value<RE::FormID>();

    auto health_keyword_form_id  = tbl[ResourceManager][KeywordHealthId].value<RE::FormID>();
    auto stamina_keyword_form_id = tbl[ResourceManager][KeywordStaminaId].value<RE::FormID>();
    auto magicka_keyword_form_id = tbl[ResourceManager][KeywordMagickaId].value<RE::FormID>();

    auto convertion_keyword_stamina_health_form_id =
        tbl[ResourceManager][ConvertionStaminaToHealthId].value<RE::FormID>();
    auto convertion_keyword_stamina_magicka_form_id =
        tbl[ResourceManager][ConvertionStaminaToMagickaId].value<RE::FormID>();
    auto convertion_keyword_health_stamina_form_id =
        tbl[ResourceManager][ConvertionHealthToStaminaId].value<RE::FormID>();
    auto convertion_keyword_health_magicka_form_id =
        tbl[ResourceManager][ConvertionHealthToMagickaId].value<RE::FormID>();
    auto convertion_keyword_magicka_stamina_form_id =
        tbl[ResourceManager][ConvertionMagickaToStaminaId].value<RE::FormID>();
    auto convertion_keyword_magicka_health_form_id =
        tbl[ResourceManager][ConvertionMagickaToHealthId].value<RE::FormID>();

    unarmed_weapon_ = data_handler.LookupForm<RE::TESObjectWEAP>(unarmed_weapon_form_id.value(),
                                                                 config.mod_name());

    health_kw_ =
        data_handler.LookupForm<RE::BGSKeyword>(health_keyword_form_id.value(), config.mod_name());
    stamina_kw_ =
        data_handler.LookupForm<RE::BGSKeyword>(stamina_keyword_form_id.value(), config.mod_name());
    magicka_kw_ =
        data_handler.LookupForm<RE::BGSKeyword>(magicka_keyword_form_id.value(), config.mod_name());

    convert_stamina_health_kw_ = data_handler.LookupForm<RE::BGSKeyword>(
        convertion_keyword_stamina_health_form_id.value(), config.mod_name());
    convert_stamina_magicka_kw_ = data_handler.LookupForm<RE::BGSKeyword>(
        convertion_keyword_stamina_magicka_form_id.value(), config.mod_name());
    convert_health_stamina_kw_ = data_handler.LookupForm<RE::BGSKeyword>(
        convertion_keyword_health_stamina_form_id.value(), config.mod_name());
    convert_health_magicka_kw_ = data_handler.LookupForm<RE::BGSKeyword>(
        convertion_keyword_health_magicka_form_id.value(), config.mod_name());
    convert_magicka_stamina_kw_ = data_handler.LookupForm<RE::BGSKeyword>(
        convertion_keyword_magicka_stamina_form_id.value(), config.mod_name());
    convert_magicka_health_kw_ = data_handler.LookupForm<RE::BGSKeyword>(
        convertion_keyword_magicka_health_form_id.value(), config.mod_name());
  }
}

Config::TkDodgeConfig::TkDodgeConfig(toml::table&  tbl, RE::TESDataHandler& data_handler,
                                     const Config& config) {
  logger::info("config init: tk dodge"sv);
  enable_ = tbl[TKDodge][Enable].value_or(false);
  if (enable_) {
    gamepad_treshold_              = 0.15f;
    iframe_duration_               = tbl[TKDodge][iFrameDuration].value_or(0.5f);
    step_                          = tbl[TKDodge][StepDodge].value_or(false);
    key_                           = tbl[TKDodge][DodgeKey].value_or(277);
    sprint_tapping_dodge_          = tbl[TKDodge][EnableTappingDodge].value_or(false);
    block_dodge_when_attack_       = tbl[TKDodge][BlockDodgeWhenAttack].value_or(false);
    block_dodge_when_power_attack_ = tbl[TKDodge][BlockDodgeWhenPowerAttack].value_or(false);
    block_dodge_when_casting_      = tbl[TKDodge][BlockDodgeWhenCasting].value_or(false);
    key_up_delay_                  = tbl[TKDodge][KeyUpDelay].value_or(0.2f);
    equipped_weight_mult_          = tbl[TKDodge][WeightMult].value_or(1.0f);
    flat_cost_                     = tbl[TKDodge][FlatCost].value_or(0.f);
    max_cost_                      = tbl[TKDodge][MaxCost].value_or(40.f);
    min_cost_                      = tbl[TKDodge][MinCost].value_or(10.f);

    const auto health_spend_keyword_form_id  = tbl[TKDodge][KeywordHealthId].value<RE::FormID>();
    const auto stamina_spend_keyword_form_id = tbl[TKDodge][KeywordStaminaId].value<RE::FormID>();
    const auto spend_magicka_keyword_form_id = tbl[TKDodge][KeywordMagickaId].value<RE::FormID>();

    const auto block_attack_perk_form_id =
        tbl[TKDodge][BlockDodgeWhenAttackPerkId].value<RE::FormID>();
    const auto block_power_attack_perk_form_id =
        tbl[TKDodge][BlockDodgeWhenPowerAttackPerkId].value<RE::FormID>();
    const auto block_casting_perk_form_id =
        tbl[TKDodge][BlockDodgeWhenCastingPerkId].value<RE::FormID>();
    const auto effect_keyword_iframe_form_id =
        tbl[TKDodge][iFrameDurationMGEFKeywordId].value<RE::FormID>();
    const auto effect_keyword_flat_cost_form_id =
        tbl[TKDodge][FlatCostMGEFKeywordId].value<RE::FormID>();

    health_kw_ = data_handler.LookupForm<RE::BGSKeyword>(health_spend_keyword_form_id.value(),
                                                         config.mod_name());
    stamina_kw_ = data_handler.LookupForm<RE::BGSKeyword>(stamina_spend_keyword_form_id.value(),
                                                          config.mod_name());
    magicka_kw_ = data_handler.LookupForm<RE::BGSKeyword>(spend_magicka_keyword_form_id.value(),
                                                          config.mod_name());

    block_dodge_when_attack_perk_ =
        data_handler.LookupForm<RE::BGSPerk>(block_attack_perk_form_id.value(), config.mod_name());
    block_dodge_when_power_attack_perk_ = data_handler.LookupForm<RE::BGSPerk>(
        block_power_attack_perk_form_id.value(), config.mod_name());
    block_dodge_when_casting_perk_ =
        data_handler.LookupForm<RE::BGSPerk>(block_casting_perk_form_id.value(), config.mod_name());
    iframe_duration_mgef_kw_ = data_handler.LookupForm<RE::BGSKeyword>(
        effect_keyword_iframe_form_id.value(), config.mod_name());
    cost_from_mgef_kw_ = data_handler.LookupForm<RE::BGSKeyword>(
        effect_keyword_flat_cost_form_id.value(), config.mod_name());
  }
}

Config::CasterAdditionsConfig::CasterAdditionsConfig(toml::table&        tbl,
                                                     RE::TESDataHandler& data_handler,
                                                     const Config&       config) {
  logger::info("config init: caster additions"sv);
  enable_ = tbl[CasterAdditions][Enable].value_or(false);
  if (enable_) {

    const auto spell_add_when_casting_form_id =
        tbl[CasterAdditions][SpellAddWhileCastingId].value<RE::FormID>();

    spell_add_when_casting_ = data_handler.LookupForm<RE::SpellItem>(
        spell_add_when_casting_form_id.value(), config.mod_name());

    stamina_cost_ = tbl[CasterAdditions][EnableStaminaCost].value_or(false);
  }
}

Config::MagicWeaponConfig::MagicWeaponConfig(toml::table&  tbl, RE::TESDataHandler& data_handler,
                                             const Config& config) {
  logger::info("config init: magic weapon"sv);
  enable_ = tbl[MagicWeapon][Enable].value_or(false);
  if (enable_) {

    const auto formlist_keywords_form_id = tbl[MagicWeapon][FormListKeywordId].value<RE::FormID>();
    const auto formlist_spells_form_id   = tbl[MagicWeapon][FormListSpellsId].value<RE::FormID>();
    const auto formlist_globals_form_id  = tbl[MagicWeapon][FormListGlobalsId].value<RE::FormID>();

    keywords_ = data_handler.LookupForm<RE::BGSListForm>(formlist_keywords_form_id.value(),
                                                         config.mod_name());
    spells_ = data_handler.LookupForm<RE::BGSListForm>(formlist_spells_form_id.value(),
                                                       config.mod_name());
    globals_ = data_handler.LookupForm<RE::BGSListForm>(formlist_globals_form_id.value(),
                                                        config.mod_name());
  }
}

Config::ResistTweaksConfig::ResistTweaksConfig(toml::table& tbl, RE::TESDataHandler&,
                                               const Config&) {
  logger::info("config init: resist tweaks"sv);
  enable_ = tbl[ResistTweaks][Enable].value_or(false);
  if (enable_) {
    ench_get_no_absorb_     = tbl[ResistTweaks][EnchGetNoAbsorb].value_or(false);
    check_resistance_       = tbl[ResistTweaks][CheckResistance].value_or(false);
    ench_ignore_resistance_ = tbl[ResistTweaks][EnchIgnoreResistance].value_or(false);
  }
}

Config::Config() {
  logger::info("start init config toml"sv);
  auto       tbl          = toml::parse_file(PathToConfig);
  const auto data_handler = RE::TESDataHandler::GetSingleton();

  mod_name_         = tbl[Reflyem][ModName].value_or("Skyrim.esm"sv);
  magic_shield_     = MagicShieldConfig(tbl, *data_handler, *this);
  petrified_blood_  = PetrifiedBloodConfig(tbl, *data_handler, *this);
  cheat_death_      = CheatDeathConfig(tbl, *data_handler, *this);
  vampirism_        = VampirismConfig(tbl, *data_handler, *this);
  magic_vampirism_  = MagicVampirismConfig(tbl, *data_handler, *this);
  speed_casting_    = SpeedCastingConfig(tbl, *data_handler, *this);
  weapon_crit_      = WeaponCritConfig(tbl, *data_handler, *this);
  magick_crit_      = MagickCritConfig(tbl, *data_handler, *this);
  cast_on_crit_     = CastOnCritConfig(tbl, *data_handler, *this);
  cast_on_hit_      = CastOnHitConfig(tbl, *data_handler, *this);
  cast_on_block_    = CastOnBlockConfig(tbl, *data_handler, *this);
  resource_manager_ = ResourceManagerConfig(tbl, *data_handler, *this);
  tk_dodge_         = TkDodgeConfig(tbl, *data_handler, *this);
  caster_additions_ = CasterAdditionsConfig(tbl, *data_handler, *this);
  magic_weapon_     = MagicWeaponConfig(tbl, *data_handler, *this);
  resist_tweaks_    = ResistTweaksConfig(tbl, *data_handler, *this);

  logger::info("finish init config"sv);
}


auto Config::get_singleton() noexcept -> const Config& {
  static Config instance;
  return instance;
}
} // namespace Reflyem
