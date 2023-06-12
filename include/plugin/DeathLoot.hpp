#pragma once

namespace Reflyem::DeathLoot
{

auto is_tagged(RE::InventoryEntryData* item) -> bool;

auto process_death(
    const RE::TESDeathEvent* event,
    RE::BSTEventSource<RE::TESDeathEvent>* event_source) -> void;
}