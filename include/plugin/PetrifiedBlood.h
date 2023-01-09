#pragma once

#include "Config.h"

namespace Reflyem
{
  namespace PetrifiedBlood
  {
    auto on_weapon_hit(
      RE::Actor* target,
      RE::HitData& hit_data,
      const  Reflyem::Config& config) -> void;
  }
}