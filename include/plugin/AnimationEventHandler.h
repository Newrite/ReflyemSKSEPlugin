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
      kNone = 99,
    };

    auto animation_handler(
      RE::BSAnimationGraphEvent* a_event,
      const Reflyem::Config& config) -> void;
  }
}