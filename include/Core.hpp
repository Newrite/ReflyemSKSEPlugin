#pragma once
#include "Config.hpp"

namespace Reflyem::Core {

enum CastExtraInfo
{
  kNone = 0,
  kCastWithManacost = 1
};

template<typename L, typename R>
struct Either final
{
  bool is_l;

  union
  {
    L left;
    R right;
  };

  explicit Either(const L& l)
  {
    is_l = true;
    left = l;
  }

  explicit Either(const R& r)
  {
    is_l = false;
    right = r;
  }
};

struct ActorsCache final
{
  struct Data final
  {

    struct BaseCooldown final
    {
      BaseCooldown(const float start_duration, const float duration)
          : start_duration(start_duration)
          , duration(duration)
      {
      }
      BaseCooldown()
          : start_duration(0.f)
          , duration(0.f)
      {
      }
      float start_duration;
      float duration;
    };

    static constexpr auto POTIONS_ARRAY_SIZE = 250;
    static constexpr auto POTIONS_CAP_PER_TYPE = 50;
    static constexpr auto INDEX_HEALTH_START = 0;
    static constexpr auto INDEX_HEALTH_END = 50;
    static constexpr auto INDEX_STAMINA_START = 50;
    static constexpr auto INDEX_STAMINA_END = 100;
    static constexpr auto INDEX_MAGICKA_START = 100;
    static constexpr auto INDEX_MAGICKA_END = 150;
    static constexpr auto INDEX_OTHER_START = 150;
    static constexpr auto INDEX_OTHER_END = 200;
    static constexpr auto INDEX_POISON_START = 200;
    static constexpr auto INDEX_POISON_END = POTIONS_ARRAY_SIZE;
    BaseCooldown potions_cooldown_timers[POTIONS_ARRAY_SIZE]{};
    
    static constexpr auto KI_ENERGY_ARRAY_SIZE = 64;
    BaseCooldown ki_energy_cooldown_timers[KI_ENERGY_ARRAY_SIZE]{};

private:
    float last_delta_update_{0.f};
    float slow_time_duration_{0.f};

    float cast_on_crit_delay_{0.f};

    float timing_block_timer_{0.f};
    bool timing_block_flag_{false};
    std::int32_t timing_parry_counter_{0};
    float timing_parry_counter_timer_{0.f};

    float update_50_timer_{0.f};
    float update_100_timer_{0.f};
    float update_1000_timer_{0.f};
    float update_10000_timer_{0.f};

    float rally_magicka_cost_{0.f};

    float hit_by_player_timer_{0.f};

    float bash_parry_timer_{0.f};
    float bash_parry_timer_no_hit_{0.f};

    float stagger_duration_{0.f};
    
    float duration_after_block_hit_{0.f};
    bool slow_after_block_hit_flag_{false};
    
    bool start_dodge_in_next_frame_{false};

    bool start_hit_flag_{false};
    float start_attack_duration_{0.f};
    
    int32_t reservations_ki_{0};

    bool start_fire_flag_{false};

    float petrified_blood_accumulator_{0.f};

    float absorb_shield_value_{0.f};
    float absorb_shield_sound_cooldown_{0.f};
    float absorb_shield_effect_cooldown_{0.f};
    float absorb_shield_degen_delay_{0.f};

    float leech_accumulator_{0.f};
    float recoup_accumulator_{0.f};
    double vampirism_accumulator_{0.f};
    
    float absorb_gettin_accumulator_{0.f};
    float absorb_damage_accumulator_{0.f};

    float delay_before_restore_stamina_{0.f};

    float poise_recovery_time_{0.f};
    float poise_damage_cooldown_{0.f};
    float poise_value_{0.f};
    std::uint8_t poise_recovery_actions_{0};
    bool poise_in_recovery_{false};

    uint64_t last_tick_count_{GetTickCount64()};

    static constexpr auto mod(auto& value, const auto delta) -> void
    {
      value += delta;
      if (value < 0) {
        value = 0;
      }
    }

    static constexpr auto set(auto& value, const auto set_value) -> void
    {
      if (set_value >= 0) {
        value = set_value;
      } else {
        value = 0;
      }
    }

public:
    // mods
    auto mod_timing_parry_counter(const int32_t count) -> void { mod(timing_parry_counter_, count); }

    auto mod_slow_time_duration(const float delta) -> void { mod(slow_time_duration_, delta); }

    auto mod_start_attack_duration(const float delta) -> void { mod(start_attack_duration_, delta); }

    auto mod_duration_after_block_hit(const float delta) -> void { mod(duration_after_block_hit_, delta); }
    
    auto mod_stagger_duration(const float delta) -> void { mod(stagger_duration_, delta); }
    
    auto mod_poise_recovery_time(const float delta) -> void { mod(poise_recovery_time_, delta); }
    
