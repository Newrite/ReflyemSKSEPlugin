#include "Hooks.hpp"

#include "AbsorbShield.hpp"
#include "CastOnGetHit.hpp"
#include "CastOnMagicProjectileGetHit.hpp"
#include "CastOnMagicProjectileHit.hpp"
#include "Core.hpp"
#include "DeclutteredLoot.hpp"
#include "GameSettingsHandler.hpp"
#include "KiEnergyPower.hpp"
#include "LeechEffect.hpp"
#include "MagicBlocker.hpp"
#include "PapyrusExtender.hpp"
#include "PoisonRework.hpp"
#include "RecoupEffect.hpp"
#include "SlowTime.hpp"
#include "SpecialTechniques.hpp"
#include "StaggerSystem.hpp"
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
#include "plugin/MagicWeapon.hpp"
#include "plugin/ParryBash.hpp"
#include "plugin/PetrifiedBlood.hpp"
#include "plugin/PotionsDrinkLimit.hpp"
#include "plugin/ResistTweaks.hpp"
#include "plugin/ResourceManager.hpp"
#include "plugin/SoulLink.hpp"
#include "plugin/SpeedCasting.hpp"
#include "plugin/SpeedMultCap.hpp"
#include "plugin/StaminaShield.hpp"
#include "plugin/TKDodge.hpp"
#include "plugin/TimingBlock.hpp"
#include "plugin/Vampirism.hpp"

#include <numbers>
#include <plugin/ProjectileBlock.hpp>

namespace Hooks {

static float player_last_delta = 0.f;
static bool object_manager_dumped = false;

static struct EquipManager
{

  private:
  static constexpr std::uint8_t NEED_FRAMES = 5;

  RE::BGSEquipSlot* slot{nullptr};
  RE::TESObjectWEAP* weapon{nullptr};
  bool ready_to_equip{false};
  std::uint8_t frame_count{0};

  public:
  auto invoke_equip() -> void
  {
    let actor_equip_manager = RE::ActorEquipManager::GetSingleton();
    if (actor_equip_manager && slot && weapon) {
      actor_equip_manager->EquipObject(RE::PlayerCharacter::GetSingleton(), weapon, nullptr, 1, slot);
      slot = nullptr;
      weapon = nullptr;
      ready_to_equip = false;
      frame_count = 0;
    }
  }

  auto update() -> void
  {
    if (frame_count >= NEED_FRAMES) {
      if (ready_to_equip) {
        invoke_equip();
      }
      return;
    }
    if (ready_to_equip) {
      frame_count++;
    }
  }

