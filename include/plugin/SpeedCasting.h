#pragma once

#include "Config.h"

namespace Reflyem
{
  namespace SpeedCasting
  {
    auto on_main_update(
      RE::Main* a_this,
      float a2,
      const Reflyem::Config& config) -> void;
  }
}