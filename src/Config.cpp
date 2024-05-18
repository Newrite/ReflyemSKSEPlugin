// ReSharper disable CppInconsistentNaming
#include "Config.hpp"

namespace Reflyem {
constexpr inline std::string_view PathToConfig = R"(Data\SKSE\Plugins\Reflyem.toml)";
constexpr inline std::string_view Reflyem = "Reflyem";
constexpr inline std::string_view MiscFixes = "MiscFixes";
constexpr inline std::string_view RegenerationFix = "RegenerationFix";
constexpr inline std::string_view EquipBoundFix = "EquipBoundFix";
constexpr inline std::string_view DisableSprintCostInCombat = "DisableSprintCostInCombat";
constexpr inline std::string_view DisableOverEncumbered = "DisableOverEncumbered";
constexpr inline std::string_view NegativeSpeedMultFix = "NegativeSpeedMultFix";
constexpr inline std::string_view NoSneakRequirementForSneakAttack = "NoSneakRequirementForSneakAttack";
constexpr inline std::string_view SpeedMultCap = "SpeedMultCap";
constexpr inline std::string_view ProjectileBlock = "ProjectileBlock";
constexpr inline std::string_view EffectAllowOvercapKeyword = "EffectAllowOvercapKeyword";
constexpr inline std::string_view PoisonRework = "PoisonRework";
constexpr inline std::string_view MagickToStamina = "MagickToStamina";
constexpr inline std::string_view EffectMutateCapKeyword = "EffectMutateCapKeyword";
constexpr inline std::string_view ModName = "ModName";
constexpr inline std::string_view SoulLink = "SoulLink";
constexpr inline std::string_view DeathLoot = "DeathLoot";
constexpr inline std::string_view GoldMult = "GoldMult";
constexpr inline std::string_view OtherCapBase = "OtherCapBase";
constexpr inline std::string_view OtherDurationBase = "OtherDurationBase";
constexpr inline std::string_view Notify = "Notify";
constexpr inline std::string_view EnableAutoRestore = "EnableAutoRestore";
constexpr inline std::string_view ItemLimit = "ItemLimit";
constexpr inline std::string_view AutoLoot = "AutoLoot";
constexpr inline std::string_view OtherExclusiveKeywordId = "OtherExclusiveKeywordId";
constexpr inline std::string_view HealthEnable = "HealthEnable";
constexpr inline std::string_view OtherEnable = "OtherEnable";
constexpr inline std::string_view HealthCapBase = "HealthCapBase";
constexpr inline std::string_view HealthDurationBase = "HealthDurationBase";
constexpr inline std::string_view HealthKeywordId = "HealthKeywordId";
constexpr inline std::string_view HealthCapKeywordId = "HealthCapKeywordId";
constexpr inline std::string_view HealthDurationKeywordId = "HealthDurationKeywordId";
constexpr inline std::string_view StaminaEnable = "StaminaEnable";
constexpr inline std::string_view StaminaCapBase = "StaminaCapBase";
constexpr inline std::string_view StaminaDurationBase = "StaminaDurationBase";
constexpr inline std::string_view StaminaKeywordId = "StaminaKeywordId";
constexpr inline std::string_view StaminaCapKeywordId = "StaminaCapKeywordId";
constexpr inline std::string_view StaminaDurationKeywordId = "StaminaDurationKeywordId";
constexpr inline std::string_view MagickaEnable = "MagickaEnable";
constexpr inline std::string_view MagickaCapBase = "MagickaCapBase";
constexpr inline std::string_view MagickaDurationBase = "MagickaDurationBase";
constexpr inline std::string_view MagickaKeywordId = "MagickaKeywordId";
constexpr inline std::string_view MagickaCapKeywordId = "MagickaCapKeywordId";
constexpr inline std::string_view MagickaDurationKeywordId = "MagickaDurationKeywordId";
constexpr inline std::string_view OtherCapKeywordId = "OtherCapKeywordId";
constexpr inline std::string_view OtherDurationKeywordId = "OtherDurationKeywordId";
constexpr inline std::string_view SummonSplitKeywordId = "SummonSplitKeywordId";
constexpr inline std::string_view StorageFormId = "StorageFormId";
constexpr inline std::string_view EnableEnderal = "EnableEnderal";
constexpr inline std::string_view FormListKeywordItemId = "FormListKeywordItemId";
constexpr inline std::string_view FormListKeywordCapId = "FormListKeywordCapId";
constexpr inline std::string_view Enable = "Enable";
constexpr inline std::string_view CapBase = "CapBase";
constexpr inline std::string_view DurationBase = "DurationBase";
constexpr inline std::string_view CapKeywordId = "CapKeywordId";
constexpr inline std::string_view DurationKeywordId = "DurationKeywordId";
constexpr inline std::string_view ApplyOnBothWeapon = "ApplyOnBothWeapon";
constexpr inline std::string_view ExclusiveKeywordId = "ExclusiveKeywordId";
constexpr inline std::string_view RevertExclusive = "RevertExclusive";
constexpr inline std::string_view PotionsDrinkLimit = "PotionsDrinkLimit";
constexpr inline std::string_view NoRemoveKeywordId = "NoRemoveKeywordId";
constexpr inline std::string_view OtherRevertExclusive = "OtherRevertExclusive";
constexpr inline std::string_view UnblockableAttack = "UnblockableAttack";
constexpr inline std::string_view EnableWeapon = "EnableWeapon";
constexpr inline std::string_view ActorValueIndex = "ActorValueIndex";
constexpr inline std::string_view ActorValueIndexPhysic = "ActorValueIndexPhysic";
constexpr inline std::string_view ActorValueIndexMagick = "ActorValueIndexMagick";
constexpr inline std::string_view MagicShield = "MagicShield";
constexpr inline std::string_view StaminaShield = "StaminaShield";
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
constexpr inline std::string_view Timer100KeywordId = "Timer100KeywordId";
constexpr inline std::string_view FlatCost = "FlatCost";
constexpr inline std::string_view FlatCostMGEFKeywordId = "FlatCostMGEFKeywordId";
constexpr inline std::string_view BlockDodgeWhenAttack = "BlockDodgeWhenAttack";
constexpr inline std::string_view BlockDodgeWhenAttackKeywordId = "BlockDodgeWhenAttackKeywordId";
constexpr inline std::string_view BlockDodgeWhenPowerAttack = "BlockDodgeWhenPowerAttack";
constexpr inline std::string_view BlockDodgeWhenPowerAttackKeywordId = "BlockDodgeWhenPowerAttackKeywordId";
constexpr inline std::string_view BlockDodgeWhenCasting = "BlockDodgeWhenCasting";
constexpr inline std::string_view BlockDodgeWhenCastingKeywordId = "BlockDodgeWhenCastingKeywordId";
constexpr inline std::string_view SpellBlockAttackId = "SpellBlockAttackId";
constexpr inline std::string_view SpellBlockPowerAttackId = "SpellBlockPowerAttackId";
constexpr inline std::string_view SpellBlockBashId = "SpellBlockBashId";
constexpr inline std::string_view SpellAddWhileCastingId = "SpellAddWhileCastingId";
constexpr inline std::string_view EnableStaminaCost = "EnableStaminaCost";
constexpr inline std::string_view CasterAdditions = "CasterAdditions";
constexpr inline std::string_view FormListGlobalsId = "FormListGlobalsId";
constexpr inline std::string_view MagickCritCooldawn = "MagickCritCooldawn";
constexpr inline std::string_view MagicWeapon = "MagicWeapon";
constexpr inline std::string_view KeywordId = "KeywordId";
constexpr inline std::string_view MustBeOrNotBe = "MustBeOrNotBe";
constexpr inline std::string_view MagickCrit = "MagickCrit";
constexpr inline std::string_view CastOnBlock = "CastOnBlock";
constexpr inline std::string_view CastOnTimingBlock = "CastOnTimingBlock";
constexpr inline std::string_view CastOnBlockParry = "CastOnBlockParry";
constexpr inline std::string_view CastOnParryBash = "CastOnParryBash";
constexpr inline std::string_view CastOnKill = "CastOnKill";
constexpr inline std::string_view CastOnProjectileBlock = "CastOnProjectileBlock";
constexpr inline std::string_view CastOnProjectileTimingBlock = "CastOnProjectileTimingBlock";
constexpr inline std::string_view RegenDelay = "RegenDelay";
constexpr inline std::string_view ResistTweaks = "ResistTweaks";
constexpr inline std::string_view MaxResistKeywordId = "MaxResistKeywordId";
constexpr inline std::string_view NegativeResistImmuneKeywordId = "NegativeResistImmuneKeywordId";
constexpr inline std::string_view EnableCheckResistance = "EnableCheckResistance";
constexpr inline std::string_view EnchGetNoAbsorb = "EnchGetNoAbsorb";
constexpr inline std::string_view EnchIgnoreResistance = "EnchIgnoreResistance";
constexpr inline std::string_view ResistWeight = "ResistWeight";
constexpr inline std::string_view NpcMaxResistance = "NpcMaxResistance";
constexpr inline std::string_view NoDoubleResistCheckMagick = "NoDoubleResistCheckMagick";
constexpr inline std::string_view NoDoubleResistCheckPoison = "NoDoubleResistCheckPoison";
constexpr inline std::string_view EnableDamageResistTweak = "EnableDamageResistTweak";
constexpr inline std::string_view NoDoubleDamageResistCheck = "NoDoubleDamageResistCheck";
constexpr inline std::string_view EnableResistPenetration = "EnableResistPenetration";
constexpr inline std::string_view FlatPenetration = "FlatPenetration";
constexpr inline std::string_view NoAvDamageResistPenetration = "NoAvDamageResistPenetration";
constexpr inline std::string_view KeywordImmunId = "KeywordImmunId";
constexpr inline std::string_view TimingBlock = "TimingBlock";
constexpr inline std::string_view BlockTiming = "BlockTiming";
constexpr inline std::string_view BaseFlatCost = "BaseFlatCost";
constexpr inline std::string_view BasePercentCost = "BasePercentCost";
constexpr inline std::string_view EnableBlockMagickForMagick = "EnableBlockMagickForMagick";
constexpr inline std::string_view EnableTimingBlock = "EnableTimingBlock";
constexpr inline std::string_view EnableArrow = "EnableArrow";
constexpr inline std::string_view EnableCone = "EnableCone";
constexpr inline std::string_view EnableFlame = "EnableFlame";
constexpr inline std::string_view EnableBeam = "EnableBeam";
constexpr inline std::string_view EnableMissile = "EnableMissile";
constexpr inline std::string_view ParryTiming = "ParryTiming";
constexpr inline std::string_view BlankActivatorId = "BlankActivatorId";
constexpr inline std::string_view SparkId = "SparkId";
constexpr inline std::string_view SparkFlareId = "SparkFlareId";
constexpr inline std::string_view SparkHaloId = "SparkHaloId";
constexpr inline std::string_view EnableSparks = "EnableSparks";
constexpr inline std::string_view EnableSound = "EnableSound";
constexpr inline std::string_view ParryKeywordId = "ParryKeywordId";
constexpr inline std::string_view ParryImmunKeywordId = "ParryImmunKeywordId";
constexpr inline std::string_view ParryTimingKeywordId = "ParryTimingKeywordId";
constexpr inline std::string_view BlockKeywordId = "BlockKeywordId";
constexpr inline std::string_view BlockImmunKeywordId = "BlockImmunKeywordId";
constexpr inline std::string_view BlockTimingKeywordId = "BlockTimingKeywordId";
constexpr inline std::string_view ParryStaggerCount = "ParryStaggerCount";
constexpr inline std::string_view ParryStaggerCountKeywordId = "ParryStaggerCountKeywordId";
constexpr inline std::string_view ParryStaggerCountTimer = "ParryStaggerCountTimer";
constexpr inline std::string_view BlockTimer = "BlockTimer";
constexpr inline std::string_view AbsoluteUnblockKeywordId = "AbsoluteUnblockKeywordId";
constexpr inline std::string_view TimingUnblockKeywordId = "TimingUnblockKeywordId";
constexpr inline std::string_view JustUnblockKeywordId = "JustUnblockKeywordId";
constexpr inline std::string_view LowEquipAbilityId = "LowEquipAbilityId";
constexpr inline std::string_view MidEquipAbilityId = "MidEquipAbilityId";
constexpr inline std::string_view HigEquipAbilityId = "HigEquipAbilityId";
constexpr inline std::string_view BlockDelay = "BlockDelay";
constexpr inline std::string_view EquipWeightMult = "EquipWeightMult";
constexpr inline std::string_view ArmorWeightMult = "ArmorWeightMult";
constexpr inline std::string_view WeaponWeightMult = "WeaponWeightMult";
constexpr inline std::string_view EnablePrecisionWeaponCollide = "EnablePrecisionWeaponCollide";
constexpr inline std::string_view LowPercent = "LowPercent";
constexpr inline std::string_view HighPercent = "HighPercent";
constexpr inline std::string_view EquipLoad = "EquipLoad";
constexpr inline std::string_view ParrySoundId = "ParrySoundId";
constexpr inline std::string_view BlockSoundId = "BlockSoundId";
constexpr inline std::string_view EnableBlockKeywordId = "EnableBlockKeywordId";
constexpr inline std::string_view EnableTimingBlockKeywordId = "EnableTimingBlockKeywordId";
constexpr inline std::string_view TimingBlockTimerKeywordId = "TimingBlockTimerKeywordId";
constexpr inline std::string_view BlockCostKeywordId = "BlockCostKeywordId";
constexpr inline std::string_view EnableArrowBlockKeywordId = "EnableArrowBlockKeywordId";
constexpr inline std::string_view EnableMagickBlockKeywordId = "EnableMagickBlockKeywordId";
constexpr inline std::string_view EnableArrowTimingBlockKeywordId = "EnableArrowTimingBlockKeywordId";
constexpr inline std::string_view EnableMagickTimingBlockKeywordId = "EnableMagickTimingBlockKeywordId";
constexpr inline std::string_view ExclusiveTimingKeywordId = "ExclusiveTimingKeywordId";
constexpr inline std::string_view PreHitFramePenalty = "PreHitFramePenalty";
constexpr inline std::string_view ParryBash = "ParryBash";
constexpr inline std::string_view EnablePreHitFrame = "EnablePreHitFrame";
constexpr inline std::string_view EnableWeaponSwing = "EnableWeaponSwing";
constexpr inline std::string_view StaminaCostMult = "StaminaCostMult";
constexpr inline std::string_view AllowCastWithoutStamina = "AllowCastWithoutStamina";

Config::MagicShieldConfig::MagicShieldConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: magic shield..."sv);
  enable_ = tbl[MagicShield][Enable].value_or(false);
  if (enable_) {
    const auto effect_keyword_cost_form_id = tbl[MagicShield][CostEffectKeywordId].value<RE::FormID>();
    mgef_cost_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(effect_keyword_cost_form_id.value(), config.mod_name());

    av_ = static_cast<RE::ActorValue>(tbl[MagicShield][ActorValueIndex].value_or(120));

    magick_ = tbl[MagicShield][Magick].value_or(false);
    physical_ = tbl[MagicShield][Physical].value_or(false);

    const auto must_or_not_effect_keyword_form_id = tbl[MagicShield][KeywordId].value<RE::FormID>();

    mgef_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(must_or_not_effect_keyword_form_id.value(), config.mod_name());

    must_be_or_not_be_ = tbl[MagicShield][MustBeOrNotBe].value_or(false);
  }
}

