#include "ComboSeries.hpp"

#include "Core.hpp"

namespace Reflyem::ComboSeries {

auto get_max_combo_series_timer(RE::Actor* actor) -> float
{

  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();

  let combo_series_timer_effects =
      Core::try_get_effects_by_keyword(actor, config.combo_series().keyword_combo_point_timer());
  let combo_series_timer = Core::get_effects_magnitude_sum(combo_series_timer_effects).value_or(0.f);

  return combo_series_timer;
}

auto get_current_combo_series_timer(const RE::Actor* actor) -> float
{

  if (!actor) {
    return 0.f;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);

  return actor_data.combo_series_timer();
}

auto set_current_combo_series_timer(RE::Actor* actor, float new_value) -> void
{

  if (!actor) {
    return;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);

  if (new_value < 0.f) {
    new_value = 0.f;
  }

  let max_timer = get_max_combo_series_timer(actor);
  if (new_value > max_timer) {
    new_value = max_timer;
  }

  actor_data.combo_series_timer(new_value);
}

auto get_max_combo_series_points(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();

  let combo_series_points_effects =
      Core::try_get_effects_by_keyword(actor, config.combo_series().keyword_combo_points());
  let combo_series_points = Core::get_effects_magnitude_sum(combo_series_points_effects).value_or(0.f);

  return combo_series_points;
}

auto get_current_combo_series_points(const RE::Actor* actor) -> float
{

  if (!actor) {
    return 0.f;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);

  return actor_data.combo_series_points();
}

auto set_current_combo_series_points(RE::Actor* actor, float new_value) -> void
{

  if (!actor) {
    return;
  }

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);

  if (new_value < 0.f) {
    new_value = 0.f;
  }

  actor_data.combo_series_points(new_value);
}

auto get_combo_points_for_magick(RE::Actor* caster, const RE::Projectile* projectile) -> float
{

  if (!caster || !projectile) {
    return 0.f;
  }

  if (!projectile->spell) {
    return 0.f;
  }

  letr config = Config::get_singleton();

  let is_dual = projectile->flags.any(RE::Projectile::Flags::kIsDual);

  let combo_series_points_effects =
      Core::try_get_effects_by_keyword(caster,
                                       is_dual ? config.combo_series().keyword_magic_projectile_dual_cast_point()
                                               : config.combo_series().keyword_magic_projectile_point());
  let combo_series_points = Core::get_effects_magnitude_sum(combo_series_points_effects).value_or(0.f);

  auto is_concentration_spell = false;
  if (projectile->spell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration) {
    is_concentration_spell = true;
  }

  auto points_mult = 1.f;
  if (is_concentration_spell && projectile->GetProjectileBase()) {
    points_mult = projectile->GetProjectileBase()->data.relaunchInterval;
  }

  if (points_mult > 1.f) {
    points_mult = 1.f;
  }

  return combo_series_points * points_mult;
}

auto get_combo_points_for_none_source(RE::Actor* actor) -> float
{

  if (!actor) {
    return 0.f;
  }

  letr config = Config::get_singleton();

  let combo_series_points_effects =
      Core::try_get_effects_by_keyword(actor, config.combo_series().keyword_no_source_point());
  let combo_series_points = Core::get_effects_magnitude_sum(combo_series_points_effects).value_or(0.f);

  return combo_series_points;
}

auto get_combo_series_for_weapon(RE::Actor* attacker, const RE::TESObjectWEAP* weapon) -> float
{
  if (!attacker || !weapon) {
    return 0.f;
  }

  letr config = Config::get_singleton();

  let list_weapons = config.combo_series().list_keywords_weapon();
  let list_points = config.combo_series().list_keywords_point();

  if (!list_points || !list_weapons) {
    return 0.f;
  }

  let weapons_size = list_weapons->forms.size();
  let points_size = list_points->forms.size();

  for (std::uint32_t index = 0u; index < weapons_size && index < points_size; index++) {
    let weapon_form = list_weapons->forms[index];
    let point_form = list_points->forms[index];
    if (!weapon_form || !point_form) {
      continue;
    }

    let weapon_keyword = weapon_form->As<RE::BGSKeyword>();
    let point_keyword = point_form->As<RE::BGSKeyword>();

    if (!weapon_keyword || !point_keyword) {
      continue;
    }

    if (Core::try_form_has_keyword(weapon, weapon_keyword)) {
      let combo_series_points_effects = Core::try_get_effects_by_keyword(attacker, point_keyword);
      let combo_series_points = Core::get_effects_magnitude_sum(combo_series_points_effects).value_or(0.f);

      return combo_series_points;
    }
  }

  return 0.f;
}

auto get_combo_series_point_from_hit(const RE::TESObjectWEAP* weapon,
                                     const RE::Projectile* projectile,
                                     RE::Actor* attacker) -> float
{
  if (!attacker) {
    return 0.f;
  }

  if (weapon) {
    return get_combo_series_for_weapon(attacker, weapon);
  }

  if (projectile && !projectile->weaponSource && projectile->spell) {
    let shooter_ref = Core::get_shooter_ref_from_projectile(projectile);
    let caster = shooter_ref ? shooter_ref->As<RE::Actor>() : nullptr;

    return get_combo_points_for_magick(caster, projectile);
  }

  return get_combo_points_for_none_source(attacker);
}

auto update(RE::Actor* actor, const float) -> void
{
  if (!actor) {
    return;
  }

  let current_timer = get_current_combo_series_timer(actor);
  if (current_timer <= 0.f) {
    set_current_combo_series_points(actor, 0.f);
  }
}

auto hit_handle(RE::Actor* attacker, RE::Actor* target, const RE::TESObjectWEAP* weapon, const RE::Projectile* projectile) -> void
{
  let combo_point = get_combo_series_point_from_hit(weapon, projectile, attacker);
  if (attacker && combo_point > 0.f) {

    letr config = Config::get_singleton();

    let current_combo_points = get_current_combo_series_points(attacker) + combo_point;

    let max_points = get_max_combo_series_points(attacker);
    if (current_combo_points >= max_points) {
      Core::cast_on_handle_formlists(
          config.combo_series().list_cast_keywords(), config.combo_series().list_cast_spells(), attacker, target);
      set_current_combo_series_points(attacker, 0.f);
      set_current_combo_series_timer(attacker, 0.f);
      return;
    }
    set_current_combo_series_timer(attacker, get_max_combo_series_timer(attacker));
    set_current_combo_series_points(attacker, current_combo_points);
  }
}

auto on_magic_hit(const RE::Projectile* projectile, RE::TESObjectREFR* refr_target) -> void
{

  if (!refr_target) {
    return;
  }
  
  let target = refr_target->As<RE::Actor>();
  if (target && target->IsDead()) {
    return;
  }

  let shooter_ref = Core::get_shooter_ref_from_projectile(projectile);

  let attacker = shooter_ref ? shooter_ref->As<RE::Actor>() : nullptr;

  hit_handle(attacker, target, nullptr, projectile);
}

auto on_weapon_hit(const RE::HitData& hit_data, RE::Actor* target) -> void
{

  if (target && target->IsDead()) {
    return;
  }

  let attacker = hit_data.aggressor ? hit_data.aggressor.get() ? hit_data.aggressor.get().get() : nullptr : nullptr;

  hit_handle(attacker, target, hit_data.weapon, nullptr);
}

}
