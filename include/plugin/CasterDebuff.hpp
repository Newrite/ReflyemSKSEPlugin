#pragma once

#include "Config.hpp"

namespace Reflyem::CasterDebuff {
auto on_update_actor(RE::Character& character, float delta, const Config& config) -> void;
}

// namespace Reflyem
