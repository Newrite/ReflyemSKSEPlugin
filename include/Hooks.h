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
static inline REL::Relocation<uintptr_t> on_dual_modify_actor_value_second_inner_call{
    RELOCATION_ID(33545, 0)}; // thx Fenix31425 for this address, call inner ModifyActorValue method of DualMod
static inline REL::Relocation<uintptr_t> on_attack_action{RELOCATION_ID(48139, 0)};
static inline REL::Relocation<uintptr_t> on_actor_update_pc{RELOCATION_ID(261916, 0)};
static inline REL::Relocation<uintptr_t> on_actor_update_npc{RELOCATION_ID(261397, 0)};
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
} // namespace Offsets

namespace Hooks {

struct OnPlayerCharacterUpdate {
public:
  static auto install_hook() -> void {
    logger::info("start hook OnPlayerCharacterUpdate"sv);
    _update = Adresses::on_actor_update_pc.write_vfunc(Offsets::on_actor_update_pc, update);
    logger::info("finish hook OnPlayerCharacterUpdate"sv);
  }

private:
  static auto update(RE::PlayerCharacter* a_this, float delta) -> void;

  static inline REL::Relocation<decltype(update)> _update;
};

struct OnCharacterUpdate {
public:
  static auto install_hook() -> void {
    logger::info("start hook OnCharacterUpdate"sv);
    _update = Adresses::on_actor_update_npc.write_vfunc(Offsets::on_actor_update_npc, update);
    logger::info("finish hook OnCharacterUpdate"sv);
  }

private:
  static auto update(RE::Character* a_this, float delta) -> void;

  static inline REL::Relocation<decltype(update)> _update;
};

struct OnAttackData {
public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void {
    logger::info("start hook on_attack_data"sv);
    _process_attack =
        trampoline.write_call<5>(Adresses::on_attack_data.address() + Offsets::on_attack_data, process_attack);
    logger::info("finish hook on_attack_data"sv);
  }

private:
  static auto process_attack(RE::ActorValueOwner* value_owner, RE::BGSAttackData* attack_data) -> void;
  static inline REL::Relocation<decltype(process_attack)> _process_attack;
};

struct OnAttackAction {
public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void {
    logger::info("start hook OnAttackAction"sv);
    _attack_action =
        trampoline.write_call<5>(Adresses::on_attack_action.address() + Offsets::on_attack_action, attack_action);
    logger::info("finish hook OnAttackAction"sv);
  }

private:
  static auto                                            attack_action(RE::TESActionData* a_actionData) -> bool;
  static inline REL::Relocation<decltype(attack_action)> _attack_action;
};

struct OnModifyActorValue {
public:
  static auto install_hook() -> void {
    logger::info("start hook OnModifyActorValue"sv);
    _modify_actor_value =
        Adresses::on_modify_actor_value.write_vfunc(Offsets::on_modify_actor_value, modify_actor_value);
    logger::info("finish hook OnModifyActorValue"sv);
  }

private:
  static auto modify_actor_value(RE::ValueModifierEffect* a_this, RE::Actor* a_actor, float a_value,
                                 RE::ActorValue a_actorValue) -> void;

  static inline REL::Relocation<decltype(modify_actor_value)> _modify_actor_value;
};

struct OnPeakModifyActorValue {
public:
  static auto install_hook() -> void {
    logger::info("start hook OnPeakModifyActorValue"sv);
    _peak_modify_actor_value =
        Adresses::on_peak_modify_actor_value.write_vfunc(Offsets::on_peak_modify_actor_value, peak_modify_actor_value);
    logger::info("finish hook OnPeakModifyActorValue"sv);
  }

private:
  static auto peak_modify_actor_value(RE::ValueModifierEffect* a_this, RE::Actor* a_actor, float a_value,
                                      RE::ActorValue a_actorValue) -> void;

