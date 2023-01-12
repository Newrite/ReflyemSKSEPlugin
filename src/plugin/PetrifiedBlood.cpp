#include "PetrifiedBlood.h"
#include "Core.h"

namespace Reflyem
{
  namespace PetrifiedBlood
  {

    auto petrified_blood(
      RE::Actor& target,
      float& damage_value,
      const Reflyem::Config& config) -> void
    {

      auto petrified_blood_percent = target.GetActorValue(config.petrified_blood_av);
      if (petrified_blood_percent <= 0.f) { return; }

      if (petrified_blood_percent > 100.f) { petrified_blood_percent = 100.f; }

      auto blood_duration = config.petrified_blood_spell->effects[0]->effectItem.duration;

      auto damage_mult = Reflyem::Core::getting_damage_mult(*target);
      auto blood_damage = (damage_value * damage_mult) * (petrified_blood_percent / 100.f);
      auto blood_damage_tick = (blood_damage / blood_duration) / damage_mult;

      config.petrified_blood_spell
        ->effects[0]
        ->effectItem.magnitude = blood_damage_tick;

      target.GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
        ->CastSpellImmediate(
          config.petrified_blood_spell,
          true,
          &target,
          1.00f,
          false,
          0.0f,
          nullptr
        );

      damage_value -= (blood_damage / damage_mult);
    }

    auto modify_actor_value(
      RE::ValueModifierEffect* a_this,
      RE::Actor* a_actor, float& a_value,
      RE::ActorValue av,
      const Reflyem::Config& config) -> void
    {

      if (Reflyem::Core::can_modify_actor_value(a_this, a_actor, a_value, av))
      {
        a_value = std::abs(a_value);
        petrified_blood(*a_actor, a_value, config);
        a_value = -a_value;
      }

    }

    auto on_weapon_hit(
      RE::Actor* target,
      RE::HitData& hit_data,
      const Reflyem::Config& config) -> void
    {
      petrified_blood(*target, hit_data.totalDamage, config);
    }
  }
}
  