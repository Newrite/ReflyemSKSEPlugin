#include "Vampirism.h"
#include "Core.h"

namespace reflyem
{
  namespace vampirism
  {

    auto vampirism(
      RE::Actor& target,
      RE::Actor& agressor,
      RE::HitData& hit_data,
      RE::ActorValue av,
      float vampirism_percent) -> void
    {

      if (vampirism_percent <= 0.f) { return; }

      if (vampirism_percent > 100.f) { vampirism_percent = 100.f; }

      auto damage_mult = reflyem::core::getting_damage_mult(target);
      if (av != RE::ActorValue::kHealth) { damage_mult = 1.f; }

      auto target_value = target.GetActorValue(RE::ActorValue::kHealth);
      auto vampirism_value = (hit_data.totalDamage * damage_mult) * (vampirism_percent / 100.f);

      if (vampirism_value > target_value)
      {
        agressor.RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, av, target_value);
      }
      else
      {
        agressor.RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, av, vampirism_value);
      }
    }

    auto av_vampirism(
      RE::Actor& target,
      RE::Actor& agressor,
      RE::HitData& hit_data,
      const reflyem::config& config) -> void
    {

      auto vampirism_percent = agressor.GetActorValue(config.vampirism_av);
      vampirism(target, agressor, hit_data, RE::ActorValue::kHealth, vampirism_percent);

    }

    auto mgef_vampirism(
      RE::Actor& target,
      RE::Actor& agressor,
      RE::HitData& hit_data,
      RE::BGSKeyword& key,
      RE::ActorValue av) -> void
    {
      auto vampirism_percent = 
        reflyem
        ::core
        ::get_effect_with_keyword_value(agressor, key)
        .value_or(0.f);

      vampirism(target, agressor, hit_data, av, vampirism_percent);
    }

    auto on_weapon_hit(
      RE::Actor* target,
      RE::HitData& hit_data,
      const reflyem::config& config) -> void
    {

      auto agressor = hit_data.aggressor.get();

      if (!agressor || target->IsDead()) { return; }

      if (config.vampirism_enable)
      {
        av_vampirism(*target, *agressor, hit_data, config);
      }

      if (config.vampirism_mgef_health_enable)
      {
        mgef_vampirism(*target, *agressor, hit_data, *config.vampirism_mgef_health_keyword, RE::ActorValue::kHealth);
      }

      if (config.vampirism_mgef_stamina_enable)
      {
        mgef_vampirism(*target, *agressor, hit_data, *config.vampirism_mgef_stamina_keyword, RE::ActorValue::kStamina);
      }

      if (config.vampirism_mgef_magicka_enable)
      {
        mgef_vampirism(*target, *agressor, hit_data, *config.vampirism_mgef_magicka_keyword, RE::ActorValue::kMagicka);
      }

    }
  }

}