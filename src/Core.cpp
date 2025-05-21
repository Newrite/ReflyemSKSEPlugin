#include "Core.hpp"

#include "KiEnergyPower.hpp"

#include <random>

namespace Reflyem::Core {

static bool fake_block_frame = false;

auto set_fake_block_frame(const bool new_state) -> void
{
  fake_block_frame = new_state;
}

auto is_fake_block() -> bool
{
  return fake_block_frame;
}

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

auto get_random_float() -> float
{
  // StackOverflow:
  // https://stackoverflow.com/questions/5008804/generating-a-random-integer-from-a-range
  static std::random_device rd;                          // Only used once to initialise (seed) engine
  static std::mt19937 rng(rd());                         // Random-number engine used (Mersenne-Twister in this case)
  static std::uniform_int_distribution<int> uni(1, 100); // Guaranteed unbiased
  return static_cast<float>(uni(rng));
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

auto get_player_followers() -> std::vector<RE::Actor*> 
{
  std::vector<RE::Actor*> result;

  if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
    for (auto& actorHandle : processLists->highActorHandles) {
      if (auto actor = actorHandle.get(); actor && actor->IsPlayerTeammate()) {
        result.push_back(actor.get());
      }
    }
  }

  if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
    for (auto& actorHandle : processLists->middleHighActorHandles) {
      if (auto actor = actorHandle.get(); actor && actor->IsPlayerTeammate()) {
        result.push_back(actor.get());
      }
    }
  }

  if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
    for (auto& actorHandle : processLists->middleLowActorHandles) {
      if (auto actor = actorHandle.get(); actor && actor->IsPlayerTeammate()) {
        result.push_back(actor.get());
      }
    }
  }

  if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
    for (auto& actorHandle : processLists->lowActorHandles) {
      if (auto actor = actorHandle.get(); actor && actor->IsPlayerTeammate()) {
        result.push_back(actor.get());
      }
    }
  }

  return result;
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
  logi("DamageActorValue: {} {}"sv, av, value);
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
      neg_value += std::abs(active_effect->magnitude);
    } else {
      pos_value += std::abs(active_effect->magnitude);
    }
  }

  if (pos_value > 0.f || neg_value > 0.f) {
    return std::optional{pos_value - neg_value};
  }
  return std::nullopt;
}

auto get_effects_magnitude_mult(const std::vector<RE::ActiveEffect*>& effects) -> float
{

  auto mult = 1.f;

  if (effects.empty()) {
    return mult;
  }

  bool negative = false;

  for (const auto active_effect : effects) {
    if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) || !active_effect->effect ||
        !active_effect->effect->baseEffect) {
      continue;
    }

    const auto effect = active_effect->effect;
    const auto base_effect = effect->baseEffect;

    if (base_effect->data.flags.any(RE::EffectSetting::EffectSettingData::Flag::kDetrimental)) {
      negative = true;
    }

    mult = mult * std::abs(active_effect->magnitude);
  }

  if (negative) {
    mult = mult * -1.f;
  }

  return mult;
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

auto try_get_effect_by_keyword(RE::Actor* actor, const RE::BGSKeyword* keyword) -> RE::ActiveEffect*
{

  if (!actor || !keyword) {
    if (!keyword && actor) {
      logd("GetEffects: keyword null for: {}"sv, actor->GetDisplayFullName());
    }
    if (keyword && !actor) {
      logd("GetEffects: actor null for: {}"sv, keyword->formEditorID.c_str());
    }
    if (!actor && !keyword) {
      logd("GetEffect: Actor and Keyword is null"sv);
    }
    return nullptr;
  }

  auto active_effects = actor->GetActiveEffectList();
  if (!active_effects) {
    return nullptr;
  }

  for (auto active_effect : *active_effects) {
    if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) || !active_effect->effect ||
        !active_effect->effect->baseEffect) {
      continue;
    }

    const auto effect = active_effect->effect;

    if (const auto base_effect = effect->baseEffect; !base_effect->HasKeyword(keyword)) {
      continue;
    }

    return active_effect;
  }

  return nullptr;
}

auto is_concentraion_spell(const RE::TESForm* spell_form) -> bool
{
  if (!spell_form) {
    return false;
  }

  const auto spell = spell_form->As<RE::SpellItem>();

  if (!spell) {
    return false;
  }

  if (spell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration) {
    return true;
  }

  return false;
}

auto try_get_effects_by_keyword(RE::Actor* actor, const RE::BGSKeyword* keyword) -> std::vector<RE::ActiveEffect*>
{
  std::vector<RE::ActiveEffect*> effects = {};

  if (!actor || !keyword) {
    if (!keyword && actor) {
      logd("GetEffects: keyword null for: {}"sv, actor->GetDisplayFullName());
    }
    if (keyword && !actor) {
      logd("GetEffects: actor null for: {}"sv, keyword->formEditorID.c_str());
    }
    if (!actor && !keyword) {
      logd("GetEffect: Actor and Keyword is null"sv);
    }
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

    if (const auto base_effect = effect->baseEffect; !base_effect->HasKeyword(keyword)) {
      continue;
    }

    effects.push_back(active_effect);
  }

  return effects;
}

