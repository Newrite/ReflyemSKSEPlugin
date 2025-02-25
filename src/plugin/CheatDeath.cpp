#include "plugin/CheatDeath.hpp"
#include "Core.hpp"

namespace Reflyem { namespace CheatDeath {

auto cheat_death(RE::Actor* target, float& damage_value, const Config& config) -> void
{

  if (!target) {
    return;
  }

  const auto effects = Core::try_get_effects_by_keyword(target, config.cheat_death().mgef_percent_keyword());
  auto cheat_death_percent = Core::get_effects_magnitude_sum(effects).value_or(100.0f);

  if (cheat_death_percent >= 100.f) {
    return;
  }
  if (cheat_death_percent < 0.f) {
    cheat_death_percent = 0.f;
  }

  const auto damage_mult = Core::getting_damage_mult(*target);

  if (const auto health_threshold =
          Core::get_actor_value_max(target, RE::ActorValue::kHealth) * (cheat_death_percent / 100.f);
      (damage_value * damage_mult) >= health_threshold) {
    damage_value = health_threshold / damage_mult;
    if (target->IsPlayerRef()) {
      Core::console_log(
          std::format("CHEAT_DEATH: Actor - {} | CheatDeathPercent - {} | HealthThreshold - {} | New DamageValue - {}",
                      target->GetDisplayFullName(),
                      cheat_death_percent,
                      health_threshold,
                      damage_value));
    }
  }
}

auto effect_added(RE::MagicTarget* this_, RE::ActiveEffect* active_effect, const Config& config) -> void
{
  if (!this_ || !active_effect) {
    return;
  }

  auto effect = active_effect->effect;
  if (!effect) {
    return;
  }

  auto base_effect = effect->baseEffect;
  if (!base_effect) {
    return;
  }

  let archetype = base_effect->GetArchetype();
  if (archetype == RE::EffectArchetype::kAbsorb || archetype == RE::EffectArchetype::kDualValueModifier ||
      archetype == RE::EffectArchetype::kPeakValueModifier || archetype == RE::EffectArchetype::kValueModifier) {
    if (base_effect->data.primaryAV == RE::ActorValue::kHealth && active_effect->duration <= 1.f) {
      const auto target = (RE::Actor*)((char*)this_ - 0x98);
      if (target) {
        cheat_death(target, active_effect->magnitude, config);
      }
    }
  }
}

auto modify_actor_value(const RE::ValueModifierEffect* this_,
                        RE::Actor* actor,
                        float& value,
                        const RE::ActorValue av,
                        const Config& config) -> void
{
  if (Core::can_modify_actor_value(this_, actor, value, av)) {
    value = std::abs(value);
    cheat_death(actor, value, config);
    value = -value;
  }
}

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void
{
  cheat_death(target, hit_data.totalDamage, config);
}
} // namespace CheatDeath
} // namespace Reflyem
