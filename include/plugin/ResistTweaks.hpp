#pragma once

#include "Config.hpp"

namespace Reflyem::ResistTweaks
{
auto get_no_absorb(const RE::MagicItem& this_) -> bool;

auto ignores_resistance(const RE::MagicItem& this_) -> bool;

auto check_resistance(
    RE::MagicTarget& this_,
    RE::MagicItem& magic_item,
    const RE::Effect& effect,
    const RE::TESBoundObject* bound_object,
    const Config& config) -> float;
} // namespace Reflyem::ResistTweaks