    auto mod_poise_damage_cooldown(const float delta) -> void { mod(poise_damage_cooldown_, delta); }

    auto mod_potions_cooldown_timers(const float delta) -> void
    {

      let debug_print_array = [this, delta]() -> void {
        for (int32_t index : views::iota(INDEX_HEALTH_START, INDEX_POISON_END)) {
          let duration = potions_cooldown_timers[index].duration;
          if (duration > 0.f) {
            logi("DPA: DELTA [{}] INDEX [{}] DURATION [{}]"sv, delta, index, duration);
          }
        }
      };

      let handle_array_chunk = [this, delta](int start_index, int end_index) -> void {
        int32_t index_to_mod = -1;
        float cooldown = FLT_MAX;
        for (int32_t index : views::iota(start_index, end_index)) {
          let duration = potions_cooldown_timers[index].duration;
          if (duration > 0.f && duration < cooldown) {
            cooldown = duration;
            index_to_mod = index;
          }
        }
        if (index_to_mod >= 0) {
          mod(potions_cooldown_timers[index_to_mod].duration, delta);
        }
      };

      // debug_print_array();

      for (int32_t index : views::iota(INDEX_POISON_START, INDEX_POISON_END)) {
        mod(potions_cooldown_timers[index].duration, delta);
      }

      letr config = Config::get_singleton();
      if (config.potions_drink_limit().enable_one_cooldown()) {
        handle_array_chunk(INDEX_HEALTH_START, INDEX_HEALTH_END);
        handle_array_chunk(INDEX_STAMINA_START, INDEX_STAMINA_END);
        handle_array_chunk(INDEX_MAGICKA_START, INDEX_MAGICKA_END);
        handle_array_chunk(INDEX_OTHER_START, INDEX_OTHER_END);
        // debug_print_array();
        return;
      }

      for (int32_t index : views::iota(0, INDEX_POISON_START)) {
        mod(potions_cooldown_timers[index].duration, delta);
      }
      // debug_print_array();
    }

    auto mod_ki_energy_cooldown_timers(const float delta) -> void
    {

      let handle_array_chunk = [this, delta](int start_index, int end_index) -> void {
        int32_t index_to_mod = -1;
        float cooldown = FLT_MAX;
        for (int32_t index : views::iota(start_index, end_index)) {
          let duration = ki_energy_cooldown_timers[index].duration;
          if (duration > 0.f && duration < cooldown) {
            cooldown = duration;
            index_to_mod = index;
          }
        }
        if (index_to_mod >= 0) {
          mod(ki_energy_cooldown_timers[index_to_mod].duration, delta);
        }
      };

      letr config = Config::get_singleton();
      if (config.ki_energy_power().enable_one_cooldown()) {
        handle_array_chunk(0, KI_ENERGY_ARRAY_SIZE);
        return;
      }

      for (int32_t index : views::iota(0, KI_ENERGY_ARRAY_SIZE)) {
        mod(ki_energy_cooldown_timers[index].duration, delta);
      }
      // debug_print_array();
    }

    auto mod_timing_parry_counter_timer(const float delta) -> void
    {
      timing_parry_counter_timer_ += delta;
      if (timing_parry_counter_timer_ < 0) {
        timing_parry_counter(0);
        timing_parry_counter_timer_ = 0;
      }
    }

    auto mod_absorb_shield_value(const float delta) -> void { mod(absorb_shield_value_, delta); }
    auto mod_absorb_shield_sound_cooldown(const float delta) -> void { mod(absorb_shield_sound_cooldown_, delta); }
    auto mod_absorb_shield_effect_cooldown(const float delta) -> void { mod(absorb_shield_effect_cooldown_, delta); }
    auto mod_absorb_shield_degen_delay(const float delta) -> void { mod(absorb_shield_degen_delay_, delta); }

    auto mod_delay_before_restore_stamina(const float delta) -> void { mod(delay_before_restore_stamina_, delta); }

    auto mod_cast_on_crit_delay(const float delta) -> void { mod(cast_on_crit_delay_, delta); }

    auto mod_timing_block_timer(const float delta) -> void { mod(timing_block_timer_, delta); }

    auto mod_update_50_timer(const float delta) -> void { mod(update_50_timer_, delta); }

    auto mod_update_100_timer(const float delta) -> void { mod(update_100_timer_, delta); }

    auto mod_update_1000_timer(const float delta) -> void { mod(update_1000_timer_, delta); }
    
    auto mod_update_10000_timer(const float delta) -> void { mod(update_10000_timer_, delta); }

    auto mod_bash_parry_timer(const float delta) -> void { mod(bash_parry_timer_, delta); }
    
    auto mod_absorb_gettin_accumulator(const float delta) -> void { mod(absorb_gettin_accumulator_, delta); }
    auto mod_absorb_damage_accumulator(const float delta) -> void { mod(absorb_damage_accumulator_, delta); }

