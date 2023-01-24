#pragma once

#include "AnimationEventHandler.hpp"
#include "Config.hpp"

// reimplement of https://github.com/max-su-2019/TK_Dodge_RE
namespace Reflyem {
namespace TKDodge {

auto process_event_player_animation(const RE::BSAnimationGraphEvent*               a_event,
                                    RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource,
                                    const Reflyem::Config&                         config) -> RE::BSEventNotifyControl;

auto process_event_input_handler(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource,
                                 const Reflyem::Config& config) -> RE::BSEventNotifyControl;

} // namespace TKDodge
} // namespace Reflyem