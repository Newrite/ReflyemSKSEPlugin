#pragma once

#include "Config.hpp"

namespace Reflyem::DeclutteredLoot {

extern bool declutter_mode;

auto is_clutter_object(RE::TESBoundObject* object) -> bool;

auto is_clutter_object(RE::TESObjectREFR* ref_object) -> bool;

auto process_input_event(RE::InputEvent* const* e, RE::BSTEventSource<RE::InputEvent*>*) -> void;
}
