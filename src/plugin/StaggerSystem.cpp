#include "StaggerSystem.hpp"

namespace Reflyem::StaggerSystem {

static const RE::BSFixedString STR_STAGGER_START = "staggerStart";
static const RE::BSFixedString STR_STAGGER_DIRECTION = "staggerDirection";
static const RE::BSFixedString STR_STAGGER_MAGNITUDE = "staggerMagnitude";

static const RE::BSFixedString STR_POISE_SMALL_BWD = "poise_small_start";
static const RE::BSFixedString STR_POISE_SMALL_FWD = "poise_small_start_fwd";

static const RE::BSFixedString STR_POISE_MEDIUM_BWD = "poise_med_start";
static const RE::BSFixedString STR_POISE_MEDIUM_FWD = "poise_med_start_fwd";

static const RE::BSFixedString STR_POISE_LARGE_BWD = "poise_large_start";
static const RE::BSFixedString STR_POISE_LARGE_FWD = "poise_large_start_fwd";

static const RE::BSFixedString STR_POISE_LARGEST_BWD = "poise_largest_start";
static const RE::BSFixedString STR_POISE_LARGEST_FWD = "poise_largest_start_fwd";

static constexpr float STAGGER_MAGNITUDE_SMALL = 0.25f;
static constexpr float STAGGER_MAGNITUDE_MEDIUM = 0.5f;
static constexpr float STAGGER_MAGNITUDE_LARGE = 0.75f;
static constexpr float STAGGER_MAGNITUDE_LARGEST = 1.f;

enum class StaggerPower
{
  kSmall,
  kMedium,
  kLarge,
  kLargest
};

enum class DamagePoiseResult
{
  kNothing,
  kSmallStagger,
  kMediumStagger,
  kLargeStagger,
  kLargestStagger
};

enum class StaggerSource
{
  kMagicHit,
  kMeleeWeaponHit,
  kRangedWeaponHit
};

auto is_forward_stagger(const float stagger_direction) -> bool
{
  if (stagger_direction > 0.25f && stagger_direction < 0.75f) {
    return true;
  }

  return false;
}

auto get_poise_damage_cooldown(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();

  let poise_damage_cooldown_effects =
      Core::try_get_effects_by_keyword(actor, config.stagger_system().poise_damage_cooldown());
  let poise_damage_cooldown = Core::get_effects_magnitude_sum(poise_damage_cooldown_effects).value_or(0.f);

  return poise_damage_cooldown;
}

auto get_current_poise_damage_cooldown(const RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);

  return actor_data.poise_damage_cooldown();
}

auto set_poise_damage_cooldown(const RE::Actor* actor, const float new_value) -> void
{

  if (!actor) {
    return;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);
  actor_data.poise_damage_cooldown(new_value);
}

auto get_poise_regeneration(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();

  let poise_regeneration_effects =
      Core::try_get_effects_by_keyword(actor, config.stagger_system().poise_regeneration());
  let poise_regeneration = Core::get_effects_magnitude_sum(poise_regeneration_effects).value_or(0.f);

  return poise_regeneration;
}

auto is_immunity_to_damage_poise(RE::Actor* actor) -> bool
{
  if (!actor) {
    return true;
  }

  if (get_current_poise_damage_cooldown(actor) > 0.f) {
    return true;
  }

  letr config = Config::get_singleton();
  if (Core::try_has_absolute_keyword(actor, config.stagger_system().poise_damage_immunity())) {
    return true;
  }

  return false;
}

auto is_projectile_damage_poise(const RE::Projectile* projectile, const RE::Actor* target, const RE::Actor* attacker)
    -> bool
{
  if (!projectile) {
    return false;
  }

  if (target && attacker && target == attacker) {
    return false;
  }

  if (!projectile->spell) {
    return false;
  }

  for (auto effect : projectile->spell->effects) {
    if (!effect) {
      continue;
    }

    if (!effect->baseEffect) {
      continue;
    }

    let base_effect = effect->baseEffect;
    if (base_effect->IsDetrimental() &&
        !base_effect->data.flags.any(RE::EffectSetting::EffectSettingData::Flag::kRecover) &&
        (base_effect->data.primaryAV == RE::ActorValue::kHealth ||
         base_effect->data.secondaryAV == RE::ActorValue::kHealth)) {
      return true;
    }
  }

  return false;
}