    auto mod_hit_by_player_timer(const float delta) -> void { mod(hit_by_player_timer_, delta); }

    auto mod_bash_parry_timer_no_hit(const float delta) -> void { mod(bash_parry_timer_no_hit_, delta); }

    auto mod_petrified_blood_accumulator(const float delta) -> void { mod(petrified_blood_accumulator_, delta); }
    
    auto mod_leech_accumulator(const float delta) -> void { mod(leech_accumulator_, delta); }
    auto mod_vampirism_accumulator(const double delta) -> void { mod(vampirism_accumulator_, delta); }
    
    auto mod_recoup_accumulator(const float delta) -> void { mod(recoup_accumulator_, delta); }

    // property
    [[nodiscard]] std::int32_t timing_parry_counter() const { return timing_parry_counter_; }

    void timing_parry_counter(const std::int32_t timing_parry_counter)
    {
      set(timing_parry_counter_, timing_parry_counter);
    }

    [[nodiscard]] float delay_before_restore_stamina() const { return delay_before_restore_stamina_; }

    void delay_before_restore_stamina(const float delay_before_restore_stamina)
    {
      set(delay_before_restore_stamina_, delay_before_restore_stamina);
    }

    [[nodiscard]] float timing_parry_counter_timer() const { return timing_parry_counter_timer_; }

    void timing_parry_counter_timer(const float timing_parry_counter_timer)
    {
      set(timing_parry_counter_timer_, timing_parry_counter_timer);
    }

    [[nodiscard]] float duration_after_block_hit() const { return duration_after_block_hit_; }

    void duration_after_block_hit(const float duration_after_block_hit)
    {
      set(duration_after_block_hit_, duration_after_block_hit);
    }

    [[nodiscard]] float poise_recovery_time() const { return poise_recovery_time_; }

    void poise_recovery_time(const float poise_recovery_time)
    {
      set(poise_recovery_time_, poise_recovery_time);
    }

    [[nodiscard]] float poise_damage_cooldown() const { return poise_damage_cooldown_; }

    void poise_damage_cooldown(const float poise_damage_cooldown)
    {
      set(poise_damage_cooldown_, poise_damage_cooldown);
    }

    [[nodiscard]] std::uint8_t poise_recovery_actions() const { return poise_recovery_actions_; }

    void poise_recovery_actions(const std::uint8_t poise_recovery_actions)
    {
      set(poise_recovery_actions_, poise_recovery_actions);
    }

    [[nodiscard]] bool poise_in_recovery() const { return poise_in_recovery_; }

    void poise_in_recovery(const bool poise_in_recovery) { poise_in_recovery_ = poise_in_recovery; }

    [[nodiscard]] float poise_value() const { return poise_value_; }
    
    void poise_value(const float poise_value) { set(poise_value_, poise_value); }

    [[nodiscard]] std::int32_t reservations_ki() const { return reservations_ki_; }

    void reservations_ki(const std::int32_t reservations_ki)
    {
      set(reservations_ki_, reservations_ki);
    }

    [[nodiscard]] float last_delta_update() const { return last_delta_update_; }

    void last_delta_update(const float last_delta_update) { set(last_delta_update_, last_delta_update); }
    
    [[nodiscard]] float stagger_duration() const { return stagger_duration_; }
    
    void stagger_duration(const float stagger_duration) { set(stagger_duration_, stagger_duration); }
    
    [[nodiscard]] bool start_hit_flag() const { return start_hit_flag_; }

    void start_hit_flag(const bool start_hit_flag) { start_hit_flag_ = start_hit_flag; }
    
    [[nodiscard]] bool start_dodge_in_next_frame() const { return start_dodge_in_next_frame_; }
    
    void start_dodge_in_next_frame(const bool start_dodge_in_next_frame) { start_dodge_in_next_frame_ = start_dodge_in_next_frame; }

    [[nodiscard]] bool slow_after_block_hit_flag() const { return slow_after_block_hit_flag_; }

    void slow_after_block_hit_flag(const bool slow_after_block_hit_flag)
    {
      slow_after_block_hit_flag_ = slow_after_block_hit_flag;
    }

    [[nodiscard]] float leech_accumulator() const { return leech_accumulator_; }

    void leech_accumulator(const float leech_accumulator)
    {
      leech_accumulator_ = leech_accumulator;
    }
    
    [[nodiscard]] double vampirism_accumulator() const { return vampirism_accumulator_; }

    void vampirism_accumulator(const double vampirism_accumulator)
    {
      vampirism_accumulator_ = vampirism_accumulator;
    }

    [[nodiscard]] float recoup_accumulator() const { return recoup_accumulator_; }

    void recoup_accumulator(const float recoup_accumulator)
    {
      recoup_accumulator_ = recoup_accumulator;
    }

    [[nodiscard]] float absorb_gettin_accumulator() const { return absorb_gettin_accumulator_; }

