#include "Hooks.hpp"
#include "Core.hpp"
#include "plugin/CastOnBlock.hpp"
#include "plugin/CastOnHit.hpp"
#include "plugin/CasterAdditions.hpp"
#include "plugin/CheatDeath.hpp"
#include "plugin/Crit.hpp"
#include "plugin/MagicShield.hpp"
#include "plugin/MagicWepon.hpp"
#include "plugin/PetrifiedBlood.hpp"
#include "plugin/ResistTweaks.hpp"
#include "plugin/ResourceManager.hpp"
#include "plugin/SpeedCasting.hpp"
#include "plugin/Vampirism.hpp"

namespace Hooks {

static float player_last_delta = 0.f;

auto update_actor(RE::Character& character, const float delta, const Reflyem::Config& config)
    -> void {

  logger::debug("update actor"sv);

  auto& actors_cache = Reflyem::Core::ActorsCache::get_singleton();
  auto& actor_data   = actors_cache.get_or_add(character.formID).get();

  actor_data.set_delta_update(player_last_delta);
  actor_data.update_ticks(player_last_delta);

  const auto last_actor_tick50 =
      actor_data.last_update_tick(Reflyem::Core::ActorsCache::Data::TickValues::k50Ms);
  const auto last_actor_tick100 =
      actor_data.last_update_tick(Reflyem::Core::ActorsCache::Data::TickValues::k100Ms);
  const auto last_actor_tick1000 =
      actor_data.last_update_tick(Reflyem::Core::ActorsCache::Data::TickValues::k1000Ms);

  if (config.resource_manager().enable() && config.resource_manager().regeneration_enable()) {
    Reflyem::ResourceManager::on_update_actor_regeneration(character, actor_data);
    actor_data.mod_all_regens_delay(-player_last_delta);
  }

  if (config.magick_crit().enable() && config.cast_on_crit().enable()) {
    actor_data.mod_cast_on_crit_delay(-player_last_delta);
  }

  if (last_actor_tick50 <= 0.f) {
    logger::debug("update actor map50 tick"sv);
    actor_data.set_tick(Reflyem::Core::ActorsCache::Data::TickValues::k50Ms);
  }

  if (last_actor_tick100 <= 0.f) {
    logger::debug("update actor map100 tick"sv);
    actor_data.set_tick(Reflyem::Core::ActorsCache::Data::TickValues::k100Ms);
    if (config.resource_manager().enable()) {
      Reflyem::ResourceManager::update_actor(character, delta, config);
      if (config.resource_manager().weapon_spend_enable()) {
        Reflyem::ResourceManager::ranged_spend_handler(character, config);
      }
    }
    if (config.caster_additions().enable()) {
      Reflyem::CasterAdditions::on_update_actor(character, delta, config);
    }
  }

  if (last_actor_tick1000 <= 0.f) {
    logger::debug("update actor map1000 tick"sv);
    actor_data.set_tick(Reflyem::Core::ActorsCache::Data::TickValues::k1000Ms);
    actor_data.set_last_update();
    logger::debug("ActorData: st {} mp {} hp {}", actor_data.regen_stamina_delay(),
                  actor_data.regen_magicka_delay(), actor_data.regen_health_delay());
    if (config.petrified_blood().enable() && config.petrified_blood().magick()) {
      Reflyem::PetrifiedBlood::character_update(character, delta, config);
    }
    if (config.resource_manager().enable() && config.resource_manager().regeneration_enable()) {
      Reflyem::Core::set_av_regen_delay(character.currentProcess, RE::ActorValue::kMagicka, 2.f);
      Reflyem::Core::set_av_regen_delay(character.currentProcess, RE::ActorValue::kStamina, 2.f);
      Reflyem::Core::set_av_regen_delay(character.currentProcess, RE::ActorValue::kHealth, 2.f);
    }
    if (config.speed_casting().enable() && character.IsPlayerRef()) {
      Reflyem::SpeedCasting::on_update_actor(character, delta, config);
    }
  }
}

auto OnPlayerCharacterUpdate::update(RE::PlayerCharacter* this_, float delta) -> void {
  if (this_) {

    auto& config      = Reflyem::Config::get_singleton();
    player_last_delta = delta;
    update_actor(*this_, delta, config);
  }
  return update_(this_, delta);
}

auto OnCharacterUpdate::update(RE::Character* this_, float delta) -> void {
  if (this_) {

    auto& config = Reflyem::Config::get_singleton();

    update_actor(*this_, delta, config);
  }
  return update_(this_, delta);
}

auto OnAttackData::process_attack(RE::ActorValueOwner* value_owner, RE::BGSAttackData* attack_data)
    -> void {
  process_attack_(value_owner, attack_data);
  return;
}

auto OnAttackAction::attack_action(const RE::TESActionData* action_data) -> bool {
  logger::info("Attack Action: {}"sv,
               static_cast<std::uint32_t>(action_data->GetSourceActorState()->GetAttackState()));
  return false;
}

auto OnAnimationEventNpc::process_event(RE::BSTEventSink<RE::BSAnimationGraphEvent>*   this_,
                                        RE::BSAnimationGraphEvent*                     event,
                                        RE::BSTEventSource<RE::BSAnimationGraphEvent>* dispatcher)
    -> void {
  if (event && event->holder) {
    auto& config = Reflyem::Config::get_singleton();
    Reflyem::AnimationEventHandler::animation_handler(event, config);
  }
  process_event_(this_, event, dispatcher);
  return;
}

auto OnAnimationEventPc::process_event(RE::BSTEventSink<RE::BSAnimationGraphEvent>*   this_,
                                       RE::BSAnimationGraphEvent*                     event,
                                       RE::BSTEventSource<RE::BSAnimationGraphEvent>* dispatcher)
    -> void {
  if (event && event->holder) {
    auto& config = Reflyem::Config::get_singleton();
    Reflyem::AnimationEventHandler::animation_handler(event, config);
  }
  process_event_(this_, event, dispatcher);
  return;
}

auto OnAdjustActiveEffect::adjust_active_effect(RE::ActiveEffect* this_, float power, bool unk)
    -> void {
  if (this_) {
    const auto caster = this_->GetCasterActor();
    const auto target = this_->GetTargetActor();
    if (caster && target) {
      logger::info("Target HP: {} Caster HP: {} Target Level: {} Caster Level: {}"sv,
                   target->GetActorValue(RE::ActorValue::kHealth),
                   caster->GetActorValue(RE::ActorValue::kHealth), target->GetLevel(),
                   caster->GetLevel());
      logger::info("Effect, mag: {}, dur: {}"sv, this_->magnitude, this_->duration);
    } else {
      logger::info("Caster or target is null"sv);
    }
  }
  adjust_active_effect_(this_, power, unk);
  return;
}

auto OnModifyActorValue::modify_actor_value(RE::ValueModifierEffect* this_, RE::Actor* actor,
                                            float value, RE::ActorValue av) -> void {
  if (!actor || !this_) {
    modify_actor_value_(this_, actor, value, av);
    return;
  }

  auto& config = Reflyem::Config::get_singleton();

  if (config.magick_crit().enable()) {
    Reflyem::Crit::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.cheat_death().enable() && config.cheat_death().magick()) {
    Reflyem::CheatDeath::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.petrified_blood().enable() && config.petrified_blood().magick()) {
    Reflyem::PetrifiedBlood::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.magic_shield().enable() && config.magic_shield().magick()) {
    Reflyem::MagicShield::modify_actor_value(this_, actor, value, av, config);
  }

  Reflyem::Vampirism::modify_actor_value(this_, actor, value, av, config);

  modify_actor_value_(this_, actor, value, av);
  return;
}

auto OnPeakModifyActorValue::peak_modify_actor_value(RE::ValueModifierEffect* this_,
                                                     RE::Actor* actor, float value,
                                                     RE::ActorValue av) -> void {
  logger::debug("peak mod actor value"sv);

  if (!actor || !this_) {
    peak_modify_actor_value_(this_, actor, value, av);
    return;
  }

  auto& config = Reflyem::Config::get_singleton();

  if (config.magick_crit().enable()) {
    Reflyem::Crit::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.cheat_death().enable() && config.cheat_death().magick()) {
    Reflyem::CheatDeath::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.petrified_blood().enable() && config.petrified_blood().magick()) {
    Reflyem::PetrifiedBlood::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.magic_shield().enable() && config.magic_shield().magick()) {
    Reflyem::MagicShield::modify_actor_value(this_, actor, value, av, config);
  }

  Reflyem::Vampirism::modify_actor_value(this_, actor, value, av, config);

  peak_modify_actor_value_(this_, actor, value, av);
  return;
}

auto OnDualModifyActorValue::dual_modify_actor_value(RE::ValueModifierEffect* this_,
                                                     RE::Actor* actor, float value,
                                                     RE::ActorValue av) -> void {
  if (!actor || !this_) {
    dual_modify_actor_value_(this_, actor, value, av);
    return;
  }

  auto& config = Reflyem::Config::get_singleton();

  if (config.magick_crit().enable()) {
    Reflyem::Crit::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.cheat_death().enable() && config.cheat_death().magick()) {
    Reflyem::CheatDeath::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.petrified_blood().enable() && config.petrified_blood().magick()) {
    Reflyem::PetrifiedBlood::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.magic_shield().enable() && config.magic_shield().magick()) {
    Reflyem::MagicShield::modify_actor_value(this_, actor, value, av, config);
  }

  Reflyem::Vampirism::modify_actor_value(this_, actor, value, av, config);

  dual_modify_actor_value_(this_, actor, value, av);
  return;
}

auto OnDualModifyActorValueSecondInnerCall::dual_modify_actor_value_second_inner_call(
    RE::ValueModifierEffect* this_, RE::Actor* actor, float value, RE::ActorValue av) -> void {
  if (!actor || !this_) {
    dual_modify_actor_value_second_inner_call_(this_, actor, value, av);
    return;
  }

  auto& config = Reflyem::Config::get_singleton();

  if (config.magick_crit().enable()) {
    Reflyem::Crit::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.cheat_death().enable() && config.cheat_death().magick()) {
    Reflyem::CheatDeath::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.petrified_blood().enable() && config.petrified_blood().magick()) {
    Reflyem::PetrifiedBlood::modify_actor_value(this_, actor, value, av, config);
  }

  if (config.magic_shield().enable() && config.magic_shield().magick()) {
    Reflyem::MagicShield::modify_actor_value(this_, actor, value, av, config);
  }

  Reflyem::Vampirism::modify_actor_value(this_, actor, value, av, config);

  dual_modify_actor_value_second_inner_call_(this_, actor, value, av);
  return;
}

auto OnMainUpdate::main_update(RE::Main* this_, float unk) -> void {

  if (const auto ui = RE::UI::GetSingleton(); ui->GameIsPaused()) {
    main_update_(this_, unk);
    return;
  }

  main_update_(this_, unk);
  return;
}

auto OnWeaponHit::weapon_hit(RE::Actor* target, RE::HitData& hit_data) -> void {
  if (!target) {
    weapon_hit_(target, hit_data);
    return;
  }

  auto& config = Reflyem::Config::get_singleton();

  if (config.magic_weapon().enable()) {
    Reflyem::MagicWeapon::on_weapon_hit(*target, hit_data);
  }

  if (config.weapon_crit().enable()) {
    Reflyem::Crit::on_weapon_hit(target, hit_data, config);
  }

  if (config.resource_manager().enable() && config.resource_manager().block_spend_enable()) {
    Reflyem::ResourceManager::on_weapon_hit(target, hit_data, config);
  }

  if (config.cast_on_hit().enable()) {
    Reflyem::CastOnHit::on_weapon_hit(target, hit_data, config);
  }

  if (config.cast_on_block().enable()) {
    Reflyem::CastOnBlock::on_weapon_hit(target, hit_data, config);
  }

  if (config.cheat_death().enable() && config.cheat_death().physical()) {
    Reflyem::CheatDeath::on_weapon_hit(target, hit_data, config);
  }

  if (config.petrified_blood().enable() && config.petrified_blood().physical()) {
    Reflyem::PetrifiedBlood::on_weapon_hit(target, hit_data, config);
  }

  if (config.magic_shield().enable() && config.magic_shield().physical()) {
    Reflyem::MagicShield::on_weapon_hit(target, hit_data, config);
  }

  Reflyem::Vampirism::on_weapon_hit(target, hit_data, config);

  return weapon_hit_(target, hit_data);
}

auto OnCheckResistance::check_resistance(RE::MagicTarget* this_, RE::MagicItem* magic_item,
                                         RE::Effect* effect, RE::TESBoundObject* bound_object)
    -> float {
  logger::info("Yeay it's check resistance"sv);
  if (!this_ || !magic_item || !effect) {
    logger::info("Null object resist"sv);
    return check_resistance_(this_, magic_item, effect, bound_object);
  }
  logger::info("Not null resist check"sv);
  return check_resistance_(this_, magic_item, effect, bound_object);
}

auto OnCheckResistanceNpc::check_resistance(RE::MagicTarget* this_, RE::MagicItem* magic_item,
                                            RE::Effect* effect, RE::TESBoundObject* bound_object)
    -> float {
  if (!this_ || !magic_item || !effect) {
    logger::debug("Original resistance call");
    return check_resistance_(this_, magic_item, effect, bound_object);
  }

  const auto& config = Reflyem::Config::get_singleton();

  if (config.resist_tweaks().enable() && config.resist_tweaks().enable_check_resistance()) {
    return Reflyem::ResistTweaks::check_resistance(*this_, *magic_item, *effect, bound_object,
                                                   config);
  }

  return check_resistance_(this_, magic_item, effect, bound_object);
}

auto OnCheckResistancePc::check_resistance(RE::MagicTarget* this_, RE::MagicItem* magic_item,
                                           RE::Effect* effect, RE::TESBoundObject* bound_object)
    -> float {
  if (!this_ || !magic_item || !effect) {
    logger::debug("Original resistance call");
    return check_resistance_(this_, magic_item, effect, bound_object);
  }

  const auto& config = Reflyem::Config::get_singleton();

  if (config.resist_tweaks().enable() && config.resist_tweaks().enable_check_resistance()) {
    return Reflyem::ResistTweaks::check_resistance(*this_, *magic_item, *effect, bound_object,
                                                   config);
  }

  return check_resistance_(this_, magic_item, effect, bound_object);
}

auto OnEnchIgnoresResistance::ignores_resistance(RE::MagicItem* this_) -> bool {
  if (!this_) {
    return ignores_resistance_(this_);
  }

  const auto& config = Reflyem::Config::get_singleton();

  if (config.resist_tweaks().enable() && config.resist_tweaks().ench_ignore_resistance()) {
    return Reflyem::ResistTweaks::ignores_resistance(*this_);
  }

  return ignores_resistance_(this_);
}

auto OnEnchGetNoAbsorb::get_no_absorb(RE::MagicItem* this_) -> bool {
  if (!this_) {
    return get_no_absorb_(this_);
  }

  const auto& config = Reflyem::Config::get_singleton();

  if (config.resist_tweaks().enable() && config.resist_tweaks().ench_get_no_absorb()) {
    return Reflyem::ResistTweaks::get_no_absorb(*this_);
  }

  return get_no_absorb_(this_);
}

auto install_hooks() -> void {
  logger::info("start install hooks"sv);
  auto& trampoline = SKSE::GetTrampoline();
  trampoline.create(1024);
  OnWeaponHit::install_hook(trampoline);
  // OnCheckResistance::install_hook(trampoline);
  OnCheckResistanceNpc::install_hook();
  OnCheckResistancePc::install_hook();
  OnEnchIgnoresResistance::install_hook();
  OnEnchGetNoAbsorb::install_hook();
  // OnMainUpdate::install_hook(trampoline);
  // OnAdjustActiveEffect::install_hook(trampoline);
  OnAnimationEventNpc::install_hook();
  OnAnimationEventPc::install_hook();
  OnCharacterUpdate::install_hook();
  OnPlayerCharacterUpdate::install_hook();
  OnModifyActorValue::install_hook();
  OnDualModifyActorValue::install_hook();
  OnPeakModifyActorValue::install_hook();
  OnDualModifyActorValueSecondInnerCall::install_hook(trampoline);
  // OnAttackAction::install_hook(trampoline);
  // OnAttackData::install_hook(trampoline);
  logger::info("finish install hooks"sv);
}
} // namespace Hooks