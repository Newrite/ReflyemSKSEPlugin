#pragma once
#include "Core.hpp"

namespace Reflyem::DeathLoot
{

struct DeathLootData
{

  static constexpr auto DISTANCE = 1500000.f;
  
  RE::Actor* victim{nullptr};
  RE::Actor* killer{nullptr};
  uint8_t frame_count{0};
  bool is_looted{false};

  [[nodiscard]] bool is_player_loot() const
  {

    if (victim) {
      auto& actors_cache = Core::ActorsCache::get_singleton();
      auto& actor_data = actors_cache.get_or_add(victim->formID).get();
      if (actor_data.hit_by_player_timer() > 0.f) {
        return true;
      }
    }

    if (!killer) {
      return false;
    }

    if (!(killer->IsPlayerRef() || killer->IsPlayerTeammate())) {
      return false;
    }
    
    return true;
  }

  [[nodiscard]] bool is_auto_loot_distance() const
  {
    
    if (!killer || !victim) {
      return false;
    }

    let killer_position = killer->GetPosition();
    let victim_position = victim->GetPosition();

    let squared_distance = killer_position.GetSquaredDistance(victim_position);

    logd("SD: {} D: {}", squared_distance, DISTANCE);

    if (squared_distance <= DISTANCE) {
      return true;
    }

    return false;
    
  }
  
};

static std::vector<DeathLootData> killed_actors{};
static std::vector<RE::FormID> location_bosses{};

auto is_tagged(const RE::InventoryEntryData* item) -> bool;

auto handle_killed_actors() -> void;

auto process_death(const RE::TESDeathEvent* event) -> void;
auto process_kill(const RE::ActorKill::Event* event) -> void;

} // namespace Reflyem::DeathLoot
