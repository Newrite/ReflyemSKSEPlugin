#include "AnimationEventHandler.h"
#include "ResourceManager.h"
#include "Core.h"

namespace Reflyem
{
  namespace AnimationEventHandler
  {

    constexpr inline auto weaponSwing = "weaponSwing";
    constexpr inline auto weaponSwingLeft = "weaponLeftSwing";
    constexpr inline auto JumpUp = "JumpUp";
    constexpr inline auto bowDrawStart = "bowDrawStart";
    constexpr inline auto bashExit = "bashExit";

    static std::map<std::string, AnimationEvent> animation_map{
      {weaponSwing, AnimationEvent::kWeaponSwing},
      {weaponSwingLeft, AnimationEvent::kWeaponLeftSwing},
      {JumpUp, AnimationEvent::kJumpUp},
      {bowDrawStart, AnimationEvent::kBowDrawStart},
      {bashExit, AnimationEvent::kBashExit} };

    auto try_find_animation(std::string& key) -> AnimationEvent
    {
      auto it = animation_map.find(key);
      if (it == animation_map.end()) { return AnimationEvent::kNone; }
      return it->second;
    }

    auto animation_handler(
      RE::BSAnimationGraphEvent* a_event,
      const Reflyem::Config& config) -> void
    {

      if (!config.resource_manager_enable) { return; }

      auto actor = const_cast<RE::Actor*>(a_event->holder->As<RE::Actor>());
      if (actor)
      {
        auto anim_event = fmt::format("{}", a_event->tag);
        auto is_power_attack = Reflyem::Core::is_power_attacking(*actor);
        auto animation = try_find_animation(anim_event);

        auto isDrawn = actor->IsWeaponDrawn();
        if ((animation == AnimationEvent::kWeaponLeftSwing 
          || animation == AnimationEvent::kWeaponSwing) && !isDrawn) { return; }

        switch (animation)
        {
        case AnimationEvent::kWeaponSwing:
        case AnimationEvent::kWeaponLeftSwing:
        case AnimationEvent::kJumpUp:
        case AnimationEvent::kBowDrawStart:
        case AnimationEvent::kBashExit:
        {
          logger::debug("event proc {}", anim_event);
          Reflyem
            ::ResourceManager
            ::handler(animation, *actor, is_power_attack, config);
          return;
        }
        default:
          return;
        }
      }
    }

  }
}