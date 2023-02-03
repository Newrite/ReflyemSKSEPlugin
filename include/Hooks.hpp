#pragma once

namespace Adresses {
static inline REL::Relocation<uintptr_t> on_weapon_hit{RELOCATION_ID(37673, 0)};
static inline REL::Relocation<uintptr_t> on_main_update{RELOCATION_ID(35551, 0)};
static inline REL::Relocation<uintptr_t> on_adjust_active_effect{RELOCATION_ID(33763, 0)};
static inline REL::Relocation<uintptr_t> on_animation_event_npc{RELOCATION_ID(261399, 0)};
static inline REL::Relocation<uintptr_t> on_animation_event_pc{RELOCATION_ID(261918, 0)};
static inline REL::Relocation<uintptr_t> on_attack_data{RELOCATION_ID(38047, 0)};
static inline REL::Relocation<uintptr_t> on_modify_actor_value{RELOCATION_ID(258043, 0)};
static inline REL::Relocation<uintptr_t> on_peak_modify_actor_value{RELOCATION_ID(257874, 0)};
static inline REL::Relocation<uintptr_t> on_dual_modify_actor_value{RELOCATION_ID(257681, 0)};
static inline REL::Relocation<uintptr_t> on_dual_modify_actor_value_second_inner_call{RELOCATION_ID(
    33545, 0)}; // thx Fenix31415 for this address, call inner ModifyActorValue method of DualMod
static inline REL::Relocation<uintptr_t> on_attack_action{RELOCATION_ID(48139, 0)};
static inline REL::Relocation<uintptr_t> on_actor_update_pc{RELOCATION_ID(261916, 0)};
static inline REL::Relocation<uintptr_t> on_actor_update_npc{RELOCATION_ID(261397, 0)};
static inline REL::Relocation<uintptr_t> on_check_resistance{RELOCATION_ID(37788, 0)};
static inline REL::Relocation<uintptr_t> on_check_resistance_npc{RELOCATION_ID(261401, 0)};
static inline REL::Relocation<uintptr_t> on_check_resistance_pc{RELOCATION_ID(261920, 0)};
static inline REL::Relocation<uintptr_t> on_ench_ignores_resistance{RELOCATION_ID(228570, 0)};
static inline REL::Relocation<uintptr_t> on_ench_get_no_absorb{RELOCATION_ID(228570, 0)};
} // namespace Adresses

namespace Offsets {
static inline auto on_weapon_hit                                = RELOCATION_OFFSET(0x3C0, 0);
static inline auto on_main_update                               = RELOCATION_OFFSET(0x11f, 0);
static inline auto on_adjust_active_effect                      = RELOCATION_OFFSET(0x4A3, 0);
static inline auto on_animation_event_npc                       = RELOCATION_OFFSET(0x1, 0);
static inline auto on_animation_event_pc                        = RELOCATION_OFFSET(0x1, 0);
static inline auto on_attack_data                               = RELOCATION_OFFSET(0xbb, 0);
static inline auto on_modify_actor_value                        = RELOCATION_OFFSET(0x20, 0);
static inline auto on_dual_modify_actor_value                   = RELOCATION_OFFSET(0x20, 0);
static inline auto on_peak_modify_actor_value                   = RELOCATION_OFFSET(0x20, 0);
static inline auto on_dual_modify_actor_value_second_inner_call = RELOCATION_OFFSET(0x5a, 0);
static inline auto on_attack_action                             = RELOCATION_OFFSET(0x4D7, 0);
static inline auto on_actor_update_pc                           = RELOCATION_OFFSET(0xAD, 0);
static inline auto on_actor_update_npc                          = RELOCATION_OFFSET(0xAD, 0);
static inline auto on_check_resistance                          = RELOCATION_OFFSET(0x102, 0);
static inline auto on_check_resistance_npc                      = RELOCATION_OFFSET(0x0A, 0);
static inline auto on_check_resistance_pc                       = RELOCATION_OFFSET(0x0A, 0);
static inline auto on_ench_ignores_resistance                   = RELOCATION_OFFSET(0x5B, 0);
static inline auto on_ench_get_no_absorb                        = RELOCATION_OFFSET(0x5E, 0);
} // namespace Offsets

namespace Hooks {

struct OnPlayerCharacterUpdate {
public:
  static auto install_hook() -> void {
    logger::info("start hook OnPlayerCharacterUpdate"sv);
    update_ = Adresses::on_actor_update_pc.write_vfunc(Offsets::on_actor_update_pc, update);
    logger::info("finish hook OnPlayerCharacterUpdate"sv);
  }

private:
  static auto update(RE::PlayerCharacter* this_, float delta) -> void;

  static inline REL::Relocation<decltype(update)> update_;
};

struct OnCharacterUpdate {
public:
  static auto install_hook() -> void {
    logger::info("start hook OnCharacterUpdate"sv);
    update_ = Adresses::on_actor_update_npc.write_vfunc(Offsets::on_actor_update_npc, update);
    logger::info("finish hook OnCharacterUpdate"sv);
  }

private:
  static auto update(RE::Character* this_, float delta) -> void;