auto get_base_poise_damage(RE::Actor* attacker,
                           RE::Actor* target,
                           RE::TESObjectWEAP* weapon,
                           RE::Projectile* projectile,
                           const bool is_power_attack = false) -> float
{

  static constexpr float TWO_HANDED_POWER_POISE_DAMAGE = 4.f;
  static constexpr float ONE_HANDED_POWER_POISE_DAMAGE = 2.f;
  static constexpr float TWO_HANDED_POISE_DAMAGE = 1.f;
  static constexpr float ONE_HANDED_POISE_DAMAGE = 0.5f;
  static constexpr float MAGIC_HIT_POISE_DAMAGE = 0.5;
  static constexpr float MAGIC_HIT_DUAL_POISE_DAMAGE = 2.f;
  static constexpr float BOW_POISE_DAMAGE = 0.5f;
  static constexpr float CROSSBOW_POISE_DAMAGE = 1.f;
  static constexpr float NONE_WEAPON_NONE_PROJECTILE_POISE_DAMAGE = 0.5f;
  static constexpr float POWER_NONE_WEAPON_NONE_PROJECTILE_POISE_DAMAGE = 2.f;
  static constexpr float BLOCKING_POISE_MULT = 0.5f;

  auto poise_damage = 0.f;
  if (weapon) {
    if (!weapon->IsRanged()) {
      let one_handed = is_power_attack ? ONE_HANDED_POWER_POISE_DAMAGE : ONE_HANDED_POISE_DAMAGE;
      let two_handed = is_power_attack ? TWO_HANDED_POWER_POISE_DAMAGE : TWO_HANDED_POISE_DAMAGE;
      poise_damage = poise_damage + (Core::object_is_two_handed_weapon(weapon) ? two_handed : one_handed);
    }
    if (weapon->IsRanged() && !weapon->IsStaff()) {
      poise_damage = poise_damage + (weapon->IsBow() ? BOW_POISE_DAMAGE : CROSSBOW_POISE_DAMAGE);
    }
  }

  if (projectile && !projectile->weaponSource && projectile->spell &&
      is_projectile_damage_poise(projectile, target, attacker)) {

    auto is_concentration_spell = false;
    if (projectile->spell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration) {
      is_concentration_spell = true;
    }

    auto poise_mult = 1.f;
    if (is_concentration_spell && projectile->GetProjectileBase()) {
      poise_mult = projectile->GetProjectileBase()->data.relaunchInterval;
    }

    if (poise_mult > 1.f) {
      poise_mult = 1.f;
    }

    poise_damage =
        poise_damage + (projectile->flags.any(RE::Projectile::Flags::kIsDual) ? MAGIC_HIT_DUAL_POISE_DAMAGE * poise_mult
                                                                              : MAGIC_HIT_POISE_DAMAGE * poise_mult);
  }

  if (!projectile && !weapon) {
    poise_damage = poise_damage + (is_power_attack ? POWER_NONE_WEAPON_NONE_PROJECTILE_POISE_DAMAGE
                                                   : NONE_WEAPON_NONE_PROJECTILE_POISE_DAMAGE);
  }

  if (target && Core::is_blocking(target)) {
    poise_damage = poise_damage * BLOCKING_POISE_MULT;
  }

  return poise_damage;
}

auto get_stagger_str(const float stagger_direction, const StaggerPower stagger_power, const bool is_normal_behavior)
    -> RE::BSFixedString
{

  if (is_normal_behavior) {
    return STR_STAGGER_START;
  }

  let is_forward = is_forward_stagger(stagger_direction);

  switch (stagger_power) {

    case StaggerPower::kSmall: {
      return is_forward ? STR_POISE_SMALL_FWD : STR_POISE_SMALL_BWD;
    }
    case StaggerPower::kMedium: {
      return is_forward ? STR_POISE_MEDIUM_FWD : STR_POISE_MEDIUM_BWD;
    }
    case StaggerPower::kLarge: {
      return is_forward ? STR_POISE_LARGE_FWD : STR_POISE_LARGE_BWD;
    }
    case StaggerPower::kLargest: {
      return is_forward ? STR_POISE_LARGEST_FWD : STR_POISE_LARGEST_BWD;
    }
  }

  loge("Not valid stagger power value");
  return STR_POISE_SMALL_BWD;
}

