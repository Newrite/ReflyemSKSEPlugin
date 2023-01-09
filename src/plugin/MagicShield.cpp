#include "MagicShield.h"
#include "Core.h"

namespace Reflyem
{
  namespace MagicShield
  {

    auto on_weapon_hit(
      RE::Actor* target,
      RE::HitData& hit_data,
      const Reflyem::Config& config) -> void
    {

      auto magic_shield_percent = target->GetActorValue(config.magic_shield_av);

      if (magic_shield_percent <= 0.f) { return; }
      if (magic_shield_percent > 100.f) { magic_shield_percent = 100.f; }

      auto damage_mult = Reflyem::Core::getting_damage_mult(*target);

      auto absorb_damage = (hit_data.totalDamage * damage_mult) * (magic_shield_percent / 100.f);
      auto magicka = target->GetActorValue(RE::ActorValue::kMagicka);

      auto can_absorb = 0.f;
      auto magicka_damage = 0.f;
      auto cost_per_damage =
        Reflyem
        ::Core
        ::get_effect_with_keyword_value(*target, *config.magic_shield_cost_keyword).value_or(1.f);

      for (auto absorb = 0.f; absorb < absorb_damage; absorb++)
      {
        if (magicka_damage < magicka)
        {
          magicka_damage += cost_per_damage;
          can_absorb = absorb;
        }
        else
        {
          break;
        }
      }

      target->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kMagicka, -magicka_damage);
      hit_data.totalDamage -= can_absorb / damage_mult;

    }
  }
}