Config::StaminaShieldConfig::StaminaShieldConfig(toml::table& tbl,
                                                 RE::TESDataHandler& data_handler,
                                                 const Config& config)
{
  logger::info("config init: stamina shield..."sv);
  enable_ = tbl[StaminaShield][Enable].value_or(false);
  if (enable_) {
    const auto effect_keyword_cost_form_id = tbl[StaminaShield][CostEffectKeywordId].value<RE::FormID>();
    mgef_cost_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(effect_keyword_cost_form_id.value(), config.mod_name());

    av_ = static_cast<RE::ActorValue>(tbl[StaminaShield][ActorValueIndex].value_or(120));

    magick_ = tbl[StaminaShield][Magick].value_or(false);
    physical_ = tbl[StaminaShield][Physical].value_or(false);

    const auto must_or_not_effect_keyword_form_id = tbl[StaminaShield][KeywordId].value<RE::FormID>();

    mgef_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(must_or_not_effect_keyword_form_id.value(), config.mod_name());

    must_be_or_not_be_ = tbl[StaminaShield][MustBeOrNotBe].value_or(false);
  }
}

Config::PetrifiedBloodConfig::PetrifiedBloodConfig(toml::table& tbl,
                                                   RE::TESDataHandler& data_handler,
                                                   const Config& config)
{
  logger::info("config init: petrified blood..."sv);
  enable_ = tbl[PetrifiedBlood][Enable].value_or(false);
  if (enable_) {
    av_physic_ = static_cast<RE::ActorValue>(tbl[PetrifiedBlood][ActorValueIndexPhysic].value_or(120));
    av_magick_ = static_cast<RE::ActorValue>(tbl[PetrifiedBlood][ActorValueIndexMagick].value_or(120));

    const auto blood_spell_form_id = tbl[PetrifiedBlood][SpellId].value<RE::FormID>();
    blood_spell_ = data_handler.LookupForm<RE::SpellItem>(blood_spell_form_id.value(), config.mod_name());

    magick_ = tbl[PetrifiedBlood][Magick].value_or(false);
    physical_ = tbl[PetrifiedBlood][Physical].value_or(false);

    const auto must_or_not_effect_keyword_form_id = tbl[PetrifiedBlood][KeywordId].value<RE::FormID>();

    mgef_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(must_or_not_effect_keyword_form_id.value(), config.mod_name());

    must_be_or_not_be_ = tbl[PetrifiedBlood][MustBeOrNotBe].value_or(false);
  }
}