auto get_current_poise_value(const RE::Actor* actor) -> float
{

  if (!actor) {
    return 0.f;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);

  logd("StaggerSystem GetCurrentPoiseValue: {}", actor_data.poise_value());

  return actor_data.poise_value();
}

auto get_max_poise_value(RE::Actor* actor) -> float
{

  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();
  let poise_value_effects = Core::try_get_effects_by_keyword(actor, config.stagger_system().poise_value());
  let poise_value = Core::get_effects_magnitude_sum(poise_value_effects).value_or(0.f);

  logd("StaggerSystem GetMaxPoiseValue: {}", poise_value);

  return poise_value;
}

auto set_poise_value(RE::Actor* actor, float new_value) -> void
{

  if (!actor) {
    return;
  }

  let max_value = get_max_poise_value(actor);
  if (new_value < 0.f) {
    new_value = 0.f;
  }
  if (new_value > max_value) {
    new_value = max_value;
  }

  logd("StaggerSystem SetPoiseValue: {}", new_value);

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);
  actor_data.poise_value(new_value);
}

auto get_threshold_poise_value(RE::Actor* actor, const StaggerPower stagger_power) -> float
{

  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();

  switch (stagger_power) {

    case StaggerPower::kSmall: {
      let small_poise_effects = Core::try_get_effects_by_keyword(actor, config.stagger_system().poise_small());
      let small_poise = Core::get_effects_magnitude_sum(small_poise_effects).value_or(0.f);
      return small_poise;
    }
    case StaggerPower::kMedium: {
      let medium_poise_effects = Core::try_get_effects_by_keyword(actor, config.stagger_system().poise_medium());
      let medium_poise = Core::get_effects_magnitude_sum(medium_poise_effects).value_or(0.f);
      return medium_poise;
    }
    case StaggerPower::kLarge: {
      let large_poise_effects = Core::try_get_effects_by_keyword(actor, config.stagger_system().poise_large());
      let large_poise = Core::get_effects_magnitude_sum(large_poise_effects).value_or(0.f);
      return large_poise;
    }
    case StaggerPower::kLargest: {
      let largest_poise_effects = Core::try_get_effects_by_keyword(actor, config.stagger_system().poise_largest());
      let largest_poise = Core::get_effects_magnitude_sum(largest_poise_effects).value_or(0.f);
      return largest_poise;
    }
  }

  return 0.f;
}

auto is_normal_behavior_for_actor(RE::Actor* actor) -> bool
{

  let data_handler = RE::TESDataHandler::GetSingleton();
  if (!data_handler) {
    return true;
  }

  let creature_keyword = data_handler->LookupForm<RE::BGSKeyword>(0x13795, "Skyrim.esm");
  let dwarven_keyword = data_handler->LookupForm<RE::BGSKeyword>(0x1397a, "Skyrim.esm");

  if (Core::try_has_absolute_keyword(actor, creature_keyword) ||
      Core::try_has_absolute_keyword(actor, dwarven_keyword)) {
    return true;
  }

  return false;
}

auto get_max_actions_count(RE::Actor* actor) -> std::uint8_t
{
  if (!actor) {
    return 0;
  }

  letr config = Config::get_singleton();

  let actions_count_effects = Core::try_get_effects_by_keyword(actor, config.stagger_system().poise_recovery_actions());
  let actions_count = static_cast<std::uint8_t>(Core::get_effects_magnitude_sum(actions_count_effects).value_or(0));

  return actions_count;
}

auto get_current_actions_count(const RE::Actor* actor) -> std::uint8_t
{
  if (!actor) {
    return 0;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);

  return actor_data.poise_recovery_actions();
}

