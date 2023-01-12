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
      if (absorb_cost >= 0.f)
      {
        Reflyem::Core::damage_actor_value(target, RE::ActorValue::kMagicka, magicka - absorb_cost);
        damage_value -= absorb_damage / damage_mult;
      }
      else
      {
        auto weapon_damage = std::abs(absorb_cost) / cost_per_damage;
        Reflyem::Core::damage_actor_value(target, RE::ActorValue::kMagicka, magicka);
        damage_value -= weapon_damage / damage_mult;
      }

    }

    auto modify_actor_value(
      RE::ValueModifierEffect* a_this,
      RE::Actor* a_actor, float& a_value,
      RE::ActorValue,
      const Reflyem::Config&) -> void
    {
      logger::info("caster ptr");
      auto caster_ptr = a_this->GetCasterActor();
      if (!caster_ptr || !a_actor || !a_this->effect || !a_this->effect->baseEffect) { return; }

      logger::info("caster");
      auto caster = caster_ptr.get();
      if (!caster) { return; }

      logger::info("caster equal actor");
      if (caster == a_actor) { return; }

      logger::info("actor value: {}, value: {}, magnitude: {}, valueeffect: {}", a_this->actorValue, a_value, a_this->magnitude, a_this->value);
      a_value = 0.f;

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