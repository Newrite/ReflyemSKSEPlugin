#include "Config.hpp"
#include "Core.hpp"

namespace Reflyem::SoulLink
{
auto on_weapon_hit(const RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void
{
  const auto total_damage = hit_data.totalDamage;
  if (target->IsCommandedActor())
    {
      const auto commander = target->currentProcess->GetCommandingActor().get().get();
      for (auto& commanded_data : commander->currentProcess->middleHigh->commandedActors)
      {
        const auto commanded_actor = commanded_data.commandedActor.get().get();
        if (commanded_actor == target || hit_data.totalDamage <= 0.f)
        {
          continue;
        }
        const auto damage = total_damage * 0.2f;
        Core::damage_actor_value(*commanded_actor, RE::ActorValue::kHealth, damage);
        hit_data.totalDamage -= damage;
        if (hit_data.totalDamage < 0.f)
        {
          hit_data.totalDamage = 0.f;
        }
      }
    return;
    }

  for (auto& commanded_data : target->currentProcess->middleHigh->commandedActors)
  {
    const auto commanded_actor = commanded_data.commandedActor.get().get();
    if (commanded_actor == target || hit_data.totalDamage <= 0.f)
    {
      continue;
    }
    const auto damage = total_damage * 0.2f;
    Core::damage_actor_value(*commanded_actor, RE::ActorValue::kHealth, damage);
    hit_data.totalDamage -= damage;
    if (hit_data.totalDamage < 0.f)
    {
      hit_data.totalDamage = 0.f;
    }
  }
    
  }
}