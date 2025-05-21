#include "AbsorbShield.hpp"
#include "Core.hpp"
#include "TimingBlock.hpp"

namespace Reflyem::AbsorbShield {

enum DelayType
{
  kAfterMagick,
  kAfterWeapon,
  kAfterCastCost,
};

auto play_impacts(RE::Actor* actor) -> void
{

  if (!actor) {
    return;
  }

  letr config = Config::get_singleton();
  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);

  if (config.absorb_shield().enable_sound() && config.absorb_shield().hit_sound() &&
      actor_data.absorb_shield_sound_cooldown() <= 0.f) {
    Core::play_sound(config.absorb_shield().hit_sound(), actor);
    actor_data.absorb_shield_sound_cooldown(config.absorb_shield().sound_cooldown());
  }

  if (config.absorb_shield().enable_effect() && config.absorb_shield().hit_effect() &&
      actor_data.absorb_shield_effect_cooldown() <= 0.f) {
    TimingBlock::place_form_at_me(actor, config.absorb_shield().hit_effect());
    actor_data.absorb_shield_effect_cooldown(config.absorb_shield().effect_cooldown());
  }
}

auto is_actor_ignore_absorb_shield(RE::Actor* actor) -> bool
{

  letr config = Config::get_singleton();
  if (Core::try_has_absolute_keyword(actor, config.absorb_shield().keyword_actor_ignore_shield())) {
    return true;
  }

  return false;
}

auto is_form_ignore_absorb_shield(const RE::BGSKeywordForm* form) -> bool
{

  letr config = Config::get_singleton();
  if (form && config.absorb_shield().keyword_actor_ignore_shield() &&
      form->HasKeyword(config.absorb_shield().keyword_form_ignore_shield())) {
    return true;
  }

  return false;
}

auto is_enable_weapon_shield(RE::Actor* actor) -> bool
{
  letr config = Config::get_singleton();
  if (Core::try_has_absolute_keyword(actor, config.absorb_shield().keyword_shield_enable_weapon())) {
    return true;
  }

  return false;
}

auto is_enable_magick_shield(RE::Actor* actor) -> bool
{
  letr config = Config::get_singleton();
  if (Core::try_has_absolute_keyword(actor, config.absorb_shield().keyword_shield_enable_magick())) {
    return true;
  }

  return false;
}

auto is_enable_drain_shield_threshold(RE::Actor* actor) -> bool
{
  letr config = Config::get_singleton();
  if (Core::try_has_absolute_keyword(actor, config.absorb_shield().keyword_shield_value_drain_threshold_enable())) {
    return true;
  }

  return false;
}

auto is_enable_shield_value_from_cast_cost(RE::Actor* actor) -> bool
{
  letr config = Config::get_singleton();
  if (Core::try_has_absolute_keyword(actor, config.absorb_shield().keyword_shield_value_from_cast_cost_enable())) {
    return true;
  }

  return false;
}

auto is_enable_shield_value_degen_delay_weapon(RE::Actor* actor) -> bool
{
  letr config = Config::get_singleton();
  if (Core::try_has_absolute_keyword(
          actor, config.absorb_shield().keyword_shield_value_degen_delay_after_weapon_hit_enable())) {
    return true;
  }

  return false;
}

auto is_enable_shield_value_degen_delay_magick(RE::Actor* actor) -> bool
{
  letr config = Config::get_singleton();
  if (Core::try_has_absolute_keyword(
          actor, config.absorb_shield().keyword_shield_value_degen_delay_after_magick_hit_enable())) {
    return true;
  }

  return false;
}

auto is_enable_shield_value_degen_delay_cast_cost(RE::Actor* actor) -> bool
{
  letr config = Config::get_singleton();
  if (Core::try_has_absolute_keyword(
          actor, config.absorb_shield().keyword_shield_value_degen_delay_after_from_cast_cost_hit_enable())) {
    return true;
          }

  return false;
}

auto get_percent_damage_for_shield_from_magic_item(RE::MagicItem* magic_item) -> float
{

  letr config = Config::get_singleton();

  if (!magic_item || !config.absorb_shield().keyword_shield_value_magic_inject()) {
    return 0.f;
  }

  auto value = 0.f;

  for (const auto effect : magic_item->effects) {
    if (effect && effect->baseEffect &&
        effect->baseEffect->HasKeyword(config.absorb_shield().keyword_shield_value_magic_inject())) {
      value += std::abs(effect->GetMagnitude());
    }
  }

  return value;
}

