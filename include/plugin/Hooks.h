#pragma once

namespace Adresses
{
  constexpr auto on_weapon_hit = 37673;
  constexpr auto on_main_update = 35551;
  constexpr auto on_adjust_active_effect = 33763;
  constexpr auto on_animation_event_npc = 261399;
  constexpr auto on_animation_event_pc = 261918;
  constexpr auto on_attack_data = 38047;
  constexpr auto on_modify_actor_value = 258043;
  constexpr auto on_dual_modify_actor_value = 257681;
  constexpr auto on_dual_modify_actor_value_second_inner_call = 33545;
}

namespace Offsets
{
  constexpr auto on_weapon_hit = 0x3C0;
  constexpr auto on_main_update = 0x11f;
  constexpr auto on_adjust_active_effect = 0x4A3;
  constexpr auto on_animation_event_npc = 0x1;
  constexpr auto on_animation_event_pc = 0x1;
  constexpr auto on_attack_data = 0xbb;
  constexpr auto on_modify_actor_value = 0x20;
  constexpr auto on_dual_modify_actor_value = 0x20;
  constexpr auto on_dual_modify_actor_value_second_inner_call = 0x5a;
}

namespace Hooks
{

  struct OnAttackData
  {
  public:
    static auto install_hook(SKSE::Trampoline& trampoline) -> void
    {
      logger::info("start hook on_attack_data");
      REL::Relocation<uintptr_t> hook{ RELOCATION_ID(Adresses::on_attack_data,0) };
      _process_attack = trampoline.write_call<5>(hook.address() + RELOCATION_OFFSET(Offsets::on_attack_data, 0), process_attack);
      logger::info("finish hook on_attack_data");
    }
  private:
    static auto process_attack(RE::ActorValueOwner* value_owner, RE::BGSAttackData* attack_data) -> void;
    static inline REL::Relocation<decltype(process_attack)> _process_attack;
  };

  struct OnModifyActorValue
  {
  public:
    static auto install_hook() -> void
    {
      logger::info("start hook OnModifyActorValue");
      REL::Relocation<uintptr_t> hook{ RELOCATION_ID(Adresses::on_modify_actor_value, 0)};
      _modify_actor_value = hook.write_vfunc(Offsets::on_modify_actor_value, modify_actor_value);
      logger::info("finish hook OnModifyActorValue");
    }

  private:
    static auto modify_actor_value(
      RE::ValueModifierEffect* a_this,
      RE::Actor* a_actor, float a_value,
      RE::ActorValue a_actorValue) -> void;

    static inline REL::Relocation<decltype(modify_actor_value)> _modify_actor_value;
  };

  struct OnDualModifyActorValue
  {
  public:
    static auto install_hook() -> void
    {
      logger::info("start hook OnDualModifyActorValue");
      REL::Relocation<uintptr_t> hook{ RELOCATION_ID(Adresses::on_dual_modify_actor_value, 0) };
      _dual_modify_actor_value = hook.write_vfunc(Offsets::on_dual_modify_actor_value, dual_modify_actor_value);
      logger::info("finish hook OnDualModifyActorValue");
    }
  private:
    static auto dual_modify_actor_value(
      RE::ValueModifierEffect* a_this,
      RE::Actor* a_actor, float a_value,
      RE::ActorValue a_actorValue) -> void;

    static inline REL::Relocation<decltype(dual_modify_actor_value)> _dual_modify_actor_value;
  };

  struct OnDualModifyActorValueSecondInnerCall
  {
  public:
    static auto install_hook(SKSE::Trampoline& trampoline) -> void
    {
      logger::info("start hook OnDualModifyActorValueSecondInnerCall");
      REL::Relocation<uintptr_t> hook{ RELOCATION_ID(Adresses::on_dual_modify_actor_value_second_inner_call, 0) };
      _dual_modify_actor_value_second_inner_call =
        trampoline.write_call<5>(
          hook.address() + RELOCATION_OFFSET(Offsets::on_dual_modify_actor_value_second_inner_call, 0), dual_modify_actor_value_second_inner_call);
      logger::info("finish hook OnDualModifyActorValueSecondInnerCall");
    }
  private:
    static auto dual_modify_actor_value_second_inner_call(
      RE::ValueModifierEffect* a_this,
      RE::Actor* a_actor, float a_value,
      RE::ActorValue a_actorValue) -> void;

