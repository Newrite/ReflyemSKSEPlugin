#include "AnimationEventHandler.h"
#include "Core.h"
#include "ResourceManager.h"
#include "TKDodge.h"

namespace Reflyem {
namespace AnimationEventHandler {

constexpr inline auto weaponSwing     = "weaponSwing";
constexpr inline auto weaponSwingLeft = "weaponLeftSwing";
constexpr inline auto JumpUp          = "JumpUp";
constexpr inline auto bowDrawStart    = "bowDrawStart";
constexpr inline auto bashExit        = "bashExit";
// constexpr inline auto TKDR_DodgeStart = "TKDR_DodgeStart";

static std::map<std::string, AnimationEvent> animation_map{{weaponSwing, AnimationEvent::kWeaponSwing},
                                                           {weaponSwingLeft, AnimationEvent::kWeaponLeftSwing},
                                                           {JumpUp, AnimationEvent::kJumpUp},
                                                           {bowDrawStart, AnimationEvent::kBowDrawStart},
                                                           {bashExit, AnimationEvent::kBashExit}};

auto
try_find_animation(std::string& key) -> AnimationEvent {
  const auto it = animation_map.find(key);
  if (it == animation_map.end()) {
    return AnimationEvent::kNone;
  }
  return it->second;
}

auto
animation_handler(RE::BSAnimationGraphEvent* a_event, const Reflyem::Config& config) -> void {
  if (!config.resource_manager_enable) {
    return;
  }

  auto actor = const_cast<RE::Actor*>(a_event->holder->As<RE::Actor>());
  if (actor) {
    auto anim_event      = fmt::format("{}", a_event->tag);
    auto is_power_attack = Reflyem::Core::is_power_attacking(*actor);

    switch (auto animation = try_find_animation(anim_event)) {
    // case AnimationEvent::kTKDR_DodgeStart:
    // {
    //   logger::info("TKDodge start");
    //   return;
    // }
    case AnimationEvent::kWeaponSwing:
    case AnimationEvent::kWeaponLeftSwing:
    case AnimationEvent::kJumpUp:
    case AnimationEvent::kBowDrawStart:
    case AnimationEvent::kBashExit: {
      logger::debug("event proc {}", anim_event);
      Reflyem ::ResourceManager ::animation_handler(animation, *actor, is_power_attack, config);
      return;
    }
    default:
      return;
    }
  }
}

} // namespace AnimationEventHandler

auto
PlayerAnimationHandler::register_sink(RE::Actor* actor) -> bool {
  static PlayerAnimationHandler g_eventhandler;

  RE::BSAnimationGraphManagerPtr graph_mgr;

  if (actor) {
    actor->GetAnimationGraphManager(graph_mgr);
  }

  if (!graph_mgr || !graph_mgr->graphs.cbegin()) {
    logger::debug("Player Graph not found!");
    return false;
  }

  graph_mgr->graphs.cbegin()->get()->AddEventSink(&g_eventhandler);

  logger::debug("Register Animation Event Handler!");

  return true;
}

auto
PlayerAnimationHandler::ProcessEvent(const RE::BSAnimationGraphEvent*               a_event,
                                     RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
    -> RE::BSEventNotifyControl {
  auto& config = Reflyem::Config::get_singleton();
  if (config.tk_dodge_enable) {
    return Reflyem::TKDodge::process_event_player_animation(a_event, a_eventSource, config);
  }
  return RE::BSEventNotifyControl::kContinue;
}

} // namespace Reflyem