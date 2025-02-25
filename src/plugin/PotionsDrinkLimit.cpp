#include "plugin/PotionsDrinkLimit.hpp"
#include "Core.hpp"

namespace Reflyem::PotionsDrinkLimit {

auto get_start_and_end_index(const PotionType potion_type, const int32_t cap) -> std::tuple<int32_t, int32_t>
{
  int32_t start_index = 0;
  int32_t end_index = 0;
  switch (potion_type) {

    case kHealth: {
      start_index = Core::ActorsCache::Data::INDEX_HEALTH_START;
      end_index = Core::ActorsCache::Data::INDEX_HEALTH_END - (Core::ActorsCache::Data::POTIONS_CAP_PER_TYPE - cap);
      break;
    }
    case kMagicka: {
      start_index = Core::ActorsCache::Data::INDEX_MAGICKA_START;
      end_index = Core::ActorsCache::Data::INDEX_MAGICKA_END - (Core::ActorsCache::Data::POTIONS_CAP_PER_TYPE - cap);
      break;
    }
    case kStamina: {
      start_index = Core::ActorsCache::Data::INDEX_STAMINA_START;
      end_index = Core::ActorsCache::Data::INDEX_STAMINA_END - (Core::ActorsCache::Data::POTIONS_CAP_PER_TYPE - cap);
      break;
    }
    case kOther: {
      start_index = Core::ActorsCache::Data::INDEX_OTHER_START;
      end_index = Core::ActorsCache::Data::INDEX_OTHER_END - (Core::ActorsCache::Data::POTIONS_CAP_PER_TYPE - cap);
      break;
    }
    case kPoison: {
      start_index = Core::ActorsCache::Data::INDEX_POISON_START;
      end_index = Core::ActorsCache::Data::INDEX_POISON_END - (Core::ActorsCache::Data::POTIONS_CAP_PER_TYPE - cap);
      break;
    }
    case kNone:
      break;
  }

  return {start_index, end_index};
}

auto get_potions_cap(RE::Actor* drinker, const RE::BGSKeyword* cap_keyword, int32_t cap) -> int32_t
{

  if (cap_keyword) {
    let effects = Core::try_get_effects_by_keyword(drinker, cap_keyword);
    let magnitudes_sum = Core::get_effects_magnitude_sum(effects).value_or(0.f);
    cap = cap + static_cast<int32_t>(magnitudes_sum);
  }

  if (cap > Core::ActorsCache::Data::POTIONS_CAP_PER_TYPE) {
    cap = Core::ActorsCache::Data::POTIONS_CAP_PER_TYPE;
  }

  return cap;
}

auto get_potion_cap_per_type(RE::Actor* drinker, const PotionType potion_type, const Config& config) -> int32_t
{
  auto cap = 0;
  switch (potion_type) {
    case kHealth: {
      cap = get_potions_cap(
          drinker, config.potions_drink_limit().health_cap_keyword(), config.potions_drink_limit().health_cap_base());
      break;
    }
    case kMagicka: {
      cap = get_potions_cap(
          drinker, config.potions_drink_limit().magicka_cap_keyword(), config.potions_drink_limit().magicka_cap_base());
      break;
    }
    case kStamina: {
      cap = get_potions_cap(
          drinker, config.potions_drink_limit().stamina_cap_keyword(), config.potions_drink_limit().stamina_cap_base());
      break;
    }
    case kOther: {
      cap = get_potions_cap(
          drinker, config.potions_drink_limit().other_cap_keyword(), config.potions_drink_limit().other_cap_base());
      break;
    }
    case kPoison: {
      cap = get_potions_cap(drinker, config.poison_rework().cap_keyword(), config.poison_rework().cap_base());
      break;
    }
    case kNone:
      break;
  }
  return cap;
}

auto get_potions_duration(RE::Actor* drinker, const RE::BGSKeyword* duration_keyword, float duration) -> float
{

  if (duration_keyword) {
    let effects = Core::try_get_effects_by_keyword(drinker, duration_keyword);
    let magnitudes_sum = Core::get_effects_magnitude_sum(effects).value_or(0.f);
    duration = duration + magnitudes_sum;
  }

  if (duration < 0.f) {
    duration = 0.f;
  }

  return duration;
}

auto get_duration_per_type(RE::Actor* drinker, const PotionType potion_type, const Config& config) -> float
{
  auto duration = 0.f;
  switch (potion_type) {
    case kHealth: {
      duration = get_potions_duration(drinker,
                                      config.potions_drink_limit().health_duration_keyword(),
                                      config.potions_drink_limit().health_duration_base());
      break;
    }
    case kMagicka: {
      duration = get_potions_duration(drinker,
                                      config.potions_drink_limit().magicka_duration_keyword(),
                                      config.potions_drink_limit().magicka_duration_base());
      break;
    }
    case kStamina: {
      duration = get_potions_duration(drinker,
                                      config.potions_drink_limit().stamina_duration_keyword(),
                                      config.potions_drink_limit().stamina_duration_base());
      break;
    }
    case kOther: {
      duration = get_potions_duration(drinker,
                                      config.potions_drink_limit().other_duration_keyword(),
                                      config.potions_drink_limit().other_duration_base());
      break;
    }
    case kPoison: {
      duration = get_potions_duration(
          drinker, config.poison_rework().duration_keyword(), config.poison_rework().duration_base());
      break;
    }
    case kNone:
      break;
  }
  return duration;
}

auto get_cooldown_percent_from_closest_slot_to_reset(RE::Actor* drinker,
                                                     const PotionType potion_type,
                                                     const Config& config) -> std::uint8_t
{

  if (!drinker) {
    return 100;
  }

  const auto& drinker_data = Core::ActorsCache::get_singleton().get_or_add(drinker->formID).get();
  let cap = get_potion_cap_per_type(drinker, potion_type, config);
  let[start_index, end_index] = get_start_and_end_index(potion_type, cap);

  auto potion_cooldown = Core::ActorsCache::Data::BaseCooldown(0.f, 0.f);
  auto is_found_some = false;
  for (int32_t index = start_index; index < end_index; index++) {
    if (drinker_data.potions_cooldown_timers[index].duration > 0.f) {

      if (!is_found_some) {
        potion_cooldown = drinker_data.potions_cooldown_timers[index];
        is_found_some = true;
        continue;
      }

      if (drinker_data.potions_cooldown_timers[index].duration < potion_cooldown.duration) {
        potion_cooldown = drinker_data.potions_cooldown_timers[index];
      }
    }
  }

  if (is_found_some) {
    if (potion_cooldown.start_duration == 0.f) {
      return 100;
    }
    return 100 - static_cast<std::uint8_t>(potion_cooldown.duration / (potion_cooldown.start_duration / 100.f));
  }

  return 100;
}

auto get_cooldown_pointer_from_closest_slot_to_reset(RE::Actor* drinker,
                                                       const PotionType potion_type,
                                                       const Config& config) -> Core::ActorsCache::Data::BaseCooldown*
{

  if (!drinker) {
    return nullptr;
  }

  auto& drinker_data = Core::ActorsCache::get_singleton().get_or_add(drinker->formID).get();
  let cap = get_potion_cap_per_type(drinker, potion_type, config);
  let[start_index, end_index] = get_start_and_end_index(potion_type, cap);

  auto cooldown_index = -1;
  auto is_found_some = false;
  for (int32_t index = start_index; index < end_index; index++) {
    if (drinker_data.potions_cooldown_timers[index].duration > 0.f) {

      if (!is_found_some) {
        cooldown_index = index;
        is_found_some = true;
        continue;
      }

      if (drinker_data.potions_cooldown_timers[index].duration <
          drinker_data.potions_cooldown_timers[cooldown_index].duration) {
        cooldown_index = index;
      }
    }
  }

  if (is_found_some && cooldown_index >= 0) {
    if (drinker_data.potions_cooldown_timers[cooldown_index].duration <= 0.f) {
      return nullptr;
    }
    return std::addressof(drinker_data.potions_cooldown_timers[cooldown_index]);
  }

  return nullptr;
}

auto modify_active_cooldowns(RE::Actor* drinker, const PotionType potion_type, const float amount, const Config& config)
    -> void
{

  if (!drinker) {
    return;
  }

  auto& drinker_data = Core::ActorsCache::get_singleton().get_or_add(drinker->formID).get();
  let cap = get_potion_cap_per_type(drinker, potion_type, config);
  let[start_index, end_index] = get_start_and_end_index(potion_type, cap);

  for (int32_t index = start_index; index < end_index; index++) {
    if (drinker_data.potions_cooldown_timers[index].duration > 0.f) {

      drinker_data.potions_cooldown_timers[index].duration += amount;
      if (drinker_data.potions_cooldown_timers[index].duration < 0.f) {
        drinker_data.potions_cooldown_timers[index].duration = 0.f;
      }
    }
  }
}

auto get_available_potions_slots(RE::Actor* drinker, const PotionType potion_type, const Config& config)
    -> GIGAWIDGET_API::AvailablePotionSlotResult
{

  if (!drinker) {
    return GIGAWIDGET_API::AvailablePotionSlotResult{};
  }

  const auto& drinker_data = Core::ActorsCache::get_singleton().get_or_add(drinker->formID).get();
  let cap = get_potion_cap_per_type(drinker, potion_type, config);
  let[start_index, end_index] = get_start_and_end_index(potion_type, cap);

  std::uint16_t slots_counter = 0;
  float minimal_duration = 0.f;
  auto is_found_some_duration = false;

  for (int32_t index = start_index; index < end_index; index++) {

    if (drinker_data.potions_cooldown_timers[index].duration <= 0.f) {
      slots_counter = slots_counter + 1;
    }

    if (drinker_data.potions_cooldown_timers[index].duration > 0.f) {

      if (!is_found_some_duration) {
        minimal_duration = drinker_data.potions_cooldown_timers[index].duration;
        is_found_some_duration = true;
        continue;
      }

      if (drinker_data.potions_cooldown_timers[index].duration < minimal_duration) {
        minimal_duration = drinker_data.potions_cooldown_timers[index].duration;
      }
    }
  }

  return {static_cast<std::uint16_t>(minimal_duration), slots_counter};
}

auto drink_potion(RE::Actor* drinker, RE::AlchemyItem* potion, RE::ExtraDataList*, const Config& config) -> bool
{

  bool allow = false;
  PotionType potion_type = PotionType::kNone;

  if (config.potions_drink_limit().health_enable()) {
    if (Core::try_form_has_keyword(potion, config.potions_drink_limit().health_keyword())) {
      potion_type = PotionType::kHealth;
      allow = true;
    }
  }

  if (config.potions_drink_limit().stamina_enable()) {
    if (Core::try_form_has_keyword(potion, config.potions_drink_limit().stamina_keyword())) {
      potion_type = PotionType::kStamina;
      allow = true;
    }
  }

  if (config.potions_drink_limit().magicka_enable()) {
    if (Core::try_form_has_keyword(potion, config.potions_drink_limit().magicka_keyword())) {
      potion_type = PotionType::kMagicka;
      allow = true;
    }
  }

  if (config.potions_drink_limit().other_enable() && potion_type == PotionType::kNone) {
    if (Core::try_form_has_keyword(potion, config.potions_drink_limit().other_exclusive_keyword())) {
      if (config.potions_drink_limit().other_revert_exclusive()) {
        potion_type = PotionType::kOther;
        allow = true;
      }
    } else {
      if (!config.potions_drink_limit().other_revert_exclusive()) {
        potion_type = PotionType::kOther;
        allow = true;
      }
    }
  }

  if (potion->IsFood() || !allow) {
    return true;
  }

  let duration = get_duration_per_type(drinker, potion_type, config);

  if (duration <= 0.f) {
    return true;
  }

  auto& drinker_data = Core::ActorsCache::get_singleton().get_or_add(drinker->formID).get();
  let cap = get_potion_cap_per_type(drinker, potion_type, config);

  if (cap <= 0) {
    RE::DebugNotification("Cap potion is 0"s.c_str());
    return false;
  }

  let[start_index, end_index] = get_start_and_end_index(potion_type, cap);

  for (int32_t index = start_index; index < end_index; index++) {
    if (drinker_data.potions_cooldown_timers[index].duration <= 0.f) {
      drinker_data.potions_cooldown_timers[index] = Core::ActorsCache::Data::BaseCooldown(duration, duration);
      return true;
    }
  }

  RE::DebugNotification(config.potions_drink_limit().notify().c_str());
  return false;
}
namespace Callbacks {
auto set_frame_for_potion_callback(GIGAWIDGET_API::PotionType potion_type) -> std::uint8_t
{

  let config = Config::get_singleton();

  switch (potion_type) {
    case GIGAWIDGET_API::PotionType::Health: {
      return get_cooldown_percent_from_closest_slot_to_reset(
          RE::PlayerCharacter::GetSingleton(), PotionType::kHealth, config);
    }
    case GIGAWIDGET_API::PotionType::Stamina: {
      return get_cooldown_percent_from_closest_slot_to_reset(
          RE::PlayerCharacter::GetSingleton(), PotionType::kStamina, config);
    }
    case GIGAWIDGET_API::PotionType::Magicka: {
      return get_cooldown_percent_from_closest_slot_to_reset(
          RE::PlayerCharacter::GetSingleton(), PotionType::kMagicka, config);
    }
    case GIGAWIDGET_API::PotionType::Other: {
      return get_cooldown_percent_from_closest_slot_to_reset(
          RE::PlayerCharacter::GetSingleton(), PotionType::kOther, config);
    }
    case GIGAWIDGET_API::PotionType::Poison: {
      return get_cooldown_percent_from_closest_slot_to_reset(
          RE::PlayerCharacter::GetSingleton(), PotionType::kPoison, config);
    }
  }
  return 100;
}

auto set_available_potion_slot_callback(GIGAWIDGET_API::PotionType potion_type)
    -> GIGAWIDGET_API::AvailablePotionSlotResult
{

  let config = Config::get_singleton();
  if (!config.potions_drink_limit().enable()) {
    return GIGAWIDGET_API::AvailablePotionSlotResult{};
  }

  switch (potion_type) {
    case GIGAWIDGET_API::PotionType::Health: {
      return get_available_potions_slots(RE::PlayerCharacter::GetSingleton(), PotionType::kHealth, config);
    }
    case GIGAWIDGET_API::PotionType::Stamina: {
      return get_available_potions_slots(RE::PlayerCharacter::GetSingleton(), PotionType::kStamina, config);
    }
    case GIGAWIDGET_API::PotionType::Magicka: {
      return get_available_potions_slots(RE::PlayerCharacter::GetSingleton(), PotionType::kMagicka, config);
    }
    case GIGAWIDGET_API::PotionType::Other: {
      return get_available_potions_slots(RE::PlayerCharacter::GetSingleton(), PotionType::kOther, config);
    }
    case GIGAWIDGET_API::PotionType::Poison: {
      return get_available_potions_slots(RE::PlayerCharacter::GetSingleton(), PotionType::kPoison, config);
    }
  }
  return GIGAWIDGET_API::AvailablePotionSlotResult{};
}
}

// papyrus function
// ReSharper disable once CppInconsistentNaming
auto GetAvailablePotions(RE::StaticFunctionTag*,
                          const int potion_type,
                          RE::Actor* actor) -> int
{

  if (actor == nullptr) {
    return -1;
  }

  PotionType potion_type_enum;
  switch (potion_type) {

    case kHealth: {
      potion_type_enum = kHealth;
      break;
    }
    case kMagicka: {
      potion_type_enum = kMagicka;
      break;
    }
    case kStamina: {
      potion_type_enum = kStamina;
      break;
    }
    case kOther: {
      potion_type_enum = kOther;
      break;
    }
    case kPoison: {
      potion_type_enum = kPoison;
      break;
    }
    case kNone:
      default: {
      return -1;
      }
  }

  letr config = Config::get_singleton();
  if (!config.potions_drink_limit().enable() && potion_type_enum != PotionType::kPoison) {
    return -1;
  }

  if (!config.poison_rework().enable() && potion_type_enum == PotionType::kPoison) {
    return -1;
  }

  let potion_slot = get_available_potions_slots(actor, potion_type_enum, config);

  return potion_slot.slots;
  
}

// papyrus function
// ReSharper disable once CppInconsistentNaming
auto ModifyPotionCooldown(RE::StaticFunctionTag*,
                          const int potion_type,
                          RE::Actor* actor,
                          const float amount,
                          const bool closest) -> void
{

  if (amount == 0.f || actor == nullptr) {
    return;
  }

  PotionType potion_type_enum;
  switch (potion_type) {

    case kHealth: {
      potion_type_enum = kHealth;
      break;
    }
    case kMagicka: {
      potion_type_enum = kMagicka;
      break;
    }
    case kStamina: {
      potion_type_enum = kStamina;
      break;
    }
    case kOther: {
      potion_type_enum = kOther;
      break;
    }
    case kPoison: {
      potion_type_enum = kPoison;
      break;
    }
    case kNone:
    default: {
      return;
    }
  }

  letr config = Config::get_singleton();
  if (!config.potions_drink_limit().enable() && potion_type_enum != PotionType::kPoison) {
    return;
  }

  if (!config.poison_rework().enable() && potion_type_enum == PotionType::kPoison) {
    return;
  }

  if (closest) {
    auto potion_cooldown = get_cooldown_pointer_from_closest_slot_to_reset(actor, potion_type_enum, config);
    if (potion_cooldown) {
      potion_cooldown->duration = potion_cooldown->duration + amount;
      if (potion_cooldown->duration < 0.f) {
        potion_cooldown->duration = 0.f;
      }
    }
    return;
  }

  modify_active_cooldowns(actor, potion_type_enum, amount, config);
  
}

} // namespace Reflyem::PotionsDrinkLimit
