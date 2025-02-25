// ReSharper disable CppInconsistentNaming
#include "Config.hpp"

namespace Reflyem {

#define get_tes_data(form_name, form_id, mod_name)                                                                     \
  [&]() -> form_name* {                                                                                                \
    let data_handler = RE::TESDataHandler::GetSingleton();                                                             \
    if (!data_handler) {                                                                                               \
      logi("Data handler is null"sv);                                                                                  \
      return nullptr;                                                                                                  \
    }                                                                                                                  \
    return data_handler->LookupForm<form_name>(form_id, mod_name);                                                     \
  }()

constexpr inline std::string_view PathToConfig = R"(Data\SKSE\Plugins\Reflyem.toml)";
constexpr inline std::string_view Reflyem = "Reflyem";
constexpr inline std::string_view MiscFixes = "MiscFixes";
constexpr inline std::string_view RegenerationFix = "RegenerationFix";
constexpr inline std::string_view EnableUnlimitedMerchantGold = "EnableUnlimitedMerchantGold";
constexpr inline std::string_view EnableBladeAndBluntTrueHudSpecialBar = "EnableBladeAndBluntTrueHudSpecialBar";
constexpr inline std::string_view EnableEnchantsIgnoreAbsorb = "EnableEnchantsIgnoreAbsorb";
constexpr inline std::string_view EnableNpcAlwaysSpendStaminaOnAttack = "EnableNpcAlwaysSpendStaminaOnAttack";
constexpr inline std::string_view CastAbilityMagickaCost = "CastAbilityMagickaCost";
constexpr inline std::string_view DisplayKeyName = "DisplayKeyName";
constexpr inline std::string_view StartAttackDuration = "StartAttackDuration";
constexpr inline std::string_view VolumeOverride = "VolumeOverride";
constexpr inline std::string_view EnableInstantRestoreStaminaNPC = "EnableInstantRestoreStaminaNPC";
constexpr inline std::string_view EquipBoundFix = "EquipBoundFix";
constexpr inline std::string_view DisableSprintCostInCombat = "DisableSprintCostInCombat";
constexpr inline std::string_view DisableOverEncumbered = "DisableOverEncumbered";
constexpr inline std::string_view NegativeSpeedMultFix = "NegativeSpeedMultFix";
constexpr inline std::string_view NoSneakRequirementForSneakAttack = "NoSneakRequirementForSneakAttack";
constexpr inline std::string_view SpeedMultCap = "SpeedMultCap";
constexpr inline std::string_view KiEnergyPower = "KiEnergyPower";
constexpr inline std::string_view ProjectileBlock = "ProjectileBlock";
constexpr inline std::string_view EffectAllowOvercapKeyword = "EffectAllowOvercapKeyword";
constexpr inline std::string_view PoisonRework = "PoisonRework";
constexpr inline std::string_view MagickToStamina = "MagickToStamina";
constexpr inline std::string_view SlowTime = "SlowTime";
constexpr inline std::string_view DeclutteredLoot = "DeclutteredLoot";
constexpr inline std::string_view HideFood = "HideFood";
constexpr inline std::string_view HideMisc = "HideMisc";
constexpr inline std::string_view HideIngredient = "HideIngredient";
constexpr inline std::string_view EnableGoldWeightThreshold = "EnableGoldWeightThreshold";
constexpr inline std::string_view GoldWeightThreshold = "GoldWeightThreshold";
constexpr inline std::string_view EnableGoldThreshold = "EnableGoldThreshold";
constexpr inline std::string_view GoldThreshold = "GoldThreshold";
constexpr inline std::string_view EnableOnBlock = "EnableOnBlock";
constexpr inline std::string_view CastAbilityOnEquip = "CastAbilityOnEquip";
constexpr inline std::string_view CastAbilityOnEquipInstant = "CastAbilityOnEquipInstant";
constexpr inline std::string_view CastShoutOnEquip = "CastShoutOnEquip";
constexpr inline std::string_view EnableOnTimingBlock = "EnableOnTimingBlock";
constexpr inline std::string_view EnableOnParryBlock = "EnableOnParryBlock";
constexpr inline std::string_view EnableOnParryBash = "EnableOnParryBash";
constexpr inline std::string_view EnableOnKill = "EnableOnKill";
constexpr inline std::string_view EnableOnProjectileBlock = "EnableOnProjectileBlock";
constexpr inline std::string_view EnableOnProjectileTimingBlock = "EnableOnProjectileTimingBlock";
constexpr inline std::string_view EnableOnWeaponCrit = "EnableOnWeaponCrit";
constexpr inline std::string_view EnableOnMagickCrit = "EnableOnMagickCrit";
constexpr inline std::string_view EnableOnVanillaWeaponCrit = "EnableOnVanillaWeaponCrit";
constexpr inline std::string_view EnableOnSneakAttack = "EnableOnSneakAttack";
constexpr inline std::string_view SlowDuration = "SlowDuration";
constexpr inline std::string_view SlowPower = "SlowPower";
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
constexpr inline std::string_view HitTimer = "HitTimer";
constexpr inline std::string_view SpellWhenLootId = "SpellWhenLootId";
constexpr inline std::string_view OtherExclusiveKeywordId = "OtherExclusiveKeywordId";
constexpr inline std::string_view HealthEnable = "HealthEnable";
constexpr inline std::string_view OtherEnable = "OtherEnable";
constexpr inline std::string_view EnableOneCooldown = "EnableOneCooldown";
constexpr inline std::string_view FormListReservationKeywordsId = "FormListReservationKeywordsId";
constexpr inline std::string_view FormListReservationSpellsId = "FormListReservationSpellsId";
constexpr inline std::string_view FormListReservationSpellsIdNoCondition = "FormListReservationSpellsIdNoCondition";
constexpr inline std::string_view EnableReservationAdjustCost = "EnableReservationAdjustCost";
constexpr inline std::string_view EnableKiSummons = "EnableKiSummons";
constexpr inline std::string_view KiSummonsFormListId = "KiSummonsFormListId";
constexpr inline std::string_view EnableReservation = "EnableReservation";
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
constexpr inline std::string_view SpecialTechniques = "SpecialTechniques";
constexpr inline std::string_view Enable360PlayerBlock = "Enable360PlayerBlock";
constexpr inline std::string_view Enable360Block = "Enable360Block";
constexpr inline std::string_view EnableMcoRecovery = "EnableMcoRecovery";
constexpr inline std::string_view PercentCost = "PercentCost";
constexpr inline std::string_view EnableInterruptCastWhenDodge = "EnableInterruptCastWhenDodge";
constexpr inline std::string_view EnableDodgeInStaggerKeywordId = "EnableDodgeInStaggerKeywordId";
constexpr inline std::string_view StaggerCostPenaltyMult = "StaggerCostPenaltyMult";
constexpr inline std::string_view SlowAfterBlockHitDuration = "SlowAfterBlockHitDuration";
constexpr inline std::string_view SlowAfterBlockHitMult = "SlowAfterBlockHitMult";
constexpr inline std::string_view NormalizePowerAttackMultPC = "NormalizePowerAttackMultPC";
constexpr inline std::string_view DynamicRegenerationHealth = "DynamicRegenerationHealth";
constexpr inline std::string_view DynamicRegenerationMagicka = "DynamicRegenerationMagicka";
constexpr inline std::string_view DynamicRegenerationStamina = "DynamicRegenerationStamina";
constexpr inline std::string_view NormalizePowerAttackMultNPC = "NormalizePowerAttackMultNPC";
constexpr inline std::string_view NormalizeNormalAttackMultPC = "NormalizeNormalAttackMultPC";
constexpr inline std::string_view NormalizeNormalAttackMultNPC = "NormalizeNormalAttackMultNPC";
constexpr inline std::string_view NormalizeRegenDelayHealth = "NormalizeRegenDelayHealth";
constexpr inline std::string_view NormalizeRegenDelayMagicka = "NormalizeRegenDelayMagicka";
constexpr inline std::string_view NormalizeRegenDelayStamina = "NormalizeRegenDelayStamina";
constexpr inline std::string_view EnableForNpc = "EnableForNpc";
constexpr inline std::string_view CapBase = "CapBase";
constexpr inline std::string_view DurationBase = "DurationBase";
constexpr inline std::string_view CapKeywordId = "CapKeywordId";
constexpr inline std::string_view DurationKeywordId = "DurationKeywordId";
constexpr inline std::string_view ApplyOnBothWeapon = "ApplyOnBothWeapon";
constexpr inline std::string_view ExclusiveKeywordId = "ExclusiveKeywordId";
constexpr inline std::string_view LootArrow = "LootArrow";
constexpr inline std::string_view LootBosses = "LootBosses";
constexpr inline std::string_view LootUniques = "LootUniques";
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
constexpr inline std::string_view EnableAv = "EnableAv";
constexpr inline std::string_view StaminaShieldValueKeywordId = "StaminaShieldValueKeywordId";
constexpr inline std::string_view Vampirism = "Vampirism";
constexpr inline std::string_view MagicVampirism = "MagicVampirism";
constexpr inline std::string_view PetrifiedBlood = "PetrifiedBlood";
constexpr inline std::string_view SpellId = "SpellId";
constexpr inline std::string_view EnableDodgeWhenNotEnoughResource = "EnableDodgeWhenNotEnoughResource";
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
constexpr inline std::string_view FormListMeleeSpellsId = "FormListMeleeSpellsId";
constexpr inline std::string_view FormListMeleeKeywordId = "FormListMeleeKeywordId";
constexpr inline std::string_view FormListBashSpellsId = "FormListBashSpellsId";
constexpr inline std::string_view FormListBashKeywordId = "FormListBashKeywordId";
constexpr inline std::string_view FormListRangedSpellsId = "FormListRangedSpellsId";
constexpr inline std::string_view CastOnMagicProjectileHit = "CastOnMagicProjectileHit";
constexpr inline std::string_view CastOnMagicProjectileGetHit = "CastOnMagicProjectileGetHit";
constexpr inline std::string_view CastOnTimingBlockStagger = "CastOnTimingBlockStagger";
constexpr inline std::string_view FormListRangedKeywordId = "FormListRangedKeywordId";
constexpr inline std::string_view AbsorbShield = "AbsorbShield";
constexpr inline std::string_view AbsoluteMaxShieldValue = "AbsoluteMaxShieldValue";
constexpr inline std::string_view BaseShieldValue = "BaseShieldValue";
constexpr inline std::string_view BaseMultShieldValueFromMaxHealth = "BaseMultShieldValueFromMaxHealth";
constexpr inline std::string_view BaseFromCastCostFlat = "BaseFromCastCostFlat";
constexpr inline std::string_view BaseFromCastCostPercent = "BaseFromCastCostPercent";
constexpr inline std::string_view BaseShieldValueDrainThresholdFlat = "BaseShieldValueDrainThresholdFlat";
constexpr inline std::string_view BaseShieldValueDrainThresholdPercent = "BaseShieldValueDrainThresholdPercent";
constexpr inline std::string_view AbsoluteMaxShieldValueDrainThreshold = "AbsoluteMaxShieldValueDrainThreshold";
constexpr inline std::string_view AbsoluteMaxShieldValueDrain = "AbsoluteMaxShieldValueDrain";
constexpr inline std::string_view BaseMultShieldValueDrainThresholdFromMaxHealth =
    "BaseMultShieldValueDrainThresholdFromMaxHealth";
constexpr inline std::string_view BaseMultShieldValueDrainThresholdFromMaxStamina =
    "BaseMultShieldValueDrainThresholdFromMaxStamina";
constexpr inline std::string_view BaseMultShieldValueDrainThresholdFromMaxMagicka =
    "BaseMultShieldValueDrainThresholdFromMaxMagicka";
constexpr inline std::string_view BaseMultShieldValueDrainThresholdFromArmor =
    "BaseMultShieldValueDrainThresholdFromArmor";
constexpr inline std::string_view KeywordShieldValueDrainThresholdEnable = "KeywordShieldValueDrainThresholdEnable";
constexpr inline std::string_view KeywordShieldValueDrainThresholdFlat = "KeywordShieldValueDrainThresholdFlat";
constexpr inline std::string_view KeywordShieldValueDrainThresholdPercent = "KeywordShieldValueDrainThresholdPercent";
constexpr inline std::string_view KeywordShieldValueDrainThresholdMult = "KeywordShieldValueDrainThresholdMult";
constexpr inline std::string_view KeywordMultShieldValueDrainThresholdFromMaxHealth =
    "KeywordMultShieldValueDrainThresholdFromMaxHealth";
constexpr inline std::string_view KeywordMultShieldValueDrainThresholdFromMaxStamina =
    "KeywordMultShieldValueDrainThresholdFromMaxStamina";
constexpr inline std::string_view KeywordMultShieldValueDrainThresholdFromMaxMagicka =
    "KeywordMultShieldValueDrainThresholdFromMaxMagicka";
constexpr inline std::string_view KeywordMultShieldValueDrainThresholdFromArmor =
    "KeywordMultShieldValueDrainThresholdFromArmor";
constexpr inline std::string_view KeywordShieldValueFromCastCostEnable = "KeywordShieldValueFromCastCostEnable";
constexpr inline std::string_view KeywordFromCastCostFlat = "KeywordFromCastCostFlat";
constexpr inline std::string_view KeywordFromCastCostPercent = "KeywordFromCastCostPercent";
constexpr inline std::string_view KeywordFromCastCostMult = "KeywordFromCastCostMult";
constexpr inline std::string_view EnableVanillaBashRanged = "EnableVanillaBashRanged";
constexpr inline std::string_view BaseMultShieldValueFromMaxStamina = "BaseMultShieldValueFromMaxStamina";
constexpr inline std::string_view KeywordShieldValueDegenDelayAfterWeaponHitEnable =
    "KeywordShieldValueDegenDelayAfterWeaponHitEnable";
constexpr inline std::string_view KeywordShieldValueDegenDelayAfterMagickHitEnable =
    "KeywordShieldValueDegenDelayAfterMagickHitEnable";
constexpr inline std::string_view KeywordShieldValueDegenDelayAfterWeaponHitFlat =
    "KeywordShieldValueDegenDelayAfterWeaponHitFlat";
constexpr inline std::string_view KeywordShieldValueDegenDelayAfterWeaponHitMult =
    "KeywordShieldValueDegenDelayAfterWeaponHitMult";
constexpr inline std::string_view KeywordShieldValueDegenDelayAfterMagickHitFlat =
    "KeywordShieldValueDegenDelayAfterMagickHitFlat";
constexpr inline std::string_view KeywordShieldValueDegenDelayAfterMagickHitMult =
    "KeywordShieldValueDegenDelayAfterMagickHitMult";
constexpr inline std::string_view BaseShieldValueDegenDelayAfterWeaponHitFlat =
    "BaseShieldValueDegenDelayAfterWeaponHitFlat";
constexpr inline std::string_view BaseShieldValueDegenDelayAfterMagickHitFlat =
    "BaseShieldValueDegenDelayAfterMagickHitFlat";
constexpr inline std::string_view BaseMultShieldValueFromMaxMagicka = "BaseMultShieldValueFromMaxMagicka";
constexpr inline std::string_view BaseShieldValueDegenDelayAfterFromCastCostHitFlat =
    "BaseShieldValueDegenDelayAfterFromCastCostHitFlat";
constexpr inline std::string_view KeywordShieldValueDegenDelayAfterFromCastCostHitEnable =
    "KeywordShieldValueDegenDelayAfterFromCastCostHitEnable";
constexpr inline std::string_view KeywordShieldValueDegenDelayAfterFromCastCostHitFlat =
    "KeywordShieldValueDegenDelayAfterFromCastCostHitFlat";
constexpr inline std::string_view KeywordShieldValueDegenDelayAfterFromCastCostHitMult =
    "KeywordShieldValueDegenDelayAfterFromCastCostHitMult";
constexpr inline std::string_view AbsoluteMaxShieldValueDegenDelay = "AbsoluteMaxShieldValueDegenDelay";
constexpr inline std::string_view BaseMultShieldValueFromArmor = "BaseMultShieldValueFromArmor";
constexpr inline std::string_view BaseDrainShieldValuePercent = "BaseDrainShieldValuePercent";
constexpr inline std::string_view BaseDrainShieldValueCurrentPercent = "BaseDrainShieldValueCurrentPercent";
constexpr inline std::string_view BaseDrainShieldValueFlat = "BaseDrainShieldValueFlat";
constexpr inline std::string_view BaseShieldValueGettinWeaponDamage = "BaseShieldValueGettinWeaponDamage";
constexpr inline std::string_view BaseShieldValueGettinMagickDamage = "BaseShieldValueGettinMagickDamage";
constexpr inline std::string_view KeywordShieldEnableMagick = "KeywordShieldEnableMagick";
constexpr inline std::string_view KeywordShieldEnableWeapon = "KeywordShieldEnableWeapon";
constexpr inline std::string_view KeywordMultShieldValueFromMaxHealth = "KeywordMultShieldValueFromMaxHealth";
constexpr inline std::string_view KeywordMultShieldValueFromMaxStamina = "KeywordMultShieldValueFromMaxStamina";
constexpr inline std::string_view KeywordMultShieldValueFromMaxMagicka = "KeywordMultShieldValueFromMaxMagicka";
constexpr inline std::string_view KeywordMultShieldValueFromArmor = "KeywordMultShieldValueFromArmor";
constexpr inline std::string_view KeywordShieldValueFlat = "KeywordShieldValueFlat";
constexpr inline std::string_view KeywordShieldValueMult = "KeywordShieldValueMult";
constexpr inline std::string_view KeywordShieldValueDrainMult = "KeywordShieldValueDrainMult";
constexpr inline std::string_view KeywordShieldValueDrainFlat = "KeywordShieldValueDrainFlat";
constexpr inline std::string_view KeywordShieldValueGettinWeaponDamage = "KeywordShieldValueGettinWeaponDamage";
constexpr inline std::string_view KeywordShieldValueGettinMagickDamage = "KeywordShieldValueGettinMagickDamage";
constexpr inline std::string_view KeywordShieldValueMagicInject = "KeywordShieldValueMagicInject";
constexpr inline std::string_view KeywordShieldValueDrainPercent = "KeywordShieldValueDrainPercent";
constexpr inline std::string_view KeywordShieldValueDrainCurrentPercent = "KeywordShieldValueDrainCurrentPercent";
constexpr inline std::string_view KeywordShieldValueWeaponInject = "KeywordShieldValueWeaponInject";
constexpr inline std::string_view KeywordDamageToShieldMult = "KeywordDamageToShieldMult";
constexpr inline std::string_view KeywordActorIgnoreShield = "KeywordActorIgnoreShield";
constexpr inline std::string_view KeywordFormIgnoreShield = "KeywordFormIgnoreShield";
constexpr inline std::string_view SoundCooldown = "SoundCooldown";
constexpr inline std::string_view HitSound = "HitSound";
constexpr inline std::string_view EnableEffect = "EnableEffect";
constexpr inline std::string_view EffectCooldown = "EffectCooldown";
constexpr inline std::string_view HitEffect = "HitEffect";
constexpr inline std::string_view MGEFStaminaKeywordId = "MGEFStaminaKeywordId";
constexpr inline std::string_view EnableMGEFMagicka = "EnableMGEFMagicka";
constexpr inline std::string_view MGEFMagickaKeywordId = "MGEFMagickaKeywordId";
constexpr inline std::string_view WeaponCrit = "WeaponCrit";
constexpr inline std::string_view ActorValueIndexCritChance = "ActorValueIndexCritChance";
constexpr inline std::string_view ActorValueIndexCritDamage = "ActorValueIndexCritDamage";
constexpr inline std::string_view CastOnCrit = "CastOnCrit";
constexpr inline std::string_view FormListSpellsId = "FormListSpellsId";
constexpr inline std::string_view WeaponCritFormListSpellsId = "WeaponCritFormListSpellsId";
constexpr inline std::string_view MagickCritFormListSpellsId = "MagickCritFormListSpellsId";
constexpr inline std::string_view FormListKeywordId = "FormListKeywordId";
constexpr inline std::string_view EnableLeechVampirism = "EnableLeechVampirism";
constexpr inline std::string_view LeechVampirismSpellId = "LeechVampirismSpellId";
constexpr inline std::string_view WeaponCritFormListKeywordId = "WeaponCritFormListKeywordId";
constexpr inline std::string_view BoundSpellsFormListId = "BoundSpellsFormListId";
constexpr inline std::string_view MagickCritFormListKeywordId = "MagickCritFormListKeywordId";
constexpr inline std::string_view BoundWeaponsFormListId = "BoundWeaponsFormListId";
constexpr inline std::string_view RecoupEffect = "RecoupEffect";
constexpr inline std::string_view FormListRecoupKeywordsMeleeId = "FormListRecoupKeywordsMeleeId";
constexpr inline std::string_view FormListRecoupKeywordsRangedId = "FormListRecoupKeywordsRangedId";
constexpr inline std::string_view FormListRecoupKeywordsMagicId = "FormListRecoupKeywordsMagicId";
constexpr inline std::string_view FormListRecoupSpellsMeleeId = "FormListRecoupSpellsMeleeId";
constexpr inline std::string_view FormListRecoupSpellsRangedId = "FormListRecoupSpellsRangedId";
constexpr inline std::string_view FormListRecoupSpellsMagicId = "FormListRecoupSpellsMagicId";
constexpr inline std::string_view LeechEffect = "LeechEffect";
constexpr inline std::string_view FormListLeechKeywordsMeleeId = "FormListLeechKeywordsMeleeId";
constexpr inline std::string_view FormListLeechKeywordsRangedId = "FormListLeechKeywordsRangedId";
constexpr inline std::string_view FormListLeechKeywordsMagicId = "FormListLeechKeywordsMagicId";
constexpr inline std::string_view FormListLeechSpellsMeleeId = "FormListLeechSpellsMeleeId";
constexpr inline std::string_view FormListLeechSpellsRangedId = "FormListLeechSpellsRangedId";
constexpr inline std::string_view FormListLeechSpellsMagicId = "FormListLeechSpellsMagicId";
constexpr inline std::string_view SpellStrikeEffect = "SpellStrikeEffect";
constexpr inline std::string_view FormListSpellStrikeKeywordsMeleeId = "FormListSpellStrikeKeywordsMeleeId";
constexpr inline std::string_view FormListSpellStrikeKeywordsRangedId = "FormListSpellStrikeKeywordsRangedId";
constexpr inline std::string_view FormListSpellStrikeKeywordsMagicId = "FormListSpellStrikeKeywordsMagicId";
constexpr inline std::string_view FormListSpellStrikeSpellsMeleeId = "FormListSpellStrikeSpellsMeleeId";
constexpr inline std::string_view FormListSpellStrikeSpellsRangedId = "FormListSpellStrikeSpellsRangedId";
constexpr inline std::string_view FormListSpellStrikeSpellsMagicId = "FormListSpellStrikeSpellsMagicId";
constexpr inline std::string_view CastOnHit = "CastOnHit";
constexpr inline std::string_view CastOnGetHit = "CastOnGetHit";
constexpr inline std::string_view CastOnMagicBlocker = "CastOnMagicBlocker";
constexpr inline std::string_view FormListBlockSpellsId = "FormListBlockSpellsId";
constexpr inline std::string_view FormListBlockKeywordId = "FormListBlockKeywordId";
constexpr inline std::string_view FormListTimingBlockSpellsId = "FormListTimingBlockSpellsId";
constexpr inline std::string_view FormListTimingBlockKeywordId = "FormListTimingBlockKeywordId";
constexpr inline std::string_view FormListParryBlockSpellsId = "FormListParryBlockSpellsId";
constexpr inline std::string_view FormListParryBlockKeywordId = "FormListParryBlockKeywordId";
constexpr inline std::string_view FormListProjectileBlockMagickSpellsId = "FormListProjectileBlockMagickSpellsId";
constexpr inline std::string_view FormListProjectileBlockMagickKeywordId = "FormListProjectileBlockMagickKeywordId";
constexpr inline std::string_view FormListProjectileBlockPhysicalSpellsId = "FormListProjectileBlockPhysicalSpellsId";
constexpr inline std::string_view FormListProjectileBlockPhysicalKeywordId = "FormListProjectileBlockPhysicalKeywordId";
constexpr inline std::string_view FormListProjectileTimingBlockPhysicalSpellsId =
    "FormListProjectileTimingBlockPhysicalSpellsId";
constexpr inline std::string_view FormListProjectileTimingBlockPhysicalKeywordId =
    "FormListProjectileTimingBlockPhysicalKeywordId";
constexpr inline std::string_view FormListProjectileTimingBlockMagickSpellsId =
    "FormListProjectileTimingBlockMagickSpellsId";
constexpr inline std::string_view FormListProjectileTimingBlockMagickKeywordId =
    "FormListProjectileTimingBlockMagickKeywordId";
constexpr inline std::string_view ResourceManager = "ResourceManager";
constexpr inline std::string_view EnableInfamy = "EnableInfamy";
constexpr inline std::string_view EnableRegeneration = "EnableRegeneration";
constexpr inline std::string_view EnableAlternateMeleeCost = "EnableAlternateMeleeCost";
constexpr inline std::string_view UseResourceManagerPowerAttackCostAV = "UseResourceManagerPowerAttackCostAV";
constexpr inline std::string_view UseStaminaMultFromAttackData = "UseStaminaMultFromAttackData";
constexpr inline std::string_view EnableOverallAttackCostAV = "EnableOverallAttackCostAV";
constexpr inline std::string_view StaminaPowerAttackWeaponBase = "StaminaPowerAttackWeaponBase";
constexpr inline std::string_view PowerBashStaminaPenalty = "PowerBashStaminaPenalty";
constexpr inline std::string_view NoBlockRotationKeyword = "NoBlockRotationKeyword";
constexpr inline std::string_view KeywordAddSpeedMultToDamageResist = "KeywordAddSpeedMultToDamageResist";
constexpr inline std::string_view AddSpeedMultToDamageResistMult = "AddSpeedMultToDamageResistMult";
constexpr inline std::string_view StaminaCostSpellKeywordId = "StaminaCostSpellKeywordId";
constexpr inline std::string_view IgnoreApplySpellAndEnchantingKeywordId = "IgnoreApplySpellAndEnchantingKeywordId";
constexpr inline std::string_view ActiveBlockKeyword = "ActiveBlockKeyword";
constexpr inline std::string_view StaminaBashMult = "StaminaBashMult";
constexpr inline std::string_view EnableWeaponSpend = "EnableWeaponSpend";
constexpr inline std::string_view EnableBashSpend = "EnableBashSpend";
constexpr inline std::string_view EnableBlock = "EnableBlock";
constexpr inline std::string_view SpellCostBase = "SpellCostBase";
constexpr inline std::string_view EnableAddSpellDamageCost = "EnableAddSpellDamageCost";
constexpr inline std::string_view SpellCostKeywordId = "SpellCostKeywordId";
constexpr inline std::string_view SpellCostMagickaCostMult = "SpellCostMagickaCostMult";
constexpr inline std::string_view SpellDamageNovice = "SpellCostNovice";
constexpr inline std::string_view SpellDamageApprentice = "SpellCostApprentice";
constexpr inline std::string_view SpellDamageAdept = "SpellCostAdept";
constexpr inline std::string_view SpellDamageExpert = "SpellCostExpert";
constexpr inline std::string_view SpellDamageMaster = "SpellCostMaster";
constexpr inline std::string_view EnableCorrectCostCalculate = "EnableCorrectCostCalculate";
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
constexpr inline std::string_view EquipSpellWithTwoHanded = "EquipSpellWithTwoHanded";
constexpr inline std::string_view TKDodge = "TKDodge";
constexpr inline std::string_view DodgeKey = "DodgeKey";
constexpr inline std::string_view SwitchKey = "SwitchKey";
constexpr inline std::string_view HoldSwitchKey = "HoldSwitchKey";
constexpr inline std::string_view HighlightKey = "HighlightKey";
constexpr inline std::string_view HoldHighlightKey = "HoldHighlightKey";
constexpr inline std::string_view HighlightDuration = "HighlightDuration";
constexpr inline std::string_view PowerAttackArtWeapon = "PowerAttackArtWeapon";
constexpr inline std::string_view PowerAttackArtOther = "PowerAttackArtOther";
constexpr inline std::string_view SoundId = "SoundId";
constexpr inline std::string_view ShaderId = "ShaderId";
constexpr inline std::string_view EnableTappingDodge = "EnableTappingDodge";
constexpr inline std::string_view StepDodge = "StepDodge";
constexpr inline std::string_view RangeReleaseArt = "RangeReleaseArt";
constexpr inline std::string_view iFrameDuration = "iFrameDuration";
constexpr inline std::string_view iFrameDurationMGEFKeywordId = "iFrameDurationMGEFKeywordId";
constexpr inline std::string_view KeyUpDelay = "KeyUpDelay";
constexpr inline std::string_view MaxCost = "MaxCost";
constexpr inline std::string_view MinCost = "MinCost";
constexpr inline std::string_view EnableDodgeWhenAttackStart = "EnableDodgeWhenAttackStart";
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
constexpr inline std::string_view EnableRallyManacost = "EnableRallyManacost";
constexpr inline std::string_view RallyManacostMult = "RallyManacostMult";
constexpr inline std::string_view RallyMagickaMult = "RallyMagickaMult";
constexpr inline std::string_view MagickaMultRestorePerSecondFromRally = "MagickaMultRestorePerSecondFromRally";
constexpr inline std::string_view PowerAttackShader = "PowerAttackShader";
constexpr inline std::string_view PowerAttackKeyword = "PowerAttackKeyword";
constexpr inline std::string_view RangeAttackKeyword = "RangeAttackKeyword";
constexpr inline std::string_view EnableStaminaCost = "EnableStaminaCost";
constexpr inline std::string_view CasterAdditions = "CasterAdditions";
constexpr inline std::string_view MagicBlocker = "MagicBlocker";
constexpr inline std::string_view TimingBlockIntegrationKeyword = "TimingBlockIntegrationKeyword";
constexpr inline std::string_view BlockParryIntegrationKeyword = "BlockParryIntegrationKeyword";
constexpr inline std::string_view ResourceManagerKeywordHolder = "ResourceManagerKeywordHolder";
constexpr inline std::string_view ProjectileBlockIntegrationKeyword = "ProjectileBlockIntegrationKeyword";
constexpr inline std::string_view ProjectileTimingBlockIntegrationKeyword = "ProjectileTimingBlockIntegrationKeyword";
constexpr inline std::string_view EnableRegenWhileCastingWard = "EnableRegenWhileCastingWard";
constexpr inline std::string_view MagicBlockerKeyword = "MagicBlockerKeyword";
constexpr inline std::string_view DisableAutoRotation360Block = "DisableAutoRotation360Block";
constexpr inline std::string_view Highlighting = "Highlighting";
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
constexpr inline std::string_view CastOnSoulTrap = "CastOnSoulTrap";
constexpr inline std::string_view Clairvoyance = "Clairvoyance";
constexpr inline std::string_view ClairvoyanceKey = "ClairvoyanceKey";
constexpr inline std::string_view CastOnDodge = "CastOnDodge";
constexpr inline std::string_view CastOnProjectileBlock = "CastOnProjectileBlock";
constexpr inline std::string_view BoundSpellSwap = "BoundSpellSwap";
constexpr inline std::string_view CastOnProjectileTimingBlock = "CastOnProjectileTimingBlock";
constexpr inline std::string_view RegenDelay = "RegenDelay";
constexpr inline std::string_view ResistTweaks = "ResistTweaks";
constexpr inline std::string_view MaxResistKeywordId = "MaxResistKeywordId";
constexpr inline std::string_view NegativeResistImmuneKeywordId = "NegativeResistImmuneKeywordId";
constexpr inline std::string_view EnableCheckResistance = "EnableCheckResistance";
constexpr inline std::string_view EnchGetNoAbsorb = "EnchGetNoAbsorb";
constexpr inline std::string_view EnchIgnoreResistance = "EnchIgnoreResistance";
constexpr inline std::string_view ResistWeight = "ResistWeight";
constexpr inline std::string_view DamageResistWeight = "DamageResistWeight";
constexpr inline std::string_view DamageResistLow = "DamageResistLow";
constexpr inline std::string_view DamageResistHigh = "DamageResistHigh";
constexpr inline std::string_view EnableCustomDamageResistWeight = "EnableCustomDamageResistWeight";
constexpr inline std::string_view CastOnIsCostKeyword = "CastOnIsCostKeyword";
constexpr inline std::string_view CastOnCasterSwapKeyword = "CastOnCasterSwapKeyword";
constexpr inline std::string_view RestrictEquipShieldKeyword = "RestrictEquipShieldKeyword";
constexpr inline std::string_view RestrictEquipOffhandKeyword = "RestrictEquipOffhandKeyword";
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
constexpr inline std::string_view BashBlockKeywordId = "BashBlockKeywordId";
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
constexpr inline std::string_view ArrowSparkId = "ArrowSparkId";
constexpr inline std::string_view ArrowSparkFlareId = "ArrowSparkFlareId";
constexpr inline std::string_view ArrowSparkHaloId = "ArrowSparkHaloId";
constexpr inline std::string_view ArrowBlockSoundId = "ArrowBlockSoundId";
constexpr inline std::string_view TimingBlockSoundId = "TimingBlockSoundId";
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
constexpr inline std::string_view EnableParryBash = "EnableParryBash";
constexpr inline std::string_view ParryBash = "ParryBash";
constexpr inline std::string_view EnablePreHitFrame = "EnablePreHitFrame";
constexpr inline std::string_view EnableBashBlock = "EnableBashBlock";
constexpr inline std::string_view EnableBashBlockWithTimingParryBlock = "EnableBashBlockWithTimingParryBlock";
constexpr inline std::string_view EnableWeaponSwing = "EnableWeaponSwing";
constexpr inline std::string_view StaminaCostMult = "StaminaCostMult";
constexpr inline std::string_view AllowCastWithoutStamina = "AllowCastWithoutStamina";

constexpr inline std::string_view StaggerSystem = "StaggerSystem";
constexpr inline std::string_view PoiseRecoveryTimeId = "PoiseRecoveryTimeId";
constexpr inline std::string_view PoiseRecoveryActionsId = "PoiseRecoveryActionsId";
constexpr inline std::string_view PoiseValueId = "PoiseValueId";
constexpr inline std::string_view PoiseSmallId = "PoiseSmallId";
constexpr inline std::string_view PoiseMediumId = "PoiseMediumId";
constexpr inline std::string_view PoiseLargeId = "PoiseLargeId";
constexpr inline std::string_view PoiseLargestId = "PoiseLargestId";
constexpr inline std::string_view PoiseDamageCooldownId = "PoiseDamageCooldownId";
constexpr inline std::string_view PoiseDamageImmunityId = "PoiseDamageImmunityId";
constexpr inline std::string_view PoiseRegenerationId = "PoiseRegenerationId";
constexpr inline std::string_view ListSmallSpellsMagickId = "ListSmallSpellsMagickId";
constexpr inline std::string_view ListSmallSpellsMeleeId = "ListSmallSpellsMeleeId";
constexpr inline std::string_view ListSmallSpellsRangedId = "ListSmallSpellsRangedId";
constexpr inline std::string_view ListSmallKeywordsMagickId = "ListSmallKeywordsMagickId";
constexpr inline std::string_view ListSmallKeywordsMeleeId = "ListSmallKeywordsMeleeId";
constexpr inline std::string_view ListSmallKeywordsRangedId = "ListSmallKeywordsRangedId";
constexpr inline std::string_view ListMediumSpellsMagickId = "ListMediumSpellsMagickId";
constexpr inline std::string_view ListMediumSpellsMeleeId = "ListMediumSpellsMeleeId";
constexpr inline std::string_view ListMediumSpellsRangedId = "ListMediumSpellsRangedId";
constexpr inline std::string_view ListMediumKeywordsMagickId = "ListMediumKeywordsMagickId";
constexpr inline std::string_view ListMediumKeywordsMeleeId = "ListMediumKeywordsMeleeId";
constexpr inline std::string_view ListMediumKeywordsRangedId = "ListMediumKeywordsRangedId";
constexpr inline std::string_view ListLargeSpellsMagickId = "ListLargeSpellsMagickId";
constexpr inline std::string_view ListLargeSpellsMeleeId = "ListLargeSpellsMeleeId";
constexpr inline std::string_view ListLargeSpellsRangedId = "ListLargeSpellsRangedId";
constexpr inline std::string_view ListLargeKeywordsMagickId = "ListLargeKeywordsMagickId";
constexpr inline std::string_view ListLargeKeywordsMeleeId = "ListLargeKeywordsMeleeId";
constexpr inline std::string_view ListLargeKeywordsRangedId = "ListLargeKeywordsRangedId";
constexpr inline std::string_view ListLargestSpellsMagickId = "ListLargestSpellsMagickId";
constexpr inline std::string_view ListLargestSpellsMeleeId = "ListLargestSpellsMeleeId";
constexpr inline std::string_view ListLargestSpellsRangedId = "ListLargestSpellsRangedId";
constexpr inline std::string_view ListLargestKeywordsMagickId = "ListLargestKeywordsMagickId";
constexpr inline std::string_view ListLargestKeywordsMeleeId = "ListLargestKeywordsMeleeId";
constexpr inline std::string_view ListLargestKeywordsRangedId = "ListLargestKeywordsRangedId";

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

