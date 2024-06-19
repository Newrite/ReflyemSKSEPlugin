#include "plugin/ProjectileBlock.hpp"
#include "Core.hpp"
#include "SlowTime.hpp"
#include "plugin/TimingBlock.hpp"

namespace Reflyem::ProjectileBlock {

constexpr static auto EXP_PROJECTILE_TIMING_BLOCK = 10.f;
constexpr static auto EXP_PROJECTILE_BLOCK = 5.f;

auto cast(RE::SpellItem* spell, RE::TESObjectREFR* target, RE::Actor* caster) -> void
{
  auto caster_ = false ? spell->HasKeyword(nullptr) ? target : caster : caster;
  auto target_ = false ? spell->HasKeyword(nullptr) ? caster : target : target;
  if (!caster_ || !spell) {
    return;
  }

  if (spell->data.delivery == RE::MagicSystem::Delivery::kSelf) {
    if (caster_->formType == RE::FormType::ActorCharacter) {
      caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
          ->CastSpellImmediate(spell, true, caster_, 1.00f, false, 0.0f, caster_->As<RE::Actor>());
    }
  } else {
    if (!target_ && caster_->formType == RE::FormType::ActorCharacter) {
    }
    caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
        ->CastSpellImmediate(spell, true, target_, 1.00f, false, 0.0f, caster_->As<RE::Actor>());
  }
}

auto cast_on_handle(RE::TESForm* keyword, RE::TESForm* spell, RE::TESObjectREFR* target, RE::Actor* caster) -> void
{
  if (!spell) {
    return;
  }

  bool allow_cast = false;

  RE::BGSKeyword* keyword_ptr = nullptr;

  logger::debug("Cast_On_Handle before cast to keyword and spell");

  if (keyword) {
    keyword_ptr = keyword->As<RE::BGSKeyword>();
  }
  const auto spell_ptr = spell->As<RE::SpellItem>();

  if (!spell_ptr) {
    return;
  }

  if (!keyword_ptr) {
    allow_cast = true;
  }

  logger::debug("Cast_On_Handle before call has_absolute_keyword");
  if (!allow_cast && !Core::try_has_absolute_keyword(caster, keyword_ptr)) {
    return;
  }

  logger::debug("Cast_On_Handle before call cast");
  cast(spell_ptr, target, caster);
}

auto cast_on_handle_formlists(RE::BGSListForm* keywords,
                              RE::BGSListForm* spells,
                              RE::Actor* caster,
                              RE::TESObjectREFR* target) -> void
{
  if (!keywords || !spells) {
    return;
  }

  const auto length_kw = keywords->forms.size();
  const auto length_sp = spells->forms.size();
  logger::debug("LKW LSP: {} {}", length_kw, length_sp);
  for (std::uint32_t index = 0u; index < length_kw && index < length_sp; index++) {
    logger::debug("LKW LSP index: {}", index);
    cast_on_handle(keywords->forms[index], spells->forms[index], target, caster);
  }
}

auto cast_on_timing_block(RE::TESObjectREFR* target, RE::Actor* caster, const Config& config) -> void
{
  if (config.cast_on_projectile_timing_block().enable()) {
    cast_on_handle_formlists(config.cast_on_projectile_timing_block().formlist_needkw(),
                             config.cast_on_projectile_timing_block().formlist_spells(),
                             caster,
                             target);
  }
}

auto cast_on_block(RE::TESObjectREFR* target, RE::Actor* caster, const Config& config) -> void
{
  if (config.cast_on_projectile_block().enable()) {
    cast_on_handle_formlists(config.cast_on_projectile_block().formlist_needkw(),
                             config.cast_on_projectile_block().formlist_spells(),
                             caster,
                             target);
  }
}

auto get_heading_angle_re(RE::TESObjectREFR* a_object, RE::TESObjectREFR* a_target, bool a_unk_flag, double& a_result)
    -> bool
{
  using func_t = decltype(&get_heading_angle_re);
  REL::Relocation<func_t> func{RELOCATION_ID(21050, 21500)};
  return func(a_object, a_target, a_unk_flag, a_result);
}

auto get_heading_angle(RE::TESObjectREFR* self, RE::TESObjectREFR* a_target) -> double
{
  double angle = 0;
  get_heading_angle_re(self, a_target, false, angle);
  return angle;
}

typedef void(_fastcall* _destroyProjectile)(RE::Projectile* a_projectile);
inline static REL::Relocation<_destroyProjectile> destroyProjectile{RELOCATION_ID(42930, 44110)};

bool is_in_block_angle(RE::Actor* blocker, RE::TESObjectREFR* a_obj)
{
  const auto settings = RE::GameSettingCollection::GetSingleton();
  if (!settings || !a_obj || !blocker) {
    return false;
  }
  const auto fCombatHitConeAngle = settings->GetSetting("fCombatHitConeAngle")->GetFloat();
  let angle = get_heading_angle(blocker, a_obj);
  return (angle <= fCombatHitConeAngle && angle >= -fCombatHitConeAngle);
}

auto is_projectile_spell_ignore_block(const RE::Projectile* projectile, const Config& config) -> bool
{
  if (!projectile) {
    return false;
  }
  let spell = projectile->spell;
  if (!spell) {
    return false;
  }
  if (!config.projectile_block().exclusive_keyword()) {
    return false;
  }
  return spell->HasKeyword(config.projectile_block().exclusive_keyword());
}

auto is_projectile_ammo_ignore_block(const RE::Projectile* projectile, const Config& config) -> bool
{
  if (!projectile) {
    return false;
  }
  let ammo = projectile->ammoSource;
  if (!ammo) {
    return false;
  }
  if (!config.projectile_block().exclusive_keyword()) {
    return false;
  }
  return ammo->HasKeyword(config.projectile_block().exclusive_keyword());
}

auto is_projectile_spell_ignore_timing_block(const RE::Projectile* projectile, const Config& config) -> bool
{
  if (!projectile) {
    return false;
  }
  let spell = projectile->spell;
  if (!spell) {
    return false;
  }
  if (!config.projectile_block().exclusive_timing_keyword()) {
    return false;
  }
  return spell->HasKeyword(config.projectile_block().exclusive_timing_keyword());
}

auto is_projectile_ammo_ignore_timing_block(const RE::Projectile* projectile, const Config& config) -> bool
{
  if (!projectile) {
    return false;
  }
  let ammo = projectile->ammoSource;
  if (!ammo) {
    return false;
  }
  if (!config.projectile_block().exclusive_timing_keyword()) {
    return false;
  }
  return ammo->HasKeyword(config.projectile_block().exclusive_timing_keyword());
}

auto get_shooter_ref_from_projectile(const RE::Projectile* projectile) -> RE::TESObjectREFR*
{
  let shooter_ref = projectile->shooter;
  if (shooter_ref) {
    let ni_shooter = shooter_ref.get();
    if (ni_shooter) {
      let shooter_object = ni_shooter.get();
      if (shooter_object) {
        return shooter_object;
      }
    }
  }
  return nullptr;
}

auto get_shooter_from_projectile(const RE::Projectile* projectile) -> RE::Actor*
{
  let shooter_ref = get_shooter_ref_from_projectile(projectile);
  if (shooter_ref) {
    return shooter_ref->As<RE::Actor>();
  }
  return nullptr;
}

auto is_shooter_ignore_block(const RE::Projectile* projectile, const Config& config) -> bool
{
  let shooter = get_shooter_from_projectile(projectile);
  if (shooter && Core::try_has_absolute_keyword(shooter, config.projectile_block().exclusive_keyword())) {
    return true;
  }
  return false;
}

auto is_allow_timing_block(RE::Actor* blocker,
                           const RE::Projectile* projectile,
                           const ProjectileType projectile_type,
                           const Core::ActorsCache::Data& actor_data,
                           const Config& config) -> bool
{
  if (!config.projectile_block().enable_timing_block()) {
    return false;
  }

  if (!blocker || !projectile) {
    return false;
  }

  if (!Core::try_has_absolute_keyword(blocker, config.projectile_block().enable_timing_block_keyword())) {
    logger::debug("Blocker has no keyword for timing block projectile"sv);
    return false;
  }

  let shooter = get_shooter_from_projectile(projectile);
  if (Core::try_has_absolute_keyword(shooter, config.projectile_block().exclusive_timing_keyword())) {
    logger::debug("Shooter has exlusive keyword"sv);
    return false;
  }

  if (projectile_type != ProjectileType::kArrow &&
      (is_projectile_spell_ignore_timing_block(projectile, config) ||
       !Core::try_has_absolute_keyword(blocker, config.projectile_block().enable_magick_timing_block_keyword()))) {
    logi("Spell ignore timing block"sv);
    return false;
  }

  if (projectile_type == ProjectileType::kArrow &&
      (is_projectile_ammo_ignore_timing_block(projectile, config) ||
       !Core::try_has_absolute_keyword(blocker, config.projectile_block().enable_arrow_timing_block_keyword()))) {
    logi("Ammo ignore timing block"sv);
    return false;
  }

  let timing_effects =
      Core::try_get_effects_by_keyword(blocker, config.projectile_block().timing_block_timer_keyword());
  let block_timing =
      config.projectile_block().block_timing() + Core::get_effects_magnitude_sum(timing_effects).value_or(0.f);

  let diff = config.timing_block().block_delay() - actor_data.timing_block_timer();
  if (diff > block_timing) {
    logger::debug("Diff more than timing"sv);
    return false;
  }

  return true;
}

auto spawn_sparks(RE::Actor* blocker, const Config& config, const bool weapon_block, const bool parry) -> void
{
  if (!config.projectile_block().enable_sparks()) {
    return;
  }

  const auto block_node = TimingBlock::place_form_at_me(blocker, config.projectile_block().blank_activator());
  if (!block_node) {
    return;
  }

  const auto node = weapon_block ? "WEAPON"sv : "SHIELD"sv;

  block_node->MoveToNode(blocker, node);

  TimingBlock::place_form_at_me(block_node, config.projectile_block().spark());
  TimingBlock::place_form_at_me(block_node, config.projectile_block().spark_flare());
  if (parry) {
    TimingBlock::place_form_at_me(block_node, config.projectile_block().spark_halo());
  }
  block_node->SetDelete(true);
}

auto try_block_projectile(RE::Actor* blocker,
                          RE::Projectile* projectile,
                          const ProjectileType projectile_type,
                          const Config& config) -> bool
{
  if (!blocker || !projectile) {
    return false;
  }

  auto& actors_cache = Core::ActorsCache::get_singleton();
  auto& actor_data = actors_cache.get_or_add(blocker->formID).get();

  let right_hand = blocker->GetEquippedObject(false);
  auto is_shield = false;
  if (right_hand) {
    is_shield = right_hand->IsArmor();
  }

  if (is_allow_timing_block(blocker, projectile, projectile_type, actor_data, config)) {
    spawn_sparks(blocker, config, !is_shield, true);
    if (config.projectile_block().enable_sound()) {
      Core::play_sound(config.projectile_block().block_sound(), blocker);
    }
    if (projectile_type != ProjectileType::kFlame) {
      destroyProjectile(projectile);
    }
    if (blocker->IsPlayerRef()) {
      RE::PlayerCharacter::GetSingleton()->AddSkillExperience(RE::ActorValue::kBlock, EXP_PROJECTILE_TIMING_BLOCK);
    }
    blocker->NotifyAnimationGraph("BlockHitStart");
    if (blocker->IsPlayerRef() && config.slow_time().enable_on_projectile_timing_block()) {
      SlowTime::start_slow_time(config);
    }
    cast_on_timing_block(get_shooter_ref_from_projectile(projectile), blocker, config);
    return true;
  }

  if (!config.projectile_block().enable_block()) {
    return false;
  }

  if (!Core::try_has_absolute_keyword(blocker, config.projectile_block().enable_block_keyword())) {
    return false;
  }

  if (projectile_type == ProjectileType::kArrow &&
      (is_projectile_ammo_ignore_block(projectile, config) ||
       !Core::try_has_absolute_keyword(blocker, config.projectile_block().enable_arrow_block_keyword()))) {
    return false;
  }

  if (projectile_type != ProjectileType::kArrow &&
      (is_projectile_spell_ignore_block(projectile, config) ||
       !Core::try_has_absolute_keyword(blocker, config.projectile_block().enable_magick_block_keyword()))) {
    return false;
  }

  auto cost_av = RE::ActorValue::kStamina;
  if (projectile_type != ProjectileType::kArrow && config.projectile_block().enable_block_magick_for_magick()) {
    cost_av = RE::ActorValue::kMagicka;
  }

  let block_stamina_percent = config.projectile_block().base_percent_cost();
  let blocker_stamina = blocker->GetActorValue(cost_av);
  let max_stamina = Reflyem::Core::get_actor_value_max(blocker, cost_av);
  let percent_of_stamina = (max_stamina / 100.f) * block_stamina_percent;

  let cost_effects = Core::try_get_effects_by_keyword(blocker, config.projectile_block().block_cost_keyword());
  let base_block_cost = (percent_of_stamina + config.projectile_block().base_flat_cost()) *
                        Core::get_effects_magnitude_sum(cost_effects).value_or(1.f);

  auto block_av = blocker->GetActorValue(RE::ActorValue::kBlock);
  if (block_av > 200.f) {
    block_av = 200.f;
  }
  if (block_av < 0.f) {
    block_av = 1.f;
  }
  let percent_block_stamina_effect = 1.f - (block_av / 200.f);

  let block_cost = base_block_cost * percent_block_stamina_effect;
  if (blocker_stamina >= block_cost) {
    spawn_sparks(blocker, config, !is_shield, false);
    if (config.projectile_block().enable_sound()) {
      Core::play_sound(config.projectile_block().block_sound(), blocker);
    }
    if (projectile_type != ProjectileType::kFlame) {
      destroyProjectile(projectile);
    }
    if (blocker->IsPlayerRef()) {
      RE::PlayerCharacter::GetSingleton()->AddSkillExperience(RE::ActorValue::kBlock, EXP_PROJECTILE_BLOCK);
    }
    Core::damage_actor_value(*blocker, cost_av, block_cost);
    blocker->NotifyAnimationGraph("BlockHitStart");
    if (blocker->IsPlayerRef() && config.slow_time().enable_on_projectile_block()) {
      SlowTime::start_slow_time(config);
    }
    cast_on_block(get_shooter_ref_from_projectile(projectile), blocker, config);
    return true;
  }

  return false;
}

auto process_projectile_block(RE::Actor* blocker,
                              RE::Projectile* projectile,
                              const ProjectileType projectile_type,
                              const Config& config) -> bool
{
  if (!blocker || !projectile || is_shooter_ignore_block(projectile, config)) {
    return false;
  }
  if (!blocker->IsBlocking() || !is_in_block_angle(blocker, projectile)) {
    return false;
  }

  switch (projectile_type) {
    case ProjectileType::kArrow: {
      if (config.projectile_block().enable_arrow()) {
        return try_block_projectile(blocker, projectile, projectile_type, config);
      }
      break;
    }
    case ProjectileType::kMissile: {
      if (config.projectile_block().enable_missile()) {
        return try_block_projectile(blocker, projectile, projectile_type, config);
      }
      break;
    }
    case ProjectileType::kCone: {
      if (config.projectile_block().enable_cone()) {
        return try_block_projectile(blocker, projectile, projectile_type, config);
      }
      break;
    }
    case ProjectileType::kFlame: {
      if (config.projectile_block().enable_flame()) {
        return try_block_projectile(blocker, projectile, projectile_type, config);
      }
      break;
    }
    case ProjectileType::kBeam: {
      if (config.projectile_block().enable_beam()) {
        return try_block_projectile(blocker, projectile, projectile_type, config);
      }
      break;
    }
  }
  return false;
}

// from dTry's Valhalla Combat
// https://github.com/D7ry/valhallaCombat/blob/d27c89bcdf32d38a579c428c46e1dcbb0a790add/src/bin/Hooks.cpp#L110C22-L110C22
auto should_ignore_hit(RE::Projectile* projectile,
                       RE::hkpAllCdPointCollector* collidable_collector,
                       const ProjectileType projectile_type) -> bool
{
  let config = Config::get_singleton();

  if (collidable_collector && config.projectile_block().enable()) {
    for (auto& hit : collidable_collector->hits) {
      auto ref_a = RE::TESHavokUtilities::FindCollidableRef(*hit.rootCollidableA);
      auto ref_b = RE::TESHavokUtilities::FindCollidableRef(*hit.rootCollidableB);
      if (ref_a && ref_a->formType == RE::FormType::ActorCharacter) {
        logi("RefA process run"sv);
        return process_projectile_block(ref_a->As<RE::Actor>(), projectile, projectile_type, config);
      }
      if (ref_b && ref_b->formType == RE::FormType::ActorCharacter) {
        logi("RefB process run"sv);
        return process_projectile_block(ref_b->As<RE::Actor>(), projectile, projectile_type, config);
      }
    }
  }
  return false;
}

}
