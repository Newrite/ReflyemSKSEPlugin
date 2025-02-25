#include "SlowTime.hpp"
#include "Core.hpp"

namespace Reflyem::SlowTime {

auto start_slow_time(const Config& config) -> void
{
  logi("StartSlowTime: {} Power: {} Duration: {}"sv, config.slow_time().enable(), config.slow_time().slow_power(), config.slow_time().slow_duration());
  if (!config.slow_time().enable()) {
    return;
  }
  let player = RE::PlayerCharacter::GetSingleton();
  auto& actors_cache = Core::ActorsCache::get_singleton();
  auto& player_data = actors_cache.get_or_add(player->formID).get();
  player_data.slow_time_duration(config.slow_time().slow_duration());
  Core::set_global_time_multiplier(config.slow_time().slow_power(), true);
}

auto update_actor(const RE::Character& actor, const float delta, const Config& config) -> void
{
  auto& actors_cache = Core::ActorsCache::get_singleton();
  auto& player_data = actors_cache.get_or_add(actor.formID).get();
  if (player_data.slow_time_duration() <= 0.f) {
    return;
  }
  let slow_time_delta = delta / config.slow_time().slow_power();
  player_data.mod_slow_time_duration(-slow_time_delta);
  let slow_time_duration = player_data.slow_time_duration();
  // logi("SlowTime: Delta: {} SlowDelta: {} SlowTimeDuration: {}"sv, delta, slow_time_delta, slow_time_duration);
  if (slow_time_duration <= 0.f) {
    Core::set_global_time_multiplier(1.f, true);
  }
  
}

auto on_weapon_hit(const RE::Actor* target, const RE::HitData& hit_data, const Config& config) -> void
{
  const auto aggressor = hit_data.aggressor.get();

  if (!aggressor || !target) {
    return;
  }

  if (target->IsPlayerRef()) {
    let blocked_hit = hit_data.flags.any(RE::HitData::Flag::kBlocked) || hit_data.flags.any(RE::HitData::Flag::kBlockWithWeapon);
    if (blocked_hit && config.slow_time().enable_on_block()) {
      start_slow_time(config);
    }
  }

  if (aggressor->IsPlayerRef()) {
    let vanilla_crit = hit_data.flags.any(RE::HitData::Flag::kCritical) || hit_data.flags.any(RE::HitData::Flag::kCriticalOnDeath);
    if (vanilla_crit && config.slow_time().enable_on_weapon_vanilla_crit()) {
      start_slow_time(config);
    }

    if (config.slow_time().enable_on_sneak_attack() && hit_data.flags.any(RE::HitData::Flag::kSneakAttack)) {
      start_slow_time(config);
    }
    
  }
  
}

}
