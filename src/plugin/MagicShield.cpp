#include "MagicShield.h"
#include "Core.h"

namespace Reflyem
{
  namespace MagicShield
  {

    auto magic_shield(
      RE::Actor& target,
      float& damage_value,
      const Reflyem::Config& config) -> void
    {

      auto magic_shield_percent = target.GetActorValue(config.magic_shield_av);

      if (magic_shield_percent <= 0.f) { return; }
      if (magic_shield_percent > 100.f) { magic_shield_percent = 100.f; }

      auto damage_mult = Reflyem::Core::getting_damage_mult(target);

      auto absorb_damage = (damage_value * damage_mult) * (magic_shield_percent / 100.f);
      auto magicka = target.GetActorValue(RE::ActorValue::kMagicka);

      auto effects =
        Reflyem::Core::get_effects_by_keyword(target, *config.magic_shield_cost_keyword);
      auto cost_per_damage =
        Reflyem
        ::Core
        ::get_effects_magnitude_sum(effects).value_or(1.f);

      auto absorb_cost = magicka - (absorb_damage * cost_per_damage);
      logger::info("magicka: {}, percent: {}, absorb_cost: {}, absorb_damage: {}, cost_per_damage: {}"
        , magicka, magic_shield_percent, absorb_cost, absorb_damage, cost_per_damage);
      if (absorb_cost >= 0.f)
      {
        Reflyem::Core::damage_actor_value(target, RE::ActorValue::kMagicka, magicka - absorb_cost);
        damage_value -= absorb_damage / damage_mult;
      }
      else
      {
        auto damage = std::abs(absorb_cost) / cost_per_damage;
        Reflyem::Core::damage_actor_value(target, RE::ActorValue::kMagicka, magicka);
        auto calc = damage / damage_mult + (damage_value * ((100.f - magic_shield_percent) / 100.f));
        logger::info("else damage: {}, damage_value: {}, calc: {}", damage, damage_value, calc);
        damage_value = calc;
      }

    }

    auto modify_actor_value(
      RE::ValueModifierEffect* a_this,
      RE::Actor* a_actor, float& a_value,
      RE::ActorValue av,
      const Reflyem::Config& config) -> void
    {

      if (Reflyem::Core::can_modify_actor_value(a_this, a_actor, a_value, av))
      {
        logger::info("magic shield value before: {}", a_value); 
        a_value = std::abs(a_value);
        magic_shield(*a_actor, a_value, config);
        a_value = -a_value;
        logger::info("magic shield value after: {}", a_value);
      }

    }

    auto on_weapon_hit(
      RE::Actor* target,
      RE::HitData& hit_data,
      const Reflyem::Config& config) -> void
    {

      magic_shield(*target, hit_data.totalDamage, config);

    }
  }
}