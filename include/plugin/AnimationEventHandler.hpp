#pragma once

#include "Config.hpp"
#include <cache/cache.hpp>

namespace Reflyem {
namespace AnimationEventHandler {

enum class AnimationEvent {
  kWeaponSwing     = 0,
  kWeaponLeftSwing = 1,
  kJumpUp          = 2,
  kBowDrawStart    = 3,
  kBashExit        = 4,
  kNone            = 99,
};

auto animation_handler(const RE::BSAnimationGraphEvent* event, const Config& config) -> void;

} // namespace AnimationEventHandler

// TK Dodge RE
// https://github.com/max-su-2019/TK_Dodge_RE
class PlayerAnimationHandler final : public RE::BSTEventSink<RE::BSAnimationGraphEvent> {
public:
  auto ProcessEvent(const RE::BSAnimationGraphEvent* event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* event_source)
      -> RE::BSEventNotifyControl override;

  static auto register_sink(const RE::Actor* actor) -> bool;
};

} // namespace Reflyem