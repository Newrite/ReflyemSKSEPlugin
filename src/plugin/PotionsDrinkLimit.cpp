#include "Config.hpp"
#include "Core.hpp"
#include "plugin/PotionsDrintLimit.hpp"

namespace Reflyem::PotionsDrinkLimit
{

auto get_potions_cap(RE::Actor* drinker, const Config& config) -> int32_t
{
  auto cap = config.potions_drink_limit().cap_base();

  if (config.potions_drink_limit().cap_keyword())
    {
      let effects =
          Core::get_effects_by_keyword(*drinker, *config.potions_drink_limit().cap_keyword());
      let magnitudes_sum = Core::get_effects_magnitude_sum(effects).value_or(0.f);
      cap = cap + static_cast<int32_t>(magnitudes_sum);
    }

  if (cap > Core::ActorsCache::Data::POTIONS_ARRAY_SIZE)
  {
    cap = Core::ActorsCache::Data::POTIONS_ARRAY_SIZE;
  }
  
  return cap;
}

auto get_potions_duration(RE::Actor* drinker, const Config& config) -> float
{
  auto duration = config.potions_drink_limit().duration_base();

  if (config.potions_drink_limit().duration_keyword())
    {
      let effects =
          Core::get_effects_by_keyword(*drinker, *config.potions_drink_limit().duration_keyword());
      let magnitudes_sum = Core::get_effects_magnitude_sum(effects).value_or(0.f);
      duration = duration + magnitudes_sum;
    }

  if (duration < 0.f)
  {
    duration = 0.f;
  }

  return duration;
}

auto drink_potion(
    RE::Actor* drinker,
    RE::AlchemyItem* potion,
    RE::ExtraDataList*,
    const Config& config) -> bool
{
  if (potion->IsFood() ||
      Core::form_has_keyword(potion, config.potions_drink_limit().exclusive_keyword()))
    {
      return true;
    }

  const auto duration = get_potions_duration(drinker, config);

  if (duration <= 0.f)
  {
    return true;
  }

  auto& drinker_data = Core::ActorsCache::get_singleton().get_or_add(drinker->formID).get();
  const auto cap = get_potions_cap(drinker, config);

  for (int32_t index = 0; index < cap; index++)
  {
    if (drinker_data.potions_cooldawn_timers[index] <= 0.f)
    {
      drinker_data.potions_cooldawn_timers[index] = duration;
      return true;
    }
  }

  RE::DebugNotification(config.potions_drink_limit().notify().c_str());
  return false;
}
} // namespace Reflyem::PotionsDrinkLimit