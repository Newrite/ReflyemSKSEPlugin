#pragma once
#include "Config.hpp"

namespace Reflyem::PotionsDrinkLimit {

enum PotionType
{
  kHealth,
  kMagicka,
  kStamina,
  kOther,
  kPoison,
  kNone
};

auto get_start_and_end_index(const PotionType potion_type, const int32_t cap) -> std::tuple<int32_t, int32_t>;
auto get_potion_cap_per_type(RE::Actor* drinker, const PotionType potion_type, const Config& config) -> int32_t;
auto get_duration_per_type(RE::Actor* drinker, const PotionType potion_type, const Config& config) -> float;
auto drink_potion(RE::Actor* drinker, RE::AlchemyItem* potion, RE::ExtraDataList* extra_data_list, const Config& config)
    -> bool;

namespace Callbacks {
auto set_frame_for_potion_callback(GIGAWIDGET_API::PotionType potion_type) -> std::uint8_t;
auto set_available_potion_slot_callback(GIGAWIDGET_API::PotionType potion_type) -> GIGAWIDGET_API::AvailablePotionSlotResult;
}

} // namespace Reflyem::PotionsDrinkLimit