auto get_percent_damage_for_shield_from_weapon_enchant(RE::EnchantmentItem* enchanting_item) -> float
{

  letr config = Config::get_singleton();

  if (!enchanting_item || !config.absorb_shield().keyword_shield_value_weapon_inject()) {
    return 0.f;
  }

  auto value = 0.f;

  for (const auto effect : enchanting_item->effects) {
    if (effect && effect->baseEffect &&
        effect->baseEffect->HasKeyword(config.absorb_shield().keyword_shield_value_weapon_inject())) {
      value += std::abs(effect->GetMagnitude());
    }
  }

  return value;
}

auto get_percent_damage_for_shield_from_weapon(RE::Actor* actor, const RE::TESEnchantableForm* enchant_form) -> float
{

  letr config = Config::get_singleton();

  let base_gettin_value = config.absorb_shield().base_shield_value_gettin_weapon_damage();

  let effects =
      Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_shield_value_gettin_weapon_damage());
  let gettin_value_from_effects = Core::get_effects_magnitude_sum(effects).value_or(0.f);
  let gettin_value_from_enchant =
      get_percent_damage_for_shield_from_weapon_enchant(enchant_form ? enchant_form->formEnchanting : nullptr);

  return base_gettin_value + gettin_value_from_effects + gettin_value_from_enchant;
}

auto get_percent_damage_for_shield_from_magic(RE::Actor* actor, RE::MagicItem* magic_item) -> float
{

  letr config = Config::get_singleton();

  let base_gettin_value = config.absorb_shield().base_shield_value_gettin_magick_damage();

  let effects =
      Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_shield_value_gettin_magick_damage());
  let gettin_value_from_effects = Core::get_effects_magnitude_sum(effects).value_or(0.f);
  let gettin_value_from_magic_item = get_percent_damage_for_shield_from_magic_item(magic_item);

  logd("AbsorbMagicPercent: Base {} ValueEffects {} ValueMagicItem {}", base_gettin_value, gettin_value_from_effects, gettin_value_from_magic_item);

  return base_gettin_value + gettin_value_from_effects + gettin_value_from_magic_item;
}

auto get_max_shield_value_from_stats(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();

  let base_hp_mult = config.absorb_shield().base_mult_shield_value_from_max_health();
  let effects_mult_hp =
      Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_mult_shield_value_from_max_health());
  let effects_sum_mult_hp = Core::get_effects_magnitude_sum(effects_mult_hp).value_or(0.f);
  let percent_of_max_hp = base_hp_mult + effects_sum_mult_hp;
  let max_shield_value_from_hp = Core::get_actor_value_max(actor, RE::ActorValue::kHealth) * percent_of_max_hp;

  let base_sp_mult = config.absorb_shield().base_mult_shield_value_from_max_stamina();
  let effects_mult_sp =
      Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_mult_shield_value_from_max_stamina());
  let effects_sum_mult_sp = Core::get_effects_magnitude_sum(effects_mult_sp).value_or(0.f);
  let percent_of_max_sp = base_sp_mult + effects_sum_mult_sp;
  let max_shield_value_from_sp = Core::get_actor_value_max(actor, RE::ActorValue::kStamina) * percent_of_max_sp;

  let base_mp_mult = config.absorb_shield().base_mult_shield_value_from_max_magicka();
  let effects_mult_mp =
      Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_mult_shield_value_from_max_magicka());
  let effects_sum_mult_mp = Core::get_effects_magnitude_sum(effects_mult_mp).value_or(0.f);
  let percent_of_max_mp = base_mp_mult + effects_sum_mult_mp;
  let max_shield_value_from_mp = Core::get_actor_value_max(actor, RE::ActorValue::kMagicka) * percent_of_max_mp;

  let base_armor_mult = config.absorb_shield().base_mult_shield_value_from_armor();
  let effects_mult_armor =
      Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_mult_shield_value_from_armor());
  let effects_sum_mult_armor = Core::get_effects_magnitude_sum(effects_mult_armor).value_or(0.f);
  let percent_of_max_armor = base_armor_mult + effects_sum_mult_armor;
  let max_shield_value_from_armor = actor->GetActorValue(RE::ActorValue::kDamageResist) * percent_of_max_armor;

  return max_shield_value_from_hp + max_shield_value_from_armor + max_shield_value_from_mp + max_shield_value_from_sp;
}

auto get_max_shield_flat(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();
  let base_value = config.absorb_shield().base_shield_value();
  let effects = Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_shield_value_flat());
  let sum_effects = Core::get_effects_magnitude_sum(effects).value_or(0.f);

  return base_value + sum_effects;
}

auto get_max_shield_mult(RE::Actor* actor) -> float
{
  if (!actor) {
    return 1.f;
  }

  letr config = Config::get_singleton();
  let effects = Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_shield_value_mult());
  let mult_effects = Core::get_effects_magnitude_mult(effects);

  return mult_effects;
}

