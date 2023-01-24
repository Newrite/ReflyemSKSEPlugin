#pragma once

#include "Config.hpp"

namespace Reflyem {
namespace CasterDebuff {
auto on_update_actor(RE::Character& player, float, const Reflyem::Config& config) -> void;
}
} // namespace Reflyem