    const auto stamina_value_keyword_id = tbl[StaminaShield][StaminaShieldValueKeywordId].value<RE::FormID>();
    keyword_shield_value_ =
        data_handler.LookupForm<RE::BGSKeyword>(stamina_value_keyword_id.value(), config.mod_name());

    av_ = static_cast<RE::ActorValue>(tbl[StaminaShield][ActorValueIndex].value_or(120));
    enable_av_ = tbl[StaminaShield][EnableAv].value_or(false);

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

  leech_vampirism_ = tbl[Vampirism][EnableLeechVampirism].value_or(false);
  leech_vampirism_spell_ =
      get_tes_data(RE::SpellItem, tbl[Vampirism][LeechVampirismSpellId].value<RE::FormID>().value(), config.mod_name());

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

  leech_vampirism_ = tbl[MagicVampirism][EnableLeechVampirism].value_or(false);
  leech_vampirism_spell_ = get_tes_data(
      RE::SpellItem, tbl[MagicVampirism][LeechVampirismSpellId].value<RE::FormID>().value(), config.mod_name());

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

    const auto sound_id = tbl[WeaponCrit][SoundId].value<RE::FormID>();

    crit_sound_ = data_handler.LookupForm<RE::BGSSoundDescriptorForm>(sound_id.value(), config.mod_name());
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