auto get_shield_value_degen_delay_flat(RE::Actor* actor, const DelayType delay_type) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();

  auto base_value = 0.f;
  switch (delay_type) {
    case kAfterMagick:{
      base_value = config.absorb_shield().base_shield_value_degen_delay_after_magick_hit_flat();
      break;
  }
    case kAfterWeapon: {
      base_value = config.absorb_shield().base_shield_value_degen_delay_after_weapon_hit_flat();
      break;
    }
    case kAfterCastCost: {
      base_value = config.absorb_shield().base_shield_value_degen_delay_after_from_cast_cost_hit_flat();
      break;
    }
  }

  RE::BGSKeyword* keyword = nullptr;
  switch (delay_type) {
    case kAfterMagick:{
      keyword = config.absorb_shield().keyword_shield_value_degen_delay_after_magick_hit_flat();
      break;
    }
    case kAfterWeapon: {
      keyword = config.absorb_shield().keyword_shield_value_degen_delay_after_weapon_hit_flat();
      break;
    }
    case kAfterCastCost: {
      keyword = config.absorb_shield().keyword_shield_value_degen_delay_after_from_cast_cost_hit_flat();
      break;
    }
  }
  
  let effects = Core::try_get_effects_by_keyword(actor, keyword);
  let sum_effects = Core::get_effects_magnitude_sum(effects).value_or(0.f);

  return base_value + sum_effects;
}

auto get_shield_value_degen_delay_mult(RE::Actor* actor, const DelayType delay_type) -> float
{
  if (!actor) {
    return 1.f;
  }

  letr config = Config::get_singleton();
  
  RE::BGSKeyword* keyword = nullptr;
  switch (delay_type) {
    case kAfterMagick:{
      keyword = config.absorb_shield().keyword_shield_value_degen_delay_after_magick_hit_mult();
      break;
    }
    case kAfterWeapon: {
      keyword = config.absorb_shield().keyword_shield_value_degen_delay_after_weapon_hit_mult();
      break;
    }
    case kAfterCastCost: {
      keyword = config.absorb_shield().keyword_shield_value_degen_delay_after_from_cast_cost_hit_mult();
      break;
    }
  }
  
  let effects = Core::try_get_effects_by_keyword(actor, keyword);
  let mult_effects = Core::get_effects_magnitude_mult(effects);

  return mult_effects;
}

auto get_shield_value_degen_delay(RE::Actor* actor, const DelayType delay_type) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();

  let absolute_max = config.absorb_shield().absolute_max_shield_value_degen_delay();
  let flat = get_shield_value_degen_delay_flat(actor, delay_type);
  let mult = get_shield_value_degen_delay_mult(actor, delay_type);

  let delay = flat * mult;
  if (delay <= 0.f) {
    return 0.f;
  }

  return delay > absolute_max ? absolute_max : delay;
}

auto get_current_shield_value_degen_delay(const RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  const auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);
  return actor_data.absorb_shield_degen_delay();
}

auto set_new_shield_value_degen_delay(const RE::Actor* actor, const float new_delay, const bool papyrus_call = false) -> void
{
  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);
  letr config = Config::get_singleton();

  let absolute_max = config.absorb_shield().absolute_max_shield_value_degen_delay();

  let current_delay = actor_data.absorb_shield_degen_delay();
  if (current_delay > new_delay && !papyrus_call) {
    return;
  }

  if (new_delay <= 0.f) {
    actor_data.absorb_shield_degen_delay(0.f);
    return;
  }

  actor_data.absorb_shield_degen_delay(new_delay > absolute_max ? absolute_max : new_delay);
}

auto get_shield_from_cast_cost_flat(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();
  let base_value = config.absorb_shield().base_from_cast_cost_flat();
  let effects = Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_from_cast_cost_flat());
  let sum_effects = Core::get_effects_magnitude_sum(effects).value_or(0.f);

  return base_value + sum_effects;
}

auto get_shield_from_cast_cost_percent(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();
  let base_value = config.absorb_shield().base_from_cast_cost_percent();
  let effects = Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_from_cast_cost_percent());
  let sum_effects = Core::get_effects_magnitude_sum(effects).value_or(0.f);

  return base_value + sum_effects;
}

auto get_shield_from_cast_cost_mult(RE::Actor* actor) -> float
{
  if (!actor) {
    return 1.f;
  }

  letr config = Config::get_singleton();
  let effects = Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_from_cast_cost_mult());
  let mult_effects = Core::get_effects_magnitude_mult(effects);

  return mult_effects;
}

