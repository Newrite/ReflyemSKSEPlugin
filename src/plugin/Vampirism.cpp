#include "Vampirism.h"
#include "Core.h"

namespace Reflyem
{
  namespace Vampirism
  {

    auto vampirism(
      RE::Actor& target,
      RE::Actor& agressor,
      float& damage_value,
      RE::ActorValue av,
      float vampirism_percent) -> void
    {

      if (vampirism_percent <= 0.f) { return; }

      if (vampirism_percent > 100.f) { vampirism_percent = 100.f; }

      auto damage_mult = Reflyem::Core::getting_damage_mult(target);
      if (av != RE::ActorValue::kHealth) { damage_mult = 1.f; }

      auto target_value = target.GetActorValue(RE::ActorValue::kHealth);
      auto vampirism_value = (damage_value * damage_mult) * (vampirism_percent / 100.f);

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
      float& damage_value,
      const Reflyem::Config& config) -> void
    {

      auto vampirism_percent = agressor.GetActorValue(config.vampirism_av);
      vampirism(target, agressor, damage_value, RE::ActorValue::kHealth, vampirism_percent);

    }

    auto mgef_vampirism(
      RE::Actor& target,
      RE::Actor& agressor,
      float& damage_value,
      RE::BGSKeyword& key,
      RE::ActorValue av) -> void
    {
      auto effects =
        Reflyem::Core::get_effects_by_keyword(agressor, key);
      auto vampirism_percent =
        Reflyem
        ::Core
        ::get_effects_magnitude_sum(effects)
        .value_or(0.f);

      vampirism(target, agressor, damage_value, av, vampirism_percent);
    }

    auto modify_actor_value(
      RE::ValueModifierEffect* a_this,
      RE::Actor* a_actor, float& a_value,
      RE::ActorValue av,
      const Reflyem::Config& config) -> void
    {

      if (Reflyem::Core::can_modify_actor_value(a_this, a_actor, a_value, av))
      {
        auto agressor = a_this->GetCasterActor().get();

        if (!agressor || a_actor->IsDead()) { return; }

        if (config.magic_vampirism_enable)
        {
          av_vampirism(*a_actor, *agressor, a_value, config);
        }

        if (config.magic_vampirism_mgef_health_enable)
        {
          mgef_vampirism(*a_actor, *agressor, a_value, *config.magic_vampirism_mgef_health_keyword, RE::ActorValue::kHealth);
        }

        if (config.magic_vampirism_mgef_stamina_enable)
        {
          mgef_vampirism(*a_actor, *agressor, a_value, *config.magic_vampirism_mgef_stamina_keyword, RE::ActorValue::kStamina);
        }

        if (config.magic_vampirism_mgef_magicka_enable)
        {
          mgef_vampirism(*a_actor, *agressor, a_value, *config.magic_vampirism_mgef_magicka_keyword, RE::ActorValue::kMagicka);
        }
      }

    }

    auto on_weapon_hit(
      RE::Actor* target,
      RE::HitData& hit_data,
      const Reflyem::Config& config) -> void
    {

      auto agressor = hit_data.aggressor.get();

      if (!agressor || target->IsDead()) { return; }

      if (config.vampirism_enable)
      {
        av_vampirism(*target, *agressor, hit_data.totalDamage, config);
      }

      if (config.vampirism_mgef_health_enable)
      {
        mgef_vampirism(*target, *agressor, hit_data.totalDamage, *config.vampirism_mgef_health_keyword, RE::ActorValue::kHealth);
      }

      if (config.vampirism_mgef_stamina_enable)
      {
        mgef_vampirism(*target, *agressor, hit_data.totalDamage, *config.vampirism_mgef_stamina_keyword, RE::ActorValue::kStamina);
      }

      if (config.vampirism_mgef_magicka_enable)
      {
        mgef_vampirism(*target, *agressor, hit_data.totalDamage, *config.vampirism_mgef_magicka_keyword, RE::ActorValue::kMagicka);
      }

    }
  }

}