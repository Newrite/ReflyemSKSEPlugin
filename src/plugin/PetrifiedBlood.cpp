#include "plugin/PetrifiedBlood.hpp"

namespace Reflyem::PetrifiedBlood
{

auto petrified_blood_cast(RE::Actor& target, const float blood_damage_tick, const Config& config)
    -> void
{
  logd("BloodCastTick: {}"sv, blood_damage_tick);
  config.petrified_blood().blood_spell()->effects[0]->effectItem.magnitude = blood_damage_tick;

  Core::cast(*config.petrified_blood().blood_spell(), target, target);
}

auto petrified_blood(
    RE::Actor& target,
    float& damage_value,
    const RE::ActorValue pb_av,
    const Config& config) -> float
{
  auto petrified_blood_percent = target.GetActorValue(pb_av);
  logd("petrified_blood_percent {}, start damage_value {}", petrified_blood_percent, damage_value);
  if (petrified_blood_percent <= 0.f) { return 0.f; }

  if (petrified_blood_percent > 100.f) { petrified_blood_percent = 100.f; }

  let blood_duration = config.petrified_blood().blood_spell()->effects[0]->effectItem.duration;

  let blood_damage = damage_value * (petrified_blood_percent / 100.f);
  let blood_damage_tick = blood_damage / static_cast<float>(blood_duration);

  damage_value -= blood_damage;
  logd(
      "blood_duration {} blood_damage {} blood_damage_tick {} after damage_value {}",
      blood_duration,
      blood_damage,
      blood_damage_tick,
      damage_value);

  return blood_damage_tick;
}

auto character_update(
    RE::Character& character,
    float,
    const Config& config,
    Core::ActorsCache::Data& actor_data) -> void
{
  const auto blood_damage_tick = actor_data.petrified_blood_accumulator();
  if (blood_damage_tick > 0.f)
    {
      actor_data.petrified_blood_accumulator(0.f);
      petrified_blood_cast(character, blood_damage_tick, config);
    }
}

auto allow_petrified_blood_effect(const RE::ActiveEffect& active_effect, const Config& config)
    -> bool
{
  if (!active_effect.effect || !active_effect.effect->baseEffect) { return false; }
  const auto base_effect = active_effect.effect->baseEffect;
  if (config.petrified_blood().must_be_or_not_be())
    {
    return Core::try_form_has_keyword(base_effect, config.petrified_blood().mgef_keyword());
    }
  return !Core::try_form_has_keyword(base_effect, config.petrified_blood().mgef_keyword());
}

auto modify_actor_value(
    const RE::ValueModifierEffect* this_,
    RE::Actor* actor,
    float& value,
    const RE::ActorValue av,
    const Config& config) -> void
{
  if (Core::can_modify_actor_value(this_, actor, value, av) &&
      allow_petrified_blood_effect(*this_, config))
    {
      value = std::abs(value);
      let blood_damage_tick =
          petrified_blood(*actor, value, config.petrified_blood().av_magick(), config);
      if (blood_damage_tick > 0.f)
        {
          auto& actors_cache = Core::ActorsCache::get_singleton();
          auto& actor_data = actors_cache.get_or_add(actor->formID).get();
          actor_data.mod_petrified_blood_accumulator(blood_damage_tick);
        }

      value = -value;
    }
}

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void
{
  const auto blood_damage_tick =
      petrified_blood(*target, hit_data.totalDamage, config.petrified_blood().av_physic(),
      config);
  logger::debug("Blood damage tick {}", blood_damage_tick);
  if (blood_damage_tick > 0.f) { petrified_blood_cast(*target, blood_damage_tick, config); }
}
} // namespace Reflyem::PetrifiedBlood
