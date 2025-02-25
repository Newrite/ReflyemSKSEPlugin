#pragma once
#include "Config.hpp"

namespace Reflyem::PoisonRework {
auto poison_prompt(RE::Actor* actor, RE::AlchemyItem* poison, const Config& config)
    -> bool;
// namespace Callbacks {
// auto set_frame_for_potion_callback(GIGAWIDGET_API::PotionType potion_type) -> std::uint8_t;
// auto set_available_potion_slot_callback(GIGAWIDGET_API::PotionType potion_type) -> std::uint16_t;
// }
} // namespace Reflyem::PoisonRework
