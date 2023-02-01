#include "plugin/MagicShield.hpp"
#include "Core.hpp"

namespace Reflyem {
namespace MagicShield {

auto magic_shield(RE::Actor& target, float& damage_value, const Config& config) -> void {
  auto magic_shield_percent = target.GetActorValue(config.magic_shield().av());

  if (magic_shield_percent <= 0.f) {
    return;
  }
  if (magic_shield_percent > 100.f) {
    magic_shield_percent = 100.f;
  }

  const auto damage_mult = Core::getting_damage_mult(target);

  auto absorb_damage = (damage_value * damage_mult) * (magic_shield_percent / 100.f);
  auto magicka       = target.GetActorValue(RE::ActorValue::kMagicka);

  const auto effects =
      Core::get_effects_by_keyword(target, *config.magic_shield().mgef_cost_keyword());
  auto cost_per_damage = Core::get_effects_magnitude_sum(effects).value_or(1.f);

  auto absorb_cost = magicka - (absorb_damage * cost_per_damage);
  logger::debug(
      "magicka: {}, percent: {}, absorb_cost: {}, absorb_damage: {}, cost_per_damage: {}"sv,
      magicka, magic_shield_percent, absorb_cost, absorb_damage, cost_per_damage);
  if (absorb_cost >= 0.f) {
    Core::damage_actor_value(target, RE::ActorValue::kMagicka, magicka - absorb_cost);
    damage_value -= absorb_damage / damage_mult;
  } else {
    auto damage = std::abs(absorb_cost) / cost_per_damage;
    Core::damage_actor_value(target, RE::ActorValue::kMagicka, magicka);
    auto calc = damage / damage_mult + (damage_value * ((100.f - magic_shield_percent) / 100.f));
    logger::debug("else damage: {}, damage_value: {}, calc: {}"sv, damage, damage_value, calc);
    damage_value = calc;
  }
}

auto allow_magic_shield_effect(const RE::ActiveEffect& active_effect, const Config& config)
    -> bool {
  if (!active_effect.effect || !active_effect.effect->baseEffect) {
    return false;
  }
  const auto base_effect = active_effect.effect->baseEffect;
  if (config.magic_shield().must_be_or_not_be()) {
    return base_effect->HasKeyword(config.magic_shield().mgef_keyword());
  }
  return !base_effect->HasKeyword(config.magic_shield().mgef_keyword());
}

auto modify_actor_value(const RE::ValueModifierEffect* this_, RE::Actor* actor, float& value,
                        const RE::ActorValue av, const Config& config) -> void {
  if (Core::can_modify_actor_value(this_, actor, value, av) &&
      allow_magic_shield_effect(*this_, config)) {
    logger::debug("magic shield value before: {}"sv, value);
    value = std::abs(value);
    magic_shield(*actor, value, config);
    value = -value;
    logger::debug("magic shield value after: {}"sv, value);
  }
}

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void {
  magic_shield(*target, hit_data.totalDamage, config);
}
} // namespace MagicShield
} // namespace Reflyem