      const auto sound_id = tbl[MagickCrit][SoundId].value<RE::FormID>();

      crit_sound_ = data_handler.LookupForm<RE::BGSSoundDescriptorForm>(sound_id.value(), config.mod_name());
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

    const auto weapon_form_keywords_form_id = tbl[CastOnCrit][WeaponCritFormListKeywordId].value<RE::FormID>();
    const auto weapon_form_spells_form_id = tbl[CastOnCrit][WeaponCritFormListSpellsId].value<RE::FormID>();
    weapon_formlist_needkw_ =
        data_handler.LookupForm<RE::BGSListForm>(weapon_form_keywords_form_id.value(), config.mod_name());
    weapon_formlist_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(weapon_form_spells_form_id.value(), config.mod_name());

    const auto magick_form_keywords_form_id = tbl[CastOnCrit][MagickCritFormListKeywordId].value<RE::FormID>();
    const auto magick_form_spells_form_id = tbl[CastOnCrit][MagickCritFormListSpellsId].value<RE::FormID>();
    magick_formlist_needkw_ =
        data_handler.LookupForm<RE::BGSListForm>(magick_form_keywords_form_id.value(), config.mod_name());
    magick_formlist_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(magick_form_spells_form_id.value(), config.mod_name());
  }
}

Config::CastOnHitConfig::CastOnHitConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: cast on hit"sv);
  enable_ = tbl[CastOnHit][Enable].value_or(false);
  if (enable_) {
    const auto melee_form_keywords_form_id = tbl[CastOnHit][FormListMeleeKeywordId].value<RE::FormID>();
    const auto melee_form_spells_form_id = tbl[CastOnHit][FormListMeleeSpellsId].value<RE::FormID>();
    formlist_melee_needkw_ =
        data_handler.LookupForm<RE::BGSListForm>(melee_form_keywords_form_id.value(), config.mod_name());
    formlist_melee_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(melee_form_spells_form_id.value(), config.mod_name());

    const auto bash_form_keywords_form_id = tbl[CastOnHit][FormListBashKeywordId].value<RE::FormID>();
    const auto bash_form_spells_form_id = tbl[CastOnHit][FormListBashSpellsId].value<RE::FormID>();
    formlist_bash_needkw_ =
        data_handler.LookupForm<RE::BGSListForm>(bash_form_keywords_form_id.value(), config.mod_name());
    formlist_bash_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(bash_form_spells_form_id.value(), config.mod_name());

    const auto ranged_form_keywords_form_id = tbl[CastOnHit][FormListRangedKeywordId].value<RE::FormID>();
    const auto ranged_form_spells_form_id = tbl[CastOnHit][FormListRangedSpellsId].value<RE::FormID>();
    formlist_ranged_needkw_ =
        data_handler.LookupForm<RE::BGSListForm>(ranged_form_keywords_form_id.value(), config.mod_name());
    formlist_ranged_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(ranged_form_spells_form_id.value(), config.mod_name());
  }
}

