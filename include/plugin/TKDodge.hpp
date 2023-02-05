#pragma once

#include "Config.hpp"

// reimplement of https://github.com/max-su-2019/TK_Dodge_RE
namespace Reflyem {
namespace TkDodge {

auto animation_handler(const RE::BSAnimationGraphEvent& event, const Config& config) -> void;

auto process_event_input_handler(RE::InputEvent* const*               event,
                                 RE::BSTEventSource<RE::InputEvent*>* event_source,
                                 const Config& config) -> RE::BSEventNotifyControl;

} // namespace TkDodge
} // namespace Reflyem