auto get_drain_shield_threshold_from_stats(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();

  let base_hp_mult = config.absorb_shield().base_mult_shield_value_drain_threshold_from_max_health();
  let effects_mult_hp = Core::try_get_effects_by_keyword(
      actor, config.absorb_shield().keyword_mult_shield_value_drain_threshold_from_max_health());
  let effects_sum_mult_hp = Core::get_effects_magnitude_sum(effects_mult_hp).value_or(0.f);
  let percent_of_max_hp = base_hp_mult + effects_sum_mult_hp;
  let max_shield_value_from_hp = Core::get_actor_value_max(actor, RE::ActorValue::kHealth) * percent_of_max_hp;

  let base_sp_mult = config.absorb_shield().base_mult_shield_value_drain_threshold_from_max_stamina();
  let effects_mult_sp = Core::try_get_effects_by_keyword(
      actor, config.absorb_shield().keyword_mult_shield_value_drain_threshold_from_max_stamina());
  let effects_sum_mult_sp = Core::get_effects_magnitude_sum(effects_mult_sp).value_or(0.f);
  let percent_of_max_sp = base_sp_mult + effects_sum_mult_sp;
  let max_shield_value_from_sp = Core::get_actor_value_max(actor, RE::ActorValue::kStamina) * percent_of_max_sp;

  let base_mp_mult = config.absorb_shield().base_mult_shield_value_drain_threshold_from_max_magicka();
  let effects_mult_mp = Core::try_get_effects_by_keyword(
      actor, config.absorb_shield().keyword_mult_shield_value_drain_threshold_from_max_magicka());
  let effects_sum_mult_mp = Core::get_effects_magnitude_sum(effects_mult_mp).value_or(0.f);
  let percent_of_max_mp = base_mp_mult + effects_sum_mult_mp;
  let max_shield_value_from_mp = Core::get_actor_value_max(actor, RE::ActorValue::kMagicka) * percent_of_max_mp;

  let base_armor_mult = config.absorb_shield().base_mult_shield_value_drain_threshold_from_armor();
  let effects_mult_armor = Core::try_get_effects_by_keyword(
      actor, config.absorb_shield().keyword_mult_shield_value_drain_threshold_from_armor());
  let effects_sum_mult_armor = Core::get_effects_magnitude_sum(effects_mult_armor).value_or(0.f);
  let percent_of_max_armor = base_armor_mult + effects_sum_mult_armor;
  let max_shield_value_from_armor = actor->GetActorValue(RE::ActorValue::kDamageResist) * percent_of_max_armor;

  return max_shield_value_from_hp + max_shield_value_from_armor + max_shield_value_from_mp + max_shield_value_from_sp;
}

auto get_drain_shield_threshold_flat(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();
  let base_value = config.absorb_shield().base_shield_value_drain_threshold_flat();
  let effects =
      Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_shield_value_drain_threshold_flat());
  let sum_effects = Core::get_effects_magnitude_sum(effects).value_or(0.f);

  return base_value + sum_effects;
}

auto get_drain_shield_threshold_percent(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();
  let base_value = config.absorb_shield().base_shield_value_drain_threshold_percent();
  let effects =
      Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_shield_value_drain_threshold_percent());
  let sum_effects = Core::get_effects_magnitude_sum(effects).value_or(0.f);

  return base_value + sum_effects;
}

auto get_drain_shield_threshold_mult(RE::Actor* actor) -> float
{
  if (!actor) {
    return 1.f;
  }

  letr config = Config::get_singleton();
  let effects =
      Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_shield_value_drain_threshold_mult());
  let mult_effects = Core::get_effects_magnitude_mult(effects);

  return mult_effects;
}

auto get_drain_shield_flat(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();
  let base_value = config.absorb_shield().base_drain_shield_value_flat();
  let effects = Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_shield_value_drain_flat());
  let sum_effects = Core::get_effects_magnitude_sum(effects).value_or(0.f);

  return base_value + sum_effects;
}

auto get_drain_shield_percent(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();
  let base_value = config.absorb_shield().base_drain_shield_value_percent();
  let effects = Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_shield_value_drain_percent());
  let sum_effects = Core::get_effects_magnitude_sum(effects).value_or(0.f);

  return base_value + sum_effects;
}

auto get_drain_current_shield_percent(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();
  let base_value = config.absorb_shield().base_drain_shield_value_current_percent();
  let effects = Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_shield_value_drain_current_percent());
  let sum_effects = Core::get_effects_magnitude_sum(effects).value_or(0.f);

  return base_value + sum_effects;
}

auto get_drain_shield_mult(RE::Actor* actor) -> float
{
  if (!actor) {
    return 1.f;
  }

  letr config = Config::get_singleton();
  let effects = Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_shield_value_drain_mult());
  let mult_effects = Core::get_effects_magnitude_mult(effects);

  return mult_effects;
}

auto get_max_shield_value(RE::Actor* actor) -> float
{

  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();
  let absolute_max = config.absorb_shield().absolute_max_shield_value();

  let max_flat_base_value = get_max_shield_flat(actor);
  let max_flat_value_from_stats = get_max_shield_value_from_stats(actor);
  let mult_value = get_max_shield_mult(actor);

  let max_value = (max_flat_base_value + max_flat_value_from_stats) * mult_value;

  if (max_value > absolute_max) {
    return absolute_max;
  }
  if (max_value < 0.f) {
    return 0.f;
  }

  return max_value;
}

