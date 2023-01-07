#include "Hooks.h"
#include "Core.h"
#include "Vampirism.h"
#include "MagicShield.h"
#include "PetrifiedBlood.h"
#include "CheatDeath.h"
#include "SpeedCasting.h"
#include "CastOnHit.h"
#include "WeaponCrit.h"

namespace hooks
{

  static float timer = 0.f;

  auto on_adjust_active_effect::adjust_active_effect(RE::ActiveEffect* a_this, float a_power, bool a_arg3) -> void
  {
    logger::info("adjust effect, magnitude {}: power {}: arg3 {}:", a_this->magnitude, a_power, a_arg3);
    _adjust_active_effect(a_this, a_power, a_arg3);
    return;
  }

  auto on_main_update::main_update(RE::Main* a_this, float a2) -> void
  {

    auto r_int = reflyem::core::get_rundom_int();
    logger::info("update standart: rint {}: timer: {} delta: {}", r_int, timer, a2);

    timer += a2;
    if (timer >= 0.5f)
    {
      timer = 0.f;
      logger::info("update timer: {}", timer);
      auto& config = reflyem::config::get_singleton();

      if (config.speed_casting_enable)
      {
        reflyem::speed_casting::on_main_update(a_this, a2, config);
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
    trampoline.create(64);
    on_weapon_hit::install_hook(trampoline);
    on_main_update::install_hook(trampoline);
    on_adjust_active_effect::install_hook(trampoline);
    logger::info("finish install hooks");
  }
}