Config::CheatDeathConfig::CheatDeathConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: cheat death"sv);
  enable_ = tbl[CheatDeath][Enable].value_or(false);
  if (enable_) {
    const auto effect_percent_keyword_form_id = tbl[CheatDeath][CheatDeathPercentKeywordId].value<RE::FormID>();
    mgef_percent_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(effect_percent_keyword_form_id.value(), config.mod_name());

    magick_ = tbl[CheatDeath][Magick].value_or(false);
    physical_ = tbl[CheatDeath][Physical].value_or(false);
  }
}

Config::VampirismConfig::VampirismConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: vampirism..."sv);
  enable_ = tbl[Vampirism][Enable].value_or(false);
  if (enable_) {
    av_ = static_cast<RE::ActorValue>(tbl[Vampirism][ActorValueIndex].value_or(120));
  }

  logger::info("config init: vampirism mgef health..."sv);
  mgef_health_enable_ = tbl[Vampirism][EnableMGEFHealth].value_or(false);
  if (mgef_health_enable_) {
    const auto effect_vampirism_health_keyword_form_id = tbl[Vampirism][MGEFHealthKeywordId].value<RE::FormID>();
    mgef_health_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(effect_vampirism_health_keyword_form_id.value(), config.mod_name());
  }

  logger::info("config init: vampirism mgef stamina..."sv);
  mgef_stamina_enable_ = tbl[Vampirism][EnableMGEFStamina].value_or(false);
  if (mgef_stamina_enable_) {
    const auto effect_vampirism_stamina_keyword_form_id = tbl[Vampirism][MGEFStaminaKeywordId].value<RE::FormID>();
    mgef_stamina_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(effect_vampirism_stamina_keyword_form_id.value(), config.mod_name());
  }

  logger::info("config init: vampirism mgef magicka..."sv);
  mgef_magicka_enable_ = tbl[Vampirism][EnableMGEFMagicka].value_or(false);
  if (mgef_magicka_enable_) {
    const auto effect_vampirism_magicka_keyword_form_id = tbl[Vampirism][MGEFMagickaKeywordId].value<RE::FormID>();
    mgef_magicka_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(effect_vampirism_magicka_keyword_form_id.value(), config.mod_name());
  }
}

Config::MagicVampirismConfig::MagicVampirismConfig(toml::table& tbl,
                                                   RE::TESDataHandler& data_handler,
                                                   const Config& config)
{
  logger::info("config init: magic vampirism..."sv);
  enable_ = tbl[MagicVampirism][Enable].value_or(false);
  if (enable_) {
    av_ = static_cast<RE::ActorValue>(tbl[MagicVampirism][ActorValueIndex].value_or(120));
  }

  logger::info("config init: magic vampirism mgef health..."sv);
  mgef_health_enable_ = tbl[MagicVampirism][EnableMGEFHealth].value_or(false);
  if (mgef_health_enable_) {
    const auto effect_vampirism_health_keyword_form_id = tbl[MagicVampirism][MGEFHealthKeywordId].value<RE::FormID>();
    mgef_health_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(effect_vampirism_health_keyword_form_id.value(), config.mod_name());
  }

  logger::info("config init: magic vampirism mgef stamina..."sv);
  mgef_stamina_enable_ = tbl[MagicVampirism][EnableMGEFStamina].value_or(false);
  if (mgef_stamina_enable_) {
    const auto effect_vampirism_stamina_keyword_form_id = tbl[MagicVampirism][MGEFStaminaKeywordId].value<RE::FormID>();
    mgef_stamina_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(effect_vampirism_stamina_keyword_form_id.value(), config.mod_name());
  }

  logger::info("config init: magic vampirism mgef magicka..."sv);
  mgef_magicka_enable_ = tbl[MagicVampirism][EnableMGEFMagicka].value_or(false);
  if (mgef_magicka_enable_) {
    const auto effect_vampirism_magicka_keyword_form_id = tbl[MagicVampirism][MGEFMagickaKeywordId].value<RE::FormID>();
    mgef_magicka_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(effect_vampirism_magicka_keyword_form_id.value(), config.mod_name());
  }

  if (enable_ || mgef_health_enable_ || mgef_stamina_enable_ || mgef_magicka_enable_) {
    const auto must_or_not_effect_keyword_form_id = tbl[MagicVampirism][KeywordId].value<RE::FormID>();

    mgef_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(must_or_not_effect_keyword_form_id.value(), config.mod_name());

    must_be_or_not_be_ = tbl[MagicVampirism][MustBeOrNotBe].value_or(false);
  }
}

Config::SpeedCastingConfig::SpeedCastingConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: speed casting"sv);
  enable_ = tbl[SpeedCasting][Enable].value_or(false);
  if (enable_) {
    const auto speed_casting_global_form_id = tbl[SpeedCasting][GlobalId].value<RE::FormID>();
    global_ = data_handler.LookupForm<RE::TESGlobal>(speed_casting_global_form_id.value(), config.mod_name());

    av_ = static_cast<RE::ActorValue>(tbl[SpeedCasting][ActorValueIndex].value_or(120));
    low_ = tbl[SpeedCasting][Low].value_or(0.20f);
    high_ = tbl[SpeedCasting][High].value_or(2.0f);
  }
}

Config::WeaponCritConfig::WeaponCritConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: weapon crit"sv);
  enable_ = tbl[WeaponCrit][Enable].value_or(false);
  if (enable_) {
    chance_av_ = static_cast<RE::ActorValue>(tbl[WeaponCrit][ActorValueIndexCritChance].value_or(120));
    damage_av_ = static_cast<RE::ActorValue>(tbl[WeaponCrit][ActorValueIndexCritDamage].value_or(120));
    high_ = tbl[WeaponCrit][High].value_or(200);

    const auto immun_keyword_form_id = tbl[WeaponCrit][KeywordImmunId].value<RE::FormID>();

    keyword_immun_ = data_handler.LookupForm<RE::BGSKeyword>(immun_keyword_form_id.value(), config.mod_name());
  }
}

Config::MagickCritConfig::MagickCritConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  {
    logger::info("config init: magick crit"sv);
    enable_ = tbl[MagickCrit][Enable].value_or(false);
    if (enable_) {
      chance_av_ = static_cast<RE::ActorValue>(tbl[MagickCrit][ActorValueIndexCritChance].value_or(120));
      damage_av_ = static_cast<RE::ActorValue>(tbl[MagickCrit][ActorValueIndexCritDamage].value_or(120));

      high_ = tbl[MagickCrit][High].value_or(200);

      const auto must_or_not_effect_keyword_form_id = tbl[MagickCrit][KeywordId].value<RE::FormID>();

      mgef_keyword_ =
          data_handler.LookupForm<RE::BGSKeyword>(must_or_not_effect_keyword_form_id.value(), config.mod_name());

      const auto immun_keyword_form_id = tbl[MagickCrit][KeywordImmunId].value<RE::FormID>();

      keyword_immun_ = data_handler.LookupForm<RE::BGSKeyword>(immun_keyword_form_id.value(), config.mod_name());

      must_be_or_not_be_ = tbl[MagickCrit][MustBeOrNotBe].value_or(false);
    }
  }
}

