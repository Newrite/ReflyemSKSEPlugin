﻿#pragma once

#include "Config.hpp"

namespace Reflyem::CastOnGetHit
{
auto on_weapon_hit(RE::Actor* target, const RE::HitData& hit_data, const Config& config) -> void;
}

// namespace Reflyem
