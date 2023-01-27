#pragma once

namespace Reflyem {
namespace Core {

template <typename L, typename R> struct Either {
  bool is_l;
  union {
    L left;
    R right;
  };

  explicit Either(const L& l) {
    is_l = true;
    left = l;
  }

  explicit Either(const R& r) {
    is_l  = false;
    right = r;
  }
};

struct ActorsCache {
  struct Data {
    enum class TickValues {
      k50Ms   = 0,
      k100Ms  = 1,
      k1000Ms = 2,
    };

  private:
    float regen_health_delay_;
    float regen_stamina_delay_;
    float regen_magicka_delay_;

    float delta_update_;

    uint64_t last_update_tick50_;
    uint64_t last_update_tick100_;
    uint64_t last_update_tick1000_;

  public:
    explicit Data()
        : regen_health_delay_(0.f), regen_stamina_delay_(0.f), regen_magicka_delay_(0.f), delta_update_(0.f),
          last_update_tick50_(GetTickCount64()), last_update_tick100_(GetTickCount64()),
          last_update_tick1000_(GetTickCount64()) {}

    [[nodiscard]] auto delta_update() const -> float { return delta_update_; }

    auto set_delta_update(const float delta) -> void { delta_update_ = delta; }

    [[nodiscard]] auto regen_health_delay() const -> float { return regen_health_delay_; }
    [[nodiscard]] auto regen_stamina_delay() const -> float { return regen_stamina_delay_; }
    [[nodiscard]] auto regen_magicka_delay() const -> float { return regen_magicka_delay_; }

    [[nodiscard]] auto last_update_tick(const TickValues tick) const -> uint64_t {
      if (tick == TickValues::k50Ms) {
        return last_update_tick50_;
      }
      if (tick == TickValues::k100Ms) {
        return last_update_tick100_;
      }
      return last_update_tick1000_;
    }

    [[nodiscard]] auto delta_tick(const TickValues tick) const -> uint64_t {
      if (tick == TickValues::k50Ms) {
        return GetTickCount64() - last_update_tick50_;
      }
      if (tick == TickValues::k100Ms) {
        return GetTickCount64() - last_update_tick100_;
      }
      return GetTickCount64() - last_update_tick1000_;
    }

    auto update_tick(const TickValues tick) -> void {
      if (tick == TickValues::k50Ms) {
        last_update_tick50_ = GetTickCount64();
        return;
      }
      if (tick == TickValues::k100Ms) {
        last_update_tick100_ = GetTickCount64();
        return;
      }
      last_update_tick1000_ = GetTickCount64();
    }

    auto set_regen_health_delay(const float delay) -> void {
      regen_health_delay_ = delay;
      if (regen_health_delay_ < 0.f) {
        regen_health_delay_ = 0.f;
      }
    }

    auto set_regen_stamina_delay(const float delay) -> void {
      regen_stamina_delay_ = delay;
      if (regen_stamina_delay_ < 0.f) {
        regen_stamina_delay_ = 0.f;
      }
    }

    auto set_regen_magicka_delay(const float delay) -> void {
      regen_magicka_delay_ = delay;
      if (regen_magicka_delay_ < 0.f) {
        regen_magicka_delay_ = 0.f;
      }
    }

    auto mod_health_delay(const float delta) -> float {
      regen_health_delay_ += delta;
      if (regen_health_delay_ < 0.f) {
        regen_health_delay_ = 0.f;
      }
      return regen_health_delay_;
    }

    auto mod_stamina_delay(const float delta) -> float {
      regen_stamina_delay_ += delta;
      if (regen_stamina_delay_ < 0.f) {
        regen_stamina_delay_ = 0.f;
      }
      return regen_stamina_delay_;
    }

    auto mod_magicka_delay(const float delta) -> float {
      regen_magicka_delay_ += delta;
      if (regen_magicka_delay_ < 0.f) {
        regen_magicka_delay_ = 0.f;
      }
      return regen_magicka_delay_;
    }

    auto mod_all_regens_delay(const float delta) -> void {
      mod_magicka_delay(delta);
      mod_health_delay(delta);
      mod_stamina_delay(delta);
    }
  };

  using FormsMap = std::unordered_map<RE::FormID, Data>;

private:
  FormsMap                  cache_map_;
  static constexpr uint64_t garbage_time = 5000;

  [[nodiscard]] static auto is_garbage(const Data& data) -> bool {
    return data.delta_tick(Data::TickValues::k1000Ms) >= garbage_time;
  }

  auto garbage_collector() -> void {
    for (const auto& [form_id, data] : cache_map_) {
      if (is_garbage(data)) {
        cache_map_.erase(form_id);
      }
    }
  }

  explicit ActorsCache() : cache_map_(FormsMap{}) {}

public:
  auto at_try(const RE::FormID key) -> std::optional<std::reference_wrapper<Data>> {

    const auto it = cache_map_.find(key);

    if (it == cache_map_.end()) {
      return std::nullopt;
    }

    return it->second;
  }

  auto at(const RE::FormID key) -> std::reference_wrapper<Data> { return cache_map_.at(key); }

  [[nodiscard]] auto exist(const RE::FormID key) const -> bool { return cache_map_.contains(key); }

  auto get_or_add(const RE::FormID key) -> std::reference_wrapper<Data> {
    if (this->exist(key)) {
      return at(key);
    }
    garbage_collector();
    cache_map_[key] = Data();
    return at(key);
  }

  static auto get_singleton() noexcept -> ActorsCache& {
    static auto cache = ActorsCache();

    return cache;
  }
};

auto character_timer_map_handler(ULONGLONG now_time, std::map<std::uintptr_t, ULONGLONG>& character_timer_map) -> void;

auto get_random_int() -> int;

auto damage_actor_value(RE::Actor& actor, RE::ActorValue av, float value) -> void;

auto set_av_regen_delay(RE::AIProcess* process, RE::ActorValue av, float time) -> void;

auto can_modify_actor_value(const RE::ValueModifierEffect* a_this, const RE::Actor* a_actor, float a_value,
                            RE::ActorValue av) -> bool;

auto actor_has_active_mgef_with_keyword(RE::Actor& actor, const RE::BGSKeyword& keyword) -> bool;

auto get_effects_magnitude_sum(const std::vector<RE::ActiveEffect*>& effects) -> std::optional<float>;

auto get_effects_by_keyword(RE::Actor& actor, const RE::BGSKeyword& keyword) -> std::vector<RE::ActiveEffect*>;

auto get_dual_value_mult(const RE::ValueModifierEffect& value_effect) -> float;

auto get_second_av(const RE::ValueModifierEffect& value_effect) -> RE::ActorValue;

auto getting_damage_mult(RE::Actor& actor) -> float;

auto cast(RE::SpellItem& spell, RE::Actor& target, RE::Actor& caster) -> void;

auto cast_on_handle(RE::TESForm* keyword, RE::TESForm* spell, RE::Actor& target, RE::Actor& caster) -> void;

auto is_power_attacking(RE::Actor& actor) -> bool;

auto has_absolute_keyword(RE::Actor& actor, RE::BGSKeyword& keyword) -> bool;

auto is_casting_actor(RE::Character& character) -> bool;

auto do_combat_spell_apply(RE::Actor* actor, RE::SpellItem* spell, RE::TESObjectREFR* target) -> void;

} // namespace Core
} // namespace Reflyem