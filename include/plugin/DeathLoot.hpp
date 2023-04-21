#pragma once

namespace Reflyem::DeathLoot
{
auto process_death(
    const RE::TESDeathEvent* event,
    RE::BSTEventSource<RE::TESDeathEvent>* event_source) -> void;
}