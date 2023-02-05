#pragma once

namespace Reflyem::Core {

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
    enum class Updates {
      k50Ms   = 0,
      k100Ms  = 1,
      k1000Ms = 2,
    };

  private:
    float regen_health_delay_{0.f};
    float regen_stamina_delay_{0.f};
    float regen_magicka_delay_{0.f};

    float last_delta_update_{0.f};

    float cast_on_crit_delay_{0.f};

    float timing_block_start_delta_{0.f};
    bool  blocking_flag_{false};

    float update005_{0.f};
    float update01_{0.f};
    float update1_{0.f};

    uint64_t last_tick_count_;

    // TODO: Тайминг блок реализовать
    // Поднимается щит, ставится делей на 1.0 и начинает убывать каждый фрейм, делей меньше 0 быть
    // не может Последующие удары которые заблокированы, смотрят дельту с 1.0 - делей, если она
    // меньше нужного значения, засчитывается тайминг блок \ парри флаг на из блокинг, который может
    // замутироваться только когда делй дойдет до 0 и позволит снова поставить его на 1 при поднятии
    // щита

    auto mod_timing_block_start_delta(const float delta) -> float {
      timing_block_start_delta_ -= std::abs(delta);
      if (timing_block_start_delta_ < 0.f) {
        timing_block_start_delta_ = 0.f;
      }
      return timing_block_start_delta_;
    }

    auto mod_health_delay(const float delta) -> float {
      regen_health_delay_ -= delta;
      if (regen_health_delay_ < 0.f) {
        regen_health_delay_ = 0.f;
      }
      return regen_health_delay_;
    }

    auto mod_stamina_delay(const float delta) -> float {
      regen_stamina_delay_ -= delta;
      if (regen_stamina_delay_ < 0.f) {
        regen_stamina_delay_ = 0.f;
      }
      return regen_stamina_delay_;
    }