Config::CastOnMagicProjectileHitConfig::CastOnMagicProjectileHitConfig(toml::table& tbl,
                                                                       RE::TESDataHandler& data_handler,
                                                                       const Config& config)
{
  logger::info("config init: CastOnMagicProjectileHit"sv);
  enable_ = tbl[CastOnMagicProjectileHit][Enable].value_or(false);
  if (enable_) {
    const auto form_keywords_form_id = tbl[CastOnMagicProjectileHit][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id = tbl[CastOnMagicProjectileHit][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_ = data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ = data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}
Config::CastOnTimingBlockStaggerConfig::CastOnTimingBlockStaggerConfig(toml::table& tbl,
                                                                       RE::TESDataHandler& data_handler,
                                                                       const Config& config)
{
  logger::info("config init: CastOnTimingBlockStagger"sv);
  enable_ = tbl[CastOnTimingBlockStagger][Enable].value_or(false);
  if (enable_) {
    const auto form_keywords_form_id = tbl[CastOnTimingBlockStagger][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id = tbl[CastOnTimingBlockStagger][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_ = data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ = data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}

Config::CastOnMagicBlockerConfig::CastOnMagicBlockerConfig(toml::table& tbl,
                                                           RE::TESDataHandler& data_handler,
                                                           const Config& config)
{
  logger::info("config init: CastOnMagicBlockerConfig"sv);
  enable_ = tbl[CastOnMagicBlocker][Enable].value_or(false);
  if (enable_) {
    const auto block_form_keywords_form_id = tbl[CastOnMagicBlocker][FormListBlockKeywordId].value<RE::FormID>();
    const auto block_form_spells_form_id = tbl[CastOnMagicBlocker][FormListBlockSpellsId].value<RE::FormID>();
    form_list_block_keyword_ =
        data_handler.LookupForm<RE::BGSListForm>(block_form_keywords_form_id.value(), config.mod_name());
    form_list_block_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(block_form_spells_form_id.value(), config.mod_name());

    const auto timing_block_form_keywords_form_id =
        tbl[CastOnMagicBlocker][FormListTimingBlockKeywordId].value<RE::FormID>();
    const auto timing_block_form_spells_form_id =
        tbl[CastOnMagicBlocker][FormListTimingBlockSpellsId].value<RE::FormID>();
    form_list_timing_block_keyword_ =
        data_handler.LookupForm<RE::BGSListForm>(timing_block_form_keywords_form_id.value(), config.mod_name());
    form_list_timing_block_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(timing_block_form_spells_form_id.value(), config.mod_name());

    const auto parry_block_form_keywords_form_id =
        tbl[CastOnMagicBlocker][FormListParryBlockKeywordId].value<RE::FormID>();
    const auto parry_block_form_spells_form_id =
        tbl[CastOnMagicBlocker][FormListParryBlockSpellsId].value<RE::FormID>();
    form_list_parry_block_keyword_ =
        data_handler.LookupForm<RE::BGSListForm>(parry_block_form_keywords_form_id.value(), config.mod_name());
    form_list_parry_block_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(parry_block_form_spells_form_id.value(), config.mod_name());

    const auto projectile_block_form_keywords_physical_form_id =
        tbl[CastOnMagicBlocker][FormListProjectileBlockPhysicalKeywordId].value<RE::FormID>();
    const auto projectile_block_form_spells_physical_form_id =
        tbl[CastOnMagicBlocker][FormListProjectileBlockPhysicalSpellsId].value<RE::FormID>();
    form_list_projectile_block_keyword_physical_ = data_handler.LookupForm<RE::BGSListForm>(
        projectile_block_form_keywords_physical_form_id.value(), config.mod_name());
    form_list_projectile_block_spells_physical_ = data_handler.LookupForm<RE::BGSListForm>(
        projectile_block_form_spells_physical_form_id.value(), config.mod_name());

    const auto projectile_timing_block_form_keywords_physical_form_id =
        tbl[CastOnMagicBlocker][FormListProjectileTimingBlockPhysicalKeywordId].value<RE::FormID>();
    const auto projectile_timing_block_form_spells_physical_form_id =
        tbl[CastOnMagicBlocker][FormListProjectileTimingBlockPhysicalSpellsId].value<RE::FormID>();
    form_list_projectile_timing_block_keyword_physical_ = data_handler.LookupForm<RE::BGSListForm>(
        projectile_timing_block_form_keywords_physical_form_id.value(), config.mod_name());
    form_list_projectile_timing_block_spells_physical_ = data_handler.LookupForm<RE::BGSListForm>(
        projectile_timing_block_form_spells_physical_form_id.value(), config.mod_name());

    const auto projectile_block_form_keywords_magick_form_id =
        tbl[CastOnMagicBlocker][FormListProjectileBlockMagickKeywordId].value<RE::FormID>();
    const auto projectile_block_form_spells_magick_form_id =
        tbl[CastOnMagicBlocker][FormListProjectileBlockMagickSpellsId].value<RE::FormID>();
    form_list_projectile_block_keyword_magick_ = data_handler.LookupForm<RE::BGSListForm>(
        projectile_block_form_keywords_magick_form_id.value(), config.mod_name());
    form_list_projectile_block_spells_magick_ = data_handler.LookupForm<RE::BGSListForm>(
        projectile_block_form_spells_magick_form_id.value(), config.mod_name());

    const auto projectile_timing_block_form_keywords_magick_form_id =
        tbl[CastOnMagicBlocker][FormListProjectileTimingBlockMagickKeywordId].value<RE::FormID>();
    const auto projectile_timing_block_form_spells_magick_form_id =
        tbl[CastOnMagicBlocker][FormListProjectileTimingBlockMagickSpellsId].value<RE::FormID>();
    form_list_projectile_timing_block_keyword_magick_ = data_handler.LookupForm<RE::BGSListForm>(
        projectile_timing_block_form_keywords_magick_form_id.value(), config.mod_name());
    form_list_projectile_timing_block_spells_magick_ = data_handler.LookupForm<RE::BGSListForm>(
        projectile_timing_block_form_spells_magick_form_id.value(), config.mod_name());
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
Config::CastOnSoulTrapConfig::CastOnSoulTrapConfig(toml::table& tbl,
                                                   RE::TESDataHandler& data_handler,
                                                   const Config& config)
{
  logger::info("config init: CastOnSoulTrap"sv);
  enable_ = tbl[CastOnSoulTrap][Enable].value_or(false);
  if (enable_) {
    const auto form_keywords_form_id = tbl[CastOnSoulTrap][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id = tbl[CastOnSoulTrap][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_ = data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ = data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}

Config::ClairvoyanceConfig::ClairvoyanceConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: ClairvoyanceConfig"sv);
  enable_ = tbl[Clairvoyance][Enable].value_or(false);
  if (enable_) {
    const auto spell_id = tbl[Clairvoyance][SpellId].value<RE::FormID>();
    clairvoyance_spell_ = data_handler.LookupForm<RE::SpellItem>(spell_id.value(), config.mod_name());
    key_ = tbl[Clairvoyance][ClairvoyanceKey].value_or(277);
  }
}

Config::CastOnDodgeConfig::CastOnDodgeConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: CastOnDodgeConfig"sv);
  enable_ = tbl[CastOnDodge][Enable].value_or(false);
  if (enable_) {
    const auto form_keywords_form_id = tbl[CastOnDodge][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id = tbl[CastOnDodge][FormListSpellsId].value<RE::FormID>();
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
    enable_alternate_melee_cost_ = tbl[ResourceManager][EnableAlternateMeleeCost].value_or(false);
    use_stamina_mult_from_attack_data_ = tbl[ResourceManager][UseStaminaMultFromAttackData].value_or(false);
    enable_overall_attack_cost_av_ = tbl[ResourceManager][EnableOverallAttackCostAV].value_or(false);
    stamina_power_attack_weapon_base_ = tbl[ResourceManager][StaminaPowerAttackWeaponBase].value_or(20.f);
    power_bash_stamina_penalty_ = tbl[ResourceManager][PowerBashStaminaPenalty].value_or(2.f);
    stamina_bash_mult_ = tbl[ResourceManager][StaminaBashMult].value_or(1.f);
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
    enable_mco_recovery_ = tbl[TKDodge][EnableMcoRecovery].value_or(false);
    enable_dodge_when_attack_start_ = tbl[TKDodge][EnableDodgeWhenAttackStart].value_or(false);
    interrupt_cast_when_dodge_ = tbl[TKDodge][EnableInterruptCastWhenDodge].value_or(false);
    iframe_duration_ = tbl[TKDodge][iFrameDuration].value_or(0.5f);
    stagger_cost_penalty_mult_ = tbl[TKDodge][StaggerCostPenaltyMult].value_or(0.5f);
    step_ = tbl[TKDodge][StepDodge].value_or(false);
    key_ = tbl[TKDodge][DodgeKey].value_or(277);
    sprint_tapping_dodge_ = tbl[TKDodge][EnableTappingDodge].value_or(false);
    block_dodge_when_attack_ = tbl[TKDodge][BlockDodgeWhenAttack].value_or(false);
    block_dodge_when_power_attack_ = tbl[TKDodge][BlockDodgeWhenPowerAttack].value_or(false);
    enable_dodge_when_not_enough_resource_ = tbl[TKDodge][EnableDodgeWhenNotEnoughResource].value_or(false);
    block_dodge_when_casting_ = tbl[TKDodge][BlockDodgeWhenCasting].value_or(false);
    key_up_delay_ = tbl[TKDodge][KeyUpDelay].value_or(0.2f);
    equipped_weight_mult_ = tbl[TKDodge][WeightMult].value_or(1.0f);
    flat_cost_ = tbl[TKDodge][FlatCost].value_or(0.f);
    percent_cost_ = tbl[TKDodge][PercentCost].value_or(0.f);
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
    const auto enable_dodge_in_stagger_keyword_id = tbl[TKDodge][EnableDodgeInStaggerKeywordId].value<RE::FormID>();

    health_kw_ = data_handler.LookupForm<RE::BGSKeyword>(health_spend_keyword_form_id.value(), config.mod_name());
    stamina_kw_ = data_handler.LookupForm<RE::BGSKeyword>(stamina_spend_keyword_form_id.value(), config.mod_name());
    magicka_kw_ = data_handler.LookupForm<RE::BGSKeyword>(spend_magicka_keyword_form_id.value(), config.mod_name());

    enable_dodge_in_stagger_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(enable_dodge_in_stagger_keyword_id.value(), config.mod_name());

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
  constexpr RE::FormID mag_fail_form_id = 0x3D0D3;
  constexpr std::string_view skyrim = "Skyrim.esm";
  mag_fail_sound_ = data_handler.LookupForm<RE::BGSSoundDescriptorForm>(mag_fail_form_id, skyrim);
  if (enable_) {
    const auto spell_add_when_casting_form_id = tbl[CasterAdditions][SpellAddWhileCastingId].value<RE::FormID>();

    spell_add_when_casting_ =
        data_handler.LookupForm<RE::SpellItem>(spell_add_when_casting_form_id.value(), config.mod_name());

    stamina_cost_ = tbl[CasterAdditions][EnableStaminaCost].value_or(false);
    enable_rally_manacost_ = tbl[CasterAdditions][EnableRallyManacost].value_or(false);
    allow_cast_without_stamina_ = tbl[CasterAdditions][AllowCastWithoutStamina].value_or(true);
    stamina_cost_mult_ = tbl[CasterAdditions][StaminaCostMult].value_or(0.25f);
    rally_manacost_mult_ = tbl[CasterAdditions][RallyManacostMult].value_or(0.20f);
    rally_magicka_mult_ = tbl[CasterAdditions][RallyMagickaMult].value_or(0.01f);
    magicka_mult_restore_per_second_from_rally_ =
        tbl[CasterAdditions][MagickaMultRestorePerSecondFromRally].value_or(0.02f);
  }
}

Config::MagicBlockerConfig::MagicBlockerConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{

  logger::info("config init: MagicBlockerConfig"sv);
  enable_ = tbl[MagicBlocker][Enable].value_or(false);
  if (enable_) {

    enable_regen_while_casting_ward_ = tbl[MagicBlocker][EnableRegenWhileCastingWard].value_or(false);
    disable_auto_rotation360_block_ = tbl[MagicBlocker][DisableAutoRotation360Block].value_or(false);

    const auto timing_block_integration_keyword_id =
        tbl[MagicBlocker][TimingBlockIntegrationKeyword].value<RE::FormID>();
    timing_block_integration_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(timing_block_integration_keyword_id.value(), config.mod_name());

    const auto block_parry_integration_keyword_id = tbl[MagicBlocker][BlockParryIntegrationKeyword].value<RE::FormID>();
    block_parry_integration_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(block_parry_integration_keyword_id.value(), config.mod_name());

    const auto projectile_block_integration_keyword_id =
        tbl[MagicBlocker][ProjectileBlockIntegrationKeyword].value<RE::FormID>();
    projectile_block_integration_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(projectile_block_integration_keyword_id.value(), config.mod_name());

    const auto projectile_timing_block_integration_keyword_id =
        tbl[MagicBlocker][ProjectileTimingBlockIntegrationKeyword].value<RE::FormID>();
    projectile_timing_block_integration_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(
        projectile_timing_block_integration_keyword_id.value(), config.mod_name());

    const auto magic_blocker_keyword_id = tbl[MagicBlocker][MagicBlockerKeyword].value<RE::FormID>();
    magic_blocker_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(magic_blocker_keyword_id.value(), config.mod_name());

    const auto blank_activator_form_id = tbl[MagicBlocker][BlankActivatorId].value<RE::FormID>();
    blank_activator_ = data_handler.LookupForm<RE::TESObjectACTI>(blank_activator_form_id.value(), config.mod_name());

    const auto spark_form_id = tbl[MagicBlocker][SparkId].value<RE::FormID>();
    spark_ = data_handler.LookupForm<RE::Explosion>(spark_form_id.value(), config.mod_name());

    const auto resource_manager_keyword_holder_id = tbl[MagicBlocker][ResourceManagerKeywordHolder].value<RE::FormID>();
    resource_manager_keyword_holder_ =
        data_handler.LookupForm<RE::TESObjectARMO>(resource_manager_keyword_holder_id.value(), config.mod_name());

    const auto spark_flare_form_id = tbl[MagicBlocker][SparkFlareId].value<RE::FormID>();
    spark_flare_ = data_handler.LookupForm<RE::Explosion>(spark_flare_form_id.value(), config.mod_name());

    const auto spark_halo_form_id = tbl[MagicBlocker][SparkHaloId].value<RE::FormID>();
    spark_halo_ = data_handler.LookupForm<RE::Explosion>(spark_halo_form_id.value(), config.mod_name());

    const auto parry_sound_form_id = tbl[MagicBlocker][ParrySoundId].value<RE::FormID>();
    parry_sound_ = data_handler.LookupForm<RE::BGSSoundDescriptorForm>(parry_sound_form_id.value(), config.mod_name());

    const auto block_sound_form_id = tbl[MagicBlocker][BlockSoundId].value<RE::FormID>();
    block_sound_ = data_handler.LookupForm<RE::BGSSoundDescriptorForm>(block_sound_form_id.value(), config.mod_name());

    const auto timing_block_sound_form_id = tbl[MagicBlocker][TimingBlockSoundId].value<RE::FormID>();
    timing_block_sound_ =
        data_handler.LookupForm<RE::BGSSoundDescriptorForm>(timing_block_sound_form_id.value(), config.mod_name());
  }
}

Config::HighlightingConfig::HighlightingConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: HighlightingConfig"sv);
  enable_ = tbl[Highlighting][Enable].value_or(false);
  if (enable_) {
    highlight_duration_ = tbl[Highlighting][HighlightDuration].value_or(0.5f);

    const auto highlight_power_attack_id = tbl[Highlighting][PowerAttackShader].value<RE::FormID>();
    power_attack_shader_ =
        data_handler.LookupForm<RE::TESEffectShader>(highlight_power_attack_id.value(), config.mod_name());

    const auto power_attack_art_weapon_id = tbl[Highlighting][PowerAttackArtWeapon].value<RE::FormID>();
    power_attack_art_weapon_ =
        data_handler.LookupForm<RE::BGSArtObject>(power_attack_art_weapon_id.value(), config.mod_name());

    const auto power_attack_art_other_id = tbl[Highlighting][PowerAttackArtOther].value<RE::FormID>();
    power_attack_art_other_ =
        data_handler.LookupForm<RE::BGSArtObject>(power_attack_art_other_id.value(), config.mod_name());

    const auto range_release_art_id = tbl[Highlighting][RangeReleaseArt].value<RE::FormID>();
    range_release_art_ = data_handler.LookupForm<RE::BGSArtObject>(range_release_art_id.value(), config.mod_name());

    const auto power_attack_keyword_id = tbl[Highlighting][PowerAttackKeyword].value<RE::FormID>();
    power_attack_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(power_attack_keyword_id.value(), config.mod_name());

    const auto ramge_attack_keyword_id = tbl[Highlighting][RangeAttackKeyword].value<RE::FormID>();
    range_release_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(ramge_attack_keyword_id.value(), config.mod_name());
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

Config::RecoupEffectConfig::RecoupEffectConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: RecoupEffectConfig"sv);
  enable_ = tbl[RecoupEffect][Enable].value_or(false);
  if (enable_) {
    const auto formlist_keywords_form_melee_id = tbl[RecoupEffect][FormListRecoupKeywordsMeleeId].value<RE::FormID>();
    const auto formlist_keywords_form_ranged_id = tbl[RecoupEffect][FormListRecoupKeywordsRangedId].value<RE::FormID>();
    const auto formlist_keywords_form_magic_id = tbl[RecoupEffect][FormListRecoupKeywordsMagicId].value<RE::FormID>();
    const auto formlist_spells_form_melee_id = tbl[RecoupEffect][FormListRecoupSpellsMeleeId].value<RE::FormID>();
    const auto formlist_spells_form_ranged_id = tbl[RecoupEffect][FormListRecoupSpellsRangedId].value<RE::FormID>();
    const auto formlist_spells_form_magic_id = tbl[RecoupEffect][FormListRecoupSpellsMagicId].value<RE::FormID>();

    keywords_melee_ =
        data_handler.LookupForm<RE::BGSListForm>(formlist_keywords_form_melee_id.value(), config.mod_name());
    spells_melee_ = data_handler.LookupForm<RE::BGSListForm>(formlist_spells_form_melee_id.value(), config.mod_name());

    keywords_ranged_ =
        data_handler.LookupForm<RE::BGSListForm>(formlist_keywords_form_ranged_id.value(), config.mod_name());
    spells_ranged_ =
        data_handler.LookupForm<RE::BGSListForm>(formlist_spells_form_ranged_id.value(), config.mod_name());

    keywords_magic_ =
        data_handler.LookupForm<RE::BGSListForm>(formlist_keywords_form_magic_id.value(), config.mod_name());
    spells_magic_ = data_handler.LookupForm<RE::BGSListForm>(formlist_spells_form_magic_id.value(), config.mod_name());
  }
}

Config::LeechEffectConfig::LeechEffectConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: LeechEffectConfig"sv);
  enable_ = tbl[LeechEffect][Enable].value_or(false);
  if (enable_) {
    const auto formlist_keywords_form_melee_id = tbl[LeechEffect][FormListLeechKeywordsMeleeId].value<RE::FormID>();
    const auto formlist_keywords_form_ranged_id = tbl[LeechEffect][FormListLeechKeywordsRangedId].value<RE::FormID>();
    const auto formlist_keywords_form_magic_id = tbl[LeechEffect][FormListLeechKeywordsMagicId].value<RE::FormID>();
    const auto formlist_spells_form_melee_id = tbl[LeechEffect][FormListLeechSpellsMeleeId].value<RE::FormID>();
    const auto formlist_spells_form_ranged_id = tbl[LeechEffect][FormListLeechSpellsRangedId].value<RE::FormID>();
    const auto formlist_spells_form_magic_id = tbl[LeechEffect][FormListLeechSpellsMagicId].value<RE::FormID>();

    keywords_melee_ =
        data_handler.LookupForm<RE::BGSListForm>(formlist_keywords_form_melee_id.value(), config.mod_name());
    spells_melee_ = data_handler.LookupForm<RE::BGSListForm>(formlist_spells_form_melee_id.value(), config.mod_name());

    keywords_ranged_ =
        data_handler.LookupForm<RE::BGSListForm>(formlist_keywords_form_ranged_id.value(), config.mod_name());
    spells_ranged_ =
        data_handler.LookupForm<RE::BGSListForm>(formlist_spells_form_ranged_id.value(), config.mod_name());

    keywords_magic_ =
        data_handler.LookupForm<RE::BGSListForm>(formlist_keywords_form_magic_id.value(), config.mod_name());
    spells_magic_ = data_handler.LookupForm<RE::BGSListForm>(formlist_spells_form_magic_id.value(), config.mod_name());
  }
}

Config::SpellStrikeEffectConfig::SpellStrikeEffectConfig(toml::table& tbl,
                                                         RE::TESDataHandler& data_handler,
                                                         const Config& config)
{
  logger::info("config init: SpellStrikeEffectConfig"sv);
  enable_ = tbl[SpellStrikeEffect][Enable].value_or(false);
  if (enable_) {
    const auto formlist_keywords_form_melee_id =
        tbl[SpellStrikeEffect][FormListSpellStrikeKeywordsMeleeId].value<RE::FormID>();
    const auto formlist_keywords_form_ranged_id =
        tbl[SpellStrikeEffect][FormListSpellStrikeKeywordsRangedId].value<RE::FormID>();
    // const auto formlist_keywords_form_magic_id = tbl[LeechEffect][FormListLeechKeywordsMagicId].value<RE::FormID>();
    const auto formlist_spells_form_melee_id =
        tbl[SpellStrikeEffect][FormListSpellStrikeSpellsMeleeId].value<RE::FormID>();
    const auto formlist_spells_form_ranged_id =
        tbl[SpellStrikeEffect][FormListSpellStrikeSpellsRangedId].value<RE::FormID>();
    // const auto formlist_spells_form_magic_id = tbl[LeechEffect][FormListLeechSpellsMagicId].value<RE::FormID>();

    keywords_melee_ =
        data_handler.LookupForm<RE::BGSListForm>(formlist_keywords_form_melee_id.value(), config.mod_name());
    spells_melee_ = data_handler.LookupForm<RE::BGSListForm>(formlist_spells_form_melee_id.value(), config.mod_name());

    keywords_ranged_ =
        data_handler.LookupForm<RE::BGSListForm>(formlist_keywords_form_ranged_id.value(), config.mod_name());
    spells_ranged_ =
        data_handler.LookupForm<RE::BGSListForm>(formlist_spells_form_ranged_id.value(), config.mod_name());

    // keywords_magic_ =
    //     data_handler.LookupForm<RE::BGSListForm>(formlist_keywords_form_magic_id.value(), config.mod_name());
    // spells_magic_ = data_handler.LookupForm<RE::BGSListForm>(formlist_spells_form_magic_id.value(),
    // config.mod_name());
  }
}

Config::SpecialTechniquesConfig::SpecialTechniquesConfig(toml::table& tbl,
                                                         RE::TESDataHandler& data_handler,
                                                         const Config& config)
{
  logger::info("config init: SpecialTechniquesConfig"sv);
  enable_ = tbl[SpecialTechniques][Enable].value_or(false);
  if (enable_) {

    const auto no_block_rotate_keyword_id = tbl[SpecialTechniques][NoBlockRotationKeyword].value<RE::FormID>();
    no_block_rotate_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(no_block_rotate_keyword_id.value(), config.mod_name());

    const auto enable_360_block_keyword_id = tbl[SpecialTechniques][Enable360Block].value<RE::FormID>();
    enable_360_block_ = data_handler.LookupForm<RE::BGSKeyword>(enable_360_block_keyword_id.value(), config.mod_name());

    const auto acitve_block_keyword_id = tbl[SpecialTechniques][ActiveBlockKeyword].value<RE::FormID>();
    active_block_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(acitve_block_keyword_id.value(), config.mod_name());

    const auto stamina_cost_spell_keyword_id = tbl[SpecialTechniques][StaminaCostSpellKeywordId].value<RE::FormID>();
    stamina_cost_spell_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(stamina_cost_spell_keyword_id.value(), config.mod_name());

    const auto ignore_apply_spell_id =
        tbl[SpecialTechniques][IgnoreApplySpellAndEnchantingKeywordId].value<RE::FormID>();
    ignore_apply_spell_and_enchanting_keyword_id_ =
        data_handler.LookupForm<RE::BGSKeyword>(ignore_apply_spell_id.value(), config.mod_name());

    const auto keyword_add_speed_mult_to_damage_resist_id =
        tbl[SpecialTechniques][KeywordAddSpeedMultToDamageResist].value<RE::FormID>();
    keyword_add_speed_mult_to_damage_resist_ =
        data_handler.LookupForm<RE::BGSKeyword>(keyword_add_speed_mult_to_damage_resist_id.value(), config.mod_name());

    // enable_360_block_ = tbl[SpecialTechniques][Enable360PlayerBlock].value_or(false);
    dynamic_regeneration_health_ = tbl[SpecialTechniques][DynamicRegenerationHealth].value_or(false);
    dynamic_regeneration_magicka_ = tbl[SpecialTechniques][DynamicRegenerationMagicka].value_or(false);
    dynamic_regeneration_stamina_ = tbl[SpecialTechniques][DynamicRegenerationStamina].value_or(false);
    normalize_power_attack_mult_pc_ = tbl[SpecialTechniques][NormalizePowerAttackMultPC].value_or(-1.f);
    normalize_normal_attack_mult_pc_ = tbl[SpecialTechniques][NormalizeNormalAttackMultPC].value_or(-1.f);
    normalize_power_attack_mult_npc_ = tbl[SpecialTechniques][NormalizePowerAttackMultNPC].value_or(-1.f);
    normalize_normal_attack_mult_npc_ = tbl[SpecialTechniques][NormalizeNormalAttackMultNPC].value_or(-1.f);
    normalize_max_regen_delay_health_ = tbl[SpecialTechniques][NormalizeRegenDelayHealth].value_or(-1.f);
    normalize_max_regen_delay_magicka_ = tbl[SpecialTechniques][NormalizeRegenDelayMagicka].value_or(-1.f);
    normalize_max_regen_delay_stamina_ = tbl[SpecialTechniques][NormalizeRegenDelayStamina].value_or(-1.f);
    slow_after_block_hit_duration_ = tbl[SpecialTechniques][SlowAfterBlockHitDuration].value_or(0.4f);
    slow_after_block_hit_mult_ = tbl[SpecialTechniques][SlowAfterBlockHitMult].value_or(0.3f);
    add_speed_mult_to_damage_resist_mult_ = tbl[SpecialTechniques][AddSpeedMultToDamageResistMult].value_or(1.0f);
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
    enable_custom_damage_resist_weight_ = tbl[ResistTweaks][EnableCustomDamageResistWeight].value_or(false);
    resist_weight_ = tbl[ResistTweaks][ResistWeight].value_or(0.001f);
    damage_resist_weight_ = tbl[ResistTweaks][DamageResistWeight].value_or(0.001f);
    damage_resist_low_ = tbl[ResistTweaks][DamageResistLow].value_or(0.0f);
    damage_resist_high_ = tbl[ResistTweaks][DamageResistHigh].value_or(1.000f);
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
    enable_for_npc_ = tbl[EquipLoad][EnableForNpc].value_or(false);
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
    enable_parry_bash_ = tbl[ParryBash][EnableParryBash].value_or(false);
    enable_weapon_swing_ = tbl[ParryBash][EnableWeaponSwing].value_or(true);
    enable_vanilla_bash_ranged_ = tbl[ParryBash][EnableVanillaBashRanged].value_or(false);
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

    const auto bash_block_keyword_id = tbl[ParryBash][BashBlockKeywordId].value<RE::FormID>();
    bash_block_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(bash_block_keyword_id.value(), config.mod_name());

    enable_bash_block_ = tbl[ParryBash][EnableBashBlock].value_or(false);
    enable_bash_block_with_timing_parry_block_ = tbl[ParryBash][EnableBashBlockWithTimingParryBlock].value_or(false);
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
    loot_arrow_ = tbl[DeathLoot][LootArrow].value_or(false);
    loot_bosses_ = tbl[DeathLoot][LootBosses].value_or(false);
    loot_uniques_ = tbl[DeathLoot][LootUniques].value_or(false);
    hit_timer_ = tbl[DeathLoot][HitTimer].value_or(15.f);

    const auto exclusive_keyword_id = tbl[DeathLoot][ExclusiveKeywordId].value<RE::FormID>();
    const auto storage_id = tbl[DeathLoot][StorageFormId].value<RE::FormID>();
    const auto spell_when_loot_id = tbl[DeathLoot][SpellWhenLootId].value<RE::FormID>();

    exclusive_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(exclusive_keyword_id.value(), config.mod_name());
    storage_ = data_handler.LookupForm<RE::TESObjectREFR>(storage_id.value(), config.mod_name());
    spell_when_loot_ = data_handler.LookupForm<RE::SpellItem>(spell_when_loot_id.value(), config.mod_name());
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
    enable_one_cooldown_ = tbl[PotionsDrinkLimit][EnableOneCooldown].value_or(false);
    other_enable_ = tbl[PotionsDrinkLimit][OtherEnable].value_or(false);
    other_cap_base_ = tbl[PotionsDrinkLimit][OtherCapBase].value_or(7);
    other_duration_base_ = tbl[PotionsDrinkLimit][OtherDurationBase].value_or(15.f);
    notify_ = tbl[PotionsDrinkLimit][Notify].value_or("Я не могу больше выпить зелье."s);
    other_revert_exclusive_ = tbl[PotionsDrinkLimit][OtherRevertExclusive].value_or(false);

    const auto other_exclusive_keyword_id = tbl[PotionsDrinkLimit][OtherExclusiveKeywordId].value<RE::FormID>();
    const auto other_cap_keyword_id = tbl[PotionsDrinkLimit][OtherCapKeywordId].value<RE::FormID>();
    const auto other_duration_keyword_id = tbl[PotionsDrinkLimit][OtherDurationKeywordId].value<RE::FormID>();
    const auto no_remove_keyword_id = tbl[PotionsDrinkLimit][NoRemoveKeywordId].value<RE::FormID>();

    other_exclusive_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(other_exclusive_keyword_id.value(), config.mod_name());
    other_cap_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(other_cap_keyword_id.value(), config.mod_name());
    other_duration_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(other_duration_keyword_id.value(), config.mod_name());
    no_remove_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(no_remove_keyword_id.value(), config.mod_name());

    health_enable_ = tbl[PotionsDrinkLimit][HealthEnable].value_or(false);
    health_duration_base_ = tbl[PotionsDrinkLimit][HealthDurationBase].value_or(15.f);
    health_cap_base_ = tbl[PotionsDrinkLimit][HealthCapBase].value_or(7);

    const auto health_cap_keyword_id = tbl[PotionsDrinkLimit][HealthCapKeywordId].value<RE::FormID>();
    const auto health_duration_keyword_id = tbl[PotionsDrinkLimit][HealthDurationKeywordId].value<RE::FormID>();
    const auto health_keyword_id = tbl[PotionsDrinkLimit][HealthKeywordId].value<RE::FormID>();

    health_cap_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(health_cap_keyword_id.value(), config.mod_name());
    health_duration_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(health_duration_keyword_id.value(), config.mod_name());
    health_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(health_keyword_id.value(), config.mod_name());

    stamina_enable_ = tbl[PotionsDrinkLimit][StaminaEnable].value_or(false);
    stamina_duration_base_ = tbl[PotionsDrinkLimit][StaminaDurationBase].value_or(15.f);
    stamina_cap_base_ = tbl[PotionsDrinkLimit][StaminaCapBase].value_or(7);

    const auto stamina_cap_keyword_id = tbl[PotionsDrinkLimit][StaminaCapKeywordId].value<RE::FormID>();
    const auto stamina_duration_keyword_id = tbl[PotionsDrinkLimit][StaminaDurationKeywordId].value<RE::FormID>();
    const auto stamina_keyword_id_ = tbl[PotionsDrinkLimit][StaminaKeywordId].value<RE::FormID>();

    stamina_cap_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(stamina_cap_keyword_id.value(), config.mod_name());
    stamina_duration_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(stamina_duration_keyword_id.value(), config.mod_name());
    stamina_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(stamina_keyword_id_.value(), config.mod_name());

    magicka_enable_ = tbl[PotionsDrinkLimit][MagickaEnable].value_or(false);
    magicka_duration_base_ = tbl[PotionsDrinkLimit][MagickaDurationBase].value_or(15.f);
    magicka_cap_base_ = tbl[PotionsDrinkLimit][MagickaCapBase].value_or(7);

    const auto magicka_cap_keyword_id = tbl[PotionsDrinkLimit][MagickaCapKeywordId].value<RE::FormID>();
    const auto magicka_duration_keyword_id = tbl[PotionsDrinkLimit][MagickaDurationKeywordId].value<RE::FormID>();
    const auto magicka_keyword_id = tbl[PotionsDrinkLimit][MagickaKeywordId].value<RE::FormID>();

    magicka_cap_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(magicka_cap_keyword_id.value(), config.mod_name());
    magicka_duration_keyword_ =
        data_handler.LookupForm<RE::BGSKeyword>(magicka_duration_keyword_id.value(), config.mod_name());
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

Config::KiEnergyPowerConfig::KiEnergyPowerConfig(toml::table& tbl,
                                                 RE::TESDataHandler& data_handler,
                                                 const Config& config)
{
  logger::info("config init: KiEnergyPowerConfig"sv);
  enable_ = tbl[KiEnergyPower][Enable].value_or(false);
  if (enable_) {
    enable_one_cooldown_ = tbl[KiEnergyPower][EnableOneCooldown].value_or(false);
    enable_reservation_ = tbl[KiEnergyPower][EnableReservation].value_or(false);
    enable_reservation_adjust_cost_ = tbl[KiEnergyPower][EnableReservationAdjustCost].value_or(false);
    enable_ki_summons_ = tbl[KiEnergyPower][EnableKiSummons].value_or(false);
    const auto duration_keyword_id = tbl[KiEnergyPower][DurationKeywordId].value<RE::FormID>();
    const auto cap_keyword_id = tbl[KiEnergyPower][CapKeywordId].value<RE::FormID>();
    const auto keyword_id = tbl[KiEnergyPower][KeywordId].value<RE::FormID>();
    const auto form_list_keywords_id = tbl[KiEnergyPower][FormListReservationKeywordsId].value<RE::FormID>();
    const auto form_list_spells_id = tbl[KiEnergyPower][FormListReservationSpellsId].value<RE::FormID>();
    const auto form_list_abilities_no_condition_id =
        tbl[KiEnergyPower][FormListReservationSpellsIdNoCondition].value<RE::FormID>();
    const auto form_list_summons_id = tbl[KiEnergyPower][KiSummonsFormListId].value<RE::FormID>();

    cap_base_ = tbl[KiEnergyPower][CapBase].value_or(5);
    duration_base_ = tbl[KiEnergyPower][DurationBase].value_or(60.f);
    duration_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(duration_keyword_id.value(), config.mod_name());
    cap_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(cap_keyword_id.value(), config.mod_name());
    ki_power_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(keyword_id.value(), config.mod_name());
    ki_reservation_keywords_ =
        data_handler.LookupForm<RE::BGSListForm>(form_list_keywords_id.value(), config.mod_name());
    ki_reservation_abilities_ =
        data_handler.LookupForm<RE::BGSListForm>(form_list_spells_id.value(), config.mod_name());
    ki_reservation_abilities_no_condition_ =
        data_handler.LookupForm<RE::BGSListForm>(form_list_abilities_no_condition_id.value(), config.mod_name());
    ki_summons_list_ = data_handler.LookupForm<RE::BGSListForm>(form_list_summons_id.value(), config.mod_name());
    notify_ = tbl[KiEnergyPower][Notify].value_or("Нет доступной энергии Ци."s);
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
    spell_cost_base_ = tbl[ProjectileBlock][SpellCostBase].value_or(0.f);
    enable_block_magick_for_magick_ = tbl[ProjectileBlock][EnableBlockMagickForMagick].value_or(false);
    enable_arrow_ = tbl[ProjectileBlock][EnableArrow].value_or(false);
    enable_missile_ = tbl[ProjectileBlock][EnableMissile].value_or(false);
    enable_cone_ = tbl[ProjectileBlock][EnableCone].value_or(false);
    enable_flame_ = tbl[ProjectileBlock][EnableFlame].value_or(false);
    enable_beam_ = tbl[ProjectileBlock][EnableBeam].value_or(false);
    enable_sparks_ = tbl[ProjectileBlock][EnableSparks].value_or(false);
    enable_sound_ = tbl[ProjectileBlock][EnableSound].value_or(false);
    enable_add_spell_damage_cost_ = tbl[ProjectileBlock][EnableAddSpellDamageCost].value_or(false);

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

    const auto arrow_spark_form_id = tbl[ProjectileBlock][ArrowSparkId].value<RE::FormID>();
    arrow_spark_ = data_handler.LookupForm<RE::Explosion>(arrow_spark_form_id.value(), config.mod_name());

    const auto arrow_spark_flare_form_id = tbl[ProjectileBlock][ArrowSparkFlareId].value<RE::FormID>();
    arrow_spark_flare_ = data_handler.LookupForm<RE::Explosion>(arrow_spark_flare_form_id.value(), config.mod_name());

    const auto arrow_spark_halo_form_id = tbl[ProjectileBlock][ArrowSparkHaloId].value<RE::FormID>();
    arrow_spark_halo_ = data_handler.LookupForm<RE::Explosion>(arrow_spark_halo_form_id.value(), config.mod_name());

    const auto arrow_block_sound_form_id = tbl[ProjectileBlock][ArrowBlockSoundId].value<RE::FormID>();
    arrow_block_sound_ =
        data_handler.LookupForm<RE::BGSSoundDescriptorForm>(arrow_block_sound_form_id.value(), config.mod_name());

    const auto spell_cost_keyword_id = tbl[ProjectileBlock][SpellCostKeywordId].value<RE::FormID>();
    spell_cost_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(spell_cost_keyword_id.value(), config.mod_name());

    enable_correct_cost_calculate_ = tbl[ProjectileBlock][EnableCorrectCostCalculate].value_or(false);

    spell_cost_magicka_mult_ = tbl[ProjectileBlock][SpellCostMagickaCostMult].value_or(0.1f);
    spell_damage_novice_ = tbl[ProjectileBlock][SpellDamageNovice].value_or(5.f);
    spell_damage_apprentice_ = tbl[ProjectileBlock][SpellDamageApprentice].value_or(10.f);
    spell_damage_adept_ = tbl[ProjectileBlock][SpellDamageAdept].value_or(15.f);
    spell_damage_expert_ = tbl[ProjectileBlock][SpellDamageExpert].value_or(20.f);
    spell_damage_master_ = tbl[ProjectileBlock][SpellDamageMaster].value_or(25.f);
  }
}

Config::BoundSpellSwapConfig::BoundSpellSwapConfig(toml::table& tbl,
                                                   RE::TESDataHandler& data_handler,
                                                   const Config& config)
{
  logger::info("config init: BoundSpellSwapConfig"sv);
  enable_ = tbl[BoundSpellSwap][Enable].value_or(false);
  if (enable_) {

    const auto spell_form_list_id = tbl[BoundSpellSwap][BoundSpellsFormListId].value<RE::FormID>();
    const auto weapons_form_list_id = tbl[BoundSpellSwap][BoundWeaponsFormListId].value<RE::FormID>();
    bound_spells_ = data_handler.LookupForm<RE::BGSListForm>(spell_form_list_id.value(), config.mod_name());
    bound_weapons_ = data_handler.LookupForm<RE::BGSListForm>(weapons_form_list_id.value(), config.mod_name());
  }
}

Config::CastOnProjectileBlockConfig::CastOnProjectileBlockConfig(toml::table& tbl,
                                                                 RE::TESDataHandler& data_handler,
                                                                 const Config& config)
{
  logger::info("config init: CastOnProjectileBlockConfig"sv);
  enable_ = tbl[CastOnProjectileBlock][Enable].value_or(false);
  if (enable_) {
    physical_ = tbl[CastOnProjectileBlock][Physical].value_or(false);
    magick_ = tbl[CastOnProjectileBlock][Magick].value_or(false);

    const auto weapon_form_keywords_form_id =
        tbl[CastOnProjectileBlock][WeaponCritFormListKeywordId].value<RE::FormID>();
    const auto weapon_form_spells_form_id = tbl[CastOnProjectileBlock][WeaponCritFormListSpellsId].value<RE::FormID>();
    weapon_formlist_needkw_ =
        data_handler.LookupForm<RE::BGSListForm>(weapon_form_keywords_form_id.value(), config.mod_name());
    weapon_formlist_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(weapon_form_spells_form_id.value(), config.mod_name());

    const auto magick_form_keywords_form_id =
        tbl[CastOnProjectileBlock][MagickCritFormListKeywordId].value<RE::FormID>();
    const auto magick_form_spells_form_id = tbl[CastOnProjectileBlock][MagickCritFormListSpellsId].value<RE::FormID>();
    magick_formlist_needkw_ =
        data_handler.LookupForm<RE::BGSListForm>(magick_form_keywords_form_id.value(), config.mod_name());
    magick_formlist_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(magick_form_spells_form_id.value(), config.mod_name());
  }
}

Config::CastOnProjectileTimingBlockConfig::CastOnProjectileTimingBlockConfig(toml::table& tbl,
                                                                             RE::TESDataHandler& data_handler,
                                                                             const Config& config)
{
  logger::info("config init: CastOnProjectileTimingBlockConfig"sv);
  enable_ = tbl[CastOnProjectileTimingBlock][Enable].value_or(false);
  if (enable_) {
    physical_ = tbl[CastOnProjectileTimingBlock][Physical].value_or(false);
    magick_ = tbl[CastOnProjectileTimingBlock][Magick].value_or(false);

    const auto weapon_form_keywords_form_id =
        tbl[CastOnProjectileTimingBlock][WeaponCritFormListKeywordId].value<RE::FormID>();
    const auto weapon_form_spells_form_id =
        tbl[CastOnProjectileTimingBlock][WeaponCritFormListSpellsId].value<RE::FormID>();
    weapon_formlist_needkw_ =
        data_handler.LookupForm<RE::BGSListForm>(weapon_form_keywords_form_id.value(), config.mod_name());
    weapon_formlist_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(weapon_form_spells_form_id.value(), config.mod_name());

    const auto magick_form_keywords_form_id =
        tbl[CastOnProjectileTimingBlock][MagickCritFormListKeywordId].value<RE::FormID>();
    const auto magick_form_spells_form_id =
        tbl[CastOnProjectileTimingBlock][MagickCritFormListSpellsId].value<RE::FormID>();
    magick_formlist_needkw_ =
        data_handler.LookupForm<RE::BGSListForm>(magick_form_keywords_form_id.value(), config.mod_name());
    magick_formlist_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(magick_form_spells_form_id.value(), config.mod_name());
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
  equip_spell_with_two_handed_ = tbl[MiscFixes][EquipSpellWithTwoHanded].value_or(false);
  cast_ability_on_equip_instant_ = tbl[MiscFixes][CastAbilityOnEquipInstant].value_or(false);
  cast_ability_magicka_cost_ = tbl[MiscFixes][CastAbilityMagickaCost].value_or(false);
  cast_ability_on_equip_ = tbl[MiscFixes][CastAbilityOnEquip].value_or(false);
  cast_shout_on_equip_ = tbl[MiscFixes][CastShoutOnEquip].value_or(false);
  display_key_name_ = tbl[MiscFixes][DisplayKeyName].value_or(false);
  enable_instant_restore_stamina_npc_ = tbl[MiscFixes][EnableInstantRestoreStaminaNPC].value_or(false);
  enable_unlimited_merchant_gold_ = tbl[MiscFixes][EnableUnlimitedMerchantGold].value_or(false);
  enable_blade_and_blunt_true_hud_special_bar_ = tbl[MiscFixes][EnableBladeAndBluntTrueHudSpecialBar].value_or(false);
  enable_enchants_ignore_absorb_ = tbl[MiscFixes][EnableEnchantsIgnoreAbsorb].value_or(false);
  enable_npc_always_spend_stamina_on_attack_ = tbl[MiscFixes][EnableNpcAlwaysSpendStaminaOnAttack].value_or(false);
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

Config::MagickToStaminaConfig::MagickToStaminaConfig(toml::table& tbl, RE::TESDataHandler&, const Config&)
{
  logger::info("config init: MagickToStamina"sv);
  enable_ = tbl[MagickToStamina][Enable].value_or(false);
}
Config::DeclutteredLootConfig::DeclutteredLootConfig(toml::table& tbl,
                                                     RE::TESDataHandler& data_handler,
                                                     const Config& config)
{
  logger::info("config init: DeclutteredLoot"sv);
  enable_ = tbl[DeclutteredLoot][Enable].value_or(false);
  if (enable_) {
    switch_key_ = tbl[DeclutteredLoot][SwitchKey].value_or(71);
    hold_switch_key_ = tbl[DeclutteredLoot][HoldSwitchKey].value_or(3.f);
    highlight_key_ = tbl[DeclutteredLoot][HighlightKey].value_or(71);
    hold_highlight_key_ = tbl[DeclutteredLoot][HoldHighlightKey].value_or(0.75f);
    highlight_duration_ = tbl[DeclutteredLoot][HighlightDuration].value_or(15.f);
    hide_food_ = tbl[DeclutteredLoot][HideFood].value_or(false);
    hide_misc_ = tbl[DeclutteredLoot][HideMisc].value_or(false);
    hide_ingredient_ = tbl[DeclutteredLoot][HideIngredient].value_or(false);
    enable_gold_weight_threshold_ = tbl[DeclutteredLoot][EnableGoldWeightThreshold].value_or(false);
    gold_weight_threshold_ = tbl[DeclutteredLoot][GoldWeightThreshold].value_or(500.f);
    enable_gold_threshold_ = tbl[DeclutteredLoot][EnableGoldThreshold].value_or(false);
    gold_threshold_ = tbl[DeclutteredLoot][GoldThreshold].value_or(500);
    enable_sound_ = tbl[DeclutteredLoot][EnableSound].value_or(false);

    let exclusive_keyword_id = tbl[DeclutteredLoot][ExclusiveKeywordId].value<RE::FormID>();
    exclusive_keyword_ = data_handler.LookupForm<RE::BGSKeyword>(exclusive_keyword_id.value(), config.mod_name());

    let shader_id = tbl[DeclutteredLoot][ShaderId].value<RE::FormID>();
    shader_ = data_handler.LookupForm<RE::TESEffectShader>(shader_id.value(), config.mod_name());

    let sound_id = tbl[DeclutteredLoot][SoundId].value<RE::FormID>();
    highlight_activation_sound_ =
        data_handler.LookupForm<RE::BGSSoundDescriptorForm>(sound_id.value(), config.mod_name());
  }
}

Config::SlowTimeConfig::SlowTimeConfig(toml::table& tbl, RE::TESDataHandler&, const Config&)
{
  logger::info("config init: SlowTime"sv);
  enable_ = tbl[SlowTime][Enable].value_or(false);
  if (enable_) {
    enable_on_block_ = tbl[SlowTime][EnableOnBlock].value_or(false);
    enable_on_kill_ = tbl[SlowTime][EnableOnKill].value_or(false);
    enable_on_magick_crit_ = tbl[SlowTime][EnableOnMagickCrit].value_or(false);
    enable_on_projectile_block_ = tbl[SlowTime][EnableOnProjectileBlock].value_or(false);
    enable_on_projectile_timing_block_ = tbl[SlowTime][EnableOnProjectileTimingBlock].value_or(false);
    enable_on_timing_block_ = tbl[SlowTime][EnableOnTimingBlock].value_or(false);
    enable_on_parry_timing_block_ = tbl[SlowTime][EnableOnParryBlock].value_or(false);
    enable_on_weapon_crit_ = tbl[SlowTime][EnableOnWeaponCrit].value_or(false);
    enable_on_weapon_vanilla_crit_ = tbl[SlowTime][EnableOnVanillaWeaponCrit].value_or(false);
    enable_on_sneak_attack_ = tbl[SlowTime][EnableOnSneakAttack].value_or(false);
    enable_on_parry_bash_ = tbl[SlowTime][EnableOnParryBash].value_or(false);
    slow_duration_ = tbl[SlowTime][SlowDuration].value_or(0.2f);
    slow_power_ = tbl[SlowTime][SlowPower].value_or(0.3f);
  }
}

Config::CastOnGetHitConfig::CastOnGetHitConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: cast on get hit"sv);
  enable_ = tbl[CastOnGetHit][Enable].value_or(false);
  if (enable_) {
    const auto melee_form_keywords_form_id = tbl[CastOnGetHit][FormListMeleeKeywordId].value<RE::FormID>();
    const auto melee_form_spells_form_id = tbl[CastOnGetHit][FormListMeleeSpellsId].value<RE::FormID>();
    formlist_melee_needkw_ =
        data_handler.LookupForm<RE::BGSListForm>(melee_form_keywords_form_id.value(), config.mod_name());
    formlist_melee_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(melee_form_spells_form_id.value(), config.mod_name());

    const auto bash_form_keywords_form_id = tbl[CastOnGetHit][FormListBashKeywordId].value<RE::FormID>();
    const auto bash_form_spells_form_id = tbl[CastOnGetHit][FormListBashSpellsId].value<RE::FormID>();
    formlist_bash_needkw_ =
        data_handler.LookupForm<RE::BGSListForm>(bash_form_keywords_form_id.value(), config.mod_name());
    formlist_bash_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(bash_form_spells_form_id.value(), config.mod_name());

    const auto ranged_form_keywords_form_id = tbl[CastOnGetHit][FormListRangedKeywordId].value<RE::FormID>();
    const auto ranged_form_spells_form_id = tbl[CastOnGetHit][FormListRangedSpellsId].value<RE::FormID>();
    formlist_ranged_needkw_ =
        data_handler.LookupForm<RE::BGSListForm>(ranged_form_keywords_form_id.value(), config.mod_name());
    formlist_ranged_spells_ =
        data_handler.LookupForm<RE::BGSListForm>(ranged_form_spells_form_id.value(), config.mod_name());
  }
}

Config::CastOnMagicProjectileGetHitConfig::CastOnMagicProjectileGetHitConfig(toml::table& tbl,
                                                                             RE::TESDataHandler& data_handler,
                                                                             const Config& config)
{
  logger::info("config init: CastOnMagicProjectileGetHit"sv);
  enable_ = tbl[CastOnMagicProjectileGetHit][Enable].value_or(false);
  if (enable_) {
    const auto form_keywords_form_id = tbl[CastOnMagicProjectileGetHit][FormListKeywordId].value<RE::FormID>();
    const auto form_spells_form_id = tbl[CastOnMagicProjectileGetHit][FormListSpellsId].value<RE::FormID>();
    formlist_needkw_ = data_handler.LookupForm<RE::BGSListForm>(form_keywords_form_id.value(), config.mod_name());
    formlist_spells_ = data_handler.LookupForm<RE::BGSListForm>(form_spells_form_id.value(), config.mod_name());
  }
}

Config::AbsorbShieldConfig::AbsorbShieldConfig(toml::table& tbl, RE::TESDataHandler& data_handler, const Config& config)
{
  logger::info("config init: AbsorbShieldConfig"sv);
  enable_ = tbl[AbsorbShield][Enable].value_or(false);
  if (enable_) {

    absolute_max_shield_value_ = tbl[AbsorbShield][AbsoluteMaxShieldValue].value_or(10000.f);
    absolute_max_shield_value_drain_ = tbl[AbsorbShield][AbsoluteMaxShieldValueDrain].value_or(10000.f);
    absolute_max_shield_value_drain_threshold_ =
        tbl[AbsorbShield][AbsoluteMaxShieldValueDrainThreshold].value_or(10000.f);
    base_shield_value_ = tbl[AbsorbShield][BaseShieldValue].value_or(0.f);
    base_mult_shield_value_from_max_health_ = tbl[AbsorbShield][BaseMultShieldValueFromMaxHealth].value_or(0.f);
    base_mult_shield_value_from_max_stamina_ = tbl[AbsorbShield][BaseMultShieldValueFromMaxStamina].value_or(0.f);
    base_mult_shield_value_from_max_magicka_ = tbl[AbsorbShield][BaseMultShieldValueFromMaxMagicka].value_or(0.f);
    base_mult_shield_value_from_armor_ = tbl[AbsorbShield][BaseMultShieldValueFromArmor].value_or(0.f);
    base_drain_shield_value_percent_ = tbl[AbsorbShield][BaseDrainShieldValuePercent].value_or(0.05f);
    base_drain_shield_value_current_percent_ = tbl[AbsorbShield][BaseDrainShieldValuePercent].value_or(0.05f);
    base_drain_shield_value_flat_ = tbl[AbsorbShield][BaseDrainShieldValueFlat].value_or(2.f);
    base_shield_value_gettin_weapon_damage_ = tbl[AbsorbShield][BaseShieldValueGettinWeaponDamage].value_or(0.f);
    base_shield_value_gettin_magick_damage_ = tbl[AbsorbShield][BaseShieldValueGettinMagickDamage].value_or(0.f);
    base_from_cast_cost_flat_ = tbl[AbsorbShield][BaseFromCastCostFlat].value_or(0.f);
    base_from_cast_cost_percent_ = tbl[AbsorbShield][BaseFromCastCostPercent].value_or(0.05f);
    base_shield_value_drain_threshold_flat_ = tbl[AbsorbShield][BaseShieldValueDrainThresholdFlat].value_or(0.f);
    base_shield_value_drain_threshold_percent_ = tbl[AbsorbShield][BaseShieldValueDrainThresholdPercent].value_or(0.f);
    absolute_max_shield_value_degen_delay_ = tbl[AbsorbShield][AbsoluteMaxShieldValueDegenDelay].value_or(5.f);
    base_shield_value_degen_delay_after_from_cast_cost_hit_flat_ =
        tbl[AbsorbShield][BaseShieldValueDegenDelayAfterFromCastCostHitFlat].value_or(1.f);
    base_shield_value_degen_delay_after_weapon_hit_flat_ =
        tbl[AbsorbShield][BaseShieldValueDegenDelayAfterWeaponHitFlat].value_or(1.f);
    base_shield_value_degen_delay_after_magick_hit_flat_ =
        tbl[AbsorbShield][BaseShieldValueDegenDelayAfterMagickHitFlat].value_or(1.f);
    base_mult_shield_value_drain_threshold_from_armor_ =
        tbl[AbsorbShield][BaseMultShieldValueDrainThresholdFromArmor].value_or(0.f);
    base_mult_shield_value_drain_threshold_from_max_health_ =
        tbl[AbsorbShield][BaseMultShieldValueDrainThresholdFromMaxHealth].value_or(0.f);
    base_mult_shield_value_drain_threshold_from_max_magicka_ =
        tbl[AbsorbShield][BaseMultShieldValueDrainThresholdFromMaxMagicka].value_or(0.f);
    base_mult_shield_value_drain_threshold_from_max_stamina_ =
        tbl[AbsorbShield][BaseMultShieldValueDrainThresholdFromMaxStamina].value_or(0.f);

    keyword_shield_value_degen_delay_after_weapon_hit_enable_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueDegenDelayAfterWeaponHitEnable].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_shield_value_degen_delay_after_magick_hit_enable_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueDegenDelayAfterMagickHitEnable].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_shield_value_degen_delay_after_from_cast_cost_hit_enable_ = get_tes_data(
        RE::BGSKeyword,
        tbl[AbsorbShield][KeywordShieldValueDegenDelayAfterFromCastCostHitEnable].value<RE::FormID>().value(),
        config.mod_name());
    keyword_shield_value_degen_delay_after_weapon_hit_flat_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueDegenDelayAfterWeaponHitFlat].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_shield_value_degen_delay_after_from_cast_cost_hit_flat_ = get_tes_data(
        RE::BGSKeyword,
        tbl[AbsorbShield][KeywordShieldValueDegenDelayAfterFromCastCostHitFlat].value<RE::FormID>().value(),
        config.mod_name());
    keyword_shield_value_degen_delay_after_weapon_hit_mult_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueDegenDelayAfterWeaponHitMult].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_shield_value_degen_delay_after_magick_hit_flat_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueDegenDelayAfterMagickHitFlat].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_shield_value_degen_delay_after_magick_hit_mult_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueDegenDelayAfterMagickHitMult].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_shield_value_degen_delay_after_from_cast_cost_hit_mult_ = get_tes_data(
        RE::BGSKeyword,
        tbl[AbsorbShield][KeywordShieldValueDegenDelayAfterFromCastCostHitMult].value<RE::FormID>().value(),
        config.mod_name());

    keyword_shield_value_drain_threshold_enable_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueDrainThresholdEnable].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_shield_value_drain_threshold_flat_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueDrainThresholdFlat].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_shield_value_drain_threshold_percent_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueDrainThresholdPercent].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_shield_value_drain_threshold_mult_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueDrainThresholdMult].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_mult_shield_value_drain_threshold_from_max_health_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordMultShieldValueDrainThresholdFromMaxHealth].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_mult_shield_value_drain_threshold_from_max_stamina_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordMultShieldValueDrainThresholdFromMaxStamina].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_mult_shield_value_drain_threshold_from_max_magicka_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordMultShieldValueDrainThresholdFromMaxMagicka].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_mult_shield_value_drain_threshold_from_armor_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordMultShieldValueDrainThresholdFromArmor].value<RE::FormID>().value(),
                     config.mod_name());

    keyword_shield_value_from_cast_cost_enable_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueFromCastCostEnable].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_from_cast_cost_flat_ = get_tes_data(
        RE::BGSKeyword, tbl[AbsorbShield][KeywordFromCastCostFlat].value<RE::FormID>().value(), config.mod_name());
    keyword_from_cast_cost_percent_ = get_tes_data(
        RE::BGSKeyword, tbl[AbsorbShield][KeywordFromCastCostPercent].value<RE::FormID>().value(), config.mod_name());
    keyword_from_cast_cost_mult_ = get_tes_data(
        RE::BGSKeyword, tbl[AbsorbShield][KeywordFromCastCostMult].value<RE::FormID>().value(), config.mod_name());

    keyword_shield_enable_magick_ = get_tes_data(
        RE::BGSKeyword, tbl[AbsorbShield][KeywordShieldEnableMagick].value<RE::FormID>().value(), config.mod_name());
    keyword_shield_enable_weapon_ = get_tes_data(
        RE::BGSKeyword, tbl[AbsorbShield][KeywordShieldEnableWeapon].value<RE::FormID>().value(), config.mod_name());
    keyword_mult_shield_value_from_max_health_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordMultShieldValueFromMaxHealth].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_mult_shield_value_from_max_stamina_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordMultShieldValueFromMaxStamina].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_mult_shield_value_from_max_magicka_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordMultShieldValueFromMaxMagicka].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_mult_shield_value_from_armor_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordMultShieldValueFromArmor].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_shield_value_flat_ = get_tes_data(
        RE::BGSKeyword, tbl[AbsorbShield][KeywordShieldValueFlat].value<RE::FormID>().value(), config.mod_name());
    keyword_shield_value_mult_ = get_tes_data(
        RE::BGSKeyword, tbl[AbsorbShield][KeywordShieldValueMult].value<RE::FormID>().value(), config.mod_name());
    keyword_shield_value_drain_mult_ = get_tes_data(
        RE::BGSKeyword, tbl[AbsorbShield][KeywordShieldValueDrainMult].value<RE::FormID>().value(), config.mod_name());
    keyword_shield_value_drain_flat_ = get_tes_data(
        RE::BGSKeyword, tbl[AbsorbShield][KeywordShieldValueDrainFlat].value<RE::FormID>().value(), config.mod_name());
    keyword_shield_value_drain_percent_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueDrainPercent].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_shield_value_drain_current_percent_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueDrainCurrentPercent].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_shield_value_gettin_weapon_damage_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueGettinWeaponDamage].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_shield_value_gettin_magick_damage_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueGettinMagickDamage].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_shield_value_magic_inject_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueMagicInject].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_shield_value_weapon_inject_ =
        get_tes_data(RE::BGSKeyword,
                     tbl[AbsorbShield][KeywordShieldValueWeaponInject].value<RE::FormID>().value(),
                     config.mod_name());
    keyword_actor_ignore_shield_ = get_tes_data(
        RE::BGSKeyword, tbl[AbsorbShield][KeywordActorIgnoreShield].value<RE::FormID>().value(), config.mod_name());
    keyword_form_ignore_shield_ = get_tes_data(
        RE::BGSKeyword, tbl[AbsorbShield][KeywordFormIgnoreShield].value<RE::FormID>().value(), config.mod_name());

    keyword_damage_to_shield_mult_ = get_tes_data(
        RE::BGSKeyword, tbl[AbsorbShield][KeywordDamageToShieldMult].value<RE::FormID>().value(), config.mod_name());

    enable_sound_ = tbl[AbsorbShield][EnableSound].value_or(false);

    if (enable_sound_) {
      sound_cooldown_ = tbl[AbsorbShield][SoundCooldown].value_or(0.5f);
      hit_sound_ = get_tes_data(
          RE::BGSSoundDescriptorForm, tbl[AbsorbShield][HitSound].value<RE::FormID>().value(), config.mod_name());
    }

    enable_effect_ = tbl[AbsorbShield][EnableEffect].value_or(false);

    if (enable_effect_) {
      effect_cooldown_ = tbl[AbsorbShield][EffectCooldown].value_or(0.05f);

      hit_effect_ =
          get_tes_data(RE::Explosion, tbl[AbsorbShield][HitEffect].value<RE::FormID>().value(), config.mod_name());
    }
  }
}

