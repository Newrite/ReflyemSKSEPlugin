#include "KiEnergyPower.hpp"

#include "Core.hpp"

namespace Reflyem::KiEnergyPower {

static constexpr auto SUMMON_KI_DURATION_TIME = 86400.f;

struct KiSummonData final
{
  RE::SpellItem* spell;
  uint16_t count;
};

// both FormID ref to spell
static std::map<RE::FormID, KiSummonData> summon_count_spell{};
static std::map<RE::FormID, uint16_t> summon_count_active{};

auto spell_is_in_form_list(const RE::SpellItem* spell) -> bool
{

  if (!spell) {
    return false;
  }

  letr config = Config::get_singleton();
  if (config.ki_energy_power().enable_ki_summons() && config.ki_energy_power().ki_summons_list()) {
    return config.ki_energy_power().ki_summons_list()->HasForm(spell);
  }
  return false;
}

auto is_in_form_list(const RE::ActiveEffect* active_effect) -> bool
{
  if (active_effect && active_effect->spell) {
    return spell_is_in_form_list(active_effect->spell->As<RE::SpellItem>());
  }
  return false;
}

auto get_count_active_effect_by_first_effect_in_spell(RE::Actor* actor, RE::SpellItem* spell) -> std::uint16_t
{

  std::uint16_t count = 0;

  if (!actor || !spell) {
    return count;
  }

  if (spell->effects.empty()) {
    return count;
  }

  let first_effect = spell->effects[0];
  if (!first_effect) {
    return count;
  }

  let active_effects = actor->GetActiveEffectList();
  if (!active_effects) {
    return count;
  }

  for (auto active_effect : *active_effects) {
    if (!active_effect || !active_effect->effect || !active_effect->effect->baseEffect || !active_effect->spell) {
      continue;
    }

    const auto effect = active_effect->effect;

    if (active_effect->spell->formID == spell->formID &&
        effect->baseEffect->formID == first_effect->baseEffect->formID) {
      count = count + 1;
    }
  }

  return count;
}

auto clear_effects_from_spell(RE::Actor* actor, RE::SpellItem* spell) -> void
{

  if (!actor || !spell) {
    return;
  }

  if (spell->effects.empty()) {
    return;
  }

  let active_effects = actor->GetActiveEffectList();
  if (!active_effects) {
    return;
  }

  for (auto active_effect : *active_effects) {
    if (!active_effect || !active_effect->effect || !active_effect->effect->baseEffect || !active_effect->spell) {
      continue;
    }

    const auto effect = active_effect->effect;

    if (active_effect->spell->formID == spell->formID) {
      for (auto effect_form_spell : spell->effects) {
        if (effect_form_spell && effect_form_spell->baseEffect &&
            effect->baseEffect->formID == effect_form_spell->baseEffect->formID) {
          active_effect->Dispel(true);
        }
      }
    }
  }
}

auto get_ki_cost_form_summon_spell_level(RE::SpellItem* spell) -> uint16_t
{
  if (!spell) {
    return 0;
  }

  if (spell->effects.empty()) {
    return 0;
  }

  if (!spell->effects[0]) {
    return 0;
  }

  let minimum_skill = spell->effects[0]->baseEffect->data.minimumSkill;

  if (minimum_skill >= 100) {
    return 5;
  }

  if (minimum_skill >= 75) {
    return 4;
  }

  if (minimum_skill >= 50) {
    return 3;
  }

  if (minimum_skill >= 25) {
    return 2;
  }

  return 1;
}

auto is_spell_has_summoned_effect(RE::SpellItem* spell) -> bool
{
  if (!spell) {
    return false;
  }

  if (spell->effects.empty()) {
    return false;
  }

  if (!spell->effects[0]) {
    return false;
  }

  let base_effect = spell->effects[0]->baseEffect;

  if (base_effect->data.archetype == RE::EffectArchetype::kSummonCreature && base_effect->data.associatedForm) {
    return true;
  }

  return false;
}

auto get_start_and_end_index(const int32_t cap) -> std::tuple<int32_t, int32_t>
{

  int32_t start_index = 0;
  int32_t end_index =
      Core::ActorsCache::Data::KI_ENERGY_ARRAY_SIZE - (Core::ActorsCache::Data::KI_ENERGY_ARRAY_SIZE - cap);

  return {start_index, end_index};
}

auto get_cap_base(RE::Actor* actor, const RE::BGSKeyword* cap_keyword, int32_t cap) -> int32_t
{

  if (cap_keyword) {
    let effects = Core::try_get_effects_by_keyword(actor, cap_keyword);
    let magnitudes_sum = Core::get_effects_magnitude_sum(effects).value_or(0.f);
    cap = cap + static_cast<int32_t>(magnitudes_sum);
  }

  if (cap > Core::ActorsCache::Data::KI_ENERGY_ARRAY_SIZE) {
    cap = Core::ActorsCache::Data::KI_ENERGY_ARRAY_SIZE;
  }

  return cap;
}

auto get_cap(RE::Actor* actor, const Config& config) -> int32_t
{
  return get_cap_base(actor, config.ki_energy_power().cap_keyword(), config.ki_energy_power().cap_base());
}

auto get_cap_with_reservation(RE::Actor* actor, const Config& config) -> int32_t
{
  auto cap = get_cap_base(actor, config.ki_energy_power().cap_keyword(), config.ki_energy_power().cap_base());

  const auto& ki_data = Core::ActorsCache::get_singleton().get_or_add(actor->formID).get();

  cap = cap - ki_data.reservations_ki();

  return cap;
}

auto get_duration_base(RE::Actor* actor, const RE::BGSKeyword* duration_keyword, float duration) -> float
{

  if (duration_keyword) {
    let effects = Core::try_get_effects_by_keyword(actor, duration_keyword);
    let magnitudes_sum = Core::get_effects_magnitude_sum(effects).value_or(0.f);
    duration = duration + magnitudes_sum;
  }

  if (duration < 0.f) {
    duration = 0.f;
  }

  return duration;
}

auto get_duration(RE::Actor* actor, const Config& config) -> float
{
  return get_duration_base(
      actor, config.ki_energy_power().duration_keyword(), config.ki_energy_power().duration_base());
}

auto update_summon_effect(RE::SummonCreatureEffect* active_effect, float) -> void
{

  if (!active_effect) {
    return;
  }

  auto caster = active_effect->GetCasterActor();
  if (!caster) {
    return;
  }

  letr config = Config::get_singleton();
  let cap = get_cap_with_reservation(caster.get(), config);
  if (cap < 0) {
    return;
  }

  if (is_in_form_list(active_effect) && caster->IsPlayerRef() && active_effect->duration < SUMMON_KI_DURATION_TIME) {
    // let caster_loc = caster->GetPosition();
    // if (active_effect->summonPlacementEffect) {
    //   logi("Move summon placement effect");
    //   active_effect->summonPlacementEffect->location = caster_loc;
    // }
    // logi("Move summon placement");
    // active_effect->commandedActor.get()->MoveTo(caster.get());
    // active_effect->commandedActor.get()->Enable(false);
    active_effect->duration = SUMMON_KI_DURATION_TIME;
  }
}

auto summons_proc(RE::Actor* caster) -> void
{
  if (!caster) {
    return;
  }

  letr config = Config::get_singleton();
  let cap = get_cap_with_reservation(caster, config);
  if (cap < 0) {
    return;
  }

  summon_count_active.clear();
  summon_count_spell.clear();

  if (config.ki_energy_power().ki_summons_list()) {
    config.ki_energy_power().ki_summons_list()->ForEachForm([&](RE::TESForm* form) -> RE::BSContainer::ForEachResult {
      if (!form || !form->As<RE::SpellItem>()) {
        return RE::BSContainer::ForEachResult::kContinue;
      }

      let spell = form->As<RE::SpellItem>();
      let effect_count = get_count_active_effect_by_first_effect_in_spell(caster, spell);
      Core::console_log(std::format("KiSummons: Spell {} Calced EffectCount {}", spell->GetFullName(), effect_count));
      summon_count_active[spell->formID] = effect_count;
      if (summon_count_spell.contains(spell->formID)) {
        Core::console_log(std::format("KiSummons: Spell {} added plus one to data count", spell->GetFullName()));
        auto& ki_summon_data = summon_count_spell[spell->formID];
        ki_summon_data.count = ki_summon_data.count + 1;
      } else {
        Core::console_log(std::format("KiSummons: Spell {} init data count", spell->GetFullName()));
        summon_count_spell[spell->formID] = {spell, 1};
      }

      return RE::BSContainer::ForEachResult::kContinue;
    });
  }

  for (auto [form_id, data] : summon_count_spell) {
    auto effects_count = summon_count_active[form_id];
    Core::console_log(std::format(
        "KiSummons: Spell {} EffectsCount {} CountsInData {}", data.spell->GetFullName(), data.count, effects_count));
    while (effects_count < data.count) {
      effects_count = effects_count + 1;
      auto old_delivery_type = RE::MagicSystem::Delivery::kTotal;
      auto old_range = -1.f;
      if (data.spell->data.delivery == RE::MagicSystem::Delivery::kTargetLocation &&
          data.spell->effects[0]->baseEffect->data.delivery == RE::MagicSystem::Delivery::kTargetLocation) {
        old_delivery_type = RE::MagicSystem::Delivery::kTargetLocation;
        Core::console_log(std::format("KiSummons: Spell {} Delivery type set to Self", data.spell->GetFullName()));
        old_range = data.spell->data.range;
        data.spell->data.range = 0.f;
        data.spell->data.delivery = RE::MagicSystem::Delivery::kSelf;
        data.spell->effects[0]->baseEffect->data.delivery = RE::MagicSystem::Delivery::kSelf;
      }
      if (data.spell->data.delivery == RE::MagicSystem::Delivery::kTargetActor &&
          data.spell->effects[0]->baseEffect->data.delivery == RE::MagicSystem::Delivery::kTargetActor) {
        old_delivery_type = RE::MagicSystem::Delivery::kTargetActor;
        Core::console_log(std::format("KiSummons: Spell {} Delivery type set to Self", data.spell->GetFullName()));
        old_range = data.spell->data.range;
        data.spell->data.range = 0.f;
        data.spell->data.delivery = RE::MagicSystem::Delivery::kSelf;
        data.spell->effects[0]->baseEffect->data.delivery = RE::MagicSystem::Delivery::kSelf;
      }
      caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
          ->CastSpellImmediate(data.spell, false, caster, 1.f, false, 0, nullptr);
      if (old_delivery_type != RE::MagicSystem::Delivery::kTotal) {
        data.spell->data.range = old_range;
        data.spell->data.delivery = old_delivery_type;
        data.spell->effects[0]->baseEffect->data.delivery = old_delivery_type;
      }
    }
  }

  summon_count_spell.clear();
  summon_count_active.clear();
}

auto get_cooldown_percent_from_closest_slot_to_reset(RE::Actor* actor, const Config& config) -> std::uint8_t
{

  if (!actor) {
    return 100;
  }

  const auto& ki_data = Core::ActorsCache::get_singleton().get_or_add(actor->formID).get();

  auto ki_energy_cooldown = Core::ActorsCache::Data::BaseCooldown(0.f, 0.f);
  auto is_found_some = false;

  auto cap = get_cap_with_reservation(actor, config);
  if (cap < 0) {
    cap = 0;
  }
  let[start_index, end_index] = get_start_and_end_index(cap);

  for (int32_t index = start_index; index < end_index; index++) {
    if (ki_data.ki_energy_cooldown_timers[index].duration > 0.f) {

      if (!is_found_some) {
        ki_energy_cooldown = ki_data.ki_energy_cooldown_timers[index];
        is_found_some = true;
        continue;
      }

      if (ki_data.ki_energy_cooldown_timers[index].duration < ki_energy_cooldown.duration) {
        ki_energy_cooldown = ki_data.ki_energy_cooldown_timers[index];
      }
    }
  }

  if (is_found_some) {
    if (ki_energy_cooldown.start_duration == 0.f) {
      return 100;
    }
    return 100 - static_cast<std::uint8_t>(ki_energy_cooldown.duration / (ki_energy_cooldown.start_duration / 100.f));
  }

  return 100;
}

auto get_cooldown_ki_energy_from_closest_slot_to_reset(RE::Actor* actor, const Config& config)
    -> Core::ActorsCache::Data::BaseCooldown*
{

  if (!actor) {
    return nullptr;
  }

  auto& ki_data = Core::ActorsCache::get_singleton().get_or_add(actor->formID).get();

  auto cap = get_cap_with_reservation(actor, config);
  if (cap < 0) {
    cap = 0;
  }
  let[start_index, end_index] = get_start_and_end_index(cap);

  auto cooldown_index = -1;
  auto is_found_some = false;
  for (int32_t index = start_index; index < end_index; index++) {
    if (ki_data.ki_energy_cooldown_timers[index].duration > 0.f) {

      if (!is_found_some) {
        cooldown_index = index;
        is_found_some = true;
        continue;
      }

      if (ki_data.ki_energy_cooldown_timers[index].duration <
          ki_data.ki_energy_cooldown_timers[cooldown_index].duration) {
        cooldown_index = index;
      }
    }
  }

  if (is_found_some && cooldown_index >= 0) {
    if (ki_data.ki_energy_cooldown_timers[cooldown_index].duration <= 0.f) {
      return nullptr;
    }
    return std::addressof(ki_data.ki_energy_cooldown_timers[cooldown_index]);
  }

  return nullptr;
}

auto modify_active_cooldowns(RE::Actor* actor, const float amount, const Config& config) -> void
{

  if (!actor) {
    return;
  }

  auto& ki_data = Core::ActorsCache::get_singleton().get_or_add(actor->formID).get();

  auto cap = get_cap_with_reservation(actor, config);
  if (cap < 0) {
    cap = 0;
  }
  let[start_index, end_index] = get_start_and_end_index(cap);

  for (int32_t index = start_index; index < end_index; index++) {
    if (ki_data.ki_energy_cooldown_timers[index].duration > 0.f) {

      ki_data.ki_energy_cooldown_timers[index].duration += amount;
      if (ki_data.ki_energy_cooldown_timers[index].duration < 0.f) {
        ki_data.ki_energy_cooldown_timers[index].duration = 0.f;
      }
    }
  }
}

auto get_reservation_ki_energy(RE::Actor* actor, const Config& config) -> std::uint32_t
{

  if (!actor) {
    return 0;
  }

  const auto& ki_data = Core::ActorsCache::get_singleton().get_or_add(actor->formID).get();

  return ki_data.reservations_ki();
}

auto get_available_ki_energy(RE::Actor* actor, const Config& config) -> GIGAWIDGET_API::AvailableKiEnergyResult
{

  if (!actor) {
    return GIGAWIDGET_API::AvailableKiEnergyResult{};
  }

  const auto& ki_data = Core::ActorsCache::get_singleton().get_or_add(actor->formID).get();

  std::uint16_t slots_counter = 0;
  float minimal_duration = 0.f;
  auto is_found_some_duration = false;
  auto cap = get_cap_with_reservation(actor, config);
  if (cap < 0) {
    cap = 0;
  }
  let[start_index, end_index] = get_start_and_end_index(cap);

  for (int32_t index = start_index; index < end_index; index++) {

    if (ki_data.ki_energy_cooldown_timers[index].duration <= 0.f) {
      slots_counter = slots_counter + 1;
    }

    if (ki_data.ki_energy_cooldown_timers[index].duration > 0.f) {

      if (!is_found_some_duration) {
        minimal_duration = ki_data.ki_energy_cooldown_timers[index].duration;
        is_found_some_duration = true;
        continue;
      }

      if (ki_data.ki_energy_cooldown_timers[index].duration < minimal_duration) {
        minimal_duration = ki_data.ki_energy_cooldown_timers[index].duration;
      }
    }
  }

  return {static_cast<std::uint16_t>(minimal_duration), slots_counter};
}

auto use_ki_energy(RE::Actor* actor, RE::SpellItem* power) -> UseKiResult
{

  letr config = Config::get_singleton();

  if (!Core::try_form_has_keyword(power, config.ki_energy_power().ki_power_keyword())) {
    return UseKiResult::kNoneKi;
  }

  auto duration = get_duration(actor, config);

  if (duration <= 0.f) {
    duration = 5.f;
    // return UseKiResult::kFail;
  }

  auto& ki_data = Core::ActorsCache::get_singleton().get_or_add(actor->formID).get();
  let cap = get_cap_with_reservation(actor, config);

  if (cap <= 0) {
    RE::DebugNotification("Cap ki is 0"s.c_str());
    return UseKiResult::kFail;
  }

  let availbe = get_available_ki_energy(actor, config);
  let cost = power->data.costOverride;

  if (availbe.slots >= cost) {
    auto used_slots = 0;
    for (auto& ki_energy_cooldown_timer : ki_data.ki_energy_cooldown_timers) {
      if (ki_energy_cooldown_timer.duration <= 0.f) {
        ki_energy_cooldown_timer = Core::ActorsCache::Data::BaseCooldown(duration, duration);
        used_slots = used_slots + 1;
        if (used_slots >= cost) {
          return UseKiResult::kOk;
        }
      }
    }
  }

  RE::DebugNotification(config.ki_energy_power().notify().c_str());
  Core::play_sound(Config::get_singleton().caster_additions().mag_fail_sound(), actor);
  return UseKiResult::kFail;
}

auto set_graph_values(RE::Actor* actor) -> void
{

  if (!actor) {
    return;
  }

  constexpr auto iKiEnergyReserved = "iKiEnergyReserved"sv;
  constexpr auto iKiEnergyCap = "iKiEnergyCap"sv;
  constexpr auto iKiEnergyAvailable = "iKiEnergyAvailable"sv;

  auto& ki_data = Core::ActorsCache::get_or_add_actor_data(actor->formID);
  letr config = Config::get_singleton();

  let ki_reversed = ki_data.reservations_ki();
  let ki_cap = get_cap_with_reservation(actor, config);
  let ki_available = get_available_ki_energy(actor, config);

  actor->SetGraphVariableInt(iKiEnergyReserved, ki_reversed);
  actor->SetGraphVariableInt(iKiEnergyCap, ki_cap);
  actor->SetGraphVariableInt(iKiEnergyAvailable, ki_available.slots);
}

auto update(RE::Actor* actor) -> void
{
  letr config = Config::get_singleton();

  set_graph_values(actor);

  if (!actor) {
    return;
  }

  auto& data = Core::ActorsCache::get_or_add_actor_data(actor->formID);
  std::int32_t ki_reservations = 0;

  if (config.ki_energy_power().enable_reservation()) {
    let keywords = config.ki_energy_power().ki_reservation_keywords();
    let spells = config.ki_energy_power().ki_reservation_abilities();
    let spells_no_condition = config.ki_energy_power().ki_reservation_abilities_no_condition();
    auto ki_cap = get_cap(actor, config);

    if (spells_no_condition && actor->IsPlayerRef()) {

      spells_no_condition->ForEachForm([&](RE::TESForm* form) -> RE::BSContainer::ForEachResult {
        if (!form || !form->As<RE::SpellItem>()) {
          return RE::BSContainer::ForEachResult::kContinue;
        }

        let spell = form->As<RE::SpellItem>();

        if (!spell) {
          return RE::BSContainer::ForEachResult::kContinue;
        }

        std::int32_t cost = spell->data.costOverride;
        if (config.ki_energy_power().enable_reservation_adjust_cost()) {
          cost = static_cast<std::int32_t>(spell->CalculateMagickaCost(actor));
        }

        if (ki_cap >= cost) {
          actor->AddSpell(spell);
          ki_cap = ki_cap - cost;
          ki_reservations = ki_reservations + cost;
        } else {
          actor->RemoveSpell(spell);
        }

        return RE::BSContainer::ForEachResult::kContinue;
      });
      
    }

    if (keywords && spells) {
      const auto length_kw = keywords->forms.size();
      const auto length_sp = spells->forms.size();

      for (std::uint32_t index = 0u; index < length_kw && index < length_sp; index++) {

        let keyword_form = keywords->forms[index];
        let spell_form = spells->forms[index];

        if (!keyword_form || !spell_form) {
          continue;
        }

        let keyword = keyword_form->As<RE::BGSKeyword>();
        let spell = spell_form->As<RE::SpellItem>();

        if (!keyword || !spell) {
          continue;
        }

        if (!Core::try_has_absolute_keyword(actor, keyword)) {
          actor->RemoveSpell(spell);
          continue;
        }

        std::int32_t cost = spell->data.costOverride;
        if (config.ki_energy_power().enable_reservation_adjust_cost()) {
          cost = static_cast<std::int32_t>(spell->CalculateMagickaCost(actor));
        }

        if (ki_cap >= cost) {
          actor->AddSpell(spell);
          ki_cap = ki_cap - cost;
          ki_reservations = ki_reservations + cost;
        } else {
          actor->RemoveSpell(spell);
        }
      }
    }
  }

  if (config.ki_energy_power().enable_ki_summons() && config.ki_energy_power().ki_summons_list()) {
    config.ki_energy_power().ki_summons_list()->ForEachForm([&](RE::TESForm* form) -> RE::BSContainer::ForEachResult {
      if (!form || !form->As<RE::SpellItem>()) {
        return RE::BSContainer::ForEachResult::kContinue;
      }

      let spell = form->As<RE::SpellItem>();
      ki_reservations = ki_reservations + get_ki_cost_form_summon_spell_level(spell);

      return RE::BSContainer::ForEachResult::kContinue;
    });
  }

  data.reservations_ki(ki_reservations);
}

auto clear_effects() -> void
{

  letr config = Config::get_singleton();
  auto player = RE::PlayerCharacter::GetSingleton();

  if (config.ki_energy_power().enable_ki_summons() && config.ki_energy_power().ki_summons_list()) {
    config.ki_energy_power().ki_summons_list()->ForEachForm([&](RE::TESForm* form) -> RE::BSContainer::ForEachResult {
      if (!form || !form->As<RE::SpellItem>()) {
        return RE::BSContainer::ForEachResult::kContinue;
      }

      let spell = form->As<RE::SpellItem>();
      clear_effects_from_spell(player, spell);

      return RE::BSContainer::ForEachResult::kContinue;
    });
  }
}

auto action_event_handler(const SKSE::ActionEvent& event, RE::BSTEventSource<SKSE::ActionEvent>&) -> void
{

  const auto& config = Config::get_singleton();

  if (!event.actor) {
    return;
  }

  switch (event.type.get()) {
    case SKSE::ActionEvent::Type::kSpellCast:
    case SKSE::ActionEvent::Type::kSpellFire: {
      if (config.ki_energy_power().enable_ki_summons() && event.sourceForm &&
          spell_is_in_form_list(event.sourceForm->As<RE::SpellItem>())) {
        RE::DebugNotification("Spell active in Ki Summon list, can't cast.");
        event.actor->InterruptCast(true);
      }
      break;
    }
    case SKSE::ActionEvent::Type::kWeaponSwing:
    case SKSE::ActionEvent::Type::kVoiceCast:
    case SKSE::ActionEvent::Type::kVoiceFire:
    case SKSE::ActionEvent::Type::kBowDraw:
    case SKSE::ActionEvent::Type::kBowRelease:
    case SKSE::ActionEvent::Type::kBeginDraw:
    case SKSE::ActionEvent::Type::kEndDraw:
    case SKSE::ActionEvent::Type::kBeginSheathe:
    case SKSE::ActionEvent::Type::kEndSheathe:
      break;
  }
}

namespace Callbacks {

auto set_frame_for_ki_energy_callback() -> std::uint8_t
{

  let config = Config::get_singleton();

  return get_cooldown_percent_from_closest_slot_to_reset(RE::PlayerCharacter::GetSingleton(), config);
}

auto set_available_ki_energy_callback() -> GIGAWIDGET_API::AvailableKiEnergyResult
{

  let config = Config::get_singleton();
  if (!config.ki_energy_power().enable()) {
    return GIGAWIDGET_API::AvailableKiEnergyResult{};
  }

  return get_available_ki_energy(RE::PlayerCharacter::GetSingleton(), config);
}

}

#undef AddForm

// papyrus function
// ReSharper disable once CppInconsistentNaming
auto CallKiSummonsHandle(RE::StaticFunctionTag*) -> void
{
  letr config = Config::get_singleton();
  if (!config.ki_energy_power().enable_ki_summons()) {
    RE::DebugNotification("Ki summons functions not enabled in config");
    return;
  }
  if (!config.ki_energy_power().ki_summons_list()) {
    RE::DebugNotification("Ki summons list from config is NULL");
    return;
  }

  auto player = RE::PlayerCharacter::GetSingleton();
  let right_hand_form = player->GetEquippedObject(false);
  if (!right_hand_form || !right_hand_form->As<RE::SpellItem>()) {
    clear_effects();
    Core::formlist_revert(config.ki_energy_power().ki_summons_list());
    RE::DebugNotification("Ki Summons cleared.");
    return;
  }

  let spell = right_hand_form->As<RE::SpellItem>();
  if (spell) {

    if (!is_spell_has_summoned_effect(spell)) {
      RE::DebugNotification("Spell in right hand without summon effect.");
      return;
    }

    config.ki_energy_power().ki_summons_list()->AddForm(spell);
    RE::DebugNotification(std::format("Spell {} add to Ki Summons", spell->GetFullName()).c_str());
  }
}

// papyrus function
// ReSharper disable once CppInconsistentNaming
auto GetAvailableKiEnergy(RE::StaticFunctionTag*, RE::Actor* actor) -> int
{

  if (actor == nullptr) {
    return -1;
  }

  letr config = Config::get_singleton();
  if (!config.ki_energy_power().enable()) {
    return -1;
  }

  let ki = get_available_ki_energy(actor, config);

  return ki.slots;
}

// papyrus function
// ReSharper disable once CppInconsistentNaming
auto ModifyKiEnergyCooldown(RE::StaticFunctionTag*, RE::Actor* actor, const float amount, const bool closest) -> void
{

  if (amount == 0.f || actor == nullptr) {
    return;
  }

  letr config = Config::get_singleton();
  if (!config.ki_energy_power().enable()) {
    return;
  }

  if (closest) {
    auto ki_energy_cooldown = get_cooldown_ki_energy_from_closest_slot_to_reset(actor, config);
    if (ki_energy_cooldown) {
      ki_energy_cooldown->duration = ki_energy_cooldown->duration + amount;
      if (ki_energy_cooldown->duration < 0.f) {
        ki_energy_cooldown->duration = 0.f;
      }
    }
    return;
  }

  modify_active_cooldowns(actor, amount, config);
}

// papyrus function
// ReSharper disable once CppInconsistentNaming
auto SpendKiEnergy(RE::StaticFunctionTag*, RE::Actor* actor, const int cost) -> void
{


  letr config = Config::get_singleton();

  auto duration = get_duration(actor, config);

  if (duration <= 0.f) {
    duration = 5.f;
  }

  auto& ki_data = Core::ActorsCache::get_singleton().get_or_add(actor->formID).get();
  let cap = get_cap_with_reservation(actor, config);

  if (cap <= 0) {
    return;
  }

  let availbe = get_available_ki_energy(actor, config);

  if (availbe.slots >= cost) {
    auto used_slots = 0;
    for (auto& ki_energy_cooldown_timer : ki_data.ki_energy_cooldown_timers) {
      if (ki_energy_cooldown_timer.duration <= 0.f) {
        ki_energy_cooldown_timer = Core::ActorsCache::Data::BaseCooldown(duration, duration);
        used_slots = used_slots + 1;
        if (used_slots >= cost) {
          return;
        }
      }
    }
  }
  
}

} // namespace Reflyem::PotionsDrinkLimit
