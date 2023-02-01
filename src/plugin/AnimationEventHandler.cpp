#include "plugin/AnimationEventHandler.hpp"
#include "Core.hpp"
#include "plugin/ResourceManager.hpp"
#include "plugin/TKDodge.hpp"

namespace Reflyem {
namespace AnimationEventHandler {

constexpr inline auto WEAPON_SWING      = "weaponSwing"sv;
constexpr inline auto WEAPON_SWING_LEFT = "weaponLeftSwing"sv;
constexpr inline auto JUMP_UP           = "JumpUp"sv;
constexpr inline auto BOW_DRAW_START    = "bowDrawStart"sv;
constexpr inline auto BASH_EXIT         = "bashExit"sv;
// constexpr inline auto TKDR_DodgeStart = "TKDR_DodgeStart";

static std::map<std::string_view, AnimationEvent> animation_map{
    {WEAPON_SWING, AnimationEvent::kWeaponSwing},
    {WEAPON_SWING_LEFT, AnimationEvent::kWeaponLeftSwing},
    {JUMP_UP, AnimationEvent::kJumpUp},
    {BOW_DRAW_START, AnimationEvent::kBowDrawStart},
    {BASH_EXIT, AnimationEvent::kBashExit}};

auto try_find_animation(const std::string& key) -> AnimationEvent {
  const auto it = animation_map.find(key);
  if (it == animation_map.end()) {
    return AnimationEvent::kNone;
  }
  return it->second;
}

auto animation_handler(const RE::BSAnimationGraphEvent* event, const Config& config) -> void {
  if (!config.resource_manager().enable()) {
    return;
  }

  // ReSharper disable once CppLocalVariableMayBeConst
  if (auto actor = const_cast<RE::Actor*>(event->holder->As<RE::Actor>())) {
    auto       anim_event      = fmt::format("{}"sv, event->tag);
    const auto is_power_attack = Core::is_power_attacking(*actor);

    switch (const auto animation = try_find_animation(anim_event)) {
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
      ResourceManager::animation_handler(animation, *actor, is_power_attack, config);
      return;
    }
    case AnimationEvent::kNone:
    default:
      return;
    }
  }
}

} // namespace AnimationEventHandler

auto PlayerAnimationHandler::register_sink(const RE::Actor* actor) -> bool {
  static PlayerAnimationHandler handler;

  RE::BSAnimationGraphManagerPtr graph_manager;

  if (actor) {
    actor->GetAnimationGraphManager(graph_manager);
  }

  if (!graph_manager || !graph_manager->graphs.cbegin()) {
    logger::debug("Player Graph not found!"sv);
    return false;
  }

  graph_manager->graphs.cbegin()->get()->AddEventSink(&handler);

  logger::debug("Register Animation Event Handler!"sv);

  return true;
}

auto PlayerAnimationHandler::ProcessEvent(
    const RE::BSAnimationGraphEvent*               event,
    RE::BSTEventSource<RE::BSAnimationGraphEvent>* event_source) -> RE::BSEventNotifyControl {
  if (const auto& config = Config::get_singleton(); config.tk_dodge().enable()) {
    return TkDodge::process_event_player_animation(event, event_source, config);
  }
  return RE::BSEventNotifyControl::kContinue;
}

} // namespace Reflyem