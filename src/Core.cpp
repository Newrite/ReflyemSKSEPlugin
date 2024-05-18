#include "Core.hpp"
#include <random>

namespace Reflyem::Core {

using EffectSettingFlag = RE::EffectSetting::EffectSettingData::Flag;

auto bound_data_comparer(const RE::TESBoundObject::BOUND_DATA& bound_data, const int16_t comparer_value) -> bool
{
  return comparer_value == bound_data.boundMax.x && comparer_value == bound_data.boundMax.y &&
         comparer_value == bound_data.boundMax.z && comparer_value == bound_data.boundMin.x &&
         comparer_value == bound_data.boundMin.y && comparer_value == bound_data.boundMin.z;
}

auto get_random_int() -> int
{
  // StackOverflow:
  // https://stackoverflow.com/questions/5008804/generating-a-random-integer-from-a-range
  static std::random_device rd;                          // Only used once to initialise (seed) engine
  static std::mt19937 rng(rd());                         // Random-number engine used (Mersenne-Twister in this case)
  static std::uniform_int_distribution<int> uni(0, 100); // Guaranteed unbiased
  return uni(rng);
}

auto actor_from_ni_pointer(const RE::NiPointer<RE::TESObjectREFR>* ni_actor) -> RE::Actor*
{
  if (!ni_actor) {
    return nullptr;
  }
  let ref = ni_actor->get();
  if (!ref) {
    return nullptr;
  }

  return ref->As<RE::Actor>();
}

auto actor_from_actor_handle(const RE::ActorHandle* handle) -> RE::Actor*
{
  if (!handle) {
    return nullptr;
  }

  let ni_actor = handle->get();
  if (!ni_actor) {
    return nullptr;
  }

  return ni_actor.get();
}

auto get_commander_actor(const RE::Actor* actor) -> RE::Actor*
{
  if (!actor) {
    return nullptr;
  }

  let target_process = actor->currentProcess;

  if (!target_process) {
    return nullptr;
  }

  let commanding_actor_handle = target_process->GetCommandingActor();

  return actor_from_actor_handle(&commanding_actor_handle);
}

auto get_commanded_actors(const RE::Actor* actor) -> RE::BSTArray<RE::CommandedActorData>*
{
  if (!actor) {
    return nullptr;
  }

  let process = actor->currentProcess;
  if (!process) {
    return nullptr;
  }

  let mid_high = process->middleHigh;
  if (!mid_high) {
    return nullptr;
  }

  if (!mid_high->commandedActors.empty()) {
    return std::addressof(mid_high->commandedActors);
  }
  return nullptr;
}

auto is_player_ally(RE::Actor* actor) -> bool
{
  if (!actor) {
    return false;
  }
  let player = RE::PlayerCharacter::GetSingleton();
  if (!player) {
    return false;
  }
  return actor->IsPlayerTeammate() || !actor->IsHostileToActor(player);
}

auto flash_hud_meter(const RE::ActorValue av) -> void
{
  using FuncT = decltype(&flash_hud_meter);
  const REL::Relocation<FuncT> func{REL::ID(51907)};
  return func(av);
}

auto get_current_equip_weapon(RE::AIProcess* process, const bool is_left) -> RE::InventoryEntryData*
{
  if (!process) {
    return nullptr;
  }
  using FuncT = decltype(&get_current_equip_weapon);
  const REL::Relocation<FuncT> func{REL::ID(38781)};
  return func(process, is_left);
}


auto character_timer_map_handler(const ULONGLONG now_time, std::map<std::uintptr_t, ULONGLONG>& character_timer_map)
    -> void
{
  logger::debug("handle timer map"sv);
  for (auto& [character_ptr, old_time] : character_timer_map) {
    logger::debug("Pre time check"sv);
    if (now_time - old_time >= 5000) {
      character_timer_map.erase(character_ptr);
      continue;
    }

    logger::debug("After time check"sv);
  }
}

auto damage_actor_value(RE::Actor& actor, const RE::ActorValue av, float value) -> void
{
  logger::debug("DamageActorValue: {} {}"sv, av, value);
  if (value > 0.f) {
    value = value * -1.f;
  }
  actor.RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, av, value);
}

auto restore_actor_value(RE::Actor& actor, const RE::ActorValue av, const float value) -> void
{
  logger::debug("RestoreActorValue: {} {}"sv, av, value);
  if (value <= 0.f) {
    return;
  }

  actor.RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, av, value);
}

