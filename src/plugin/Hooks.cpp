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

    if (config.resource_manager_enable)
    {
      Reflyem::ResourceManager::on_weapon_hit(target, hit_data, config);
    }

    if (config.weapon_crit_enable)
    {
      Reflyem::WeaponCrit::on_weapon_hit(target, hit_data, config);
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
    // OnAttackData::install_hook(trampoline);
    logger::info("finish install hooks");
  }
}