Config::StaggerSystemConfig::StaggerSystemConfig(toml::table& tbl,
                                                 RE::TESDataHandler& data_handler,
                                                 const Config& config)
{
  logger::info("config init: StaggerSystemConfig"sv);
  enable_ = tbl[StaggerSystem][Enable].value_or(false);
  if (enable_) {
    poise_recovery_time_ = get_tes_data(
        RE::BGSKeyword, tbl[StaggerSystem][PoiseRecoveryTimeId].value<RE::FormID>().value(), config.mod_name());
    poise_recovery_actions_ = get_tes_data(
        RE::BGSKeyword, tbl[StaggerSystem][PoiseRecoveryActionsId].value<RE::FormID>().value(), config.mod_name());
    poise_value_ =
        get_tes_data(RE::BGSKeyword, tbl[StaggerSystem][PoiseValueId].value<RE::FormID>().value(), config.mod_name());
    poise_small_ =
        get_tes_data(RE::BGSKeyword, tbl[StaggerSystem][PoiseSmallId].value<RE::FormID>().value(), config.mod_name());
    poise_medium_ =
        get_tes_data(RE::BGSKeyword, tbl[StaggerSystem][PoiseMediumId].value<RE::FormID>().value(), config.mod_name());
    poise_large_ =
        get_tes_data(RE::BGSKeyword, tbl[StaggerSystem][PoiseLargeId].value<RE::FormID>().value(), config.mod_name());
    poise_largest_ =
        get_tes_data(RE::BGSKeyword, tbl[StaggerSystem][PoiseLargestId].value<RE::FormID>().value(), config.mod_name());
    poise_damage_cooldown_ = get_tes_data(
        RE::BGSKeyword, tbl[StaggerSystem][PoiseDamageCooldownId].value<RE::FormID>().value(), config.mod_name());
    poise_damage_immunity_ = get_tes_data(
        RE::BGSKeyword, tbl[StaggerSystem][PoiseDamageImmunityId].value<RE::FormID>().value(), config.mod_name());
    poise_regeneration_ = get_tes_data(
        RE::BGSKeyword, tbl[StaggerSystem][PoiseRegenerationId].value<RE::FormID>().value(), config.mod_name());

    list_small_spells_magick_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListSmallSpellsMagickId].value<RE::FormID>().value(), config.mod_name());
    list_small_spells_melee_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListSmallSpellsMeleeId].value<RE::FormID>().value(), config.mod_name());
    list_small_spells_ranged_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListSmallSpellsRangedId].value<RE::FormID>().value(), config.mod_name());
    list_small_keywords_magick_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListSmallKeywordsMagickId].value<RE::FormID>().value(), config.mod_name());
    list_small_keywords_melee_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListSmallKeywordsMeleeId].value<RE::FormID>().value(), config.mod_name());
    list_small_keywords_ranged_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListSmallKeywordsRangedId].value<RE::FormID>().value(), config.mod_name());

    list_medium_spells_magick_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListMediumSpellsMagickId].value<RE::FormID>().value(), config.mod_name());
    list_medium_spells_melee_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListMediumSpellsMeleeId].value<RE::FormID>().value(), config.mod_name());
    list_medium_spells_ranged_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListMediumSpellsRangedId].value<RE::FormID>().value(), config.mod_name());
    list_medium_keywords_magick_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListMediumKeywordsMagickId].value<RE::FormID>().value(), config.mod_name());
    list_medium_keywords_melee_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListMediumKeywordsMeleeId].value<RE::FormID>().value(), config.mod_name());
    list_medium_keywords_ranged_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListMediumKeywordsRangedId].value<RE::FormID>().value(), config.mod_name());

    list_large_spells_magick_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListLargeSpellsMagickId].value<RE::FormID>().value(), config.mod_name());
    list_large_spells_melee_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListLargeSpellsMeleeId].value<RE::FormID>().value(), config.mod_name());
    list_large_spells_ranged_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListLargeSpellsRangedId].value<RE::FormID>().value(), config.mod_name());
    list_large_keywords_magick_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListLargeKeywordsMagickId].value<RE::FormID>().value(), config.mod_name());
    list_large_keywords_melee_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListLargeKeywordsMeleeId].value<RE::FormID>().value(), config.mod_name());
    list_large_keywords_ranged_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListLargeKeywordsRangedId].value<RE::FormID>().value(), config.mod_name());

    list_largest_spells_magick_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListLargestSpellsMagickId].value<RE::FormID>().value(), config.mod_name());
    list_largest_spells_melee_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListLargestSpellsMeleeId].value<RE::FormID>().value(), config.mod_name());
    list_largest_spells_ranged_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListLargestSpellsRangedId].value<RE::FormID>().value(), config.mod_name());
    list_largest_keywords_magick_ =
        get_tes_data(RE::BGSListForm,
                     tbl[StaggerSystem][ListLargestKeywordsMagickId].value<RE::FormID>().value(),
                     config.mod_name());
    list_largest_keywords_melee_ = get_tes_data(
        RE::BGSListForm, tbl[StaggerSystem][ListLargestKeywordsMeleeId].value<RE::FormID>().value(), config.mod_name());
    list_largest_keywords_ranged_ =
        get_tes_data(RE::BGSListForm,
                     tbl[StaggerSystem][ListLargestKeywordsRangedId].value<RE::FormID>().value(),
                     config.mod_name());
  }
}