auto can_modify_actor_value(const RE::ValueModifierEffect* a_this,
                            const RE::Actor* a_actor,
                            const float a_value,
                            const RE::ActorValue av) -> bool
{
  if (av != RE::ActorValue::kNone && av != RE::ActorValue::kHealth) {
    return false;
  }

  if (av == RE::ActorValue::kNone && a_this->actorValue != RE::ActorValue::kHealth) {
    return false;
  }

  if (a_value >= 0.f) {
    return false;
  }

  if (a_actor->IsDead()) {
    return false;
  }

  const auto caster_ptr = a_this->GetCasterActor();
  if (!caster_ptr || !a_actor || !a_this->effect || !a_this->effect->baseEffect) {
    return false;
  }

  const auto caster = caster_ptr.get();
  if (!caster) {
    return false;
  }

  if (caster == a_actor) {
    return false;
  }

  if (a_this->effect->baseEffect->data.flags.any(EffectSettingFlag::kRecover)) {
    return false;
  }

  if (!a_this->effect->baseEffect->data.flags.any(EffectSettingFlag::kDetrimental)) {
    return false;
  }

  return true;
}

auto get_effects_magnitude_sum(const std::vector<RE::ActiveEffect*>& effects) -> std::optional<float>
{
  if (effects.empty()) {
    return std::nullopt;
  }

  auto pos_value = 0.f;
  auto neg_value = 0.f;

  for (const auto active_effect : effects) {
    if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) || !active_effect->effect ||
        !active_effect->effect->baseEffect) {
      continue;
    }

    const auto effect = active_effect->effect;
    const auto base_effect = effect->baseEffect;

    if (base_effect->data.flags.any(RE::EffectSetting::EffectSettingData::Flag::kDetrimental)) {
      neg_value += active_effect->magnitude;
    } else {
      pos_value += active_effect->magnitude;
    }
  }

  if (pos_value > 0.f || neg_value > 0.f) {
    return std::optional{pos_value - neg_value};
  }
  return std::nullopt;
}

auto get_effects_by_keyword(RE::Actor& actor, const RE::BGSKeyword& keyword) -> std::vector<RE::ActiveEffect*>
{
  auto active_effects = actor.GetActiveEffectList();
  std::vector<RE::ActiveEffect*> effects = {};
  if (!active_effects) {
    return effects;
  }

  for (auto active_effect : *active_effects) {
    if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) || !active_effect->effect ||
        !active_effect->effect->baseEffect) {
      continue;
    }

    const auto effect = active_effect->effect;

    if (const auto base_effect = effect->baseEffect; !base_effect->HasKeywordID(keyword.formID)) {
      continue;
    }

    effects.push_back(active_effect);
  }

  return effects;
}

auto try_get_effects_by_keyword(RE::Actor* actor, const RE::BGSKeyword* keyword) -> std::vector<RE::ActiveEffect*>
{
  std::vector<RE::ActiveEffect*> effects = {};

  if (!actor || !keyword) {
    return effects;
  }

  auto active_effects = actor->GetActiveEffectList();
  if (!active_effects) {
    return effects;
  }

  for (auto active_effect : *active_effects) {
    if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) || !active_effect->effect ||
        !active_effect->effect->baseEffect) {
      continue;
    }

    const auto effect = active_effect->effect;

    if (const auto base_effect = effect->baseEffect; !base_effect->HasKeywordID(keyword->formID)) {
      continue;
    }

    effects.push_back(active_effect);
  }

  return effects;
}

auto get_dual_value_mult(const RE::ValueModifierEffect& active_effect) -> float
{
  if (!active_effect.effect || !active_effect.effect->baseEffect) {
    return 0.f;
  }
  return active_effect.effect->baseEffect->data.secondAVWeight;
}

auto get_second_av(const RE::ActiveEffect& active_effect) -> RE::ActorValue
{
  if (!active_effect.effect || !active_effect.effect->baseEffect) {
    return RE::ActorValue::kNone;
  }

  if (!active_effect.effect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kDualValueModifier)) {
    return RE::ActorValue::kNone;
  }

  return active_effect.effect->baseEffect->data.secondaryAV;
}

auto set_av_regen_delay(RE::AIProcess* process, RE::ActorValue av, float time) -> void
{
  if (!process) {
    return;
  }

  using FuncT = decltype(&set_av_regen_delay);
  const REL::Relocation<FuncT> func{RELOCATION_ID(38526, 0)};
  return func(process, av, time);
}

