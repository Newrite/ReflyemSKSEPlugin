#pragma once

#include "Config.hpp"

namespace Reflyem::CastOnSoulTrap
{

auto soul_trap_handler(RE::Actor& trapper, RE::Actor& target, const Config& config) -> void;
}
