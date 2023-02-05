#include "plugin/TimingBlock.hpp"
#include "Core.hpp"

// TODO: Реализовать тайминг парри (с меньшим окном) и тайминг блок (с большим окном), в теории оба
// не расходуют ресурс на блок и полностью нивилируют урон, при опр. количестве тайминг парри
// стаггерят врага, через маг эффекты выдать возможность регулировать окно для тайминг парри и
// блока, выдавать иммун к тайминг парри или блоку, регулировать количество нужных парри для
// стаггера, проигрывать дополнительный vfx
namespace Reflyem::TimingBlock {

auto spawn_sparks(RE::Actor& target, const Config& config, const bool weapon_block) -> void {
  const auto block_node =
      Core::place_at_me(&target, config.timing_block().blank_activator(), 1, false, false);

  const auto node = weapon_block ? "WEAPON"sv : "SHIELD"sv;

  block_node->MoveToNode(&target, node);

  Core::place_at_me(block_node, config.timing_block().spark(), 1, false, false);
  Core::place_at_me(block_node, config.timing_block().spark_flare(), 1, false, false);

  block_node->SetDelete(true);
}

auto on_weapon_hit(RE::Actor& target, const RE::HitData& hit_data, const Config& config)
    -> void {

  if (target.IsDead()) {
    return;
  }

  if (hit_data.flags.any(RE::HitData::Flag::kBlocked)) {
    auto&       actors_cache = Core::ActorsCache::get_singleton();
    const auto& actor_data   = actors_cache.get_or_add(target.formID).get();
    const auto  diff         = 1.f - actor_data.timing_block_start_delta();
    logger::info("Diff: {} TimingBlockStartDelta: {} BlockFlag: {}", diff,
                 actor_data.timing_block_start_delta(), actor_data.blocking_flag());
    if (diff < config.timing_block().block_timing()) {

      const auto weapon_block = hit_data.flags.any(RE::HitData::Flag::kBlockWithWeapon);
      spawn_sparks(target, config, weapon_block);
      
      const auto ni_aggressor = hit_data.aggressor.get();
      if (ni_aggressor) {
        const auto aggressor = ni_aggressor.get();
        if (aggressor) {
          RE::DebugNotification("Larger stagger");
          logger::info("Larger stagger");
          aggressor->SetGraphVariableFloat("StaggerMagnitude", 0.2f);
          aggressor->NotifyAnimationGraph("staggerStart");
          // aggressor->NotifyAnimationGraph("recoillargestart");
        }
      }

      RE::DebugNotification("Timing block!");
      logger::info("Timing block!");
    } else {
      RE::DebugNotification("Not timing block!");
      logger::info("Not timing block!");
    }
  }
}
}