  static inline REL::Relocation<decltype(update)> update_;
};

struct OnAttackData {
public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void {
    logger::info("start hook on_attack_data"sv);
    process_attack_ = trampoline.write_call<5>(
        Adresses::on_attack_data.address() + Offsets::on_attack_data, process_attack);
    logger::info("finish hook on_attack_data"sv);
  }

private:
  static auto process_attack(RE::ActorValueOwner* value_owner, RE::BGSAttackData* attack_data)
      -> void;
  static inline REL::Relocation<decltype(process_attack)> process_attack_;
};

struct OnAttackAction {
public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void {
    logger::info("start hook OnAttackAction"sv);
    attack_action_ = trampoline.write_call<5>(
        Adresses::on_attack_action.address() + Offsets::on_attack_action, attack_action);
    logger::info("finish hook OnAttackAction"sv);
  }

private:
  static auto attack_action(const RE::TESActionData* action_data) -> bool;
  static inline REL::Relocation<decltype(attack_action)> attack_action_;
};

struct OnModifyActorValue {
public:
  static auto install_hook() -> void {
    logger::info("start hook OnModifyActorValue"sv);
    modify_actor_value_ = Adresses::on_modify_actor_value.write_vfunc(
        Offsets::on_modify_actor_value, modify_actor_value);
    logger::info("finish hook OnModifyActorValue"sv);
  }

private:
  static auto modify_actor_value(RE::ValueModifierEffect* this_, RE::Actor* actor, float value,
                                 RE::ActorValue av) -> void;

  static inline REL::Relocation<decltype(modify_actor_value)> modify_actor_value_;
};

struct OnPeakModifyActorValue {
public:
  static auto install_hook() -> void {
    logger::info("start hook OnPeakModifyActorValue"sv);
    peak_modify_actor_value_ = Adresses::on_peak_modify_actor_value.write_vfunc(
        Offsets::on_peak_modify_actor_value, peak_modify_actor_value);
    logger::info("finish hook OnPeakModifyActorValue"sv);
  }

private:
  static auto peak_modify_actor_value(RE::ValueModifierEffect* this_, RE::Actor* actor, float value,
                                      RE::ActorValue av) -> void;

  static inline REL::Relocation<decltype(peak_modify_actor_value)> peak_modify_actor_value_;
};

struct OnDualModifyActorValue {
public:
  static auto install_hook() -> void {
    logger::info("start hook OnDualModifyActorValue"sv);
    dual_modify_actor_value_ = Adresses::on_dual_modify_actor_value.write_vfunc(
        Offsets::on_dual_modify_actor_value, dual_modify_actor_value);
    logger::info("finish hook OnDualModifyActorValue"sv);
  }

private:
  static auto dual_modify_actor_value(RE::ValueModifierEffect* this_, RE::Actor* actor, float value,
                                      RE::ActorValue av) -> void;

  static inline REL::Relocation<decltype(dual_modify_actor_value)> dual_modify_actor_value_;
};

struct OnDualModifyActorValueSecondInnerCall {
public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void {
    logger::info("start hook OnDualModifyActorValueSecondInnerCall"sv);
    dual_modify_actor_value_second_inner_call_ =
        trampoline.write_call<5>(Adresses::on_dual_modify_actor_value_second_inner_call.address() +
                                     Offsets::on_dual_modify_actor_value_second_inner_call,
                                 dual_modify_actor_value_second_inner_call);
    logger::info("finish hook OnDualModifyActorValueSecondInnerCall"sv);
  }

private:
  static auto dual_modify_actor_value_second_inner_call(RE::ValueModifierEffect* this_,
                                                        RE::Actor* actor, float value,
                                                        RE::ActorValue av) -> void;

  static inline REL::Relocation<decltype(dual_modify_actor_value_second_inner_call)>
      dual_modify_actor_value_second_inner_call_;
};

struct OnAnimationEventPc {
public:
  static auto install_hook() -> void {
    logger::info("start hook on_animation_event_pc"sv);
    REL::Relocation<uintptr_t> hook{RELOCATION_ID(261918, 0)};
    process_event_ = hook.write_vfunc(Offsets::on_animation_event_pc, process_event);
    logger::info("finish hook on_animation_event_pc"sv);
  }

private:
  static auto process_event(RE::BSTEventSink<RE::BSAnimationGraphEvent>*   this_,
                            RE::BSAnimationGraphEvent*                     event,
                            RE::BSTEventSource<RE::BSAnimationGraphEvent>* dispatcher) -> void;

