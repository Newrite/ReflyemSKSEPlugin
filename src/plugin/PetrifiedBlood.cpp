#include "PetrifiedBlood.hpp"
#include "Core.hpp"

namespace Reflyem {
namespace PetrifiedBlood {

auto petrified_blood_cast(RE::Actor& target, float blood_damage_tick, const Reflyem::Config& config) -> void {
  config.petrified_blood_spell->effects[0]->effectItem.magnitude = blood_damage_tick;

  Reflyem::Core::cast(*config.petrified_blood_spell, target, target);
}

auto petrified_blood(RE::Actor& target, float& damage_value, const Reflyem::Config& config) -> float {
  auto petrified_blood_percent = target.GetActorValue(config.petrified_blood_av);
  if (petrified_blood_percent <= 0.f) {
    return 0.f;
  }

  if (petrified_blood_percent > 100.f) {
    petrified_blood_percent = 100.f;
  }

  auto blood_duration = config.petrified_blood_spell->effects[0]->effectItem.duration;

  auto damage_mult       = Reflyem::Core::getting_damage_mult(target);
  auto blood_damage      = (damage_value * damage_mult) * (petrified_blood_percent / 100.f);
  auto blood_damage_tick = (blood_damage / blood_duration) / damage_mult;

  damage_value -= (blood_damage / damage_mult);

  return blood_damage_tick;
}

auto character_update(RE::Character& character, float, const Reflyem::Config& config) -> void {
  auto effects_damage = Reflyem ::Core ::get_effects_by_keyword(character, *config.petrified_blood_acc_mgef_kw);

  if (effects_damage.size() <= 0) {
    return;
  }

  auto blood_damage_tick          = effects_damage.at(0)->magnitude;
  effects_damage.at(0)->magnitude = 0.f;
  petrified_blood_cast(character, blood_damage_tick, config);
}

auto modify_actor_value(RE::ValueModifierEffect* a_this, RE::Actor* a_actor, float& a_value, RE::ActorValue av,
                        const Reflyem::Config& config) -> void {
  if (Reflyem::Core::can_modify_actor_value(a_this, a_actor, a_value, av)) {
    auto effects_damage = Reflyem ::Core ::get_effects_by_keyword(*a_actor, *config.petrified_blood_acc_mgef_kw);

    a_value                = std::abs(a_value);
    auto blood_damage_tick = petrified_blood(*a_actor, a_value, config);

    if (blood_damage_tick > 0.f) {
      effects_damage[0]->magnitude += blood_damage_tick;
    }

    a_value = -a_value;
  }
}

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Reflyem::Config& config) -> void {
  auto blood_damage_tick = petrified_blood(*target, hit_data.totalDamage, config);

  if (blood_damage_tick > 0.f) {
    petrified_blood_cast(*target, blood_damage_tick, config);
  }
}
} // namespace PetrifiedBlood
} // namespace Reflyem
