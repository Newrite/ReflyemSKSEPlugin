#include "plugin/ParryBash.hpp"
#include "Core.hpp"
#include "SlowTime.hpp"
#include "plugin/AnimationEventHandler.hpp"

namespace Reflyem::ParryBash
{

auto is_allow_parry_bash(RE::Actor* attacker, RE::Actor* target, const Config& config) -> bool
{
  if (!attacker || !target) { return false; }

  if (!target->IsAttacking()) { return false; }

  const auto is_attacker_bashing = Core::is_bashing(attacker);
  if (!is_attacker_bashing) { return false; }

  const auto& target_data = Core::ActorsCache::get_singleton().get_or_add(target->formID).get();

  if (!Core::try_has_absolute_keyword(attacker, config.parry_bash().parry_keyword()))
    {
      logger::debug("Attacker has no keyword"sv);
      return false;
    }

  if (Core::try_has_absolute_keyword(target, config.parry_bash().parry_immun_keyword()))
    {
      logger::debug("Target has immun"sv);
      return false;
    }

  const auto timing_effects =
      Core::try_get_effects_by_keyword(attacker, config.parry_bash().parry_timing_keyword());
  const auto parry_timing = config.parry_bash().parry_timing() +
                            Core::get_effects_magnitude_sum(timing_effects).value_or(0.f);

  const auto diff = 1.f - target_data.bash_parry_timer();

  logger::debug(
      "{} -> {}: diff {} parry_timing {}"sv,
      attacker->GetDisplayFullName(),
      target->GetDisplayFullName(),
      diff,
      parry_timing);
  if (diff > parry_timing)
    {
      logger::debug(
          "{} -> {}: ignore bash hit"sv,
          attacker->GetDisplayFullName(),
          target->GetDisplayFullName());
      return false;
    }
  logger::debug(
      "{} -> {}: parry bash"sv,
      attacker->GetDisplayFullName(),
      target->GetDisplayFullName());
  return true;
}

auto cast_on_parry_bash(RE::Actor& target, RE::Actor& caster, const Config& config) -> void
{
  if (config.cast_on_parry_bash().enable())
    {
      Core::cast_on_handle_formlists(
          config.cast_on_parry_bash().formlist_needkw(),
          config.cast_on_parry_bash().formlist_spells(),
          caster,
          target);
    }
}

auto parry_bash_handler(RE::Actor& target, RE::Actor& attacker, const Config& config) -> void
{
  if (attacker.IsPlayerRef() && config.slow_time().enable_on_parry_bash()) {
    SlowTime::start_slow_time(config);
  }
  cast_on_parry_bash(target, attacker, config);
  Core::play_sound(config.parry_bash().parry_sound(), &attacker);
  target.SetGraphVariableFloat("StaggerMagnitude", 5.f);
  target.NotifyAnimationGraph("staggerStart");
}

auto precision_pre_hit_callback(const PRECISION_API::PrecisionHitData& hit_data)
    -> PRECISION_API::PreHitCallbackReturn
{
  PRECISION_API::PreHitCallbackReturn result;
  result.bIgnoreHit = false;
  const auto& config = Config::get_singleton();
  if (!hit_data.target || !hit_data.attacker || !config.parry_bash().enable()) { return result; }

  const auto& attacker_data =
      Core::ActorsCache::get_singleton().get_or_add(hit_data.attacker->formID).get();
  if (attacker_data.bash_parry_timer_no_hit() > 0.f)
    {
      result.bIgnoreHit = true;
      return result;
    }

  const auto target = hit_data.target->As<RE::Actor>();
  if (is_allow_parry_bash(hit_data.attacker, target, config))
    {
      auto& target_data = Core::ActorsCache::get_singleton().get_or_add(target->formID).get();
      target_data.bash_parry_timer_no_hit(0.5f);
      parry_bash_handler(*target, *hit_data.attacker, config);
    }
  else
    {
      if (hit_data.attacker && Core::is_bashing(hit_data.attacker)) { result.bIgnoreHit = true; }
    }

  return result;
}

auto precision_weapons_collide_callback(const PRECISION_API::PrecisionHitData& hit_data)
    -> PRECISION_API::WeaponCollisionCallbackReturn
{
  PRECISION_API::WeaponCollisionCallbackReturn result;
  result.bIgnoreHit = false;
  const auto& config = Config::get_singleton();
  if (!hit_data.target || !hit_data.attacker || !config.parry_bash().enable() || !config.parry_bash().enable_precision_weapon_collide()) { return result; }

  const auto& attacker_data =
      Core::ActorsCache::get_singleton().get_or_add(hit_data.attacker->formID).get();
  if (attacker_data.bash_parry_timer_no_hit() > 0.f)
  {
    result.bIgnoreHit = true;
    return result;
  }

  const auto target = hit_data.target->As<RE::Actor>();
  if (is_allow_parry_bash(hit_data.attacker, target, config))
  {
    auto& target_data = Core::ActorsCache::get_singleton().get_or_add(target->formID).get();
    target_data.bash_parry_timer_no_hit(0.5f);
    parry_bash_handler(*target, *hit_data.attacker, config);
  }
  else
  {
    if (hit_data.attacker && Core::is_bashing(hit_data.attacker)) { result.bIgnoreHit = true; }
  }

  return result;
}

auto on_melee_collision(RE::Actor& attacker, RE::Actor& victim, const Config& config) -> bool
{
  const auto& attacker_data = Core::ActorsCache::get_singleton().get_or_add(attacker.formID).get();
  if (attacker_data.bash_parry_timer_no_hit() > 0.f) { return true; }

  if (is_allow_parry_bash(&attacker, &victim, config))
    {
      auto& target_data = Core::ActorsCache::get_singleton().get_or_add(victim.formID).get();
      target_data.bash_parry_timer_no_hit(0.5f);
      parry_bash_handler(victim, attacker, config);
      return false;
    }
  if (Core::is_bashing(&attacker)) { return true; }

  return false;
}

auto animation_handler(const RE::BSAnimationGraphEvent& event, const Config& config) -> void
{
  if (const auto actor = const_cast<RE::Actor*>(event.holder->As<RE::Actor>()); actor)
    {
      switch (AnimationEventHandler::try_find_animation(event.tag.c_str()))
        {
          case AnimationEventHandler::AnimationEvent::kWeaponSwing: {
            if (!config.parry_bash().enable_weapon_swing()) { return; }

            logger::debug("{}: kWeaponSwing: {}"sv, actor->GetDisplayFullName(), GetTickCount64());

            Core::ActorsCache::get_singleton()
                .get_or_add(actor->formID)
                .get()
                .bash_parry_timer(1.f);
            return;
          }
          case AnimationEventHandler::AnimationEvent::kWeaponLeftSwing: {
            if (!config.parry_bash().enable_weapon_swing()) { return; }

            logger::debug(
                "{}: kWeaponLeftSwing: {}"sv,
                actor->GetDisplayFullName(),
                GetTickCount64());

            Core::ActorsCache::get_singleton()
                .get_or_add(actor->formID)
                .get()
                .bash_parry_timer(1.f);
            return;
          }
          case AnimationEventHandler::AnimationEvent::kPreHitFrame: {
            if (!config.parry_bash().enable_pre_hit_frame()) { return; }

            logger::debug("{}: kPreHitFrame: {}"sv, actor->GetDisplayFullName(), GetTickCount64());

            Core::ActorsCache::get_singleton()
                .get_or_add(actor->formID)
                .get()
                .bash_parry_timer(1.f - config.parry_bash().pre_hit_frame_penalty());
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
} // namespace Reflyem::ParryBash
