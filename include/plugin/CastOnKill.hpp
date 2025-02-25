#pragma once

#include "Config.hpp"

namespace Reflyem::CastOnKill
{

auto kill_handler(RE::Actor& killer, RE::Actor& victim, const Config& config) -> void;
}