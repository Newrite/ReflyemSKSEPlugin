#include "plugin/PetrifiedBlood.hpp"
#include "Core.hpp"

namespace Reflyem::PetrifiedBlood {

auto petrified_blood_cast(RE::Actor& target, const float blood_damage_tick, const Config& config)
    -> void {
  config.petrified_blood().blood_spell()->effects[0]->effectItem.magnitude = blood_damage_tick;

  Core::cast(*config.petrified_blood().blood_spell(), target, target);
}

auto petrified_blood(RE::Actor& target, float& damage_value, const Config& config) -> float {
  auto petrified_blood_percent = target.GetActorValue(config.petrified_blood().av());
  if (petrified_blood_percent <= 0.f) {
    return 0.f;
  }

  if (petrified_blood_percent > 100.f) {
    petrified_blood_percent = 100.f;
  }

  const auto blood_duration =
      config.petrified_blood().blood_spell()->effects[0]->effectItem.duration;

  const auto blood_damage      = damage_value * (petrified_blood_percent / 100.f);
  const auto blood_damage_tick = blood_damage / static_cast<float>(blood_duration);

  damage_value -= blood_damage;

  return blood_damage_tick;
}

auto character_update(RE::Character& character, float, const Config& config) -> void {
  const auto effects_damage = Core::get_effects_by_keyword(
      character, *config.petrified_blood().mgef_accumulate_keyword_damage());

  if (effects_damage.empty()) {
    return;
  }

  const auto blood_damage_tick    = effects_damage.at(0)->magnitude;
  effects_damage.at(0)->magnitude = 0.f;
  petrified_blood_cast(character, blood_damage_tick, config);
}

auto allow_petrified_blood_effect(const RE::ActiveEffect& active_effect, const Config& config)
    -> bool {
  if (!active_effect.effect || !active_effect.effect->baseEffect) {
    return false;
  }
  const auto base_effect = active_effect.effect->baseEffect;
  if (config.petrified_blood().must_be_or_not_be()) {
    return base_effect->HasKeyword(config.petrified_blood().mgef_keyword());
  }
  return !base_effect->HasKeyword(config.petrified_blood().mgef_keyword());
}

auto modify_actor_value(const RE::ValueModifierEffect* this_, RE::Actor* actor, float& value,
                        const RE::ActorValue av, const Config& config) -> void {
  if (Core::can_modify_actor_value(this_, actor, value, av) &&
      allow_petrified_blood_effect(*this_, config)) {
    const auto effects_damage = Core::get_effects_by_keyword(
        *actor, *config.petrified_blood().mgef_accumulate_keyword_damage());

    value = std::abs(value);

    if (const auto blood_damage_tick = petrified_blood(*actor, value, config);
        blood_damage_tick > 0.f) {
      effects_damage[0]->magnitude += blood_damage_tick;
    }

    value = -value;
  }
}

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void {

  if (const auto blood_damage_tick = petrified_blood(*target, hit_data.totalDamage, config);
      blood_damage_tick > 0.f) {
    petrified_blood_cast(*target, blood_damage_tick, config);
  }
}
} // namespace Reflyem::PetrifiedBlood
