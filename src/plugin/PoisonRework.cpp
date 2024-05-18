#include "PoisonRework.hpp"
#include "Core.hpp"
#include "PotionsDrinkLimit.hpp"

namespace Reflyem::PoisonRework {
auto poison_prompt(RE::Actor* actor, RE::AlchemyItem* poison, const Config& config) -> bool
{

  bool allow = false;

  if (Core::try_form_has_keyword(poison, config.poison_rework().exclusive_keyword())) {
    if (config.poison_rework().revert_exclusive()) {
      allow = true;
    }
  } else {
    if (!config.poison_rework().revert_exclusive()) {
      allow = true;
    }
  }

  if (!allow) {
    logi("Poison not allow"sv);
    return false;
  }

  let duration = get_duration_per_type(actor, PotionsDrinkLimit::PotionType::kPoison, config) + poison->data.addictionChance;

  if (duration <= 0.f) {
    logi("Poison duration 0"sv);
    return false;
  }

  auto& drinker_data = Core::ActorsCache::get_singleton().get_or_add(actor->formID).get();
  let cap = get_potion_cap_per_type(actor, PotionsDrinkLimit::PotionType::kPoison, config);

  if (cap <= 0) {
    RE::DebugNotification("Cap poison is 0"s.c_str());
    return true;
  }

  let[start_index, end_index] = get_start_and_end_index(PotionsDrinkLimit::PotionType::kPoison, cap);
  logi("Poison start check {} {} {} {}"sv, start_index, end_index, cap, duration);

  for (int32_t index = start_index; index < end_index; index++) {
    if (drinker_data.potions_cooldown_timers[index].duration <= 0.f) {
      drinker_data.potions_cooldown_timers[index] = Core::ActorsCache::Data::PotionCooldown(duration, duration);
      return false;
    }
  }

  RE::DebugNotification(config.poison_rework().notify().c_str());
  return true;
}
}
