#include "plugin/TimingBlock.hpp"

#include "MagicBlocker.hpp"
#include "SlowTime.hpp"

namespace Reflyem::TimingBlock {

constexpr static auto EXP_TIMING_BLOCK = 10.f;

auto place_form_at_me(RE::TESObjectREFR* node, RE::TESForm* form) -> RE::TESObjectREFR*
{
  if (!node || !form) {
    return nullptr;
  }

  return Core::place_at_me(node, form, 1, false, false);
}

auto cast_on_timing_block(RE::Actor& target, RE::Actor& caster, const Config& config, const bool is_magic_blocker)
    -> void
{
  if (config.cast_on_timing_block().enable()) {
    if (is_magic_blocker && config.cast_on_magic_blocker().enable()) {
      Core::cast_on_handle_formlists(config.cast_on_magic_blocker().form_list_timing_block_keyword(),
                                     config.cast_on_magic_blocker().form_list_timing_block_spells(),
                                     caster,
                                     target);
    }
    if (!is_magic_blocker) {
      Core::cast_on_handle_formlists(config.cast_on_timing_block().formlist_needkw(),
                                     config.cast_on_timing_block().formlist_spells(),
                                     caster,
                                     target);
    }
  }
}

auto cast_on_block_parry(RE::Actor& target, RE::Actor& caster, const Config& config, const bool is_magic_blocker)
    -> void
{
  if (config.cast_on_block_parry().enable()) {
    if (is_magic_blocker && config.cast_on_magic_blocker().enable()) {
      Core::cast_on_handle_formlists(config.cast_on_magic_blocker().form_list_parry_block_keyword(),
                                     config.cast_on_magic_blocker().form_list_parry_block_spells(),
                                     caster,
                                     target);
    }
    if (!is_magic_blocker) {
      Core::cast_on_handle_formlists(config.cast_on_block_parry().formlist_needkw(),
                                     config.cast_on_block_parry().formlist_spells(),
                                     caster,
                                     target);
    }
  }
}

auto cast_on_stagger(RE::Actor& target, RE::Actor& caster, const Config& config) -> void
{
  if (config.cast_on_timing_block_stagger().enable()) {
    Core::cast_on_handle_formlists(config.cast_on_timing_block_stagger().formlist_needkw(),
                                   config.cast_on_timing_block_stagger().formlist_spells(),
                                   caster,
                                   target);
  }
}

auto spawn_sparks(RE::Actor& target,
                  const Config& config,
                  const bool weapon_block,
                  const bool parry,
                  const bool is_magic_blocker) -> void
{
  if (!config.timing_block().enable_sparks()) {
    return;
  }

  const auto block_node = place_form_at_me(&target, config.timing_block().blank_activator());
  if (!block_node) {
    return;
  }

  auto node = weapon_block ? "WEAPON"sv : "SHIELD"sv;
  if (is_magic_blocker) {
    node = "SHIELD"sv;
  }

  block_node->MoveToNode(&target, node);
  if (is_magic_blocker) {
    place_form_at_me(block_node, config.magic_blocker().spark());
    place_form_at_me(block_node, config.magic_blocker().spark_flare());
    if (parry) {
      place_form_at_me(block_node, config.magic_blocker().spark_halo());
    }
  } else {
    place_form_at_me(block_node, config.timing_block().spark());
    place_form_at_me(block_node, config.timing_block().spark_flare());
    if (parry) {
      place_form_at_me(block_node, config.timing_block().spark_halo());
    }
  }
  block_node->SetDelete(true);
}

auto play_impacts(RE::Actor& target,
                  const Config& config,
                  const RE::HitData& hit_data,
                  const bool parry,
                  const bool is_magic_blocker) -> void
{

  spawn_sparks(target, config, hit_data.flags.any(RE::HitData::Flag::kBlockWithWeapon), parry, is_magic_blocker);
  if (parry) {
    if (is_magic_blocker) {
      Core::play_sound(config.magic_blocker().parry_sound(), &target);
    } else {
      Core::play_sound(config.timing_block().parry_sound(), &target);
    }
  } else {
    if (is_magic_blocker) {
      Core::play_sound(config.magic_blocker().timing_block_sound(), &target);
    } else {
      Core::play_sound(config.timing_block().block_sound(), &target);
    }
  }
}

auto is_allow_timing_parry(RE::Actor& attacker,
                           RE::Actor& target,
                           const Core::ActorsCache::Data& actor_data,
                           const Config& config) -> bool
{
  if (!Core::try_has_absolute_keyword(&target, config.timing_block().parry_keyword())) {
    return false;
  }

  if (Core::try_has_absolute_keyword(&attacker, config.timing_block().parry_immun_keyword())) {
    return false;
  }

  if (config.magic_blocker().enable() &&
      Core::try_has_absolute_keyword(&target, config.magic_blocker().magic_blocker_keyword()) &&
      !Core::try_has_absolute_keyword(&target, config.magic_blocker().block_parry_integration_keyword())) {
    logger::debug("Magic Blocker without block parry integration"sv);
    return false;
  }

  const auto timing_effects = Core::try_get_effects_by_keyword(&target, config.timing_block().parry_timing_keyword());
  const auto parry_timing =
      config.timing_block().parry_timing() + Core::get_effects_magnitude_sum(timing_effects).value_or(0.f);

  const auto diff = config.timing_block().block_delay() - actor_data.timing_block_timer();
  if (diff > parry_timing) {
    return false;
  }

  return true;
}

auto is_allow_timing_block(RE::Actor& attacker,
                           RE::Actor& target,
                           const Core::ActorsCache::Data& actor_data,
                           const Config& config) -> bool
{
  if (!Core::try_has_absolute_keyword(&target, config.timing_block().block_keyword())) {
    logger::debug("Target has no keyword"sv);
    return false;
  }

  if (Core::try_has_absolute_keyword(&attacker, config.timing_block().block_immun_keyword())) {
    logger::debug("Attacker has immun keyword"sv);
    return false;
  }

  if (config.magic_blocker().enable() &&
      Core::try_has_absolute_keyword(&target, config.magic_blocker().magic_blocker_keyword()) &&
      !Core::try_has_absolute_keyword(&target, config.magic_blocker().timing_block_integration_keyword())) {
    logger::debug("Magic Blocker without timing block integration"sv);
    return false;
  }

  const auto timing_effects = Core::try_get_effects_by_keyword(&target, config.timing_block().block_timing_keyword());
  const auto block_timing =
      config.timing_block().block_timing() + Core::get_effects_magnitude_sum(timing_effects).value_or(0.f);

  const auto diff = config.timing_block().block_delay() - actor_data.timing_block_timer();
  logger::debug("BlockTiming eval: {} BlockTiming config: {} Diff: {}"sv,
                block_timing,
                config.timing_block().block_timing(),
                diff);
  if (diff > block_timing) {
    logger::debug("Diff more than timing"sv);
    return false;
  }

  return true;
}

auto parry_stagger_handler(RE::Actor& attacker, RE::Actor& blocker, Core::ActorsCache::Data& actor_data, const Config& config) -> void
{
  const auto parry_count_effects =
      Core::try_get_effects_by_keyword(&attacker, config.timing_block().parry_stagger_count_keyword());
  const auto need_parry_count =
      config.timing_block().parry_stagger_count() +
      static_cast<int32_t>(Core::get_effects_magnitude_sum(parry_count_effects).value_or(0.f));

  logd("ParryCountDebug: Name:{}"sv, attacker.GetDisplayFullName());
  logd("ParryCountDebug: Effects:{} NeedCount:{} TimingCount:{}"sv,
       parry_count_effects.size(),
       need_parry_count,
       actor_data.timing_parry_counter());

  if (actor_data.timing_parry_counter() >= need_parry_count) {
    cast_on_stagger(attacker, blocker, config);
    actor_data.timing_parry_counter(0);
    logger::debug("Parry stagger");
    attacker.SetGraphVariableFloat("StaggerMagnitude", 5.f);
    attacker.NotifyAnimationGraph("staggerStart");
  }
}

auto apply_spells_from_attack(RE::Actor& attacker, RE::Actor& target, const RE::HitData& hit_data, const Config& config)
    -> bool
{
  if (target.IsDead()) {
    return false;
  }

  if (!hit_data.flags.any(RE::HitData::Flag::kBlocked)) {
    return false;
  }

  auto& actors_cache = Core::ActorsCache::get_singleton();
  const auto& actor_data = actors_cache.get_or_add(target.formID).get();

  if (is_allow_timing_parry(attacker, target, actor_data, config)) {
    return true;
  }

  if (is_allow_timing_block(attacker, target, actor_data, config)) {
    return true;
  }

  return false;
}

auto on_weapon_hit(RE::Actor& target, RE::HitData& hit_data, const Config& config) -> void
{
  if (target.IsDead()) {
    return;
  }

  if (!hit_data.flags.any(RE::HitData::Flag::kBlocked)) {
    return;
  }

  if (!config.parry_bash().enable_bash_block_with_timing_parry_block() && config.parry_bash().enable() &&
      config.parry_bash().enable_bash_block() && Core::is_bashing(&target)) {
    return;
  }

  const auto ni_aggressor = hit_data.aggressor.get();
  if (!ni_aggressor) {
    return;
  }

  const auto aggressor = ni_aggressor.get();
  if (!aggressor) {
    return;
  }

  auto& actors_cache = Core::ActorsCache::get_singleton();
  auto& actor_data = actors_cache.get_or_add(target.formID).get();

  logger::debug("Timing block weapon hit"sv);

  auto is_magic_blocker = false;

  if (is_allow_timing_parry(*aggressor, target, actor_data, config)) {

    logger::debug("Allow timing parry"sv);
    if (target.IsPlayerRef() && config.slow_time().enable_on_parry_timing_block()) {
      SlowTime::start_slow_time(config);
    }

    if (config.magic_blocker().enable() &&
        Core::try_has_absolute_keyword(&target, config.magic_blocker().magic_blocker_keyword()) &&
        Core::try_has_absolute_keyword(&target, config.magic_blocker().block_parry_integration_keyword())) {
      is_magic_blocker = true;
    }

    if (Core::ExternalApi::true_hud_api) {
      Core::ExternalApi::true_hud_api->FlashActorSpecialBar(SKSE::GetPluginHandle(), aggressor->GetHandle(), true);
    }
    play_impacts(target, config, hit_data, true, is_magic_blocker);
    // spawn_sparks(target, config, hit_data.flags.any(RE::HitData::Flag::kBlockWithWeapon), true);
    // Core::play_sound(config.timing_block().parry_sound(), &target);
    actor_data.mod_timing_parry_counter(1);
    actor_data.timing_parry_counter_timer(config.timing_block().parry_stagger_count_timer());
    hit_data.totalDamage = 0.f;
    parry_stagger_handler(*aggressor, target, actor_data, config);
    cast_on_block_parry(*aggressor, target, config, is_magic_blocker);
    if (target.IsPlayerRef()) {
      RE::PlayerCharacter::GetSingleton()->AddSkillExperience(
          is_magic_blocker ? MagicBlocker::get_ward_associated_skill(&target) : RE::ActorValue::kBlock,
          EXP_TIMING_BLOCK);
    }
    hit_data.attackDataSpell = nullptr;
    return;
  }

  if (is_allow_timing_block(*aggressor, target, actor_data, config)) {
    logger::debug("Allow timing block"sv);
    if (target.IsPlayerRef() && config.slow_time().enable_on_timing_block()) {
      SlowTime::start_slow_time(config);
    }

    if (config.magic_blocker().enable() &&
        Core::try_has_absolute_keyword(&target, config.magic_blocker().magic_blocker_keyword()) &&
        Core::try_has_absolute_keyword(&target, config.magic_blocker().timing_block_integration_keyword())) {
      is_magic_blocker = true;
    }

    if (Core::ExternalApi::true_hud_api) {
      Core::ExternalApi::true_hud_api->FlashActorSpecialBar(SKSE::GetPluginHandle(), aggressor->GetHandle(), true);
    }
    cast_on_timing_block(*aggressor, target, config, is_magic_blocker);
    play_impacts(target, config, hit_data, false, is_magic_blocker);
    // spawn_sparks(target, config, hit_data.flags.any(RE::HitData::Flag::kBlockWithWeapon), false);
    // Core::play_sound(config.timing_block().block_sound(), &target);
    if (target.IsPlayerRef()) {
      RE::PlayerCharacter::GetSingleton()->AddSkillExperience(
          is_magic_blocker ? MagicBlocker::get_ward_associated_skill(&target) : RE::ActorValue::kBlock,
          EXP_TIMING_BLOCK);
    }
    hit_data.attackDataSpell = nullptr;
    hit_data.totalDamage = 0.f;
  }
}
} // namespace Reflyem::TimingBlock
