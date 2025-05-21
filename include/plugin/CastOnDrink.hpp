#pragma once

#include "Config.hpp"
#include "Core.hpp"

namespace Reflyem::CastOnDrink {

auto drink_potion(RE::Actor* drinker, RE::AlchemyItem* potion, RE::ExtraDataList* extra_data_list, const Config& config)
    -> void;

}