auto getting_damage_mult(RE::Actor& actor) -> float
{
  static auto ByPCDiffMap = std::map<std::int32_t, const char*>{{0, "fDiffMultHPByPCVE"},
                                                                {1, "fDiffMultHPByPCE"},
                                                                {2, "fDiffMultHPByPCN"},
                                                                {3, "fDiffMultHPByPCH"},
                                                                {4, "fDiffMultHPByPCVH"},
                                                                {5, "fDiffMultHPByPCL"}};

  static auto ToPCDiffMap = std::map<std::int32_t, const char*>{{0, "fDiffMultHPToPCVE"},
                                                                {1, "fDiffMultHPToPCE"},
                                                                {2, "fDiffMultHPToPCN"},
                                                                {3, "fDiffMultHPToPCH"},
                                                                {4, "fDiffMultHPToPCVH"},
                                                                {5, "fDiffMultHPToPCL"}};

  let settings_collection = RE::GameSettingCollection::GetSingleton();
  let player = RE::PlayerCharacter::GetSingleton();

  if (!settings_collection || !player) {
    return 1.f;
  }

  let difficulty = player->difficulty;
  if (difficulty < 0 || difficulty > 5) {
    return 1.f;
  }

  let f_diff_mult_hp_by_pc = settings_collection->GetSetting(ByPCDiffMap[difficulty]);
  let f_diff_mult_hp_to_pc = settings_collection->GetSetting(ToPCDiffMap[difficulty]);

  if (!f_diff_mult_hp_by_pc || !f_diff_mult_hp_to_pc) {
    return 1.f;
  }

  if (actor.IsPlayer() || actor.IsPlayerTeammate() || !actor.IsHostileToActor(player)) {
    return f_diff_mult_hp_to_pc->GetFloat();
  }

  return f_diff_mult_hp_by_pc->GetFloat();
}

auto actor_has_active_mgef_with_keyword(RE::Actor& actor, const RE::BGSKeyword& keyword) -> bool
{
  auto active_effects = actor.GetActiveEffectList();
  if (!active_effects) {
    return false;
  }
  logger::debug("Start search mgef wigh keyword"sv);
  for (const auto active_effect : *active_effects) {
    if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) || !active_effect->effect ||
        !active_effect->effect->baseEffect) {
      continue;
    }
    const auto base_effect = active_effect->effect->baseEffect;

    logger::debug("Not null effect and active"sv);

    if (base_effect->HasKeyword(&keyword)) {
      logger::debug("Found keyword with id"sv);
      return true;
    }
  }
  logger::debug("Not found keyword with id"sv);
  return false;
}

auto try_actor_has_active_mgef_with_keyword(RE::Actor* actor, const RE::BGSKeyword* keyword) -> bool
{
  if (!actor || !keyword) {
    return false;
  }

  auto active_effects = actor->GetActiveEffectList();
  if (!active_effects) {
    return false;
  }
  logd("Start search mgef wigh keyword"sv);
  for (const auto active_effect : *active_effects) {
    if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) || !active_effect->effect ||
        !active_effect->effect->baseEffect) {
      logd("Skip effect"sv);
      continue;
    }
    const auto base_effect = active_effect->effect->baseEffect;

    logd("Not null effect and active"sv);

    if (base_effect->HasKeyword(keyword)) {
      logd("Found keyword with id"sv);
      return true;
    }
  }
  logd("Not found keyword with id"sv);
  return false;
}

auto cast(RE::SpellItem& spell, RE::Actor& target, RE::Actor& caster) -> void
{
  auto& caster_ = false ? spell.HasKeyword(nullptr) ? target : caster : caster;
  auto& target_ = false ? spell.HasKeyword(nullptr) ? caster : target : target;
  if (spell.data.delivery == RE::MagicSystem::Delivery::kSelf) {
    caster.GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
        ->CastSpellImmediate(&spell, true, &caster_, 1.00f, false, 0.0f, &caster_);
  } else {
    caster.GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
        ->CastSpellImmediate(&spell, true, &target_, 1.00f, false, 0.0f, &caster_);
  }
}

// template<typename FormType>
// void get_data(RE::FormID form_id, std::string_view mod_name) {
//   let data_handler = RE::TESDataHandler::GetSingleton();
//   if (!data_handler) {
//     logi("Data handler is null"sv);
//     return;
//   }
//   return;
//   return data_handler->LookupForm<FormType>(form_id, mod_name);
// }

