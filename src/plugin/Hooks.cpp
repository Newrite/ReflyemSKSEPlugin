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

namespace hooks
{

  static ULONGLONG timer500 = 0;
  static ULONGLONG timer100 = 0;

  auto on_animation_event_npc::process_event(
    RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this,
    RE::BSAnimationGraphEvent* a_event,
    RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_dispatcher) -> void
  {
    if (a_event && a_event->holder)
    {
      auto& config = reflyem::config::get_singleton();
      reflyem::animation_event_handler::animation_handler(a_event, config);
    }
    _process_event(a_this, a_event, a_dispatcher);
    return;
  }

  auto on_animation_event_pc::process_event(
    RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this,
    RE::BSAnimationGraphEvent* a_event,
    RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_dispatcher) -> void
  {
    if (a_event && a_event->holder)
    {
      auto& config = reflyem::config::get_singleton();
      reflyem::animation_event_handler::animation_handler(a_event, config);
    }
    _process_event(a_this, a_event, a_dispatcher);
    return;
  }

  auto on_adjust_active_effect::adjust_active_effect(RE::ActiveEffect* a_this, float a_power, bool a_arg3) -> void
  {
    _adjust_active_effect(a_this, a_power, a_arg3);
    return;
  }

  auto on_main_update::main_update(RE::Main* a_this, float a2) -> void
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
      auto& config = reflyem::config::get_singleton();

      if (config.speed_casting_enable)
      {
        reflyem::speed_casting::on_main_update(a_this, a2, config);
      }
    }
    
    if (tick - timer100 >= 100)
    {
      timer100 = tick;
      auto& config = reflyem::config::get_singleton();
      if (config.resource_manager_enable)
      {
        reflyem::resource_manager::ranged_spend_handler();
      }
    }

    _main_update(a_this, a2);
    return;
  }

  auto on_weapon_hit::weapon_hit(RE::Actor* target, RE::HitData& hit_data) -> void
  {

    if (!target) {
      _weapon_hit(target, hit_data);
      return;
    }

    auto& config = reflyem::config::get_singleton();

    if (config.weapon_crit_enable)
    {
      reflyem::weapon_crit::on_weapon_hit(target, hit_data, config);
    }

    if (config.cast_on_hit_enable)
    {
      reflyem::cast_on_hit::on_weapon_hit(target, hit_data, config);
    }

    if (config.cheat_death_enable)
    {
      reflyem::cheat_death::on_weapon_hit(target, hit_data, config);
    }

    if (config.petrified_blood_enable)
    {
      reflyem::petrified_blood::on_weapon_hit(target, hit_data, config);
    }

    if (config.magic_shield_enable)
    {
      reflyem::magic_shield::on_weapon_hit(target, hit_data, config);
    }

    reflyem::vampirism::on_weapon_hit(target, hit_data, config);

    _weapon_hit(target, hit_data);
    return;
  }

  auto install_hooks() -> void
  {
    logger::info("start install hooks");
    auto& trampoline = SKSE::GetTrampoline();
    trampoline.create(1024);
    on_weapon_hit::install_hook(trampoline);
    on_main_update::install_hook(trampoline);
    on_adjust_active_effect::install_hook(trampoline);
    on_animation_event_npc::install_hook();
    on_animation_event_pc::install_hook();
    logger::info("finish install hooks");
  }
}