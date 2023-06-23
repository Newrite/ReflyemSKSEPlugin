#pragma once

namespace Reflyem::DeathLoot
{

auto is_loot(const RE::InventoryEntryData* data) -> bool;

auto process_death(
    const RE::TESDeathEvent* event,
    RE::BSTEventSource<RE::TESDeathEvent>* event_source) -> void;
} // namespace Reflyem::DeathLoot