#pragma once

#include "Config.hpp"

namespace Reflyem::MagicResistRescaled {
auto check_resistance(RE::MagicTarget&      this_, RE::MagicItem& magic_item,
                      const RE::Effect&         effect,
                      const RE::TESBoundObject* bound_object, const Config& config) -> float;
}