    void absorb_gettin_accumulator(const float absorb_gettin_accumulator)
    {
      absorb_gettin_accumulator_ = absorb_gettin_accumulator;
    }

    [[nodiscard]] float absorb_damage_accumulator() const { return absorb_damage_accumulator_; }

    void absorb_damage_accumulator(const float absorb_damage_accumulator)
    {
      absorb_damage_accumulator_ = absorb_damage_accumulator;
    }

    [[nodiscard]] bool start_fire_flag() const { return start_fire_flag_; }

    void start_fire_flag(const bool start_fire_flag) { start_fire_flag_ = start_fire_flag; }

    [[nodiscard]] float start_attack_duration() const { return start_attack_duration_; }

    void start_attack_duration(const float start_attack_duration)
    {
      set(start_attack_duration_, start_attack_duration);
    }

    [[nodiscard]] float cast_on_crit_delay() const { return cast_on_crit_delay_; }

    void cast_on_crit_delay(const float cast_on_crit_delay) { set(cast_on_crit_delay_, cast_on_crit_delay); }

    [[nodiscard]] float slow_time_duration() const { return slow_time_duration_; }

    void slow_time_duration(const float slow_time_duration) { set(slow_time_duration_, slow_time_duration); }

    [[nodiscard]] float hit_by_player_timer() const { return hit_by_player_timer_; }

    void hit_by_player_timer(const float hit_by_player_timer) { set(hit_by_player_timer_, hit_by_player_timer); }

    [[nodiscard]] float timing_block_timer() const { return timing_block_timer_; }

    void timing_block_timer(const float timing_block_timer)
    {
      if (!timing_block_flag_ && timing_block_timer_ <= 0.f) {
        timing_block_timer_ = timing_block_timer;
      }
    }

    [[nodiscard]] bool timing_block_flag() const { return timing_block_flag_; }

    void timing_block_flag(const bool timing_block_flag)
    {
      if (timing_block_flag) {
        timing_block_flag_ = true;
      }
      if (timing_block_timer() <= 0.f) {
        timing_block_flag_ = timing_block_flag;
      }
    }

    [[nodiscard]] float update_50_timer() const { return update_50_timer_; }

    void update_50_timer_refresh() { update_50_timer_ = 0.05f; }

    [[nodiscard]] float update_100_timer() const { return update_100_timer_; }

    void update_100_timer_refresh() { update_100_timer_ = 0.1f; }

    [[nodiscard]] float update_1000_timer() const { return update_1000_timer_; }

    void update_1000_timer_refresh() { update_1000_timer_ = 1.f; }

    [[nodiscard]] float update_10000_timer() const { return update_10000_timer_; }

    void update_10000_timer_refresh() { update_10000_timer_ = 10.f; }

    [[nodiscard]] float bash_parry_timer() const { return bash_parry_timer_; }

    void bash_parry_timer(const float bash_parry_timer) { set(bash_parry_timer_, bash_parry_timer); }

    [[nodiscard]] float bash_parry_timer_no_hit() const { return bash_parry_timer_no_hit_; }
    
    void rally_magicka_cost(const float rally_magicka_cost) { set(rally_magicka_cost_, rally_magicka_cost); }

    [[nodiscard]] float rally_magicka_cost() const { return rally_magicka_cost_; }

    void bash_parry_timer_no_hit(const float bash_parry_timer_no_hit)
    {
      set(bash_parry_timer_no_hit_, bash_parry_timer_no_hit);
    }

    [[nodiscard]] float petrified_blood_accumulator() const { return petrified_blood_accumulator_; }

    void petrified_blood_accumulator(const float petrified_blood_accumulator)
    {
      set(petrified_blood_accumulator_, petrified_blood_accumulator);
    }

    void absorb_shield_value(const float absorb_shield_value)
    {
      set(absorb_shield_value_, absorb_shield_value);
    }
    void absorb_shield_sound_cooldown(const float absorb_shield_sound_cooldown)
    {
      set(absorb_shield_sound_cooldown_, absorb_shield_sound_cooldown);
    }
    void absorb_shield_effect_cooldown(const float absorb_shield_effect_cooldown)
    {
      set(absorb_shield_effect_cooldown_, absorb_shield_effect_cooldown);
    }
    void absorb_shield_degen_delay(const float absorb_shield_degen_delay)
    {
      set(absorb_shield_degen_delay_, absorb_shield_degen_delay);
    }

    [[nodiscard]] float absorb_shield_value() const { return absorb_shield_value_; }
    [[nodiscard]] float absorb_shield_sound_cooldown() const { return absorb_shield_sound_cooldown_; }
    [[nodiscard]] float absorb_shield_effect_cooldown() const { return absorb_shield_effect_cooldown_; }
    [[nodiscard]] float absorb_shield_degen_delay() const { return absorb_shield_degen_delay_; }