Config::CastOnCritConfig::CastOnCritConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: cast on crit"sv);
  enable_ = tbl[CastOnCrit][Enable].value_or(false);
  if ((config.weapon_crit().enable() || config.magick_crit().enable()) && enable_) {
    enable_weapon_ = tbl[CastOnCrit][EnableWeapon].value_or(false);
    physical_ = tbl[CastOnCrit][Physical].value_or(false);
    magick_ = tbl[CastOnCrit][Magick].value_or(false);
    magick_cooldawn_ = tbl[CastOnCrit][MagickCritCooldawn].value_or(1.0f);

    const auto form_keywords_form_id = tbl[CastOnCrit][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id = tbl[CastOnCrit][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_ = data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ = data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}

Config::CastOnHitConfig::CastOnHitConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: cast on hit"sv);
  enable_ = tbl[CastOnHit][Enable].value_or(false);
  if (enable_) {
    const auto form_keywords_form_id = tbl[CastOnHit][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id = tbl[CastOnHit][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_ = data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ = data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}

Config::CastOnBlockConfig::CastOnBlockConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: CastOnBlock"sv);
  enable_ = tbl[CastOnBlock][Enable].value_or(false);
  if (enable_) {
    const auto form_keywords_form_id = tbl[CastOnBlock][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id = tbl[CastOnBlock][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_ = data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ = data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}

Config::CastOnBlockParryConfig::CastOnBlockParryConfig(toml::table& tbl,
                                                       RE::TESDataHandler& data_handler,
                                                       const Config& config)
{
  logger::info("config init: CastOnBlockParry"sv);
  enable_ = tbl[CastOnBlockParry][Enable].value_or(false);
  if (enable_) {
    const auto form_keywords_form_id = tbl[CastOnBlockParry][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id = tbl[CastOnBlockParry][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_ = data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ = data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}

Config::CastOnTimingBlockConfig::CastOnTimingBlockConfig(toml::table& tbl,
                                                         RE::TESDataHandler& data_handler,
                                                         const Config& config)
{
  logger::info("config init: CastOnTimingBlock"sv);
  enable_ = tbl[CastOnTimingBlock][Enable].value_or(false);
  if (enable_) {
    const auto form_keywords_form_id = tbl[CastOnTimingBlock][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id = tbl[CastOnTimingBlock][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_ = data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ = data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}

Config::CastOnParryBashConfig::CastOnParryBashConfig(toml::table& tbl,
                                                     RE::TESDataHandler& data_handler,
                                                     const Config& config)
{
  logger::info("config init: CastOnParryBash"sv);
  enable_ = tbl[CastOnParryBash][Enable].value_or(false);
  if (enable_) {
    const auto form_keywords_form_id = tbl[CastOnParryBash][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id = tbl[CastOnParryBash][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_ = data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ = data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}

Config::CastOnKillConfig::CastOnKillConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: CastOnKill"sv);
  enable_ = tbl[CastOnKill][Enable].value_or(false);
  if (enable_) {
    const auto form_keywords_form_id = tbl[CastOnKill][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id = tbl[CastOnKill][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_ = data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ = data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}

Config::ResourceManagerConfig::ResourceManagerConfig(toml::table& tbl,
                                                     RE::TESDataHandler& data_handler,
                                                     const Config& config)
{
  logger::info("config init: resource manager"sv);
  enable_ = tbl[ResourceManager][Enable].value_or(false);
  regen_delay_ = tbl[ResourceManager][RegenDelay].value_or(2.5f);
  if (enable_) {
    infamy_enable_ = tbl[ResourceManager][EnableInfamy].value_or(false);
    regeneration_enable_ = tbl[ResourceManager][EnableRegeneration].value_or(false);
    weapon_spend_enable_ = tbl[ResourceManager][EnableWeaponSpend].value_or(false);
    block_spend_enable_ = tbl[ResourceManager][EnableBlock].value_or(false);
    bash_spend_enable_ = tbl[ResourceManager][EnableBashSpend].value_or(false);

    auto spell_for_block_attack = tbl[ResourceManager][SpellBlockAttackId].value<RE::FormID>();
    auto spell_for_block_power_attack = tbl[ResourceManager][SpellBlockPowerAttackId].value<RE::FormID>();
    auto spell_for_block_bash = tbl[ResourceManager][SpellBlockBashId].value<RE::FormID>();

    spell_block_attack_ = data_handler.LookupForm<RE::SpellItem>(spell_for_block_attack.value(), config.mod_name());
    spell_block_power_attack_ =
        data_handler.LookupForm<RE::SpellItem>(spell_for_block_power_attack.value(), config.mod_name());
    spell_block_bash_ = data_handler.LookupForm<RE::SpellItem>(spell_for_block_bash.value(), config.mod_name());

    attack_cost_av_ = static_cast<RE::ActorValue>(tbl[ResourceManager][ActorValueAttackCostIndex].value_or(120));
    power_attack_cost_av_ =
        static_cast<RE::ActorValue>(tbl[ResourceManager][ActorValuePowerAttackCostIndex].value_or(120));
    attack_cost_high_ = tbl[ResourceManager][AttackCostHigh].value_or(100);
    attack_cost_low_ = tbl[ResourceManager][AttackCostLow].value_or(-100);
    power_attack_cost_high_ = tbl[ResourceManager][PowerAttackCostHigh].value_or(100);
    power_attack_cost_low_ = tbl[ResourceManager][PowerAttackCostLow].value_or(-100);
    weight_mult_ = tbl[ResourceManager][WeightMult].value_or(1.0f);
    global_mult_ = tbl[ResourceManager][GlobalMult].value_or(1.0f);
    jump_cost_ = tbl[ResourceManager][JumpCost].value_or(15.f);
    damage_mult_ = tbl[ResourceManager][DamageMult].value_or(1.f);
    armor_mult_ = tbl[ResourceManager][ArmorMult].value_or(1.f);
    power_attack_mult_ = tbl[ResourceManager][PowerAttackMult].value_or(2.f);

    auto unarmed_weapon_form_id = tbl[ResourceManager][UnarmedWeaponId].value<RE::FormID>();

    auto health_keyword_form_id = tbl[ResourceManager][KeywordHealthId].value<RE::FormID>();
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

    unarmed_weapon_ = data_handler.LookupForm<RE::TESObjectWEAP>(unarmed_weapon_form_id.value(), config.mod_name());

    health_kw_ = data_handler.LookupForm<RE::BGSKeyword>(health_keyword_form_id.value(), config.mod_name());
    stamina_kw_ = data_handler.LookupForm<RE::BGSKeyword>(stamina_keyword_form_id.value(), config.mod_name());
    magicka_kw_ = data_handler.LookupForm<RE::BGSKeyword>(magicka_keyword_form_id.value(), config.mod_name());

    convert_stamina_health_kw_ =
        data_handler.LookupForm<RE::BGSKeyword>(convertion_keyword_stamina_health_form_id.value(), config.mod_name());
    convert_stamina_magicka_kw_ =
        data_handler.LookupForm<RE::BGSKeyword>(convertion_keyword_stamina_magicka_form_id.value(), config.mod_name());
    convert_health_stamina_kw_ =
        data_handler.LookupForm<RE::BGSKeyword>(convertion_keyword_health_stamina_form_id.value(), config.mod_name());
    convert_health_magicka_kw_ =
        data_handler.LookupForm<RE::BGSKeyword>(convertion_keyword_health_magicka_form_id.value(), config.mod_name());
    convert_magicka_stamina_kw_ =
        data_handler.LookupForm<RE::BGSKeyword>(convertion_keyword_magicka_stamina_form_id.value(), config.mod_name());
    convert_magicka_health_kw_ =
        data_handler.LookupForm<RE::BGSKeyword>(convertion_keyword_magicka_health_form_id.value(), config.mod_name());
  }
}

Config::TkDodgeConfig::TkDodgeConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: tk dodge"sv);
  enable_ = tbl[TKDodge][Enable].value_or(false);
  if (enable_) {
    gamepad_treshold_ = 0.15f;
    iframe_duration_ = tbl[TKDodge][iFrameDuration].value_or(0.5f);
    step_ = tbl[TKDodge][StepDodge].value_or(false);
    key_ = tbl[TKDodge][DodgeKey].value_or(277);
    sprint_tapping_dodge_ = tbl[TKDodge][EnableTappingDodge].value_or(false);
    block_dodge_when_attack_ = tbl[TKDodge][BlockDodgeWhenAttack].value_or(false);
    block_dodge_when_power_attack_ = tbl[TKDodge][BlockDodgeWhenPowerAttack].value_or(false);
    block_dodge_when_casting_ = tbl[TKDodge][BlockDodgeWhenCasting].value_or(false);
    key_up_delay_ = tbl[TKDodge][KeyUpDelay].value_or(0.2f);
    equipped_weight_mult_ = tbl[TKDodge][WeightMult].value_or(1.0f);
    flat_cost_ = tbl[TKDodge][FlatCost].value_or(0.f);
    max_cost_ = tbl[TKDodge][MaxCost].value_or(40.f);
    min_cost_ = tbl[TKDodge][MinCost].value_or(10.f);

    const auto health_spend_keyword_form_id = tbl[TKDodge][KeywordHealthId].value<RE::FormID>();
    const auto stamina_spend_keyword_form_id = tbl[TKDodge][KeywordStaminaId].value<RE::FormID>();
    const auto spend_magicka_keyword_form_id = tbl[TKDodge][KeywordMagickaId].value<RE::FormID>();

    const auto block_attack_perk_form_id = tbl[TKDodge][BlockDodgeWhenAttackKeywordId].value<RE::FormID>();
    const auto block_power_attack_perk_form_id = tbl[TKDodge][BlockDodgeWhenPowerAttackKeywordId].value<RE::FormID>();
    const auto block_casting_perk_form_id = tbl[TKDodge][BlockDodgeWhenCastingKeywordId].value<RE::FormID>();
    const auto effect_keyword_iframe_form_id = tbl[TKDodge][iFrameDurationMGEFKeywordId].value<RE::FormID>();
    const auto effect_keyword_flat_cost_form_id = tbl[TKDodge][FlatCostMGEFKeywordId].value<RE::FormID>();

    health_kw_ = data_handler.LookupForm<RE::BGSKeyword>(health_spend_keyword_form_id.value(), config.mod_name());
    stamina_kw_ = data_handler.LookupForm<RE::BGSKeyword>(stamina_spend_keyword_form_id.value(), config.mod_name());
    magicka_kw_ = data_handler.LookupForm<RE::BGSKeyword>(spend_magicka_keyword_form_id.value(), config.mod_name());

    block_dodge_when_attack_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(block_attack_perk_form_id.value(), config.mod_name());
    block_dodge_when_power_attack_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(block_power_attack_perk_form_id.value(), config.mod_name());
    block_dodge_when_casting_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(block_casting_perk_form_id.value(), config.mod_name());
    iframe_duration_mgef_kw_ =
        data_handler.LookupForm<RE::BGSKeyword>(effect_keyword_iframe_form_id.value(), config.mod_name());
    cost_from_mgef_kw_ =
        data_handler.LookupForm<RE::BGSKeyword>(effect_keyword_flat_cost_form_id.value(), config.mod_name());
  }
}

Config::CasterAdditionsConfig::CasterAdditionsConfig(toml::table& tbl,
                                                     RE::TESDataHandler& data_handler,
                                                     const Config& config)
{
  logger::info("config init: caster additions"sv);
  enable_ = tbl[CasterAdditions][Enable].value_or(false);
  if (enable_) {
    const auto spell_add_when_casting_form_id = tbl[CasterAdditions][SpellAddWhileCastingId].value<RE::FormID>();

    spell_add_when_casting_ =
        data_handler.LookupForm<RE::SpellItem>(spell_add_when_casting_form_id.value(), config.mod_name());

    stamina_cost_ = tbl[CasterAdditions][EnableStaminaCost].value_or(false);
    allow_cast_without_stamina_ = tbl[CasterAdditions][AllowCastWithoutStamina].value_or(true);
    stamina_cost_mult_ = tbl[CasterAdditions][StaminaCostMult].value_or(0.25f);
    constexpr RE::FormID mag_fail_form_id = 0x3D0D3;
    constexpr std::string_view skyrim = "Skyrim.esm";
    mag_fail_sound_ = data_handler.LookupForm<RE::BGSSoundDescriptorForm>(mag_fail_form_id, skyrim);
  }
}

Config::MagicWeaponConfig::MagicWeaponConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: magic weapon"sv);
  enable_ = tbl[MagicWeapon][Enable].value_or(false);
  if (enable_) {
    const auto formlist_keywords_form_id = tbl[MagicWeapon][FormListKeywordId].value<RE::FormID>();
    const auto formlist_spells_form_id = tbl[MagicWeapon][FormListSpellsId].value<RE::FormID>();
    const auto formlist_globals_form_id = tbl[MagicWeapon][FormListGlobalsId].value<RE::FormID>();

    keywords_ = data_handler.LookupForm<RE::BGSListForm>(formlist_keywords_form_id.value(), config.mod_name());
    spells_ = data_handler.LookupForm<RE::BGSListForm>(formlist_spells_form_id.value(), config.mod_name());
    globals_ = data_handler.LookupForm<RE::BGSListForm>(formlist_globals_form_id.value(), config.mod_name());
  }
}

Config::ResistTweaksConfig::ResistTweaksConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: resist tweaks"sv);
  enable_ = tbl[ResistTweaks][Enable].value_or(false);
  if (enable_) {
    ench_get_no_absorb_ = tbl[ResistTweaks][EnchGetNoAbsorb].value_or(false);
    enable_check_resistance_ = tbl[ResistTweaks][EnableCheckResistance].value_or(false);
    const auto max_resist_keyword_form_id = tbl[ResistTweaks][MaxResistKeywordId].value<RE::FormID>();
    max_resist_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(max_resist_keyword_form_id.value(), config.mod_name());
    const auto negative_resist_immune_keyword_form_id =
        tbl[ResistTweaks][NegativeResistImmuneKeywordId].value<RE::FormID>();
    negative_resist_immune_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(negative_resist_immune_keyword_form_id.value(), config.mod_name());
    low_ = tbl[ResistTweaks][Low].value_or(-100.f);
    no_double_resist_check_magick_ = tbl[ResistTweaks][NoDoubleResistCheckMagick].value_or(false);
    no_double_resist_check_poison_ = tbl[ResistTweaks][NoDoubleResistCheckPoison].value_or(false);
    npc_max_resistance_ = tbl[ResistTweaks][NpcMaxResistance].value_or(false);
    resist_weight_ = tbl[ResistTweaks][ResistWeight].value_or(0.001f);
    ench_ignore_resistance_ = tbl[ResistTweaks][EnchIgnoreResistance].value_or(false);
    flat_penetration_ = tbl[ResistTweaks][FlatPenetration].value_or(false);
    enable_resist_penetration_ = tbl[ResistTweaks][EnableResistPenetration].value_or(false);
    enable_damage_resist_tweak_ = tbl[ResistTweaks][EnableDamageResistTweak].value_or(false);
    no_double_damage_resist_check_ = tbl[ResistTweaks][NoDoubleDamageResistCheck].value_or(false);
    penetration_high_ = tbl[ResistTweaks][High].value_or(100);
    no_av_damage_resist_penetration_ = tbl[ResistTweaks][NoAvDamageResistPenetration].value_or(false);
    penetration_av_ = static_cast<RE::ActorValue>(tbl[ResistTweaks][ActorValueIndex].value_or(120));

    const auto keyword_form_id = tbl[ResistTweaks][KeywordId].value<RE::FormID>();
    penetration_kw_ = data_handler.LookupForm<RE::BGSKeyword>(keyword_form_id.value(), config.mod_name());
  }
}

Config::TimingBlockConfig::TimingBlockConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: timing block"sv);
  enable_ = tbl[TimingBlock][Enable].value_or(false);
  block_delay_ = tbl[TimingBlock][BlockDelay].value_or(0.30f);
  if (enable_) {
    parry_timing_ = tbl[TimingBlock][ParryTiming].value_or(0.15f);
    block_timing_ = tbl[TimingBlock][BlockTiming].value_or(0.25f);
    enable_sparks_ = tbl[TimingBlock][EnableSparks].value_or(false);
    parry_stagger_count_ = tbl[TimingBlock][ParryStaggerCount].value_or(3);
    parry_stagger_count_timer_ = tbl[TimingBlock][ParryStaggerCountTimer].value_or(10.f);

    const auto blank_activator_form_id = tbl[TimingBlock][BlankActivatorId].value<RE::FormID>();
    blank_activator_ = data_handler.LookupForm<RE::TESObjectACTI>(blank_activator_form_id.value(), config.mod_name());

    const auto spark_form_id = tbl[TimingBlock][SparkId].value<RE::FormID>();
    spark_ = data_handler.LookupForm<RE::Explosion>(spark_form_id.value(), config.mod_name());

    const auto spark_flare_form_id = tbl[TimingBlock][SparkFlareId].value<RE::FormID>();
    spark_flare_ = data_handler.LookupForm<RE::Explosion>(spark_flare_form_id.value(), config.mod_name());

    const auto parry_keyword_form_id = tbl[TimingBlock][ParryKeywordId].value<RE::FormID>();
    parry_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(parry_keyword_form_id.value(), config.mod_name());

    const auto parry_immun_keyword_form_id = tbl[TimingBlock][ParryImmunKeywordId].value<RE::FormID>();
    parry_immun_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(parry_immun_keyword_form_id.value(), config.mod_name());

    const auto parry_timing_keyword_form_id = tbl[TimingBlock][ParryTimingKeywordId].value<RE::FormID>();
    parry_timing_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(parry_timing_keyword_form_id.value(), config.mod_name());

    const auto block_keyword_form_id = tbl[TimingBlock][BlockKeywordId].value<RE::FormID>();
    block_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(block_keyword_form_id.value(), config.mod_name());

    const auto block_immun_keyword_form_id = tbl[TimingBlock][BlockImmunKeywordId].value<RE::FormID>();
    block_immun_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(block_immun_keyword_form_id.value(), config.mod_name());

    const auto block_timing_keyword_form_id = tbl[TimingBlock][BlockTimingKeywordId].value<RE::FormID>();
    block_timing_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(block_timing_keyword_form_id.value(), config.mod_name());

    const auto parry_stagger_count_keyword_form_id = tbl[TimingBlock][ParryStaggerCountKeywordId].value<RE::FormID>();
    parry_stagger_count_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(parry_stagger_count_keyword_form_id.value(), config.mod_name());

    const auto spark_halo_form_id = tbl[TimingBlock][SparkHaloId].value<RE::FormID>();
    spark_halo_ = data_handler.LookupForm<RE::Explosion>(spark_halo_form_id.value(), config.mod_name());

    const auto parry_sound_form_id = tbl[TimingBlock][ParrySoundId].value<RE::FormID>();
    parry_sound_ = data_handler.LookupForm<RE::BGSSoundDescriptorForm>(parry_sound_form_id.value(), config.mod_name());

    const auto block_sound_form_id = tbl[TimingBlock][BlockSoundId].value<RE::FormID>();
    block_sound_ = data_handler.LookupForm<RE::BGSSoundDescriptorForm>(block_sound_form_id.value(), config.mod_name());
  }
}

Config::EquipLoadConfig::EquipLoadConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  enable_ = tbl[EquipLoad][Enable].value_or(false);
  if (enable_) {
    equip_weight_mult_ = tbl[EquipLoad][EquipWeightMult].value_or(1.f);
    armor_weight_mult_ = tbl[EquipLoad][ArmorWeightMult].value_or(1.f);
    weapon_weight_mult_ = tbl[EquipLoad][WeaponWeightMult].value_or(1.f);
    low_percent_ = tbl[EquipLoad][LowPercent].value_or(0.3f);
    high_percent_ = tbl[EquipLoad][HighPercent].value_or(0.7f);

    const auto low_equip_spell_form_id = tbl[EquipLoad][LowEquipAbilityId].value<RE::FormID>();
    const auto med_equip_spell_form_id = tbl[EquipLoad][MidEquipAbilityId].value<RE::FormID>();
    const auto hig_equip_spell_form_id = tbl[EquipLoad][HigEquipAbilityId].value<RE::FormID>();

    low_equip_spell_ = data_handler.LookupForm<RE::SpellItem>(low_equip_spell_form_id.value(), config.mod_name());
    med_equip_spell_ = data_handler.LookupForm<RE::SpellItem>(med_equip_spell_form_id.value(), config.mod_name());
    hig_equip_spell_ = data_handler.LookupForm<RE::SpellItem>(hig_equip_spell_form_id.value(), config.mod_name());
  }
}

Config::ParryBashConfig::ParryBashConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  enable_ = tbl[ParryBash][Enable].value_or(false);
  if (enable_) {
    parry_timing_ = tbl[ParryBash][ParryTiming].value_or(0.18f);
    pre_hit_frame_penalty_ = tbl[ParryBash][PreHitFramePenalty].value_or(0.05f);
    enable_pre_hit_frame_ = tbl[ParryBash][EnablePreHitFrame].value_or(true);
    enable_weapon_swing_ = tbl[ParryBash][EnableWeaponSwing].value_or(true);
    enable_precision_weapon_collide_ = tbl[ParryBash][EnablePrecisionWeaponCollide].value_or(true);

    const auto parry_keyword_form_id = tbl[ParryBash][ParryKeywordId].value<RE::FormID>();
    parry_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(parry_keyword_form_id.value(), config.mod_name());

    const auto parry_immun_keyword_form_id = tbl[ParryBash][ParryImmunKeywordId].value<RE::FormID>();
    parry_immun_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(parry_immun_keyword_form_id.value(), config.mod_name());

    const auto parry_timing_keyword_form_id = tbl[ParryBash][ParryTimingKeywordId].value<RE::FormID>();
    parry_timing_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(parry_timing_keyword_form_id.value(), config.mod_name());

    const auto parry_sound_form_id = tbl[ParryBash][ParrySoundId].value<RE::FormID>();
    parry_sound_ = data_handler.LookupForm<RE::BGSSoundDescriptorForm>(parry_sound_form_id.value(), config.mod_name());
  }
}

Config::ItemLimitConfig::ItemLimitConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: item limit"sv);
  enable_ = tbl[ItemLimit][Enable].value_or(false);
  if (enable_) {
    const auto formlist_keywords_item_form_id = tbl[ItemLimit][FormListKeywordItemId].value<RE::FormID>();
    const auto formlist_keywords_cap_form_id = tbl[ItemLimit][FormListKeywordCapId].value<RE::FormID>();
    const auto formlist_globals_form_id = tbl[ItemLimit][FormListGlobalsId].value<RE::FormID>();
    const auto storage_form_id = tbl[ItemLimit][StorageFormId].value<RE::FormID>();
    const auto keyword_form_id = tbl[ItemLimit][StorageFormId].value<RE::FormID>();
    const auto exclusive_keyword_id = tbl[ItemLimit][ExclusiveKeywordId].value<RE::FormID>();

    const auto enderal_enable = tbl[ItemLimit][EnableEnderal].value_or(false);
    enable_auto_restore_ = tbl[ItemLimit][EnableAutoRestore].value_or(false);
    keywords_cap_mod_ =
        data_handler.LookupForm<RE::BGSListForm>(formlist_keywords_cap_form_id.value(), config.mod_name());
    keywords_item_ =
        data_handler.LookupForm<RE::BGSListForm>(formlist_keywords_item_form_id.value(), config.mod_name());
    globals_ = data_handler.LookupForm<RE::BGSListForm>(formlist_globals_form_id.value(), config.mod_name());
    auto_restore_block_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(keyword_form_id.value(), config.mod_name());
    exclusive_restore_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(exclusive_keyword_id.value(), config.mod_name());
    storage_ = data_handler.LookupForm<RE::TESObjectREFR>(storage_form_id.value(),
                                                          enderal_enable ? "Skyrim.esm" : config.mod_name());
  }
}

Config::DeathLootConfig::DeathLootConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: death loot"sv);
  enable_ = tbl[DeathLoot][Enable].value_or(false);
  if (enable_) {
    gold_value_mult_ = tbl[DeathLoot][GoldMult].value_or(0.2f);
    auto_loot_ = tbl[DeathLoot][AutoLoot].value_or(false);

    const auto exclusive_keyword_id = tbl[DeathLoot][ExclusiveKeywordId].value<RE::FormID>();

    exclusive_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(exclusive_keyword_id.value(), config.mod_name());
  }
}

Config::SoulLinkConfig::SoulLinkConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: soul link"sv);
  enable_ = tbl[SoulLink][Enable].value_or(false);
  if (enable_) {
    av_ = static_cast<RE::ActorValue>(tbl[SoulLink][ActorValueIndex].value_or(120));

    physic_ = tbl[SoulLink][Physical].value_or(false);
    magick_ = tbl[SoulLink][Magick].value_or(false);
    const auto exclusive_keyword_id = tbl[SoulLink][ExclusiveKeywordId].value<RE::FormID>();
    const auto summon_split_keyword_id = tbl[SoulLink][SummonSplitKeywordId].value<RE::FormID>();

    exclusive_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(exclusive_keyword_id.value(), config.mod_name());
    summons_split_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(summon_split_keyword_id.value(), config.mod_name());
  }
}

Config::PotionsDrinkLimitConfig::PotionsDrinkLimitConfig(toml::table& tbl,
                                                         RE::TESDataHandler& data_handler,
                                                         const Config& config)
{
  logger::info("config init: potions drink limit"sv);
  enable_ = tbl[PotionsDrinkLimit][Enable].value_or(false);
  if (enable_) {
    other_enable_ = tbl[PotionsDrinkLimit][OtherEnable].value_or(false);
    other_cap_base_ = tbl[PotionsDrinkLimit][OtherCapBase].value_or(7);
    other_duration_base_ = tbl[PotionsDrinkLimit][OtherDurationBase].value_or(15.f);
    notify_ = tbl[PotionsDrinkLimit][Notify].value_or("Я не могу больше выпить зелье."s);
    other_revert_exclusive_ = tbl[PotionsDrinkLimit][OtherRevertExclusive].value_or(false);

    const auto other_exclusive_keyword_id = tbl[PotionsDrinkLimit][OtherExclusiveKeywordId].value<RE::FormID>();
    const auto other_cap_keyword_id = tbl[PotionsDrinkLimit][OtherCapKeywordId].value<RE::FormID>();
    const auto other_duration_keyword_id = tbl[PotionsDrinkLimit][OtherDurationKeywordId].value<RE::FormID>();
    const auto no_remove_keyword_id = tbl[PotionsDrinkLimit][NoRemoveKeywordId].value<RE::FormID>();

    other_exclusive_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(other_exclusive_keyword_id.value(), config.mod_name());
    other_cap_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(other_cap_keyword_id.value(), config.mod_name());
    other_duration_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(other_duration_keyword_id.value(), config.mod_name());
    no_remove_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(no_remove_keyword_id.value(), config.mod_name());

    health_enable_ = tbl[PotionsDrinkLimit][HealthEnable].value_or(false);
    health_duration_base_ = tbl[PotionsDrinkLimit][HealthDurationBase].value_or(15.f);
    health_cap_base_ = tbl[PotionsDrinkLimit][HealthCapBase].value_or(7);
    
    const auto health_cap_keyword_id = tbl[PotionsDrinkLimit][HealthCapKeywordId].value<RE::FormID>();
    const auto health_duration_keyword_id = tbl[PotionsDrinkLimit][HealthDurationKeywordId].value<RE::FormID>();
    const auto health_keyword_id = tbl[PotionsDrinkLimit][HealthKeywordId].value<RE::FormID>();

    health_cap_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(health_cap_keyword_id.value(), config.mod_name());
    health_duration_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(health_duration_keyword_id.value(), config.mod_name());
    health_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(health_keyword_id.value(), config.mod_name());
    
    stamina_enable_ = tbl[PotionsDrinkLimit][StaminaEnable].value_or(false);
    stamina_duration_base_ = tbl[PotionsDrinkLimit][StaminaDurationBase].value_or(15.f);
    stamina_cap_base_ = tbl[PotionsDrinkLimit][StaminaCapBase].value_or(7);
    
    const auto stamina_cap_keyword_id = tbl[PotionsDrinkLimit][StaminaCapKeywordId].value<RE::FormID>();
    const auto stamina_duration_keyword_id = tbl[PotionsDrinkLimit][StaminaDurationKeywordId].value<RE::FormID>();
    const auto stamina_keyword_id_ = tbl[PotionsDrinkLimit][StaminaKeywordId].value<RE::FormID>();

    stamina_cap_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(stamina_cap_keyword_id.value(), config.mod_name());
    stamina_duration_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(stamina_duration_keyword_id.value(), config.mod_name());
    stamina_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(stamina_keyword_id_.value(), config.mod_name());
    
    magicka_enable_ = tbl[PotionsDrinkLimit][MagickaEnable].value_or(false);
    magicka_duration_base_ = tbl[PotionsDrinkLimit][MagickaDurationBase].value_or(15.f);
    magicka_cap_base_ = tbl[PotionsDrinkLimit][MagickaCapBase].value_or(7);
    
    const auto magicka_cap_keyword_id = tbl[PotionsDrinkLimit][MagickaCapKeywordId].value<RE::FormID>();
    const auto magicka_duration_keyword_id = tbl[PotionsDrinkLimit][MagickaDurationKeywordId].value<RE::FormID>();
    const auto magicka_keyword_id = tbl[PotionsDrinkLimit][MagickaKeywordId].value<RE::FormID>();

    magicka_cap_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(magicka_cap_keyword_id.value(), config.mod_name());
    magicka_duration_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(magicka_duration_keyword_id.value(), config.mod_name());
    magicka_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(magicka_keyword_id.value(), config.mod_name());
  }
}
Config::UnblockableAttackConfig::UnblockableAttackConfig(toml::table& tbl,
                                                         RE::TESDataHandler& data_handler,
                                                         const Config& config)
{
  logger::info("config init: UnblockableAttack"sv);
  enable_ = tbl[UnblockableAttack][Enable].value_or(false);
  if (enable_) {
    const auto absolute_unblock_form_id = tbl[UnblockableAttack][AbsoluteUnblockKeywordId].value<RE::FormID>();
    const auto just_unblock_form_id = tbl[UnblockableAttack][JustUnblockKeywordId].value<RE::FormID>();
    const auto timing_unblock_form_id = tbl[UnblockableAttack][TimingUnblockKeywordId].value<RE::FormID>();

    absolute_unblock_ = data_handler.LookupForm<RE::BGSKeyword>(absolute_unblock_form_id.value(), config.mod_name());
    just_block_unblock_ = data_handler.LookupForm<RE::BGSKeyword>(just_unblock_form_id.value(), config.mod_name());
    timing_block_unblock_ = data_handler.LookupForm<RE::BGSKeyword>(timing_unblock_form_id.value(), config.mod_name());
  }
}
Config::SpeedMultCapConfig::SpeedMultCapConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: SpeedMultCap"sv);
  enable_ = tbl[SpeedMultCap][Enable].value_or(false);
  if (enable_) {
    const auto allow_keyword_id = tbl[SpeedMultCap][EffectAllowOvercapKeyword].value<RE::FormID>();
    const auto cap_mutate_keyword_id = tbl[SpeedMultCap][EffectMutateCapKeyword].value<RE::FormID>();
    const auto exclusive_keyword_id = tbl[SpeedMultCap][ExclusiveKeywordId].value<RE::FormID>();

    cap_base_ = tbl[SpeedMultCap][OtherCapBase].value_or(100.f);
    effect_allow_overcap_ = data_handler.LookupForm<RE::BGSKeyword>(allow_keyword_id.value(), config.mod_name());
    effect_mutate_cap_ = data_handler.LookupForm<RE::BGSKeyword>(cap_mutate_keyword_id.value(), config.mod_name());
    exclusive_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(exclusive_keyword_id.value(), config.mod_name());
  }
}
Config::ProjectileBlockConfig::ProjectileBlockConfig(toml::table& tbl,
                                                     RE::TESDataHandler& data_handler,
                                                     const Config& config)
{
  logger::info("config init: ProjectileBlock"sv);
  enable_ = tbl[ProjectileBlock][Enable].value_or(false);
  if (enable_) {
    enable_block_ = tbl[ProjectileBlock][EnableBlock].value_or(false);
    enable_timing_block_ = tbl[ProjectileBlock][EnableTimingBlock].value_or(false);
    block_timing_ = tbl[ProjectileBlock][BlockTiming].value_or(0.25f);
    base_flat_cost_ = tbl[ProjectileBlock][BaseFlatCost].value_or(10.f);
    base_percent_cost_ = tbl[ProjectileBlock][BasePercentCost].value_or(5.f);
    enable_block_magick_for_magick_ = tbl[ProjectileBlock][EnableBlockMagickForMagick].value_or(false);
    enable_arrow_ = tbl[ProjectileBlock][EnableArrow].value_or(false);
    enable_missile_ = tbl[ProjectileBlock][EnableMissile].value_or(false);
    enable_cone_ = tbl[ProjectileBlock][EnableCone].value_or(false);
    enable_flame_ = tbl[ProjectileBlock][EnableFlame].value_or(false);
    enable_beam_ = tbl[ProjectileBlock][EnableBeam].value_or(false);
    enable_sparks_ = tbl[ProjectileBlock][EnableSparks].value_or(false);
    enable_sound_ = tbl[ProjectileBlock][EnableSound].value_or(false);

    const auto enable_block_keyword_id = tbl[ProjectileBlock][EnableBlockKeywordId].value<RE::FormID>();
    enable_block_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(enable_block_keyword_id.value(), config.mod_name());

    const auto enable_timing_block_keyword_id = tbl[ProjectileBlock][EnableTimingBlockKeywordId].value<RE::FormID>();
    enable_timing_block_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(enable_timing_block_keyword_id.value(), config.mod_name());

    const auto timing_block_timer_keyword_id = tbl[ProjectileBlock][TimingBlockTimerKeywordId].value<RE::FormID>();
    timing_block_timer_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(timing_block_timer_keyword_id.value(), config.mod_name());

    const auto block_cost_keyword_id = tbl[ProjectileBlock][BlockCostKeywordId].value<RE::FormID>();
    block_cost_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(block_cost_keyword_id.value(), config.mod_name());

    const auto enable_arrow_block_keyword_id = tbl[ProjectileBlock][EnableArrowBlockKeywordId].value<RE::FormID>();
    enable_arrow_block_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(enable_arrow_block_keyword_id.value(), config.mod_name());

    const auto enable_magick_block_keyword_id = tbl[ProjectileBlock][EnableMagickBlockKeywordId].value<RE::FormID>();
    enable_magick_block_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(enable_magick_block_keyword_id.value(), config.mod_name());

    const auto enable_arrow_timing_block_keyword_id =
        tbl[ProjectileBlock][EnableArrowTimingBlockKeywordId].value<RE::FormID>();
    enable_arrow_timing_block_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(enable_arrow_timing_block_keyword_id.value(), config.mod_name());

    const auto enable_magick_timing_block_keyword_id =
        tbl[ProjectileBlock][EnableMagickTimingBlockKeywordId].value<RE::FormID>();
    enable_magick_timing_block_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(enable_magick_timing_block_keyword_id.value(), config.mod_name());

    const auto exclusive_keyword_id = tbl[ProjectileBlock][ExclusiveKeywordId].value<RE::FormID>();
    exclusive_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(exclusive_keyword_id.value(), config.mod_name());

    const auto exclusive_timing_keyword_id = tbl[ProjectileBlock][ExclusiveTimingKeywordId].value<RE::FormID>();
    exclusive_timing_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(exclusive_timing_keyword_id.value(), config.mod_name());

    const auto block_sound_form_id = tbl[ProjectileBlock][BlockSoundId].value<RE::FormID>();
    block_sound_ = data_handler.LookupForm<RE::BGSSoundDescriptorForm>(block_sound_form_id.value(), config.mod_name());

    const auto blank_activator_form_id = tbl[ProjectileBlock][BlankActivatorId].value<RE::FormID>();
    blank_activator_ = data_handler.LookupForm<RE::TESObjectACTI>(blank_activator_form_id.value(), config.mod_name());

    const auto spark_form_id = tbl[ProjectileBlock][SparkId].value<RE::FormID>();
    spark_ = data_handler.LookupForm<RE::Explosion>(spark_form_id.value(), config.mod_name());

    const auto spark_flare_form_id = tbl[ProjectileBlock][SparkFlareId].value<RE::FormID>();
    spark_flare_ = data_handler.LookupForm<RE::Explosion>(spark_flare_form_id.value(), config.mod_name());

    const auto spark_halo_form_id = tbl[ProjectileBlock][SparkHaloId].value<RE::FormID>();
    spark_halo_ = data_handler.LookupForm<RE::Explosion>(spark_halo_form_id.value(), config.mod_name());
   
  }
}

Config::CastOnProjectileBlockConfig::CastOnProjectileBlockConfig(toml::table& tbl,
                                                                 RE::TESDataHandler& data_handler,
                                                                 const Config& config)
{
  logger::info("config init: CastOnProjectileBlockConfig"sv);
  enable_ = tbl[CastOnProjectileBlock][Enable].value_or(false);
  if (enable_) {
    const auto form_keywords_form_id = tbl[CastOnProjectileBlock][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id = tbl[CastOnProjectileBlock][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_ = data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ = data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}

Config::CastOnProjectileTimingBlockConfig::CastOnProjectileTimingBlockConfig(toml::table& tbl,
                                                                             RE::TESDataHandler& data_handler,
                                                                             const Config& config)
{
  logger::info("config init: CastOnProjectileTimingBlockConfig"sv);
  enable_ = tbl[CastOnProjectileTimingBlock][Enable].value_or(false);
  if (enable_) {
    const auto form_keywords_form_id = tbl[CastOnProjectileTimingBlock][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id = tbl[CastOnProjectileTimingBlock][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_ = data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ = data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}

Config::MiscFixesConfig::MiscFixesConfig(toml::table& tbl, RE::TESDataHandler&, const Config&)
{
  logger::info("config init: MiscFixesConfig"sv);
  regeneration_fix_ = tbl[MiscFixes][RegenerationFix].value_or(false);
  equip_bound_fix_ = tbl[MiscFixes][EquipBoundFix].value_or(false);
  disable_sprint_cost_in_combat_ = tbl[MiscFixes][DisableSprintCostInCombat].value_or(false);
  disable_over_encumbered_ = tbl[MiscFixes][DisableOverEncumbered].value_or(false);
  negative_speed_mult_fix_ = tbl[MiscFixes][NegativeSpeedMultFix].value_or(false);
  no_sneak_requirement_for_sneak_attack_ = tbl[MiscFixes][NoSneakRequirementForSneakAttack].value_or(false);
}
Config::PoisonReworkConfig::PoisonReworkConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: PoisonRework"sv);
  enable_ = tbl[PoisonRework][Enable].value_or(false);
  if (enable_) {
    notify_ = tbl[PoisonRework][Notify].value_or("Я не могу сейчас применить яд."s);
    cap_base_ = tbl[PoisonRework][CapBase].value_or(3);
    duration_base_ = tbl[PoisonRework][DurationBase].value_or(30.f);
    revert_exclusive_ = tbl[PoisonRework][RevertExclusive].value_or(false);
    apply_on_both_weapon_ = tbl[PoisonRework][ApplyOnBothWeapon].value_or(false);

    let duration_keyword_id = tbl[PoisonRework][DurationKeywordId].value<RE::FormID>();
    let cap_keyword_id = tbl[PoisonRework][CapKeywordId].value<RE::FormID>();
    let exclusive_keyword_id = tbl[PoisonRework][ExclusiveKeywordId].value<RE::FormID>();
    let no_remove_keyword_id = tbl[PoisonRework][NoRemoveKeywordId].value<RE::FormID>();

    duration_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(duration_keyword_id.value(), config.mod_name());
    cap_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(cap_keyword_id.value(), config.mod_name());
    exclusive_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(exclusive_keyword_id.value(), config.mod_name());
    no_remove_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(no_remove_keyword_id.value(), config.mod_name());
  }
}

Config::MagickToStaminaConfig::MagickToStaminaConfig(toml::table& tbl,
                                                     RE::TESDataHandler&,
                                                     const Config&)
{
  logger::info("config init: MagickToStamina"sv);
  enable_ = tbl[MagickToStamina][Enable].value_or(false);
}

auto Config::load() -> void
{
  logger::info("start load config"sv);
  logger::info("start parse toml"sv);
  auto tbl = toml::parse_file(PathToConfig);
  const auto data_handler = RE::TESDataHandler::GetSingleton();

  mod_name_ = tbl[Reflyem][ModName].value_or("Skyrim.esm"sv);
  magic_shield_ = MagicShieldConfig{tbl, *data_handler, *this};
  stamina_shield_ = StaminaShieldConfig{tbl, *data_handler, *this};
  petrified_blood_ = PetrifiedBloodConfig{tbl, *data_handler, *this};
  cheat_death_ = CheatDeathConfig{tbl, *data_handler, *this};
  vampirism_ = VampirismConfig{tbl, *data_handler, *this};
  magic_vampirism_ = MagicVampirismConfig{tbl, *data_handler, *this};
  speed_casting_ = SpeedCastingConfig{tbl, *data_handler, *this};
  weapon_crit_ = WeaponCritConfig{tbl, *data_handler, *this};
  magick_crit_ = MagickCritConfig{tbl, *data_handler, *this};
  cast_on_crit_ = CastOnCritConfig{tbl, *data_handler, *this};
  cast_on_hit_ = CastOnHitConfig{tbl, *data_handler, *this};
  cast_on_block_ = CastOnBlockConfig{tbl, *data_handler, *this};
  cast_on_block_parry_ = CastOnBlockParryConfig{tbl, *data_handler, *this};
  cast_on_timing_block_ = CastOnTimingBlockConfig{tbl, *data_handler, *this};
  cast_on_parry_bash_ = CastOnParryBashConfig{tbl, *data_handler, *this};
  cast_on_kill_ = CastOnKillConfig{tbl, *data_handler, *this};
  resource_manager_ = ResourceManagerConfig{tbl, *data_handler, *this};
  tk_dodge_ = TkDodgeConfig{tbl, *data_handler, *this};
  caster_additions_ = CasterAdditionsConfig{tbl, *data_handler, *this};
  magic_weapon_ = MagicWeaponConfig{tbl, *data_handler, *this};
  resist_tweaks_ = ResistTweaksConfig{tbl, *data_handler, *this};
  timing_block_ = TimingBlockConfig{tbl, *data_handler, *this};
  equip_load_ = EquipLoadConfig{tbl, *data_handler, *this};
  parry_bash_ = ParryBashConfig{tbl, *data_handler, *this};
  item_limit_ = ItemLimitConfig{tbl, *data_handler, *this};
  death_loot_ = DeathLootConfig{tbl, *data_handler, *this};
  soul_link_ = SoulLinkConfig{tbl, *data_handler, *this};
  unblockable_attack_ = UnblockableAttackConfig(tbl, *data_handler, *this);
  misc_fixes_ = MiscFixesConfig(tbl, *data_handler, *this);
  potions_drink_limit_ = PotionsDrinkLimitConfig{tbl, *data_handler, *this};
  speed_mult_cap_ = SpeedMultCapConfig{tbl, *data_handler, *this};
  projectile_block_ = ProjectileBlockConfig{tbl, *data_handler, *this};
  cast_on_projectile_block_ = CastOnProjectileBlockConfig{tbl, *data_handler, *this};
  poison_rework_ = PoisonReworkConfig{tbl, *data_handler, *this};
  cast_on_projectile_timing_block_ = CastOnProjectileTimingBlockConfig{tbl, *data_handler, *this};
  magicka_to_stamina_ = MagickToStaminaConfig{tbl, *data_handler, *this};

  logger::info("finish load config"sv);
}
Config::Config()
{
  logger::info("start init config toml"sv);
  load();
  logger::info("finish init config"sv);
}

[[nodiscard]] auto Config::get_singleton() noexcept -> Config&
{
  static Config instance;
  return instance;
}
} // namespace Reflyem