auto set_actions_count(RE::Actor* actor, std::uint8_t new_count) -> void
{
  if (!actor) {
    return;
  }

  let max_count = get_max_actions_count(actor);
  if (new_count > max_count) {
    new_count = max_count;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);
  actor_data.poise_recovery_actions(new_count);
}

auto is_poise_in_recovery_mode(const RE::Actor* actor) -> bool
{
  if (!actor) {
    return false;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);

  return actor_data.poise_recovery_time() > 0.f;
}

auto set_poise_recovery_mode(const RE::Actor* actor, const bool new_mode) -> void
{
  if (!actor) {
    return;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);

  actor_data.poise_in_recovery(new_mode);
}

auto get_poise_recovery_time(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();

  let poise_recovery_time_effects =
      Core::try_get_effects_by_keyword(actor, config.stagger_system().poise_recovery_time());
  let poise_recovery_time = Core::get_effects_magnitude_sum(poise_recovery_time_effects).value_or(0.f);

  return poise_recovery_time;
}

auto get_current_poise_recovery_time(const RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);

  return actor_data.poise_recovery_time();
}

auto set_poise_recovery_time(const RE::Actor* actor, const float time) -> void
{
  if (!actor) {
    return;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);

  actor_data.poise_recovery_time(time);
}

auto damage_poise_value(RE::Actor* actor, const float damage_value) -> DamagePoiseResult
{

  auto result = DamagePoiseResult::kNothing;

  if (damage_value <= 0.f) {
    return result;
  }

  if (!actor) {
    return result;
  }

  let current_poise_value = get_current_poise_value(actor);

  if (current_poise_value <= 0.f) {
    return result;
  }

  let small_threshold = get_threshold_poise_value(actor, StaggerPower::kSmall);
  let medium_threshold = get_threshold_poise_value(actor, StaggerPower::kMedium);
  let large_threshold = get_threshold_poise_value(actor, StaggerPower::kLarge);
  let largest_threshold = get_threshold_poise_value(actor, StaggerPower::kLargest);

  auto new_poise_value = current_poise_value - damage_value;

  if (current_poise_value > small_threshold && new_poise_value <= small_threshold) {
    new_poise_value = small_threshold;
    result = DamagePoiseResult::kSmallStagger;
  } else if (current_poise_value > medium_threshold && new_poise_value <= medium_threshold) {
    new_poise_value = medium_threshold;
    result = DamagePoiseResult::kMediumStagger;
  } else if (current_poise_value > large_threshold && new_poise_value <= large_threshold) {
    new_poise_value = large_threshold;
    result = DamagePoiseResult::kLargeStagger;
  } else if (current_poise_value > largest_threshold && new_poise_value <= largest_threshold) {
    new_poise_value = largest_threshold;
    result = DamagePoiseResult::kLargestStagger;
  }

  set_poise_value(actor, new_poise_value);
  return result;
}

auto get_stagger_magnitude_from_damage_poise_result(const DamagePoiseResult damage_result) -> std::optional<float>
{
  switch (damage_result) {

    case DamagePoiseResult::kNothing: {
      return std::nullopt;
    }
    case DamagePoiseResult::kSmallStagger: {
      return STAGGER_MAGNITUDE_SMALL;
    }
    case DamagePoiseResult::kMediumStagger: {
      return STAGGER_MAGNITUDE_MEDIUM;
    }
    case DamagePoiseResult::kLargeStagger: {
      return STAGGER_MAGNITUDE_LARGE;
    }
    case DamagePoiseResult::kLargestStagger: {
      return STAGGER_MAGNITUDE_LARGEST;
    }
  }

  loge("incorrect damage poise result");
  return std::nullopt;
}

auto get_stagger_power_from_magnitude(const float stagger_magnitude) -> StaggerPower
{
  if (stagger_magnitude <= STAGGER_MAGNITUDE_SMALL) {
    return StaggerPower::kSmall;
  }
  if (stagger_magnitude <= STAGGER_MAGNITUDE_MEDIUM) {
    return StaggerPower::kMedium;
  }
  if (stagger_magnitude <= STAGGER_MAGNITUDE_LARGE) {
    return StaggerPower::kLarge;
  }
  if (stagger_magnitude <= STAGGER_MAGNITUDE_LARGEST) {
    return StaggerPower::kLargest;
  }
  loge("Not valid stagger magnitude value");
  return StaggerPower::kLargest;
}

