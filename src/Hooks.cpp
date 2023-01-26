#include "Hooks.hpp"
#include "CastOnHit.hpp"
#include "CasterDebuff.hpp"
#include "CheatDeath.hpp"
#include "Core.hpp"
#include "Crit.hpp"
#include "MagicShield.hpp"
#include "PetrifiedBlood.hpp"
#include "ResourceManager.hpp"
#include "SpeedCasting.hpp"
#include "Vampirism.hpp"

namespace Hooks {

static ULONGLONG timer1000 = 0;
static ULONGLONG timer500  = 0;
static ULONGLONG timer100  = 0;

auto update_actor(RE::Character& character, const float delta, const Reflyem::Config& config) -> void {

  logger::debug("update actor"sv);

  constexpr auto ptr_key = 0; // reinterpret_cast<std::uintptr_t>(&character);

  const auto tick = GetTickCount64();

  ULONGLONG last_tick100  = 0;
  ULONGLONG last_tick1000 = 0;

  if (Reflyem::Core::character_timer_map100.contains(ptr_key)) {
    logger::debug("update actor map100"sv);
    last_tick100 = Reflyem::Core::character_timer_map100.at(ptr_key);
  }

  if (Reflyem::Core::character_timer_map1000.contains(ptr_key)) {
    logger::debug("update actor map1000"sv);
    last_tick1000 = Reflyem::Core::character_timer_map1000.at(ptr_key);
  }

  if (tick - last_tick100 > 100) {
    logger::debug("update actor map100 tick"sv);
    Reflyem::Core::character_timer_map100[ptr_key] = tick;
    if (config.resource_manager_enable) {
      Reflyem::ResourceManager::update_actor(character, delta, config);
    }
    if (config.caster_debuff_enable) {
      Reflyem::CasterDebuff::on_update_actor(character, delta, config);
    }
  }

  if (tick - last_tick1000 > 1000) {
    logger::debug("update actor map1000 tick"sv);
    Reflyem::Core::character_timer_map1000[ptr_key] = tick;
    if (config.petrified_blood_enable && config.petrified_blood_magick) {
      Reflyem::PetrifiedBlood::character_update(character, delta, config);
    }
  }

  if (tick - timer100 >= 100) {
    timer100 = tick;
    Reflyem::Core::character_timer_map_handler(tick, Reflyem::Core::character_timer_map100);
    if (config.resource_manager_enable) {
      Reflyem::ResourceManager::ranged_spend_handler();
    }
  }

  if (tick - timer500 >= 500) {
    timer500 = tick;
    if (config.speed_casting_enable && character.IsPlayerRef()) {
      Reflyem::SpeedCasting::on_update_actor(character, delta, config);
    }
  }

  if (tick - timer1000 >= 1000) {
    timer1000 = tick;
    Reflyem::Core::character_timer_map_handler(tick, Reflyem::Core::character_timer_map1000);
  }

  return;
}

auto OnPlayerCharacterUpdate::update(RE::PlayerCharacter* this_, float delta) -> void {
  if (this_) {

    auto& config = Reflyem::Config::get_singleton();

    update_actor(*this_, delta, config);
  }
  return update_(this_, delta);
}

auto OnCharacterUpdate::update(RE::Character* this_, float delta) -> void {
  if (this_) {

    auto& config = Reflyem::Config::get_singleton();

    update_actor(*this_, delta, config);
  }
  return update_(this_, delta);
}

auto OnAttackData::process_attack(RE::ActorValueOwner* value_owner, RE::BGSAttackData* attack_data) -> void {
  process_attack_(value_owner, attack_data);
  return;
}

auto OnAttackAction::attack_action(const RE::TESActionData* action_data) -> bool {
  logger::info("Attack Action: {}"sv, static_cast<std::uint32_t>(action_data->GetSourceActorState()->GetAttackState()));
  return false;
}

auto OnAnimationEventNpc::process_event(RE::BSTEventSink<RE::BSAnimationGraphEvent>*   this_,
                                        RE::BSAnimationGraphEvent*                     event,
                                        RE::BSTEventSource<RE::BSAnimationGraphEvent>* dispatcher) -> void {
  if (event && event->holder) {
    auto& config = Reflyem::Config::get_singleton();
    Reflyem::AnimationEventHandler::animation_handler(event, config);
  }
  process_event_(this_, event, dispatcher);
  return;
}

auto OnAnimationEventPc::process_event(RE::BSTEventSink<RE::BSAnimationGraphEvent>*   this_,
                                       RE::BSAnimationGraphEvent*                     event,
                                       RE::BSTEventSource<RE::BSAnimationGraphEvent>* dispatcher) -> void {
  if (event && event->holder) {
    auto& config = Reflyem::Config::get_singleton();
    Reflyem::AnimationEventHandler::animation_handler(event, config);
  }
  process_event_(this_, event, dispatcher);
  return;
}

auto OnAdjustActiveEffect::adjust_active_effect(RE::ActiveEffect* this_, float power, bool unk) -> void {
  if (this_) {
    const auto caster = this_->GetCasterActor();
    const auto target = this_->GetTargetActor();
    if (caster && target) {
      logger::info("Target HP: {} Caster HP: {} Target Level: {} Caster Level: {}"sv,
                   target->GetActorValue(RE::ActorValue::kHealth), caster->GetActorValue(RE::ActorValue::kHealth),
                   target->GetLevel(), caster->GetLevel());
      logger::info("Effect, mag: {}, dur: {}"sv, this_->magnitude, this_->duration);
    } else {
      logger::info("Caster or target is null"sv);
    }
  }
  adjust_active_effect_(this_, power, unk);
  return;
}

auto OnModifyActorValue::modify_actor_value(RE::ValueModifierEffect* this_, RE::Actor* actor, float value,
                                            RE::ActorValue av) -> void {
  if (!actor || !this_) {
    modify_actor_value_(this_, actor, value, av);
    return;
  }

  auto& config = Reflyem::Config::get_singleton();

  if (config.magic_crit_enable) {
    Reflyem::Crit::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.petrified_blood_enable && config.petrified_blood_magick) {
    Reflyem::PetrifiedBlood::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.magic_shield_enable && config.magic_shield_magick) {
    Reflyem::MagicShield::modify_actor_value(this_, actor, value, av, config);
  }

  Reflyem::Vampirism::modify_actor_value(this_, actor, value, av, config);

  modify_actor_value_(this_, actor, value, av);
  return;
}

auto OnPeakModifyActorValue::peak_modify_actor_value(RE::ValueModifierEffect* this_, RE::Actor* actor, float value,
                                                     RE::ActorValue av) -> void {
  logger::debug("peak mod actor value"sv);

  if (!actor || !this_) {
    peak_modify_actor_value_(this_, actor, value, av);
    return;
  }

  auto& config = Reflyem::Config::get_singleton();

  if (config.magic_crit_enable) {
    Reflyem::Crit::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.petrified_blood_enable && config.petrified_blood_magick) {
    Reflyem::PetrifiedBlood::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.magic_shield_enable && config.magic_shield_magick) {
    Reflyem::MagicShield::modify_actor_value(this_, actor, value, av, config);
  }

  Reflyem::Vampirism::modify_actor_value(this_, actor, value, av, config);

  peak_modify_actor_value_(this_, actor, value, av);
  return;
}

auto OnDualModifyActorValue::dual_modify_actor_value(RE::ValueModifierEffect* this_, RE::Actor* actor, float value,
                                                     RE::ActorValue av) -> void {
  if (!actor || !this_) {
    dual_modify_actor_value_(this_, actor, value, av);
    return;
  }

  auto& config = Reflyem::Config::get_singleton();

  if (config.magic_crit_enable) {
    Reflyem::Crit::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.petrified_blood_enable && config.petrified_blood_magick) {
    Reflyem::PetrifiedBlood::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.magic_shield_enable && config.magic_shield_magick) {
    Reflyem::MagicShield::modify_actor_value(this_, actor, value, av, config);
  }

  Reflyem::Vampirism::modify_actor_value(this_, actor, value, av, config);

  dual_modify_actor_value_(this_, actor, value, av);
  return;
}

auto OnDualModifyActorValueSecondInnerCall::dual_modify_actor_value_second_inner_call(RE::ValueModifierEffect* this_,
                                                                                      RE::Actor* actor, float value,
                                                                                      RE::ActorValue av) -> void {
  if (!actor || !this_) {
    dual_modify_actor_value_second_inner_call_(this_, actor, value, av);
    return;
  }

  auto& config = Reflyem::Config::get_singleton();

  if (config.magic_crit_enable) {
    Reflyem::Crit::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.petrified_blood_enable && config.petrified_blood_magick) {
    Reflyem::PetrifiedBlood::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.magic_shield_enable && config.magic_shield_magick) {
    Reflyem::MagicShield::modify_actor_value(this_, actor, value, av, config);
  }

  Reflyem::Vampirism::modify_actor_value(this_, actor, value, av, config);

  dual_modify_actor_value_second_inner_call_(this_, actor, value, av);
  return;
}

auto OnMainUpdate::main_update(RE::Main* this_, float unk) -> void {

  if (const auto ui = RE::UI::GetSingleton(); ui->GameIsPaused()) {
    main_update_(this_, unk);
    return;
  }

  main_update_(this_, unk);
  return;
}

auto OnWeaponHit::weapon_hit(RE::Actor* target, RE::HitData& hit_data) -> void {
  if (!target) {
    weapon_hit_(target, hit_data);
    return;
  }

  auto& config = Reflyem::Config::get_singleton();

  if (config.weapon_crit_enable) {
    Reflyem::Crit::on_weapon_hit(target, hit_data, config);
  }

  if (config.resource_manager_enable && config.resource_manager_block_spend_enable) {
    Reflyem::ResourceManager::on_weapon_hit(target, hit_data, config);
  }

  if (config.cast_on_hit_enable) {
    Reflyem::CastOnHit::on_weapon_hit(target, hit_data, config);
  }

  if (config.cheat_death_enable && config.cheat_death_physical) {
    Reflyem::CheatDeath::on_weapon_hit(target, hit_data, config);
  }

  if (config.petrified_blood_enable && config.petrified_blood_physical) {
    Reflyem::PetrifiedBlood::on_weapon_hit(target, hit_data, config);
  }

  if (config.magic_shield_enable && config.magic_shield_physical) {
    Reflyem::MagicShield::on_weapon_hit(target, hit_data, config);
  }

  Reflyem::Vampirism::on_weapon_hit(target, hit_data, config);

  weapon_hit_(target, hit_data);
  return;
}

auto install_hooks() -> void {
  logger::info("start install hooks"sv);
  auto& trampoline = SKSE::GetTrampoline();
  trampoline.create(1024);
  OnWeaponHit::install_hook(trampoline);
  // OnMainUpdate::install_hook(trampoline);
  // OnAdjustActiveEffect::install_hook(trampoline);
  OnAnimationEventNpc::install_hook();
  OnAnimationEventPc::install_hook();
  OnCharacterUpdate::install_hook();
  OnPlayerCharacterUpdate::install_hook();
  OnModifyActorValue::install_hook();
  OnDualModifyActorValue::install_hook();
  OnPeakModifyActorValue::install_hook();
  OnDualModifyActorValueSecondInnerCall::install_hook(trampoline);
  // OnAttackAction::install_hook(trampoline);
  // OnAttackData::install_hook(trampoline);
  logger::info("finish install hooks"sv);
}
} // namespace Hooks