#include "Hooks.h"
#include "CastOnHit.h"
#include "CheatDeath.h"
#include "Core.h"
#include "Crit.h"
#include "MagicShield.h"
#include "PetrifiedBlood.h"
#include "ResourceManager.h"
#include "SpeedCasting.h"
#include "Vampirism.h"

namespace Hooks
{

	static ULONGLONG timer500 = 0;
	static ULONGLONG timer100 = 0;

	auto OnAttackData::process_attack(RE::ActorValueOwner* value_owner, RE::BGSAttackData* attack_data) -> void
	{
		_process_attack(value_owner, attack_data);
		return;
	}

	auto OnAttackAction::attack_action(RE::TESActionData* a_actionData) -> bool
	{
		logger::info("Attack Action: {}", static_cast<std::uint32_t>(a_actionData->GetSourceActorState()->GetAttackState()));
		return false;
	}

	auto OnAnimationEventNpc::process_event(
		RE::BSTEventSink<RE::BSAnimationGraphEvent>*   a_this,
		RE::BSAnimationGraphEvent*                     a_event,
		RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_dispatcher) -> void
	{
		if (a_event && a_event->holder) {
			auto& config = Reflyem::Config::get_singleton();
			Reflyem::AnimationEventHandler::animation_handler(a_event, config);
		}
		_process_event(a_this, a_event, a_dispatcher);
		return;
	}

	auto OnAnimationEventPc::process_event(
		RE::BSTEventSink<RE::BSAnimationGraphEvent>*   a_this,
		RE::BSAnimationGraphEvent*                     a_event,
		RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_dispatcher) -> void
	{
		if (a_event && a_event->holder) {
			auto& config = Reflyem::Config::get_singleton();
			Reflyem::AnimationEventHandler::animation_handler(a_event, config);
		}
		_process_event(a_this, a_event, a_dispatcher);
		return;
	}

	auto OnAdjustActiveEffect::adjust_active_effect(RE::ActiveEffect* a_this, float a_power, bool a_arg3) -> void
	{
		_adjust_active_effect(a_this, a_power, a_arg3);
		return;
	}

	auto OnModifyActorValue::modify_actor_value(
		RE::ValueModifierEffect* a_this,
		RE::Actor* a_actor, float a_value,
		RE::ActorValue a_actorValue) -> void
	{
		if (!a_actor || !a_this) {
			_modify_actor_value(a_this, a_actor, a_value, a_actorValue);
			return;
		}

		auto& config = Reflyem::Config::get_singleton();

		if (config.magic_crit_enable) {
			Reflyem::Crit::modify_actor_value(a_this, a_actor, a_value, a_actorValue, config);
		}

		if (config.petrified_blood_enable && config.petrified_blood_magick) {
			Reflyem::PetrifiedBlood::modify_actor_value(a_this, a_actor, a_value, a_actorValue, config);
		}

		if (config.magic_shield_enable && config.magic_shield_magick) {
			Reflyem::MagicShield::modify_actor_value(a_this, a_actor, a_value, a_actorValue, config);
		}

		Reflyem::Vampirism::modify_actor_value(a_this, a_actor, a_value, a_actorValue, config);

		_modify_actor_value(a_this, a_actor, a_value, a_actorValue);
		return;
	}

	auto OnDualModifyActorValue::dual_modify_actor_value(
		RE::ValueModifierEffect* a_this,
		RE::Actor* a_actor, float a_value,
		RE::ActorValue a_actorValue) -> void
	{
		if (!a_actor || !a_this) {
			_dual_modify_actor_value(a_this, a_actor, a_value, a_actorValue);
			return;
		}

		auto& config = Reflyem::Config::get_singleton();

		if (config.magic_crit_enable) {
			Reflyem::Crit::modify_actor_value(a_this, a_actor, a_value, a_actorValue, config);
		}

		if (config.petrified_blood_enable && config.petrified_blood_magick) {
			Reflyem::PetrifiedBlood::modify_actor_value(a_this, a_actor, a_value, a_actorValue, config);
		}

		if (config.magic_shield_enable && config.magic_shield_magick) {
			Reflyem::MagicShield::modify_actor_value(a_this, a_actor, a_value, a_actorValue, config);
		}

		Reflyem::Vampirism::modify_actor_value(a_this, a_actor, a_value, a_actorValue, config);

		_dual_modify_actor_value(a_this, a_actor, a_value, a_actorValue);
		return;
	}