  static inline REL::Relocation<decltype(peak_modify_actor_value)> _peak_modify_actor_value;
};

struct OnDualModifyActorValue {
public:
  static auto install_hook() -> void {
    logger::info("start hook OnDualModifyActorValue"sv);
    _dual_modify_actor_value =
        Adresses::on_dual_modify_actor_value.write_vfunc(Offsets::on_dual_modify_actor_value, dual_modify_actor_value);
    logger::info("finish hook OnDualModifyActorValue"sv);
  }

private:
  static auto dual_modify_actor_value(RE::ValueModifierEffect* a_this, RE::Actor* a_actor, float a_value,
                                      RE::ActorValue a_actorValue) -> void;

  static inline REL::Relocation<decltype(dual_modify_actor_value)> _dual_modify_actor_value;
};

struct OnDualModifyActorValueSecondInnerCall {
public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void {
    logger::info("start hook OnDualModifyActorValueSecondInnerCall"sv);
    _dual_modify_actor_value_second_inner_call =
        trampoline.write_call<5>(Adresses::on_dual_modify_actor_value_second_inner_call.address() +
                                     Offsets::on_dual_modify_actor_value_second_inner_call,
                                 dual_modify_actor_value_second_inner_call);
    logger::info("finish hook OnDualModifyActorValueSecondInnerCall"sv);
  }

private:
  static auto dual_modify_actor_value_second_inner_call(RE::ValueModifierEffect* a_this, RE::Actor* a_actor,
                                                        float a_value, RE::ActorValue a_actorValue) -> void;

  static inline REL::Relocation<decltype(dual_modify_actor_value_second_inner_call)>
      _dual_modify_actor_value_second_inner_call;
};

struct OnAnimationEventPc {
public:
  static auto install_hook() -> void {
    logger::info("start hook on_animation_event_pc"sv);
    REL::Relocation<uintptr_t> hook{RELOCATION_ID(261918, 0)};
    _process_event = hook.write_vfunc(Offsets::on_animation_event_pc, process_event);
    logger::info("finish hook on_animation_event_pc"sv);
  }

private:
  static auto process_event(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this, RE::BSAnimationGraphEvent* a_event,
                            RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_dispatcher) -> void;

  static inline REL::Relocation<decltype(process_event)> _process_event;
};

struct OnAnimationEventNpc {
public:
  static auto install_hook() -> void {
    logger::info("start hook on_animation_event_npc"sv);
    _process_event = Adresses::on_animation_event_npc.write_vfunc(Offsets::on_animation_event_pc, process_event);
    logger::info("finish hook on_animation_event_npc"sv);
  }

private:
  static auto process_event(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this, RE::BSAnimationGraphEvent* a_event,
                            RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_dispatcher) -> void;

  static inline REL::Relocation<decltype(process_event)> _process_event;
};

struct OnAdjustActiveEffect {
public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void {
    logger::info("start hook on_adjust_active_effect"sv);
    _adjust_active_effect = trampoline.write_call<5>(
        Adresses::on_adjust_active_effect.address() + Offsets::on_adjust_active_effect, adjust_active_effect);
    logger::info("on_adjust_active_effect hook install"sv);
  }

private:
  static auto adjust_active_effect(RE::ActiveEffect* a_this, float a_power, bool a_arg3) -> void;
  static inline REL::Relocation<decltype(adjust_active_effect)> _adjust_active_effect;
};

struct OnMainUpdate {
public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void {
    logger::info("start hoon on_main_update"sv);
    _main_update = trampoline.write_call<5>(Adresses::on_main_update.address() + Offsets::on_main_update, main_update);
    logger::info("on_main_update hook install"sv);
  }

private:
  static auto                                          main_update(RE::Main* a_this, float a2) -> void;
  static inline REL::Relocation<decltype(main_update)> _main_update;
};

struct OnWeaponHit {
public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void {
    logger::info("start hook on_weapon_hit"sv);
    _weapon_hit = trampoline.write_call<5>(Adresses::on_weapon_hit.address() + Offsets::on_weapon_hit, weapon_hit);
    logger::info("on_weapon_hit hook install"sv);
  }

private:
  static auto                                         weapon_hit(RE::Actor* target, RE::HitData& hit_data) -> void;
  static inline REL::Relocation<decltype(weapon_hit)> _weapon_hit;
};

auto install_hooks() -> void;
} // namespace Hooks