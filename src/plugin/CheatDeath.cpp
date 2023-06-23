#include "plugin/CheatDeath.hpp"
#include "Core.hpp"

namespace Reflyem
{
namespace CheatDeath
{

auto cheat_death(RE::Actor& target, float& damage_value, const Config& config) -> void
{
  const auto effects =
      Core::get_effects_by_keyword(target, *config.cheat_death().mgef_percent_keyword());
  auto cheat_death_percent = Core::get_effects_magnitude_sum(effects).value_or(100.0f);

  if (cheat_death_percent >= 100.f) { return; }
  if (cheat_death_percent < 0.f) { cheat_death_percent = 0.f; }

  const auto damage_mult = Core::getting_damage_mult(target);

  if (const auto health_threshold = Core::get_actor_value_max(&target, RE::ActorValue::kHealth) *
                                    (cheat_death_percent / 100.f);
      (damage_value * damage_mult) >= health_threshold)
    {
      damage_value = health_threshold / damage_mult;
    }
}

auto modify_actor_value(
    const RE::ValueModifierEffect* this_,
    RE::Actor* actor,
    float& value,
    const RE::ActorValue av,
    const Config& config) -> void
{
  if (Core::can_modify_actor_value(this_, actor, value, av))
    {
      value = std::abs(value);
      cheat_death(*actor, value, config);
      value = -value;
    }
}

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void
{
  cheat_death(*target, hit_data.totalDamage, config);
}
} // namespace CheatDeath
} // namespace Reflyem