auto get_stagger_direction(RE::Actor* attacker, RE::Actor* target) -> float
{
  if (!attacker || !target) {
    return 0.f;
  }

  if (attacker->GetHandle() == target->GetHandle()) {
    return 0.f;
  }

  let hit_pos = attacker->GetPosition();
  let heading = target->GetHeadingAngle(hit_pos, false);
  let stagger_direction = heading >= 0.f ? heading / 360.f : (360.f + heading) / 360.f;

  return stagger_direction;
}

namespace TrueHUD {

auto flash_special_bar(RE::Actor* actor) -> void
{
  if (actor && Core::ExternalApi::true_hud_api) {
    Core::ExternalApi::true_hud_api->FlashActorSpecialBar(SKSE::GetPluginHandle(), actor->GetHandle(), true);
  }
}

auto get_max_resource_special_bar(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  logd("StaggerSystem Call max resource");

  return get_max_poise_value(actor);
}

auto get_current_resource_special_bar(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  if (is_immunity_to_damage_poise(actor)) {
    return 0.f;
  }

  logd("StaggerSystem get current resource");

  return get_current_poise_value(actor);
}

}

auto cast_on_stagger(RE::Actor* target,
                     RE::Actor* attacker,
                     const StaggerSource stagger_source,
                     const StaggerPower stagger_power) -> void
{
  if (!target || !attacker) {
    return;
  }

  letr config = Config::get_singleton();

  RE::BGSListForm* spells = nullptr;
  RE::BGSListForm* keywords = nullptr;

  switch (stagger_power) {

    case StaggerPower::kSmall: {
      switch (stagger_source) {
        case StaggerSource::kMagicHit: {
          spells = config.stagger_system().list_small_spells_magick();
          keywords = config.stagger_system().list_small_keywords_magick();
          break;
        }
        case StaggerSource::kMeleeWeaponHit: {
          spells = config.stagger_system().list_small_spells_melee();
          keywords = config.stagger_system().list_small_keywords_melee();
          break;
        }
        case StaggerSource::kRangedWeaponHit: {
          spells = config.stagger_system().list_small_spells_ranged();
          keywords = config.stagger_system().list_small_keywords_ranged();
          break;
        }
      }
      break;
    }
    case StaggerPower::kMedium: {
      switch (stagger_source) {
        case StaggerSource::kMagicHit: {
          spells = config.stagger_system().list_medium_spells_magick();
          keywords = config.stagger_system().list_medium_keywords_magick();
          break;
        }
        case StaggerSource::kMeleeWeaponHit: {
          spells = config.stagger_system().list_medium_spells_melee();
          keywords = config.stagger_system().list_medium_keywords_melee();
          break;
        }
        case StaggerSource::kRangedWeaponHit: {
          spells = config.stagger_system().list_medium_spells_ranged();
          keywords = config.stagger_system().list_medium_keywords_ranged();
          break;
        }
      }
      break;
    }
    case StaggerPower::kLarge: {
      switch (stagger_source) {
        case StaggerSource::kMagicHit: {
          spells = config.stagger_system().list_large_spells_magick();
          keywords = config.stagger_system().list_large_keywords_magick();
          break;
        }
        case StaggerSource::kMeleeWeaponHit: {
          spells = config.stagger_system().list_large_spells_melee();
          keywords = config.stagger_system().list_large_keywords_melee();
          break;
        }
        case StaggerSource::kRangedWeaponHit: {
          spells = config.stagger_system().list_large_spells_ranged();
          keywords = config.stagger_system().list_large_keywords_ranged();
          break;
        }
      }
      break;
    }
    case StaggerPower::kLargest: {
      switch (stagger_source) {
        case StaggerSource::kMagicHit: {
          spells = config.stagger_system().list_largest_spells_magick();
          keywords = config.stagger_system().list_largest_keywords_magick();
          break;
        }
        case StaggerSource::kMeleeWeaponHit: {
          spells = config.stagger_system().list_largest_spells_melee();
          keywords = config.stagger_system().list_largest_keywords_melee();
          break;
        }
        case StaggerSource::kRangedWeaponHit: {
          spells = config.stagger_system().list_largest_spells_ranged();
          keywords = config.stagger_system().list_largest_keywords_ranged();
          break;
        }
      }
      break;
    }
  }

  Core::cast_on_handle_formlists(keywords, spells, attacker, target);
}