  auto set_up_manager(RE::BGSEquipSlot* _slot, RE::TESObjectWEAP* _weapon) -> void
  {
    frame_count = 0;
    ready_to_equip = true;
    slot = _slot;
    weapon = _weapon;
  }

} player_equip_manager;

auto dump_default_object_manager() -> void
{
  let object_manager = RE::BGSDefaultObjectManager::GetSingleton();

  if (!object_manager) {
    object_manager_dumped = true;
    logi("Object manager is null"sv);
  }

  for (uint32_t index : views::iota(0, RE::DEFAULT_OBJECTS::kTotal)) {
    if (!object_manager->objectInit[index]) {
      continue;
    }

    let form = object_manager->objects[index];

    if (!form) {
      continue;
    }

    logi("Object[{}] : {} | {} | {}", index, form->GetFormType(), form->GetName(), form->GetObjectTypeName());
  }

  object_manager_dumped = true;
}

auto update_actor(RE::Character& character, const float delta, const Reflyem::Config& config) -> void
{
  logger::debug("update actor, {}"sv, character.GetDisplayFullName());
  static auto poison_visitor = PoisonReworkVisitor();

  auto& actors_cache = Reflyem::Core::ActorsCache::get_singleton();
  auto& actor_data = actors_cache.get_or_add(character.formID).get();

  actor_data.last_delta_update(player_last_delta);
  let old_delay_before_restore_stamina = actor_data.delay_before_restore_stamina();
  actor_data.update_handler(character, player_last_delta);

  if (config.special_techniques().enable() && config.special_techniques().slow_after_block_hit_mult() != 1.0) {
    if (actor_data.slow_after_block_hit_flag() && actor_data.duration_after_block_hit() <= 0.f) {
      actor_data.slow_after_block_hit_flag(false);
    }
  }

  if (config.stagger_system().enable()) {
    Reflyem::StaggerSystem::update(&character, delta);
  }

  if (config.caster_additions().enable() && config.caster_additions().enable_rally_manacost()) {
    let magick_rally = actor_data.rally_magicka_cost();

    if (magick_rally > 0.f) {

      let min_restore = (Reflyem::Core::get_actor_value_max(&character, RE::ActorValue::kMagicka) *
                         config.caster_additions().magicka_mult_restore_per_second_from_rally()) *
                        player_last_delta;
      if (magick_rally > min_restore) {
        Reflyem::Core::restore_actor_value(character, RE::ActorValue::kMagicka, min_restore);
        logd("RestoreRallyMagickaValue: {}", min_restore);
        let new_rally = magick_rally - min_restore;
        actor_data.rally_magicka_cost(new_rally);
      } else {
        Reflyem::Core::restore_actor_value(character, RE::ActorValue::kMagicka, magick_rally);
        logd("RestoreRallyMagickaValue: {}", magick_rally);
        actor_data.rally_magicka_cost(0.f);
      }
    }
  }

  if (config.special_techniques().enable()) {
    Reflyem::SpecialTechniques::update(&character, delta);
  }

  if (config.absorb_shield().enable()) {
    Reflyem::AbsorbShield::update_actor(&character, delta);
  }

  if (static_cast<bool>(character.actorState2.staggered)) {
    actor_data.mod_stagger_duration(delta);
  } else {
    actor_data.stagger_duration(0.f);
  }

  if (config.ki_energy_power().enable()) {
    Reflyem::KiEnergyPower::update(&character);
  }

  if (!character.IsPlayerRef() && character.IsInCombat() && config.misc_fixes().enable_instant_restore_stamina_npc() &&
      old_delay_before_restore_stamina > 0.f && actor_data.delay_before_restore_stamina() <= 0.f) {
    static constexpr float STAMINA_RESTORE_VALUE = 10000.f;
    Reflyem::Core::restore_actor_value(character, RE::ActorValue::kStamina, STAMINA_RESTORE_VALUE);
  }

  if (character.IsPlayerRef() && config.slow_time().enable()) {
    Reflyem::SlowTime::update_actor(character, delta, config);
  }

  if (character.IsPlayerRef() && config.death_loot().enable()) {
    Reflyem::DeathLoot::handle_killed_actors();
  }

  if (character.GetAttackState() == RE::ATTACK_STATE_ENUM::kDraw) {

    if (config.highlighting().enable() && !character.IsPlayerRef() && !actor_data.start_hit_flag() &&
        Reflyem::Core::is_power_attacking(character)) {
      let combat_target = character.currentCombatTarget;
      if (combat_target && combat_target.get() && combat_target.get()->IsPlayerRef()) {
        if (config.highlighting().power_attack_shader()) {
          character.ApplyEffectShader(config.highlighting().power_attack_shader(),
                                      config.highlighting().highlight_duration());
        }
        if (config.highlighting().power_attack_art_other() && config.highlighting().power_attack_art_weapon()) {

          if (config.highlighting().power_attack_keyword() &&
              !Reflyem::Core::try_has_absolute_keyword(&character, config.highlighting().power_attack_keyword())) {

          } else {
            let weapon_node = character.GetNodeByName("WEAPON"sv);
            // if (weapon_node) {
            //   logi("HasWeaponNode");
            //   let weapon_node_node = character.GetNodeByName("WEAPON"sv)->AsNode();
            //   if (weapon_node_node && weapon_node_node->GetChildren().size() > 0) {
            //     logi("HasWeaponNodeNode and not empty");
            //     if (auto weapTip = weapon_node_node->GetChildren().front()) {
            //       logi("HasWeaponTip");
            //       if (auto weapTipNode = weapTip->AsNode()) {
            //         logi("WeaponTip NODE_NAME: ");
            //       }
            //     }
            //   }
            // }
            let shield_node = character.GetNodeByName("SHIELD"sv);
            let right_object = character.GetEquippedObject(false);
            let left_object = character.GetEquippedObject(true);
            if (!right_object && !left_object) {
              character.ApplyArtObject(config.highlighting().power_attack_art_weapon(),
                                       config.highlighting().highlight_duration(),
                                       nullptr,
                                       false,
                                       false,
                                       weapon_node);
              character.ApplyArtObject(config.highlighting().power_attack_art_other(),
                                       config.highlighting().highlight_duration(),
                                       nullptr,
                                       false,
                                       false,
                                       shield_node);
            } else {
              if (right_object && right_object->IsWeapon()) {
                character.ApplyArtObject(config.highlighting().power_attack_art_weapon(),
                                         config.highlighting().highlight_duration(),
                                         nullptr,
                                         false,
                                         false,
                                         weapon_node);
              }
              if (left_object && left_object->IsWeapon()) {
                character.ApplyArtObject(config.highlighting().power_attack_art_weapon(),
                                         config.highlighting().highlight_duration(),
                                         nullptr,
                                         false,
                                         false,
                                         weapon_node);
              }
              if (left_object && left_object->IsArmor()) {
                character.ApplyArtObject(config.highlighting().power_attack_art_other(),
                                         config.highlighting().highlight_duration(),
                                         nullptr,
                                         false,
                                         false,
                                         shield_node);
              }
            }
          }
        }
      }
    }

    if (!actor_data.start_hit_flag()) {
      actor_data.start_attack_duration(config.start_attack_duration());
    }

    actor_data.start_hit_flag(true);
  } else {
    actor_data.start_hit_flag(false);
    actor_data.start_attack_duration(0.f);
  }

  if (character.GetAttackState() == RE::ATTACK_STATE_ENUM::kBowReleasing ||
      character.GetAttackState() == RE::ATTACK_STATE_ENUM::kBowReleased) {

    if (config.highlighting().enable() && config.highlighting().range_release_art() && !character.IsPlayerRef() &&
        !actor_data.start_fire_flag()) {
      if (config.highlighting().range_release_keyword() &&
          !Reflyem::Core::try_has_absolute_keyword(&character, config.highlighting().range_release_keyword())) {

      } else {
        let combat_target = character.currentCombatTarget;
        if (combat_target && combat_target.get() && combat_target.get()->IsPlayerRef()) {
          character.ApplyArtObject(config.highlighting().range_release_art(),
                                   config.highlighting().highlight_duration());
        }
      }
    }

    actor_data.start_fire_flag(true);
  } else {
    actor_data.start_fire_flag(false);
  }

  if (config.timing_block().enable() || config.projectile_block().enable()) {
    if (Reflyem::Core::is_blocking(&character)) {
      actor_data.timing_block_timer(config.timing_block().block_delay());
      actor_data.timing_block_flag(true);
    } else {
      actor_data.timing_block_flag(false);
    }
  }

  if (actor_data.update_50_timer() <= 0.f) {
    logger::debug("update actor map50 tick"sv);
    actor_data.update_50_timer_refresh();
    if (config.vampirism().enable() || config.magic_vampirism().enable()) {
      Reflyem::Vampirism::character_update(character, delta, config, actor_data);
    }
  }

  if (actor_data.update_100_timer() <= 0.f) {
    logger::debug("update actor map100 tick"sv);

    actor_data.update_100_timer_refresh();

    if (config.resource_manager().enable()) {
      Reflyem::ResourceManager::update_actor(character, delta, config);
      if (config.resource_manager().weapon_spend_enable()) {
        Reflyem::ResourceManager::ranged_spend_handler(character, config);
      }
    }

    if (config.caster_additions().enable()) {
      Reflyem::CasterAdditions::on_update_actor(character, delta, config);
    }

    if (config.equip_load().enable()) {
      Reflyem::EquipLoad::update_actor(character, config);
    }

    if (config.leech_effect().enable()) {
      Reflyem::LeechEffect::character_update(character, delta, config, actor_data);
    }

    if (config.recoup_effect().enable()) {
      Reflyem::RecoupEffect::character_update(character, delta, config, actor_data);
    }
  }

  if (actor_data.update_1000_timer() <= 0.f) {
    logger::debug("update actor map1000 tick"sv);

    actor_data.update_1000_timer_refresh();
    actor_data.last_tick_count_refresh();

    if (config.poison_rework().enable()) {
      let inventory_changes = character.GetInventoryChanges();
      if (inventory_changes) {
        inventory_changes->VisitInventory(poison_visitor);
      }
    }

    if (config.misc_fixes().enable_instant_restore_stamina_npc() && character.IsInCombat() &&
        !character.IsPlayerRef()) {
      if (character.GetActorValue(RE::ActorValue::kStamina) <= 0.f &&
          actor_data.delay_before_restore_stamina() <= 0.f) {
        static constexpr float DELAY_RESTORE = 4.f;
        actor_data.delay_before_restore_stamina(DELAY_RESTORE);
      }
    }

    if (config.petrified_blood().enable() && config.petrified_blood().magick()) {
      Reflyem::PetrifiedBlood::character_update(character, delta, config, actor_data);
    }

    if (character.IsPlayerRef()) {
      Reflyem::GameSettingsHandler::GMST::game_settings_handler(config);
    }

    if (config.speed_casting().enable() && character.IsPlayerRef()) {
      Reflyem::SpeedCasting::on_update_actor(character, delta, config);
    }

    if (character.IsPlayerRef() && config.item_limit().enable()) {
      Reflyem::ItemLimit::update_actor(character, delta, config);
    }
  }

  if (actor_data.update_10000_timer() <= 0.f) {
    logger::debug("update actor map10000 tick"sv);

    actor_data.update_10000_timer_refresh();
    actor_data.last_tick_count_refresh();

    if (character.IsPlayerRef() && config.ki_energy_power().enable() && config.ki_energy_power().enable_ki_summons() &&
        config.ki_energy_power().ki_summons_list()) {
      Reflyem::KiEnergyPower::summons_proc(&character);
    }
  }
}

auto on_modify_actor_value(RE::ValueModifierEffect* this_, RE::Actor* actor, float& value, RE::ActorValue av) -> void
{
  auto& config = Reflyem::Config::get_singleton();

  let aggressor = this_->caster;
  if (actor && !actor->IsPlayerRef() && aggressor && aggressor.get() && aggressor.get().get() &&
      aggressor.get()->IsPlayerRef()) {
    auto& actors_cache = Reflyem::Core::ActorsCache::get_singleton();
    auto& actor_data = actors_cache.get_or_add(actor->formID).get();
    actor_data.hit_by_player_timer(config.death_loot().hit_timer());
  }

  if (config.magick_crit().enable()) {
    Reflyem::Crit::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.cheat_death().enable() && config.cheat_death().magick()) {
    Reflyem::CheatDeath::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.absorb_shield().enable()) {
    Reflyem::AbsorbShield::modify_actor_value_absorb(this_, actor, value, av, config);
  }

  if (config.petrified_blood().enable() && config.petrified_blood().magick()) {
    Reflyem::PetrifiedBlood::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.stamina_shield().enable() && config.stamina_shield().magick()) {
    Reflyem::StaminaShield::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.magic_shield().enable() && config.magic_shield().magick()) {
    Reflyem::MagicShield::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.soul_link().enable() && config.soul_link().magick()) {
    Reflyem::SoulLink::modify_actor_value(this_, actor, std::addressof(value), av, config);
  }

  Reflyem::Vampirism::modify_actor_value(this_, actor, value, av, config);

  if (config.leech_effect().enable()) {
    Reflyem::LeechEffect::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.recoup_effect().enable()) {
    Reflyem::RecoupEffect::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.absorb_shield().enable()) {
    Reflyem::AbsorbShield::modify_actor_value_gettin(this_, actor, value, av, config);
  }
}

auto calculate_stamina_regeneration(RE::Actor* actor, float delta) -> void
{
  if (!actor) {
    return;
  }

  let sub_140620CC0_character_update_regen_delay = [](RE::Actor* character, RE::ActorValue av, float delta_) -> bool {
    using FuncT = bool (*)(RE::Actor*, RE::ActorValue, float);
    const REL::Relocation<FuncT> func{RELOCATION_ID(37516, 0)};
    return func(character, av, delta_);
  };

  let sub_140620900_restore_actor_value = [](RE::Actor* character, RE::ActorValue av, float value) -> bool {
    using FuncT = bool (*)(RE::Actor*, RE::ActorValue, float);
    const REL::Relocation<FuncT> func{RELOCATION_ID(37513, 0)};
    return func(character, av, value);
  };

  let updated = sub_140620CC0_character_update_regen_delay(actor, RE::ActorValue::kStamina, delta);
  let process = actor->currentProcess;
  if (!process || !process->cachedValues) {
    return;
  }

  let cached_values = process->cachedValues;
  if (cached_values->flags.any(RE::CachedValues::Flags::kStaminaDamaged) && !updated &&
      (!actor->IsSprinting() || actor->IsOnMount())) {
    let combat_regen_mult = Reflyem::Core::get_float_game_setting("fCombatStaminaRegenRateMult").value_or(1.f);

    letr config = Reflyem::Config::get_singleton();
    let changed_regen = config.resource_manager().enable() && config.resource_manager().regeneration_enable();

    let stamina_rate = actor->GetActorValue(RE::ActorValue::kStaminaRate);
    let stamina_rate_mult = actor->GetActorValue(RE::ActorValue::kStaminaRateMult);
    let max_stamina = Reflyem::Core::get_actor_value_max(actor, RE::ActorValue::kStamina);

    auto restore_value = changed_regen ? Reflyem::ResourceManager::regeneration_actor_value(
                                             *actor, RE::ActorValue::kStaminaRate, RE::ActorValue::kStaminaRateMult)
                                       : max_stamina * (stamina_rate * 0.01f) * (stamina_rate_mult * 0.01f);
    if (actor->IsInCombat()) {
      restore_value = restore_value * combat_regen_mult;
    }

    logger::debug("Calculate regeneration value for Stamina it's {}"sv, restore_value);

    auto mult = 1.f;
    if (config.special_techniques().enable() && config.special_techniques().dynamic_regeneration_stamina()) {
      mult = 1.f + ((1.f - Reflyem::Core::get_actor_value_percent(actor, RE::ActorValue::kStamina)) / 4.f);
    }

    restore_value = restore_value * mult;

    if (restore_value > 0.f) {
      sub_140620900_restore_actor_value(actor, RE::ActorValue::kStamina, restore_value * delta);
    }
  }
}

auto calculate_actor_value_regeneration(RE::Character* character, const RE::ActorValue av) -> float
{
  if (!character) {
    return 0.f;
  }

  auto av_rate = RE::ActorValue::kNone;
  auto av_rate_mult = RE::ActorValue::kNone;
  auto entry_point = RE::BGSEntryPoint::ENTRY_POINTS::kTotal;
  auto combat_regen_mult = 0.f;
  auto essentialNonCombatHealRateBonus = 0.f;
  auto restore_value = 0.f;

  let get_bleedout_rate = [](const RE::Character* character_) -> float {
    let current_process = character_->currentProcess;
    if (!current_process) {
      return 0.f;
    }
    let middle_high = current_process->middleHigh;
    if (!middle_high) {
      return 0.f;
    }
    return middle_high->bleedoutRate;
  };

  let sub_1405E1F40_object_ref_form_flags = [](RE::Character* character_) -> void* {
    using FuncT = void* (*)(RE::Character*);
    const REL::Relocation<FuncT> func{RELOCATION_ID(36469, 0)};
    return func(character_);
  };

  let sub_1403BD3C0_actor_value_owner_restore_health =
      [](RE::ActorValueOwner* owner, RE::TESBoundObject* object_reference, void* form_flags) -> float {
    using FuncT = float (*)(RE::ActorValueOwner*, RE::TESBoundObject*, void*);
    const REL::Relocation<FuncT> func{RELOCATION_ID(25830, 0)};
    return func(owner, object_reference, form_flags);
  };

  let sub_1405E1F40_set_bleedout_rate = [](RE::Character* character_, float bleedout_rate) -> void* {
    using FuncT = void* (*)(RE::Character*);
    const REL::Relocation<FuncT> func{RELOCATION_ID(37680, 0)};
    return func(character_);
  };

  switch (av) {
    case RE::ActorValue::kHealth: {
      av_rate = RE::ActorValue::kHealRate;
      av_rate_mult = RE::ActorValue::kHealRateMult;
      entry_point = RE::BGSEntryPoint::ENTRY_POINT::kModRecoveredHealth;

      let combat_health_regen_mult = character->GetActorValue(RE::ActorValue::kCombatHealthRegenMultiply);
      combat_regen_mult = combat_health_regen_mult;

      if (character->IsProtected() && !character->IsInCombat() && !character->IsPlayerRef()) {
        essentialNonCombatHealRateBonus =
            Reflyem::Core::get_float_game_setting("fEssentialNonCombatHealRateBonus").value_or(0.f);
      }

      if (character->IsBleedingOut()) {
        av_rate = RE::ActorValue::kNone;
        if (combat_health_regen_mult == 0.f) {
          combat_regen_mult =
              Reflyem::Core::get_float_game_setting("fEssentialDownCombatHealthRegenMult").value_or(0.f);
        }

        if (character->boolFlags.none(RE::Actor::BOOL_FLAGS::kNoBleedoutRecovery)) {
          let bleedout_rate = get_bleedout_rate(character);
          restore_value = bleedout_rate;

          if (get_bleedout_rate(character) == 0.f) {
            let object_reference = character->data.objectReference;
            let bleedout_rate_setting = Reflyem::Core::get_float_game_setting("fBleedoutRate").value_or(1.f);

            const auto form_flags = sub_1405E1F40_object_ref_form_flags(character);

            let new_bleedout_rate =
                sub_1403BD3C0_actor_value_owner_restore_health(character, object_reference, form_flags) /
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
      combat_regen_mult = Reflyem::Core::get_float_game_setting("fCombatStaminaRegenRateMult").value_or(1.f);
      av_rate = RE::ActorValue::kStaminaRate;
      av_rate_mult = RE::ActorValue::kStaminaRateMult;
      break;
    }
    case RE::ActorValue::kMagicka: {
      combat_regen_mult = Reflyem::Core::get_float_game_setting("fCombatMagickaRegenRateMult").value_or(1.f);
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

  if (av_rate != RE::ActorValue::kNone) {
    restore_value = changed_regen ? character->GetActorValue(av_rate)
                                  : max_actor_value * (character->GetActorValue(av_rate) * 0.01f);
  }

  if (restore_value > 0.f) {
    if (character->IsInCombat() &&
        (av != RE::ActorValue::kHealth || character->race->data.flags.none(RE::RACE_DATA::Flag::kRegenHPInCombat))) {
      restore_value = restore_value * combat_regen_mult;
    }
    if (entry_point != RE::BGSEntryPoint::ENTRY_POINT::kTotal) {
      RE::BGSEntryPoint::HandleEntryPoint(entry_point, character, &restore_value);
    }
    auto mult = 1.f + (actor_value_rate_mult / 100.f);
    if (mult < 0.f) {
      mult = 1.f;
    }
    restore_value = changed_regen ? restore_value * mult : restore_value * (actor_value_rate_mult * 0.01f);
  }

  restore_value = essentialNonCombatHealRateBonus + restore_value;

  logd("Calculate regeneration value for {} it's {}"sv, av, restore_value);

  return restore_value;
}

static bool shouting_from_equip_flag = false;
static bool unequip_after_shout_flag = false;
static constexpr float MAX_VOICE_TIME_ELAPSED = 1.5f;
static float non_combat_timer = 0.f;
static constexpr float MAX_NON_COMBAT_TIMER = 5.f;
static constexpr int MAX_FAKE_BLOCK_FRAMES = 2;
static int frames_with_fake_block_state = 0;

auto OnCharacterUpdate::update_pc(RE::PlayerCharacter* this_, float delta) -> void
{
  letr config = Reflyem::Config::get_singleton();
  if (this_) {

    player_last_delta = delta;
    if (frames_with_fake_block_state >= MAX_FAKE_BLOCK_FRAMES && Reflyem::Core::is_fake_block()) {
      frames_with_fake_block_state = 0;
      Reflyem::Core::set_fake_block_frame(false);
    }

    // logi("PlayerBlocking: {}"sv, this_->IsBlocking());
    Reflyem::ProjectileBlock::update(delta);

    let object_right = this_->GetEquippedObject(false);
    let object_left = this_->GetEquippedObject(true);
    let equip_manager = RE::ActorEquipManager::GetSingleton();
    if (this_->IsPlayerRef() && config.misc_fixes().equip_spell_with_two_handed() && object_right &&
        object_right->IsWeapon() &&
        Reflyem::Core::object_is_two_handed_weapon(object_right->As<RE::TESBoundObject>()) && object_left &&
        object_left->IsArmor()) {
      equip_manager->UnequipObject(this_, object_right->As<RE::TESBoundObject>());
    }

    if (this_->IsPlayerRef() && config.misc_fixes().equip_spell_with_two_handed() && object_right &&
        object_right->IsWeapon() &&
        Reflyem::Core::object_is_two_handed_weapon(object_right->As<RE::TESBoundObject>()) && object_left &&
        object_left->IsWeapon()) {
      equip_manager->UnequipObject(this_, object_left->As<RE::TESBoundObject>());
    }

    if (config.bound_spell_swap().enable() && config.bound_spell_swap().bound_spells() &&
        config.bound_spell_swap().bound_weapons()) {
      auto has_equipped_bound = false;
      let right_equip = this_->GetEquippedObject(false);
      let left_equip = this_->GetEquippedObject(true);
      let size_spells = config.bound_spell_swap().bound_spells()->forms.size();
      let size_weapons = config.bound_spell_swap().bound_weapons()->forms.size();
      if (size_spells == size_weapons) {
        for (uint32_t index = 0; index < size_spells; index++) {
          auto already_equipped = false;
          let spell_form = config.bound_spell_swap().bound_spells()->forms[index];
          let weapon_form = config.bound_spell_swap().bound_weapons()->forms[index];
          if (spell_form && spell_form->As<RE::SpellItem>() && weapon_form && weapon_form->As<RE::TESObjectWEAP>()) {
            if (right_equip && right_equip == weapon_form) {
              already_equipped = true;
              has_equipped_bound = true;
            }
            if (right_equip && right_equip == spell_form) {
              if (!already_equipped) {
                this_->AddObjectToContainer(weapon_form->As<RE::TESBoundObject>(), nullptr, 1, nullptr);
                equip_manager->EquipObject(this_,
                                           weapon_form->As<RE::TESBoundObject>(),
                                           nullptr,
                                           1,
                                           Reflyem::Core::get_right_hand_equip_slot());
                has_equipped_bound = true;
                already_equipped = true;
              }
            }
            if (left_equip && left_equip == spell_form) {
              if (!already_equipped) {
                this_->AddObjectToContainer(weapon_form->As<RE::TESBoundObject>(), nullptr, 1, nullptr);
                equip_manager->EquipObject(this_,
                                           weapon_form->As<RE::TESBoundObject>(),
                                           nullptr,
                                           1,
                                           Reflyem::Core::get_right_hand_equip_slot());
                has_equipped_bound = true;
              }
              equip_manager->UnequipObject(this_, spell_form->As<RE::TESBoundObject>());
            }
          }
        }
        if (!has_equipped_bound) {
          for (uint32_t index = 0; index < size_weapons; index++) {
            let weapon_form = config.bound_spell_swap().bound_weapons()->forms[index];
            if (weapon_form && weapon_form->As<RE::TESBoundObject>()) {
              this_->RemoveItem(
                  weapon_form->As<RE::TESBoundObject>(), 99, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
            }
          }
        }
      }
    }

    if (config.restrict_equip_offhand()) {
      let right_equip = this_->GetEquippedObject(false);
      let left_equip = this_->GetEquippedObject(true);
      if (right_equip && left_equip && right_equip->As<RE::BGSKeywordForm>() && right_equip->As<RE::TESBoundObject>() &&
          left_equip->As<RE::TESObjectWEAP>()) {
        let left_weapon = left_equip->As<RE::TESObjectWEAP>();
        if (right_equip->As<RE::BGSKeywordForm>()->HasKeyword(config.restrict_equip_offhand()) &&
            (left_weapon->IsMelee() || left_weapon->IsBow() || left_weapon->IsCrossbow())) {
          equip_manager->UnequipObject(this_, right_equip->As<RE::TESBoundObject>());
        }
      }
    }

    if (config.restrict_equip_shield()) {
      let right_equip = this_->GetEquippedObject(false);
      let left_equip = this_->GetEquippedObject(true);
      if (right_equip && left_equip && right_equip->As<RE::BGSKeywordForm>() && right_equip->As<RE::TESBoundObject>() &&
          left_equip->IsArmor()) {
        if (right_equip->As<RE::BGSKeywordForm>()->HasKeyword(config.restrict_equip_shield())) {
          equip_manager->UnequipObject(this_, right_equip->As<RE::TESBoundObject>());
        }
      }
    }

    update_actor(*this_, delta, config);

    if (config.death_loot().enable() && config.death_loot().auto_loot() && config.death_loot().storage() &&
        !this_->IsInCombat()) {
      let ui = RE::UI::GetSingleton();
      if (ui && !ui->GameIsPaused() && !ui->IsMenuOpen(RE::TweenMenu::MENU_NAME) &&
          !ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME) && !ui->IsMenuOpen(RE::MagicMenu::MENU_NAME) &&
          !ui->IsMenuOpen(RE::InventoryMenu::MENU_NAME) && !ui->IsMenuOpen(RE::StatsMenu::MENU_NAME) &&
          !ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME) && !ui->IsMenuOpen(RE::FaderMenu::MENU_NAME) &&
          !ui->IsMenuOpen(RE::FavoritesMenu::MENU_NAME))
        non_combat_timer = non_combat_timer + delta;
      if (non_combat_timer >= MAX_NON_COMBAT_TIMER) {
        non_combat_timer = MAX_NON_COMBAT_TIMER;
        if (config.death_loot().storage() && config.death_loot().storage()->GetInventoryCount() > 0) {
          if (config.death_loot().spell_when_loot()) {
            Reflyem::Core::cast(*config.death_loot().spell_when_loot(), *this_, *this_, Reflyem::Core::kNone, 1.f);
          }
          config.death_loot().storage()->GetInventoryChanges()->RemoveAllItems(
              config.death_loot().storage(), this_, true, true, true);
        }
      }
    } else {
      non_combat_timer = 0.f;
    }

    if (config.misc_fixes().cast_shout_on_equip() && shouting_from_equip_flag) {
      auto player_controls = RE::PlayerControls::GetSingleton();
      let ai_process = this_->currentProcess;
      if (ai_process && ai_process->high &&
          (ai_process->high->voiceTimeElapsed <= MAX_VOICE_TIME_ELAPSED && ai_process->high->currentShout)) {

        player_controls->shoutHandler->allow_shouting =
            !Reflyem::Core::player_start_shout(this_, ai_process->high->currentShout);
        player_controls->shoutHandler->still_holding = true;

      } else {

        if (unequip_after_shout_flag && this_->selectedPower && this_->selectedPower->As<RE::TESShout>()) {
          logd("Proc unequip shout");
          if (let equip_manager = RE::ActorEquipManager::GetSingleton()) {
            logd("Start unequip shout: {}", this_->selectedPower->GetName());
            equip_manager->UnEquipShout(this_, this_->selectedPower->As<RE::TESShout>());
          }
        }

        shouting_from_equip_flag = false;
        player_controls->shoutHandler->still_holding = false;
        player_controls->shoutHandler->allow_shouting = !player_controls->shoutHandler->allow_shouting;
      }
    }
  }
  update_pc_(this_, delta);
  auto& actors_cache = Reflyem::Core::ActorsCache::get_singleton();
  auto& actor_data = actors_cache.get_or_add(this_->formID).get();
  if (actor_data.start_dodge_in_next_frame()) {
    actor_data.start_dodge_in_next_frame(false);

    if (config.tk_dodge().interrupt_cast_when_dodge() && Reflyem::Core::is_casting_actor(*this_)) {
      this_->InterruptCast(true);
    }

    this_->SetGraphVariableInt("iStep",
                               config.tk_dodge().step()); // Set Step Dodge
    this_->SetGraphVariableFloat("TKDR_IframeDuration",
                                 Reflyem::TkDodge::get_current_frame_duration()); // Set invulnerable frame duration
    // Send TK Dodge Event
    if (this_->NotifyAnimationGraph(Reflyem::TkDodge::get_current_event_name()) && config.cast_on_dodge().enable()) {
      Reflyem::Core::cast_on_handle_formlists(
          config.cast_on_dodge().formlist_needkw(), config.cast_on_dodge().formlist_spells(), this_, nullptr);
    }
  }
  if (Reflyem::Core::is_fake_block()) {
    frames_with_fake_block_state = frames_with_fake_block_state + 1;
  }
}

auto OnProjectileHit::arrow_handle_hit(RE::ArrowProjectile* arrow, RE::hkpAllCdPointCollector* collidable_collector)
    -> void
{
  logd("arrow_handle_hit"sv);
  if (arrow && collidable_collector &&
      should_ignore_hit(arrow, collidable_collector, Reflyem::ProjectileBlock::ProjectileType::kArrow)) {
    logd("Ignore arrow"sv);
    return;
  }
  return arrow_handle_hit_(arrow, collidable_collector);
}

auto OnProjectileHit::missile_handle_hit(RE::MissileProjectile* missle,
                                         RE::hkpAllCdPointCollector* collidable_collector) -> void
{
  logd("missle_handle_hit");
  if (missle && collidable_collector &&
      should_ignore_hit(missle, collidable_collector, Reflyem::ProjectileBlock::ProjectileType::kMissile)) {
    logd("Ignore missle"sv);
    return;
  }
  return missile_handle_hit_(missle, collidable_collector);
}

auto OnProjectileHit::cone_handle_hit(RE::ConeProjectile* cone, RE::hkpAllCdPointCollector* collidable_collector)
    -> void
{
  logd("cone_handle_hit");
  if (cone && collidable_collector &&
      should_ignore_hit(cone, collidable_collector, Reflyem::ProjectileBlock::ProjectileType::kCone)) {
    logd("Ignore cone"sv);
    return;
  }
  return cone_handle_hit_(cone, collidable_collector);
}

auto OnProjectileHit::beam_handle_hit(RE::BeamProjectile* beam, RE::hkpAllCdPointCollector* collidable_collector)
    -> void
{
  logi("beam_handle_hit");
  if (beam && collidable_collector &&
      should_ignore_hit(beam, collidable_collector, Reflyem::ProjectileBlock::ProjectileType::kBeam)) {
    logi("Ignore beam"sv);
    return;
  }
  return beam_handle_hit_(beam, collidable_collector);
}

auto OnProjectileHit::flame_handle_hit(RE::FlameProjectile* flame, RE::hkpAllCdPointCollector* collidable_collector)
    -> void
{
  logi("flame_handle_hit");
  if (flame && collidable_collector &&
      should_ignore_hit(flame, collidable_collector, Reflyem::ProjectileBlock::ProjectileType::kFlame)) {
    logi("Ignore flame"sv);
    return;
  }
  return flame_handle_hit_(flame, collidable_collector);
}

auto OnCharacterUpdate::update_npc(RE::Character* this_, float delta) -> void
{
  if (this_) {
    auto& config = Reflyem::Config::get_singleton();

    update_actor(*this_, delta, config);
  }
  return update_npc_(this_, delta);
}

auto OnActorUpdateEquipBound::process_event(const RE::ActorInventoryEvent* event,
                                            RE::BSTEventSource<RE::ActorInventoryEvent>* event_source)
    -> RE::BSEventNotifyControl
{
  logger::info("Process bound event"sv);

  letr config = Reflyem::Config::get_singleton();

  if (config.misc_fixes().equip_bound_fix()) {
    return RE::BSEventNotifyControl::kContinue;
  }

  return process_event_(event, event_source);
}

static void update_label(const RE::ContainerMenu* menu, RE::TESObjectARMO* armor)
{
  if (let movie = menu->uiMovie.get(); menu->itemList && movie) {
    RE::GFxValue ApparelArmorLabel;
    RE::GFxValue ApparelArmorValue;
    RE::GFxValue ApparelArmorValueText;
    if (movie->GetVariable(&ApparelArmorValue, "_root.Menu_mc.itemCardFadeHolder.ItemCard_mc.ApparelArmorValue") &&
        movie->GetVariable(&ApparelArmorValueText,
                           "_root.Menu_mc.itemCardFadeHolder.ItemCard_mc.ApparelArmorValue.text")) {
      logi("{} {} {}"sv,
           ApparelArmorValueText.GetNumber(),
           ApparelArmorValueText.GetSInt(),
           ApparelArmorValueText.GetUInt());
      ApparelArmorValue.SetText(std::format("{}/{}", ApparelArmorValueText.GetString(), "10").c_str());
    }
    if (movie->GetVariable(&ApparelArmorLabel, "_root.Menu_mc.itemCardFadeHolder.ItemCard_mc.ApparelArmorLabel")) {
      ApparelArmorLabel.SetMember("htmlText", std::format("{}/{}", "Д", "ВУ").c_str());
    }
  }
}

auto OnContainerMenu::process_message(RE::ContainerMenu* this_, RE::UIMessage& message) -> RE::UI_MESSAGE_RESULTS
{
  logi("Container menu process"sv);

  if (!this_ || !this_->itemCard || !this_->itemList) {
    logi("Container menu process some null"sv);
    return process_message_(this_, message);
  }

  if (let selected_item = this_->itemList->GetSelectedItem()) {
    logi("Selected Item not null: {}"sv, selected_item->data.GetName());
    if (selected_item->data.objDesc && selected_item->data.objDesc->object &&
        selected_item->data.objDesc->object->IsArmor()) {
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

  if (!this_ || !this_->itemCard || !this_->itemList) {
    logi("Container menu accept some null"sv);
    return;
  }

  if (let selected_item = this_->itemList->GetSelectedItem()) {
    logi("Selected Item not null: {}"sv, selected_item->data.GetName());
    if (selected_item->data.objDesc && selected_item->data.objDesc->object &&
        selected_item->data.objDesc->object->IsArmor()) {
      logi("Selected armor, start change, old text: {}"sv, this_->itemCard->infoText.c_str());
      this_->itemCard->infoText = "BzIk ChOrT";
    }
  }
}

// 0 = none, 1 = power, 2 = left, 3 = right, 4 = both

auto OnStandardItemData::get_equip_state(RE::StandardItemData* this_) -> std::uint32_t
{
  if (this_) {
    logi("OnStandardItemData:Check equip state for: {}"sv, this_->GetName());
    if (this_->objDesc && Reflyem::Core::object_is_two_handed_weapon(this_->objDesc->object)) {
      return 4;
    }
  }
  return get_equip_state_(this_);
}

auto OnMagicItemData::get_equip_state(RE::StandardItemData* this_) -> std::uint32_t
{
  if (this_) {
    logi("OnMagicItemData:Check equip state for: {}"sv, this_->GetName());
  }
  // return 4;
  return get_equip_state_(this_);
}

auto OnFavoriteMenuEquipState::get_equip_state_01(RE::Actor* actor, RE::TESBoundObject* object, void* a3, void* a4)
    -> std::uint32_t
{
  if (object) {
    logi("OnFavoriteMenuEquipState1:Check state for: {}"sv, object->GetName());
  }
  return get_equip_state_01_(actor, object, a3, a4);
}

auto OnFavoriteMenuEquipState::get_equip_state_02(RE::Actor* actor, RE::TESBoundObject* object, void* a3, void* a4)
    -> std::uint32_t
{
  if (object) {
    logi("OnFavoriteMenuEquipState2:Check state for: {}"sv, object->GetName());
  }
  return get_equip_state_02_(actor, object, a3, a4);
}

auto OnFavoriteMenuEquipState::get_equip_state_03(RE::Actor* actor, RE::TESBoundObject* object, void* a3, void* a4)
    -> std::uint32_t
{
  if (object) {
    logi("OnFavoriteMenuEquipState3:Check state for: {}"sv, object->GetName());
  }
  return get_equip_state_03_(actor, object, a3, a4);
}

static RE::TESBoundObject* object_no_give_gold = nullptr;
static bool flag_no_give_gold = false;
auto OnActorAddRemoveItems::add_object_to_container(RE::Actor* this_,
                                                    RE::TESBoundObject* object,
                                                    RE::ExtraDataList* extra_list,
                                                    int32_t count,
                                                    RE::TESObjectREFR* from_refr) -> void
{
  logi("AddObject: Count: {}", count);

  letr config = Reflyem::Config::get_singleton();
  if (RE::UI::GetSingleton()->IsMenuOpen(RE::BarterMenu::MENU_NAME) &&
      config.misc_fixes().enable_unlimited_merchant_gold() && object && object_no_give_gold &&
      object->GetFormID() == object_no_give_gold->GetFormID()) {
    logi("UnlimitedMerchant: OnActorAddRemoveItems::add_object_to_container flag_no_give_gold = true");
    flag_no_give_gold = true;
  }

  return add_object_to_container_(this_, object, extra_list, count, from_refr);
  // constexpr RE::FormID container_form_id = 0x2EDD7;
  constexpr RE::FormID container_form_id = 0x5C132;
  const auto data_handler = RE::TESDataHandler::GetSingleton();
  const auto container = data_handler->LookupForm<RE::TESObjectREFR>(container_form_id, "Skyrim.esm"sv);

  const auto alchemy = object->As<RE::AlchemyItem>();
  if (alchemy) {
    logger::info("Add alchemy object: {}, count: {}"sv, alchemy->GetFullName(), count);
    // if (container->AddObjectToContainer(object, count, this_)) {
    //   logger::info("Add to container"sv);
    //   return;
    // }
    // logger::info("Not add to container"sv);
    container->AddObjectToContainer(object, nullptr, count, nullptr);
    return;
  } else {
    logger::info("Null alchemy: {}"sv, object->GetName());
  }

  return add_object_to_container_(this_, object, extra_list, count, from_refr);
}

auto OnActorAddRemoveItems::add_object_to_container_ref(RE::TESObjectREFR* this_,
                                                        RE::TESBoundObject* object,
                                                        RE::ExtraDataList* extra_list,
                                                        int32_t count,
                                                        RE::TESObjectREFR* from_refr) -> void
{
  logi("AddObjectRef: Count: {}", count);
  letr config = Reflyem::Config::get_singleton();
  if (RE::UI::GetSingleton()->IsMenuOpen(RE::BarterMenu::MENU_NAME) &&
      config.misc_fixes().enable_unlimited_merchant_gold() && object && flag_no_give_gold && object_no_give_gold &&
      object->IsGold()) {
    logi("UnlimitedMerchant: OnActorAddRemoveItems::add_object_to_container_ref flag_no_give_gold = false "
         "object_no_give_gold = nullptr count = -1");
    flag_no_give_gold = false;
    object_no_give_gold = nullptr;
    count = -1;
    if (this_) {
      logi("UnlimitedMerchant: OnActorAddRemoveItems::add_object_to_container_ref add_object_to_container_ref_(this_, "
           "object, extra_list, 1, from_refr)");
      add_object_to_container_ref_(this_, object, extra_list, 1, from_refr);
    }
  }
  return add_object_to_container_ref_(this_, object, extra_list, count, from_refr);
}

auto OnActorAddRemoveItems::add_object_to_container_npc(RE::Actor* this_,
                                                        RE::TESBoundObject* object,
                                                        RE::ExtraDataList* extra_list,
                                                        int32_t count,
                                                        RE::TESObjectREFR* from_refr) -> void
{
  logi("AddObjectNpc: Count: {}", count);
  return add_object_to_container_npc_(this_, object, extra_list, count, from_refr);
}

struct GetActivateText
{
  static bool thunk(RE::TESBoundObject* a_this, RE::TESObjectREFR* ref_object, RE::BSString& a_dst)
  {

    logd("GetActivateText mode: {}", Reflyem::DeclutteredLoot::declutter_mode);
    letr config = Reflyem::Config::get_singleton();
    if (config.decluttered_loot().enable() && Reflyem::DeclutteredLoot::declutter_mode &&
        Reflyem::DeclutteredLoot::is_clutter_object(ref_object)) {
      logd("Change label");
      a_dst = "";
      return false;
    }

    return func(a_this, ref_object, a_dst);
  }
  static inline REL::Relocation<decltype(thunk)> func;

  static inline constexpr std::size_t size = 0x4C;
};

struct UseSkill
{
  static void thunk(RE::PlayerCharacter* player, RE::ActorValue skill_av, float value, RE::TESForm* form)
  {

    if (!player) {
      logi("UseSkill no player");
      return func(player, skill_av, value, form);
    }
    letr config = Reflyem::Config::get_singleton();
    if (skill_av == RE::ActorValue::kBlock && config.magic_blocker().enable() &&
        config.magic_blocker().magic_blocker_keyword() &&
        Reflyem::Core::try_has_absolute_keyword(player, config.magic_blocker().magic_blocker_keyword())) {
      logd("Magic block active, no use block");
      return;
    }

    return func(player, skill_av, value, form);
  }
  static inline REL::Relocation<decltype(thunk)> func;

  static inline constexpr std::size_t size = 0xF7;
};

struct GetActivate
{
  static bool thunk(RE::TESBoundObject* a_this,
                    RE::TESObjectREFR* a_targetRef,
                    RE::TESObjectREFR* ref_object,
                    std::uint8_t a_arg3,
                    RE::TESBoundObject* a_object,
                    std::int32_t a_targetCount)
  {

    logd("GetActivate mode: {}", Reflyem::DeclutteredLoot::declutter_mode);
    if (a_targetRef && !a_targetRef->IsPlayerRef()) {
      logi("Activate no player");
      return func(a_this, a_targetRef, ref_object, a_arg3, a_object, a_targetCount);
    }
    letr config = Reflyem::Config::get_singleton();
    if (config.decluttered_loot().enable() && Reflyem::DeclutteredLoot::declutter_mode &&
        Reflyem::DeclutteredLoot::is_clutter_object(ref_object)) {
      logd("No activate");
      return false;
    }

    return func(a_this, a_targetRef, ref_object, a_arg3, a_object, a_targetCount);
  }
  static inline REL::Relocation<decltype(thunk)> func;

  static inline constexpr std::size_t size = 0x37;
};

struct SummonCreatureEffectUpdate
{
  static void thunk(RE::SummonCreatureEffect* active_effect, float delta)
  {

    letr config = Reflyem::Config::get_singleton();
    if (config.ki_energy_power().enable() && config.ki_energy_power().enable_ki_summons()) {
      Reflyem::KiEnergyPower::update_summon_effect(active_effect, delta);
    }

    return func(active_effect, delta);
  }
  static inline REL::Relocation<decltype(thunk)> func;

  static inline constexpr std::size_t size = 0x04;
};

auto OnActorAddRemoveItems::pick_up_object(RE::Actor* this_,
                                           RE::TESObjectREFR* ref_object,
                                           int32_t count,
                                           bool arg3,
                                           bool play_sound) -> void
{
  logi("In pick_up_object"sv);
  if (count > 0 && ref_object && this_ && this_->IsPlayerRef()) {
    letr config = Reflyem::Config::get_singleton();
    if (config.decluttered_loot().enable() && Reflyem::DeclutteredLoot::declutter_mode &&
        Reflyem::DeclutteredLoot::is_clutter_object(ref_object)) {
      logi("Item {} skipped", ref_object->GetDisplayFullName());
      return;
    }
    // let form_type = ref_object->GetFormType();
    // if (form_type != RE::TESObjectARMO::FORMTYPE && form_type != RE::TESObjectWEAP::FORMTYPE &&
    //     form_type != RE::TESAmmo::FORMTYPE && form_type != RE::AlchemyItem::FORMTYPE &&
    //     form_type != RE::ScrollItem::FORMTYPE && form_type != RE::FormType::Book &&
    //     form_type != RE::FormType::SoulGem && form_type != RE::FormType::Light &&
    //     form_type != RE::FormType::Activator && !ref_object->IsGold() && !ref_object->HasQuestObject() &&
    //     !ref_object->IsEnchanted() && !ref_object->IsLockpick() &&
    //     !Reflyem::Core::try_form_has_keyword(ref_object, Reflyem::Core::get_gem_keyword()) &&
    //     !Reflyem::Core::try_form_has_keyword(ref_object, Reflyem::Core::get_ore_ingot_keyword()) &&
    //     !Reflyem::Core::is_script_attached(ref_object)) {
    //   logi("Item {} skipped", ref_object->GetDisplayFullName());
    //   return;
    // }
  }
  return pick_up_object_(this_, ref_object, count, arg3, play_sound);

  // constexpr RE::FormID container_form_id = 0x2EDD7;
  constexpr RE::FormID container_form_id = 0x5C132;
  const auto data_handler = RE::TESDataHandler::GetSingleton();
  auto container = data_handler->LookupForm<RE::TESObjectREFR>(container_form_id, "Skyrim.esm"sv);

  const auto bound_object = ref_object->GetObjectReference();
  if (bound_object) {
    const auto alchemy = bound_object->As<RE::AlchemyItem>();
    if (alchemy) {
      logger::info("Add alchemy object: {} count: {}"sv, alchemy->GetFullName(), count);
      // if (container->AddObjectToContainer(bound_object, count, this_)) {
      //   logger::info("Add to container"sv);
      //   return;
      // }
      // logger::info("Not add to container"sv);
      container->AddObjectToContainer(bound_object, nullptr, count, nullptr);
      count = 0;
    } else {
      logger::info("Pick null alchemy: {}"sv, ref_object->GetName());
    }
  }

  return pick_up_object_(this_, ref_object, count, arg3, play_sound);
}

auto OnActorAddRemoveItems::remove_item(RE::Actor* this_,
                                        RE::TESBoundObject* item,
                                        std::int32_t count,
                                        RE::ITEM_REMOVE_REASON reason,
                                        RE::ExtraDataList* extra_list,
                                        RE::TESObjectREFR* move_to_ref,
                                        const RE::NiPoint3* drop_loc,
                                        const RE::NiPoint3* rotate) -> RE::ObjectRefHandle
{

  letr config = Reflyem::Config::get_singleton();

  // logi("Start RemoveItem Count: {}", count);

  if (config.poison_rework().enable() && extra_list) {
    PoisonReworkVisitor::handle_extra_data_list(extra_list, true);
  }

  if (this_ && reason == RE::ITEM_REMOVE_REASON::kRemove && !move_to_ref && !drop_loc && !rotate && count > 0 && item) {

    if (config.potions_drink_limit().enable() && config.potions_drink_limit().no_remove_keyword()) {

      let alchemy_item = item->As<RE::AlchemyItem>();
      if (alchemy_item && alchemy_item->HasKeyword(config.potions_drink_limit().no_remove_keyword())) {

        logd("No remove alchemy item: {}"sv, alchemy_item->GetFullName());

        return remove_item_(this_, item, 0, reason, extra_list, move_to_ref, drop_loc, rotate);
      }
    }

    if (config.poison_rework().enable() && config.poison_rework().no_remove_keyword()) {

      let alchemy_item = item->As<RE::AlchemyItem>();
      if (alchemy_item && alchemy_item->HasKeyword(config.poison_rework().no_remove_keyword())) {

        logd("No remove poison alchemy item: {}"sv, alchemy_item->GetFullName());

        return remove_item_(this_, item, 0, reason, extra_list, move_to_ref, drop_loc, rotate);
      }
    }
  }

  if (RE::UI::GetSingleton()->IsMenuOpen(RE::BarterMenu::MENU_NAME) &&
      config.misc_fixes().enable_unlimited_merchant_gold() && reason == RE::ITEM_REMOVE_REASON::kSelling && count > 0 &&
      item && item->IsGold()) {
    logi("UnlimitedMerchant: OnActorAddRemoveItems::remove_item before if");
    if (move_to_ref && !move_to_ref->IsPlayerRef() && this_ && this_->IsPlayerRef()) {
      logi("UnlimitedMerchant: OnActorAddRemoveItems::remove_item after if");
      remove_item_ref_(this_, item, count, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr, nullptr, nullptr);
      logi("UnlimitedMerchant: OnActorAddRemoveItems::remove_item after after if");
      count = 0;
    }
  }

  // logi("End RemoveItem Count: {}", count);

  return remove_item_(this_, item, count, reason, extra_list, move_to_ref, drop_loc, rotate);
}

auto OnActorAddRemoveItems::remove_item_npc(RE::Actor* this_,
                                            RE::TESBoundObject* item,
                                            std::int32_t count,
                                            RE::ITEM_REMOVE_REASON reason,
                                            RE::ExtraDataList* extra_list,
                                            RE::TESObjectREFR* move_to_ref,
                                            const RE::NiPoint3* drop_loc,
                                            const RE::NiPoint3* rotate) -> RE::ObjectRefHandle
{
  // logi("Start RemoveItemNpc Count: {}", count);
  // letr config = Reflyem::Config::get_singleton();

  // logi("RemoveItemNPC");
  // if (config.misc_fixes().enable_unlimited_merchant_gold() && reason == RE::ITEM_REMOVE_REASON::kSelling && count > 0
  // &&
  //     item && item->IsGold()) {
  //   logi("RemoveItemNPC step1");
  //   if (move_to_ref && move_to_ref->IsPlayerRef() && this_ && !this_->IsPlayerRef()) {
  //     logi("RemoveItemNPC step2");
  //     let object_manager = RE::BGSDefaultObjectManager::GetSingleton();
  //     if (object_manager && object_manager->objectInit[RE::BGSDefaultObjectManager::DefaultObject::kGold] &&
  //         object_manager->objects[RE::BGSDefaultObjectManager::DefaultObject::kGold]) {
  //       logi("RemoveItemNPC step3");
  //       let gold = object_manager->objects[RE::BGSDefaultObjectManager::DefaultObject::kGold];
  //       this_->AddObjectToContainer(gold->As<RE::TESBoundObject>(), nullptr, count, nullptr);
  //     }
  //   }
  // }

  return remove_item_npc_(this_, item, count, reason, extra_list, move_to_ref, drop_loc, rotate);
}

auto OnActorAddRemoveItems::remove_item_ref(RE::TESObjectREFR* this_,
                                            RE::TESBoundObject* item,
                                            std::int32_t count,
                                            RE::ITEM_REMOVE_REASON reason,
                                            RE::ExtraDataList* extra_list,
                                            RE::TESObjectREFR* move_to_ref,
                                            const RE::NiPoint3* drop_loc,
                                            const RE::NiPoint3* rotate) -> RE::ObjectRefHandle
{

  letr config = Reflyem::Config::get_singleton();

  // logi("Start RemoveItemRef Count: {}", count);

  if (RE::UI::GetSingleton()->IsMenuOpen(RE::BarterMenu::MENU_NAME) &&
      config.misc_fixes().enable_unlimited_merchant_gold() && reason == RE::ITEM_REMOVE_REASON::kSelling && count > 0 &&
      item) {

    logi("UnlimitedMerchant: OnActorAddRemoveItems::remove_item_ref before if");

    if (move_to_ref && move_to_ref->IsPlayerRef() && this_ && !this_->IsPlayerRef() && item->IsGold()) {
      logi("UnlimitedMerchant: OnActorAddRemoveItems::remove_item_ref after if");
      move_to_ref->AddObjectToContainer(item, nullptr, count, nullptr);
      logi("UnlimitedMerchant: OnActorAddRemoveItems::remove_item_ref after after if");
      count = 0;
    }

    if (move_to_ref && move_to_ref->IsPlayerRef()) {
      logi("UnlimitedMerchant: OnActorAddRemoveItems::remove_item_ref object_no_give_gold = item");
      object_no_give_gold = item;
    }
  }

  logd("RemoveItemREF step4");
  return remove_item_ref_(this_, item, count, reason, extra_list, move_to_ref, drop_loc, rotate);
}

auto OnDrinkPotion::drink_potion(RE::Actor* this_, RE::AlchemyItem* potion, RE::ExtraDataList* extra_data_list) -> bool
{
  if (!this_ || !potion) {
    return drink_potion_(this_, potion, extra_data_list);
  }

  letr config = Reflyem::Config::get_singleton();
  if (config.potions_drink_limit().enable()) {
    return Reflyem::PotionsDrinkLimit::drink_potion(this_, potion, extra_data_list, config) &&
           drink_potion_(this_, potion, extra_data_list);
  }

  return drink_potion_(this_, potion, extra_data_list);
}
auto OnShoutHandler::process_button(RE::ShoutHandler* handler,
                                    RE::ButtonEvent* button_event,
                                    RE::PlayerControlsData* controls_data) -> void
{
  if (!handler || !button_event || !controls_data) {
    return process_button_(handler, button_event, controls_data);
  }

  // logi("Shout process button, Unk10:{} Unk18{}", handler->allow_shouting, handler->unk18);

  let player = RE::PlayerCharacter::GetSingleton();
  let process = player->currentProcess;
  if (process->high->voiceState != RE::VOICE_STATE::kNone) {
    // logi("Shout process not none");
    button_event->heldDownSecs = 0.5f;
    button_event->value = 0.0f;
  }

  return process_button_(handler, button_event, controls_data);
}

auto OnSetCurrentSpell::set_spell_impl_pc(RE::ActorMagicCaster* caster,
                                          RE::MagicItem* magic_item,
                                          RE::TESObjectREFR* ref,
                                          bool a4) -> bool
{
  // logi("In set current spell implPC"sv);
  if (magic_item) {
    logi("SpellSetPC: {}"sv, magic_item->fullName.c_str());
  }
  return set_spell_impl_pc_(caster, magic_item, ref, a4);
}

static RE::MagicItem* staff_spell = nullptr;

auto OnActorMagicCaster::on_set_spell_impl(RE::ActorMagicCaster* caster, RE::MagicItem* magic_item) -> void
{
  logi("On set spell impl"sv);

  if (staff_spell) {
    caster->currentSpell = magic_item;
    caster->actor->selectedSpells[static_cast<int>(caster->castingSource)] = magic_item;
    return on_set_spell_impl_(caster, staff_spell);
  }
  return on_set_spell_impl_(caster, magic_item);
}

auto OnSetCurrentSpell::set_spell_impl_block(RE::ActorMagicCaster* caster,
                                             RE::MagicItem* magic_item,
                                             RE::TESObjectREFR* ref,
                                             bool a4) -> bool
{
  // logi("In set current spell impl BLOCK"sv);
  if (magic_item) {
    logi("SpellSetBLOCK: {}"sv, magic_item->fullName.c_str());
  }
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

auto OnSetCurrentSpell::set_spell_impl_fun(RE::ActorMagicCaster* caster,
                                           RE::MagicItem* magic_item,
                                           RE::TESObjectREFR* ref,
                                           bool a4) -> bool
{
  // logi("In set current spell impl FUN"sv);
  if (magic_item) {
    logi("SpellSetFUN: {}"sv, magic_item->fullName.c_str());
  }
  return set_spell_impl_FUN_(caster, staff_spell ? staff_spell : magic_item, ref, a4);
}

auto OnPoisonRework::get_poison_item_for_decrement(RE::ExtraDataList* list) -> RE::AlchemyItem*
{
  logi("OnDecrementPoisonCount entry"sv);
  if (Reflyem::Config::get_singleton().poison_rework().enable()) {
    return nullptr;
  }
  return get_poison_item_for_decrement_(list);
}

auto OnPoisonRework::poison_object(RE::InventoryEntryData* data, RE::AlchemyItem* poison, int count) -> void
{
  if (data && poison && count > 0) {
    let config = Reflyem::Config::get_singleton();
    let player = RE::PlayerCharacter::GetSingleton();
    if (config.poison_rework().enable() && config.poison_rework().apply_on_both_weapon() && player) {
      Reflyem::Core::poison_object(player->GetEquippedEntryData(false), poison, count);
      Reflyem::Core::poison_object(player->GetEquippedEntryData(true), poison, count);
    }
  }
  return poison_object_(data, poison, count);
}

auto OnPoisonRework::poison_prompt(RE::Actor* actor, RE::AlchemyItem* poison) -> void
{
  logi("Poison prompt entry"sv);
  if (actor && poison && Reflyem::Config::get_singleton().poison_rework().enable()) {

    auto allow = true;

    let data = Reflyem::Core::get_current_equip_weapon(actor->currentProcess, false);

    if (data && data->object) {

      logi("Data name: {}", data->GetDisplayName());

      let weapon_object = data->object->As<RE::TESObjectWEAP>();
      if (weapon_object) {
        let allow_anim_type = weapon_object->weaponData.animationType.any(RE::WEAPON_TYPE::kOneHandAxe,
                                                                          RE::WEAPON_TYPE::kOneHandDagger,
                                                                          RE::WEAPON_TYPE::kOneHandMace,
                                                                          RE::WEAPON_TYPE::kTwoHandSword,
                                                                          RE::WEAPON_TYPE::kOneHandSword,
                                                                          RE::WEAPON_TYPE::kCrossbow,
                                                                          RE::WEAPON_TYPE::kBow,
                                                                          RE::WEAPON_TYPE::kTwoHandAxe,
                                                                          RE::WEAPON_TYPE::kHandToHandMelee);
        if (!allow_anim_type) {
          allow = false;
        }
      }
      if (data->IsPoisoned()) {
        allow = false;
      }
    } else {
      allow = false;
    }

    if (allow) {
      if (Reflyem::PoisonRework::poison_prompt(actor, poison, Reflyem::Config::get_singleton())) {
        logi("Skip poison"sv);
        return;
      }
    }

    logi("Allow poison"sv);
  }

  return poison_prompt_(actor, poison);
}

auto OnAttackData::process_attack(RE::ActorValueOwner* value_owner, RE::BGSAttackData* attack_data) -> void
{
  process_attack_(value_owner, attack_data);
}

auto OnSpellEquip::equip_spell_left(RE::Actor* actor, RE::Actor::SlotTypes slot_type, RE::MagicItem* magic_item) -> void
{
  logi("In on spell equip left call"sv);

  if (!magic_item || !actor) {
    return equip_spell_left_(actor, slot_type, magic_item);
  }

  logi("EquipSpell: {}", magic_item->GetFullName());

  let bound_weapon = Reflyem::Core::get_effect_assoc_item_bound_weapon(magic_item);
  if (bound_weapon) {
    logi("Equip Bound: {}", bound_weapon->GetFullName());
    let actor_equip_manager = RE::ActorEquipManager::GetSingleton();
    if (actor_equip_manager) {
      equip_spell_left_(actor, slot_type, magic_item);
      // auto equip_slot = slot_type == RE::MagicSystem::CastingSource::kRightHand
      //                       ? Reflyem::Core::get_right_hand_equip_slot()
      //                       : Reflyem::Core::get_left_hand_equip_slot();
      actor_equip_manager->EquipObject(actor, bound_weapon);
      return;
    }
  }

  logi("On spell equip left call end"sv);
  return equip_spell_left_(actor, slot_type, magic_item);
}

auto OnSpellEquip::equip_spell_right(RE::Actor* actor, RE::Actor::SlotTypes slot_type, RE::MagicItem* magic_item)
    -> void
{
  logi("In on spell equip right call"sv);

  if (!magic_item || !actor) {
    return equip_spell_right_(actor, slot_type, magic_item);
  }

  logi("EquipSpell: {}", magic_item->GetFullName());

  let bound_weapon = Reflyem::Core::get_effect_assoc_item_bound_weapon(magic_item);
  if (bound_weapon) {
    logi("Equip Bound: {}", bound_weapon->GetFullName());
    let actor_equip_manager = RE::ActorEquipManager::GetSingleton();
    if (actor_equip_manager) {
      equip_spell_right_(actor, slot_type, magic_item);
      // auto equip_slot = slot_type == RE::MagicSystem::CastingSource::kRightHand
      //                       ? Reflyem::Core::get_right_hand_equip_slot()
      //                       : Reflyem::Core::get_left_hand_equip_slot();
      actor_equip_manager->EquipObject(actor, bound_weapon);
      return;
    }
  }

  logi("On spell equip right call end"sv);
  return equip_spell_right_(actor, slot_type, magic_item);
}

auto OnActorEquipManager::equip_object(RE::ActorEquipManager* equip_manager,
                                       RE::Actor* actor,
                                       RE::TESBoundObject* bound_object,
                                       ExtraData* extra_data) -> void
{
  logi("In on some equip call"sv);

  letr config = Reflyem::Config::get_singleton();

  if (actor && actor->IsPlayerRef() && !actor->IsPlayerRef() && bound_object && extra_data &&
      config.misc_fixes().equip_spell_with_two_handed()) {
    let equip_slot = extra_data->slot;
    let right_slot = Reflyem::Core::get_right_hand_equip_slot();
    let left_slot = Reflyem::Core::get_left_hand_equip_slot();
    logd("Equip Check 1: {}", bound_object->GetName());

    if (!bound_object->IsMagicItem() && equip_slot &&
        (bound_object->IsWeapon() || Reflyem::Core::object_is_shield(bound_object))) {
      logd("Equip check 2");

      if (Reflyem::Core::equip_slot_comparer(equip_slot, right_slot)) {
        logd("Equip check 6");
        if (Reflyem::Core::object_is_two_handed_weapon(bound_object)) {
          logd("Equip check 6:1");
          let object_left = actor->GetEquippedObject(true);
          if (object_left && (object_left->IsArmor() || object_left->IsWeapon())) {
            equip_manager->UnequipObject(actor, object_left->As<RE::TESBoundObject>());
          }
        }
      }

      if (Reflyem::Core::equip_slot_comparer(equip_slot, left_slot)) {
        logd("Equip check 3");
        let object_right = actor->GetEquippedObject(false);
        if (object_right) {
          logd("Equip check 4");

          if (Reflyem::Core::object_is_two_handed_weapon(bound_object)) {
            logd("Equip check 5:1");
            extra_data->slot = right_slot;
            logd("Equip check 5:1:test");
            let object_left = actor->GetEquippedObject(true);
            if (object_left && (object_left->IsArmor() || object_left->IsWeapon())) {
              equip_manager->UnequipObject(actor, object_left->As<RE::TESBoundObject>());
            }
          } else {
            logd("Equip check 5:2");
            if (Reflyem::Core::object_is_two_handed_weapon(object_right->As<RE::TESBoundObject>())) {
              equip_manager->UnequipObject(actor, object_right->As<RE::TESBoundObject>());
            }
          }
        }
      }
    }
  }

  return some_equip_(equip_manager, actor, bound_object, extra_data);
}

auto OnActorEquipManager::equip_object_default_slot(RE::ActorEquipManager* equip_manager,
                                                    RE::Actor* actor,
                                                    RE::TESBoundObject* bound_object) -> RE::BGSEquipSlot*
{
  // logi("In default equip"sv);
  // if (!equip_manager || !actor || !bound_object) {
  return some_equip_default_equip_(equip_manager, actor, bound_object);
  // }

  // let left_equip = Reflyem::Core::get_left_hand_equip_slot();
  // let return_value = some_equip_default_equip_(equip_manager, actor, bound_object);
  // if (left_equip == return_value) {
  //   return return_value;
  // }
  //
  // if (!bound_object->IsWeapon()) {
  //   return return_value;
  // }
  //
  // let weapon = bound_object->As<RE::TESObjectWEAP>();
  // if (!weapon) {
  //   return return_value;
  // }
  //
  // if (weapon->GetWeaponType() == RE::WEAPON_TYPE::kStaff) {
  //   // logi("Weapon is staff"sv);
  //   return left_equip;
  // }
  //
  // return return_value;
}

static bool skip_insta_cast = false;
auto OnActorEquipManagerEquipSpell::equip_spell(RE::ActorEquipManager* equip_manager,
                                                RE::Actor* actor,
                                                RE::SpellItem* spell_item,
                                                RE::BGSEquipSlot** equip_slot) -> void
{

  if (!spell_item || !actor || !equip_manager || !equip_slot) {
    return equip_spell_(equip_manager, actor, spell_item, equip_slot);
  }

  // let bound_weapon = Reflyem::Core::get_effect_assoc_item_bound_weapon(spell_item);
  // if (bound_weapon) {
  //   logi("Equip Bound: {}", bound_weapon->GetFullName());
  //
  //   equip_spell_(equip_manager, actor, spell_item, equip_slot);
  //   actor->AddObjectToContainer(bound_weapon, nullptr, 1, nullptr);
  //   equip_manager->EquipObject(actor, bound_weapon, nullptr, 1, *equip_slot);
  //   // player_equip_manager.set_up_manager(*equip_slot, bound_weapon);
  //   return;
  // }

  using SpellType = RE::MagicSystem::SpellType;

  letr config = Reflyem::Config::get_singleton();
  let ui = RE::UI::GetSingleton();
  if (config.misc_fixes().cast_ability_on_equip() && !config.misc_fixes().cast_ability_on_equip_instant() && ui &&
      !ui->GameIsPaused() && actor->IsPlayerRef()) {
    let spell_type = spell_item->GetSpellType();
    if (spell_type == SpellType::kPower || spell_type == SpellType::kLesserPower ||
        spell_type == SpellType::kVoicePower) {
      let magic_caster = actor->magicCasters[static_cast<int>(RE::MagicSystem::CastingSource::kOther)];
      let instant_caster = actor->magicCasters[static_cast<int>(RE::MagicSystem::CastingSource::kInstant)];
      if (magic_caster) {
        // equip_spell_(equip_manager, actor, spell_item, equip_slot);
        unequip_after_shout_flag = false;
        if (skip_insta_cast) {
          skip_insta_cast = false;
          return equip_spell_(equip_manager, actor, spell_item, equip_slot);
        }

        if (config.misc_fixes().cast_ability_on_equip_instant() && instant_caster && instant_caster->actor) {

          if (config.ki_energy_power().enable()) {
            let result = Reflyem::KiEnergyPower::use_ki_energy(actor, spell_item);
            if (result == Reflyem::KiEnergyPower::kOk) {
              instant_caster->CastSpellImmediate(spell_item, false, nullptr, 1.f, false, 0.f, actor);
              return;
            }
            if (result == Reflyem::KiEnergyPower::kFail) {
              return;
            }
          }

          auto cost_av = RE::ActorValue::kMagicka;
          if (config.special_techniques().enable() && config.special_techniques().stamina_cost_spell_keyword() &&
              spell_item->HasKeyword(config.special_techniques().stamina_cost_spell_keyword())) {
            cost_av = RE::ActorValue::kStamina;
          }

          let cost = spell_item->CalculateMagickaCost(instant_caster->actor);
          if (config.misc_fixes().cast_ability_magicka_cost() && cost > instant_caster->actor->GetActorValue(cost_av)) {

            if (instant_caster->actor->IsPlayerRef()) {
              Reflyem::Core::play_sound_base(config.caster_additions().mag_fail_sound(), instant_caster->actor);
              Reflyem::Core::flash_hud_meter(cost_av);
            }

            return;
          }

          if (config.misc_fixes().cast_ability_magicka_cost()) {
            Reflyem::Core::damage_actor_value(*instant_caster->actor, cost_av, cost);
          }
          instant_caster->CastSpellImmediate(spell_item, false, nullptr, 1.f, false, 0.f, actor);
          return;
        }

        Reflyem::Core::direct_apply_spell_on_target(magic_caster, spell_item, nullptr, nullptr, false);
        return;
      }
    }
  }
  return equip_spell_(equip_manager, actor, spell_item, equip_slot);
}

auto OnEquipSpell47C5D7::papyrus_equip_spell(RE::ActorEquipManager* equip_manager,
                                             RE::Actor* actor,
                                             RE::TESBoundObject* spell_item,
                                             RE::MagicSystem::CastingSource source) -> void
{
  letr config = Reflyem::Config::get_singleton();
  logi("Papyrus call");
  if (config.misc_fixes().cast_ability_on_equip() && actor && actor->IsPlayerRef() &&
      source == RE::MagicSystem::CastingSource::kOther) {
    logi("Papyrus call flag insta skip");
    skip_insta_cast = true;
  }
  papyrus_equip_spell_(equip_manager, actor, spell_item, source);
}

// if (config.misc_fixes().cast_shout_on_equip() && ui && !ui->GameIsPaused() && !player_controls->blockPlayerInput &&
//     player->IsPlayerRef() && player->GetFlyState() == RE::FLY_STATE::kNone &&
//     player->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal &&
//     player->GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal && !player->IsSwimming() && !player->IsInKillMove() &&
//     (!actor->IsOnMount() || Reflyem::Core::actor_is_on_flying_mount(actor))) {

auto OnActorEquipManagerEquipShout::equip_shout(RE::ActorEquipManager* equip_manager,
                                                RE::Actor* actor,
                                                RE::TESShout* shout,
                                                void* a4) -> void
{

  if (!shout || !actor || !equip_manager || !a4) {
    return equip_shout_(equip_manager, actor, shout, a4);
  }

  letr config = Reflyem::Config::get_singleton();
  let ui = RE::UI::GetSingleton();
  let player_controls = RE::PlayerControls::GetSingleton();

  if (config.misc_fixes().cast_shout_on_equip() && ui && !ui->GameIsPaused() && actor->IsPlayerRef() &&
      actor->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal &&
      actor->GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal && !actor->IsInKillMove() &&
      (!actor->IsOnMount() || Reflyem::Core::actor_is_on_flying_mount(actor))) {

    let ai_process = actor->currentProcess;
    if (ai_process && ai_process->high && !ai_process->high->currentShout &&
        ai_process->high->voiceTimeElapsed <= 0.f) {

      equip_shout_(equip_manager, actor, shout, a4);
      player_controls->shoutHandler->allow_shouting =
          !Reflyem::Core::player_start_shout(RE::PlayerCharacter::GetSingleton(), shout);
      player_controls->shoutHandler->still_holding = true;
      shouting_from_equip_flag = true;
      unequip_after_shout_flag = true;

      return;
    }
  }

  return equip_shout_(equip_manager, actor, shout, a4);
}

auto OnEquipSpell47C5D7::equip_spell_47(RE::ActorEquipManager* equip_manager,
                                        RE::Actor* actor,
                                        RE::TESBoundObject* bound_item,
                                        RE::BGSEquipSlot* equip_slot) -> void
{
  letr config = Reflyem::Config::get_singleton();
  using SpellType = RE::MagicSystem::SpellType;

  let ui = RE::UI::GetSingleton();

  if (config.misc_fixes().cast_ability_on_equip() && config.misc_fixes().cast_ability_on_equip_instant() && ui &&
      !ui->GameIsPaused() && actor && bound_item && actor->IsPlayerRef() &&
      actor->magicCasters[static_cast<int>(RE::MagicSystem::CastingSource::kInstant)]) {
    let spell_item = bound_item->As<RE::SpellItem>();
    if (spell_item) {
      let spell_type = spell_item->GetSpellType();
      if (spell_type == SpellType::kPower || spell_type == SpellType::kLesserPower ||
          spell_type == SpellType::kVoicePower) {
        unequip_after_shout_flag = false;
        if (skip_insta_cast) {
          skip_insta_cast = false;
          return equip_spell_47_(equip_manager, actor, bound_item, equip_slot);
        }

        auto cost_av = RE::ActorValue::kMagicka;

        if (config.special_techniques().enable() && config.special_techniques().stamina_cost_spell_keyword() &&
            spell_item->HasKeyword(config.special_techniques().stamina_cost_spell_keyword())) {
          cost_av = RE::ActorValue::kStamina;
        }

        let instant_caster = actor->magicCasters[static_cast<int>(RE::MagicSystem::CastingSource::kInstant)];

        if (config.ki_energy_power().enable()) {
          let result = Reflyem::KiEnergyPower::use_ki_energy(actor, spell_item);
          if (result == Reflyem::KiEnergyPower::kOk) {
            instant_caster->CastSpellImmediate(spell_item, false, nullptr, 1.f, false, 0.f, actor);
            return;
          }
          if (result == Reflyem::KiEnergyPower::kFail) {
            return;
          }
        }

        let cost = spell_item->CalculateMagickaCost(instant_caster->actor);
        if (config.misc_fixes().cast_ability_magicka_cost() && cost > instant_caster->actor->GetActorValue(cost_av)) {

          if (instant_caster->actor->IsPlayerRef()) {
            Reflyem::Core::play_sound_base(config.caster_additions().mag_fail_sound(), instant_caster->actor);
            Reflyem::Core::flash_hud_meter(cost_av);
          }

          return;
        }

        if (config.misc_fixes().cast_ability_magicka_cost()) {
          Reflyem::Core::damage_actor_value(*instant_caster->actor, cost_av, cost);
        }
        instant_caster->CastSpellImmediate(spell_item, false, nullptr, 1.f, false, 0.f, actor);
        return;
      }
    }
  }
  return equip_spell_47_(equip_manager, actor, bound_item, equip_slot);
}

auto OnEquipSpell47C5D7::equip_spell_C5(RE::ActorEquipManager* equip_manager,
                                        RE::Actor* actor,
                                        RE::TESBoundObject* spell_item,
                                        RE::BGSEquipSlot* equip_slot) -> void
{
  return equip_spell_C5_(equip_manager, actor, spell_item, equip_slot);
}

auto OnEquipSpell47C5D7::equip_spell_D7(RE::ActorEquipManager* equip_manager,
                                        RE::Actor* actor,
                                        RE::TESBoundObject* spell_item,
                                        RE::BGSEquipSlot* equip_slot) -> void
{

  return equip_spell_D7_(equip_manager, actor, spell_item, equip_slot);
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

auto OnEquipSpellMenu::equip_spell_menu(RE::ActorEquipManager* equip_manager,
                                        RE::Actor* actor,
                                        RE::TESBoundObject* bound_object,
                                        RE::BGSEquipSlot* equip_slot) -> RE::BGSEquipSlot*
{
  logi("Equip spell menu"sv);
  if (bound_object && bound_object->IsMagicItem()) {
    staff_spell = bound_object->As<RE::MagicItem>();
  }
  return equip_slot;
}

auto OnEquipSpellMenu::equip_spell_menu_favorite(RE::ActorEquipManager* equip_manager,
                                                 RE::Actor* actor,
                                                 RE::TESBoundObject* bound_object,
                                                 RE::BGSEquipSlot* equip_slot) -> RE::BGSEquipSlot*
{
  logi("Equip spell menu favorite"sv);
  if (bound_object && bound_object->IsMagicItem()) {
    staff_spell = bound_object->As<RE::MagicItem>();
  }
  // return nullptr;
  return equip_slot;
}

auto OnMagicHit::on_magic_hit_01(RE::MagicCaster* magic_caster,
                                 RE::NiPoint3* ni_point3,
                                 RE::Projectile* projectile,
                                 RE::TESObjectREFR* refr_target,
                                 float a5,
                                 float a6,
                                 bool a7,
                                 bool a8) -> void
{
  letr config = Reflyem::Config::get_singleton();

  if (config.stagger_system().enable()) {
    Reflyem::StaggerSystem::on_magic_hit(projectile, refr_target);
  }

  if (Reflyem::ProjectileBlock::on_magic_hit(magic_caster, ni_point3, projectile, refr_target, a5, a6, a7, a8)) {
    return;
  }
  if (config.cast_on_magic_projectile_hit().enable()) {
    Reflyem::CastOnMagicProjectileHit::on_magic_hit(magic_caster, ni_point3, projectile, refr_target, a5, a6, a7, a8);
  }
  if (config.cast_on_magic_projectile_get_hit().enable()) {
    Reflyem::CastOnMagicProjectileGetHit::on_magic_hit(
        magic_caster, ni_point3, projectile, refr_target, a5, a6, a7, a8);
  }
  return on_magic_hit_01_(magic_caster, ni_point3, projectile, refr_target, a5, a6, a7, a8);
}

auto OnMagicHit::on_magic_hit_02(RE::MagicCaster* magic_caster,
                                 RE::NiPoint3* ni_point3,
                                 RE::Projectile* projectile,
                                 RE::TESObjectREFR* refr_target,
                                 float a5,
                                 float a6,
                                 bool a7,
                                 bool a8) -> void
{

  letr config = Reflyem::Config::get_singleton();

  if (config.stagger_system().enable()) {
    Reflyem::StaggerSystem::on_magic_hit(projectile, refr_target);
  }

  if (Reflyem::ProjectileBlock::on_magic_hit(magic_caster, ni_point3, projectile, refr_target, a5, a6, a7, a8)) {
    return;
  }
  if (config.cast_on_magic_projectile_hit().enable()) {
    Reflyem::CastOnMagicProjectileHit::on_magic_hit(magic_caster, ni_point3, projectile, refr_target, a5, a6, a7, a8);
  }
  if (config.cast_on_magic_projectile_get_hit().enable()) {
    Reflyem::CastOnMagicProjectileGetHit::on_magic_hit(
        magic_caster, ni_point3, projectile, refr_target, a5, a6, a7, a8);
  }
  return on_magic_hit_02_(magic_caster, ni_point3, projectile, refr_target, a5, a6, a7, a8);
}

auto OnActorIsOverEncumbered::is_actor_on_mount(RE::Actor* actor) -> bool
{
  if (actor && actor->IsPlayerRef() && Reflyem::Config::get_singleton().misc_fixes().disable_over_encumbered()) {
    return true;
  }
  return is_actor_on_mount_(actor);
}

auto OnGetCombatStyleForCheckOffensiveMult::get_combat_style(RE::Character* character) -> RE::TESCombatStyle*
{
  if (!character) {
    return get_combat_style_(character);
  }

  let combat_style = get_combat_style_(character);

  if (combat_style) {
    return combat_style;
  }

  loge(
      "Null combat style in OnGetCombatStyleForCheckOffensiveMult for character {} with id {}, try fix with dafault combat style"sv,
      character->GetDisplayFullName(),
      character->formID);
  // let object_manager = RE::BGSDefaultObjectManager::GetSingleton();
  // let default_combat_style =
  //     object_manager->objects[RE::BGSDefaultObjectManager::DefaultObject::kCombatStyle]
  //         ->As<RE::TESCombatStyle>();
  // if (default_combat_style) { return default_combat_style; }
  //
  // loge("Default Combat Style NULL!!!!!!"sv);

  let data_handler = RE::TESDataHandler::GetSingleton();
  let default_combat_style_from_data_handler = data_handler->LookupForm<RE::TESCombatStyle>(0x3D, "Skyrim.esm");

  if (!default_combat_style_from_data_handler) {
    loge("Default Combat Style Data Handler NULL!!!!!!"sv);
  }

  return default_combat_style_from_data_handler;
}

auto OnGetCombatStyleForCheckOffensiveMult::get_combat_style2(RE::Character* character) -> RE::TESCombatStyle*
{
  if (!character) {
    return get_combat_style_(character);
  }

  let combat_style = get_combat_style_(character);

  if (combat_style) {
    return combat_style;
  }

  loge(
      "Null combat style in OnGetCombatStyleForCheckOffensiveMult_2 for character {} with id {}, try fix with dafault combat style"sv,
      character->GetDisplayFullName(),
      character->formID);
  // let object_manager = RE::BGSDefaultObjectManager::GetSingleton();
  // let default_combat_style =
  //     object_manager->objects[RE::BGSDefaultObjectManager::DefaultObject::kCombatStyle]
  //         ->As<RE::TESCombatStyle>();
  // if (default_combat_style) { return default_combat_style; }
  //
  // loge("Default Combat Style NULL!!!!!!"sv);

  let data_handler = RE::TESDataHandler::GetSingleton();
  let default_combat_style_from_data_handler = data_handler->LookupForm<RE::TESCombatStyle>(0x3D, "Skyrim.esm");

  if (!default_combat_style_from_data_handler) {
    loge("Default Combat Style Data Handler NULL!!!!!!"sv);
  }

  return default_combat_style_from_data_handler;
}

auto OnCalculatePlayerSprintCost::calculate_sprint_cost(float player_stamina, float maybe_delta) -> float
{
  logger::debug("Calculate player sprint {} {}"sv, player_stamina, maybe_delta);
  if (let player = RE::PlayerCharacter::GetSingleton();
      !player->IsInCombat() && Reflyem::Config::get_singleton().misc_fixes().disable_sprint_cost_in_combat()) {
    return 0.f;
  }
  return calculate_sprint_cost_(player_stamina, maybe_delta);
}

auto OnInventoryOpen::is_displayed_item(RE::InventoryEntryData* item) -> bool
{
  logger::debug("In open inv hook"sv);
  if (!item) {
    return is_displayed_item_(item);
  }

  const auto& config = Reflyem::Config::get_singleton();
  if (config.death_loot().enable()) {
    return !Reflyem::DeathLoot::is_tagged(item) && is_displayed_item_(item);
  }

  return is_displayed_item_(item);
}

auto OnGetAttackCost::get_attack_cost(RE::ActorValueOwner* owner, RE::BGSAttackData* attack_data) -> float
{

  if (!owner || !attack_data) {
    return get_attack_cost_(owner, attack_data);
  }

  letr config = Reflyem::Config::get_singleton();
  if (config.resource_manager().enable() && config.resource_manager().enable_alternate_melee_cost() &&
      (config.resource_manager().bash_spend_enable() || config.resource_manager().weapon_spend_enable())) {
    if (Reflyem::ResourceManager::get_attack_cost(owner, attack_data)) {
      return -1.f;
    }
  }

  auto actor = (RE::Actor*)((char*)owner - 0xB0);
  if (actor && !actor->IsPlayerRef() && config.misc_fixes().enable_npc_always_spend_stamina_on_attack() &&
      !attack_data->data.flags.any(RE::AttackData::AttackFlag::kPowerAttack)) {
    attack_data->data.flags.set(RE::AttackData::AttackFlag::kPowerAttack);
    // let old_value = attack_data->data.staminaMult;
    // attack_data->data.staminaMult = old_value * 0.75f;
    auto result = get_attack_cost_(owner, attack_data);
    attack_data->data.flags.reset(RE::AttackData::AttackFlag::kPowerAttack);
    // attack_data->data.staminaMult = old_value;
    return result;
  }

  return get_attack_cost_(owner, attack_data);
}

auto OnAttackAction::attack_action(const RE::TESActionData* action_data) -> bool
{
  logger::info("Attack Action: {}"sv, static_cast<std::uint32_t>(action_data->GetSourceActorState()->GetAttackState()));
  return false;
}

auto OnSendHudMessage::hud_message(RE::UIMessageQueue* this_,
                                   const RE::BSFixedString& menu_name,
                                   RE::UI_MESSAGE_TYPE message_type,
                                   RE::IUIMessageData* message_data) -> void
{
  logd("call send hud message");
  const auto hud_data = message_data ? static_cast<RE::HUDData*>(message_data) : nullptr;
  const auto ref_crosshair = hud_data ? hud_data->crossHairRef.get() : RE::TESObjectREFRPtr();

  if (hud_data && ref_crosshair && ref_crosshair.get()) {
    auto ref_object = ref_crosshair.get();
    letr config = Reflyem::Config::get_singleton();
    if (ref_object->IsLocked() && config.misc_fixes().display_key_name()) {
      let lock = ref_object->GetLock();
      if (lock->key && ref_object->GetLockLevel() == RE::LOCK_LEVEL::kRequiresKey) {
        logd("hud_text_data kRequiresKey: {}", hud_data->text.c_str());
        hud_data->text = RE::BSString{std::format("{}: {}", hud_data->text.c_str(), lock->key->GetFullName())};
      }
      if (lock->key && ref_object->GetLockLevel() != RE::LOCK_LEVEL::kRequiresKey &&
          ref_object->GetLockLevel() != RE::LOCK_LEVEL::kUnlocked &&
          RE::PlayerCharacter::GetSingleton()->GetItemCount(lock->key) > 0) {
        logd("hud_text_data not kRequiresKey: {}", hud_data->text.c_str());
        hud_data->text = RE::BSString{std::format("{}: {}", hud_data->text.c_str(), lock->key->GetFullName())};
      }
    }
    // if (config.decluttered_loot().enable() && Reflyem::DeclutteredLoot::declutter_mode &&
    //     Reflyem::DeclutteredLoot::is_clutter_object(ref_object)) {
    //   logd("No activate: {} Message: {}", ref_object->GetDisplayFullName(), static_cast<int>(message_type));
    //   hud_data->type = RE::HUDData::Type::kUnk0;
    //   // hud_data->text = "";
    // }
  }

  hud_message_(this_, menu_name, message_type, message_data);
}

auto OnApplyVoiceAbility::direct_apply_spell_on_target(RE::ActorMagicCaster* other_caster,
                                                       RE::MagicItem* ability,
                                                       RE::TESObject* null_target,
                                                       RE::TESBoundObject* null_object,
                                                       bool false_value) -> bool
{

  letr config = Reflyem::Config::get_singleton();
  if (ability && other_caster && other_caster->actor && ability->As<RE::SpellItem>()) {
    if (config.ki_energy_power().enable()) {
      let result = Reflyem::KiEnergyPower::use_ki_energy(other_caster->actor, ability->As<RE::SpellItem>());
      if (result == Reflyem::KiEnergyPower::kOk) {
        other_caster->CastSpellImmediate(
            ability->As<RE::SpellItem>(), false, nullptr, 1.f, false, 0.f, other_caster->actor);
        return true;
      }
      if (result == Reflyem::KiEnergyPower::kFail) {
        return false;
      }
    }
  }

  if (config.misc_fixes().cast_ability_magicka_cost() && other_caster && ability && other_caster->actor &&
      ability->GetChargeTime() > 0.f) {

    auto cost_av = RE::ActorValue::kMagicka;

    if (config.special_techniques().enable() && config.special_techniques().stamina_cost_spell_keyword() &&
        ability->HasKeyword(config.special_techniques().stamina_cost_spell_keyword())) {
      cost_av = RE::ActorValue::kStamina;
    }

    let cost = ability->CalculateMagickaCost(other_caster->actor);
    if (cost > other_caster->actor->GetActorValue(cost_av)) {
      Reflyem::Core::play_sound_base(config.caster_additions().mag_fail_sound(), other_caster->actor);
      Reflyem::Core::flash_hud_meter(cost_av);
      return false;
    }

    Reflyem::Core::damage_actor_value(*other_caster->actor, cost_av, cost);
    if (config.misc_fixes().cast_ability_on_equip_instant()) {
      RE::PlayerCharacter::GetSingleton()
          ->magicCasters[static_cast<int>(RE::MagicSystem::CastingSource::kInstant)]
          ->CastSpellImmediate(ability, false, nullptr, 1.f, false, 0.f, other_caster->actor);
      return true;
    }
  }

  return direct_apply_spell_on_target_(other_caster, ability, null_target, null_object, false_value);
}

auto OnApplySpellsFromAttack::apply_spells_from_attack(RE::Character* attacker,
                                                       RE::InventoryEntryData* weapon,
                                                       bool is_left,
                                                       RE::Actor* target) -> void
{
  if (!attacker || !weapon || !target) {
    return apply_spells_from_attack_(attacker, weapon, is_left, target);
  }

  letr config = Reflyem::Config::get_singleton();

  if (config.special_techniques().enable() &&
      Reflyem::Core::try_has_absolute_keyword(
          target, config.special_techniques().ignore_apply_spell_and_enchanting_keyword_id())) {
    return;
  }

  RE::HitData hit_data{};
  Reflyem::Core::initialization_hit_data(hit_data, attacker, target, weapon, is_left);

  /*if (config.unblockable_attack().enable() && hit_data.flags.any(RE::HitData::Flag::kBlocked) &&
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
    }*/

  if (config.timing_block().enable()) {
    if (Reflyem::TimingBlock::apply_spells_from_attack(
            *attacker, *target, hit_data, Reflyem::Config::get_singleton())) {
      return;
    }
  }

  return apply_spells_from_attack_(attacker, weapon, is_left, target);
}

// auto OnApplySpellsFromAttack::apply_spells_from_attack2(RE::Actor* attacker, RE::SpellItem* spell, RE::Actor*
// target)
//     -> void
// {
//
//   if (!attacker || !spell || !target) {
//     return apply_spells_from_attack2_(attacker, spell, target);
//   }
//
//   letr config = Reflyem::Config::get_singleton();
//   RE::HitData hit_data{};
//   Reflyem::Core::initialization_hit_data(hit_data, attacker, target, weapon, is_left);
//
// }

auto OnRegenerationPermanentValue::calculate_regeneration_value_magicka(RE::Character* character, RE::ActorValue av)
    -> float
{
  letr config = Reflyem::Config::get_singleton();

  auto mult = 1.f;
  if (config.special_techniques().enable() && config.special_techniques().dynamic_regeneration_magicka()) {
    mult = 1.f + ((1.f - Reflyem::Core::get_actor_value_percent(character, av)) / 4.f);
  }

  if (character && (config.misc_fixes().regeneration_fix() ||
                    (config.resource_manager().enable() && config.resource_manager().regeneration_enable()))) {
    return calculate_actor_value_regeneration(character, av) * mult;
  }
  return calculate_regeneration_value_health_(character, av) * mult;
}

auto OnRegenerationPermanentValue::calculate_regeneration_value_stamina_const_delta(RE::Actor* actor, float delta)
    -> void
{
  letr config = Reflyem::Config::get_singleton();

  if (actor && (config.misc_fixes().regeneration_fix() ||
                (config.resource_manager().enable() && config.resource_manager().regeneration_enable()))) {
    return calculate_stamina_regeneration(actor, delta);
  }
  return calculate_regeneration_value_stamina_const_delta_(actor, delta);
}

auto OnRegenerationPermanentValue::calculate_regeneration_value_stamina(RE::Actor* actor, float delta) -> void
{
  letr config = Reflyem::Config::get_singleton();

  if (actor && (config.misc_fixes().regeneration_fix() ||
                (config.resource_manager().enable() && config.resource_manager().regeneration_enable()))) {
    return calculate_stamina_regeneration(actor, delta);
  }
  return calculate_regeneration_value_stamina_(actor, delta);
}

auto OnRegenerationPermanentValue::calculate_regeneration_value_health(RE::Character* character, RE::ActorValue av)
    -> float
{
  letr config = Reflyem::Config::get_singleton();

  auto mult = 1.f;
  if (config.special_techniques().enable() && config.special_techniques().dynamic_regeneration_magicka()) {
    mult = 1.f + ((1.f - Reflyem::Core::get_actor_value_percent(character, av)) / 4.f);
  }

  if (character && (config.misc_fixes().regeneration_fix() ||
                    (config.resource_manager().enable() && config.resource_manager().regeneration_enable()))) {
    return calculate_actor_value_regeneration(character, av) * mult;
  }
  return calculate_regeneration_value_health_(character, av) * mult;
}

auto OnArrowCallHit::arrow_call_hit(RE::Character* attacker,
                                    RE::Actor* target,
                                    RE::Projectile* projectile,
                                    bool is_left) -> void
{
  return arrow_call_hit_(attacker, target, projectile, is_left);
}

auto OnAttackIsBlocked::is_blocked(RE::Actor* target,
                                   RE::Actor* attacker,
                                   float* attacker_location_x,
                                   float* target_location_x,
                                   void* arg5,
                                   float arg6,
                                   float* arg7,
                                   char arg8) -> bool
{
  if (!attacker || !target) {
    return is_blocked_(target, attacker, attacker_location_x, target_location_x, arg5, arg6, arg7, arg8);
  }

  let is_attacker_actor = attacker->formType.any(RE::Actor::FORMTYPE);
  let is_target_actor = target->formType.any(RE::Actor::FORMTYPE);

  if (!is_attacker_actor || !is_target_actor) {
    return is_blocked_(target, attacker, attacker_location_x, target_location_x, arg5, arg6, arg7, arg8);
  }

  letr config = Reflyem::Config::get_singleton();
  if (config.unblockable_attack().enable()) {
    logi("Is block unblock check start Attacker: {} | Target: {}"sv,
         attacker->GetDisplayFullName(),
         target->GetDisplayFullName());

    if (Reflyem::Core::try_has_absolute_keyword(attacker, config.unblockable_attack().absolute_unblock())) {
      logd("Is absolute keyword"sv);
      return false;
    }

    if (config.timing_block().enable()) {
      auto& actor_cache = Reflyem::Core::ActorsCache::get_singleton();
      const auto& actor_data = actor_cache.get_or_add(target->formID).get();
      if (Reflyem::TimingBlock::is_allow_timing_parry(*attacker, *target, actor_data, config)) {
        logd("Is timing parry"sv);
        return is_blocked_(target, attacker, attacker_location_x, target_location_x, arg5, arg6, arg7, arg8);
      }
      if (Reflyem::TimingBlock::is_allow_timing_block(*attacker, *target, actor_data, config) &&
          !Reflyem::Core::try_has_absolute_keyword(attacker, config.unblockable_attack().timing_block_unblock())) {
        logd("Is timing block"sv);
        return is_blocked_(target, attacker, attacker_location_x, target_location_x, arg5, arg6, arg7, arg8);
      }
    }

    if (Reflyem::Core::try_has_absolute_keyword(attacker, config.unblockable_attack().just_block_unblock())) {
      logd("Is just unblock"sv);
      return false;
    }
  }

  if (config.special_techniques().enable() &&
      Reflyem::Core::try_has_absolute_keyword(target, config.special_techniques().enable_360_block())) {
    let settings_collection = RE::GameSettingCollection::GetSingleton();
    auto angle_setting = settings_collection->GetSetting("fCombatHitConeAngle");
    let old_angle = angle_setting->data.f;
    angle_setting->data.f = 180.f;
    let result = is_blocked_(target, attacker, attacker_location_x, target_location_x, arg5, arg6, arg7, arg8);
    angle_setting->data.f = old_angle;
    return result;
  }

  logd("Is blocked function final"sv);
  return is_blocked_(target, attacker, attacker_location_x, target_location_x, arg5, arg6, arg7, arg8);
}

auto OnAttackIsBlocked::is_blocking_in_hit(RE::Actor* blocker) -> bool
{
  if (!blocker) {
    return is_blocking_in_hit_(blocker);
  }

  letr config = Reflyem::Config::get_singleton();
  if ((config.parry_bash().enable_bash_block() && config.parry_bash().enable()) || config.magic_blocker().enable()) {
    return is_blocking_in_hit_(blocker) || Reflyem::Core::is_blocking(blocker);
  }

  return is_blocking_in_hit_(blocker);
}

auto OnAllowCharacterMagickaRegen::is_allow_magicka_regen(RE::Character* caster) -> bool
{

  letr config = Reflyem::Config::get_singleton();
  let result = is_allow_magicka_regen_(caster);

  if (caster && config.magic_blocker().enable() && config.magic_blocker().enable_regen_while_casting_ward() &&
      !result) {
    if (Reflyem::Core::try_has_absolute_keyword(caster, config.magic_blocker().magic_blocker_keyword())) {

      if (!caster->IsPlayerRef()) {
        return false;
      }

      let left_caster = caster->GetMagicCaster(RE::MagicSystem::CastingSource::kLeftHand);
      let right_caster = caster->GetMagicCaster(RE::MagicSystem::CastingSource::kRightHand);

      if (left_caster && left_caster->currentSpell && left_caster->currentSpell->As<RE::SpellItem>()) {
        auto left_spell = left_caster->currentSpell->As<RE::SpellItem>();
        if (left_caster->state.get() != RE::MagicCaster::State::kNone &&
            !Reflyem::Core::magic_item_has_keyword(left_spell, config.magic_blocker().magic_blocker_keyword())) {
          return false;
        }
      }

      if (right_caster && right_caster->currentSpell && right_caster->currentSpell->As<RE::SpellItem>()) {
        auto right_spell = right_caster->currentSpell->As<RE::SpellItem>();
        if (right_caster->state.get() != RE::MagicCaster::State::kNone &&
            !Reflyem::Core::magic_item_has_keyword(right_spell, config.magic_blocker().magic_blocker_keyword())) {
          return false;
        }
      }

      return true;
    }
  }

  return result;
}

auto OnAnimationEvent::process_event_pc(RE::BSTEventSink<RE::BSAnimationGraphEvent>* this_,
                                        RE::BSAnimationGraphEvent* event,
                                        RE::BSTEventSource<RE::BSAnimationGraphEvent>* dispatcher) -> void
{
  if (!event || !event->holder) {
    return process_event_pc_(this_, event, dispatcher);
  }
  auto& config = Reflyem::Config::get_singleton();
  if (config.resource_manager().enable()) {
    Reflyem::ResourceManager::animation_handler(*event, config);
  }

  if (config.tk_dodge().enable()) {
    Reflyem::TkDodge::animation_handler(*event, config);
  }
  if (config.parry_bash().enable()) {
    Reflyem::ParryBash::animation_handler(*event, config);
  }

  // if (config.timing_block().enable() || config.projectile_block().enable()) {
  //   // logi("TAG: {}", event->tag.c_str());
  //   RE::BSFixedString block_stop{"blockStop"};
  //   if (event->tag.contains(block_stop)) {
  //     Reflyem::Core::set_fake_block_frame(true);
  //   }
  // }

  return process_event_pc_(this_, event, dispatcher);
}

auto OnAnimationEvent::process_event_npc(RE::BSTEventSink<RE::BSAnimationGraphEvent>* this_,
                                         RE::BSAnimationGraphEvent* event,
                                         RE::BSTEventSource<RE::BSAnimationGraphEvent>* dispatcher) -> void
{
  if (!event || !event->holder) {
    return process_event_npc_(this_, event, dispatcher);
  }
  auto& config = Reflyem::Config::get_singleton();
  if (config.resource_manager().enable()) {
    Reflyem::ResourceManager::animation_handler(*event, config);
  }
  if (config.tk_dodge().enable()) {
    Reflyem::TkDodge::animation_handler(*event, config);
  }
  Reflyem::ParryBash::animation_handler(*event, config);
  return process_event_npc_(this_, event, dispatcher);
}

auto OnAdjustActiveEffect::adjust_active_effect(RE::ActiveEffect* this_, float power, bool unk) -> void
{
  if (this_) {
    const auto caster = this_->GetCasterActor();
    const auto target = this_->GetTargetActor();
    if (caster && target) {
      logger::info("Target HP: {} Caster HP: {} Target Level: {} Caster Level: {}"sv,
                   target->GetActorValue(RE::ActorValue::kHealth),
                   caster->GetActorValue(RE::ActorValue::kHealth),
                   target->GetLevel(),
                   caster->GetLevel());
      logger::info("Effect, mag: {}, dur: {}"sv, this_->magnitude, this_->duration);
    } else {
      logger::info("Caster or target is null"sv);
    }
  }
  adjust_active_effect_(this_, power, unk);
  return;
}

auto OnModifyActorValue::modify_actor_value(RE::ValueModifierEffect* this_,
                                            RE::Actor* actor,
                                            float value,
                                            RE::ActorValue av) -> void
{
  if (!actor || !this_) {
    modify_actor_value_(this_, actor, value, av);
    return;
  }

  on_modify_actor_value(this_, actor, value, av);

  modify_actor_value_(this_, actor, value, av);
}

auto OnModifyActorValue::absorb_effect_modify_actor_value(RE::ValueModifierEffect* this_,
                                                          RE::Actor* actor,
                                                          float value,
                                                          RE::ActorValue av) -> void
{
  if (!actor || !this_) {
    absorb_effect_modify_actor_value_(this_, actor, value, av);
    return;
  }

  on_modify_actor_value(this_, actor, value, av);

  absorb_effect_modify_actor_value_(this_, actor, value, av);
}

auto OnModifyActorValue::peak_modify_actor_value(RE::ValueModifierEffect* this_,
                                                 RE::Actor* actor,
                                                 float value,
                                                 RE::ActorValue av) -> void
{
  logger::debug("peak mod actor value"sv);

  if (!actor || !this_) {
    peak_modify_actor_value_(this_, actor, value, av);
    return;
  }

  on_modify_actor_value(this_, actor, value, av);

  peak_modify_actor_value_(this_, actor, value, av);
}

auto OnModifyActorValue::dual_modify_actor_value(RE::ValueModifierEffect* this_,
                                                 RE::Actor* actor,
                                                 float value,
                                                 RE::ActorValue av) -> void
{
  if (!actor || !this_) {
    dual_modify_actor_value_(this_, actor, value, av);
    return;
  }

  on_modify_actor_value(this_, actor, value, av);

  dual_modify_actor_value_(this_, actor, value, av);
}

auto OnModifyActorValue::dual_modify_actor_value_second_inner_call(RE::ValueModifierEffect* this_,
                                                                   RE::Actor* actor,
                                                                   float value,
                                                                   RE::ActorValue av) -> void
{
  if (!actor || !this_) {
    dual_modify_actor_value_second_inner_call_(this_, actor, value, av);
    return;
  }

  on_modify_actor_value(this_, actor, value, av);

  dual_modify_actor_value_second_inner_call_(this_, actor, value, av);
}

auto OnMainUpdate::main_update(RE::Main* this_, float unk) -> void
{
  if (const auto ui = RE::UI::GetSingleton(); ui->GameIsPaused()) {
    main_update_(this_, unk);
    return;
  }

  main_update_(this_, unk);
  return;
}

auto OnGetBowPower::get_bow_power(float draw_power, float weapon_speed) -> float
{
  if (Reflyem::PapyrusExtender::is_papyrus_call_bow() || Reflyem::AnimationEventHandler::is_annotation_bow_call()) {
    logi("Fake call get bow power, return 1.f");
    return 1.f;
  }
  return get_bow_power_(draw_power, weapon_speed);
}

auto OnWeaponHit::weapon_hit(RE::Actor* target, RE::HitData& hit_data) -> void
{
  if (!target) {
    weapon_hit_(target, hit_data);
    return;
  }

  auto& config = Reflyem::Config::get_singleton();

  let aggressor = hit_data.aggressor;
  if (aggressor && aggressor.get()->IsPlayerRef() && !target->IsPlayerRef()) {
    auto& actors_cache = Reflyem::Core::ActorsCache::get_singleton();
    auto& actor_data = actors_cache.get_or_add(target->formID).get();
    logd("Add HitTimer to {} time {}", target->GetDisplayFullName(), config.death_loot().hit_timer());
    actor_data.hit_by_player_timer(config.death_loot().hit_timer());
  }

  if (aggressor && (target->IsPlayerRef() || aggressor.get()->IsPlayerRef())) {
    Reflyem::Core::console_log("--------------------------START WEAPON HIT--------------------------");
    Reflyem::Core::console_log(std::format(
        "WEAPON_HIT: Aggressor: {} | Target: {}", aggressor.get()->GetDisplayFullName(), target->GetDisplayFullName()));

    Reflyem::Core::console_log(std::format("WEAPON_HIT: TotalDamage: {} | "
                                           "PhysicalDamage: {} | PhysicalResisted: {} | BlockedPercent: {}",
                                           hit_data.totalDamage,
                                           hit_data.physicalDamage,
                                           hit_data.resistedPhysicalDamage,
                                           hit_data.percentBlocked));

    Reflyem::Core::console_log(std::format("WEAPON_HIT: Weapon: {} | AttackDataSpell: {} | CriticalEffect: {} ",
                                           hit_data.weapon ? hit_data.weapon->GetFullName() : "None",
                                           hit_data.attackDataSpell ? hit_data.attackDataSpell->GetFullName() : "None",
                                           hit_data.criticalEffect ? hit_data.criticalEffect->GetFullName() : "None"));

    Reflyem::Core::console_log(
        std::format("WEAPON_HIT: CriticalDamageMult: {} | "
                    "SneakAttackBonus: {} | StaggerValue: {} | ReflectedDamage: {} | BonusHealthDamageMult: {}",
                    hit_data.criticalDamageMult,
                    hit_data.sneakAttackBonus,
                    hit_data.stagger,
                    hit_data.reflectedDamage,
                    hit_data.bonusHealthDamageMult));

    let att_loc = hit_data.aggressor.get()->data.location;
    let tar_loc = target->data.location;
    Reflyem::Core::console_log(std::format("WEAPON_HIT: AttkCorX: {} | "
                                           "AttkCorY: {} | AttkCorZ: {} | TargCorX: {} | TargCorY: {} | TargCorZ: {}",
                                           att_loc.x,
                                           att_loc.y,
                                           att_loc.z,
                                           tar_loc.x,
                                           tar_loc.y,
                                           tar_loc.z));

    let att_ang = hit_data.aggressor.get()->data.angle;
    let tar_ang = target->data.angle;
    Reflyem::Core::console_log(std::format("WEAPON_HIT: AttkAngX: {} | "
                                           "AttkAngY: {} | AttkAngZ: {} | TargAngX: {} | TargAngY: {} | TargAngZ: {}",
                                           att_ang.x,
                                           att_ang.y,
                                           att_ang.z,
                                           tar_ang.x,
                                           tar_ang.y,
                                           tar_ang.z));

    if (let attack_data = hit_data.attackData ? hit_data.attackData.get() : nullptr) {
      Reflyem::Core::console_log(std::format(
          "WEAPON_HIT: AtDataEvent: {} | "
          "AtDataDamage: {} | AtDataAtAngle: {} | AtDataAtChance: {} | AtDataStAngle: {} | AtDataStamina: {}",
          attack_data->event.c_str(),
          attack_data->data.damageMult,
          attack_data->data.attackAngle,
          attack_data->data.attackChance,
          attack_data->data.strikeAngle,
          attack_data->data.staminaMult));
    }

    Reflyem::Core::console_log("--------------------------START WEAPON HIT--------------------------");
  }

  if (config.magic_blocker().enable()) {
    Reflyem::MagicBlocker::handle_hit_data_block_percent(hit_data, target);
  }

  if (config.special_techniques().enable() && aggressor && aggressor.get()) {
    Reflyem::SpecialTechniques::normalize_attack(hit_data, config, aggressor.get()->IsPlayerRef());
  }

  if (config.cast_on_block().enable()) {
    Reflyem::CastOnBlock::on_weapon_hit(target, hit_data, config);
  }

  if (config.timing_block().enable() && hit_data.weapon && hit_data.weapon->IsMelee()) {
    Reflyem::TimingBlock::on_weapon_hit(*target, hit_data, config);
  }

  if (config.cast_on_hit().enable()) {
    Reflyem::CastOnHit::on_weapon_hit(target, hit_data, config);
  }

  if (config.cast_on_get_hit().enable()) {
    Reflyem::CastOnGetHit::on_weapon_hit(target, hit_data, config);
  }

  if (config.magic_weapon().enable()) {
    Reflyem::MagicWeapon::on_weapon_hit(*target, hit_data);
  }

  if (config.weapon_crit().enable()) {
    Reflyem::Crit::on_weapon_hit(target, hit_data, config);
  }

  if (config.resource_manager().enable() && config.resource_manager().block_spend_enable() &&
      hit_data.flags.any(RE::HitData::Flag::kBlocked, RE::HitData::Flag::kBlockWithWeapon)) {
    Reflyem::ResourceManager::on_weapon_hit(target, hit_data, config);
  }

  if (config.slow_time().enable()) {
    Reflyem::SlowTime::on_weapon_hit(target, hit_data, config);
  }

  if (config.cheat_death().enable() && config.cheat_death().physical()) {
    Reflyem::CheatDeath::on_weapon_hit(target, hit_data, config);
  }

  if (config.absorb_shield().enable()) {
    Reflyem::AbsorbShield::on_weapon_hit_absorb(target, &hit_data, config);
  }

  if (config.petrified_blood().enable() && config.petrified_blood().physical()) {
    Reflyem::PetrifiedBlood::on_weapon_hit(target, hit_data, config);
  }

  if (config.stamina_shield().enable() && config.stamina_shield().physical()) {
    Reflyem::StaminaShield::on_weapon_hit(target, hit_data, config);
  }

  if (config.magic_shield().enable() && config.magic_shield().physical()) {
    Reflyem::MagicShield::on_weapon_hit(target, hit_data, config);
  }

  if (config.soul_link().enable() && config.soul_link().physic()) {
    Reflyem::SoulLink::on_weapon_hit(target, hit_data, config);
  }

  if (config.special_techniques().enable()) {

    if (hit_data.aggressor && hit_data.aggressor.get() && hit_data.aggressor.get().get() &&
        hit_data.flags.any(RE::HitData::Flag::kBlocked, RE::HitData::Flag::kBlockWithWeapon)) {
      Reflyem::SpecialTechniques::rotate_blocker(target, hit_data.aggressor.get().get(), config);
    }

    if (Reflyem::Core::try_has_absolute_keyword(
            target, config.special_techniques().ignore_apply_spell_and_enchanting_keyword_id()) &&
        hit_data.attackDataSpell) {
      hit_data.attackDataSpell = nullptr;
    }

    if (config.special_techniques().slow_after_block_hit_mult() != 1.f &&
        hit_data.flags.any(RE::HitData::Flag::kBlocked, RE::HitData::Flag::kBlockWithWeapon)) {
      auto& actors_cache = Reflyem::Core::ActorsCache::get_singleton();
      auto& actor_data = actors_cache.get_or_add(target->formID).get();
      actor_data.slow_after_block_hit_flag(true);
      actor_data.duration_after_block_hit(config.special_techniques().slow_after_block_hit_duration());
    }
  }

  Reflyem::Vampirism::on_weapon_hit(target, hit_data, config);

  if (config.leech_effect().enable()) {
    Reflyem::LeechEffect::on_weapon_hit(target, hit_data, config);
  }

  if (config.recoup_effect().enable()) {
    Reflyem::RecoupEffect::on_weapon_hit(target, hit_data, config);
  }

  if (config.absorb_shield().enable()) {
    Reflyem::AbsorbShield::on_weapon_hit_gettin(target, &hit_data, config);
  }

  if (config.stagger_system().enable()) {
    Reflyem::StaggerSystem::on_weapon_hit(hit_data, target);
  }

  if (aggressor && (target->IsPlayerRef() || aggressor.get()->IsPlayerRef())) {
    Reflyem::Core::console_log("--------------------------END WEAPON HIT--------------------------");
    Reflyem::Core::console_log(std::format(
        "WEAPON_HIT: Aggressor: {} | Target: {}", aggressor.get()->GetDisplayFullName(), target->GetDisplayFullName()));

    Reflyem::Core::console_log(std::format("WEAPON_HIT: TotalDamage: {} | "
                                           "PhysicalDamage: {} | PhysicalResisted: {} | BlockedPercent: {}",
                                           hit_data.totalDamage,
                                           hit_data.physicalDamage,
                                           hit_data.resistedPhysicalDamage,
                                           hit_data.percentBlocked));

    Reflyem::Core::console_log(std::format("WEAPON_HIT: Weapon: {} | AttackDataSpell: {} | CriticalEffect: {} ",
                                           hit_data.weapon ? hit_data.weapon->GetFullName() : "None",
                                           hit_data.attackDataSpell ? hit_data.attackDataSpell->GetFullName() : "None",
                                           hit_data.criticalEffect ? hit_data.criticalEffect->GetFullName() : "None"));

    Reflyem::Core::console_log(
        std::format("WEAPON_HIT: CriticalDamageMult: {} | "
                    "SneakAttackBonus: {} | StaggerValue: {} | ReflectedDamage: {} | BonusHealthDamageMult: {}",
                    hit_data.criticalDamageMult,
                    hit_data.sneakAttackBonus,
                    hit_data.stagger,
                    hit_data.reflectedDamage,
                    hit_data.bonusHealthDamageMult));

    let att_loc = hit_data.aggressor.get()->data.location;
    let tar_loc = target->data.location;
    Reflyem::Core::console_log(std::format("WEAPON_HIT: AttkCorX: {} | "
                                           "AttkCorY: {} | AttkCorZ: {} | TargCorX: {} | TargCorY: {} | TargCorZ: {}",
                                           att_loc.x,
                                           att_loc.y,
                                           att_loc.z,
                                           tar_loc.x,
                                           tar_loc.y,
                                           tar_loc.z));

    let att_ang = hit_data.aggressor.get()->data.angle;
    let tar_ang = target->data.angle;
    Reflyem::Core::console_log(std::format("WEAPON_HIT: AttkAngX: {} | "
                                           "AttkAngY: {} | AttkAngZ: {} | TargAngX: {} | TargAngY: {} | TargAngZ: {}",
                                           att_ang.x,
                                           att_ang.y,
                                           att_ang.z,
                                           tar_ang.x,
                                           tar_ang.y,
                                           tar_ang.z));

    if (let attack_data = hit_data.attackData ? hit_data.attackData.get() : nullptr) {
      Reflyem::Core::console_log(std::format(
          "WEAPON_HIT: AtDataEvent: {} | "
          "AtDataDamage: {} | AtDataAtAngle: {} | AtDataAtChance: {} | AtDataStAngle: {} | AtDataStamina: {}",
          attack_data->event.c_str(),
          attack_data->data.damageMult,
          attack_data->data.attackAngle,
          attack_data->data.attackChance,
          attack_data->data.strikeAngle,
          attack_data->data.staminaMult));
    }

    Reflyem::Core::console_log("--------------------------END WEAPON HIT--------------------------");
  }

  return weapon_hit_(target, hit_data);
}

auto OnCalculateCurrentCost::calculate_current_cost_conc(RE::ActorMagicCaster* caster) -> float
{
  let result = calculate_current_cost_conc_(caster);

  letr config = Reflyem::Config::get_singleton();
  if (result > 0.f && caster && caster->actor) {

    if (caster->currentSpell &&
        caster->currentSpell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration &&
        caster->currentSpell->As<RE::SpellItem>()) {
      if (config.caster_additions().enable() && config.caster_additions().enable_rally_manacost()) {
        let rally_from_manacost = result * config.caster_additions().rally_manacost_mult();
        let rally_from_magicka = Reflyem::Core::get_actor_value_max(caster->actor, RE::ActorValue::kMagicka) *
                                 config.caster_additions().rally_magicka_mult();
        auto& actor_cache = Reflyem::Core::ActorsCache::get_singleton();
        auto& actor_data = actor_cache.get_or_add(caster->actor->formID).get();
        let new_rally =
            ((rally_from_magicka + rally_from_manacost) * player_last_delta) + actor_data.rally_magicka_cost();
        logd("Rallys for conc: {} {} {}", rally_from_magicka, rally_from_manacost, new_rally);
        actor_data.rally_magicka_cost(new_rally);
      }
      if (config.absorb_shield().enable() &&
          Reflyem::AbsorbShield::is_enable_shield_value_from_cast_cost(caster->actor)) {
        let shield_from_cast_cost =
            Reflyem::AbsorbShield::calculate_shield_value_from_cast_cost(caster->currentSpell, caster->actor);
        Reflyem::AbsorbShield::modify_shield_value_from_cast_cost(caster->actor,
                                                                  shield_from_cast_cost * player_last_delta);
      }
    }
  }

  return result;
}

auto OnSneakRequirementForSneakAttack::is_sneaking(RE::Actor* actor) -> bool
{
  if (Reflyem::Config::get_singleton().misc_fixes().no_sneak_requirement_for_sneak_attack()) {
    return true;
  }
  return is_sneaking_(actor);
}

auto OnCastActorValue::actor_value_for_cost_check_cast(RE::MagicItem* magic_item,
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

  letr config = Reflyem::Config::get_singleton();
  if (config.special_techniques().enable() && config.special_techniques().stamina_cost_spell_keyword() && magic_item &&
      magic_item->HasKeyword(config.special_techniques().stamina_cost_spell_keyword())) {
    return RE::ActorValue::kStamina;
  }

  return actor_value_for_cost_check_cast_(magic_item, cast_source);
}

auto OnCastActorValue::actor_value_for_cost_during_cast(RE::MagicItem* magic_item,
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

  letr config = Reflyem::Config::get_singleton();
  if (config.special_techniques().enable() && config.special_techniques().stamina_cost_spell_keyword() && magic_item &&
      magic_item->HasKeyword(config.special_techniques().stamina_cost_spell_keyword())) {
    return RE::ActorValue::kStamina;
  }

  return actor_value_for_cost_during_cast_(magic_item, cast_source);
}
auto OnCastActorValue::actor_value_for_cost_restore_value(RE::MagicItem* magic_item,
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

  letr config = Reflyem::Config::get_singleton();
  if (config.special_techniques().enable() && config.special_techniques().stamina_cost_spell_keyword() && magic_item &&
      magic_item->HasKeyword(config.special_techniques().stamina_cost_spell_keyword())) {
    return RE::ActorValue::kStamina;
  }

  return actor_value_for_cost_restore_value_(magic_item, cast_source);
}

auto OnTrapHit::trap_hit(RE::Actor* target, RE::HitData* hit_data) -> void
{
  if (!target || !hit_data) {
    return trap_hit_(target, hit_data);
  }

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

  let aggressor = RE::PlayerCharacter::GetSingleton();
  if (aggressor && !target->IsPlayerRef()) {
    auto& actors_cache = Reflyem::Core::ActorsCache::get_singleton();
    auto& actor_data = actors_cache.get_or_add(target->formID).get();
    actor_data.hit_by_player_timer(config.death_loot().hit_timer());
  }

  if (config.magic_blocker().enable()) {
    Reflyem::MagicBlocker::handle_hit_data_block_percent(*hit_data, target);
  }

  if (config.absorb_shield().enable()) {
    Reflyem::AbsorbShield::on_weapon_hit_absorb(target, hit_data, config);
  }

  if (config.cheat_death().enable() && config.cheat_death().physical()) {
    Reflyem::CheatDeath::on_weapon_hit(target, *hit_data, config);
  }

  if (config.petrified_blood().enable() && config.petrified_blood().physical()) {
    Reflyem::PetrifiedBlood::on_weapon_hit(target, *hit_data, config);
  }

  if (config.stamina_shield().enable() && config.stamina_shield().physical()) {
    Reflyem::StaminaShield::on_weapon_hit(target, *hit_data, config);
  }

  if (config.magic_shield().enable() && config.magic_shield().physical()) {
    Reflyem::MagicShield::on_weapon_hit(target, *hit_data, config);
  }

  if (config.soul_link().enable() && config.soul_link().physic()) {
    Reflyem::SoulLink::on_weapon_hit(target, *hit_data, config);
  }

  if (config.recoup_effect().enable()) {
    Reflyem::RecoupEffect::on_weapon_hit(target, *hit_data, config);
  }

  return trap_hit_(target, hit_data);
}

auto OnMeleeCollision::melee_collision(RE::Actor* attacker, RE::Actor* victim, RE::Projectile* projectile, char aleft)
    -> void
{
  if (!attacker || !victim) {
    return melee_collision_(attacker, victim, projectile, aleft);
  }

  const auto& config = Reflyem::Config::get_singleton();

  if (config.parry_bash().enable()) {
    if (Reflyem::ParryBash::on_melee_collision(*attacker, *victim, config)) {
      return;
    }
  }

  return melee_collision_(attacker, victim, projectile, aleft);
}

auto OnCheckResistance::check_resistance_npc(RE::MagicTarget* this_,
                                             RE::MagicItem* magic_item,
                                             RE::Effect* effect,
                                             RE::TESBoundObject* bound_object) -> float
{
  if (!this_ || !magic_item || !effect) {
    logger::debug("Original resistance call");
    return check_resistance_npc_(this_, magic_item, effect, bound_object);
  }

  const auto& config = Reflyem::Config::get_singleton();

  if (config.resist_tweaks().enable() && config.resist_tweaks().enable_check_resistance()) {
    return Reflyem::ResistTweaks::check_resistance(*this_, *magic_item, *effect, bound_object, config);
  }

  return check_resistance_npc_(this_, magic_item, effect, bound_object);
}

auto OnCheckResistance::check_resistance_pc(RE::MagicTarget* this_,
                                            RE::MagicItem* magic_item,
                                            RE::Effect* effect,
                                            RE::TESBoundObject* bound_object) -> float
{
  if (!this_ || !magic_item || !effect) {
    logger::debug("Original resistance call"sv);
    return check_resistance_pc_(this_, magic_item, effect, bound_object);
  }

  const auto& config = Reflyem::Config::get_singleton();

  if (config.resist_tweaks().enable() && config.resist_tweaks().enable_check_resistance()) {
    return Reflyem::ResistTweaks::check_resistance(*this_, *magic_item, *effect, bound_object, config);
  }

  return check_resistance_pc_(this_, magic_item, effect, bound_object);
}

auto OnEffectAdded::effect_added(RE::MagicTarget* this_, RE::ActiveEffect* active_effect) -> void
{
  if (!active_effect) {
    return;
  }

  letr config = Reflyem::Config::get_singleton();

  // let caster_handle = active_effect->caster;
  // let target_handle = active_effect->target;
  // if (caster_handle && caster_handle.get() && caster_handle.get().get() && target_handle) {
  //   let target = target_handle->GetTargetAsActor();
  //   let caster = caster_handle.get().get();
  //   if (target && !target->IsPlayerRef() && (caster->IsPlayerRef() || caster->IsPlayerTeammate())) {
  //
  //     auto& actors_cache = Reflyem::Core::ActorsCache::get_singleton();
  //     auto& actor_data = actors_cache.get_or_add(target->formID).get();
  //     actor_data.hit_by_player_timer(config.death_loot().hit_timer());
  //   }
  // }

  let caster_handle = active_effect->caster;
  let target_handle = active_effect->target;

  if (caster_handle && caster_handle.get() && caster_handle.get().get() && target_handle && active_effect->effect &&
      active_effect->effect->baseEffect) {
    const auto target = (RE::Actor*)((char*)this_ - 0x98);
    if (caster_handle.get()->IsPlayerRef() || (target && target->IsPlayerRef())) {
      Reflyem::Core::console_log(
          std::format("EFFECT_ADDED: Aggressor: {} Target: {} SpellName: {} SourceName: {} EffectName: {} "
                      "EffectEditorId: {} Magnitude: {}: Duration: {}",
                      caster_handle.get()->GetDisplayFullName(),
                      target->GetDisplayFullName(),
                      active_effect->spell ? active_effect->spell->GetFullName() : "None",
                      active_effect->source ? active_effect->source->GetName() : "None",
                      active_effect->effect->baseEffect->GetFullName(),
                      active_effect->effect->baseEffect->GetFormEditorID(),
                      active_effect->GetMagnitude(),
                      active_effect->duration));
    }
  }

  if (config.cheat_death().enable() && config.cheat_death().magick()) {
    Reflyem::CheatDeath::effect_added(this_, active_effect, config);
  }
}

auto OnEffectAdded::effect_added_npc(RE::MagicTarget* this_, RE::ActiveEffect* active_effect) -> void
{

  effect_added(this_, active_effect);

  if (!this_ || !active_effect) {
    return effect_added_npc_(this_, active_effect);
  }

  // if (active_effect->effect && active_effect->effect->baseEffect) {
  //   logi("Effect: {} {} {}"sv,
  //        active_effect->effect->baseEffect->GetName(),
  //        active_effect->GetMagnitude(),
  //        active_effect->duration);
  // }

  return effect_added_npc_(this_, active_effect);
}

auto OnEffectAdded::effect_added_pc(RE::MagicTarget* this_, RE::ActiveEffect* active_effect) -> void
{

  effect_added(this_, active_effect);

  if (!this_ || !active_effect) {
    return effect_added_pc_(this_, active_effect);
  }

  // if (active_effect->effect && active_effect->effect->baseEffect) {
  //   logi("{} {} {}"sv,
  //        active_effect->effect->baseEffect->GetName(),
  //        active_effect->GetMagnitude(),
  //        active_effect->duration);
  // }

  return effect_added_pc_(this_, active_effect);
}

auto OnEnchIgnoresResistance::ignores_resistance(RE::MagicItem* this_) -> bool
{
  if (!this_) {
    return ignores_resistance_(this_);
  }

  const auto& config = Reflyem::Config::get_singleton();

  if (config.resist_tweaks().enable() && config.resist_tweaks().ench_ignore_resistance()) {
    return Reflyem::ResistTweaks::ignores_resistance(*this_);
  }

  return ignores_resistance_(this_);
}

auto OnEnchGetNoAbsorb::get_no_absorb(RE::MagicItem* this_) -> bool
{
  if (!this_) {
    return get_no_absorb_(this_);
  }

  const auto& config = Reflyem::Config::get_singleton();

  if (config.misc_fixes().enable_enchants_ignore_absorb()) {
    return true;
  }

  if (config.resist_tweaks().enable() && config.resist_tweaks().ench_get_no_absorb()) {
    return Reflyem::ResistTweaks::get_no_absorb(*this_);
  }

  return get_no_absorb_(this_);
}

auto OnActorValueOwner::get_actor_value_npc(RE::ActorValueOwner* this_, RE::ActorValue av) -> float
{
  if (!this_) {
    return get_actor_value_npc_(this_, av);
  }

  const auto actor = (RE::Actor*)((char*)this_ - 0xB0);
  if (!actor) {
    logi("getAvPc: actor null");
  }

  letr config = Reflyem::Config::get_singleton();

  if (config.equip_load().enable() && av == RE::ActorValue::kInventoryWeight && config.equip_load().enable_for_npc()) {
    return Reflyem::EquipLoad::get_actor_value(*this_, av).value_or(get_actor_value_npc_(this_, av));
  }

  if (av == RE::ActorValue::kSpeedMult) {
    let default_speed_mult = get_actor_value_npc_(this_, av);

    let speed_mult =
        config.speed_mult_cap().enable()
            ? Reflyem::SpeedMultCap::get_actor_value(this_, default_speed_mult).value_or(default_speed_mult)
            : default_speed_mult;

    if (config.misc_fixes().negative_speed_mult_fix()) {
      if (speed_mult < 1.f)
        return 1.f;
    }

    if (config.special_techniques().enable() && config.special_techniques().slow_after_block_hit_mult() != 1.f &&
        actor) {
      auto& actors_cache = Reflyem::Core::ActorsCache::get_singleton();
      auto& actor_data = actors_cache.get_or_add(actor->formID).get();
      if (actor_data.slow_after_block_hit_flag()) {
        return speed_mult * config.special_techniques().slow_after_block_hit_mult();
      }
    }

    return speed_mult;
  }

  if (av == RE::ActorValue::kDamageResist && config.special_techniques().enable() &&
      config.special_techniques().keyword_add_speed_mult_to_damage_resist() && actor) {
    let speed_mult = actor->GetActorValue(RE::ActorValue::kSpeedMult);
    if (speed_mult > 0.f && config.special_techniques().add_speed_mult_to_damage_resist_mult() > 0.f) {
      return (speed_mult * config.special_techniques().add_speed_mult_to_damage_resist_mult()) +
             get_actor_value_npc_(this_, av);
    }
  }

  return get_actor_value_npc_(this_, av);
}

auto OnActorValueOwner::set_actor_value_npc(RE::ActorValueOwner* this_, RE::ActorValue av, float value) -> void
{
  if (!this_) {
    return set_actor_value_npc_(this_, av, value);
  }

  if (Reflyem::Config::get_singleton().equip_load().enable() && av == RE::ActorValue::kInventoryWeight) {
    return set_actor_value_npc_(this_, av, Reflyem::EquipLoad::set_actor_value(*this_, av, value));
  }

  return set_actor_value_npc_(this_, av, value);
}

auto OnActorValueOwner::mod_actor_value_npc(RE::ActorValueOwner* this_, RE::ActorValue av, float value) -> void
{
  if (!this_) {
    return mod_actor_value_npc_(this_, av, value);
  }

  if (Reflyem::Config::get_singleton().equip_load().enable() && av == RE::ActorValue::kInventoryWeight) {
    return mod_actor_value_npc_(this_, av, Reflyem::EquipLoad::mod_actor_value(*this_, av, value));
  }

  return mod_actor_value_npc_(this_, av, value);
}

auto OnActorValueOwner::get_actor_value_pc(RE::ActorValueOwner* this_, RE::ActorValue av) -> float
{
  if (!this_) {
    return get_actor_value_pc_(this_, av);
  }

  const auto actor = (RE::Actor*)((char*)this_ - 0xB0);
  if (!actor) {
    logi("getAvPc: actor null");
  }

  letr config = Reflyem::Config::get_singleton();

  if (config.equip_load().enable() && av == RE::ActorValue::kInventoryWeight) {
    return Reflyem::EquipLoad::get_actor_value(*this_, av).value_or(get_actor_value_pc_(this_, av));
  }

  if (av == RE::ActorValue::kSpeedMult) {
    let default_speed_mult = get_actor_value_pc_(this_, av);

    let speed_mult =
        config.speed_mult_cap().enable()
            ? Reflyem::SpeedMultCap::get_actor_value(this_, default_speed_mult).value_or(default_speed_mult)
            : default_speed_mult;

    if (config.misc_fixes().negative_speed_mult_fix()) {
      if (speed_mult < 1.f)
        return 1.f;
    }

    if (config.special_techniques().enable() && config.special_techniques().slow_after_block_hit_mult() != 1.f &&
        actor) {
      auto& actors_cache = Reflyem::Core::ActorsCache::get_singleton();
      auto& actor_data = actors_cache.get_or_add(actor->formID).get();
      if (actor_data.slow_after_block_hit_flag()) {
        return speed_mult * config.special_techniques().slow_after_block_hit_mult();
      }
    }

    return speed_mult;
  }

  if (av == RE::ActorValue::kDamageResist && config.special_techniques().enable() &&
      config.special_techniques().keyword_add_speed_mult_to_damage_resist() && actor) {
    let speed_mult = actor->GetActorValue(RE::ActorValue::kSpeedMult);
    if (speed_mult > 0.f && config.special_techniques().add_speed_mult_to_damage_resist_mult() > 0.f) {
      return (speed_mult * config.special_techniques().add_speed_mult_to_damage_resist_mult()) +
             get_actor_value_npc_(this_, av);
    }
  }

  return get_actor_value_pc_(this_, av);
}

auto OnActorValueOwner::set_actor_value_pc(RE::ActorValueOwner* this_, RE::ActorValue av, float value) -> void
{
  if (!this_) {
    return set_actor_value_pc_(this_, av, value);
  }

  if (Reflyem::Config::get_singleton().equip_load().enable() && av == RE::ActorValue::kInventoryWeight) {
    return set_actor_value_pc_(this_, av, Reflyem::EquipLoad::set_actor_value(*this_, av, value));
  }

  return set_actor_value_pc_(this_, av, value);
}

auto OnActorValueOwner::mod_actor_value_pc(RE::ActorValueOwner* this_, RE::ActorValue av, float value) -> void
{
  if (!this_) {
    return mod_actor_value_pc_(this_, av, value);
  }

  if (Reflyem::Config::get_singleton().equip_load().enable() && av == RE::ActorValue::kInventoryWeight) {
    return mod_actor_value_npc_(this_, av, Reflyem::EquipLoad::mod_actor_value(*this_, av, value));
  }

  return mod_actor_value_pc_(this_, av, value);
}

auto OnHealthMagickaStaminaRegeneration::restore_health(RE::Actor* actor, RE::ActorValue av, float value) -> void
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

auto OnHealthMagickaStaminaRegeneration::restore_magicka(RE::Actor* actor, RE::ActorValue av, float value) -> void
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

auto OnHealthMagickaStaminaRegeneration::restore_stamina(RE::Actor* actor, RE::ActorValue av, float value) -> void
{
  // if (!actor) { return restore_stamina_(actor, av, value); }
  //
  // const auto& config = Reflyem::Config::get_singleton();
  // if (config.resource_manager().enable() && config.resource_manager().regeneration_enable())
  //   {
  //     value = Reflyem::ResourceManager::regeneration(*actor, av, player_last_delta);
  //   }

  // if (actor->IsPlayerRef()) { logger::info("RestoreStamina Value: {}"sv, value); }

  letr config = Reflyem::Config::get_singleton();

  auto mult = 1.f;
  if (config.special_techniques().enable() && config.special_techniques().dynamic_regeneration_stamina()) {
    mult = 1.f + ((1.f - Reflyem::Core::get_actor_value_percent(actor, RE::ActorValue::kStamina)) / 4.f);
  }
  value = value * mult;
  return restore_stamina_(actor, av, value);
}

void handle_all_forms()
{

  // letr config = Reflyem::Config::get_singleton();

  auto location_ref_boss = Reflyem::Core::get_location_ref_type_boss();

  auto [forms_map, lock] = RE::TESForm::GetAllForms();

  if (!forms_map || !location_ref_boss) {
    loge("Error when try get all game forms"sv);
    return;
  }

  for (auto&& [key, form] : *forms_map) {
    if (!form) {
      continue;
    }

    let location = form->As<RE::BGSLocation>();
    if (location && !location->specialRefs.empty()) {
      for (auto& special_ref : location->specialRefs) {
        if (special_ref.type && special_ref.type == location_ref_boss) {
          Reflyem::DeathLoot::location_bosses.push_back(special_ref.refData.refID);
        }
      }
    }
  }
}

auto install_hooks() -> void
{
  logger::info("start install hooks"sv);
  auto& trampoline = SKSE::GetTrampoline();
  trampoline.create(2048);

  stl::write_vfunc<RE::TESObjectMISC, GetActivateText>();
  stl::write_vfunc<RE::SummonCreatureEffect, SummonCreatureEffectUpdate>();
  // stl::write_vfunc<RE::AlchemyItem, GetActivateText>();
  // stl::write_vfunc<RE::IngredientItem, GetActivateText>();

  stl::write_vfunc<RE::TESObjectMISC, GetActivate>();
  stl::write_vfunc<RE::PlayerCharacter, UseSkill>();
  // stl::write_vfunc<RE::AlchemyItem, GetActivate>();
  // stl::write_vfunc<RE::IngredientItem, GetActivate>();

  logger::info("Registered weapon hook");

  OnMeleeCollision::install_hook(trampoline);
  OnWeaponHit::install_hook(trampoline);
  OnSendHudMessage::install_hook(trampoline);
  OnSneakRequirementForSneakAttack::install_hook(trampoline);
  OnAttackIsBlocked::install_hook(trampoline);
  OnTrapHit::install_hook(trampoline);
  OnInventoryOpen::install_hook(trampoline);
  OnCastActorValue::install_hook(trampoline);
  OnCalculatePlayerSprintCost::install_hook(trampoline);
  OnGetCombatStyleForCheckOffensiveMult::install_hook(trampoline);
  OnRegenerationPermanentValue::install_hook(trampoline);
  OnActorIsOverEncumbered::install_hook(trampoline);
  OnMagicHit::install_hook(trampoline);
  OnPoisonRework::install_hook(trampoline);
  OnGetBowPower::install_hook(trampoline);
  OnDrinkPotion::install_hook();
  OnActorUpdateEquipBound::install_hook();
  OnCheckResistance::install_hook();
  OnEffectAdded::install_hook();
  OnGetAttackCost::install_hook(trampoline);
  OnAllowCharacterMagickaRegen::install_hook(trampoline);
  OnEnchIgnoresResistance::install_hook();
  OnEnchGetNoAbsorb::install_hook();
  OnActorValueOwner::install_hook();
  OnProjectileHit::install_hook();
  // OnContainerMenu::install_hook();
  // OnStandardItemData::install_hook();
  // OnMagicItemData::install_hook();
  // OnFavoriteMenuEquipState::install_hook(trampoline);
  // OnMainUpdate::install_hook(trampoline);
  // OnAdjustActiveEffect::install_hook(trampoline);
  OnAnimationEvent::install_hook();
  OnCharacterUpdate::install_hook();
  // OnActorMagicCaster::install_hook();
  // OnSetCurrentSpell::install_hook(trampoline);
  OnModifyActorValue::install_hook(trampoline);
  // OnSpellEquip::install_hook(trampoline); // broken call sign, don't use
  // OnActorEquipManager::install_hook(trampoline);
  OnActorEquipManagerEquipSpell::install_hook(trampoline);
  OnCalculateCurrentCost::install_hook(trampoline);
  OnApplyVoiceAbility::install_hook(trampoline);
  OnActorEquipManagerEquipShout::install_hook(trampoline);
  // OnEquipSpellMenu::install_hook(trampoline);
  OnEquipSpell47C5D7::install_hook(trampoline);
  OnHealthMagickaStaminaRegeneration::install_hook(trampoline);
  OnApplySpellsFromAttack::install_hook(trampoline);
  // OnFillCasterEnchantData::install_hook(trampoline);
  OnActorAddRemoveItems::install_hook();
  // OnShoutHandler::install_hook();
  // OnAttackAction::install_hook(trampoline);
  // OnAttackData::install_hook(trampoline);
  logger::info("finish install hooks"sv);
}
} // namespace Hooks
