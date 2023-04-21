#pragma once

#include "Config.hpp"

namespace Reflyem::ItemLimit
{
// ReSharper disable once CppInconsistentNaming
auto UpdateItems(RE::StaticFunctionTag*) -> void;
auto update_actor(RE::Character& character, const float delta, const Config& config) -> void;
} // namespace Reflyem::ItemLimit