#include "Hooks.hpp"
#include "Core.hpp"
#include "plugin/CastOnBlock.hpp"
#include "plugin/CastOnHit.hpp"
#include "plugin/CasterAdditions.hpp"
#include "plugin/CheatDeath.hpp"
#include "plugin/Crit.hpp"
#include "plugin/EquipLoad.hpp"
#include "plugin/ItemLimit.hpp"
#include "plugin/MagicShield.hpp"
#include "plugin/MagicWepon.hpp"
#include "plugin/ParryBash.hpp"
#include "plugin/PetrifiedBlood.hpp"
#include "plugin/ResistTweaks.hpp"
#include "plugin/ResourceManager.hpp"
#include "plugin/SpeedCasting.hpp"
#include "plugin/TKDodge.hpp"
#include "plugin/TimingBlock.hpp"
#include "plugin/Vampirism.hpp"

namespace Hooks
{

static float player_last_delta = 0.f;

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

auto OnCharacterUpdate::update_pc(RE::PlayerCharacter* this_, float delta) -> void
{
  if (this_)
    {
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

auto OnAttackData::process_attack(RE::ActorValueOwner* value_owner, RE::BGSAttackData* attack_data)
    -> void
{
  process_attack_(value_owner, attack_data);
  return;
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

  if (Reflyem::Config::get_singleton().timing_block().enable())
    {
      RE::HitData hit_data{};
      Reflyem::Core::initialization_hit_data(hit_data, attacker, target, weapon, is_left);
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

  if (config.magic_shield().enable() && config.magic_shield().magick())
    {
      Reflyem::MagicShield::modify_actor_value(this_, actor, value, av, config);
    }

  Reflyem::Vampirism::modify_actor_value(this_, actor, value, av, config);

  modify_actor_value_(this_, actor, value, av);
  return;
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

  if (config.magic_shield().enable() && config.magic_shield().magick())
    {
      Reflyem::MagicShield::modify_actor_value(this_, actor, value, av, config);
    }

  Reflyem::Vampirism::modify_actor_value(this_, actor, value, av, config);

  absorb_effect_modify_actor_value_(this_, actor, value, av);
  return;
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

  if (config.magic_shield().enable() && config.magic_shield().magick())
    {
      Reflyem::MagicShield::modify_actor_value(this_, actor, value, av, config);
    }

  Reflyem::Vampirism::modify_actor_value(this_, actor, value, av, config);

  peak_modify_actor_value_(this_, actor, value, av);
  return;
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

  if (config.magic_shield().enable() && config.magic_shield().magick())
    {
      Reflyem::MagicShield::modify_actor_value(this_, actor, value, av, config);
    }

  Reflyem::Vampirism::modify_actor_value(this_, actor, value, av, config);

  dual_modify_actor_value_(this_, actor, value, av);
  return;
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

  if (config.magic_shield().enable() && config.magic_shield().magick())
    {
      Reflyem::MagicShield::modify_actor_value(this_, actor, value, av, config);
    }

  Reflyem::Vampirism::modify_actor_value(this_, actor, value, av, config);

  dual_modify_actor_value_second_inner_call_(this_, actor, value, av);
  return;
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

  if (config.magic_shield().enable() && config.magic_shield().physical())
    {
      Reflyem::MagicShield::on_weapon_hit(target, hit_data, config);
    }

  Reflyem::Vampirism::on_weapon_hit(target, hit_data, config);

  return weapon_hit_(target, hit_data);
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

  if (Reflyem::Config::get_singleton().equip_load().enable())
    {
      return Reflyem::EquipLoad::get_actor_value(*this_, av)
          .value_or(get_actor_value_npc_(this_, av));
    }

  return get_actor_value_npc_(this_, av);
}

auto OnActorValueOwner::set_actor_value_npc(
    RE::ActorValueOwner* this_,
    RE::ActorValue av,
    float value) -> void
{
  if (!this_) { return set_actor_value_npc_(this_, av, value); }

  if (Reflyem::Config::get_singleton().equip_load().enable())
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

  if (Reflyem::Config::get_singleton().equip_load().enable())
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

  if (Reflyem::Config::get_singleton().equip_load().enable())
    {
      return Reflyem::EquipLoad::get_actor_value(*this_, av)
          .value_or(get_actor_value_pc_(this_, av));
    }

  return get_actor_value_pc_(this_, av);
}

auto OnActorValueOwner::set_actor_value_pc(
    RE::ActorValueOwner* this_,
    RE::ActorValue av,
    float value) -> void
{
  if (!this_) { return set_actor_value_pc_(this_, av, value); }

  if (Reflyem::Config::get_singleton().equip_load().enable())
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

  if (Reflyem::Config::get_singleton().equip_load().enable())
    {
      return mod_actor_value_pc_(this_, av, Reflyem::EquipLoad::mod_actor_value(*this_, av, value));
    }

  return mod_actor_value_pc_(this_, av, value);
}

auto OnHealthMagickaStaminaRegeneration::restore_health(
    RE::Actor* actor,
    RE::ActorValue av,
    float value) -> void
{
  if (!actor) { return restore_health_(actor, av, value); }

  const auto& config = Reflyem::Config::get_singleton();
  if (config.resource_manager().enable() && config.resource_manager().regeneration_enable())
    {
      value = Reflyem::ResourceManager::regeneration(*actor, av, player_last_delta);
    }

  return restore_health_(actor, av, value);
}

auto OnHealthMagickaStaminaRegeneration::restore_magicka(
    RE::Actor* actor,
    RE::ActorValue av,
    float value) -> void
{
  if (!actor) { return restore_magicka_(actor, av, value); }

  const auto& config = Reflyem::Config::get_singleton();
  if (config.resource_manager().enable() && config.resource_manager().regeneration_enable())
    {
      value = Reflyem::ResourceManager::regeneration(*actor, av, player_last_delta);
    }

  return restore_magicka_(actor, av, value);
}

auto OnHealthMagickaStaminaRegeneration::restore_stamina(
    RE::Actor* actor,
    RE::ActorValue av,
    float value) -> void
{
  if (!actor) { return restore_stamina_(actor, av, value); }

  const auto& config = Reflyem::Config::get_singleton();
  if (config.resource_manager().enable() && config.resource_manager().regeneration_enable())
    {
      value = Reflyem::ResourceManager::regeneration(*actor, av, player_last_delta);
    }

  return restore_stamina_(actor, av, value);
}

auto install_hooks() -> void
{
  logger::info("start install hooks"sv);
  auto& trampoline = SKSE::GetTrampoline();
  trampoline.create(1024);
  OnMeleeCollision::install_hook(trampoline);
  OnWeaponHit::install_hook(trampoline);
  // OnCheckResistance::install_hook(trampoline);
  OnCheckResistance::install_hook();
  OnEnchIgnoresResistance::install_hook();
  OnEnchGetNoAbsorb::install_hook();
  OnActorValueOwner::install_hook();
  // OnMainUpdate::install_hook(trampoline);
  // OnAdjustActiveEffect::install_hook(trampoline);
  OnAnimationEvent::install_hook();
  OnCharacterUpdate::install_hook();
  OnModifyActorValue::install_hook(trampoline);
  OnHealthMagickaStaminaRegeneration::install_hook(trampoline);
  OnApplySpellsFromAttack::install_hook(trampoline);
  // OnActorAddObject::install_hook();
  // OnAttackAction::install_hook(trampoline);
  // OnAttackData::install_hook(trampoline);
  logger::info("finish install hooks"sv);
}
} // namespace Hooks