auto is_menu_allow() -> bool
{
  let ui = RE::UI::GetSingleton();
  let player = RE::PlayerCharacter::GetSingleton();
  if (player && ui &&
      (ui->GameIsPaused() || ui->IsMenuOpen(RE::TweenMenu::MENU_NAME) || ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME) ||
       ui->IsMenuOpen(RE::MagicMenu::MENU_NAME) || ui->IsMenuOpen(RE::InventoryMenu::MENU_NAME) ||
       ui->IsMenuOpen(RE::StatsMenu::MENU_NAME) || ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME) ||
       ui->IsMenuOpen(RE::Console::MENU_NAME) || ui->IsMenuOpen(RE::FaderMenu::MENU_NAME) ||
       ui->IsMenuOpen(RE::FavoritesMenu::MENU_NAME) || ui->IsMenuOpen(RE::SleepWaitMenu::MENU_NAME) ||
       ui->IsMenuOpen(RE::JournalMenu::MENU_NAME) || ui->IsMenuOpen(RE::BarterMenu::MENU_NAME) ||
       ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME) || ui->IsMenuOpen(RE::GiftMenu::MENU_NAME))) {
    return false;
  }
  return true;
}

auto get_second_av_weight_from_effect(const RE::ActiveEffect& active_effect) -> float
{
  if (!active_effect.effect || !active_effect.effect->baseEffect) {
    return 0.f;
  }
  return active_effect.effect->baseEffect->data.secondAVWeight;
}

auto get_second_av_from_effect(const RE::ActiveEffect& active_effect) -> RE::ActorValue
{
  if (!active_effect.effect || !active_effect.effect->baseEffect) {
    return RE::ActorValue::kNone;
  }

  if (!active_effect.effect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kDualValueModifier)) {
    return RE::ActorValue::kNone;
  }

  return active_effect.effect->baseEffect->data.secondaryAV;
}

auto get_av_from_effect(const RE::ActiveEffect& active_effect) -> RE::ActorValue
{
  if (!active_effect.effect || !active_effect.effect->baseEffect) {
    return RE::ActorValue::kNone;
  }

  if (active_effect.effect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kDualValueModifier)) {
    return active_effect.effect->baseEffect->data.primaryAV;
  }

  if (active_effect.effect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kValueModifier)) {
    return active_effect.effect->baseEffect->data.primaryAV;
  }

  if (active_effect.effect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kPeakValueModifier)) {
    return active_effect.effect->baseEffect->data.primaryAV;
  }

  return RE::ActorValue::kNone;
}

