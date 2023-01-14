#pragma once

#include "Config.h"

namespace Reflyem
{
  namespace AnimationEventHandler
  {

    enum class AnimationEvent
    {
      kWeaponSwing = 0,
      kWeaponLeftSwing = 1,
      kJumpUp = 2,
      kBowDrawStart = 3,
      kBashExit = 4,
      kNone = 99,
    };

    auto animation_handler(
      RE::BSAnimationGraphEvent* a_event,
      const Reflyem::Config& config) -> void;

  }

  // TK Dodge RE
  // https://github.com/max-su-2019/TK_Dodge_RE
	class PlayerAnimationHandler final : public RE::BSTEventSink<RE::BSAnimationGraphEvent>
  {

	public:
		auto ProcessEvent(
      const RE::BSAnimationGraphEvent* a_event,
      RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) -> RE::BSEventNotifyControl override;

    static auto register_sink(RE::Actor* pc) -> bool;

	};

}