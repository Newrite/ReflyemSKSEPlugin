#include "Hooks.h"
#include "Core.h"
#include "Vampirism.h"
#include "MagicShield.h"
#include "PetrifiedBlood.h"
#include "CheatDeath.h"
#include "SpeedCasting.h"
#include "CastOnHit.h"
#include "WeaponCrit.h"
#include "ResourceManager.h"

namespace Hooks
{

  static ULONGLONG timer500 = 0;
  static ULONGLONG timer100 = 0;

  auto OnAttackData::process_attack(RE::ActorValueOwner* value_owner, RE::BGSAttackData* attack_data) -> void
  {
    if (!value_owner)
    {
      logger::info("Value ownrer or attack data is null");
      _process_attack(value_owner, attack_data);
      return;
    }
    auto actor = static_cast<RE::Actor*>(value_owner);
    if (actor)
    {
      logger::info("Success cast actor, level is: {}", actor->GetLevel());
    }
    else
    {
      logger::info("actor is null");
    }
    if (attack_data)
    {
      logger::info("attack_data, event: {}", attack_data->event);
    }
    else
    {
      logger::info("attack_data is null");
    }
    _process_attack(value_owner, attack_data);
    return;
  }

  auto OnAnimationEventNpc::process_event(
    RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this,
    RE::BSAnimationGraphEvent* a_event,
    RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_dispatcher) -> void
  {
    if (a_event && a_event->holder)
    {
      auto& config = Reflyem::Config::get_singleton();
      Reflyem::AnimationEventHandler::animation_handler(a_event, config);
    }
    _process_event(a_this, a_event, a_dispatcher);
    return;
  }

  auto OnAnimationEventPc::process_event(
    RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this,
    RE::BSAnimationGraphEvent* a_event,
    RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_dispatcher) -> void
  {
    if (a_event && a_event->holder)
    {
      auto& config = Reflyem::Config::get_singleton();
      Reflyem::AnimationEventHandler::animation_handler(a_event, config);
    }
    _process_event(a_this, a_event, a_dispatcher);
    return;
  }

  auto OnAdjustActiveEffect::adjust_active_effect(RE::ActiveEffect* a_this, float a_power, bool a_arg3) -> void
  {
    _adjust_active_effect(a_this, a_power, a_arg3);
    return;
  }

  auto OnModifyActorValue::modify_actor_value(
    RE::ValueModifierEffect* a_this,
    RE::Actor* a_actor, float a_value,
    RE::ActorValue a_actorValue) -> void
  {
    logger::info("modify_actor_value");
    if (!a_this)
    {
      _modify_actor_value(a_this, a_actor, a_value, a_actorValue);
      return;
    }
    // auto& config = Reflyem::Config::get_singleton();
    logger::info("value before: {}", a_value);
    // Reflyem::MagicShield::modify_actor_value(a_this, a_actor, a_value, a_actorValue, config);
    // logger::info("value after: {}", a_value);
    // a_this->magnitude = 0.f;
    // a_this->value = 0.f;
    _modify_actor_value(a_this, a_actor, 0.f, a_actorValue);
    return;
  }

  auto OnDualModifyActorValue::dual_modify_actor_value(
    RE::ValueModifierEffect* a_this,
    RE::Actor* a_actor, float a_value,
    RE::ActorValue a_actorValue) -> void
  {
    logger::info("main virtual call, a_value: {}, actor_value: {}, actor_value_data: {}", a_value, a_actorValue, a_this->actorValue);
    if (!a_actor || !a_this)
    {
      _dual_modify_actor_value(a_this, a_actor, a_value, a_actorValue);
      return;
    }
    _dual_modify_actor_value(a_this, a_actor, a_value, a_actorValue);
    return;
  }

  auto OnDualModifyActorValueSecondInnerCall::dual_modify_actor_value_second_inner_call(
    RE::ValueModifierEffect* a_this,
    RE::Actor* a_actor, float a_value,
    RE::ActorValue a_actorValue) -> void
  {
    logger::info("second inner call, a_value: {}, actor_value: {}, actor_value_data: {}", a_value, a_actorValue, a_this->actorValue);
    if (!a_actor || !a_this)
    {
      _dual_modify_actor_value_second_inner_call(a_this, a_actor, a_value, a_actorValue);
      return;
    }
    _dual_modify_actor_value_second_inner_call(a_this, a_actor, a_value, a_actorValue);
    return;
  }

  auto OnMainUpdate::main_update(RE::Main* a_this, float a2) -> void
  {

    auto ui = RE::UI::GetSingleton();

    if (ui->GameIsPaused())
    {
      _main_update(a_this, a2);
      return;
    }

    auto tick = GetTickCount64();

    if (tick - timer500 >= 500)
    {
      timer500 = tick;
      auto& config = Reflyem::Config::get_singleton();

      if (config.speed_casting_enable)
      {
        Reflyem::SpeedCasting::on_main_update(a_this, a2, config);
      }
    }
    
    if (tick - timer100 >= 100)
    {
      timer100 = tick;
      auto& config = Reflyem::Config::get_singleton();
      if (config.resource_manager_enable)
      {
        Reflyem::ResourceManager::ranged_spend_handler();
      }
    }

    _main_update(a_this, a2);
    return;
  }

  auto OnWeaponHit::weapon_hit(RE::Actor* target, RE::HitData& hit_data) -> void
  {

    if (!target) {
      _weapon_hit(target, hit_data);
      return;
    }

    auto& config = Reflyem::Config::get_singleton();

    if (config.weapon_crit_enable)
    {
      Reflyem::WeaponCrit::on_weapon_hit(target, hit_data, config);
    }

    if (config.resource_manager_enable)
    {
      Reflyem::ResourceManager::on_weapon_hit(target, hit_data, config);
    }

    if (config.cast_on_hit_enable)
    {
      Reflyem::CastOnHit::on_weapon_hit(target, hit_data, config);
    }

    if (config.cheat_death_enable)
    {
      Reflyem::CheatDeath::on_weapon_hit(target, hit_data, config);
    }

    if (config.petrified_blood_enable)
    {
      Reflyem::PetrifiedBlood::on_weapon_hit(target, hit_data, config);
    }

    if (config.magic_shield_enable)
    {
      Reflyem::MagicShield::on_weapon_hit(target, hit_data, config);
    }

    Reflyem::Vampirism::on_weapon_hit(target, hit_data, config);

    _weapon_hit(target, hit_data);
    return;
  }

  auto install_hooks() -> void
  {
    logger::info("start install hooks");
    auto& trampoline = SKSE::GetTrampoline();
    trampoline.create(1024);
    OnWeaponHit::install_hook(trampoline);
    OnMainUpdate::install_hook(trampoline);
    OnAdjustActiveEffect::install_hook(trampoline);
    OnAnimationEventNpc::install_hook();
    OnAnimationEventPc::install_hook();
    OnModifyActorValue::install_hook();
    OnDualModifyActorValue::install_hook();
    OnDualModifyActorValueSecondInnerCall::install_hook(trampoline);
    // OnAttackData::install_hook(trampoline);
    logger::info("finish install hooks");
  }
}