auto get_drain_shield_threshold_value(RE::Actor* actor) -> float
{

  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();
  let absolute_max = config.absorb_shield().absolute_max_shield_value_drain_threshold();

  let max_shield_value = get_max_shield_value(actor);

  let threshold_flat_value = get_drain_shield_threshold_flat(actor);
  let threshold_flat_value_from_stats = get_drain_shield_threshold_from_stats(actor);
  let threshold_mult = get_drain_shield_threshold_mult(actor);
  let threshold_percent_value = max_shield_value * get_drain_shield_threshold_percent(actor);

  let result = (threshold_flat_value + threshold_flat_value_from_stats + threshold_percent_value) * threshold_mult;

  if (result < 0.f) {
    return result < (absolute_max * -1.f) ? (absolute_max * -1.f) : result;
  }

  return result > absolute_max ? absolute_max : result;
}

auto set_new_shield_value(RE::Actor* actor, const float new_shield) -> void
{
  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);
  let max_shield_value = get_max_shield_value(actor);

  if (new_shield > max_shield_value) {
    actor_data.absorb_shield_value(max_shield_value);
    return;
  }

  if (new_shield < 0.f) {
    actor_data.absorb_shield_value(0.f);
    return;
  }

  actor_data.absorb_shield_value(new_shield);
}

auto get_current_shield_value(const RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  const auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);
  return actor_data.absorb_shield_value();
}

auto get_current_shield_value_as_percent(RE::Actor* actor) -> float
{

  if (!actor) {
    return 0.f;
  }

  let max_shield_value = get_max_shield_value(actor);
  let current_shield_value = get_current_shield_value(actor);

  if (current_shield_value >= max_shield_value) {
    return 100.f;
  }

  if (max_shield_value <= 0.f || current_shield_value <= 0.f) {
    return 0.f;
  }

  let percent = max_shield_value / 100.f;
  return current_shield_value / percent;
}

auto calculate_drain_shield_value(RE::Actor* actor) -> float
{

  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();

  let absolute_max = config.absorb_shield().absolute_max_shield_value_drain();

  let current_shield = get_current_shield_value(actor);
  let max_shield_value = get_max_shield_value(actor);

  let drain_flat = get_drain_shield_flat(actor);
  let drain_percent = max_shield_value * get_drain_shield_percent(actor);
  let drain_current_percent = current_shield * get_drain_current_shield_percent(actor);
  let drain_mult = get_drain_shield_mult(actor);

  let result = (drain_flat + drain_percent + drain_current_percent) * drain_mult;
  if (result < 0.f) {
    return result < (absolute_max * -1.f) ? (absolute_max * -1.f) : result;
  }

  return result > absolute_max ? absolute_max : result;
}

auto drain_shield(RE::Actor* actor, const float drain_value) -> void
{
  if (!actor) {
    return;
  }

  let current_shield = get_current_shield_value(actor);
  let new_shield = current_shield - drain_value;
  let threshold_shield_value = get_drain_shield_threshold_value(actor);
  let is_threshold_enable = is_enable_drain_shield_threshold(actor);
  let current_degen_delay = get_current_shield_value_degen_delay(actor);

  if (current_degen_delay > 0.f) {
    return;
  }

  if (is_threshold_enable && drain_value < 0.f && threshold_shield_value < 0.f &&
      new_shield > std::abs(threshold_shield_value)) {
    return;
  }

  if (is_threshold_enable && drain_value > 0.f && threshold_shield_value > 0.f &&
      new_shield < std::abs(threshold_shield_value)) {
    return;
  }

  set_new_shield_value(actor, new_shield);
}

auto get_mult_damage_to_shield(RE::Actor* actor) -> float
{
  if (!actor) {
    return 1.f;
  }

  letr config = Config::get_singleton();
  let effects = Core::try_get_effects_by_keyword(actor, config.absorb_shield().keyword_damage_to_shield_mult());
  let mult_effects = Core::get_effects_magnitude_mult(effects);

  return mult_effects;
}

auto modify_shield_value(RE::Actor* actor, const float amount) -> void
{

  if (!actor) {
    return;
  }

  let current_shield = get_current_shield_value(actor);
  let new_shield = current_shield + amount;

  set_new_shield_value(actor, new_shield);
}