auto cast_with_source(RE::SpellItem* spell,
                      RE::Actor* target,
                      RE::Actor* caster,
                      const RE::MagicSystem::CastingSource casting_source) -> void
{
  if (!spell || !target || !caster) {
    return;
  }

  let caster_ = false ? spell->HasKeyword(nullptr) ? target : caster : caster;
  let target_ = false ? spell->HasKeyword(nullptr) ? caster : target : target;
  if (spell->data.delivery == RE::MagicSystem::Delivery::kSelf) {
    caster->GetMagicCaster(casting_source)->CastSpellImmediate(spell, true, caster_, 1.00f, false, 0.0f, caster_);
  } else {
    caster->GetMagicCaster(casting_source)->CastSpellImmediate(spell, true, target_, 1.00f, false, 0.0f, caster_);
  }
}

auto get_left_hand_equip_slot() -> RE::BGSEquipSlot*
{
  let object_manager = RE::BGSDefaultObjectManager::GetSingleton();
  if (!object_manager) {
    return nullptr;
  }
  return object_manager->objects[19]->As<RE::BGSEquipSlot>();
}

auto get_right_hand_equip_slot() -> RE::BGSEquipSlot*
{
  let object_manager = RE::BGSDefaultObjectManager::GetSingleton();
  if (!object_manager) {
    return nullptr;
  }
  return object_manager->objects[20]->As<RE::BGSEquipSlot>();
}

auto get_voice_equip_slot() -> RE::BGSEquipSlot*
{
  let object_manager = RE::BGSDefaultObjectManager::GetSingleton();
  if (!object_manager) {
    return nullptr;
  }
  return object_manager->objects[20]->As<RE::BGSEquipSlot>();
}

auto equip_slot_comparer(RE::BGSEquipSlot* first, RE::BGSEquipSlot* second) -> bool
{
  if (!first || !second) {
    logi("Some of equip slots is null"sv);
    return false;
  }

  const REL::Relocation<decltype(&equip_slot_comparer)> func{RELOCATION_ID(23148, 0)};
  return func(first, second);
}

auto cast_on_handle_formlists(RE::BGSListForm* keywords, RE::BGSListForm* spells, RE::Actor& caster, RE::Actor& target)
    -> void
{
  if (!keywords || !spells) {
    return;
  }

  const auto length_kw = keywords->forms.size();
  const auto length_sp = spells->forms.size();
  logger::debug("LKW LSP: {} {}", length_kw, length_sp);
  for (std::uint32_t index = 0u; index < length_kw && index < length_sp; index++) {
    logger::debug("LKW LSP index: {}", index);
    cast_on_handle(keywords->forms[index], spells->forms[index], target, caster);
  }
}

auto cast_on_handle(RE::TESForm* keyword, RE::TESForm* spell, RE::Actor& target, RE::Actor& caster) -> void
{
  if (!spell) {
    return;
  }

  bool allow_cast = false;

  RE::BGSKeyword* keyword_ptr = nullptr;

  logger::debug("Cast_On_Handle before cast to keyword and spell");

  if (keyword) {
    keyword_ptr = keyword->As<RE::BGSKeyword>();
  }
  const auto spell_ptr = spell->As<RE::SpellItem>();

  if (!spell_ptr) {
    return;
  }

  if (!keyword_ptr) {
    allow_cast = true;
  }

  logger::debug("Cast_On_Handle before call has_absolute_keyword");
  if (!allow_cast && !has_absolute_keyword(caster, *keyword_ptr)) {
    return;
  }

  logger::debug("Cast_On_Handle before call cast");
  cast(*spell_ptr, target, caster);
}

auto is_power_attacking(RE::Actor& actor) -> bool
{
  const auto current_process = actor.currentProcess;
  if (!current_process) {
    return false;
  }

  const auto high_process = current_process->high;
  if (!high_process) {
    return false;
  }

  const auto& attack_data = high_process->attackData;
  if (!attack_data) {
    return false;
  }

  const auto flags = attack_data->data.flags;
  return flags.any(RE::AttackData::AttackFlag::kPowerAttack);
}

auto worn_has_keyword(RE::Actor* actor, RE::BGSKeyword* keyword) -> bool
{
  if (actor && keyword) {
    auto inv = actor->GetInventoryChanges();
    if (!inv) {
      return false;
    }
    using FuncT = bool (*)(RE::InventoryChanges*, RE::BGSKeyword*);
    const REL::Relocation<FuncT> func{RELOCATION_ID(15808, 0)};
    logger::debug("Call worn_has_keyword");
    return func(inv, keyword);
  }
  return false;
}