auto set_av_regen_delay(RE::AIProcess* process, RE::ActorValue av, float time) -> void
{
  if (!process || time <= 0.f) {
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

auto object_is_two_handed_weapon(RE::TESBoundObject* object) -> bool
{
  if (object && object->IsWeapon()) {
    if (let weapon = object->As<RE::TESObjectWEAP>(); weapon) {

      if (weapon->HasKeywordString("WeapTypeSword") || weapon->HasKeywordString("WeapTypeMace") ||
          weapon->HasKeywordString("WeapTypeWarAxe") || weapon->HasKeywordString("WeapTypeDagger")) {
        return false;
      }

      if (weapon->HasKeywordString("WeapTypeGreatsword") || weapon->HasKeywordString("WeapTypeWarhammer") ||
          weapon->HasKeywordString("WeapTypeBattleAxe")) {
        return true;
      }

      // return weapon->IsTwoHandedAxe() || weapon->IsTwoHandedSword() ||
      //        equip_slot_comparer(get_both_hand_equip_slot(), weapon->equipSlot);
    }
  }
  return false;
}

auto object_is_shield(RE::TESBoundObject* object) -> bool
{
  if (object && object->IsArmor()) {
    if (let armor = object->As<RE::TESObjectARMO>(); armor) {
      return armor->IsShield();
    }
  }
  return false;
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

auto cast(RE::SpellItem& spell, RE::Actor& target, RE::Actor& caster, CastExtraInfo extra, const float effect_mult)
    -> void
{

  if (extra == CastExtraInfo::kCastWithManacost) {
    let manacost = spell.CalculateMagickaCost(&caster);
    if (manacost > caster.GetActorValue(RE::ActorValue::kMagicka)) {
      flash_hud_meter(RE::ActorValue::kMagicka);
      return;
    }
    damage_actor_value(caster, RE::ActorValue::kMagicka, manacost);
  }
  if (spell.data.delivery == RE::MagicSystem::Delivery::kSelf) {
    caster.GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
        ->CastSpellImmediate(&spell, true, &caster, effect_mult, false, 0.0f, &caster);
  } else {
    caster.GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
        ->CastSpellImmediate(&spell, true, &target, effect_mult, false, 0.0f, &caster);
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

auto get_either_hand_equip_slot() -> RE::BGSEquipSlot*
{
  let data_handler = RE::TESDataHandler::GetSingleton();
  if (!data_handler) {
    return nullptr;
  }
  return data_handler->LookupForm<RE::BGSEquipSlot>(0x13F44, "Skyrim.esm");
}

inline constexpr std::array<std::string_view, 86> baseScripts = {"Action"sv,
                                                                 "Activator"sv,
                                                                 "ActiveMagicEffect"sv,
                                                                 "Actor"sv,
                                                                 "ActorBase"sv,
                                                                 "ActorValueInfo"sv,
                                                                 "Alias"sv,
                                                                 "Ammo"sv,
                                                                 "Apparatus"sv,
                                                                 "Armor"sv,
                                                                 "ArmorAddon"sv,
                                                                 "Art"sv,
                                                                 "AssociationType"sv,
                                                                 "Book"sv,
                                                                 "Cell"sv,
                                                                 "Class"sv,
                                                                 "ColorForm"sv,
                                                                 "CombatStyle"sv,
                                                                 "ConstructibleObject"sv,
                                                                 "Container"sv,
                                                                 "Debris"sv,
                                                                 "DefaultObjectManager"sv,
                                                                 "Door"sv,
                                                                 "EffectShader"sv,
                                                                 "Enchantment"sv,
                                                                 "EncounterZone"sv,
                                                                 "EquipSlot"sv,
                                                                 "Explosion"sv,
                                                                 "Faction"sv,
                                                                 "Flora"sv,
                                                                 "FootstepSet"sv,
                                                                 "Form"sv,
                                                                 "FormList"sv,
                                                                 "Furniture"sv,
                                                                 "GlobalVariable"sv,
                                                                 "Hazard"sv,
                                                                 "HeadPart"sv,
                                                                 "Idle"sv,
                                                                 "ImageSpaceModifier"sv,
                                                                 "ImpactDataSet"sv,
                                                                 "Ingredient"sv,
                                                                 "Key"sv,
                                                                 "Keyword"sv,
                                                                 "LeveledActor"sv,
                                                                 "LeveledItem"sv,
                                                                 "LeveledSpell"sv,
                                                                 "Light"sv,
                                                                 "LightingTemplate"sv,
                                                                 "Location"sv,
                                                                 "LocationAlias"sv,
                                                                 "LocationRefType"sv,
                                                                 "MagicEffect"sv,
                                                                 "MaterialObject"sv,
                                                                 "Message"sv,
                                                                 "MiscObject"sv,
                                                                 "MusicType"sv,
                                                                 "ObjectReference"sv,
                                                                 "Outfit"sv,
                                                                 "PROJECTILE"sv,
                                                                 "Package"sv,
                                                                 "Perk"sv,
                                                                 "Potion"sv,
                                                                 "Quest"sv,
                                                                 "Race"sv,
                                                                 "ReferenceAlias"sv,
                                                                 "SPELL"sv,
                                                                 "STATIC"sv,
                                                                 "Scene"sv,
                                                                 "Scroll"sv,
                                                                 "ShaderParticleGeometry"sv,
                                                                 "Shout"sv,
                                                                 "SoulGem"sv,
                                                                 "Sound"sv,
                                                                 "SoundCategory"sv,
                                                                 "SoundDescriptor"sv,
                                                                 "TalkingActivator"sv,
                                                                 "TextureSet"sv,
                                                                 "Topic"sv,
                                                                 "TopicInfo"sv,
                                                                 "TreeObject"sv,
                                                                 "VisualEffect"sv,
                                                                 "VoiceType"sv,
                                                                 "WEAPON"sv,
                                                                 "Weather"sv,
                                                                 "WordOfPower"sv,
                                                                 "WorldSpace"};

static bool check_script(const RE::BSScript::Internal::VirtualMachine& a_vm, const RE::VMHandle a_handle)
{
  // if (!a_scriptName.empty()) {
  //   RE::BSTSmartPointer<RE::BSScript::Object> object;
  //   return a_vm.FindBoundObject(a_handle, a_scriptName.c_str(), object) && object;
  // }
  RE::BSSpinLockGuard locker(a_vm.attachedScriptsLock);
  if (const auto it = a_vm.attachedScripts.find(a_handle); it != a_vm.attachedScripts.end()) {
    return std::ranges::any_of(it->second, [&](const auto& script) {
      auto typeInfo = script ? script->GetTypeInfo() : nullptr;
      return typeInfo && std::find(baseScripts.begin(), baseScripts.end(), typeInfo->name) ==
                             baseScripts.end(); // is not a base script
    });
  }
  return false;
}

bool is_script_attached(const RE::TESForm* a_form)
{

  logd("script check");

  const auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
  if (!vm) {
    return false;
  }

  const auto policy = vm->GetObjectHandlePolicy();
  if (!policy) {
    return false;
  }

  const auto check_script = [&](const RE::TESForm* b_form) {
    const auto handle = policy->GetHandleForObject(static_cast<RE::VMTypeID>(b_form->GetFormType()), b_form);
    if (handle == policy->EmptyHandle()) {
      return false;
    }
    return Core::check_script(*vm, handle);
  };

  auto result = check_script(a_form);
  if (!result) {
    const auto ref = a_form->As<RE::TESObjectREFR>();
    const auto base = ref ? ref->GetObjectReference() : nullptr;
    if (base) {
      result = check_script(base);
    }
  }

  logd("script check end");

  return result;
}

auto get_gem_keyword() -> RE::BGSKeyword*
{
  let data_handler = RE::TESDataHandler::GetSingleton();
  if (!data_handler) {
    return nullptr;
  }
  return data_handler->LookupForm<RE::BGSKeyword>(0x914ED, "Skyrim.esm");
}

auto get_magic_disallow_keyword() -> RE::BGSKeyword*
{
  let data_handler = RE::TESDataHandler::GetSingleton();
  if (!data_handler) {
    return nullptr;
  }
  return data_handler->LookupForm<RE::BGSKeyword>(0xC27BD, "Skyrim.esm");
}

auto get_soul_gem_keyword() -> RE::BGSKeyword*
{
  let data_handler = RE::TESDataHandler::GetSingleton();
  if (!data_handler) {
    return nullptr;
  }
  return data_handler->LookupForm<RE::BGSKeyword>(0x937A3, "Skyrim.esm");
}

auto get_location_ref_type_boss() -> RE::BGSLocationRefType*
{
  let data_handler = RE::TESDataHandler::GetSingleton();
  if (!data_handler) {
    return nullptr;
  }
  return data_handler->LookupForm<RE::BGSLocationRefType>(0x130F7, "Skyrim.esm");
}

auto is_soul_gem_form(RE::TESBoundObject* object) -> bool
{
  if (object) {
    let misc = object->As<RE::TESSoulGem>();
    if (misc) {
      logd("is soul gem");
      return true;
    }
  }
  logd("Not soul gem");
  return false;
}

auto is_furniture_form(RE::TESBoundObject* object) -> bool
{
  if (object) {
    let misc = object->As<RE::TESFurniture>();
    if (misc) {
      logd("is furniture");
      return true;
    }
  }
  logd("Not furniture");
  return false;
}

auto get_ghost_shader() -> RE::TESEffectShader*
{
  let data_handler = RE::TESDataHandler::GetSingleton();
  if (!data_handler) {
    return nullptr;
  }
  return data_handler->LookupForm<RE::TESEffectShader>(0x3B6CB, "Skyrim.esm");
}

auto get_vendor_item_key() -> RE::BGSKeyword*
{
  let data_handler = RE::TESDataHandler::GetSingleton();
  if (!data_handler) {
    return nullptr;
  }
  return data_handler->LookupForm<RE::BGSKeyword>(0x914EF, "Skyrim.esm");
}

auto is_misc_form(RE::TESBoundObject* object) -> bool
{
  if (object) {
    let misc = object->As<RE::TESObjectMISC>();
    if (misc) {
      logd("is misc");
      return true;
    }
  }
  logd("Not misc");
  return false;
}

auto get_effect_assoc_item_bound_weapon(const RE::EffectSetting* effect) -> RE::TESObjectWEAP*
{
  if (effect && effect->GetArchetype() == RE::EffectArchetype::kBoundWeapon) {
    let assoc_item = effect->data.associatedForm;
    if (assoc_item) {
      return assoc_item->As<RE::TESObjectWEAP>();
    }
  }
  return nullptr;
}

auto get_effect_assoc_item_bound_weapon(const RE::MagicItem* magic_item) -> RE::TESObjectWEAP*
{
  if (!magic_item) {
    logi("AssocBound: magic item null");
    return nullptr;
  }

  if (magic_item->effects.empty()) {
    logi("AssocBound: effects empty");
    return nullptr;
  }

  for (auto effect : magic_item->effects) {
    if (effect && effect->baseEffect) {
      logi("AssocBound: check effect: {}", effect->baseEffect->GetFullName());
      let bound_weapon = get_effect_assoc_item_bound_weapon(effect->baseEffect);
      if (bound_weapon) {
        return bound_weapon;
      }
    }
  }

  return nullptr;
}

auto is_container_form(RE::TESBoundObject* object) -> bool
{
  if (object) {
    let misc = object->As<RE::TESObjectCONT>();
    if (misc) {
      logd("is container");
      return true;
    }
  }
  logd("no container");
  return false;
}

auto is_activator_form(RE::TESBoundObject* object) -> bool
{
  if (object) {
    let misc = object->As<RE::TESObjectACTI>();
    if (misc) {
      logd("is activator");
      return true;
    }
  }
  logd("no activator");
  return false;
}

auto is_pickup_sound_form(RE::TESBoundObject* object) -> bool
{
  if (object) {
    let misc = object->As<RE::BGSPickupPutdownSounds>();
    if (misc) {
      logd("is pickup sound");
      return true;
    }
  }
  logd("no pickup sound");
  return false;
}

auto is_food_form(RE::TESBoundObject* object) -> bool
{
  if (object) {
    let alchemy_item = object->As<RE::AlchemyItem>();
    if (alchemy_item) {
      logd("is alchemy");
      return alchemy_item->IsFood();
    }
  }
  logd("Not food");
  return false;
}

auto get_full_damage_from_hit_data(const RE::HitData* hit_data) -> std::optional<float>
{
  if (!hit_data || !hit_data->weapon) {
    return std::nullopt;
  }

  const auto ni_aggressor = hit_data->aggressor.get();
  if (!ni_aggressor) {
    return std::nullopt;
  }
  const auto aggressor = ni_aggressor.get();
  if (!aggressor) {
    return std::nullopt;
  }

  const auto ni_target = hit_data->target.get();
  if (!ni_target) {
    return std::nullopt;
  }
  const auto target = ni_target.get();
  if (!target) {
    return std::nullopt;
  }

  auto damage_resist = 1.f;
  RE::BGSEntryPoint::HandleEntryPoint(RE::BGSEntryPoint::ENTRY_POINT::kModIncomingDamage,
                                      target,
                                      aggressor,
                                      hit_data->weapon,
                                      std::addressof(damage_resist));

  return hit_data->physicalDamage * damage_resist;
}

auto is_ingredient_form(RE::TESBoundObject* object) -> bool
{
  if (object) {
    let ingredient_item = object->As<RE::IngredientItem>();
    if (ingredient_item) {
      logd("is ingredient");
      return true;
    }
  }
  logd("Not ingredient");
  return false;
}

auto is_key_form(RE::TESBoundObject* object) -> bool
{
  if (object) {
    let key_item = object->As<RE::TESKey>();
    if (key_item) {
      logd("is key");
      return true;
    }
  }
  logd("Not key");
  return false;
}

auto get_ore_ingot_keyword() -> RE::BGSKeyword*
{
  let data_handler = RE::TESDataHandler::GetSingleton();
  if (!data_handler) {
    return nullptr;
  }
  return data_handler->LookupForm<RE::BGSKeyword>(0x914EC, "Skyrim.esm");
}

auto get_both_hand_equip_slot() -> RE::BGSEquipSlot*
{
  let data_handler = RE::TESDataHandler::GetSingleton();
  if (!data_handler) {
    return nullptr;
  }
  return data_handler->LookupForm<RE::BGSEquipSlot>(0x13F45, "Skyrim.esm");
}

auto get_voice_equip_slot() -> RE::BGSEquipSlot*
{
  let object_manager = RE::BGSDefaultObjectManager::GetSingleton();
  if (!object_manager) {
    return nullptr;
  }
  return object_manager->objects[22]->As<RE::BGSEquipSlot>();
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

auto cast_on_handle_formlists(RE::BGSListForm* keywords,
                              RE::BGSListForm* spells,
                              RE::Actor& caster,
                              RE::Actor& target,
                              CastExtraInfo extra,
                              const float effect_mult) -> void
{
  if (!keywords || !spells) {
    return;
  }

  const auto length_kw = keywords->forms.size();
  const auto length_sp = spells->forms.size();
  logger::debug("LKW LSP: {} {}", length_kw, length_sp);
  for (std::uint32_t index = 0u; index < length_kw && index < length_sp; index++) {
    logger::debug("LKW LSP index: {}", index);
    cast_on_handle(keywords->forms[index], spells->forms[index], target, caster, extra, effect_mult);
  }
}

auto cast_on_handle(RE::TESForm* keyword,
                    RE::TESForm* spell,
                    RE::Actor& target,
                    RE::Actor& caster,
                    CastExtraInfo extra,
                    const float effect_mult) -> void
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

  letr config = Config::get_singleton();

  auto caster_ptr = &caster;
  auto target_ptr = &target;

  if (config.cast_on_caster_swap() && target_ptr && caster_ptr &&
      try_form_has_keyword(spell_ptr, config.cast_on_caster_swap())) {
    const auto* temp_caster = &caster;
    caster = target;
    target = *temp_caster;
  }

  logger::debug("Cast_On_Handle before call has_absolute_keyword");
  if (!allow_cast && !has_absolute_keyword(caster, *keyword_ptr)) {
    return;
  }

  if (try_form_has_keyword(spell, config.cast_on_is_cost())) {
    extra = CastExtraInfo::kCastWithManacost;
  }

  logger::debug("Cast_On_Handle before call cast");
  cast(*spell_ptr, target, caster, extra, effect_mult);
}

auto console_log(std::string&& str) -> void
{
  logi("{}", str);
  if (let console = RE::ConsoleLog::GetSingleton()) {
    console->Print(str.c_str());
  }
}

auto cast(RE::SpellItem* spell,
          RE::TESObjectREFR* target,
          RE::TESObjectREFR* caster,
          CastExtraInfo extra,
          const float effect_mult) -> void
{
  let caster_actor = caster->As<RE::Actor>();
  if (caster_actor && extra == CastExtraInfo::kCastWithManacost) {
    let manacost = spell->CalculateMagickaCost(caster_actor);
    if (manacost > caster_actor->GetActorValue(RE::ActorValue::kMagicka)) {
      flash_hud_meter(RE::ActorValue::kMagicka);
      return;
    }
    damage_actor_value(*caster_actor, RE::ActorValue::kMagicka, manacost);
  }
  if (spell->data.delivery == RE::MagicSystem::Delivery::kSelf) {
    if (caster) {
      caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
          ->CastSpellImmediate(spell, true, caster, effect_mult, false, 0.0f, caster->As<RE::Actor>());
    }
  } else {
    if (caster) {
      caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
          ->CastSpellImmediate(spell, true, target, effect_mult, false, 0.0f, caster->As<RE::Actor>());
    }
  }
}

auto cast_on_handle(RE::TESForm* keyword,
                    RE::TESForm* spell,
                    RE::TESObjectREFR* target,
                    RE::TESObjectREFR* caster,
                    CastExtraInfo extra,
                    const float effect_mult) -> void
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

  letr config = Config::get_singleton();

  if (config.cast_on_caster_swap() && try_form_has_keyword(spell_ptr, config.cast_on_caster_swap())) {
    auto* temp_caster = caster;
    caster = target;
    target = temp_caster;
  }

  if (!caster) {
    return;
  }

  logger::debug("Cast_On_Handle before call has_absolute_keyword");
  if (!allow_cast && !try_has_absolute_keyword(caster->As<RE::Actor>(), keyword_ptr)) {
    return;
  }

  if (try_form_has_keyword(spell, config.cast_on_is_cost())) {
    extra = CastExtraInfo::kCastWithManacost;
  }

  logger::debug("Cast_On_Handle before call cast");
  cast(spell_ptr, target, caster, extra, effect_mult);
}

auto get_actor_value_percent(RE::Actor* actor, const RE::ActorValue av) -> float
{

  if (!actor) {
    return 0.f;
  }

  switch (av) {
    case RE::ActorValue::kHealth: {
      let max_value = get_actor_value_max(actor, av);
      let current_value = actor->GetActorValue(av);
      return (current_value / (max_value / 100.f)) / 100.f;
    }
    case RE::ActorValue::kMagicka: {
      let max_value = get_actor_value_max(actor, av);
      let current_value = actor->GetActorValue(av);
      return (current_value / (max_value / 100.f)) / 100.f;
    }
    case RE::ActorValue::kStamina: {
      let max_value = get_actor_value_max(actor, av);
      let current_value = actor->GetActorValue(av);
      return (current_value / (max_value / 100.f)) / 100.f;
    }
  }

  return 0.f;
}

auto direct_apply_spell_on_target(RE::ActorMagicCaster* actor_caster,
                                  RE::MagicItem* magic_item,
                                  RE::TESObjectREFR* target,
                                  RE::TESBoundObject* object,
                                  bool a5) -> bool
{
  letr config = Config::get_singleton();

  if (magic_item && actor_caster && actor_caster->actor && magic_item->As<RE::SpellItem>()) {
    if (config.ki_energy_power().enable()) {
      let result = KiEnergyPower::use_ki_energy(actor_caster->actor, magic_item->As<RE::SpellItem>());
      if (result == KiEnergyPower::kOk) {
        actor_caster->CastSpellImmediate(
            magic_item->As<RE::SpellItem>(), false, nullptr, 1.f, false, 0.f, actor_caster->actor);
        return true;
      }
      if (result == KiEnergyPower::kFail) {
        return false;
      }
    }
  }

  if (config.misc_fixes().cast_ability_magicka_cost() && actor_caster && magic_item && actor_caster->actor &&
      magic_item->GetChargeTime() > 0.f) {

    auto cost_av = RE::ActorValue::kMagicka;

    if (config.special_techniques().enable() && config.special_techniques().stamina_cost_spell_keyword() &&
        magic_item->HasKeyword(config.special_techniques().stamina_cost_spell_keyword())) {
      cost_av = RE::ActorValue::kStamina;
    }

    let cost = magic_item->CalculateMagickaCost(actor_caster->actor);
    if (cost > actor_caster->actor->GetActorValue(cost_av)) {

      if (actor_caster->actor->IsPlayerRef()) {
        play_sound_base(config.caster_additions().mag_fail_sound(), actor_caster->actor);
        flash_hud_meter(cost_av);
      }

      return false;
    }

    damage_actor_value(*actor_caster->actor, cost_av, cost);
  }
  const REL::Relocation<decltype(&direct_apply_spell_on_target)> func{RELOCATION_ID(33631, 0)};
  return func(actor_caster, magic_item, target, object, a5);
}

auto cast_on_handle_formlists(RE::BGSListForm* keywords,
                              RE::BGSListForm* spells,
                              RE::Actor* caster,
                              RE::TESObjectREFR* target,
                              CastExtraInfo extra,
                              const float effect_mult) -> void
{
  if (!keywords || !spells) {
    return;
  }

  const auto length_kw = keywords->forms.size();
  const auto length_sp = spells->forms.size();
  logger::debug("LKW LSP: {} {}", length_kw, length_sp);
  for (std::uint32_t index = 0u; index < length_kw && index < length_sp; index++) {
    logger::debug("LKW LSP index: {}", index);
    cast_on_handle(keywords->forms[index], spells->forms[index], target, caster, extra, effect_mult);
  }
}

auto is_blocking(RE::Actor* actor) -> bool
{

  auto result = false;

  if (!actor) {
    return result;
  }

  if (actor->IsPlayerRef() && fake_block_frame) {
    return true;
  }

  result = actor->IsBlocking();

  letr config = Config::get_singleton();
  if (config.parry_bash().enable() && config.parry_bash().enable_bash_block()) {
    let allow_bash_block = config.parry_bash().bash_block_keyword()
                               ? try_has_absolute_keyword(actor, config.parry_bash().bash_block_keyword())
                               : true;
    result = actor->IsBlocking() || (is_bashing(actor) && allow_bash_block);
  }

  if (!result && config.magic_blocker().enable() &&
      try_has_absolute_keyword(actor, config.magic_blocker().magic_blocker_keyword())) {
    result = true;
  }

  if (!result && config.special_techniques().enable() &&
      try_has_absolute_keyword(actor, config.special_techniques().active_block_keyword())) {
    result = true;
  }

  return result;
}


auto vector_keyword_form_exist(std::vector<RE::BGSKeyword*>* vector, const RE::BGSKeyword* value) -> bool
{

  if (!vector) {
    return false;
  }
  
  for (auto* vector_value : *vector) {
    if (vector_value && vector_value == value) {
      return true;
    }
  }

  return false;
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

auto get_shooter_ref_from_projectile(const RE::Projectile* projectile) -> RE::TESObjectREFR*
{

  if (!projectile) {
    return nullptr;
  }
  
  let shooter_ref = projectile->shooter;
  if (shooter_ref) {
    let ni_shooter = shooter_ref.get();
    if (ni_shooter) {
      let shooter_object = ni_shooter.get();
      if (shooter_object) {
        return shooter_object;
      }
    }
  }
  return nullptr;
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

auto weapon_fire(RE::TESObjectWEAP* weapon,
                 RE::TESObjectREFR* fire_source,
                 RE::TESAmmo* ammo,
                 RE::EnchantmentItem* enchantment,
                 RE::AlchemyItem* poison) -> void
{
  const REL::Relocation<decltype(&weapon_fire)> func{RELOCATION_ID(17693, 0)};
  return func(weapon, fire_source, ammo, enchantment, poison);
}

auto bow_fire(RE::Actor* fire_actor) -> void
{

  if (!fire_actor) {
    logi("Bow fire: actor null");
    return;
  }

  let right_entry = fire_actor->GetEquippedEntryData(false);
  let have_ammo = fire_actor->GetCurrentAmmo();
  if (!have_ammo) {
    logi("Bow fire: ammo null");
    return;
  }

  if (right_entry && right_entry->object && right_entry->object->IsWeapon()) {
    logi("Bow fire: is entry");
    let weapon = right_entry->object->As<RE::TESObjectWEAP>();
    if (weapon && weapon->weaponData.animationType == RE::WEAPON_TYPE::kBow) {
      logi("Bow fire: weapon fire entry");
      return weapon_fire(weapon, fire_actor, have_ammo, right_entry->GetEnchantment(), get_poison(right_entry));
    }
  }

  let right_object = fire_actor->GetEquippedObject(false);
  if (right_object && right_object->As<RE::TESObjectWEAP>()) {
    logi("Bow fire: is object");
    let weapon = right_object->As<RE::TESObjectWEAP>();
    if (weapon && weapon->weaponData.animationType == RE::WEAPON_TYPE::kBow) {
      logi("Bow fire: weapon fire object");
      return weapon_fire(weapon, fire_actor, have_ammo, weapon->formEnchanting, nullptr);
    }
  }

  logi("Bow fire: end");
}

auto has_absolute_keyword(RE::Actor& actor, RE::BGSKeyword& keyword) -> bool
{
  logger::debug("Before check keywords in has_absolute_keyword");
  const auto result = actor.HasKeyword(&keyword) || actor_has_active_mgef_with_keyword(actor, keyword) ||
                      worn_has_keyword(&actor, &keyword);
  logger::debug("After check keywords in has_absolute_keyword, result: {}", result);
  return result;
}

auto magic_item_has_keyword(RE::MagicItem* magic_item, const RE::BGSKeyword* keyword) -> bool
{

  if (!magic_item || !keyword) {
    return false;
  }

  for (const auto effect : magic_item->effects) {
    if (effect && effect->baseEffect && effect->baseEffect->HasKeyword(keyword)) {
      return true;
    }
  }

  return false;
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

auto is_casting_actor(RE::Actor& character) -> bool
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

auto is_bow_or_crossbow_attack_actor(const RE::Actor& character) -> bool
{
  if (character.IsDead() || !character.IsAttacking()) {
    return false;
  }

  let weapon = character.GetEquippedObject(false);
  if (weapon && weapon->As<RE::TESObjectWEAP>()) {
    let w = weapon->As<RE::TESObjectWEAP>();
    if (w->IsCrossbow() || w->IsBow()) {
      return true;
    }
  }

  return false;
}

auto get_equipped_shield(const RE::Actor* actor) -> RE::TESObjectARMO*
{
  if (!actor) {
    return nullptr;
  }

  let left_object = actor->GetEquippedObject(true);
  if (left_object && !left_object->IsArmor()) {
    return nullptr;
  }

  if (left_object) {
    return left_object->As<RE::TESObjectARMO>();
  }

  return nullptr;
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

auto actor_is_on_flying_mount(RE::Actor* actor) -> bool
{
  const REL::Relocation<decltype(&actor_is_on_flying_mount)> func{RELOCATION_ID(36877, 0)};
  return func(actor);
}

auto player_start_shout(RE::PlayerCharacter* player, RE::TESShout* shout) -> bool
{
  const REL::Relocation<decltype(&player_start_shout)> func{RELOCATION_ID(39542, 0)};
  return func(player, shout);
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

auto initialization_hit_proj_data(RE::HitData& hit_data,
                                  RE::Actor* attacker,
                                  RE::Actor* target,
                                  RE::Projectile* projectile) -> void
{
  const REL::Relocation<decltype(&initialization_hit_proj_data)> func{RELOCATION_ID(42833, 0)};
  return func(hit_data, attacker, target, projectile);
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

auto calculate_block_cost(float shield_armor_or_weapon_damage,
                          float block_av,
                          float shield_perks_av,
                          bool block_with_weapon,
                          bool block_with_unarmed,
                          bool is_power_attack) -> float
{
  const REL::Relocation<decltype(&calculate_block_cost)> func{RELOCATION_ID(25859, 0)};
  return func(
      shield_armor_or_weapon_damage, block_av, shield_perks_av, block_with_weapon, block_with_unarmed, is_power_attack);
}

auto set_global_time_multiplier(float mult, bool a3) -> void
{
  const REL::Relocation<void*> time_manager{REL::ID(523657)};
  const REL::Relocation<void (*)(void*, float, bool)> func{RELOCATION_ID(66988, 0)};
  return func(time_manager.get(), mult, a3);
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
    logd("Null form or keyword"sv);
    return false;
  }

  const auto keyword_form = form->As<RE::BGSKeywordForm>();

  if (!keyword_form) {
    logd("Null keyword_form: {}"sv, form->GetName());
    return false;
  }

  logd("keyword_form: {}"sv, keyword->GetName());
  return keyword_form->HasKeyword(keyword);
}

auto try_keyword_form_has_keyword(const RE::BGSKeywordForm* form, const RE::BGSKeyword* keyword) -> bool
{
  if (!form || !keyword) {
    logd("Null form or keyword"sv);
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

  if (attacker->GetAttackState() == RE::ATTACK_STATE_ENUM::kBash) {
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

inline int sound_helper_a(void* manager, RE::BSSoundHandle* a2, int a3, int a4) // sub_140BEEE70
{
  using func_t = decltype(&sound_helper_a);
  REL::Relocation<func_t> func{RELOCATION_ID(66401, 67663)};
  return func(manager, a2, a3, a4);
}

inline void sound_helper_b(RE::BSSoundHandle* a1, RE::NiAVObject* source_node) // sub_140BEDB10
{
  using func_t = decltype(&sound_helper_b);
  REL::Relocation<func_t> func{RELOCATION_ID(66375, 67636)};
  return func(a1, source_node);
}

inline char __fastcall sound_helper_c(RE::BSSoundHandle* a1) // sub_140BED530
{
  using func_t = decltype(&sound_helper_c);
  REL::Relocation<func_t> func{RELOCATION_ID(66355, 67616)};
  return func(a1);
}

inline char set_sound_position(RE::BSSoundHandle* a1, float x, float y, float z)
{
  using func_t = decltype(&set_sound_position);
  REL::Relocation<func_t> func{RELOCATION_ID(66370, 67631)};
  return func(a1, x, y, z);
}

auto formlist_revert(RE::BGSListForm* form_list) -> void
{
  using func_t = decltype(&formlist_revert);
  REL::Relocation<func_t> func{RELOCATION_ID(20469, 0)};
  return func(form_list);
}

auto play_sound_base(RE::BGSSoundDescriptorForm* sound, RE::Actor* actor) -> void
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

void play_sound_with_volume(RE::Actor* a, RE::BGSSoundDescriptorForm* a_descriptor, float a_volumeOverride)
{

  RE::BSSoundHandle handle;
  handle.soundID = static_cast<uint32_t>(-1);
  handle.assumeSuccess = false;
  *(uint32_t*)&handle.state = 0;

  auto audio_manager = RE::BSAudioManager::GetSingleton();
  if (!audio_manager || !a_descriptor) {
    loge("Play sound with volume null args");
    return;
  }

  sound_helper_a(audio_manager, &handle, a_descriptor->GetFormID(), 16);
  if (set_sound_position(&handle, a->data.location.x, a->data.location.y, a->data.location.z)) {
    handle.SetVolume(a_volumeOverride);
    sound_helper_b(&handle, a->Get3D());
    sound_helper_c(&handle);
  }
}

void play_sound(RE::BGSSoundDescriptorForm* sound, RE::Actor* a)
{
  letr config = Config::get_singleton();
  play_sound_with_volume(a, sound, config.volume_override());
}
} // namespace Reflyem::Core