auto Config::load() -> void
{
  logger::info("start load config"sv);
  logger::info("start parse toml"sv);
  auto tbl = toml::parse_file(PathToConfig);
  const auto data_handler = RE::TESDataHandler::GetSingleton();

  mod_name_ = tbl[Reflyem][ModName].value_or("Skyrim.esm"sv);
  let cast_on_is_cost_id = tbl[Reflyem][CastOnIsCostKeyword].value_or(0x700);
  cast_on_is_cost_ = data_handler->LookupForm<RE::BGSKeyword>(cast_on_is_cost_id, mod_name_);
  let cast_on_caster_swap_id = tbl[Reflyem][CastOnCasterSwapKeyword].value_or(0x700);
  cast_on_caster_swap_ = data_handler->LookupForm<RE::BGSKeyword>(cast_on_caster_swap_id, mod_name_);
  let restrict_equip_shield_id = tbl[Reflyem][RestrictEquipShieldKeyword].value_or(0x700);
  restrict_equip_shield_ = data_handler->LookupForm<RE::BGSKeyword>(restrict_equip_shield_id, mod_name_);
  let restrict_equip_offhand_id = tbl[Reflyem][RestrictEquipOffhandKeyword].value_or(0x700);
  restrict_equip_offhand_ = data_handler->LookupForm<RE::BGSKeyword>(restrict_equip_offhand_id, mod_name_);
  start_attack_duration_ = tbl[Reflyem][StartAttackDuration].value_or(0.2f);
  volume_override_ = tbl[Reflyem][VolumeOverride].value_or(0.6f);

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
  cast_on_soul_trap_ = CastOnSoulTrapConfig{tbl, *data_handler, *this};
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
  decluttered_loot_ = DeclutteredLootConfig{tbl, *data_handler, *this};
  cast_on_dodge_ = CastOnDodgeConfig{tbl, *data_handler, *this};
  slow_time_ = SlowTimeConfig{tbl, *data_handler, *this};
  special_techniques_ = SpecialTechniquesConfig{tbl, *data_handler, *this};
  highlighting_ = HighlightingConfig{tbl, *data_handler, *this};
  magic_blocker_config_ = MagicBlockerConfig{tbl, *data_handler, *this};
  cast_on_magic_blocker_ = CastOnMagicBlockerConfig{tbl, *data_handler, *this};
  ki_energy_power_ = KiEnergyPowerConfig{tbl, *data_handler, *this};
  clairvoyance_ = ClairvoyanceConfig{tbl, *data_handler, *this};
  cast_on_magic_projectile_hit_ = CastOnMagicProjectileHitConfig{tbl, *data_handler, *this};
  cast_on_timing_block_stagger_ = CastOnTimingBlockStaggerConfig{tbl, *data_handler, *this};
  cast_on_get_hit_ = CastOnGetHitConfig{tbl, *data_handler, *this};
  cast_on_magic_projectile_get_hit_ = CastOnMagicProjectileGetHitConfig{tbl, *data_handler, *this};
  absorb_shield_ = AbsorbShieldConfig{tbl, *data_handler, *this};
  recoup_effect_ = RecoupEffectConfig{tbl, *data_handler, *this};
  leech_effect_ = LeechEffectConfig{tbl, *data_handler, *this};
  spell_strike_effect_ = SpellStrikeEffectConfig{tbl, *data_handler, *this};
  bound_spell_swap_ = BoundSpellSwapConfig{tbl, *data_handler, *this};
  stagger_system_ = StaggerSystemConfig{tbl, *data_handler, *this};

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