    [[nodiscard]] uint64_t last_tick_count() const { return last_tick_count_; }

    void last_tick_count_refresh() { last_tick_count_ = GetTickCount64(); }

    // function
    auto update_handler(RE::Character& character, const float delta) -> void
    {
      const auto negative_delta = -std::abs(delta);
      mod_bash_parry_timer(negative_delta);
      // mod_slow_time_duration(negative_delta);
      mod_bash_parry_timer_no_hit(negative_delta);
      mod_cast_on_crit_delay(negative_delta);
      mod_timing_block_timer(negative_delta);
      mod_hit_by_player_timer(negative_delta);
      mod_update_50_timer(negative_delta);
      mod_update_100_timer(negative_delta);
      mod_update_1000_timer(negative_delta);
      mod_update_10000_timer(negative_delta);
      mod_timing_parry_counter_timer(negative_delta);
      mod_potions_cooldown_timers(negative_delta);
      // mod_ki_energy_cooldown_timers(character.IsInCombat() ? negative_delta : negative_delta * 3.f);
      mod_ki_energy_cooldown_timers(negative_delta);
      mod_delay_before_restore_stamina(negative_delta);
      mod_start_attack_duration(negative_delta);
      mod_duration_after_block_hit(negative_delta);
      mod_absorb_shield_effect_cooldown(negative_delta);
      mod_absorb_shield_sound_cooldown(negative_delta);
      mod_absorb_shield_degen_delay(negative_delta);
      mod_poise_damage_cooldown(negative_delta);
      mod_poise_recovery_time(negative_delta);
    }
  };

  using FormsMap = std::unordered_map<RE::FormID, Data>;

  private:
  FormsMap cache_map_{};
  static constexpr uint64_t GARBAGE_TIME = 5000;
  std::mutex mutex_;
  static constexpr uint32_t LABEL = 'ACCA';
  // ReSharper disable once CppVariableCanBeMadeConstexpr
  static const int32_t SERIALIZATION_VERSION = 15;

  [[nodiscard]] static auto is_garbage(const Data& data) -> bool
  {
    return (GetTickCount64() - data.last_tick_count()) >= GARBAGE_TIME;
  }

  auto clean_cache_map(const std::vector<RE::FormID>* keys_to_delete) -> void
  {
    if (!keys_to_delete) {
      return;
    }
    for (auto key : *keys_to_delete) {
      cache_map_.erase(key);
    }
  }

  auto garbage_collector() -> void
  {

    auto keys_to_delete = std::vector<RE::FormID>{};
    for (const auto& [form_id, data] : cache_map_) {
      if (is_garbage(data)) {
        keys_to_delete.push_back(form_id);
      }
    }

    clean_cache_map(&keys_to_delete);
  }

  auto load(const SKSE::SerializationInterface& a_interface) -> void
  {
    const auto lock = std::lock_guard{mutex_};
    uint32_t type;
    uint32_t version;
    uint32_t length;

    cache_map_.clear();
    logger::info("Start read actors cache"sv);

    while (a_interface.GetNextRecordInfo(type, version, length)) {
      switch (type) {
        case LABEL: {
          int32_t serialization_version;

          if (!a_interface.ReadRecordData(serialization_version)) {
            logger::error("Fail load ser version, return"sv);
            cache_map_.clear();
            return;
          }

          if (serialization_version != SERIALIZATION_VERSION) {
            logger::warn("Serialization version mismatched, clear cache and return, "
                         "versions: {} | {}"sv,
                         serialization_version,
                         SERIALIZATION_VERSION);
            cache_map_.clear();
            return;
          }

          size_t size;

          if (!a_interface.ReadRecordData(size)) {
            logger::error("Fail load size"sv);
            break;
          }

          for (size_t i = 0; i < size; ++i) {
            RE::FormID form_id;
            if (!a_interface.ReadRecordData(form_id)) {
              logger::error("Fail read formid"sv);
              break;
            }

            Data data;
            if (!a_interface.ReadRecordData(data)) {
              logger::error("Fail read formid"sv);
              break;
            }

            logger::info("Success read record with FormId: {} Data last tick: {}"sv, form_id, data.last_tick_count());
            cache_map_[form_id] = data;
          }
        }
        default: {
          logger::warn("Unrecognized signature type: {}"sv, type);
          break;
        }
      }
    }

    logger::info("Finish read actors cache"sv);
  }