    auto mod_magicka_delay(const float delta) -> float {
      regen_magicka_delay_ -= delta;
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

    auto mod_cast_on_crit_delay(const float delay) -> void {
      cast_on_crit_delay_ -= delay;
      if (cast_on_crit_delay_ < 0.f) {
        cast_on_crit_delay_ = 0.f;
      }
    }

    auto mod_update_ticks(const float delta) -> void {
      update005_ -= delta;
      update01_ -= delta;
      update1_ -= delta;
    }

  public:
    explicit Data() : last_tick_count_(GetTickCount64()) {}

    [[nodiscard]] auto blocking_flag() const -> bool { return blocking_flag_; }

    auto set_blocking_flag(const bool flag) -> void {
      if (flag) {
        blocking_flag_ = true;
      }
      if (timing_block_start_delta_ <= 0.f) {
        blocking_flag_ = flag;
      }
    }

    auto set_timing_block_start_delta(const float delta) -> void {
      if (!blocking_flag_ && timing_block_start_delta_ <= 0.f) {
        timing_block_start_delta_ = delta;
      }
    }

    [[nodiscard]] auto timing_block_start_delta() const -> float {
      return timing_block_start_delta_;
    }

    [[nodiscard]] auto last_delta_update() const -> float { return last_delta_update_; }

    auto set_last_delta_update(const float delta) -> void { last_delta_update_ = delta; }

    [[nodiscard]] auto cast_on_crit_delay() const -> float { return cast_on_crit_delay_; }

    auto set_cast_on_crit_delay(const float cast_on_crit_delay) -> void {
      cast_on_crit_delay_ = cast_on_crit_delay;
    }

    [[nodiscard]] auto regen_health_delay() const -> float { return regen_health_delay_; }
    [[nodiscard]] auto regen_stamina_delay() const -> float { return regen_stamina_delay_; }
    [[nodiscard]] auto regen_magicka_delay() const -> float { return regen_magicka_delay_; }
    [[nodiscard]] auto last_tick_count() const -> uint64_t { return last_tick_count_; }
    auto               refresh_last_tick_count() -> void { last_tick_count_ = GetTickCount64(); }

    [[nodiscard]] auto update(const Updates tick) const -> float {
      switch (tick) {
      case Updates::k50Ms: {
        return update005_;
      }
      case Updates::k100Ms: {
        return update01_;
      }
      case Updates::k1000Ms: {
        return update1_;
      }
      }
      return 1.f;
    }

    auto refresh_update(const Updates tick) -> void {
      switch (tick) {
      case Updates::k50Ms: {
        update005_ = 0.05f;
        return;
      }
      case Updates::k100Ms: {
        update01_ = 0.1f;
        return;
      }
      case Updates::k1000Ms: {
        update1_ = 1.f;
      }
      }
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

    auto handle_delta(const float delta) -> void {
      mod_update_ticks(delta);
      mod_all_regens_delay(delta);
      mod_cast_on_crit_delay(delta);
      mod_timing_block_start_delta(delta);
    }
  };

  using FormsMap = std::unordered_map<RE::FormID, Data>;

private:
  FormsMap                  cache_map_;
  static constexpr uint64_t garbage_time = 5000;
  std::mutex                mutex_;

  [[nodiscard]] static auto is_garbage(const Data& data) -> bool {
    const auto tick_now = GetTickCount64();
    return (tick_now - data.last_tick_count()) >= garbage_time;
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
    mutex_.lock();
    garbage_collector();
    cache_map_[key] = Data();
    mutex_.unlock();
    return at(key);
  }

  static auto get_singleton() noexcept -> ActorsCache& {
    static auto cache = ActorsCache();

    return cache;
  }
};

auto character_timer_map_handler(ULONGLONG                            now_time,
                                 std::map<std::uintptr_t, ULONGLONG>& character_timer_map) -> void;

auto bound_data_comparer(const RE::TESBoundObject::BOUND_DATA& bound_data,
                         const int16_t                         comparer_value) -> bool;

auto get_random_int() -> int;

auto damage_actor_value(RE::Actor& actor, RE::ActorValue av, float value) -> void;

auto set_av_regen_delay(RE::AIProcess* process, RE::ActorValue av, float time) -> void;

auto can_modify_actor_value(const RE::ValueModifierEffect* a_this, const RE::Actor* a_actor,
                            float a_value, RE::ActorValue av) -> bool;

auto flash_hud_meter(const RE::ActorValue av) -> void;

auto actor_has_active_mgef_with_keyword(RE::Actor& actor, const RE::BGSKeyword& keyword) -> bool;

auto get_effects_magnitude_sum(const std::vector<RE::ActiveEffect*>& effects)
    -> std::optional<float>;

auto get_effects_by_keyword(RE::Actor& actor, const RE::BGSKeyword& keyword)
    -> std::vector<RE::ActiveEffect*>;

auto get_dual_value_mult(const RE::ValueModifierEffect& active_effect) -> float;

auto get_second_av(const RE::ActiveEffect& active_effect) -> RE::ActorValue;

auto getting_damage_mult(RE::Actor& actor) -> float;

auto cast(RE::SpellItem& spell, RE::Actor& target, RE::Actor& caster) -> void;

auto cast_on_handle(RE::TESForm* keyword, RE::TESForm* spell, RE::Actor& target, RE::Actor& caster)
    -> void;

auto is_power_attacking(RE::Actor& actor) -> bool;

auto has_absolute_keyword(RE::Actor& actor, RE::BGSKeyword& keyword) -> bool;

auto is_casting_actor(RE::Character& character) -> bool;

auto do_combat_spell_apply(RE::Actor* actor, RE::SpellItem* spell, RE::TESObjectREFR* target)
    -> void;

auto place_at_me(RE::TESObjectREFR* target, RE::TESForm* form, std::uint32_t count,
                 bool force_persist, bool initially_disabled) -> RE::TESObjectREFR*;

} // namespace Reflyem::Core