auto play_stagger(RE::Actor* actor,
                  RE::Actor* attacker,
                  const float stagger_direction,
                  const float stagger_magnitude,
                  const bool is_normal_behavior,
                  const StaggerSource stagger_source) -> void
{

  if (!actor) {
    return;
  }

  let stagger_power = get_stagger_power_from_magnitude(stagger_magnitude);
  let str_stagger = get_stagger_str(stagger_direction, stagger_power, is_normal_behavior);

  actor->SetGraphVariableFloat(STR_STAGGER_DIRECTION, stagger_direction);
  if (is_normal_behavior) {
    actor->SetGraphVariableFloat(STR_STAGGER_MAGNITUDE, stagger_magnitude);
  }
  cast_on_stagger(actor, attacker, stagger_source, stagger_power);
  actor->NotifyAnimationGraph(str_stagger);
  TrueHUD::flash_special_bar(actor);
}

auto is_allow_to_damage_poise(RE::Actor* actor) -> bool
{
  if (!actor) {
    return false;
  }

  if (is_immunity_to_damage_poise(actor)) {
    return false;
  }

  if (is_poise_in_recovery_mode(actor)) {
    return false;
  }

  return true;
}

auto is_allow_to_damage_poise_from_weapon_hit(const RE::HitData& hit_data, const RE::Actor* target) -> bool
{
  // if (hit_data.flags.any(RE::HitData::Flag::kTimedBash, RE::HitData::Flag::kBash)) {
  //   return false;
  // }

  if (target && target->IsDead()) {
    return false;
  }

  // if (hit_data.weapon && !hit_data.weapon->IsRanged()) {
  //   if (!hit_data.flags.any(RE::HitData::Flag::kPowerAttack)) {
  //     return false;
  //   }
  // }

  return true;
}

auto is_allow_to_damage_poise_from_magic_hit(const RE::TESObjectREFR* refr_target) -> bool
{
  if (refr_target && refr_target->As<RE::Actor>()) {
    return true;
  }
  return false;
}

auto hit_handle(RE::Actor* attacker,
                RE::Actor* target,
                RE::TESObjectWEAP* weapon,
                RE::Projectile* projectile,
                const StaggerSource stagger_source,
                const bool is_power_attack = false) -> void
{
  let base_poise_damage = get_base_poise_damage(attacker, target, weapon, projectile, is_power_attack);

  let damage_result = damage_poise_value(target, base_poise_damage);
  let stagger_magnitude = get_stagger_magnitude_from_damage_poise_result(damage_result);
  if (stagger_magnitude.has_value()) {
    let stagger_direction = get_stagger_direction(attacker, target);
    let is_normal_beh = is_normal_behavior_for_actor(target);
    play_stagger(target, attacker, stagger_direction, stagger_magnitude.value(), is_normal_beh, stagger_source);
    if (get_current_poise_value(target) <= 0.f) {
      set_poise_recovery_time(target, get_poise_recovery_time(target));
      set_actions_count(target, 0);
      set_poise_recovery_mode(target, true);
      return;
    }

    set_poise_damage_cooldown(target, get_poise_damage_cooldown(target));
  }
}