auto do_combat_spell_apply(RE::Actor* actor, RE::SpellItem* spell, RE::TESObjectREFR* target) -> void
{
  if (!actor || !spell || !target) {
    return;
  }

  using FuncT = void (*)(int32_t, int32_t, RE::Actor*, RE::SpellItem*, RE::TESObjectREFR*);
  const REL::Relocation<FuncT> func{RELOCATION_ID(54124, 0)};
  return func(0, 0, actor, spell, target);
}

auto has_absolute_keyword(RE::Actor& actor, RE::BGSKeyword& keyword) -> bool
{
  logger::debug("Before check keywords in has_absolute_keyword");
  const auto result = actor.HasKeyword(&keyword) || actor_has_active_mgef_with_keyword(actor, keyword) ||
                      worn_has_keyword(&actor, &keyword);
  logger::debug("After check keywords in has_absolute_keyword, result: {}", result);
  return result;
}

auto try_has_absolute_keyword(RE::Actor* actor, RE::BGSKeyword* keyword) -> bool
{
  if (!actor || !keyword) {
    return false;
  }
  logger::debug("Before check keywords in has_absolute_keyword");
  const auto result = actor->HasKeyword(keyword) || try_actor_has_active_mgef_with_keyword(actor, keyword) ||
                      worn_has_keyword(actor, keyword);
  logger::debug("After check keywords in has_absolute_keyword, result: {}", result);
  return result;
}

auto is_casting_actor(RE::Character& character) -> bool
{
  if (character.IsDead()) {
    return false;
  }

  const auto caster_left = character.GetMagicCaster(RE::MagicSystem::CastingSource::kLeftHand);
  const auto caster_right = character.GetMagicCaster(RE::MagicSystem::CastingSource::kRightHand);

  if ((caster_left && caster_left->currentSpell) || (caster_right && caster_right->currentSpell)) {
    return true;
  }
  return false;
}

auto place_at_me(RE::TESObjectREFR* target,
                 RE::TESForm* form,
                 std::uint32_t count,
                 bool force_persist,
                 bool initially_disabled) -> RE::TESObjectREFR*
{
  using FuncT = RE::TESObjectREFR*(RE::BSScript::Internal::VirtualMachine*,
                                   RE::VMStackID,
                                   RE::TESObjectREFR*,
                                   RE::TESForm*,
                                   std::uint32_t,
                                   bool,
                                   bool);

  RE::VMStackID frame = 0;

  const REL::Relocation<FuncT> func{RELOCATION_ID(55672, 0)};

  auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();

  return func(vm, frame, target, form, count, force_persist, initially_disabled);
}

auto initialization_hit_data(RE::HitData& hit_data,
                             RE::Actor* attacker,
                             RE::Actor* target,
                             RE::InventoryEntryData* weapon,
                             bool is_left) -> void
{
  const REL::Relocation<decltype(&initialization_hit_data)> func{RELOCATION_ID(42832, 0)};
  return func(hit_data, attacker, target, weapon, is_left);
}

auto apply_all_combat_spells_from_attack(RE::Character* attacker,
                                         RE::TESObjectWEAP* weapon,
                                         bool is_left,
                                         RE::Actor* target) -> void
{
  const REL::Relocation<decltype(&apply_all_combat_spells_from_attack)> func{RELOCATION_ID(37799, 0)};
  return func(attacker, weapon, is_left, target);
}

auto get_poison(RE::InventoryEntryData* _this) -> RE::AlchemyItem*
{
  const REL::Relocation<decltype(&get_poison)> func{RELOCATION_ID(15761, 0)};
  return func(_this);
}

auto poison_object(RE::InventoryEntryData* data, RE::AlchemyItem* poison, int count) -> void
{
  if (!data || !poison || count < 1) {
    return;
  }
  const REL::Relocation<decltype(&poison_object)> func{RELOCATION_ID(15786, 0)};
  return func(data, poison, count);
}

auto get_actor_value_max(RE::Actor* actor, const RE::ActorValue av) -> float
{
  if (!actor) {
    return 0.f;
  }
  return actor->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, av) + actor->GetPermanentActorValue(av);
}

auto get_weapon(const RE::Actor& actor, const bool is_left_hand, RE::TESObjectWEAP* fallback_weapon)
    -> RE::TESObjectWEAP*
{
  logger::debug("get weapon start"sv);
  const auto weapon = actor.GetEquippedObject(is_left_hand);
  if (!weapon) {
    return fallback_weapon;
  }

  const auto as_weapon = weapon->As<RE::TESObjectWEAP>();

  if (!as_weapon) {
    return fallback_weapon;
  }

  return as_weapon;
}