auto handle_damage_to_shield_base(RE::Actor* shield_owner, float& damage, const float damage_mult) -> void
{

  if (!shield_owner) {
    return;
  }

  let damage_mult_for_div = damage_mult == 0.f ? 1.f : damage_mult;

  let damage_diff_mult = Core::getting_damage_mult(*shield_owner);
  let damage_to_shield = damage * damage_diff_mult * damage_mult;

  let current_shield = get_current_shield_value(shield_owner);

  if (current_shield > 0.f) {
    play_impacts(shield_owner);
  }

  let shield_after_absorb = current_shield - damage_to_shield;
  if (shield_after_absorb >= 0.f) {
    set_new_shield_value(shield_owner, shield_after_absorb);
    damage = 0.f;
    return;
  }

  set_new_shield_value(shield_owner, 0.f);
  damage = std::abs(shield_after_absorb / damage_diff_mult / damage_mult_for_div);
}

auto handle_damage_to_shield(RE::Actor* shield_owner,
                             RE::Actor* attacker,
                             const RE::BGSKeywordForm* form,
                             float& damage) -> void
{
  if (!shield_owner) {
    return;
  }

  if (damage <= 0.f) {
    return;
  }

  if (is_actor_ignore_absorb_shield(attacker)) {
    return;
  }

  if (is_form_ignore_absorb_shield(form)) {
    return;
  }

  let damage_mult = get_mult_damage_to_shield(attacker);
  handle_damage_to_shield_base(shield_owner, damage, damage_mult);
}

auto gettin_absorb_shield_magic(RE::Actor* caster, const RE::ActiveEffect* active_effect, const float value) -> void
{
  if (!caster || !active_effect) {
    return;
  }

  let magic_item = active_effect->spell;
  let gettin_mult = get_percent_damage_for_shield_from_magic(caster, magic_item);
  let value_for_shield = value * gettin_mult;

  let current_shield = get_current_shield_value(caster);
  let new_shield = current_shield + value_for_shield;

  logd("AbsorbGettinFinal: GettinMult {} ValueForShield {} CurrentShield {} NewShield {}", gettin_mult, value_for_shield, current_shield, new_shield);

  set_new_shield_value(caster, new_shield);
}

auto gettin_absorb_shield_weapon(RE::Actor* attacker, const RE::TESEnchantableForm* enchantable, const float damage)
    -> void
{
  if (!attacker) {
    return;
  }

  let gettin_mult = get_percent_damage_for_shield_from_weapon(attacker, enchantable);
  let value_for_shield = damage * gettin_mult;

  let current_shield = get_current_shield_value(attacker);
  auto new_shield = current_shield + value_for_shield;

  set_new_shield_value(attacker, new_shield);
}

auto modify_actor_value_absorb(const RE::ValueModifierEffect* this_,
                               RE::Actor* actor,
                               float& value,
                               const RE::ActorValue av,
                               const Config&) -> void
{
  if (Core::can_modify_actor_value(this_, actor, value, av)) {

    logger::debug("absorb shield value before: {}"sv, value);
    value = std::abs(value);
    let caster_ptr = this_->GetCasterActor();
    if (is_enable_magick_shield(actor)) {
      if (value > 0.f && is_enable_shield_value_degen_delay_magick(actor)) {
        set_new_shield_value_degen_delay(actor, get_shield_value_degen_delay(actor, kAfterMagick));
      }
      handle_damage_to_shield(actor, caster_ptr ? caster_ptr.get() : nullptr, this_->spell, value);
    }
    value = -value;
    logger::debug("absorb shield value after: {}"sv, value);
  }
}

auto modify_actor_value_gettin(const RE::ValueModifierEffect* this_,
                               RE::Actor* actor,
                               float value,
                               const RE::ActorValue av,
                               const Config&) -> void
{
  if (Core::can_modify_actor_value(this_, actor, value, av)) {

    logd("AbsorbModifyGettin, value: {}", value);
    value = std::abs(value);
    let caster_ptr = this_->GetCasterActor();
    if (actor && caster_ptr && caster_ptr.get()) {
      logd("AbsorbModifyGettin: CasterNonNull");
      value = value * Core::getting_damage_mult(*actor);
      gettin_absorb_shield_magic(caster_ptr.get(), this_, value);
    }
  }
}

auto on_weapon_hit_absorb(RE::Actor* target, RE::HitData* hit_data, const Config&) -> void
{

  if (!target || !hit_data) {
    return;
  }

  if (target->IsDead()) {
    return;
  }

  auto aggressor = hit_data->aggressor.get();

  if (is_enable_weapon_shield(target)) {
    if (hit_data->totalDamage > 0.f && is_enable_shield_value_degen_delay_weapon(target)) {
      set_new_shield_value_degen_delay(target, get_shield_value_degen_delay(target, kAfterWeapon));
    }
    handle_damage_to_shield(target, aggressor ? aggressor.get() : nullptr, hit_data->weapon, hit_data->totalDamage);
  }
}