auto action_event_handler(const SKSE::ActionEvent& event, RE::BSTEventSource<SKSE::ActionEvent>&) -> void
{
  static constexpr std::uint8_t VOICE_ACTION = 2;
  static constexpr std::uint8_t SPELL_ACTION = 1;
  static constexpr std::uint8_t SWING_ACTION = 1;
  static constexpr std::uint8_t BOW_ACTION = 2;

  if (!event.actor || !is_poise_in_recovery_mode(event.actor)) {
    return;
  }

  switch (event.type.get()) {
    case SKSE::ActionEvent::Type::kSpellFire: {
      set_actions_count(event.actor, get_current_actions_count(event.actor) + SPELL_ACTION);
      break;
    }
    case SKSE::ActionEvent::Type::kWeaponSwing: {
      set_actions_count(event.actor, get_current_actions_count(event.actor) + SWING_ACTION);
      break;
    }
    case SKSE::ActionEvent::Type::kVoiceFire: {
      set_actions_count(event.actor, get_current_actions_count(event.actor) + VOICE_ACTION);
      break;
    }
    case SKSE::ActionEvent::Type::kBowRelease: {
      set_actions_count(event.actor, get_current_actions_count(event.actor) + BOW_ACTION);
      break;
    }
    case SKSE::ActionEvent::Type::kSpellCast:
    case SKSE::ActionEvent::Type::kVoiceCast:
    case SKSE::ActionEvent::Type::kBowDraw:
    case SKSE::ActionEvent::Type::kBeginDraw:
    case SKSE::ActionEvent::Type::kEndDraw:
    case SKSE::ActionEvent::Type::kBeginSheathe:
    case SKSE::ActionEvent::Type::kEndSheathe:
      break;
  }
}

auto update(RE::Actor* actor, const float delta) -> void
{
  if (!actor) {
    return;
  }

  if (!actor->IsInCombat()) {
    let max_poise = get_max_poise_value(actor);
    if (max_poise > get_current_poise_value(actor)) {
      set_poise_value(actor, max_poise);
      TrueHUD::flash_special_bar(actor);
      return;
    }
  }

  if (is_poise_in_recovery_mode(actor)) {
    let current_recovery_time = get_current_poise_recovery_time(actor);
    if (current_recovery_time <= 0.f) {
      set_poise_recovery_mode(actor, false);
      set_actions_count(actor, 0);
      set_poise_value(actor, get_max_poise_value(actor));
      TrueHUD::flash_special_bar(actor);
      return;
    }
    let current_actions_count = get_current_actions_count(actor);
    let max_actions_count = get_max_actions_count(actor);
    if (current_actions_count >= max_actions_count) {
      set_poise_recovery_mode(actor, false);
      set_poise_recovery_time(actor, 0.f);
      set_actions_count(actor, 0);
      set_poise_value(actor, get_max_poise_value(actor));
      TrueHUD::flash_special_bar(actor);
      return;
    }
    return;
  }

  if (get_current_poise_value(actor) <= 0.f) {
    set_poise_value(actor, get_max_poise_value(actor));
  }

  set_poise_value(actor, get_current_poise_value(actor) + (get_poise_regeneration(actor) * delta));
}

auto on_magic_hit(RE::Projectile* projectile, RE::TESObjectREFR* refr_target) -> void
{
  if (!is_allow_to_damage_poise_from_magic_hit(refr_target)) {
    return;
  }

  let target = refr_target->As<RE::Actor>();

  if (!is_allow_to_damage_poise(target)) {
    return;
  }

  let shooter_ref = Core::get_shooter_ref_from_projectile(projectile);

  let attacker = shooter_ref ? shooter_ref->As<RE::Actor>() : nullptr;

  hit_handle(attacker, target, nullptr, projectile, StaggerSource::kMagicHit);
}

auto on_weapon_hit(const RE::HitData& hit_data, RE::Actor* target) -> void
{

  if (!is_allow_to_damage_poise_from_weapon_hit(hit_data, target)) {
    return;
  }

  if (!is_allow_to_damage_poise(target)) {
    return;
  }

  let attacker = hit_data.aggressor ? hit_data.aggressor.get() ? hit_data.aggressor.get().get() : nullptr : nullptr;

  let is_ranged_weapon = hit_data.weapon ? hit_data.weapon->IsRanged() : false;
  let stagger_source = is_ranged_weapon ? StaggerSource::kRangedWeaponHit : StaggerSource::kMeleeWeaponHit;

  hit_handle(
      attacker, target, hit_data.weapon, nullptr, stagger_source, hit_data.flags.any(RE::HitData::Flag::kPowerAttack));
}
}