  auto save(const SKSE::SerializationInterface& a_interface) -> void
  {
    const auto lock = std::lock_guard{mutex_};
    logger::info("Start write actors cache"sv);

    if (!a_interface.OpenRecord(LABEL, 1)) {
      logger::error("Error when try open record REFL on save"sv);
      return;
    }
    garbage_collector();

    if (!a_interface.WriteRecordData(&SERIALIZATION_VERSION, sizeof SERIALIZATION_VERSION)) {
      logger::error("Failed to write SERIALIZATION_VERSION"sv);
      return;
    }

    const size_t size = cache_map_.size();
    if (!a_interface.WriteRecordData(&size, sizeof size)) {
      logger::error("Failed to write size of map"sv);
      return;
    }

    for (const auto& [form_id, data] : cache_map_) {
      if (!a_interface.WriteRecordData(&form_id, sizeof form_id)) {
        logger::error("Failed to write form id"sv);
        return;
      }
      if (!a_interface.WriteRecordData(&data, sizeof data)) {
        logger::error("Failed to write data"sv);
        return;
      }
    }
    logger::info("Finish write actors cache"sv);
  }

  public:
  static auto skse_save_callback(SKSE::SerializationInterface* a_interface) -> void
  {
    if (!a_interface) {
      logger::error("Null skse serialization interface error when save"sv);
      return;
    }
    get_singleton().save(*a_interface);
  }

  static auto skse_load_callback(SKSE::SerializationInterface* a_interface) -> void
  {
    if (!a_interface) {
      logger::error("Null skse serialization interface error when load"sv);
      return;
    }
    get_singleton().load(*a_interface);
  }

  auto at_try(const RE::FormID key) -> std::optional<std::reference_wrapper<Data>>
  {
    const auto it = cache_map_.find(key);

    if (it == cache_map_.end()) {
      return std::nullopt;
    }

    return it->second;
  }

  auto at(const RE::FormID key) -> std::reference_wrapper<Data> { return cache_map_.at(key); }

  [[nodiscard]] auto exist(const RE::FormID key) const -> bool { return cache_map_.contains(key); }

  auto get_or_add(const RE::FormID key) -> std::reference_wrapper<Data>
  {
    const auto lock = std::lock_guard{mutex_};
    if (exist(key)) {
      const auto data = at(key);
      return data;
    }
    cache_map_[key] = Data();
    const auto data = at(key);
    return data;
  }

  static auto get_singleton() noexcept -> ActorsCache&
  {
    static auto cache = ActorsCache();

    return cache;
  }

