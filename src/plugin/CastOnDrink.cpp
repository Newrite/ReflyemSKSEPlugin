#include "CastOnDrink.hpp"

auto Reflyem::CastOnDrink::drink_potion(RE::Actor* drinker,
                                        RE::AlchemyItem* potion,
                                        RE::ExtraDataList*,
                                        const Config& config) -> void
{
  if (!drinker || !potion) {
    return;
  }

  if (potion->IsFood() || potion->IsPoison()) {
    return;
  }

  Core::cast_on_handle_formlists(
    config.cast_on_drink().formlist_needkw(), config.cast_on_drink().formlist_spells(), drinker, drinker);
  
}
