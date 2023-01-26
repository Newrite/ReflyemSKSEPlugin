#pragma once

#include "AnimationEventHandler.hpp"
#include "Config.hpp"

// reimplement of https://github.com/max-su-2019/TK_Dodge_RE
namespace Reflyem {
namespace TkDodge {

auto process_event_player_animation(const RE::BSAnimationGraphEvent*               event,
                                    RE::BSTEventSource<RE::BSAnimationGraphEvent>* event_source, const Config& config)
    -> RE::BSEventNotifyControl;

auto process_event_input_handler(RE::InputEvent* const* event, RE::BSTEventSource<RE::InputEvent*>* event_source,
                                 const Config& config) -> RE::BSEventNotifyControl;

} // namespace TkDodge
} // namespace Reflyem