  static auto get_or_add_actor_data(const RE::FormID form_id) -> Data&
  {
    auto& cache = get_singleton();
    return cache.get_or_add(form_id).get();
  }
};

auto set_fake_block_frame(const bool new_state) -> void;

auto is_fake_block() -> bool;

auto character_timer_map_handler(ULONGLONG now_time, std::map<std::uintptr_t, ULONGLONG>& character_timer_map) -> void;

auto bound_data_comparer(const RE::TESBoundObject::BOUND_DATA& bound_data, const int16_t comparer_value) -> bool;

auto get_random_int() -> int;

auto actor_from_ni_pointer(const RE::NiPointer<RE::TESObjectREFR>* ni_actor) -> RE::Actor*;

auto actor_from_actor_handle(const RE::ActorHandle* handle) -> RE::Actor*;

auto get_commander_actor(const RE::Actor* actor) -> RE::Actor*;

auto get_player_followers() -> std::vector<RE::Actor*> ;

auto get_commanded_actors(const RE::Actor* actor) -> RE::BSTArray<RE::CommandedActorData>*;

auto is_player_ally(RE::Actor* actor) -> bool;

auto damage_actor_value(RE::Actor& actor, RE::ActorValue av, float value) -> void;

auto restore_actor_value(RE::Actor& actor, const RE::ActorValue av, const float value) -> void;

auto set_av_regen_delay(RE::AIProcess* process, RE::ActorValue av, float time) -> void;

auto can_modify_actor_value(const RE::ValueModifierEffect* a_this,
                            const RE::Actor* a_actor,
                            float a_value,
                            RE::ActorValue av) -> bool;

auto flash_hud_meter(const RE::ActorValue av) -> void;

auto get_current_equip_weapon(RE::AIProcess* process, const bool is_left) -> RE::InventoryEntryData*;

auto object_is_two_handed_weapon(RE::TESBoundObject* object) -> bool;

auto object_is_shield(RE::TESBoundObject* object) -> bool;

auto actor_has_active_mgef_with_keyword(RE::Actor& actor, const RE::BGSKeyword& keyword) -> bool;

auto try_actor_has_active_mgef_with_keyword(RE::Actor* actor, const RE::BGSKeyword* keyword) -> bool;

auto get_effects_magnitude_sum(const std::vector<RE::ActiveEffect*>& effects) -> std::optional<float>;

auto get_effects_magnitude_mult(const std::vector<RE::ActiveEffect*>& effects) -> float;
auto is_concentraion_spell(const RE::TESForm* spell_form) -> bool;

auto get_effects_by_keyword(RE::Actor& actor, const RE::BGSKeyword& keyword) -> std::vector<RE::ActiveEffect*>;

auto try_get_effect_by_keyword(RE::Actor* actor, const RE::BGSKeyword* keyword) -> RE::ActiveEffect*;

auto try_get_effects_by_keyword(RE::Actor* actor, const RE::BGSKeyword* keyword) -> std::vector<RE::ActiveEffect*>;

auto is_menu_allow() -> bool;

auto get_dual_value_mult(const RE::ValueModifierEffect& active_effect) -> float;

auto get_second_av(const RE::ActiveEffect& active_effect) -> RE::ActorValue;

auto getting_damage_mult(RE::Actor& actor) -> float;

auto cast(RE::SpellItem& spell, RE::Actor& target, RE::Actor& caster, CastExtraInfo extra, const float effect_mult) -> void;

// template<typename FormType>
// void get_data(RE::FormID form_id, std::string_view mod_name);

// #define get_data(TypeName, RE::FormID form_id, std::string_view mod_name) (let data_handler =
// RE::TESDataHandler::GetSingleton(); if (!data_handler) { logi("Data handler is null"sv); return
// nullptr; return data_handler->LookupForm<TypeName>(form_id, mod_name);) #define
// test_macro(some_typename) some_typename func (some_typename x) {return x;}

auto get_left_hand_equip_slot() -> RE::BGSEquipSlot*;

auto get_right_hand_equip_slot() -> RE::BGSEquipSlot*;

auto get_both_hand_equip_slot() -> RE::BGSEquipSlot*;

auto get_either_hand_equip_slot() -> RE::BGSEquipSlot*;

bool is_script_attached(const RE::TESForm* a_form);

auto get_gem_keyword() -> RE::BGSKeyword*;

auto get_magic_disallow_keyword() -> RE::BGSKeyword*;

auto get_soul_gem_keyword() -> RE::BGSKeyword*;

auto get_location_ref_type_boss() -> RE::BGSLocationRefType*;

auto is_soul_gem_form(RE::TESBoundObject* object) -> bool;

auto is_furniture_form(RE::TESBoundObject* object) -> bool;

auto get_ghost_shader() -> RE::TESEffectShader*;

auto get_vendor_item_key() -> RE::BGSKeyword*;

auto is_misc_form(RE::TESBoundObject* object) -> bool;

auto get_effect_assoc_item_bound_weapon(const RE::EffectSetting* effect) -> RE::TESObjectWEAP*;

auto get_effect_assoc_item_bound_weapon(const RE::MagicItem* magic_item) -> RE::TESObjectWEAP*;

auto is_container_form(RE::TESBoundObject* object) -> bool;

auto is_activator_form(RE::TESBoundObject* object) -> bool;

auto is_pickup_sound_form(RE::TESBoundObject* object) -> bool;

auto is_food_form(RE::TESBoundObject* object) -> bool;

auto get_full_damage_from_hit_data(const RE::HitData* hit_data) -> std::optional<float>;

auto is_ingredient_form(RE::TESBoundObject* object) -> bool;

auto is_key_form(RE::TESBoundObject* object) -> bool;

auto get_ore_ingot_keyword() -> RE::BGSKeyword*;

auto get_voice_equip_slot() -> RE::BGSEquipSlot*;

auto equip_slot_comparer(RE::BGSEquipSlot* first, RE::BGSEquipSlot* second) -> bool;

auto cast_on_handle_formlists(RE::BGSListForm* keywords,
                              RE::BGSListForm* spells,
                              RE::Actor& caster,
                              RE::Actor& target,
                              CastExtraInfo extra = kNone, const float effect_mult = 1.f) -> void;

auto cast_on_handle(RE::TESForm* keyword,
                    RE::TESForm* spell,
                    RE::Actor& target,
                    RE::Actor& caster,
                    CastExtraInfo extra = kNone, const float effect_mult = 1.f) -> void;

auto console_log(std::string&& str) -> void;

auto cast(RE::SpellItem* spell, RE::TESObjectREFR* target, RE::Actor* caster, CastExtraInfo extra, const float effect_mult) -> void;

auto get_actor_value_percent(RE::Actor* actor, const RE::ActorValue av) -> float;

auto cast_on_handle(RE::TESForm* keyword,
                    RE::TESForm* spell,
                    RE::TESObjectREFR* target,
                    RE::TESObjectREFR* caster,
                    CastExtraInfo extra = kNone, const float effect_mult = 1.f) -> void;

auto direct_apply_spell_on_target(RE::ActorMagicCaster* actor_caster,
                                  RE::MagicItem* magic_item,
                                  RE::TESObjectREFR* target,
                                  RE::TESBoundObject* object,
                                  bool a5) -> bool;

auto cast_on_handle_formlists(RE::BGSListForm* keywords,
                              RE::BGSListForm* spells,
                              RE::Actor* caster,
                              RE::TESObjectREFR* target,
                              CastExtraInfo extra = kNone, const float effect_mult = 1.f) -> void;

auto is_blocking(RE::Actor* actor) -> bool;

auto is_power_attacking(RE::Actor& actor) -> bool;

auto get_shooter_ref_from_projectile(const RE::Projectile* projectile) -> RE::TESObjectREFR*;

auto has_absolute_keyword(RE::Actor& actor, RE::BGSKeyword& keyword) -> bool;

auto magic_item_has_keyword(RE::MagicItem* magic_item, const RE::BGSKeyword* keyword) -> bool;

auto try_has_absolute_keyword(RE::Actor* actor, RE::BGSKeyword* keyword) -> bool;

auto is_casting_actor(RE::Actor& character) -> bool;

auto is_bow_or_crossbow_attack_actor(const RE::Actor& character) -> bool;

auto do_combat_spell_apply(RE::Actor* actor, RE::SpellItem* spell, RE::TESObjectREFR* target) -> void;

auto bow_fire(RE::Actor* fire_actor) -> void;

auto place_at_me(RE::TESObjectREFR* target,
                 RE::TESForm* form,
                 std::uint32_t count,
                 bool force_persist,
                 bool initially_disabled) -> RE::TESObjectREFR*;

auto get_equipped_shield(const RE::Actor* actor) -> RE::TESObjectARMO*;

auto actor_is_on_flying_mount(RE::Actor* actor) -> bool;

auto player_start_shout(RE::PlayerCharacter* player, RE::TESShout* shout) -> bool;

auto initialization_hit_data(RE::HitData& hit_data,
                             RE::Actor* attacker,
                             RE::Actor* target,
                             RE::InventoryEntryData* weapon,
                             bool is_left) -> void;

auto initialization_hit_proj_data(RE::HitData& hit_data,
                                  RE::Actor* attacker,
                                  RE::Actor* target,
                                  RE::Projectile* projectile) -> void;

auto apply_all_combat_spells_from_attack(RE::Character* attacker,
                                         RE::TESObjectWEAP* weapon,
                                         bool is_left,
                                         RE::Actor* target) -> void;

auto get_poison(RE::InventoryEntryData* _this) -> RE::AlchemyItem*;

auto calculate_block_cost(float shield_armor_or_weapon_damage,
                          float block_av,
                          float shield_perks_av,
                          bool block_with_weapon,
                          bool block_with_unarmed,
                          bool is_power_attack) -> float;

auto set_global_time_multiplier(float mult, bool a3) -> void;

auto poison_object(RE::InventoryEntryData* data, RE::AlchemyItem* poison, int count) -> void;

auto get_actor_value_max(RE::Actor* actor, const RE::ActorValue av) -> float;

auto get_weapon(const RE::Actor& actor, const bool is_left_hand, RE::TESObjectWEAP* fallback_weapon)
    -> RE::TESObjectWEAP*;

auto get_float_game_setting(const char* setting_name) -> std::optional<float>;

auto try_form_has_keyword(const RE::TESForm* form, const RE::BGSKeyword* keyword) -> bool;

auto try_keyword_form_has_keyword(const RE::BGSKeywordForm* form, const RE::BGSKeyword* keyword) -> bool;

auto get_actor_value_owner_as_actor(RE::ActorValueOwner* actor_value_owner) -> RE::Actor*;

auto is_dual_wielding(const RE::Actor* actor) -> bool;

auto is_bashing(const RE::Actor* attacker) -> bool;

auto string_split(const std::string& string_to_split, const char delimiter) -> std::vector<std::string>;

auto string_to_float(const std::string& input) -> std::optional<float>;

auto string_to_int(const std::string& input) -> std::optional<int>;

auto play_sound_base(RE::BGSSoundDescriptorForm* sound, RE::Actor* actor) -> void;

auto formlist_revert(RE::BGSListForm* form_list) -> void;

auto play_sound(RE::BGSSoundDescriptorForm* sound, RE::Actor* actor) -> void;

namespace ExternalApi {
static PRECISION_API::IVPrecision4* precision_api = nullptr;
static TRUEHUD_API::IVTrueHUD4* true_hud_api = nullptr;
static GIGAWIDGET_API::IVGigaWidget1* giga_widget_api = nullptr;

static auto get_max_resource_special_bar(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  return 10.f;
}

static auto get_current_resource_special_bar(RE::Actor* actor) -> float
{
  if (!actor) {
    return 0.f;
  }

  static RE::FormID keyword_id = 0xADA123;
  static std::string_view mod_name = "Update.esm";
  static std::string_view editor_id = "MAG_StaggerCooldownKeyword";
  let keyword_stagger_form = RE::TESForm::LookupByEditorID(editor_id);
  if (keyword_stagger_form && keyword_stagger_form->As<RE::BGSKeyword>()) {
    if (try_has_absolute_keyword(actor, keyword_stagger_form->As<RE::BGSKeyword>())) {
      return 0.f;
    }
    return 10.f;
  }

  return 5.f;
}

}

} // namespace Reflyem::Core