auto on_weapon_hit_gettin(const RE::Actor* target, const RE::HitData* hit_data, const Config&) -> void
{

  if (!target || !hit_data) {
    return;
  }

  if (target->IsDead()) {
    return;
  }

  auto aggressor = hit_data->aggressor.get();
  gettin_absorb_shield_weapon(aggressor ? aggressor.get() : nullptr, hit_data->weapon, hit_data->totalDamage);
}

auto set_graph_values(RE::Actor* actor) -> void
{

  constexpr auto fAbsorbShieldMaxValue = "fAbsorbShieldMaxValue"sv;
  constexpr auto fAbsorbShieldCurrentValue = "fAbsorbShieldCurrentValue"sv;
  constexpr auto fAbsorbShieldThresholdValue = "fAbsorbShieldThresholdValue"sv;
  constexpr auto fAbsorbShieldDrainValue = "fAbsorbShieldDrainValue"sv;
  constexpr auto fAbsorbShieldPercent = "fAbsorbShieldPercent"sv;
  constexpr auto fAbsorbShieldCurrentDelay = "fAbsorbShieldCurrentDelay"sv;
  constexpr auto fAbsorbShieldMaxWeaponDelay = "fAbsorbShieldMaxWeaponDelay"sv;
  constexpr auto fAbsorbShieldMaxMagickaDelay = "fAbsorbShieldMaxMagickaDelay"sv;
  constexpr auto fAbsorbShieldMaxCastCostDelay = "fAbsorbShieldMaxCastCostDelay"sv;

  constexpr auto bAbsorbShieldEnableWeapon = "bAbsorbShieldEnableWeapon"sv;
  constexpr auto bAbsorbShieldEnableMagick = "bAbsorbShieldEnableMagick"sv;
  constexpr auto bAbsorbShieldEnableFromCastCost = "bAbsorbShieldEnableFromCastCost"sv;
  constexpr auto bAbsorbShieldEnableThreshold = "bAbsorbShieldEnableThreshold"sv;
  constexpr auto bAbsorbShieldEnableWeaponDelay = "bAbsorbShieldEnableWeaponDelay"sv;
  constexpr auto bAbsorbShieldEnableMagickDelay = "bAbsorbShieldEnableMagickDelay"sv;
  constexpr auto bAbsorbShieldEnableCastCostDelay = "bAbsorbShieldEnableCastCostDelay"sv;
  constexpr auto bAbsorbShieldIsDelayed = "bAbsorbShieldIsDelayed"sv;

  let max_shield = get_max_shield_value(actor);
  let current_shield = get_current_shield_value(actor);
  let threshold_shield = get_drain_shield_threshold_value(actor);
  let drain_value = calculate_drain_shield_value(actor);
  let shield_percent = get_current_shield_value_as_percent(actor);
  let current_delay = get_current_shield_value_degen_delay(actor);
  let weapon_delay = get_shield_value_degen_delay(actor, kAfterWeapon);
  let magick_delay = get_shield_value_degen_delay(actor, kAfterMagick);
  let cast_cost_delay = get_shield_value_degen_delay(actor, kAfterCastCost);

  let enable_weapon = is_enable_weapon_shield(actor);
  let enable_magick = is_enable_magick_shield(actor);
  let enable_cast_cost = is_enable_shield_value_from_cast_cost(actor);
  let enable_threshold = is_enable_drain_shield_threshold(actor);
  let enable_weapon_delay = is_enable_shield_value_degen_delay_weapon(actor);
  let enable_magick_delay = is_enable_shield_value_degen_delay_magick(actor);
  let enable_cast_cost_delay = is_enable_shield_value_degen_delay_cast_cost(actor);

  actor->SetGraphVariableFloat(fAbsorbShieldMaxValue, max_shield);
  actor->SetGraphVariableFloat(fAbsorbShieldCurrentValue, current_shield);
  actor->SetGraphVariableFloat(fAbsorbShieldThresholdValue, threshold_shield);
  actor->SetGraphVariableFloat(fAbsorbShieldDrainValue, drain_value);
  actor->SetGraphVariableFloat(fAbsorbShieldPercent, shield_percent);
  actor->SetGraphVariableFloat(fAbsorbShieldCurrentDelay, current_delay);
  actor->SetGraphVariableFloat(fAbsorbShieldMaxWeaponDelay, weapon_delay);
  actor->SetGraphVariableFloat(fAbsorbShieldMaxMagickaDelay, magick_delay);
  actor->SetGraphVariableFloat(fAbsorbShieldMaxCastCostDelay, cast_cost_delay);

  actor->SetGraphVariableBool(bAbsorbShieldEnableWeapon, enable_weapon);
  actor->SetGraphVariableBool(bAbsorbShieldEnableMagick, enable_magick);
  actor->SetGraphVariableBool(bAbsorbShieldEnableFromCastCost, enable_cast_cost);
  actor->SetGraphVariableBool(bAbsorbShieldEnableThreshold, enable_threshold);
  actor->SetGraphVariableBool(bAbsorbShieldEnableWeaponDelay, enable_weapon_delay);
  actor->SetGraphVariableBool(bAbsorbShieldEnableMagickDelay, enable_magick_delay);
  actor->SetGraphVariableBool(bAbsorbShieldEnableCastCostDelay, enable_cast_cost_delay);
  actor->SetGraphVariableBool(bAbsorbShieldIsDelayed, current_delay > 0.f);
}

