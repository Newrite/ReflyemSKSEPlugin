#include "plugin/ParryBash.hpp"
#include "Core.hpp"
#include "plugin/AnimationEventHandler.hpp"

namespace Reflyem::ParryBash {
auto precision_pre_hit_callback(const PRECISION_API::PrecisionHitData& hit_data)
    -> PRECISION_API::PreHitCallbackReturn {
  // const auto& conifg = Config::get_singleton();

  PRECISION_API::PreHitCallbackReturn result;
  result.bIgnoreHit = false;

  const auto attacker = hit_data.attacker;
  const auto target   = [](RE::TESObjectREFR* refr) -> RE::Actor* {
    if (refr) {
      return refr->As<RE::Actor>();
    }
    return nullptr;
  }(hit_data.target);

  if (!attacker || !target) {
    return result;
  }

  const auto who =
      fmt::format("{} -> {}:"sv, attacker->GetDisplayFullName(), target->GetDisplayFullName());

  auto& target_data = Core::ActorsCache::get_singleton().get_or_add(target->formID).get();
  // const auto bash_parryng_diff = 1.f - target_data.bash_parryng_start_delta();
  // logger::info("{} bash_parryng_diff: {}"sv, who, bash_parryng_diff);
  // if (bash_parryng_diff <= 0.15f) {
  //   logger::info("{} Ignores hit becouse target parrying"sv, who);
  //   result.bIgnoreHit = true;
  //   return result;
  // }

  const auto is_attacker_bashing = attacker->GetAttackState() == RE::ATTACK_STATE_ENUM::kBash;

  if (!is_attacker_bashing) {
    return result;
  }

  const auto pre_hit_diff = 1.f - target_data.bash_parry_timer();

  logger::info("{} AttackerBash: {} TargetPreHitDiff: {}"sv, who, is_attacker_bashing,
               pre_hit_diff);

  if (pre_hit_diff > 0.175f) {
    logger::info("{} Ignore bash hit"sv, who);
    result.bIgnoreHit = true;
  } else {
    logger::info("{} Bash hit"sv, who);
    target->SetGraphVariableFloat("StaggerMagnitude", 5.f);
    target->NotifyAnimationGraph("staggerStart");
  }

  return result;
}

auto animation_handler(const RE::BSAnimationGraphEvent& event, const Config&) -> void {

  if (const auto actor = const_cast<RE::Actor*>(event.holder->As<RE::Actor>()); actor) {

    switch (AnimationEventHandler::try_find_animation(fmt::format("{}"sv, event.tag))) {
    case AnimationEventHandler::AnimationEvent::kWeaponSwing: {

      logger::info("{}: kWeaponSwing: {}"sv, actor->GetDisplayFullName(), GetTickCount64());

      Core::ActorsCache::get_singleton().get_or_add(actor->formID).get().bash_parry_timer(1.f);
      return;
    }
    case AnimationEventHandler::AnimationEvent::kWeaponLeftSwing: {

      logger::info("{}: kWeaponLeftSwing: {}"sv, actor->GetDisplayFullName(), GetTickCount64());

      Core::ActorsCache::get_singleton().get_or_add(actor->formID).get().bash_parry_timer(1.f);
      return;
    }
    case AnimationEventHandler::AnimationEvent::kPreHitFrame: {

      logger::info("{}: kPreHitFrame: {}"sv, actor->GetDisplayFullName(), GetTickCount64());

      Core::ActorsCache::get_singleton()
          .get_or_add(actor->formID)
          .get()
          .bash_parry_timer(1.f - 0.075f);
      return;
    }
    case AnimationEventHandler::AnimationEvent::kHitFrame: {

      // logger::info("{}: hitFrame: {}"sv, actor->GetDisplayFullName(), GetTickCount64());
      //
      // Core::ActorsCache::get_singleton()
      //     .get_or_add(actor->formID)
      //     .get()
      //     .set_pre_hit_frame_start_delta(1.f);
      return;
    }
    case AnimationEventHandler::AnimationEvent::kJumpUp:
    case AnimationEventHandler::AnimationEvent::kBowDrawStart:
    case AnimationEventHandler::AnimationEvent::kBashExit:
    case AnimationEventHandler::AnimationEvent::kNone:;
    case AnimationEventHandler::AnimationEvent::kTkDodgeStart:
    case AnimationEventHandler::AnimationEvent::kTkDodgeIFrameEnd:;
    }
  }
}
}