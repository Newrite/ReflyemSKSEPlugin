#include "Hooks.hpp"
#include "Core.hpp"
#include "plugin/AnimationEventHandler.hpp"
#include "plugin/CastOnBlock.hpp"
#include "plugin/CastOnHit.hpp"
#include "plugin/CasterAdditions.hpp"
#include "plugin/CheatDeath.hpp"
#include "plugin/Crit.hpp"
#include "plugin/DeathLoot.hpp"
#include "plugin/EquipLoad.hpp"
#include "plugin/ItemLimit.hpp"
#include "plugin/MagicShield.hpp"
#include "plugin/MagicWepon.hpp"
#include "plugin/ParryBash.hpp"
#include "plugin/PetrifiedBlood.hpp"
#include "plugin/PotionsDrintLimit.hpp"
#include "plugin/ResistTweaks.hpp"
#include "plugin/ResourceManager.hpp"
#include "plugin/SoulLink.hpp"
#include "plugin/SpeedCasting.hpp"
#include "plugin/SpeedMultCap.hpp"
#include "plugin/StaminaShield.hpp"
#include "plugin/TKDodge.hpp"
#include "plugin/TimingBlock.hpp"
#include "plugin/Vampirism.hpp"

namespace Hooks
{

static float player_last_delta = 0.f;
static bool object_manager_dumped = false;


auto dump_default_object_manager() -> void
{
  let object_manager = RE::BGSDefaultObjectManager::GetSingleton();

  if (!object_manager)
    {
      object_manager_dumped = true;
      logi("Object manager is null"sv);
    }

  for (uint32_t index : views::iota(0u, RE::DEFAULT_OBJECTS::kTotal))
    {
      if (!object_manager->objectInit[index]) { continue; }

      let form = object_manager->objects[index];

      if (!form) { continue; }

      logi(
          "Object[{}] : {} | {} | {}",
          index,
          form->GetFormType(),
          form->GetName(),
          form->GetObjectTypeName());
    }

  object_manager_dumped = true;
}

auto update_actor(RE::Character& character, const float delta, const Reflyem::Config& config)
    -> void
{
  logger::debug("update actor"sv);

  auto& actors_cache = Reflyem::Core::ActorsCache::get_singleton();
  auto& actor_data = actors_cache.get_or_add(character.formID).get();

  actor_data.last_delta_update(player_last_delta);
  actor_data.update_handler(player_last_delta);

  if (config.timing_block().enable())
    {
      if (character.IsBlocking())
        {
          actor_data.timing_block_timer(Reflyem::TimingBlock::BLOCK_DELAY);
          actor_data.timing_block_flag(true);
        }
      else { actor_data.timing_block_flag(false); }
    }

  if (actor_data.update_50_timer() <= 0.f)
    {
      logger::debug("update actor map50 tick"sv);
      actor_data.update_50_timer_refresh();
    }

  if (actor_data.update_100_timer() <= 0.f)
    {
      logger::debug("update actor map100 tick"sv);

      actor_data.update_100_timer_refresh();

      if (config.resource_manager().enable())
        {
          Reflyem::ResourceManager::update_actor(character, delta, config);
          if (config.resource_manager().weapon_spend_enable())
            {
              Reflyem::ResourceManager::ranged_spend_handler(character, config);
            }
        }

      if (config.caster_additions().enable())
        {
          Reflyem::CasterAdditions::on_update_actor(character, delta, config);
        }

      if (config.equip_load().enable()) { Reflyem::EquipLoad::update_actor(character, config); }
    }

  if (actor_data.update_1000_timer() <= 0.f)
    {
      logger::debug("update actor map1000 tick"sv);

      actor_data.update_1000_timer_refresh();
      actor_data.last_tick_count_refresh();

      let caster_right =
          character.magicCasters[static_cast<int>(RE::MagicSystem::CastingSource::kRightHand)];
      let caster_left =
          character.magicCasters[static_cast<int>(RE::MagicSystem::CastingSource::kLeftHand)];
      let equip_spell_right =
          character.selectedSpells[static_cast<int>(RE::MagicSystem::CastingSource::kRightHand)];
      let equip_spell_left =
          character.selectedSpells[static_cast<int>(RE::MagicSystem::CastingSource::kLeftHand)];
      let caster_info = [](const RE::ActorMagicCaster* caster)
      {
        if (!caster)
          {
            logi("Caster null"sv);
            return;
          }

        logi(
            "SpellName: {} Cost: {}"sv,
            caster->currentSpell ? caster->currentSpell->fullName.c_str() : "null spell"sv,
            caster->currentSpellCost);
      };

      let spell_info = [](const RE::MagicItem* magic_item)
      {
        if (!magic_item)
          {
            logi("Spell null"sv);
            return;
          }

        logi("EquippedSpellName: {}"sv, magic_item->fullName.c_str());
      };

      // caster_info(caster_left);
      // caster_info(caster_right);
      // spell_info(equip_spell_left);
      // spell_info(equip_spell_right);

      if (config.petrified_blood().enable() && config.petrified_blood().magick())
        {
          Reflyem::PetrifiedBlood::character_update(character, delta, config, actor_data);
        }

      if (config.resource_manager().enable())
        {
          Reflyem::ResourceManager::GMST::game_settings_handler(config);
        }

      if (config.speed_casting().enable() && character.IsPlayerRef())
        {
          Reflyem::SpeedCasting::on_update_actor(character, delta, config);
        }

      if (character.IsPlayerRef() && config.item_limit().enable())
        {
          Reflyem::ItemLimit::update_actor(character, delta, config);
        }
    }
}

auto on_modify_actor_value(
    RE::ValueModifierEffect* this_,
    RE::Actor* actor,
    float& value,
    RE::ActorValue av) -> void
{
  auto& config = Reflyem::Config::get_singleton();

  if (config.magick_crit().enable())
    {
      Reflyem::Crit::modify_actor_value(this_, actor, value, av, config);
    }

  if (config.cheat_death().enable() && config.cheat_death().magick())
    {
      Reflyem::CheatDeath::modify_actor_value(this_, actor, value, av, config);
    }

  if (config.petrified_blood().enable() && config.petrified_blood().magick())
    {
      Reflyem::PetrifiedBlood::modify_actor_value(this_, actor, value, av, config);
    }

  if (config.stamina_shield().enable() && config.stamina_shield().magick())
    {
      Reflyem::StaminaShield::modify_actor_value(this_, actor, value, av, config);
    }

  if (config.magic_shield().enable() && config.magic_shield().magick())
    {
      Reflyem::MagicShield::modify_actor_value(this_, actor, value, av, config);
    }

  if (config.soul_link().enable() && config.soul_link().magick())
    {
      Reflyem::SoulLink::modify_actor_value(this_, actor, std::addressof(value), av, config);
    }

  Reflyem::Vampirism::modify_actor_value(this_, actor, value, av, config);
}

auto calculate_stamina_regeneration(RE::Actor* actor, float delta) -> void
{
  if (!actor) { return; }

  let sub_140620CC0_character_update_regen_delay =
      [](RE::Actor* character, RE::ActorValue av, float delta_) -> bool
  {
    using FuncT = bool (*)(RE::Actor*, RE::ActorValue, float);
    const REL::Relocation<FuncT> func{RELOCATION_ID(37516, 0)};
    return func(character, av, delta_);
  };

  let sub_140620900_restore_actor_value =
      [](RE::Actor* character, RE::ActorValue av, float value) -> bool
  {
    using FuncT = bool (*)(RE::Actor*, RE::ActorValue, float);
    const REL::Relocation<FuncT> func{RELOCATION_ID(37513, 0)};
    return func(character, av, value);
  };

  let updated = sub_140620CC0_character_update_regen_delay(actor, RE::ActorValue::kStamina, delta);
  let process = actor->currentProcess;
  if (!process || !process->cachedValues) { return; }

  let cached_values = process->cachedValues;
  if (cached_values->flags.any(RE::CachedValues::Flags::kStaminaDamaged) && !updated &&
      (!actor->IsSprinting() || actor->IsOnMount()))
    {
      let combat_regen_mult =
          Reflyem::Core::get_float_game_setting("fCombatStaminaRegenRateMult").value_or(1.f);

      let config = Reflyem::Config::get_singleton().resource_manager();
      let changed_regen = config.enable() && config.regeneration_enable();

      let stamina_rate = actor->GetActorValue(RE::ActorValue::kStaminaRate);
      let stamina_rate_mult = actor->GetActorValue(RE::ActorValue::kStaminaRateMult);
      let max_stamina = Reflyem::Core::get_actor_value_max(actor, RE::ActorValue::kStamina);

      auto restore_value = changed_regen
                               ? Reflyem::ResourceManager::regeneration_actor_value(
                                     *actor,
                                     RE::ActorValue::kStaminaRate,
                                     RE::ActorValue::kStaminaRateMult)
                               : max_stamina * (stamina_rate * 0.01f) * (stamina_rate_mult * 0.01f);
      if (actor->IsInCombat()) { restore_value = restore_value * combat_regen_mult; }

      logger::debug("Calculate regeneration value for Stamina it's {}"sv, restore_value);

      if (restore_value > 0.f)
        {
          sub_140620900_restore_actor_value(actor, RE::ActorValue::kStamina, restore_value * delta);
        }
    }
}

auto calculate_actor_value_regeneration(RE::Character* character, const RE::ActorValue av) -> float
{
  if (!character) { return 0.f; }

  auto av_rate = RE::ActorValue::kNone;
  auto av_rate_mult = RE::ActorValue::kNone;
  auto entry_point = RE::BGSEntryPoint::ENTRY_POINTS::kTotal;
  auto combat_regen_mult = 0.f;
  auto essentialNonCombatHealRateBonus = 0.f;
  auto restore_value = 0.f;

  let get_bleedout_rate = [](const RE::Character* character_) -> float
  {
    let current_process = character_->currentProcess;
    if (!current_process) { return 0.f; }
    let middle_high = current_process->middleHigh;
    if (!middle_high) { return 0.f; }
    return middle_high->bleedoutRate;
  };

  let sub_1405E1F40_object_ref_form_flags = [](RE::Character* character_) -> void*
  {
    using FuncT = void* (*)(RE::Character*);
    const REL::Relocation<FuncT> func{RELOCATION_ID(36469, 0)};
    return func(character_);
  };

  let sub_1403BD3C0_actor_value_owner_restore_health = [](RE::ActorValueOwner* owner,
                                                          RE::TESBoundObject* object_reference,
                                                          void* form_flags) -> float
  {
    using FuncT = float (*)(RE::ActorValueOwner*, RE::TESBoundObject*, void*);
    const REL::Relocation<FuncT> func{RELOCATION_ID(25830, 0)};
    return func(owner, object_reference, form_flags);
  };

  let sub_1405E1F40_set_bleedout_rate = [](RE::Character* character_, float bleedout_rate) -> void*
  {
    using FuncT = void* (*)(RE::Character*);
    const REL::Relocation<FuncT> func{RELOCATION_ID(37680, 0)};
    return func(character_);
  };

  switch (av)
    {
      case RE::ActorValue::kHealth: {
        av_rate = RE::ActorValue::kHealRate;
        av_rate_mult = RE::ActorValue::kHealRateMult;
        entry_point = RE::BGSEntryPoint::ENTRY_POINT::kModRecoveredHealth;

        let combat_health_regen_mult =
            character->GetActorValue(RE::ActorValue::kCombatHealthRegenMultiply);
        combat_regen_mult = combat_health_regen_mult;

        if (character->IsProtected() && !character->IsInCombat() && !character->IsPlayerRef())
          {
            essentialNonCombatHealRateBonus =
                Reflyem::Core::get_float_game_setting("fEssentialNonCombatHealRateBonus")
                    .value_or(0.f);
          }

        if (character->IsBleedingOut())
          {
            av_rate = RE::ActorValue::kNone;
            if (combat_health_regen_mult == 0.f)
              {
                combat_regen_mult =
                    Reflyem::Core::get_float_game_setting("fEssentialDownCombatHealthRegenMult")
                        .value_or(0.f);
              }

            if (character->boolFlags.none(RE::Actor::BOOL_FLAGS::kNoBleedoutRecovery))
              {
                let bleedout_rate = get_bleedout_rate(character);
                restore_value = bleedout_rate;

                if (get_bleedout_rate(character) == 0.f)
                  {
                    let object_reference = character->data.objectReference;
                    let bleedout_rate_setting =
                        Reflyem::Core::get_float_game_setting("fBleedoutRate").value_or(1.f);

                    const auto form_flags = sub_1405E1F40_object_ref_form_flags(character);

                    let new_bleedout_rate = sub_1403BD3C0_actor_value_owner_restore_health(
                                                character,
                                                object_reference,
                                                form_flags) /
                                            bleedout_rate_setting;
                    sub_1405E1F40_set_bleedout_rate(character, new_bleedout_rate);
                    restore_value = new_bleedout_rate;
                  }

                break;
              }
            essentialNonCombatHealRateBonus = 0.f;
          }
        break;
      }
      case RE::ActorValue::kStamina: {
        combat_regen_mult =
            Reflyem::Core::get_float_game_setting("fCombatStaminaRegenRateMult").value_or(1.f);
        av_rate = RE::ActorValue::kStaminaRate;
        av_rate_mult = RE::ActorValue::kStaminaRateMult;
        break;
      }
      case RE::ActorValue::kMagicka: {
        combat_regen_mult =
            Reflyem::Core::get_float_game_setting("fCombatMagickaRegenRateMult").value_or(1.f);
        av_rate = RE::ActorValue::kMagickaRate;
        av_rate_mult = RE::ActorValue::kMagickaRateMult;
        break;
      }
      default: {
        return essentialNonCombatHealRateBonus + restore_value;
      }
    }

  let max_actor_value = Reflyem::Core::get_actor_value_max(character, av);
  let actor_value_rate_mult = character->GetActorValue(av_rate_mult);

  let config = Reflyem::Config::get_singleton().resource_manager();
  let changed_regen = config.enable() && config.regeneration_enable();

  if (av_rate != RE::ActorValue::kNone)
    {
      restore_value = changed_regen ? character->GetActorValue(av_rate)
                                    : max_actor_value * (character->GetActorValue(av_rate) * 0.01f);
    }

  if (restore_value > 0.f)
    {
      if (character->IsInCombat() &&
          (av != RE::ActorValue::kHealth ||
           character->race->data.flags.none(RE::RACE_DATA::Flag::kRegenHPInCombat)))
        {
          restore_value = restore_value * combat_regen_mult;
        }
      if (entry_point != RE::BGSEntryPoint::ENTRY_POINT::kTotal)
        {
          RE::BGSEntryPoint::HandleEntryPoint(entry_point, character, &restore_value);
        }
      auto mult = 1.f + (actor_value_rate_mult / 100.f);
      if (mult < 0.f) { mult = 1.f; }
      restore_value =
          changed_regen ? restore_value * mult : restore_value * (actor_value_rate_mult * 0.01f);
    }

  restore_value = essentialNonCombatHealRateBonus + restore_value;

  logd("Calculate regeneration value for {} it's {}"sv, av, restore_value);

  return restore_value;
}

auto OnCharacterUpdate::update_pc(RE::PlayerCharacter* this_, float delta) -> void
{
  if (this_)
    {
      // if (!object_manager_dumped) { dump_default_object_manager(); }
      auto& config = Reflyem::Config::get_singleton();
      player_last_delta = delta;
      update_actor(*this_, delta, config);
    }
  return update_pc_(this_, delta);
}

auto OnCharacterUpdate::update_npc(RE::Character* this_, float delta) -> void
{
  if (this_)
    {
      auto& config = Reflyem::Config::get_singleton();

      update_actor(*this_, delta, config);
    }
  return update_npc_(this_, delta);
}

auto OnActorUpdateEquipBound::process_event(
    const RE::ActorInventoryEvent* event,
    RE::BSTEventSource<RE::ActorInventoryEvent>* event_source) -> RE::BSEventNotifyControl
{
  logger::info("Process bound event"sv);

  letr config = Reflyem::Config::get_singleton();

  if (config.misc_fixes().equip_bound_fix()) { return RE::BSEventNotifyControl::kContinue; }

  return process_event_(event, event_source);
}

static void update_label(const RE::ContainerMenu* menu, RE::TESObjectARMO* armor)
{
  if (let movie = menu->uiMovie.get(); menu->itemList && movie)
    {
      RE::GFxValue ApparelArmorLabel;
      RE::GFxValue ApparelArmorValue;
      RE::GFxValue ApparelArmorValueText;
      if (movie->GetVariable(
              &ApparelArmorValue,
              "_root.Menu_mc.itemCardFadeHolder.ItemCard_mc.ApparelArmorValue") &&
          movie->GetVariable(
              &ApparelArmorValueText,
              "_root.Menu_mc.itemCardFadeHolder.ItemCard_mc.ApparelArmorValue.text"))
        {
          logi(
              "{} {} {}"sv,
              ApparelArmorValueText.GetNumber(),
              ApparelArmorValueText.GetSInt(),
              ApparelArmorValueText.GetUInt());
          ApparelArmorValue.SetText(
              std::format("{}/{}", ApparelArmorValueText.GetString(), "10").c_str());
        }
      if (movie->GetVariable(
              &ApparelArmorLabel,
              "_root.Menu_mc.itemCardFadeHolder.ItemCard_mc.ApparelArmorLabel"))
        {
          ApparelArmorLabel.SetMember("htmlText", std::format("{}/{}", "Д", "ВУ").c_str());
        }
    }
}

auto OnContainerMenu::process_message(RE::ContainerMenu* this_, RE::UIMessage& message)
    -> RE::UI_MESSAGE_RESULTS
{
  logi("Container menu process"sv);

  if (!this_ || !this_->itemCard || !this_->itemList)
    {
      logi("Container menu process some null"sv);
      return process_message_(this_, message);
    }

  if (let selected_item = this_->itemList->GetSelectedItem())
    {
      logi("Selected Item not null: {}"sv, selected_item->data.GetName());
      if (selected_item->data.objDesc && selected_item->data.objDesc->object &&
          selected_item->data.objDesc->object->IsArmor())
        {
          // logi("Selected armor, start change, old text: {}"sv,
          // this_->itemCard->infoText.c_str());
          update_label(this_, selected_item->data.objDesc->object->As<RE::TESObjectARMO>());
        }
    }

  return process_message_(this_, message);
}

auto OnContainerMenu::accept(RE::ContainerMenu* this_, RE::CallbackProcessor* processor) -> void
{
  logi("Container menu accept"sv);

  return accept_(this_, processor);

  if (!this_ || !this_->itemCard || !this_->itemList)
    {
      logi("Container menu accept some null"sv);
      return;
    }

  if (let selected_item = this_->itemList->GetSelectedItem())
    {
      logi("Selected Item not null: {}"sv, selected_item->data.GetName());
      if (selected_item->data.objDesc && selected_item->data.objDesc->object &&
          selected_item->data.objDesc->object->IsArmor())
        {
          logi("Selected armor, start change, old text: {}"sv, this_->itemCard->infoText.c_str());
          this_->itemCard->infoText = "BzIk ChOrT";
        }
    }
}

auto OnStandardItemData::get_equip_state(RE::StandardItemData* this_) -> std::uint32_t
{
  if (this_) { logi("Check equip state for: {}"sv, this_->GetName()); }
  return 1;
}

auto OnMagicItemData::get_equip_state(RE::StandardItemData* this_) -> std::uint32_t
{
  if (this_) { logi("Check equip state for: {}"sv, this_->GetName()); }
  return 1;
}

auto OnActorAddObject::add_object_to_container(
    RE::Actor* this_,
    RE::TESBoundObject* object,
    RE::ExtraDataList* extra_list,
    int32_t count,
    RE::TESObjectREFR* from_refr) -> void
{
  logger::info("In add_object_to_container"sv);
  // constexpr RE::FormID container_form_id = 0x2EDD7;
  constexpr RE::FormID container_form_id = 0x5C132;
  const auto data_handler = RE::TESDataHandler::GetSingleton();
  const auto container =
      data_handler->LookupForm<RE::TESObjectREFR>(container_form_id, "Skyrim.esm"sv);

  const auto alchemy = object->As<RE::AlchemyItem>();
  if (alchemy)
    {
      logger::info("Add alchemy object: {}, count: {}"sv, alchemy->GetFullName(), count);
      // if (container->AddObjectToContainer(object, count, this_)) {
      //   logger::info("Add to container"sv);
      //   return;
      // }
      // logger::info("Not add to container"sv);
      container->AddObjectToContainer(object, nullptr, count, nullptr);
      return;
    }
  else { logger::info("Null alchemy: {}"sv, object->GetName()); }

  return add_object_to_container_(this_, object, extra_list, count, from_refr);
}

auto OnActorAddObject::pick_up_object(
    RE::Actor* this_,
    RE::TESObjectREFR* object,
    int32_t count,
    bool arg3,
    bool play_sound) -> void
{
  logger::info("In pick_up_object"sv);

  // constexpr RE::FormID container_form_id = 0x2EDD7;
  constexpr RE::FormID container_form_id = 0x5C132;
  const auto data_handler = RE::TESDataHandler::GetSingleton();
  auto container = data_handler->LookupForm<RE::TESObjectREFR>(container_form_id, "Skyrim.esm"sv);

  const auto bound_object = object->GetObjectReference();
  if (bound_object)
    {
      const auto alchemy = bound_object->As<RE::AlchemyItem>();
      if (alchemy)
        {
          logger::info("Add alchemy object: {} count: {}"sv, alchemy->GetFullName(), count);
          // if (container->AddObjectToContainer(bound_object, count, this_)) {
          //   logger::info("Add to container"sv);
          //   return;
          // }
          // logger::info("Not add to container"sv);
          container->AddObjectToContainer(bound_object, nullptr, count, nullptr);
          count = 0;
        }
      else { logger::info("Pick null alchemy: {}"sv, object->GetName()); }
    }

  return pick_up_object_(this_, object, count, arg3, play_sound);
}

auto OnDrinkPotion::drink_potion(
    RE::Actor* this_,
    RE::AlchemyItem* potion,
    RE::ExtraDataList* extra_data_list) -> bool
{
  if (!this_ || !potion) { return drink_potion_(this_, potion, extra_data_list); }

  letr config = Reflyem::Config::get_singleton();
  if (config.potions_drink_limit().enable())
    {
      return Reflyem::PotionsDrinkLimit::drink_potion(this_, potion, extra_data_list, config) &&
             drink_potion_(this_, potion, extra_data_list);
    }

  return drink_potion_(this_, potion, extra_data_list);
}
auto OnSetCurrentSpell::set_spell_impl_pc(
    RE::ActorMagicCaster* caster,
    RE::MagicItem* magic_item,
    RE::TESObjectREFR* ref,
    bool a4) -> bool
{
  // logi("In set current spell implPC"sv);
  if (magic_item) { logi("SpellSetPC: {}"sv, magic_item->fullName.c_str()); }
  return set_spell_impl_pc_(caster, magic_item, ref, a4);
}

static RE::MagicItem* staff_spell = nullptr;

auto OnActorMagicCaster::on_set_spell_impl(RE::ActorMagicCaster* caster, RE::MagicItem* magic_item)
    -> void
{
  logi("On set spell impl"sv);

  if (staff_spell)
    {
      caster->currentSpell = magic_item;
      caster->actor->selectedSpells[static_cast<int>(caster->castingSource)] = magic_item;
      return on_set_spell_impl_(caster, staff_spell);
    }
  return on_set_spell_impl_(caster, magic_item);
}

auto OnSetCurrentSpell::set_spell_impl_block(
    RE::ActorMagicCaster* caster,
    RE::MagicItem* magic_item,
    RE::TESObjectREFR* ref,
    bool a4) -> bool
{
  // logi("In set current spell impl BLOCK"sv);
  if (magic_item) { logi("SpellSetBLOCK: {}"sv, magic_item->fullName.c_str()); }
  // let fire_ball = get_data(RE::MagicItem, 0x1C789, "Skyrim.esm"sv);
  // if (fire_ball) {
  //   logi("Success get skyrim fireball"sv);
  // }
  // let data_handler = RE::TESDataHandler::GetSingleton();
  // if (!data_handler)
  //   {
  //     logi("Data handler is null"sv);
  //     return false;
  //   }
  // let fire_ball = data_handler->LookupForm<RE::SpellItem>(0x01C789, "Skyrim.esm"sv);
  // if (!fire_ball) { logi("Fireball is null"sv); }
  return set_spell_impl_block_(caster, staff_spell ? staff_spell : magic_item, ref, a4);
}

auto OnSetCurrentSpell::set_spell_impl_fun(
    RE::ActorMagicCaster* caster,
    RE::MagicItem* magic_item,
    RE::TESObjectREFR* ref,
    bool a4) -> bool
{
  // logi("In set current spell impl FUN"sv);
  if (magic_item) { logi("SpellSetFUN: {}"sv, magic_item->fullName.c_str()); }
  return set_spell_impl_FUN_(caster, staff_spell ? staff_spell : magic_item, ref, a4);
}


auto OnAttackData::process_attack(RE::ActorValueOwner* value_owner, RE::BGSAttackData* attack_data)
    -> void
{
  process_attack_(value_owner, attack_data);
}

auto OnSomeMenuEquipState::get_equip_state_01(
    RE::Actor* actor,
    RE::TESBoundObject* object,
    void* a3,
    void* a4) -> std::uint32_t
{
  if (object) { logi("01 Check state for: {}"sv, object->GetName()); }
  return 1;
}

auto OnSomeMenuEquipState::get_equip_state_02(
    RE::Actor* actor,
    RE::TESBoundObject* object,
    void* a3,
    void* a4) -> std::uint32_t
{
  if (object) { logi("02 Check state for: {}"sv, object->GetName()); }
  return 1;
}

auto OnSomeMenuEquipState::get_equip_state_03(
    RE::Actor* actor,
    RE::TESBoundObject* object,
    void* a3,
    void* a4) -> std::uint32_t
{
  if (object) { logi("03 Check state for: {}"sv, object->GetName()); }
  return 1;
}

auto OnSpellEquip::equip_spell_left(
    RE::Actor* actor,
    RE::Actor::SlotTypes slot_type,
    RE::MagicItem* magic_item) -> void*
{
  logi("In on spell equip left call"sv);

  if (!magic_item || !actor) { return equip_spell_left_(actor, slot_type, magic_item); }
  return equip_spell_left_(actor, slot_type, magic_item);
}

auto OnSpellEquip::equip_spell_right(
    RE::Actor* actor,
    RE::Actor::SlotTypes slot_type,
    RE::MagicItem* magic_item) -> void*
{
  logi("In on spell equip left call"sv);

  if (!magic_item || !actor) { return equip_spell_right_(actor, slot_type, magic_item); }
  return equip_spell_right_(actor, slot_type, magic_item);
}

static bool is_equipped = false;

auto OnSomeEquip::some_equip(
    RE::ActorEquipManager* equip_manager,
    RE::Actor* actor,
    RE::TESBoundObject* bound_object,
    void* extra_data_list) -> void
{
  // logi("In on some equip call"sv);

  if (!equip_manager || !actor || !bound_object || !extra_data_list)
    {
      return some_equip_(equip_manager, actor, bound_object, extra_data_list);
    }

  // let is_left =
  //     Reflyem::Core::equip_slot_comparer(equip_slot, Reflyem::Core::get_left_hand_equip_slot());
  // let is_right =
  //     Reflyem::Core::equip_slot_comparer(equip_slot, Reflyem::Core::get_right_hand_equip_slot());
  // let is_voice =
  //     Reflyem::Core::equip_slot_comparer(equip_slot, Reflyem::Core::get_voice_equip_slot());

  logi("BoundObjectName: {}"sv, bound_object->GetName());
  // is_right,
  // is_left,
  // is_voice);

  if (!is_equipped && bound_object->IsWeapon() &&
      bound_object->As<RE::TESObjectWEAP>()->IsOneHandedSword())
    {
      is_equipped = true;
      logi("dual equip"sv);
      let left_hand_equip = Reflyem::Core::get_left_hand_equip_slot();
      equip_manager->EquipObject(actor, bound_object, nullptr, 1, left_hand_equip);
    }


  return some_equip_(equip_manager, actor, bound_object, extra_data_list);
}
auto OnSomeEquip::some_equip_default_equip(
    RE::ActorEquipManager* equip_manager,
    RE::Actor* actor,
    RE::TESBoundObject* bound_object) -> RE::BGSEquipSlot*
{
  // logi("In default equip"sv);
  if (!equip_manager || !actor || !bound_object)
    {
      return some_equip_default_equip_(equip_manager, actor, bound_object);
    }

  let left_equip = Reflyem::Core::get_left_hand_equip_slot();
  let return_value = some_equip_default_equip_(equip_manager, actor, bound_object);
  if (left_equip == return_value) { return return_value; }

  if (!bound_object->IsWeapon()) { return return_value; }

  let weapon = bound_object->As<RE::TESObjectWEAP>();
  if (!weapon) { return return_value; }

  if (weapon->GetWeaponType() == RE::WEAPON_TYPE::kStaff)
    {
      // logi("Weapon is staff"sv);
      return left_equip;
    }

  return return_value;
}

auto OnEquipManagerEquipSpell::equip_spell(
    RE::ActorEquipManager* equip_manager,
    RE::Actor* actor,
    RE::TESBoundObject* bound_object,
    RE::BGSEquipSlot* equip_slot) -> void*
{
  logi("Manager equip slot"sv);
  if (bound_object && bound_object->IsMagicItem())
    {
      staff_spell = bound_object->As<RE::MagicItem>();
    }
  return equip_slot;
  // return equip_spell_(equip_manager, actor, bound_object, equip_slot);
}
auto OnFillCasterEnchantData::fill_caster_enchant_data(RE::ActorMagicCaster* caster) -> void
{
  // logi("In fill ench data"sv);
  // if (!caster || !caster->actor) { return fill_caster_enchant_data_(caster); }
  //
  // let spell = caster->actor->selectedSpells[static_cast<int>(caster->castingSource)];
  // if (spell) { logi("Spell in fill: {}"sv, spell->fullName.c_str()); }
  //
  // if (caster->weaponEnchantmentController) { logi("Ench controller not null"sv); }

  return fill_caster_enchant_data_(caster);
}

auto OnEquipSpellMenu::equip_spell_menu(
    RE::ActorEquipManager* equip_manager,
    RE::Actor* actor,
    RE::TESBoundObject* bound_object,
    RE::BGSEquipSlot* equip_slot) -> RE::BGSEquipSlot*
{
  logi("Equip spell menu"sv);
  if (bound_object && bound_object->IsMagicItem())
    {
      staff_spell = bound_object->As<RE::MagicItem>();
    }
  return equip_slot;
}
auto OnEquipSpellMenu::equip_spell_menu_favorite(
    RE::ActorEquipManager* equip_manager,
    RE::Actor* actor,
    RE::TESBoundObject* bound_object,
    RE::BGSEquipSlot* equip_slot) -> RE::BGSEquipSlot*
{
  logi("Equip spell menu favorite"sv);
  if (bound_object && bound_object->IsMagicItem())
    {
      staff_spell = bound_object->As<RE::MagicItem>();
    }
  // return nullptr;
  return equip_slot;
}

auto OnActorIsOverEncumbered::is_actor_on_mount(RE::Actor* actor) -> bool
{
  if (actor && actor->IsPlayerRef() &&
      Reflyem::Config::get_singleton().misc_fixes().disable_over_encumbered())
    {
      return true;
    }
  return is_actor_on_mount_(actor);
}

auto OnCalculatePlayerSprintCost::calculate_sprint_cost(float player_stamina, float maybe_delta)
    -> float
{
  logger::debug("Calculate player sprint {} {}"sv, player_stamina, maybe_delta);
  if (let player = RE::PlayerCharacter::GetSingleton();
      !player->IsInCombat() &&
      Reflyem::Config::get_singleton().misc_fixes().disable_sprint_cost_in_combat())
    {
      return 0.f;
    }
  return calculate_sprint_cost_(player_stamina, maybe_delta);
}

auto OnInventoryOpen::is_displayed_item(RE::InventoryEntryData* item) -> bool
{
  logger::debug("In open inv hook"sv);
  if (!item) { return is_displayed_item_(item); }

  const auto& config = Reflyem::Config::get_singleton();
  if (config.death_loot().enable())
    {
      return Reflyem::DeathLoot::is_loot(item) && is_displayed_item_(item);
    }

  return is_displayed_item_(item);
}

auto OnAttackAction::attack_action(const RE::TESActionData* action_data) -> bool
{
  logger::info(
      "Attack Action: {}"sv,
      static_cast<std::uint32_t>(action_data->GetSourceActorState()->GetAttackState()));
  return false;
}

auto OnApplySpellsFromAttack::apply_spells_from_attack(
    RE::Character* attacker,
    RE::InventoryEntryData* weapon,
    bool is_left,
    RE::Actor* target) -> void
{
  if (!attacker || !weapon || !target)
    {
      return apply_spells_from_attack_(attacker, weapon, is_left, target);
    }

  letr config = Reflyem::Config::get_singleton();
  RE::HitData hit_data{};
  Reflyem::Core::initialization_hit_data(hit_data, attacker, target, weapon, is_left);

  if (config.unblockable_attack().enable() && hit_data.flags.any(RE::HitData::Flag::kBlocked) &&
      !target->IsDead())
    {
      logd("Apply spells unblock check start"sv);
      if (Reflyem::Core::try_has_absolute_keyword(
              attacker,
              config.unblockable_attack().absolute_unblock()))
        {
          logd("Is absolute unblock"sv);
          return apply_spells_from_attack_(attacker, weapon, is_left, target);
        }

      if (config.timing_block().enable())
        {
          auto& actor_cache = Reflyem::Core::ActorsCache::get_singleton();
          const auto& actor_data = actor_cache.get_or_add(target->formID).get();
          if (Reflyem::TimingBlock::is_allow_timing_parry(*attacker, *target, actor_data, config))
            {
              logd("Is allow timing parry"sv);
              return;
            }
          if (Reflyem::TimingBlock::is_allow_timing_block(*attacker, *target, actor_data, config) &&
              !Reflyem::Core::try_has_absolute_keyword(
                  attacker,
                  config.unblockable_attack().timing_block_unblock()))
            {
              logd("Is allow timing block"sv);
              return;
            }
        }

      if (Reflyem::Core::try_has_absolute_keyword(
              attacker,
              config.unblockable_attack().just_block_unblock()))
        {
          logd("Just unblock attack"sv);
          return apply_spells_from_attack_(attacker, weapon, is_left, target);
        }
    }

  if (config.timing_block().enable())
    {
      if (Reflyem::TimingBlock::apply_spells_from_attack(
              *attacker,
              *target,
              hit_data,
              Reflyem::Config::get_singleton()))
        {
          return;
        }
    }

  return apply_spells_from_attack_(attacker, weapon, is_left, target);
}
auto OnRegenerationPermanentValue::calculate_regeneration_value_magicka(
    RE::Character* character,
    RE::ActorValue av) -> float
{
  letr config = Reflyem::Config::get_singleton();

  if (character &&
      (config.misc_fixes().regeneration_fix() ||
       (config.resource_manager().enable() && config.resource_manager().regeneration_enable())))
    {
      return calculate_actor_value_regeneration(character, av);
    }
  return calculate_regeneration_value_health_(character, av);
}

auto OnRegenerationPermanentValue::calculate_regeneration_value_stamina_const_delta(
    RE::Actor* actor,
    float delta) -> void
{
  letr config = Reflyem::Config::get_singleton();

  if (actor &&
      (config.misc_fixes().regeneration_fix() ||
       (config.resource_manager().enable() && config.resource_manager().regeneration_enable())))
    {
      return calculate_stamina_regeneration(actor, delta);
    }
  return calculate_regeneration_value_stamina_const_delta_(actor, delta);
}

auto OnRegenerationPermanentValue::calculate_regeneration_value_stamina(
    RE::Actor* actor,
    float delta) -> void
{
  letr config = Reflyem::Config::get_singleton();

  if (actor &&
      (config.misc_fixes().regeneration_fix() ||
       (config.resource_manager().enable() && config.resource_manager().regeneration_enable())))
    {
      return calculate_stamina_regeneration(actor, delta);
    }
  return calculate_regeneration_value_stamina_(actor, delta);
}

auto OnRegenerationPermanentValue::calculate_regeneration_value_health(
    RE::Character* character,
    RE::ActorValue av) -> float
{
  letr config = Reflyem::Config::get_singleton();

  if (character &&
      (config.misc_fixes().regeneration_fix() ||
       (config.resource_manager().enable() && config.resource_manager().regeneration_enable())))
    {
      return calculate_actor_value_regeneration(character, av);
    }
  return calculate_regeneration_value_health_(character, av);
}

auto OnArrowCallHit::arrow_call_hit(
    RE::Character* attacker,
    RE::Actor* target,
    RE::Projectile* projectile,
    bool is_left) -> void
{
  return arrow_call_hit_(attacker, target, projectile, is_left);
}

auto OnAttackIsBlocked::is_blocked(
    RE::Actor* target,
    RE::Actor* attacker,
    float* attacker_location_x,
    float* target_location_x,
    void* arg5,
    float arg6,
    float* arg7,
    char arg8) -> bool
{
  if (!attacker || !target)
    {
      return is_blocked_(
          target,
          attacker,
          attacker_location_x,
          target_location_x,
          arg5,
          arg6,
          arg7,
          arg8);
    }

  letr config = Reflyem::Config::get_singleton();
  if (config.unblockable_attack().enable())
    {
      logd(
          "Is block unblock check start Attacker: {} | Target: {}"sv,
          attacker->GetDisplayFullName(),
          target->GetDisplayFullName());

      if (Reflyem::Core::try_has_absolute_keyword(
              attacker,
              config.unblockable_attack().absolute_unblock()))
        {
          logd("Is absolute keyword"sv);
          return false;
        }

      if (config.timing_block().enable())
        {
          auto& actor_cache = Reflyem::Core::ActorsCache::get_singleton();
          const auto& actor_data = actor_cache.get_or_add(target->formID).get();
          if (Reflyem::TimingBlock::is_allow_timing_parry(*attacker, *target, actor_data, config))
            {
              logd("Is timing parry"sv);
              return is_blocked_(
                  target,
                  attacker,
                  attacker_location_x,
                  target_location_x,
                  arg5,
                  arg6,
                  arg7,
                  arg8);
            }
          if (Reflyem::TimingBlock::is_allow_timing_block(*attacker, *target, actor_data, config) &&
              !Reflyem::Core::try_has_absolute_keyword(
                  attacker,
                  config.unblockable_attack().timing_block_unblock()))
            {
              logd("Is timing block"sv);
              return is_blocked_(
                  target,
                  attacker,
                  attacker_location_x,
                  target_location_x,
                  arg5,
                  arg6,
                  arg7,
                  arg8);
            }
        }

      if (Reflyem::Core::try_has_absolute_keyword(
              attacker,
              config.unblockable_attack().just_block_unblock()))
        {
          logd("Is just unblock"sv);
          return false;
        }
    }

  logd("Is blocked function final"sv);
  return is_blocked_(
      target,
      attacker,
      attacker_location_x,
      target_location_x,
      arg5,
      arg6,
      arg7,
      arg8);
}

auto OnAnimationEvent::process_event_pc(
    RE::BSTEventSink<RE::BSAnimationGraphEvent>* this_,
    RE::BSAnimationGraphEvent* event,
    RE::BSTEventSource<RE::BSAnimationGraphEvent>* dispatcher) -> void
{
  if (!event || !event->holder) { return process_event_pc_(this_, event, dispatcher); }
  auto& config = Reflyem::Config::get_singleton();
  if (config.resource_manager().enable())
    {
      Reflyem::ResourceManager::animation_handler(*event, config);
    }
  // if (Reflyem::AnimationEventHandler::try_find_animation(event->tag.c_str()) ==
  // Reflyem::AnimationEventHandler::AnimationEvent::kWeaponSwing)
  // {
  //   logger::info("WeaponSwing"sv);
  //   const auto data_handler = RE::TESDataHandler::GetSingleton();
  //   constexpr auto mod_name = "[RfaD] Pickpocket Rebalance.esp"sv;
  //   constexpr RE::FormID arrow_id = 0x45FA22;
  //   constexpr RE::FormID dagger_id = 0x45FA21;
  //   const auto arrow = data_handler->LookupForm<RE::TESAmmo>(arrow_id, mod_name);
  //   const auto dagger = data_handler->LookupForm<RE::TESObjectWEAP>(dagger_id, mod_name);
  //   const auto actor = const_cast<RE::Actor*>(event->holder->As<RE::Actor>());
  //   const auto current_process = actor->currentProcess;
  //   const auto weap = actor->GetEquippedEntryData(false);
  //   logger::info("Start equals"sv);
  //   if (weap->GetWeight() == dagger->GetWeight())
  //   {
  //     RE::NiAVObject* fire_node;
  //
  //     if (current_process) {
  //       const auto& biped = actor->GetBiped2();
  //       fire_node = dagger->IsCrossbow() ? current_process->GetMagicNode(biped) :
  //       current_process->GetWeaponNode(biped);
  //     } else {
  //       fire_node = dagger->GetFireNode(actor->IsPlayerRef() ? actor->GetCurrent3D() :
  //       actor->Get3D2());
  //     }
  //
  //     RE::NiPoint3      origin;
  //     RE::Projectile::ProjectileRot angles{};
  //
  //     if (fire_node) {
  //       origin = fire_node->world.translate;
  //       actor->Unk_A0(fire_node, angles.x, angles.z, origin);
  //     } else {
  //       origin = actor->GetPosition();
  //       origin.z += 96.0f;
  //
  //       angles.x = actor->GetAimAngle();
  //       angles.z = actor->GetAimHeading();
  //     }
  //     RE::ProjectileHandle handle;
  //     auto launch_data = RE::Projectile::LaunchData(actor, origin, angles, arrow, dagger);
  //     launch_data.enchantItem = weap->GetEnchantment();
  //     launch_data.poison = Reflyem::Core::get_poison(weap);
  //     RE::Projectile::Launch(&handle, launch_data);
  //   }
  //
  // }
  if (config.tk_dodge().enable()) { Reflyem::TkDodge::animation_handler(*event, config); }
  if (config.parry_bash().enable()) { Reflyem::ParryBash::animation_handler(*event, config); }
  return process_event_pc_(this_, event, dispatcher);
}

auto OnAnimationEvent::process_event_npc(
    RE::BSTEventSink<RE::BSAnimationGraphEvent>* this_,
    RE::BSAnimationGraphEvent* event,
    RE::BSTEventSource<RE::BSAnimationGraphEvent>* dispatcher) -> void
{
  if (!event || !event->holder) { return process_event_npc_(this_, event, dispatcher); }
  auto& config = Reflyem::Config::get_singleton();
  if (config.resource_manager().enable())
    {
      Reflyem::ResourceManager::animation_handler(*event, config);
    }
  if (config.tk_dodge().enable()) { Reflyem::TkDodge::animation_handler(*event, config); }
  Reflyem::ParryBash::animation_handler(*event, config);
  return process_event_npc_(this_, event, dispatcher);
}

auto OnAdjustActiveEffect::adjust_active_effect(RE::ActiveEffect* this_, float power, bool unk)
    -> void
{
  if (this_)
    {
      const auto caster = this_->GetCasterActor();
      const auto target = this_->GetTargetActor();
      if (caster && target)
        {
          logger::info(
              "Target HP: {} Caster HP: {} Target Level: {} Caster Level: {}"sv,
              target->GetActorValue(RE::ActorValue::kHealth),
              caster->GetActorValue(RE::ActorValue::kHealth),
              target->GetLevel(),
              caster->GetLevel());
          logger::info("Effect, mag: {}, dur: {}"sv, this_->magnitude, this_->duration);
        }
      else { logger::info("Caster or target is null"sv); }
    }
  adjust_active_effect_(this_, power, unk);
  return;
}

auto OnModifyActorValue::modify_actor_value(
    RE::ValueModifierEffect* this_,
    RE::Actor* actor,
    float value,
    RE::ActorValue av) -> void
{
  if (!actor || !this_)
    {
      modify_actor_value_(this_, actor, value, av);
      return;
    }

  on_modify_actor_value(this_, actor, value, av);

  modify_actor_value_(this_, actor, value, av);
}

auto OnModifyActorValue::absorb_effect_modify_actor_value(
    RE::ValueModifierEffect* this_,
    RE::Actor* actor,
    float value,
    RE::ActorValue av) -> void
{
  if (!actor || !this_)
    {
      absorb_effect_modify_actor_value_(this_, actor, value, av);
      return;
    }

  on_modify_actor_value(this_, actor, value, av);

  absorb_effect_modify_actor_value_(this_, actor, value, av);
}

auto OnModifyActorValue::peak_modify_actor_value(
    RE::ValueModifierEffect* this_,
    RE::Actor* actor,
    float value,
    RE::ActorValue av) -> void
{
  logger::debug("peak mod actor value"sv);

  if (!actor || !this_)
    {
      peak_modify_actor_value_(this_, actor, value, av);
      return;
    }

  on_modify_actor_value(this_, actor, value, av);

  peak_modify_actor_value_(this_, actor, value, av);
}

auto OnModifyActorValue::dual_modify_actor_value(
    RE::ValueModifierEffect* this_,
    RE::Actor* actor,
    float value,
    RE::ActorValue av) -> void
{
  if (!actor || !this_)
    {
      dual_modify_actor_value_(this_, actor, value, av);
      return;
    }

  on_modify_actor_value(this_, actor, value, av);

  dual_modify_actor_value_(this_, actor, value, av);
}

auto OnModifyActorValue::dual_modify_actor_value_second_inner_call(
    RE::ValueModifierEffect* this_,
    RE::Actor* actor,
    float value,
    RE::ActorValue av) -> void
{
  if (!actor || !this_)
    {
      dual_modify_actor_value_second_inner_call_(this_, actor, value, av);
      return;
    }

  on_modify_actor_value(this_, actor, value, av);

  dual_modify_actor_value_second_inner_call_(this_, actor, value, av);
}

auto OnMainUpdate::main_update(RE::Main* this_, float unk) -> void
{
  if (const auto ui = RE::UI::GetSingleton(); ui->GameIsPaused())
    {
      main_update_(this_, unk);
      return;
    }

  main_update_(this_, unk);
  return;
}

auto OnWeaponHit::weapon_hit(RE::Actor* target, RE::HitData& hit_data) -> void
{
  if (!target)
    {
      weapon_hit_(target, hit_data);
      return;
    }

  auto& config = Reflyem::Config::get_singleton();

  if (config.timing_block().enable() && hit_data.weapon && hit_data.weapon->IsMelee())
    {
      Reflyem::TimingBlock::on_weapon_hit(*target, hit_data, config);
    }

  if (config.magic_weapon().enable()) { Reflyem::MagicWeapon::on_weapon_hit(*target, hit_data); }

  if (config.weapon_crit().enable()) { Reflyem::Crit::on_weapon_hit(target, hit_data, config); }

  if (config.resource_manager().enable() && config.resource_manager().block_spend_enable())
    {
      Reflyem::ResourceManager::on_weapon_hit(target, hit_data, config);
    }

  if (config.cast_on_hit().enable())
    {
      Reflyem::CastOnHit::on_weapon_hit(target, hit_data, config);
    }

  if (config.cast_on_block().enable())
    {
      Reflyem::CastOnBlock::on_weapon_hit(target, hit_data, config);
    }

  if (config.cheat_death().enable() && config.cheat_death().physical())
    {
      Reflyem::CheatDeath::on_weapon_hit(target, hit_data, config);
    }

  if (config.petrified_blood().enable() && config.petrified_blood().physical())
    {
      Reflyem::PetrifiedBlood::on_weapon_hit(target, hit_data, config);
    }

  if (config.stamina_shield().enable() && config.stamina_shield().physical())
    {
      Reflyem::StaminaShield::on_weapon_hit(target, hit_data, config);
    }

  if (config.magic_shield().enable() && config.magic_shield().physical())
    {
      Reflyem::MagicShield::on_weapon_hit(target, hit_data, config);
    }

  if (config.soul_link().enable() && config.soul_link().physic())
    {
      Reflyem::SoulLink::on_weapon_hit(target, hit_data, config);
    }

  Reflyem::Vampirism::on_weapon_hit(target, hit_data, config);

  return weapon_hit_(target, hit_data);
}

auto OnCastActorValue::actor_value_for_cost_check_cast(
    RE::MagicItem* magic_item,
    RE::MagicSystem::CastingSource cast_source) -> RE::ActorValue
{
  // let cost_av = actor_value_for_cost_check_cast_(magic_item, cast_source);
  // if (magic_item)
  //   {
  //     logger::info(
  //         "CheckCast AV {} Spell {} CastSource {}"sv,
  //         cost_av,
  //         magic_item->fullName.c_str(),
  //         static_cast<int>(cast_source));
  //   }
  // if (cost_av == RE::ActorValue::kMagicka) { return RE::ActorValue::kHealth; }
  // return cost_av;
  return actor_value_for_cost_check_cast_(magic_item, cast_source);
}

auto OnCastActorValue::actor_value_for_cost_during_cast(
    RE::MagicItem* magic_item,
    RE::MagicSystem::CastingSource cast_source) -> RE::ActorValue
{
  // let cost_av = actor_value_for_cost_during_cast_(magic_item, cast_source);
  // if (magic_item)
  //   {
  //     logger::info(
  //         "DuringCast AV {} Spell {} CastSource {}"sv,
  //         cost_av,
  //         magic_item->fullName.c_str(),
  //         static_cast<int>(cast_source));
  //   }
  // if (cost_av == RE::ActorValue::kMagicka) { return RE::ActorValue::kHealth; }
  // return cost_av;
  return actor_value_for_cost_during_cast_(magic_item, cast_source);
}
auto OnCastActorValue::actor_value_for_cost_restore_value(
    RE::MagicItem* magic_item,
    RE::MagicSystem::CastingSource cast_source) -> RE::ActorValue
{
  // let cost_av = actor_value_for_cost_restore_value_(magic_item, cast_source);
  // if (magic_item)
  //   {
  //     logger::info(
  //         "RestoreCost AV {} Spell {} CastSource {}"sv,
  //         cost_av,
  //         magic_item->fullName.c_str(),
  //         static_cast<int>(cast_source));
  //   }
  // if (cost_av == RE::ActorValue::kMagicka) { return RE::ActorValue::kHealth; }
  // return cost_av;
  return actor_value_for_cost_restore_value_(magic_item, cast_source);
}

auto OnTrapHit::trap_hit(RE::Actor* target, RE::HitData* hit_data) -> void
{
  if (!target || !hit_data) { return trap_hit_(target, hit_data); }

  // const auto is_trap_tweak = true;
  //   if (is_trap_tweak)
  //   {
  //     auto damage_resist = 1.f;
  //     RE::BGSEntryPoint::HandleEntryPoint(
  //         RE::BGSEntryPoint::ENTRY_POINT::kModIncomingDamage,
  //         &target,
  //         aggressor,
  //         hit_data.weapon,
  //         std::addressof(damage_resist));
  //   }

  auto& config = Reflyem::Config::get_singleton();

  if (config.cheat_death().enable() && config.cheat_death().physical())
    {
      Reflyem::CheatDeath::on_weapon_hit(target, *hit_data, config);
    }

  if (config.petrified_blood().enable() && config.petrified_blood().physical())
    {
      Reflyem::PetrifiedBlood::on_weapon_hit(target, *hit_data, config);
    }

  if (config.stamina_shield().enable() && config.stamina_shield().physical())
    {
      Reflyem::StaminaShield::on_weapon_hit(target, *hit_data, config);
    }

  if (config.magic_shield().enable() && config.magic_shield().physical())
    {
      Reflyem::MagicShield::on_weapon_hit(target, *hit_data, config);
    }

  if (config.soul_link().enable() && config.soul_link().physic())
    {
      Reflyem::SoulLink::on_weapon_hit(target, *hit_data, config);
    }

  return trap_hit_(target, hit_data);
}

auto OnMeleeCollision::melee_collision(
    RE::Actor* attacker,
    RE::Actor* victim,
    RE::Projectile* projectile,
    char aleft) -> void
{
  if (!attacker || !victim) { return melee_collision_(attacker, victim, projectile, aleft); }

  const auto& config = Reflyem::Config::get_singleton();

  if (config.parry_bash().enable())
    {
      if (Reflyem::ParryBash::on_melee_collision(*attacker, *victim, config)) { return; }
    }

  return melee_collision_(attacker, victim, projectile, aleft);
}

auto OnCheckResistance::check_resistance_npc(
    RE::MagicTarget* this_,
    RE::MagicItem* magic_item,
    RE::Effect* effect,
    RE::TESBoundObject* bound_object) -> float
{
  if (!this_ || !magic_item || !effect)
    {
      logger::debug("Original resistance call");
      return check_resistance_npc_(this_, magic_item, effect, bound_object);
    }

  const auto& config = Reflyem::Config::get_singleton();

  if (config.resist_tweaks().enable() && config.resist_tweaks().enable_check_resistance())
    {
      return Reflyem::ResistTweaks::check_resistance(
          *this_,
          *magic_item,
          *effect,
          bound_object,
          config);
    }

  return check_resistance_npc_(this_, magic_item, effect, bound_object);
}

auto OnCheckResistance::check_resistance_pc(
    RE::MagicTarget* this_,
    RE::MagicItem* magic_item,
    RE::Effect* effect,
    RE::TESBoundObject* bound_object) -> float
{
  if (!this_ || !magic_item || !effect)
    {
      logger::debug("Original resistance call"sv);
      return check_resistance_pc_(this_, magic_item, effect, bound_object);
    }

  const auto& config = Reflyem::Config::get_singleton();

  if (config.resist_tweaks().enable() && config.resist_tweaks().enable_check_resistance())
    {
      return Reflyem::ResistTweaks::check_resistance(
          *this_,
          *magic_item,
          *effect,
          bound_object,
          config);
    }

  return check_resistance_pc_(this_, magic_item, effect, bound_object);
}

auto OnEnchIgnoresResistance::ignores_resistance(RE::MagicItem* this_) -> bool
{
  if (!this_) { return ignores_resistance_(this_); }

  const auto& config = Reflyem::Config::get_singleton();

  if (config.resist_tweaks().enable() && config.resist_tweaks().ench_ignore_resistance())
    {
      return Reflyem::ResistTweaks::ignores_resistance(*this_);
    }

  return ignores_resistance_(this_);
}

auto OnEnchGetNoAbsorb::get_no_absorb(RE::MagicItem* this_) -> bool
{
  if (!this_) { return get_no_absorb_(this_); }

  const auto& config = Reflyem::Config::get_singleton();

  if (config.resist_tweaks().enable() && config.resist_tweaks().ench_get_no_absorb())
    {
      return Reflyem::ResistTweaks::get_no_absorb(*this_);
    }

  return get_no_absorb_(this_);
}

auto OnActorValueOwner::get_actor_value_npc(RE::ActorValueOwner* this_, RE::ActorValue av) -> float
{
  if (!this_) { return get_actor_value_npc_(this_, av); }

  letr config = Reflyem::Config::get_singleton();

  if (config.equip_load().enable() && av == RE::ActorValue::kInventoryWeight)
    {
      return Reflyem::EquipLoad::get_actor_value(*this_, av)
          .value_or(get_actor_value_npc_(this_, av));
    }

  if (av == RE::ActorValue::kSpeedMult)
    {
      let speed_mult =
          config.speed_mult_cap_config().enable()
              ? Reflyem::SpeedMultCap::get_actor_value(*this_, av, get_actor_value_npc_)
                    .value_or(get_actor_value_npc_(this_, av))
              : get_actor_value_npc_(this_, av);

      if (config.misc_fixes().negative_speed_mult_fix())
        {
          if (speed_mult < 1.f) return 1.f;
        }

      return speed_mult;
    }

  return get_actor_value_npc_(this_, av);
}

auto OnActorValueOwner::set_actor_value_npc(
    RE::ActorValueOwner* this_,
    RE::ActorValue av,
    float value) -> void
{
  if (!this_) { return set_actor_value_npc_(this_, av, value); }

  if (Reflyem::Config::get_singleton().equip_load().enable() &&
      av == RE::ActorValue::kInventoryWeight)
    {
      return set_actor_value_npc_(
          this_,
          av,
          Reflyem::EquipLoad::set_actor_value(*this_, av, value));
    }

  return set_actor_value_npc_(this_, av, value);
}

auto OnActorValueOwner::mod_actor_value_npc(
    RE::ActorValueOwner* this_,
    RE::ActorValue av,
    float value) -> void
{
  if (!this_) { return mod_actor_value_npc_(this_, av, value); }

  if (Reflyem::Config::get_singleton().equip_load().enable() &&
      av == RE::ActorValue::kInventoryWeight)
    {
      return mod_actor_value_npc_(
          this_,
          av,
          Reflyem::EquipLoad::mod_actor_value(*this_, av, value));
    }

  return mod_actor_value_npc_(this_, av, value);
}

auto OnActorValueOwner::get_actor_value_pc(RE::ActorValueOwner* this_, RE::ActorValue av) -> float
{
  if (!this_) { return get_actor_value_pc_(this_, av); }

  letr config = Reflyem::Config::get_singleton();

  if (config.equip_load().enable() && av == RE::ActorValue::kInventoryWeight)
  {
    return Reflyem::EquipLoad::get_actor_value(*this_, av)
        .value_or(get_actor_value_pc_(this_, av));
  }

  if (av == RE::ActorValue::kSpeedMult)
  {
    let speed_mult =
        config.speed_mult_cap_config().enable()
            ? Reflyem::SpeedMultCap::get_actor_value(*this_, av, get_actor_value_pc_)
                  .value_or(get_actor_value_pc_(this_, av))
            : get_actor_value_pc_(this_, av);

    if (config.misc_fixes().negative_speed_mult_fix())
    {
      if (speed_mult < 1.f) return 1.f;
    }

    return speed_mult;
  }

  return get_actor_value_pc_(this_, av);
}

auto OnActorValueOwner::set_actor_value_pc(
    RE::ActorValueOwner* this_,
    RE::ActorValue av,
    float value) -> void
{
  if (!this_) { return set_actor_value_pc_(this_, av, value); }

  if (Reflyem::Config::get_singleton().equip_load().enable() &&
      av == RE::ActorValue::kInventoryWeight)
    {
      return set_actor_value_pc_(this_, av, Reflyem::EquipLoad::set_actor_value(*this_, av, value));
    }

  return set_actor_value_pc_(this_, av, value);
}

auto OnActorValueOwner::mod_actor_value_pc(
    RE::ActorValueOwner* this_,
    RE::ActorValue av,
    float value) -> void
{
  if (!this_) { return mod_actor_value_pc_(this_, av, value); }

  if (Reflyem::Config::get_singleton().equip_load().enable() &&
      av == RE::ActorValue::kInventoryWeight)
    {
      return mod_actor_value_npc_(
          this_,
          av,
          Reflyem::EquipLoad::mod_actor_value(*this_, av, value));
    }

  return mod_actor_value_pc_(this_, av, value);
}

auto OnHealthMagickaStaminaRegeneration::restore_health(
    RE::Actor* actor,
    RE::ActorValue av,
    float value) -> void
{
  // if (!actor) { return restore_health_(actor, av, value); }
  //
  // const auto& config = Reflyem::Config::get_singleton();
  // if (config.resource_manager().enable() && config.resource_manager().regeneration_enable())
  //   {
  //     value = Reflyem::ResourceManager::regeneration(*actor, av, player_last_delta);
  //   }

  return restore_health_(actor, av, value);
}

auto OnHealthMagickaStaminaRegeneration::restore_magicka(
    RE::Actor* actor,
    RE::ActorValue av,
    float value) -> void
{
  // if (!actor) { return restore_magicka_(actor, av, value); }
  //
  // const auto& config = Reflyem::Config::get_singleton();
  // if (config.resource_manager().enable() && config.resource_manager().regeneration_enable())
  //   {
  //     value = Reflyem::ResourceManager::regeneration(*actor, av, player_last_delta);
  //   }

  return restore_magicka_(actor, av, value);
}

auto OnHealthMagickaStaminaRegeneration::restore_stamina(
    RE::Actor* actor,
    RE::ActorValue av,
    float value) -> void
{
  // if (!actor) { return restore_stamina_(actor, av, value); }
  //
  // const auto& config = Reflyem::Config::get_singleton();
  // if (config.resource_manager().enable() && config.resource_manager().regeneration_enable())
  //   {
  //     value = Reflyem::ResourceManager::regeneration(*actor, av, player_last_delta);
  //   }

  // if (actor->IsPlayerRef()) { logger::info("RestoreStamina Value: {}"sv, value); }
  return restore_stamina_(actor, av, value);
}

auto install_hooks() -> void
{
  logger::info("start install hooks"sv);
  auto& trampoline = SKSE::GetTrampoline();
  trampoline.create(1024);
  OnMeleeCollision::install_hook(trampoline);
  OnWeaponHit::install_hook(trampoline);
  OnAttackIsBlocked::install_hook(trampoline);
  OnTrapHit::install_hook(trampoline);
  OnInventoryOpen::install_hook(trampoline);
  OnCastActorValue::install_hook(trampoline);
  OnCalculatePlayerSprintCost::install_hook(trampoline);
  OnRegenerationPermanentValue::install_hook(trampoline);
  OnActorIsOverEncumbered::install_hook(trampoline);
  OnDrinkPotion::install_hook();
  OnActorUpdateEquipBound::install_hook();
  OnCheckResistance::install_hook();
  OnEnchIgnoresResistance::install_hook();
  OnEnchGetNoAbsorb::install_hook();
  OnActorValueOwner::install_hook();
  // OnContainerMenu::install_hook();
  // OnStandardItemData::install_hook();
  // OnMagicItemData::install_hook();
  // OnMainUpdate::install_hook(trampoline);
  // OnAdjustActiveEffect::install_hook(trampoline);
  OnAnimationEvent::install_hook();
  OnCharacterUpdate::install_hook();
  // OnActorMagicCaster::install_hook();
  // OnSetCurrentSpell::install_hook(trampoline);
  OnModifyActorValue::install_hook(trampoline);
  // OnSpellEquip::install_hook(trampoline);
  // OnSomeEquip::install_hook(trampoline);
  // OnEquipManagerEquipSpell::install_hook(trampoline);
  // OnEquipSpellMenu::install_hook(trampoline);
  OnHealthMagickaStaminaRegeneration::install_hook(trampoline);
  OnApplySpellsFromAttack::install_hook(trampoline);
  // OnSomeMenuEquipState::install_hook(trampoline);
  // OnFillCasterEnchantData::install_hook(trampoline);
  // OnActorAddObject::install_hook();
  // OnAttackAction::install_hook(trampoline);
  // OnAttackData::install_hook(trampoline);
  logger::info("finish install hooks"sv);
}
} // namespace Hooks