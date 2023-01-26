#include "plugin/PetrifiedBlood.hpp"
#include "Core.hpp"

namespace Reflyem {
namespace PetrifiedBlood {

auto petrified_blood_cast(RE::Actor& target, const float blood_damage_tick, const Config& config) -> void {
  config.petrified_blood_spell->effects[0]->effectItem.magnitude = blood_damage_tick;

  Core::cast(*config.petrified_blood_spell, target, target);
}

auto petrified_blood(RE::Actor& target, float& damage_value, const Config& config) -> float {
  auto petrified_blood_percent = target.GetActorValue(config.petrified_blood_av);
  if (petrified_blood_percent <= 0.f) {
    return 0.f;
  }

  if (petrified_blood_percent > 100.f) {
    petrified_blood_percent = 100.f;
  }

  const auto blood_duration = config.petrified_blood_spell->effects[0]->effectItem.duration;

  const auto damage_mult       = Core::getting_damage_mult(target);
  const auto blood_damage      = (damage_value * damage_mult) * (petrified_blood_percent / 100.f);
  const auto blood_damage_tick = (blood_damage / blood_duration) / damage_mult;

  damage_value -= (blood_damage / damage_mult);

  return blood_damage_tick;
}

auto character_update(RE::Character& character, float, const Config& config) -> void {
  const auto effects_damage = Core ::get_effects_by_keyword(character, *config.petrified_blood_acc_mgef_kw);

  if (effects_damage.size() <= 0) {
    return;
  }

  const auto blood_damage_tick    = effects_damage.at(0)->magnitude;
  effects_damage.at(0)->magnitude = 0.f;
  petrified_blood_cast(character, blood_damage_tick, config);
}

auto modify_actor_value(const RE::ValueModifierEffect* this_, RE::Actor* actor, float& value, const RE::ActorValue av,
                        const Config& config) -> void {
  if (Core::can_modify_actor_value(this_, actor, value, av)) {
    const auto effects_damage = Core ::get_effects_by_keyword(*actor, *config.petrified_blood_acc_mgef_kw);

    value                        = std::abs(value);
    const auto blood_damage_tick = petrified_blood(*actor, value, config);

    if (blood_damage_tick > 0.f) {
      effects_damage[0]->magnitude += blood_damage_tick;
    }

    value = -value;
  }
}

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void {
  const auto blood_damage_tick = petrified_blood(*target, hit_data.totalDamage, config);

  if (blood_damage_tick > 0.f) {
    petrified_blood_cast(*target, blood_damage_tick, config);
  }
}
} // namespace PetrifiedBlood
} // namespace Reflyem
