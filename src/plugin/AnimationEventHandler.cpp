#include "AnimationEventHandler.h"
#include "ResourceManager.h"
#include "Core.h"

namespace reflyem
{
  namespace animation_event_handler
  {

    static std::map<std::string, animation_event> animation_map{
      {weaponSwing, animation_event::kWeaponSwing},
      {weaponSwingLeft, animation_event::kWeaponLeftSwing},
      {JumpUp, animation_event::kJumpUp},
      {bowDrawStart, animation_event::kBowDrawStart} };

    auto tryFindAnimation(std::string& key) -> animation_event
    {
      auto it = animation_map.find(key);
      if (it == animation_map.end()) { return animation_event::kNone; }
      return it->second;
    }

    auto animation_handler(
      RE::BSAnimationGraphEvent* a_event,
      const reflyem::config& config) -> void
    {

      if (!config.resource_manager_enable) { return; }

      auto actor = const_cast<RE::Actor*>(a_event->holder->As<RE::Actor>());
      if (actor)
      {
        auto anim_event = fmt::format("{}", a_event->tag);
        auto is_power_attack = reflyem::core::is_power_attacking(*actor);
        auto animation = tryFindAnimation(anim_event);
        switch (animation)
        {
        case animation_event::kWeaponSwing:
        case animation_event::kWeaponLeftSwing:
        case animation_event::kJumpUp:
        case animation_event::kBowDrawStart:
        {
          logger::debug("event proc {}", anim_event);
          reflyem
            ::resource_manager
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