	auto OnDualModifyActorValueSecondInnerCall::dual_modify_actor_value_second_inner_call(
		RE::ValueModifierEffect* a_this,
		RE::Actor* a_actor, float a_value,
		RE::ActorValue a_actorValue) -> void
	{
		if (!a_actor || !a_this) {
			_dual_modify_actor_value_second_inner_call(a_this, a_actor, a_value, a_actorValue);
			return;
		}

		auto& config = Reflyem::Config::get_singleton();

		if (config.magic_crit_enable) {
			Reflyem::Crit::modify_actor_value(a_this, a_actor, a_value, a_actorValue, config);
		}

		if (config.petrified_blood_enable && config.petrified_blood_magick) {
			Reflyem::PetrifiedBlood::modify_actor_value(a_this, a_actor, a_value, a_actorValue, config);
		}

		if (config.magic_shield_enable && config.magic_shield_magick) {
			Reflyem::MagicShield::modify_actor_value(a_this, a_actor, a_value, a_actorValue, config);
		}

		Reflyem::Vampirism::modify_actor_value(a_this, a_actor, a_value, a_actorValue, config);

		_dual_modify_actor_value_second_inner_call(a_this, a_actor, a_value, a_actorValue);
		return;
	}

	auto OnMainUpdate::main_update(RE::Main* a_this, float a2) -> void
	{
		auto ui = RE::UI::GetSingleton();

		if (ui->GameIsPaused()) {
			_main_update(a_this, a2);
			return;
		}

		auto tick = GetTickCount64();

		if (tick - timer500 >= 500) {
			timer500 = tick;
			auto& config = Reflyem::Config::get_singleton();

			if (config.speed_casting_enable) {
				Reflyem::SpeedCasting::on_main_update(a_this, a2, config);
			}
		}

		if (tick - timer100 >= 100) {
			timer100 = tick;
			auto& config = Reflyem::Config::get_singleton();
			if (config.resource_manager_enable) {
				Reflyem::ResourceManager::ranged_spend_handler();
			}
		}

		_main_update(a_this, a2);
		return;
	}

	auto OnWeaponHit::weapon_hit(RE::Actor* target, RE::HitData& hit_data) -> void
	{
		if (!target) {
			_weapon_hit(target, hit_data);
			return;
		}

		auto& config = Reflyem::Config::get_singleton();

		if (config.weapon_crit_enable) {
			Reflyem::Crit::on_weapon_hit(target, hit_data, config);
		}

		if (config.resource_manager_enable && config.resource_manager_block_spend_enable) {
			Reflyem::ResourceManager::on_weapon_hit(target, hit_data, config);
		}

		if (config.cast_on_hit_enable) {
			Reflyem::CastOnHit::on_weapon_hit(target, hit_data, config);
		}

		if (config.cheat_death_enable && config.cheat_death_physical) {
			Reflyem::CheatDeath::on_weapon_hit(target, hit_data, config);
		}

		if (config.petrified_blood_enable && config.petrified_blood_physical) {
			Reflyem::PetrifiedBlood::on_weapon_hit(target, hit_data, config);
		}

		if (config.magic_shield_enable && config.magic_shield_physical) {
			Reflyem::MagicShield::on_weapon_hit(target, hit_data, config);
		}

		Reflyem::Vampirism::on_weapon_hit(target, hit_data, config);

		_weapon_hit(target, hit_data);
		return;
	}

	auto install_hooks() -> void
	{
		logger::info("start install hooks");
		auto& trampoline = SKSE::GetTrampoline();
		trampoline.create(1024);
		OnWeaponHit::install_hook(trampoline);
		OnMainUpdate::install_hook(trampoline);
		// OnAdjustActiveEffect::install_hook(trampoline);
		OnAnimationEventNpc::install_hook();
		OnAnimationEventPc::install_hook();
		OnModifyActorValue::install_hook();
		OnDualModifyActorValue::install_hook();
		OnDualModifyActorValueSecondInnerCall::install_hook(trampoline);
		// OnAttackAction::install_hook(trampoline);
		// OnAttackData::install_hook(trampoline);
		logger::info("finish install hooks");
	}
}