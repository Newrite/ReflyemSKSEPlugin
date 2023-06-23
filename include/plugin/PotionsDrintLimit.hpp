#pragma once

namespace Reflyem::PotionsDrinkLimit
{
auto drink_potion(RE::Actor* drinker, RE::AlchemyItem* potion, RE::ExtraDataList* extra_data_list, const Config& config) -> bool;
} // namespace Reflyem::PotionsDrinkLimit