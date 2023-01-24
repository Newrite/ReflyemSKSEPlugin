#pragma once

#include "Config.hpp"

namespace Reflyem {
namespace SpeedCasting {
auto on_update_actor(RE::Character& player, float, const Reflyem::Config& config) -> void;
}
} // namespace Reflyem