#pragma once

#include "Config.h"

namespace reflyem
{
  namespace animation_event_handler
  {

    constexpr inline auto weaponSwing = "weaponSwing";
    constexpr inline auto weaponSwingLeft = "weaponLeftSwing";
    constexpr inline auto JumpUp = "JumpUp";
    constexpr inline auto bowDrawStart = "bowDrawStart";

    enum class animation_event
    {
      kWeaponSwing = 0,
      kWeaponLeftSwing = 1,
      kJumpUp = 2,
      kBowDrawStart = 3,
      kNone = 99,
    };

    auto animation_handler(
      RE::BSAnimationGraphEvent* a_event,
      const reflyem::config& config) -> void;
  }
}