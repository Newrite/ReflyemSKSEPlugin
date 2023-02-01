#pragma once

#include "Config.hpp"

namespace Reflyem::CasterAdditions {
auto on_update_actor(RE::Character& character, float delta, const Config& config) -> void;
auto action_event_handler(const SKSE::ActionEvent&               event,
                          RE::BSTEventSource<SKSE::ActionEvent>& event_source) -> void;
} // namespace Reflyem::CasterAdditions

// namespace Reflyem
