#include "plugin/StaminaShield.hpp"
#include "Core.hpp"

namespace Reflyem::StaminaShield
{

auto stamina_shield(RE::Actor& target, float& damage_value, const Config& config) -> void
{
  auto stamina_shield_percent = target.GetActorValue(config.stamina_shield().av());

  if (stamina_shield_percent <= 0.f) { return; }
  if (stamina_shield_percent > 100.f) { stamina_shield_percent = 100.f; }

  const auto damage_mult = Core::getting_damage_mult(target);

  auto absorb_damage = (damage_value * damage_mult) * (stamina_shield_percent / 100.f);
  auto stamina = target.GetActorValue(RE::ActorValue::kStamina);

  const auto effects =
      Core::get_effects_by_keyword(target, *config.stamina_shield().mgef_cost_keyword());
  auto cost_per_damage = Core::get_effects_magnitude_sum(effects).value_or(1.f);

  auto absorb_cost = stamina - (absorb_damage * cost_per_damage);
  logger::debug(
      "stamina: {}, percent: {}, absorb_cost: {}, absorb_damage: {}, "
      "cost_per_damage: {}"sv,
      stamina,
      stamina_shield_percent,
      absorb_cost,
      absorb_damage,
      cost_per_damage);
  if (absorb_cost >= 0.f)
    {
      Core::damage_actor_value(target, RE::ActorValue::kStamina, stamina - absorb_cost);
      damage_value -= absorb_damage / damage_mult;
    }
  else
    {
      auto damage = std::abs(absorb_cost) / cost_per_damage;
      Core::damage_actor_value(target, RE::ActorValue::kStamina, stamina);
      auto calc =
          damage / damage_mult + (damage_value * ((100.f - stamina_shield_percent) / 100.f));
      logger::debug("else damage: {}, damage_value: {}, calc: {}"sv, damage, damage_value, calc);
      damage_value = calc;
    }
}

auto allow_magic_shield_effect(const RE::ActiveEffect& active_effect, const Config& config) -> bool
{
  if (!active_effect.effect || !active_effect.effect->baseEffect) { return false; }
  const auto base_effect = active_effect.effect->baseEffect;
  if (config.stamina_shield().must_be_or_not_be())
    {
      return base_effect->HasKeyword(config.stamina_shield().mgef_keyword());
    }
  return !base_effect->HasKeyword(config.stamina_shield().mgef_keyword());
}

auto modify_actor_value(
    const RE::ValueModifierEffect* this_,
    RE::Actor* actor,
    float& value,
    const RE::ActorValue av,
    const Config& config) -> void
{
  if (Core::can_modify_actor_value(this_, actor, value, av) &&
      allow_magic_shield_effect(*this_, config))
    {
      logger::debug("stamina shield value before: {}"sv, value);
      value = std::abs(value);
      stamina_shield(*actor, value, config);
      value = -value;
      logger::debug("stamina shield value after: {}"sv, value);
    }
}

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void
{
  stamina_shield(*target, hit_data.totalDamage, config);
}
} // namespace Reflyem::StaminaShield