auto update_actor(RE::Character* character, const float delta) -> void
{
  if (!character) {
    return;
  }

  set_graph_values(character);

  let drain_value = calculate_drain_shield_value(character) * delta;
  drain_shield(character, drain_value);
}

auto calculate_shield_value_from_cast_cost(const RE::TESForm* spell_form, RE::Actor* caster) -> float
{
  if (!spell_form || !caster) {
    return 0.f;
  }

  let spell = spell_form->As<RE::SpellItem>();

  if (!spell) {
    return 0.f;
  }

  let cost = spell->CalculateMagickaCost(caster);

  let flat = get_shield_from_cast_cost_flat(caster);
  let percent = get_shield_from_cast_cost_percent(caster);
  let mult = get_shield_from_cast_cost_mult(caster);

  return (flat + (percent * cost)) * mult;
}

auto modify_shield_value_from_cast_cost(RE::Actor* actor, const float value_from_cast_cost) -> void
{

  if (is_enable_shield_value_degen_delay_cast_cost(actor)) {
    set_new_shield_value_degen_delay(actor, get_shield_value_degen_delay(actor, kAfterCastCost));
  }

  let current_shield = get_current_shield_value(actor);
  let new_shield = current_shield + value_from_cast_cost;

  set_new_shield_value(actor, new_shield);
}

auto action_event_handler(const SKSE::ActionEvent& event, RE::BSTEventSource<SKSE::ActionEvent>&) -> void
{
  if (!event.actor) {
    return;
  }

  switch (event.type.get()) {
    case SKSE::ActionEvent::Type::kSpellFire: {
      if (is_enable_shield_value_from_cast_cost(event.actor) && !Core::is_concentraion_spell(event.sourceForm)) {
        modify_shield_value_from_cast_cost(event.actor,
                                           calculate_shield_value_from_cast_cost(event.sourceForm, event.actor));
        break;
      }
      case SKSE::ActionEvent::Type::kSpellCast:
      case SKSE::ActionEvent::Type::kWeaponSwing:
      case SKSE::ActionEvent::Type::kVoiceCast:
      case SKSE::ActionEvent::Type::kVoiceFire:
      case SKSE::ActionEvent::Type::kBowDraw:
      case SKSE::ActionEvent::Type::kBowRelease:
      case SKSE::ActionEvent::Type::kBeginDraw:
      case SKSE::ActionEvent::Type::kEndDraw:
      case SKSE::ActionEvent::Type::kBeginSheathe:
      case SKSE::ActionEvent::Type::kEndSheathe:
        break;
    }
  }
}

namespace Callbacks {
auto set_frame_for_absorb_shield_callback() -> std::uint8_t
{
  return static_cast<std::uint8_t>(get_current_shield_value_as_percent(RE::PlayerCharacter::GetSingleton()));
}
}

namespace Papyrus {

// papyrus function
// ReSharper disable once CppInconsistentNaming
auto ModifyAbsorbShieldValue(RE::StaticFunctionTag*, RE::Actor* actor, const float amount) -> void
{
  modify_shield_value(actor, amount);
}

// papyrus function
// ReSharper disable once CppInconsistentNaming
auto GetCurrentAbsorbShieldValue(RE::StaticFunctionTag*, const RE::Actor* actor) -> float
{
  return get_current_shield_value(actor);
}

// papyrus function
// ReSharper disable once CppInconsistentNaming
auto GetMaxAbsorbShieldValue(RE::StaticFunctionTag*, RE::Actor* actor) -> float
{
  return get_max_shield_value(actor);
}

// papyrus function
// ReSharper disable once CppInconsistentNaming
auto GetAbsorbShieldValueDegenDelay(RE::StaticFunctionTag*, RE::Actor* actor) -> float
{
  return get_current_shield_value_degen_delay(actor);
}

// papyrus function
// ReSharper disable once CppInconsistentNaming
auto SetAbsorbShieldValueDegenDelay(RE::StaticFunctionTag*, RE::Actor* actor, const float amount) -> void
{
  set_new_shield_value_degen_delay(actor, amount, true);
}

}

}
