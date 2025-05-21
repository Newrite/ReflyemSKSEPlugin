#include "CritRevised.hpp"
#include "Core.hpp"
#include "SlowTime.hpp"

namespace Reflyem::CritRevised {

enum class CritSourceType
{
  kWeapon,
  kMagick,
};

auto set_is_critical_frame(const RE::Actor* actor, const bool is_critical) -> void
{
  if (!actor) {
    return;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);
  actor_data.is_critical_frame(is_critical);
}

auto set_is_casted_on_critical(const RE::Actor* actor, const bool is_casted) -> void
{
  if (!actor) {
    return;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);
  actor_data.is_critical_casted_frame(is_casted);
}

auto get_is_critical_frame(const RE::Actor* actor) -> bool
{
  if (!actor) {
    return false;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);
  return actor_data.is_critical_frame();
}

auto get_is_casted_on_critical(const RE::Actor* actor) -> bool
{
  if (!actor) {
    return false;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);
  return actor_data.is_critical_casted_frame();
}

auto get_critical_chance(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();

  auto crit_chance = 0.f;
  if (config.crit_revised().enable_chance_av()) {
    crit_chance = actor->GetActorValue(config.crit_revised().chance_av());
  }
  let effects = Core::try_get_effects_by_keyword(actor, config.crit_revised().keyword_chance());
  let sum_effects = Core::get_effects_magnitude_sum(effects).value_or(0.f);
  crit_chance = crit_chance + sum_effects;

  return crit_chance;
}

auto get_critical_damage(RE::Actor* actor) -> float
{
  if (!actor) {
    return 1.f;
  }

  letr config = Config::get_singleton();

  auto crit_damage = 0.f;
  if (config.crit_revised().enable_damage_av()) {
    crit_damage = actor->GetActorValue(config.crit_revised().damage_av());
  }
  let effects = Core::try_get_effects_by_keyword(actor, config.crit_revised().keyword_damage());
  let sum_effects = Core::get_effects_magnitude_sum(effects).value_or(0.f);
  crit_damage = crit_damage + sum_effects;

  if (crit_damage <= 0.f) {
    return 1.f;
  }

  let damage_mult = 1.f + (crit_damage / 100.f);

  return damage_mult;
}

auto calculate_critical_chance(RE::Actor* actor) -> bool
{
  if (!actor) {
    return false;
  }

  let critical_chance = get_critical_chance(actor);
  let random = Core::get_random_float();

  return critical_chance >= random;
}

auto casted_on_critical(RE::Actor* actor, RE::TESObjectREFR* target) -> void
{
  if (!get_is_casted_on_critical(actor)) {
    return;
  }

  letr config = Config::get_singleton();

  Core::cast_on_handle_formlists(
      config.crit_revised().cast_keywords(), config.crit_revised().cast_spells(), actor, target);

  set_is_casted_on_critical(actor, true);
}

auto play_impact(RE::Actor* actor, const CritSourceType source_type) -> void
{

  if (!actor) {
    return;
  }

  if (actor->IsPlayerRef() == false) {
    return;
  }

  letr config = Config::get_singleton();
  
  if (source_type == CritSourceType::kMagick && config.crit_revised().magick_crit_sound()) {
    Core::play_sound(config.crit_revised().magick_crit_sound(), actor);
    if (config.slow_time().enable() && config.slow_time().enable_on_magick_crit()) {
      SlowTime::start_slow_time(config);
    }
  }

  if (source_type == CritSourceType::kWeapon && config.crit_revised().weapon_crit_sound()) {
    Core::play_sound(config.crit_revised().weapon_crit_sound(), actor);
    if (config.slow_time().enable() && config.slow_time().enable_on_weapon_crit()) {
      SlowTime::start_slow_time(config);
    }
  }
}

auto update(RE::Actor* actor, const float) -> void
{
  if (!actor) {
    return;
  }

  let is_critical_frame = calculate_critical_chance(actor);
  set_is_critical_frame(actor, is_critical_frame);
  set_is_casted_on_critical(actor, false);
}

auto on_magic_hit(const RE::Projectile* projectile, RE::TESObjectREFR* refr_target) -> void
{

  if (!refr_target || !projectile) {
    return;
  }

  let target = refr_target->As<RE::Actor>();

  if (!target) {
    return;
  }

  if (target->IsDead()) {
    return;
  }

  letr config = Config::get_singleton();

  if (Core::try_has_absolute_keyword(target, config.crit_revised().keyword_immun())) {
    return;
  }

  let shooter_ref = Core::get_shooter_ref_from_projectile(projectile);
  let caster = shooter_ref ? shooter_ref->As<RE::Actor>() : nullptr;

  if (!caster) {
    return;
  }

  if (get_is_critical_frame(caster)) {
    casted_on_critical(caster, target);
    play_impact(caster, CritSourceType::kMagick);
  }
}

auto on_weapon_hit(RE::HitData& hit_data, RE::Actor* target) -> void
{

  let attacker = hit_data.aggressor ? hit_data.aggressor.get() ? hit_data.aggressor.get().get() : nullptr : nullptr;

  if (!attacker || !target) {
    return;
  }

  if (target->IsDead()) {
    return;
  }

  letr config = Config::get_singleton();

  if (Core::try_has_absolute_keyword(target, config.crit_revised().keyword_immun())) {
    return;
  }

  if (get_is_critical_frame(attacker)) {
    casted_on_critical(attacker, target);
    if (hit_data.weapon && hit_data.weapon->criticalData.effect) {
      Core::cast_on_handle(nullptr, hit_data.weapon->criticalData.effect, *target, *attacker);
    }
    hit_data.totalDamage = hit_data.totalDamage * get_critical_damage(attacker);
    play_impact(attacker, CritSourceType::kWeapon);
  }
}

auto modify_actor_value(const RE::ValueModifierEffect* this_,
                        RE::Actor* actor,
                        float& value,
                        const RE::ActorValue av,
                        const Config& config) -> void
{

  if (Core::try_has_absolute_keyword(actor, config.crit_revised().keyword_immun())) {
    return;
  }

  if (Core::can_modify_actor_value(this_, actor, value, av)) {

    value = std::abs(value);
    let caster_ptr = this_->GetCasterActor();
    if (caster_ptr && caster_ptr.get() && get_is_critical_frame(caster_ptr.get())) {
      value = value * get_critical_damage(caster_ptr.get());
    }
    value = -value;
  }
}

}