    static inline REL::Relocation<decltype(dual_modify_actor_value_second_inner_call)> _dual_modify_actor_value_second_inner_call;
  };

  struct OnAnimationEventPc
  {
  public:
    static auto install_hook() -> void
    {
      logger::info("start hook on_animation_event_pc");
      REL::Relocation<uintptr_t> hook{ RELOCATION_ID(Adresses::on_animation_event_pc, 0) };
      _process_event = hook.write_vfunc(Offsets::on_animation_event_pc, process_event);
      logger::info("finish hook on_animation_event_pc");
    }
  private:
    static auto process_event(
      RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this,
      RE::BSAnimationGraphEvent* a_event,
      RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_dispatcher) -> void;

    static inline REL::Relocation<decltype(process_event)> _process_event;
  };

  struct OnAnimationEventNpc
  {
  public:
    static auto install_hook() -> void
    {
      logger::info("start hook on_animation_event_npc");
      REL::Relocation<uintptr_t> hook{ RELOCATION_ID(Adresses::on_animation_event_npc, 0) };
      _process_event = hook.write_vfunc(Offsets::on_animation_event_pc, process_event);
      logger::info("finish hook on_animation_event_npc");
    }
  private:
    static auto process_event(
      RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this,
      RE::BSAnimationGraphEvent* a_event,
      RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_dispatcher) -> void;

    static inline REL::Relocation<decltype(process_event)> _process_event;
  };

  struct OnAdjustActiveEffect
  {
  public:
    static auto install_hook(SKSE::Trampoline& trampoline) -> void
    {
      logger::info("start hook on_adjust_active_effect");
      REL::Relocation<uintptr_t> hook{ RELOCATION_ID(Adresses::on_adjust_active_effect, 0) };
      _adjust_active_effect = trampoline.write_call<5>(hook.address() + RELOCATION_OFFSET(Offsets::on_adjust_active_effect, 0), adjust_active_effect);
      logger::info("on_adjust_active_effect hook install");
    }

  private:
    static auto adjust_active_effect(RE::ActiveEffect* a_this, float a_power, bool a_arg3) -> void;
    static inline REL::Relocation<decltype(adjust_active_effect)> _adjust_active_effect;
  };

  struct OnMainUpdate
  {
  public:
    static auto install_hook(SKSE::Trampoline& trampoline) -> void
    {
      logger::info("start hoon on_main_update");
      REL::Relocation<uintptr_t> hook{ RELOCATION_ID(Adresses::on_main_update, 0) };
      _main_update = trampoline.write_call<5>(hook.address() + RELOCATION_OFFSET(Offsets::on_main_update, 0), main_update);
      logger::info("on_main_update hook install");
    }

  private:
    static auto main_update(RE::Main* a_this, float a2) -> void;
    static inline REL::Relocation<decltype(main_update)> _main_update;
  };

  struct OnWeaponHit
  {
  public:
    static auto install_hook(SKSE::Trampoline& trampoline) -> void
    {
      logger::info("start hook on_weapon_hit");
      REL::Relocation<uintptr_t> hook{ RELOCATION_ID(Adresses::on_weapon_hit, 0) };
      _weapon_hit = trampoline.write_call<5>(hook.address() + RELOCATION_OFFSET(Offsets::on_weapon_hit, 0), weapon_hit);
      logger::info("on_weapon_hit hook install");
    }

  private:
    static auto weapon_hit(RE::Actor* target, RE::HitData& hit_data) -> void;
    static inline REL::Relocation<decltype(weapon_hit)> _weapon_hit;
  };

  auto install_hooks() -> void;
}