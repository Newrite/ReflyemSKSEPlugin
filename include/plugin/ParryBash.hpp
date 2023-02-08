#pragma once

#include "Config.hpp"
#include "PrecisionAPI.hpp"

namespace Reflyem::ParryBash {
auto precision_pre_hit_callback(const PRECISION_API::PrecisionHitData&)
    -> PRECISION_API::PreHitCallbackReturn;
auto animation_handler(const RE::BSAnimationGraphEvent& event, const Config& config) -> void;
} // namespace Reflyem::ParryBash