#include "plugin/Vampirism.hpp"
#include "Core.hpp"

namespace Reflyem::Vampirism {

auto vampirism(RE::Actor& target, RE::Actor& aggressor, const float& damage_value,
               const RE::ActorValue av, float vampirism_percent) -> void {
  if (vampirism_percent <= 0.f) {
    return;
  }

  if (vampirism_percent > 100.f) {
    vampirism_percent = 100.f;
  }

  auto damage_mult = Core::getting_damage_mult(target);
  if (av != RE::ActorValue::kHealth) {
    damage_mult = 1.f;
  }

  const auto target_value    = target.GetActorValue(RE::ActorValue::kHealth);
  const auto vampirism_value = (damage_value * damage_mult) * (vampirism_percent / 100.f);

  if (vampirism_value > target_value) {
    aggressor.RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, av, target_value);
  } else {
    aggressor.RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, av, vampirism_value);
  }
}

auto av_vampirism(RE::Actor& target, RE::Actor& aggressor, const float& damage_value,
                  const Config& config) -> void {
  const auto vampirism_percent = aggressor.GetActorValue(config.vampirism().av());
  vampirism(target, aggressor, damage_value, RE::ActorValue::kHealth, vampirism_percent);
}

auto mgef_vampirism(RE::Actor& target, RE::Actor& aggressor, const float& damage_value,
                    const RE::BGSKeyword& key, const RE::ActorValue av) -> void {
  const auto effects           = Core::get_effects_by_keyword(aggressor, key);
  const auto vampirism_percent = Core::get_effects_magnitude_sum(effects).value_or(0.f);

  vampirism(target, aggressor, damage_value, av, vampirism_percent);
}

auto allow_vampirism_effect(const RE::ActiveEffect& active_effect, const Config& config) -> bool {
  if (!active_effect.effect || !active_effect.effect->baseEffect) {
    return false;
  }
  const auto base_effect = active_effect.effect->baseEffect;
  if (config.magic_vampirism().must_be_or_not_be()) {
    return base_effect->HasKeyword(config.magic_vampirism().mgef_keyword());
  }
  return !base_effect->HasKeyword(config.magic_vampirism().mgef_keyword());
}

auto modify_actor_value(const RE::ValueModifierEffect* this_, RE::Actor* actor, const float& value,
                        const RE::ActorValue av, const Config& config) -> void {
  if (Core::can_modify_actor_value(this_, actor, value, av)) {
    const auto aggressor = this_->GetCasterActor().get();

    if (!aggressor) {
      return;
    }

    if (config.magic_vampirism().enable() && allow_vampirism_effect(*this_, config)) {
      av_vampirism(*actor, *aggressor, value, config);
    }

    if (config.magic_vampirism().mgef_health_enable() && allow_vampirism_effect(*this_, config)) {
      mgef_vampirism(*actor, *aggressor, value, *config.magic_vampirism().mgef_health_keyword(),
                     RE::ActorValue::kHealth);
    }

    if (config.magic_vampirism().mgef_stamina_enable() && allow_vampirism_effect(*this_, config)) {
      mgef_vampirism(*actor, *aggressor, value, *config.magic_vampirism().mgef_stamina_keyword(),
                     RE::ActorValue::kStamina);
    }

    if (config.magic_vampirism().mgef_magicka_enable() && allow_vampirism_effect(*this_, config)) {
      mgef_vampirism(*actor, *aggressor, value, *config.magic_vampirism().mgef_magicka_keyword(),
                     RE::ActorValue::kMagicka);
    }
  }
}

auto on_weapon_hit(RE::Actor* target, const RE::HitData& hit_data, const Config& config) -> void {
  const auto aggressor = hit_data.aggressor.get();

  if (!aggressor || target->IsDead()) {
    return;
  }

  if (config.vampirism().enable()) {
    av_vampirism(*target, *aggressor, hit_data.totalDamage, config);
  }

  if (config.vampirism().mgef_health_enable()) {
    mgef_vampirism(*target, *aggressor, hit_data.totalDamage,
                   *config.vampirism().mgef_health_keyword(), RE::ActorValue::kHealth);
  }

  if (config.vampirism().mgef_stamina_enable()) {
    mgef_vampirism(*target, *aggressor, hit_data.totalDamage,
                   *config.vampirism().mgef_stamina_keyword(), RE::ActorValue::kStamina);
  }

  if (config.vampirism().mgef_magicka_enable()) {
    mgef_vampirism(*target, *aggressor, hit_data.totalDamage,
                   *config.vampirism().mgef_magicka_keyword(), RE::ActorValue::kMagicka);
  }
}

} // namespace Reflyem::Vampirism
