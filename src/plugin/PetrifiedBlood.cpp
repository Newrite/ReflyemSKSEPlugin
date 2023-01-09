#include "PetrifiedBlood.h"
#include "Core.h"

namespace Reflyem
{
  namespace PetrifiedBlood
  {
    auto on_weapon_hit(
      RE::Actor* target,
      RE::HitData& hit_data,
      const Reflyem::Config& config) -> void
    {

      auto petrified_blood_percent = target->GetActorValue(config.petrified_blood_av);
      if (petrified_blood_percent <= 0.f) { return; }

      if (petrified_blood_percent > 100.f) { petrified_blood_percent = 100.f; }

      auto blood_duration = config.petrified_blood_spell->effects[0]->effectItem.duration;

      auto damage_mult = Reflyem::Core::getting_damage_mult(*target);
      auto blood_damage = (hit_data.totalDamage * damage_mult) * (petrified_blood_percent / 100.f);
      auto blood_damage_tick = (blood_damage / blood_duration) / damage_mult;

      config.petrified_blood_spell
        ->effects[0]
        ->effectItem.magnitude = blood_damage_tick;

      target
        ->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
        ->CastSpellImmediate(
          config.petrified_blood_spell,
          true,
          target,
          1.00f,
          false,
          0.0f,
          nullptr
        );

      hit_data.totalDamage -= (blood_damage / damage_mult);
    }
  }
}
  