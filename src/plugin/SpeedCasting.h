#pragma once

#include "Config.h"

namespace reflyem
{
  namespace speed_casting
  {
    auto on_main_update(
      RE::Main* a_this,
      float a2,
      const reflyem::config& config) -> void;
  }
}