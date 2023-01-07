#pragma once

#include "CheatDeath.h"
#include "Core.h"

namespace reflyem
{
  namespace cheat_death
  {
    auto on_weapon_hit(
      RE::Actor* target,
      RE::HitData& hit_data,
      const reflyem::config& config) -> void
    {

      auto cheat_death_percent = 
        reflyem
        ::core
        ::get_effect_with_keyword_value(*target, *config.cheat_death_percent_keyword).value_or(100.0f);

      if (cheat_death_percent >= 100.f) { return; }
      if (cheat_death_percent < 0.f) { cheat_death_percent = 0.f; }

      auto damage_mult = reflyem::core::getting_damage_mult(*target);

      auto health_treshold = target->GetActorValue(RE::ActorValue::kHealth) * (cheat_death_percent / 100.f);

      if ((hit_data.totalDamage * damage_mult) >= health_treshold)
      {
        hit_data.totalDamage = health_treshold / damage_mult;
      }
    }
  }
}