  static inline REL::Relocation<decltype(process_event)> process_event_;
};

struct OnAnimationEventNpc {
public:
  static auto install_hook() -> void {
    logger::info("start hook on_animation_event_npc"sv);
    process_event_ =
        Adresses::on_animation_event_npc.write_vfunc(Offsets::on_animation_event_pc, process_event);
    logger::info("finish hook on_animation_event_npc"sv);
  }

private:
  static auto process_event(RE::BSTEventSink<RE::BSAnimationGraphEvent>*   this_,
                            RE::BSAnimationGraphEvent*                     event,
                            RE::BSTEventSource<RE::BSAnimationGraphEvent>* dispatcher) -> void;

  static inline REL::Relocation<decltype(process_event)> process_event_;
};

struct OnAdjustActiveEffect {
public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void {
    logger::info("start hook on_adjust_active_effect"sv);
    adjust_active_effect_ = trampoline.write_call<5>(Adresses::on_adjust_active_effect.address() +
                                                         Offsets::on_adjust_active_effect,
                                                     adjust_active_effect);
    logger::info("on_adjust_active_effect hook install"sv);
  }

private:
  static auto adjust_active_effect(RE::ActiveEffect* this_, float power, bool unk) -> void;
  static inline REL::Relocation<decltype(adjust_active_effect)> adjust_active_effect_;
};

struct OnMainUpdate {
public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void {
    logger::info("start hoon on_main_update"sv);
    main_update_ = trampoline.write_call<5>(
        Adresses::on_main_update.address() + Offsets::on_main_update, main_update);
    logger::info("on_main_update hook install"sv);
  }

private:
  static auto main_update(RE::Main* this_, float unk) -> void;
  static inline REL::Relocation<decltype(main_update)> main_update_;
};

struct OnWeaponHit {
public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void {
    logger::info("start hook on_weapon_hit"sv);
    weapon_hit_ = trampoline.write_call<5>(
        Adresses::on_weapon_hit.address() + Offsets::on_weapon_hit, weapon_hit);
    logger::info("on_weapon_hit hook install"sv);
  }

private:
  static auto weapon_hit(RE::Actor* target, RE::HitData& hit_data) -> void;
  static inline REL::Relocation<decltype(weapon_hit)> weapon_hit_;
};

struct OnCheckResistance {
public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void {
    logger::info("start hook OnCheckResistance"sv);
    check_resistance_ = trampoline.write_call<5>(
        Adresses::on_check_resistance.address() + Offsets::on_check_resistance, check_resistance);
    logger::info("OnCheckResistance hook install"sv);
  }

private:
  static auto check_resistance(RE::MagicTarget* this_, RE::MagicItem* magic_item,
                               RE::Effect* effect, RE::TESBoundObject* bound_object) -> float;
  static inline REL::Relocation<decltype(check_resistance)> check_resistance_;
};

struct OnCheckResistanceNpc {
public:
  static auto install_hook() -> void {
    logger::info("start hook OnCheckResistanceNPC"sv);
    check_resistance_ = Adresses::on_check_resistance_npc.write_vfunc(
        Offsets::on_check_resistance_npc, check_resistance);
    logger::info("finish hook OnCheckResistanceNPC"sv);
  }

private:
  static auto check_resistance(RE::MagicTarget* this_, RE::MagicItem* magic_item,
                               RE::Effect* effect, RE::TESBoundObject* bound_object) -> float;

  static inline REL::Relocation<decltype(check_resistance)> check_resistance_;
};

struct OnCheckResistancePc {
public:
  static auto install_hook() -> void {
    logger::info("start hook OnCheckResistancePc"sv);
    check_resistance_ = Adresses::on_check_resistance_pc.write_vfunc(
        Offsets::on_check_resistance_pc, check_resistance);
    logger::info("finish hook OnCheckResistancePc"sv);
  }

private:
  static auto check_resistance(RE::MagicTarget* this_, RE::MagicItem* magic_item,
                               RE::Effect* effect, RE::TESBoundObject* bound_object) -> float;

  static inline REL::Relocation<decltype(check_resistance)> check_resistance_;
};

struct OnEnchIgnoresResistance {
public:
  static auto install_hook() -> void {
    logger::info("start hook OnEnchIgnoresResistance"sv);
    ignores_resistance_ = Adresses::on_ench_ignores_resistance.write_vfunc(
        Offsets::on_ench_ignores_resistance, ignores_resistance);
    logger::info("finish hook OnEnchIgnoresResistance"sv);
  }

private:
  static auto ignores_resistance(RE::MagicItem* this_) -> bool;

  static inline REL::Relocation<decltype(ignores_resistance)> ignores_resistance_;
};

struct OnEnchGetNoAbsorb {
public:
  static auto install_hook() -> void {
    logger::info("start hook OnEnchGetNoAbsorb"sv);
    get_no_absorb_ =
        Adresses::on_ench_get_no_absorb.write_vfunc(Offsets::on_ench_get_no_absorb, get_no_absorb);
    logger::info("finish hook OnEnchGetNoAbsorb"sv);
  }

private:
  static auto get_no_absorb(RE::MagicItem* this_) -> bool;

  static inline REL::Relocation<decltype(get_no_absorb)> get_no_absorb_;
};

auto install_hooks() -> void;
} // namespace Hooks
