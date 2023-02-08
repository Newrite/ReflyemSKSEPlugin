#include "plugin/TimingBlock.hpp"
#include "Core.hpp"

namespace Reflyem::TimingBlock {

auto spawn_sparks(RE::Actor& target, const Config& config, const bool weapon_block) -> void {

  if (!config.timing_block().enable_sparks()) {
    return;
  }

  const auto block_node =
      Core::place_at_me(&target, config.timing_block().blank_activator(), 1, false, false);

  const auto node = weapon_block ? "WEAPON"sv : "SHIELD"sv;

  block_node->MoveToNode(&target, node);

  Core::place_at_me(block_node, config.timing_block().spark(), 1, false, false);
  Core::place_at_me(block_node, config.timing_block().spark_flare(), 1, false, false);

  block_node->SetDelete(true);
}

auto is_allow_timing_parry(RE::Actor& attacker, RE::Actor& target,
                           const Core::ActorsCache::Data& actor_data, const Config& config)
    -> bool {

  if (!Core::has_absolute_keyword(attacker, *config.timing_block().parry_keyword())) {
    return false;
  }

  if (Core::has_absolute_keyword(target, *config.timing_block().parry_immun_keyword())) {
    return false;
  }

  const auto timing_effects =
      Core::get_effects_by_keyword(attacker, *config.timing_block().parry_timing_keyword());
  const auto parry_timing = config.timing_block().parry_timing() +
                            Core::get_effects_magnitude_sum(timing_effects).value_or(0.f);

  const auto diff = 1.f - actor_data.timing_block_timer();
  if (diff > parry_timing) {
    return false;
  }

  return true;
}

auto is_allow_timing_block(RE::Actor& attacker, RE::Actor& target,
                           const Core::ActorsCache::Data& actor_data, const Config& config)
    -> bool {

  if (!Core::has_absolute_keyword(attacker, *config.timing_block().block_keyword())) {
    return false;
  }

  if (Core::has_absolute_keyword(target, *config.timing_block().block_immun_keyword())) {
    return false;
  }

  const auto timing_effects =
      Core::get_effects_by_keyword(attacker, *config.timing_block().block_timing_keyword());
  const auto parry_timing = config.timing_block().block_timing() +
                            Core::get_effects_magnitude_sum(timing_effects).value_or(0.f);

  const auto diff = 1.f - actor_data.timing_block_timer();
  if (diff > parry_timing) {
    return false;
  }

  return true;
}

auto parry_stagger_handler(RE::Actor& attacker, RE::Actor& target,
                           Core::ActorsCache::Data actor_data, const Config& config) -> void {

  const auto parry_count_effects =
      Core::get_effects_by_keyword(target, *config.timing_block().parry_stagger_count_keyword());
  const auto need_parry_count =
      config.timing_block().parry_stagger_count() +
      static_cast<int32_t>(Core::get_effects_magnitude_sum(parry_count_effects).value_or(0.f));

  if (actor_data.timing_parry_counter() >= need_parry_count) {
    actor_data.timing_parry_counter(0);
    logger::info("Parry stagger");
    attacker.SetGraphVariableFloat("StaggerMagnitude", 0.2f);
    attacker.NotifyAnimationGraph("staggerStart");
  } else {
    attacker.NotifyAnimationGraph("recoilLargeStart");
  }
}

auto on_weapon_hit(RE::Actor& target, RE::HitData& hit_data, const Config& config) -> void {

  if (target.IsDead()) {
    return;
  }

  if (!hit_data.flags.any(RE::HitData::Flag::kBlocked)) {
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
  auto& actor_data   = actors_cache.get_or_add(target.formID).get();

  if (is_allow_timing_parry(*aggressor, target, actor_data, config)) {
    spawn_sparks(target, config, hit_data.flags.any(RE::HitData::Flag::kBlockWithWeapon));
    actor_data.mod_timing_parry_counter(1);
    actor_data.timing_parry_counter_timer(config.timing_block().parry_stagger_count_timer());
    hit_data.totalDamage = 0.f;
    parry_stagger_handler(*aggressor, target, actor_data, config);
    return;
  }

  if (is_allow_timing_block(*aggressor, target, actor_data, config)) {
    spawn_sparks(target, config, hit_data.flags.any(RE::HitData::Flag::kBlockWithWeapon));
    hit_data.totalDamage = 0.f;
    aggressor->NotifyAnimationGraph("recoilLargeStart");
  }
}
}