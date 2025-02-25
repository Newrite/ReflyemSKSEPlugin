#include "plugin/ProjectileBlock.hpp"
#include "Core.hpp"
#include "JoshuaSawyerArmorSystem.hpp"
#include "MagicBlocker.hpp"
#include "ResourceManager.hpp"
#include "SlowTime.hpp"
#include "SpecialTechniques.hpp"
#include "StaggerSystem.hpp"
#include "plugin/TimingBlock.hpp"

#include <numbers>

namespace Reflyem::ProjectileBlock {

constexpr static auto EXP_PROJECTILE_TIMING_BLOCK = 10.f;
constexpr static auto EXP_PROJECTILE_BLOCK = 5.f;
constexpr static float SOUND_COOLDOWN_SET = 1.25f;
constexpr static float BLOCK_ANIMATION_COOLDOWN_SET = 1.25f;
constexpr static float CONCENTRATION_HANDLE_POINT = 0.35f;

static float sound_cooldown = 0.f;
static float block_animation_cooldown = 0.f;

auto cast_on_timing_block(RE::TESObjectREFR* target,
                          RE::Actor* caster,
                          const Config& config,
                          const bool has_weapon_source,
                          const bool is_magic_blocker) -> void
{
  if (!config.cast_on_projectile_timing_block().enable()) {
    return;
  }

  if (has_weapon_source && config.cast_on_projectile_timing_block().physical()) {
    if (is_magic_blocker && config.cast_on_magic_blocker().enable()) {
      Core::cast_on_handle_formlists(
          config.cast_on_magic_blocker().form_list_projectile_timing_block_keyword_physical(),
          config.cast_on_magic_blocker().form_list_projectile_timing_block_spells_physical(),
          caster,
          target);
    }
    if (!is_magic_blocker) {
      Core::cast_on_handle_formlists(config.cast_on_projectile_timing_block().weapon_formlist_needkw(),
                                     config.cast_on_projectile_timing_block().weapon_formlist_spells(),
                                     caster,
                                     target);
    }
  }

  if (!has_weapon_source && config.cast_on_projectile_timing_block().magick()) {
    if (is_magic_blocker && config.cast_on_magic_blocker().enable()) {
      Core::cast_on_handle_formlists(config.cast_on_magic_blocker().form_list_projectile_timing_block_keyword_magick(),
                                     config.cast_on_magic_blocker().form_list_projectile_timing_block_spells_magick(),
                                     caster,
                                     target);
    }
    if (!is_magic_blocker) {
      Core::cast_on_handle_formlists(config.cast_on_projectile_timing_block().magick_formlist_needkw(),
                                     config.cast_on_projectile_timing_block().magick_formlist_spells(),
                                     caster,
                                     target);
    }
  }
}

auto cast_on_block(RE::TESObjectREFR* target,
                   RE::Actor* caster,
                   const Config& config,
                   const bool has_weapon_source,
                   const bool is_magic_blocker) -> void
{

  if (!config.cast_on_projectile_block().enable()) {
    return;
  }

  if (has_weapon_source && config.cast_on_projectile_block().physical()) {
    if (is_magic_blocker && config.cast_on_magic_blocker().enable()) {
      Core::cast_on_handle_formlists(config.cast_on_magic_blocker().form_list_projectile_block_keyword_physical(),
                                     config.cast_on_magic_blocker().form_list_projectile_block_spells_physical(),
                                     caster,
                                     target);
    }
    if (!is_magic_blocker) {
      Core::cast_on_handle_formlists(config.cast_on_projectile_block().weapon_formlist_needkw(),
                                     config.cast_on_projectile_block().weapon_formlist_spells(),
                                     caster,
                                     target);
    }
  }

  if (!has_weapon_source && config.cast_on_projectile_block().magick()) {
    if (is_magic_blocker && config.cast_on_magic_blocker().enable()) {
      Core::cast_on_handle_formlists(config.cast_on_magic_blocker().form_list_projectile_block_keyword_magick(),
                                     config.cast_on_magic_blocker().form_list_projectile_block_spells_magick(),
                                     caster,
                                     target);
    }
    if (!is_magic_blocker) {
      Core::cast_on_handle_formlists(config.cast_on_projectile_block().magick_formlist_needkw(),
                                     config.cast_on_projectile_block().magick_formlist_spells(),
                                     caster,
                                     target);
    }
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
  auto fCombatHitConeAngle = settings->GetSetting("fCombatHitConeAngle")->GetFloat();
  letr config = Config::get_singleton();
  if (config.special_techniques().enable() &&
      Core::try_has_absolute_keyword(blocker, config.special_techniques().enable_360_block())) {
    fCombatHitConeAngle = 180.f;
  }
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
                           const Core::ActorsCache::Data& actor_data,
                           const Config& config) -> bool
{
  if (!config.projectile_block().enable_timing_block()) {
    return false;
  }

  if (!blocker || !projectile) {
    return false;
  }

  if (!config.parry_bash().enable_bash_block_with_timing_parry_block() && config.parry_bash().enable() &&
      config.parry_bash().enable_bash_block() && Core::is_bashing(blocker)) {
    return false;
  }

  if (config.magic_blocker().enable() &&
      Core::try_has_absolute_keyword(blocker, config.magic_blocker().magic_blocker_keyword()) &&
      !Core::try_has_absolute_keyword(blocker, config.magic_blocker().projectile_timing_block_integration_keyword())) {
    logger::debug("Magic Blocker without projectile timing block integration"sv);
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

  if (!projectile->weaponSource &&
      (is_projectile_spell_ignore_timing_block(projectile, config) ||
       !Core::try_has_absolute_keyword(blocker, config.projectile_block().enable_magick_timing_block_keyword()))) {
    logi("Spell ignore timing block"sv);
    return false;
  }

  if (projectile->weaponSource &&
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

auto spawn_sparks(RE::Actor* blocker,
                  const Config& config,
                  const bool arrow_block,
                  const bool weapon_block,
                  const bool parry,
                  const bool is_magic_blocker) -> void
{
  if (!config.projectile_block().enable_sparks()) {
    return;
  }

  const auto block_node = TimingBlock::place_form_at_me(blocker, config.projectile_block().blank_activator());
  if (!block_node) {
    return;
  }

  auto node = weapon_block ? "WEAPON"sv : "SHIELD"sv;
  if (is_magic_blocker) {
    node = "SHIELD"sv;
  }

  block_node->MoveToNode(blocker, node);

  if (is_magic_blocker) {
    TimingBlock::place_form_at_me(block_node, config.magic_blocker().spark());
    TimingBlock::place_form_at_me(block_node, config.magic_blocker().spark_flare());
    if (parry) {
      TimingBlock::place_form_at_me(block_node, config.magic_blocker().spark_halo());
    }
  } else {
    if (arrow_block) {
      TimingBlock::place_form_at_me(block_node, config.projectile_block().arrow_spark());
      TimingBlock::place_form_at_me(block_node, config.projectile_block().arrow_spark_flare());
      if (parry) {
        TimingBlock::place_form_at_me(block_node, config.projectile_block().arrow_spark_halo());
      }
    } else {
      TimingBlock::place_form_at_me(block_node, config.projectile_block().spark());
      TimingBlock::place_form_at_me(block_node, config.projectile_block().spark_flare());
      if (parry) {
        TimingBlock::place_form_at_me(block_node, config.projectile_block().spark_halo());
      }
    }
  }
  block_node->SetDelete(true);
}

auto get_drain_values(RE::Actor* target, const float in_total_damage, RE::ActorValue cost_av, const Config& config)
    -> std::shared_ptr<ResourceManager::ResourceDrain>
{
  logd("on_weapon_hit resource manager"sv);

  if (in_total_damage <= 0.f) {
    return nullptr;
  }

  logd("on_weapon_hit blocked hit"sv);

  let weapon_or_shield = ResourceManager::get_weapon_or_shield(*target);
  let form = [&]() -> RE::TESForm* {
    if (config.magic_blocker().enable() &&
        Core::try_has_absolute_keyword(target, config.magic_blocker().magic_blocker_keyword())) {
      return config.magic_blocker().resource_manager_keyword_holder();
    }
    if (weapon_or_shield.has_value()) {
      if (weapon_or_shield.value().is_l) {
        return weapon_or_shield.value().left;
      }
      return weapon_or_shield.value().right;
    }
    return nullptr;
  }();
  if (!form) {
    return nullptr;
  }

  logd("success get form"sv);

  const auto keyword_form = form->As<RE::BGSKeywordForm>();
  if (!keyword_form) {
    return nullptr;
  }

  logd("success get keyword form"sv);

  const auto f_mask =
      ResourceManager::get_form_mask(*keyword_form, config, cost_av == RE::ActorValue::kMagicka ? 2 : 0);

  logd("mask values: {} {} {}"sv, f_mask->at(0).at(0), f_mask->at(0).at(1), f_mask->at(0).at(2));

  auto damage_mult = Core::getting_damage_mult(*target);
  auto total_damage = in_total_damage * damage_mult;

  auto mask_sum = ResourceManager::calc_mask_sum(*f_mask);

  return ResourceManager::handle_mask_sum_for_drain_values(mask_sum, total_damage);
}

auto is_enough_resource_for_block(RE::Actor& actor, const ResourceManager::ResourceDrain& drain_values) -> bool
{

  auto magicka = actor.GetActorValue(RE::ActorValue::kMagicka);
  auto health = actor.GetActorValue(RE::ActorValue::kHealth);
  auto stamina = actor.GetActorValue(RE::ActorValue::kStamina);

  auto d_magicka = std::abs(drain_values.magicka);
  auto d_health = std::abs(drain_values.health);
  auto d_stamina = std::abs(drain_values.stamina);

  logger::debug("Check allow block, magicka: {} health: {} stamina: {} dv m h s: {} {} {}"sv,
                magicka,
                health,
                stamina,
                d_magicka,
                d_health,
                d_stamina);

  if (d_magicka > 0.f && d_magicka > magicka) {

    Core::flash_hud_meter(RE::ActorValue::kMagicka);
    return false;
  }
  if (d_health > 0.f && d_health > health) {

    Core::flash_hud_meter(RE::ActorValue::kHealth);
    return false;
  }
  if (d_stamina > 0.f && d_stamina > stamina) {

    Core::flash_hud_meter(RE::ActorValue::kStamina);
    return false;
  }

  logger::debug("Allow block"sv);

  return true;
}

auto play_impacts(RE::Actor* blocker,
                  const Config& config,
                  const bool arrow_block,
                  const bool weapon_block,
                  const bool parry,
                  const bool is_magic_blocker,
                  const bool is_concentration_handle) -> void
{

  spawn_sparks(blocker, config, arrow_block, weapon_block, parry, is_magic_blocker);

  if (is_magic_blocker) {
    Core::play_sound(config.magic_blocker().block_sound(), blocker);
  }

  if (config.projectile_block().enable_sound() && (sound_cooldown <= 0.f || !is_concentration_handle)) {
    if (is_magic_blocker) {
      if (parry) {
        Core::play_sound(config.magic_blocker().parry_sound(), blocker);
      } else {
        Core::play_sound(config.magic_blocker().timing_block_sound(), blocker);
      }
    } else {
      if (arrow_block) {
        Core::play_sound(config.projectile_block().arrow_block_sound(), blocker);
      } else {
        Core::play_sound(config.projectile_block().block_sound(), blocker);
      }
    }
  }

  if (!is_magic_blocker && (block_animation_cooldown <= 0.f || !is_concentration_handle)) {
    blocker->NotifyAnimationGraph("BlockHitStart");
  }

  if (!is_magic_blocker && block_animation_cooldown <= 0.f && is_concentration_handle) {
    block_animation_cooldown = BLOCK_ANIMATION_COOLDOWN_SET;
  }
  if (sound_cooldown <= 0.f && is_concentration_handle) {
    sound_cooldown = SOUND_COOLDOWN_SET;
  }
}

auto try_block_projectile(RE::Actor* blocker,
                          RE::Projectile* projectile,
                          const ProjectileType projectile_type,
                          const Config& config) -> bool
{

  if (!blocker || !projectile) {
    return false;
  }

  let has_weapon_source = projectile->weaponSource ? true : false;

  auto& actors_cache = Core::ActorsCache::get_singleton();
  auto& actor_data = actors_cache.get_or_add(blocker->formID).get();

  auto is_concentration_spell = false;
  if (!projectile->weaponSource && projectile->spell) {
    if (projectile->spell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration) {
      is_concentration_spell = true;
    }
  }

  auto cost_mult = -1.f;
  if (is_concentration_spell && projectile->GetProjectileBase()) {
    cost_mult = projectile->GetProjectileBase()->data.relaunchInterval;
  }

  if (cost_mult > 1.f) {
    cost_mult = 1.f;
  }

  auto is_concentration_handle = false;
  if (cost_mult > 0.f && cost_mult <= CONCENTRATION_HANDLE_POINT) {
    is_concentration_handle = true;
  }

  let right_hand = blocker->GetEquippedObject(false);
  auto is_shield = false;
  if (right_hand) {
    is_shield = right_hand->IsArmor();
  }

  auto is_magic_blocker = false;

  auto shooter = get_shooter_from_projectile(projectile);
  auto shooter_ref = get_shooter_ref_from_projectile(projectile);
  if (is_allow_timing_block(blocker, projectile, actor_data, config)) {

    if (config.magic_blocker().enable() &&
        Core::try_has_absolute_keyword(blocker, config.magic_blocker().magic_blocker_keyword()) &&
        Core::try_has_absolute_keyword(blocker, config.magic_blocker().projectile_timing_block_integration_keyword())) {
      is_magic_blocker = true;
    }

    play_impacts(blocker, config, has_weapon_source, !is_shield, true, is_magic_blocker, is_concentration_handle);

    if (projectile_type != ProjectileType::kBeam) {
      destroyProjectile(projectile);
    }

    if (blocker->IsPlayerRef()) {
      RE::PlayerCharacter::GetSingleton()->AddSkillExperience(
          is_magic_blocker ? MagicBlocker::get_ward_associated_skill(blocker) : RE::ActorValue::kBlock,
          EXP_PROJECTILE_TIMING_BLOCK);
    }

    if (blocker->IsPlayerRef() && config.slow_time().enable_on_projectile_timing_block()) {
      SlowTime::start_slow_time(config);
    }

    cast_on_block(get_shooter_ref_from_projectile(projectile), blocker, config, has_weapon_source, is_magic_blocker);
    cast_on_timing_block(
        get_shooter_ref_from_projectile(projectile), blocker, config, has_weapon_source, is_magic_blocker);
    if (config.special_techniques().enable()) {
      SpecialTechniques::rotate_blocker(blocker, shooter_ref, config);
      if (config.special_techniques().slow_after_block_hit_mult() != 1.f) {
        actor_data.slow_after_block_hit_flag(true);
        actor_data.duration_after_block_hit(config.special_techniques().slow_after_block_hit_duration());
      }
    }
    return true;
  }

  if (!config.projectile_block().enable_block()) {
    return false;
  }

  if (config.magic_blocker().enable() &&
      Core::try_has_absolute_keyword(blocker, config.magic_blocker().magic_blocker_keyword()) &&
      !Core::try_has_absolute_keyword(blocker, config.magic_blocker().projectile_block_integration_keyword())) {
    logger::debug("Magic Blocker without projectile block integration"sv);
    return false;
  }

  if (config.magic_blocker().enable() &&
      Core::try_has_absolute_keyword(blocker, config.magic_blocker().magic_blocker_keyword()) &&
      Core::try_has_absolute_keyword(blocker, config.magic_blocker().projectile_block_integration_keyword())) {
    is_magic_blocker = true;
  }

  if (!Core::try_has_absolute_keyword(blocker, config.projectile_block().enable_block_keyword())) {
    return false;
  }

  if (projectile->weaponSource &&
      (is_projectile_ammo_ignore_block(projectile, config) ||
       !Core::try_has_absolute_keyword(blocker, config.projectile_block().enable_arrow_block_keyword()))) {
    return false;
  }

  if (!projectile->weaponSource &&
      (is_projectile_spell_ignore_block(projectile, config) ||
       !Core::try_has_absolute_keyword(blocker, config.projectile_block().enable_magick_block_keyword()))) {
    return false;
  }

  auto cost_av = RE::ActorValue::kStamina;
  if (!projectile->weaponSource && config.projectile_block().enable_block_magick_for_magick()) {
    cost_av = RE::ActorValue::kMagicka;
  }
  if (is_magic_blocker) {
    cost_av = RE::ActorValue::kMagicka;
  }

  auto block_cost = 0.f;
  let blocker_stamina = blocker->GetActorValue(cost_av);
  auto block_av = blocker->GetActorValue(RE::ActorValue::kBlock);
  if (is_magic_blocker) {
    block_av = blocker->GetActorValue(MagicBlocker::get_ward_associated_skill(blocker));
  }
  let block_shield_av = blocker->GetActorValue(RE::ActorValue::kShieldPerks);

  std::shared_ptr<ResourceManager::ResourceDrain> drain_values = nullptr;

  let fall_back_cost_calc = [&]() -> void {
    let block_stamina_percent = config.projectile_block().base_percent_cost();
    let max_stamina = Core::get_actor_value_max(blocker, cost_av);
    let percent_of_stamina = (max_stamina / 100.f) * block_stamina_percent;

    let cost_effects = Core::try_get_effects_by_keyword(blocker, config.projectile_block().block_cost_keyword());
    let base_block_cost = (percent_of_stamina + config.projectile_block().base_flat_cost()) *
                          Core::get_effects_magnitude_sum(cost_effects).value_or(1.f);

    if (block_av > 200.f) {
      block_av = 200.f;
    }
    if (block_av < 0.f) {
      block_av = 1.f;
    }
    let percent_block_stamina_effect = 1.f - (block_av / 200.f);

    block_cost = base_block_cost * percent_block_stamina_effect;
    if (blocker->IsPlayerRef() || (shooter && shooter->IsPlayerRef())) {
      Core::console_log("PROJ_BLOCK: FallBaskOrBaseCalc");
    }
  };

  if (!config.projectile_block().enable_correct_cost_calculate()) {
    fall_back_cost_calc();
  } else {

    bool is_calculated = false;
    let shield = Core::get_equipped_shield(blocker);
    auto damage_or_shield_armor = 0.f;

    if (projectile->weaponSource && shooter) {
      RE::HitData hit_data{};
      Core::initialization_hit_proj_data(hit_data, shooter, blocker, projectile);

      damage_or_shield_armor =
          shield ? shield->GetArmorRating() : static_cast<float>(projectile->weaponSource->GetAttackDamage());
      auto block_with_weapon = shield ? false : true;

      if (is_magic_blocker) {
        damage_or_shield_armor = MagicBlocker::get_ward_magnitude(blocker);
        block_with_weapon = false;
      }

      auto blocked_percent = Core::calculate_block_cost(
          damage_or_shield_armor, block_av, block_shield_av, block_with_weapon, false, false);

      if (blocker->IsPlayerRef() || (shooter && shooter->IsPlayerRef())) {
        Core::console_log(std::format("PROJ_BLOCK: Shooter TotalDamage: {} DamageOrArmor: {} IsWeaponBlock: {}",
                                      hit_data.totalDamage,
                                      damage_or_shield_armor,
                                      block_with_weapon));
      }

      if (blocker->IsPlayerRef() || (shooter && shooter->IsPlayerRef())) {
        Core::console_log(std::format("PROJ_BLOCK: Percent block: {}", blocked_percent));
      }

      if (is_magic_blocker) {
        // blocked_percent = blocked_percent * (1.f + (block_av / 100.f) + ((damage_or_shield_armor * 2.f) / 100.f));
        // blocked_percent = blocked_percent * (1.f + ((block_av * 0.7f) / 100.f) + ((damage_or_shield_armor) / 100.f));
        blocked_percent = blocked_percent * (1.f + (damage_or_shield_armor / 100.f));
      } else {
        RE::BGSEntryPoint::HandleEntryPoint(
            RE::BGSEntryPoint::ENTRY_POINT::kModPercentBlocked, blocker, std::addressof(blocked_percent));
      }

      if (blocker->IsPlayerRef() || (shooter && shooter->IsPlayerRef())) {
        Core::console_log(std::format("PROJ_BLOCK: Returned percent block: {}", blocked_percent));
      }

      if (blocked_percent > 0.9f) {
        blocked_percent = 0.9f;
      }

      if (blocked_percent > 0.0f) {
        if (config.resource_manager().enable() && config.resource_manager().block_spend_enable()) {
          auto damage = hit_data.totalDamage * (1.0f - blocked_percent);
          drain_values = get_drain_values(blocker, damage, cost_av, config);
        } else {
          block_cost = hit_data.totalDamage * (1.0f - blocked_percent) * Core::getting_damage_mult(*blocker);
        }
      }

      is_calculated = true;
    }

    if (projectile->weaponSource && !shooter) {
      RE::HitData hit_data{};
      Core::initialization_hit_proj_data(hit_data, blocker, blocker, projectile);

      damage_or_shield_armor =
          shield ? shield->GetArmorRating() : static_cast<float>(projectile->weaponSource->GetAttackDamage());
      auto block_with_weapon = shield ? false : true;

      if (is_magic_blocker) {
        damage_or_shield_armor = MagicBlocker::get_ward_magnitude(blocker);
        block_with_weapon = false;
      }

      auto blocked_percent = Core::calculate_block_cost(
          damage_or_shield_armor, block_av, block_shield_av, block_with_weapon, false, false);

      if (blocker->IsPlayerRef() || (shooter && shooter->IsPlayerRef())) {
        Core::console_log(std::format("PROJ_BLOCK: No shooter TotalDamage: {} DamageOrArmor: {} IsWeaponBlock: {}",
                                      hit_data.totalDamage,
                                      damage_or_shield_armor,
                                      block_with_weapon));
      }

      if (blocker->IsPlayerRef() || (shooter && shooter->IsPlayerRef())) {
        Core::console_log(std::format("PROJ_BLOCK: No shooter Percent block: {}", blocked_percent));
      }

      if (is_magic_blocker) {
        // blocked_percent = blocked_percent * (1.f + (block_av / 100.f) + ((damage_or_shield_armor * 2.f) / 100.f));
        // blocked_percent = blocked_percent * (1.f + ((block_av * 0.7f) / 100.f) + ((damage_or_shield_armor) / 100.f));
        blocked_percent = blocked_percent * (1.f + (damage_or_shield_armor / 100.f));
      } else {
        RE::BGSEntryPoint::HandleEntryPoint(
            RE::BGSEntryPoint::ENTRY_POINT::kModPercentBlocked, blocker, std::addressof(blocked_percent));
      }

      if (blocker->IsPlayerRef() || (shooter && shooter->IsPlayerRef())) {
        Core::console_log(std::format("PROJ_BLOCK: No shooter Returned percent block: {}", blocked_percent));
      }

      if (blocked_percent > 0.9f) {
        blocked_percent = 0.9f;
      }
      if (blocked_percent > 0.0f) {
        if (config.resource_manager().enable() && config.resource_manager().block_spend_enable()) {
          auto damage = hit_data.totalDamage * (1.0f - blocked_percent);
          drain_values = get_drain_values(blocker, damage, cost_av, config);
        } else {
          block_cost = hit_data.totalDamage * (1.0f - blocked_percent) * Core::getting_damage_mult(*blocker);
        }
      }

      is_calculated = true;
    }

    if (!projectile->weaponSource && projectile->spell) {

      damage_or_shield_armor = config.projectile_block().spell_damage_novice();
      auto spell_cost = 0.f;
      let is_cost_override = (projectile->spell->GetData1()->flags & 1) != 0;
      if (is_cost_override) {
        spell_cost = static_cast<float>(projectile->spell->GetData1()->costOverride) *
                     config.projectile_block().spell_cost_magicka_mult();
      }

      if (blocker->IsPlayerRef() || (shooter && shooter->IsPlayerRef())) {
        Core::console_log(
            std::format("PROJ_BLOCK: StartSpellCost: {} IsCostOverride: {}", spell_cost, is_cost_override));
      }

      for (auto effect : projectile->spell->effects) {
        if (effect && effect->baseEffect) {

          if (config.projectile_block().spell_cost_keyword() &&
              effect->baseEffect->HasKeyword(config.projectile_block().spell_cost_keyword())) {
            if (effect->baseEffect->data.flags.any(RE::EffectSetting::EffectSettingData::Flag::kDetrimental)) {
              spell_cost = spell_cost - effect->GetMagnitude();
            } else {
              spell_cost = spell_cost + effect->GetMagnitude();
            }
          }

          if (!is_cost_override && effect->cost > 0.f) {
            spell_cost = spell_cost + (effect->cost * config.projectile_block().spell_cost_magicka_mult());
          }

          let minimum_skill = effect->baseEffect->data.minimumSkill;
          if (minimum_skill >= 100) {
            if (damage_or_shield_armor < config.projectile_block().spell_damage_master()) {
              damage_or_shield_armor = config.projectile_block().spell_damage_master();
            }
          }

          if (minimum_skill >= 75) {
            if (damage_or_shield_armor < config.projectile_block().spell_damage_expert()) {
              damage_or_shield_armor = config.projectile_block().spell_damage_expert();
            }
          }

          if (minimum_skill >= 50) {
            if (damage_or_shield_armor < config.projectile_block().spell_damage_adept()) {
              damage_or_shield_armor = config.projectile_block().spell_damage_adept();
            }
          }

          if (minimum_skill >= 25) {
            if (damage_or_shield_armor < config.projectile_block().spell_damage_apprentice()) {
              damage_or_shield_armor = config.projectile_block().spell_damage_apprentice();
            }
          }
        }
      }

      let is_dual_cast = projectile->flags.any(RE::Projectile::Flags::kIsDual);
      if (config.projectile_block().enable_add_spell_damage_cost()) {
        spell_cost = spell_cost + damage_or_shield_armor;
      }
      spell_cost = spell_cost + config.projectile_block().spell_cost_base();

      RE::BGSEntryPoint::HandleEntryPoint(RE::BGSEntryPoint::ENTRY_POINT::kModIncomingSpellMagnitude,
                                          blocker,
                                          projectile->spell,
                                          std::addressof(spell_cost));

      if (shooter) {
        RE::BGSEntryPoint::HandleEntryPoint(RE::BGSEntryPoint::ENTRY_POINT::kModSpellMagnitude,
                                            shooter,
                                            projectile->spell,
                                            blocker,
                                            std::addressof(spell_cost));
      }

      damage_or_shield_armor = shield ? shield->GetArmorRating() : damage_or_shield_armor;
      auto block_with_weapon = shield ? false : true;

      if (is_magic_blocker) {
        damage_or_shield_armor = MagicBlocker::get_ward_magnitude(blocker);
        block_with_weapon = false;
      }

      auto blocked_percent = Core::calculate_block_cost(
          damage_or_shield_armor, block_av, block_shield_av, block_with_weapon, false, is_dual_cast);

      if (blocker->IsPlayerRef() || (shooter && shooter->IsPlayerRef())) {
        Core::console_log(
            std::format("PROJ_BLOCK: FinalSpellCost: {} IsWeaponBlock: {} DamageOrArmor: {} IsDualCast: {}",
                        spell_cost,
                        block_with_weapon,
                        damage_or_shield_armor,
                        is_dual_cast));
      }

      if (blocker->IsPlayerRef() || (shooter && shooter->IsPlayerRef())) {
        Core::console_log(std::format("PROJ_BLOCK: Magicka Percent block: {}", blocked_percent));
      }

      if (is_magic_blocker) {
        // blocked_percent = blocked_percent * (1.f + (block_av / 100.f) + ((damage_or_shield_armor * 2.f) / 100.f));
        // blocked_percent = blocked_percent * (1.f + ((block_av * 0.7f) / 100.f) + ((damage_or_shield_armor) / 100.f));
        blocked_percent = blocked_percent * (1.f + (damage_or_shield_armor / 100.f));
      } else {
        RE::BGSEntryPoint::HandleEntryPoint(
            RE::BGSEntryPoint::ENTRY_POINT::kModPercentBlocked, blocker, std::addressof(blocked_percent));
      }

      if (blocker->IsPlayerRef() || (shooter && shooter->IsPlayerRef())) {
        Core::console_log(std::format("PROJ_BLOCK: Magicka return Percent block: {}", blocked_percent));
      }

      if (blocked_percent > 0.9f) {
        blocked_percent = 0.9f;
      }
      if (blocked_percent > 0.0f) {
        if (config.resource_manager().enable() && config.resource_manager().block_spend_enable()) {
          auto damage = spell_cost * (1.0f - blocked_percent);
          drain_values = get_drain_values(blocker, damage, cost_av, config);
        } else {
          block_cost = spell_cost * (1.0f - blocked_percent) * Core::getting_damage_mult(*blocker);
        }
      }

      is_calculated = true;
    }

    if (!is_calculated) {
      fall_back_cost_calc();
    }
  }

  if (blocker->IsPlayerRef() || (shooter && shooter->IsPlayerRef())) {
    Core::console_log(std::format("PROJ_BLOCK: BlockerStamina: {} BlockCost: {} BlockAv: {} BlockShieldAv: {}",
                                  blocker_stamina,
                                  block_cost,
                                  block_av,
                                  block_shield_av));
  }

  if (drain_values) {
    block_cost = 10000.f;
  }

  if (blocker_stamina >= block_cost || (drain_values && is_enough_resource_for_block(*blocker, *drain_values))) {

    play_impacts(blocker, config, has_weapon_source, !is_shield, false, is_magic_blocker, is_concentration_handle);

    if (projectile_type != ProjectileType::kBeam) {
      destroyProjectile(projectile);
    }

    if (blocker->IsPlayerRef()) {
      RE::PlayerCharacter::GetSingleton()->AddSkillExperience(
          is_magic_blocker ? MagicBlocker::get_ward_associated_skill(blocker) : RE::ActorValue::kBlock,
          EXP_PROJECTILE_BLOCK);
    }

    if (is_concentration_handle) {
      block_cost = block_cost * cost_mult;
    }

    if (drain_values && is_concentration_handle) {
      drain_values->health = drain_values->health * cost_mult;
      drain_values->stamina = drain_values->stamina * cost_mult;
      drain_values->magicka = drain_values->magicka * cost_mult;
    }

    if (drain_values) {
      drain_values->drain(*blocker, true);
    } else {
      Core::damage_actor_value(*blocker, cost_av, block_cost);
    }

    if (blocker->IsPlayerRef() && config.slow_time().enable_on_projectile_block()) {
      SlowTime::start_slow_time(config);
    }

    cast_on_block(get_shooter_ref_from_projectile(projectile), blocker, config, has_weapon_source, is_magic_blocker);
    if (config.special_techniques().enable()) {
      SpecialTechniques::rotate_blocker(blocker, shooter_ref, config);
      if (config.special_techniques().slow_after_block_hit_mult() != 1.f) {
        actor_data.slow_after_block_hit_flag(true);
        actor_data.duration_after_block_hit(config.special_techniques().slow_after_block_hit_duration());
      }
    }
    return true;
  }

  return false;
}

auto process_projectile_block(RE::Actor* blocker,
                              RE::Projectile* projectile,
                              const ProjectileType projectile_type,
                              const Config& config) -> bool
{
  auto result = false;

  if (!blocker || !projectile || is_shooter_ignore_block(projectile, config)) {
    return result;
  }
  if (!Core::is_blocking(blocker) || !is_in_block_angle(blocker, projectile)) {
    logd("Proj not blocked, blocker on in block or not angle, proj: {}", projectile->GetDisplayFullName());
    return result;
  }

  switch (projectile_type) {
    case ProjectileType::kArrow: {
      if (config.projectile_block().enable_arrow()) {
        result = try_block_projectile(blocker, projectile, projectile_type, config);
      }
      break;
    }
    case ProjectileType::kMissile: {
      if (config.projectile_block().enable_missile()) {
        result = try_block_projectile(blocker, projectile, projectile_type, config);
      }
      break;
    }
    case ProjectileType::kCone: {
      if (config.projectile_block().enable_cone()) {
        result = try_block_projectile(blocker, projectile, projectile_type, config);
      }
      break;
    }
    case ProjectileType::kFlame: {
      if (config.projectile_block().enable_flame()) {
        result = try_block_projectile(blocker, projectile, projectile_type, config);
      }
      break;
    }
    case ProjectileType::kBeam: {
      if (config.projectile_block().enable_beam()) {
        result = try_block_projectile(blocker, projectile, projectile_type, config);
      }
      break;
    }
  }

  if (result && config.stagger_system().enable()) {
    StaggerSystem::on_magic_hit(projectile, blocker);
  }
  
  return result;
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
        logd("RefA process run, proj: {}"sv, projectile->GetDisplayFullName());
        return process_projectile_block(ref_a->As<RE::Actor>(), projectile, projectile_type, config);
      }
      if (ref_b && ref_b->formType == RE::FormType::ActorCharacter) {
        logd("RefB process run, proj: {}"sv, projectile->GetDisplayFullName());
        return process_projectile_block(ref_b->As<RE::Actor>(), projectile, projectile_type, config);
      }
    }
  }
  return false;
}

auto update(const float delta) -> void
{
  if (sound_cooldown > 0.f) {
    sound_cooldown = sound_cooldown - delta;
  }
  if (block_animation_cooldown > 0.f) {
    block_animation_cooldown = block_animation_cooldown - delta;
  }
}

auto on_magic_hit(RE::MagicCaster* magic_caster,
                  RE::NiPoint3* ni_point3,
                  RE::Projectile* projectile,
                  RE::TESObjectREFR* refr_target,
                  float,
                  float,
                  bool,
                  bool) -> bool
{

  if (!magic_caster || !projectile || !ni_point3 || !refr_target) {
    logd("OnMagicHit: some null, false");
    return false;
  }

  let beam = projectile->As<RE::BeamProjectile>();
  let flame = projectile->As<RE::FlameProjectile>();
  if (!beam && !flame) {
    logd("OnMagicHit: not beam and not flame, false");
    return false;
  }

  let shooter = get_shooter_ref_from_projectile(projectile);
  if (shooter && shooter->GetFormID() == refr_target->GetFormID()) {
    logd("OnMagicHit: target is caster");
    return false;
  }

  let blocker = refr_target->As<RE::Actor>();
  if (!blocker) {
    logd("Blocker not actor");
    return false;
  }

  letr config = Config::get_singleton();

  if (beam && config.projectile_block().enable_beam()) {
    logd("Start process beam");
    return process_projectile_block(blocker, projectile, ProjectileType::kBeam, config);
  }

  if (flame && config.projectile_block().enable_flame()) {
    logd("Start process flame");
    return process_projectile_block(blocker, projectile, ProjectileType::kFlame, config);
  }

  logd("No block magic hit");
  return false;
}

}
