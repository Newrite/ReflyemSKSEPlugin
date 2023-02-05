#pragma once

#include "Config.hpp"

namespace Reflyem::AnimationEventHandler {

enum class AnimationEvent {
  kWeaponSwing      = 0,
  kWeaponLeftSwing  = 1,
  kJumpUp           = 2,
  kBowDrawStart     = 3,
  kBashExit         = 4,
  kTkDodgeStart     = 5,
  kTkDodgeIFrameEnd = 6,
  kNone             = 99,
};

constexpr inline auto WEAPON_SWING      = "weaponSwing"sv;
constexpr inline auto WEAPON_SWING_LEFT = "weaponLeftSwing"sv;
constexpr inline auto JUMP_UP           = "JumpUp"sv;
constexpr inline auto BOW_DRAW_START    = "bowDrawStart"sv;
constexpr inline auto BASH_EXIT         = "bashExit"sv;
constexpr inline auto TKDR_DODGE_START  = "TKDR_DodgeStart"sv;
constexpr inline auto TKDR_I_FRAME_END  = "TKDR_IFrameEnd"sv;

static std::map<std::string_view, AnimationEvent> animation_map{
    {WEAPON_SWING, AnimationEvent::kWeaponSwing},
    {WEAPON_SWING_LEFT, AnimationEvent::kWeaponLeftSwing},
    {JUMP_UP, AnimationEvent::kJumpUp},
    {BOW_DRAW_START, AnimationEvent::kBowDrawStart},
    {BASH_EXIT, AnimationEvent::kBashExit},
    {TKDR_DODGE_START, AnimationEvent::kTkDodgeStart},
    {TKDR_I_FRAME_END, AnimationEvent::kTkDodgeIFrameEnd}};

auto try_find_animation(const std::string& key) -> AnimationEvent;

} // namespace Reflyem::AnimationEventHandler