auto get_float_game_setting(const char* setting_name) -> std::optional<float>
{
  let game_settings = RE::GameSettingCollection::GetSingleton();
  if (!game_settings) {
    return std::nullopt;
  }

  let setting = game_settings->GetSetting(setting_name);
  if (!setting) {
    return std::nullopt;
  }

  return setting->data.f;
}

auto try_form_has_keyword(const RE::TESForm* form, const RE::BGSKeyword* keyword) -> bool
{
  if (!form || !keyword) {
    logger::debug("Null form or keyword"sv);
    return false;
  }

  const auto keyword_form = form->As<RE::BGSKeywordForm>();

  if (!keyword_form) {
    logger::debug("Null keyword_form: {}"sv, form->GetName());
    return false;
  }

  return keyword_form->HasKeyword(keyword);
}

auto try_keyword_form_has_keyword(const RE::BGSKeywordForm* form, const RE::BGSKeyword* keyword) -> bool
{
  if (!form || !keyword) {
    logger::debug("Null form or keyword"sv);
    return false;
  }

  return form->HasKeyword(keyword);
}

auto get_actor_value_owner_as_actor(RE::ActorValueOwner* actor_value_owner) -> RE::Actor*
{
  const auto actor = (RE::Actor*)((char*)actor_value_owner - 0xB0);
  if (!actor) {
    return nullptr;
  }
  return actor;
}

auto is_dual_wielding(const RE::Actor* actor) -> bool
{
  let left_hand = actor->GetEquippedEntryData(true);
  if (!left_hand || !left_hand->object) {
    return false;
  }
  let right_hand = actor->GetEquippedEntryData(false);
  if (!right_hand || !right_hand->object) {
    return false;
  }

  if (left_hand->object->IsWeapon() && right_hand->object->IsWeapon()) {
    return true;
  }

  return false;
}

auto is_bashing(const RE::Actor* attacker) -> bool
{
  if (!attacker) {
    return false;
  }

  let is_bashing_state = attacker->GetAttackState() == RE::ATTACK_STATE_ENUM::kBash;
  if (is_bashing_state) {
    return true;
  }

  let process = attacker->currentProcess;
  if (!process) {
    return false;
  }

  let high_process = process->high;
  if (!high_process) {
    return false;
  }

  let attack_data = high_process->attackData ? high_process->attackData.get() : nullptr;
  if (!attack_data) {
    return false;
  }

  return attack_data->data.flags.any(RE::AttackData::AttackFlag::kBashAttack);
}

auto string_split(const std::string& string_to_split, const char delimiter) -> std::vector<std::string>
{
  std::vector<std::string> ret;

  size_t first = 0;

  while (first < string_to_split.size()) {
    const auto second = string_to_split.find_first_of(delimiter, first);

    if (first != second)
      ret.emplace_back(string_to_split.substr(first, second - first));

    if (second == std::string_view::npos)
      break;

    first = second + 1;
  }

  return ret;
}

auto string_to_float(const std::string& input) -> std::optional<float>
{
  float float_result = 0.0f;

  const std::from_chars_result res = std::from_chars(input.data(), input.data() + input.size(), float_result);

  if (res.ec == std::errc::invalid_argument || res.ec == std::errc::result_out_of_range) {
    return std::nullopt;
  }
  return float_result;
}

auto string_to_int(const std::string& input) -> std::optional<int>
{
  int int_result = 0;

  const std::from_chars_result res = std::from_chars(input.data(), input.data() + input.size(), int_result);

  if (res.ec == std::errc::invalid_argument || res.ec == std::errc::result_out_of_range) {
    return std::nullopt;
  }
  return int_result;
}

auto play_sound(RE::BGSSoundDescriptorForm* sound, RE::Actor* actor) -> void
{
  logger::debug("Play sound"sv);
  using FuncT = void (*)(RE::TESForm*, int32_t, RE::NiPoint3*, RE::NiNode*);
  if (sound && actor) {
    logger::debug("Sound allow, not null"sv);
    auto actor_position = actor->GetPosition();
    auto actor_node = actor->GetFireNode();
    const REL::Relocation<FuncT> func{RELOCATION_ID(32301, 0)};
    return func(sound, 0, &actor_position, actor_node);
  }
}
} // namespace Reflyem::Core
