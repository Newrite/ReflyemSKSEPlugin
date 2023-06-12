#pragma once

namespace Adresses
{
static inline REL::Relocation<uintptr_t> on_weapon_hit{RELOCATION_ID(37673, 0)};
// static inline REL::Relocation<uintptr_t> on_init_hit_data{RELOCATION_ID(37673, 0)};
static inline REL::Relocation<uintptr_t> on_apply_spells_from_attack{RELOCATION_ID(37673, 0)};
static inline REL::Relocation<uintptr_t> on_regeneration_health{RELOCATION_ID(37509, 0)};
static inline REL::Relocation<uintptr_t> on_regeneration_stamina{RELOCATION_ID(37510, 0)};
static inline REL::Relocation<uintptr_t> on_regeneration_magicka{RELOCATION_ID(37511, 0)};
static inline REL::Relocation<uintptr_t> on_melee_collision{RELOCATION_ID(37650, 0)};
static inline REL::Relocation<uintptr_t> on_main_update{RELOCATION_ID(35551, 0)};
static inline REL::Relocation<uintptr_t> on_adjust_active_effect{RELOCATION_ID(33763, 0)};
static inline REL::Relocation<uintptr_t> on_animation_event_npc{RELOCATION_ID(261399, 0)};
static inline REL::Relocation<uintptr_t> on_animation_event_pc{RELOCATION_ID(261918, 0)};
static inline REL::Relocation<uintptr_t> on_attack_data{RELOCATION_ID(38047, 0)};
static inline REL::Relocation<uintptr_t> on_modify_actor_value{RELOCATION_ID(258043, 0)};
static inline REL::Relocation<uintptr_t> on_absorb_effect_modify_actor_value{
    RELOCATION_ID(257544, 0)};
static inline REL::Relocation<uintptr_t> on_peak_modify_actor_value{RELOCATION_ID(257874, 0)};
static inline REL::Relocation<uintptr_t> on_dual_modify_actor_value{RELOCATION_ID(257681, 0)};
static inline REL::Relocation<uintptr_t> on_dual_modify_actor_value_second_inner_call{RELOCATION_ID(
    33545,
    0)}; // thx Fenix31415 for this address, call inner ModifyActorValue method of DualMod
static inline REL::Relocation<uintptr_t> on_attack_action{RELOCATION_ID(48139, 0)};
static inline REL::Relocation<uintptr_t> on_actor_update_pc{RELOCATION_ID(261916, 0)};
static inline REL::Relocation<uintptr_t> on_actor_update_npc{RELOCATION_ID(261397, 0)};
static inline REL::Relocation<uintptr_t> on_actor_add_object_pc{RELOCATION_ID(261916, 0)};
static inline REL::Relocation<uintptr_t> on_actor_pickup_object_pc{RELOCATION_ID(261916, 0)};
static inline REL::Relocation<uintptr_t> on_check_resistance_npc{RELOCATION_ID(261401, 0)};
static inline REL::Relocation<uintptr_t> on_check_resistance_pc{RELOCATION_ID(261920, 0)};
static inline REL::Relocation<uintptr_t> on_ench_ignores_resistance{RELOCATION_ID(228570, 0)};
static inline REL::Relocation<uintptr_t> on_ench_get_no_absorb{RELOCATION_ID(228570, 0)};
static inline REL::Relocation<uintptr_t> on_trap_apply_damage{RELOCATION_ID(36509, 0)};
static inline REL::Relocation<uintptr_t> on_inventory_open{RELOCATION_ID(
    50239,
    0)}; // thx Fenix31415! https://github.com/fenix31415/DeathLoseLoot/blob/master/src/main.cpp#L134
static inline REL::Relocation<uintptr_t> on_value_owner_get_actor_value_npc{
    RELOCATION_ID(261402, 0)};
static inline REL::Relocation<uintptr_t> on_value_owner_set_actor_value_npc{
    RELOCATION_ID(261402, 0)};
static inline REL::Relocation<uintptr_t> on_value_owner_mod_actor_value_npc{
    RELOCATION_ID(261402, 0)};
static inline REL::Relocation<uintptr_t> on_value_owner_get_actor_value_pc{
    RELOCATION_ID(261921, 0)};
static inline REL::Relocation<uintptr_t> on_value_owner_set_actor_value_pc{
    RELOCATION_ID(261921, 0)};
static inline REL::Relocation<uintptr_t> on_value_owner_mod_actor_value_pc{
    RELOCATION_ID(261921, 0)};
} // namespace Adresses

namespace Offsets
{
static inline auto on_weapon_hit = RELOCATION_OFFSET(0x3C0, 0);
// static inline auto on_init_hit_data                             = RELOCATION_OFFSET(0x18F, 0);
static inline auto on_apply_spells_from_attack = RELOCATION_OFFSET(0x185, 0);
static inline auto on_regeneration_health = RELOCATION_OFFSET(0x68, 0);
static inline auto on_regeneration_stamina = RELOCATION_OFFSET(0x176, 0);
static inline auto on_regeneration_magicka = RELOCATION_OFFSET(0x68, 0);
static inline auto on_melee_collision = RELOCATION_OFFSET(0x38B, 0);
static inline auto on_main_update = RELOCATION_OFFSET(0x11f, 0);
static inline auto on_adjust_active_effect = RELOCATION_OFFSET(0x4A3, 0);
static inline auto on_animation_event_npc = RELOCATION_OFFSET(0x1, 0);
static inline auto on_animation_event_pc = RELOCATION_OFFSET(0x1, 0);
static inline auto on_attack_data = RELOCATION_OFFSET(0xbb, 0);
static inline auto on_modify_actor_value = RELOCATION_OFFSET(0x20, 0);
static inline auto on_absorb_effect_modify_actor_value = RELOCATION_OFFSET(0x20, 0);
static inline auto on_dual_modify_actor_value = RELOCATION_OFFSET(0x20, 0);
static inline auto on_peak_modify_actor_value = RELOCATION_OFFSET(0x20, 0);
static inline auto on_dual_modify_actor_value_second_inner_call = RELOCATION_OFFSET(0x5a, 0);
static inline auto on_attack_action = RELOCATION_OFFSET(0x4D7, 0);
static inline auto on_actor_update_pc = RELOCATION_OFFSET(0xAD, 0);
static inline auto on_actor_update_npc = RELOCATION_OFFSET(0xAD, 0);
static inline auto on_actor_add_object_pc = RELOCATION_OFFSET(0x5A, 0);
static inline auto on_actor_pickup_object_pc = RELOCATION_OFFSET(0xCC, 0);
static inline auto on_check_resistance_npc = RELOCATION_OFFSET(0x0A, 0);
static inline auto on_check_resistance_pc = RELOCATION_OFFSET(0x0A, 0);
static inline auto on_ench_ignores_resistance = RELOCATION_OFFSET(0x5B, 0);
static inline auto on_ench_get_no_absorb = RELOCATION_OFFSET(0x5E, 0);
static inline auto on_trap_apply_damage = RELOCATION_OFFSET(0x43, 0);
static inline auto on_inventory_open = RELOCATION_OFFSET(
    0x13,
    0); // thx Fenix31415! https://github.com/fenix31415/DeathLoseLoot/blob/master/src/main.cpp#L134
static inline auto on_value_owner_get_actor_value_npc = RELOCATION_OFFSET(0x01, 0);
static inline auto on_value_owner_set_actor_value_npc = RELOCATION_OFFSET(0x07, 0);
static inline auto on_value_owner_mod_actor_value_npc = RELOCATION_OFFSET(0x05, 0);
static inline auto on_value_owner_get_actor_value_pc = RELOCATION_OFFSET(0x01, 0);
static inline auto on_value_owner_set_actor_value_pc = RELOCATION_OFFSET(0x07, 0);
static inline auto on_value_owner_mod_actor_value_pc = RELOCATION_OFFSET(0x05, 0);
} // namespace Offsets

namespace Hooks
{

struct OnCharacterUpdate final
{
  public:
  static auto install_hook() -> void
  {
    logger::info("start hook OnCharacterUpdate"sv);
    update_npc_ =
        Adresses::on_actor_update_npc.write_vfunc(Offsets::on_actor_update_npc, update_npc);
    update_pc_ = Adresses::on_actor_update_pc.write_vfunc(Offsets::on_actor_update_pc, update_pc);
    logger::info("finish hook OnCharacterUpdate"sv);
  }

  private:
  static auto update_npc(RE::Character* this_, float delta) -> void;
  static auto update_pc(RE::PlayerCharacter* this_, float delta) -> void;

  static inline REL::Relocation<decltype(update_npc)> update_npc_;
  static inline REL::Relocation<decltype(update_pc)> update_pc_;
};

struct OnActorAddObject final
{
  public:
  static auto install_hook() -> void
  {
    logger::info("start hook OnActorAddObject"sv);
    add_object_to_container_ = Adresses::on_actor_add_object_pc.write_vfunc(
        Offsets::on_actor_add_object_pc,
        add_object_to_container);
    pick_up_object_ = Adresses::on_actor_pickup_object_pc.write_vfunc(
        Offsets::on_actor_pickup_object_pc,
        pick_up_object);
    logger::info("finish hook OnActorAddObject"sv);
  }

  private:
  static auto add_object_to_container(
      RE::Actor* this_,
      RE::TESBoundObject* object,
      RE::ExtraDataList* extra_list,
      int32_t count,
      RE::TESObjectREFR* from_refr) -> void;
  static auto pick_up_object(
      RE::Actor* this_,
      RE::TESObjectREFR* object,
      int32_t count,
      bool arg3 = false,
      bool play_sound = true) -> void;

  static inline REL::Relocation<decltype(add_object_to_container)> add_object_to_container_;
  static inline REL::Relocation<decltype(pick_up_object)> pick_up_object_;
};

struct OnAttackData final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook on_attack_data"sv);
    process_attack_ = trampoline.write_call<5>(
        Adresses::on_attack_data.address() + Offsets::on_attack_data,
        process_attack);
    logger::info("finish hook on_attack_data"sv);
  }

  private:
  static auto process_attack(RE::ActorValueOwner* value_owner, RE::BGSAttackData* attack_data)
      -> void;
  static inline REL::Relocation<decltype(process_attack)> process_attack_;
};

struct OnInventoryOpen final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnInventoryOpen"sv);
    is_displayed_item_ = trampoline.write_call<5>(
        Adresses::on_inventory_open.address() + Offsets::on_inventory_open,
        is_displayed_item);
    logger::info("finish hook OnInventoryOpen"sv);
  }

  private:
  static auto is_displayed_item(RE::InventoryEntryData* item) -> bool;
  static inline REL::Relocation<decltype(is_displayed_item)> is_displayed_item_;
};

struct OnApplySpellsFromAttack
{
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnApplySpellsFromAttack"sv);
    apply_spells_from_attack_ = trampoline.write_call<5>(
        Adresses::on_apply_spells_from_attack.address() + Offsets::on_apply_spells_from_attack,
        apply_spells_from_attack);
    logger::info("finish hook OnApplySpellsFromAttack"sv);
  }

  static auto apply_spells_from_attack(
      RE::Character* attacker,
      RE::InventoryEntryData* weapon,
      bool is_left,
      RE::Actor* target) -> void;
  static inline REL::Relocation<decltype(apply_spells_from_attack)> apply_spells_from_attack_;
};

struct OnAttackAction final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnAttackAction"sv);
    attack_action_ = trampoline.write_call<5>(
        Adresses::on_attack_action.address() + Offsets::on_attack_action,
        attack_action);
    logger::info("finish hook OnAttackAction"sv);
  }

  private:
  static auto attack_action(const RE::TESActionData* action_data) -> bool;
  static inline REL::Relocation<decltype(attack_action)> attack_action_;
};

struct OnModifyActorValue final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnModifyActorValue"sv);
    modify_actor_value_ = Adresses::on_modify_actor_value.write_vfunc(
        Offsets::on_modify_actor_value,
        modify_actor_value);
    absorb_effect_modify_actor_value_ = Adresses::on_absorb_effect_modify_actor_value.write_vfunc(
        Offsets::on_absorb_effect_modify_actor_value,
        modify_actor_value);
    peak_modify_actor_value_ = Adresses::on_peak_modify_actor_value.write_vfunc(
        Offsets::on_peak_modify_actor_value,
        peak_modify_actor_value);
    dual_modify_actor_value_ = Adresses::on_dual_modify_actor_value.write_vfunc(
        Offsets::on_dual_modify_actor_value,
        dual_modify_actor_value);
    dual_modify_actor_value_second_inner_call_ = trampoline.write_call<5>(
        Adresses::on_dual_modify_actor_value_second_inner_call.address() +
            Offsets::on_dual_modify_actor_value_second_inner_call,
        dual_modify_actor_value_second_inner_call);
    logger::info("finish hook OnModifyActorValue"sv);
  }

  private:
  static auto modify_actor_value(
      RE::ValueModifierEffect* this_,
      RE::Actor* actor,
      float value,
      RE::ActorValue av) -> void;
  static auto absorb_effect_modify_actor_value(
      RE::ValueModifierEffect* this_,
      RE::Actor* actor,
      float value,
      RE::ActorValue av) -> void;
  static auto peak_modify_actor_value(
      RE::ValueModifierEffect* this_,
      RE::Actor* actor,
      float value,
      RE::ActorValue av) -> void;
  static auto dual_modify_actor_value(
      RE::ValueModifierEffect* this_,
      RE::Actor* actor,
      float value,
      RE::ActorValue av) -> void;
  static auto dual_modify_actor_value_second_inner_call(
      RE::ValueModifierEffect* this_,
      RE::Actor* actor,
      float value,
      RE::ActorValue av) -> void;

  static inline REL::Relocation<decltype(modify_actor_value)> modify_actor_value_;
  static inline REL::Relocation<decltype(absorb_effect_modify_actor_value)>
      absorb_effect_modify_actor_value_;
  static inline REL::Relocation<decltype(peak_modify_actor_value)> peak_modify_actor_value_;
  static inline REL::Relocation<decltype(dual_modify_actor_value)> dual_modify_actor_value_;
  static inline REL::Relocation<decltype(dual_modify_actor_value_second_inner_call)>
      dual_modify_actor_value_second_inner_call_;
};

struct OnAnimationEvent final
{
  public:
  static auto install_hook() -> void
  {
    logger::info("start hook on_animation_event"sv);
    REL::Relocation<uintptr_t> hook{RELOCATION_ID(261918, 0)};
    process_event_pc_ = hook.write_vfunc(Offsets::on_animation_event_pc, process_event_pc);
    process_event_npc_ = Adresses::on_animation_event_npc.write_vfunc(
        Offsets::on_animation_event_npc,
        process_event_npc);
    logger::info("finish hook on_animation_event"sv);
  }

  private:
  static auto process_event_pc(
      RE::BSTEventSink<RE::BSAnimationGraphEvent>* this_,
      RE::BSAnimationGraphEvent* event,
      RE::BSTEventSource<RE::BSAnimationGraphEvent>* dispatcher) -> void;
  static auto process_event_npc(
      RE::BSTEventSink<RE::BSAnimationGraphEvent>* this_,
      RE::BSAnimationGraphEvent* event,
      RE::BSTEventSource<RE::BSAnimationGraphEvent>* dispatcher) -> void;

  static inline REL::Relocation<decltype(process_event_pc)> process_event_pc_;
  static inline REL::Relocation<decltype(process_event_npc)> process_event_npc_;
};

struct OnAdjustActiveEffect final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook on_adjust_active_effect"sv);
    adjust_active_effect_ = trampoline.write_call<5>(
        Adresses::on_adjust_active_effect.address() + Offsets::on_adjust_active_effect,
        adjust_active_effect);
    logger::info("on_adjust_active_effect hook install"sv);
  }

  private:
  static auto adjust_active_effect(RE::ActiveEffect* this_, float power, bool unk) -> void;
  static inline REL::Relocation<decltype(adjust_active_effect)> adjust_active_effect_;
};

struct OnMainUpdate final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hoon on_main_update"sv);
    main_update_ = trampoline.write_call<5>(
        Adresses::on_main_update.address() + Offsets::on_main_update,
        main_update);
    logger::info("on_main_update hook install"sv);
  }

  private:
  static auto main_update(RE::Main* this_, float unk) -> void;
  static inline REL::Relocation<decltype(main_update)> main_update_;
};

struct OnWeaponHit final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook on_weapon_hit"sv);
    weapon_hit_ = trampoline.write_call<5>(
        Adresses::on_weapon_hit.address() + Offsets::on_weapon_hit,
        weapon_hit);
    logger::info("on_weapon_hit hook install"sv);
  }

  private:
  static auto weapon_hit(RE::Actor* target, RE::HitData& hit_data) -> void;
  static inline REL::Relocation<decltype(weapon_hit)> weapon_hit_;
};

struct OnTrapHit final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnTrapHit"sv);
    trap_hit_ = trampoline.write_call<5>(
        Adresses::on_trap_apply_damage.address() + Offsets::on_trap_apply_damage,
        trap_hit);
    logger::info("OnTrapHit hook install"sv);
  }

  private:
  static auto trap_hit(RE::Actor* target, RE::HitData* hit_data) -> void;
  static inline REL::Relocation<decltype(trap_hit)> trap_hit_;
};

struct OnMeleeCollision final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnMeleeCollision"sv);
    melee_collision_ = trampoline.write_call<5>(
        Adresses::on_melee_collision.address() + Offsets::on_melee_collision,
        melee_collision);
    logger::info("OnMeleeCollision hook install"sv);
  }

  private:
  static auto
  melee_collision(RE::Actor* attacker, RE::Actor* victim, RE::Projectile* projectile, char aleft)
      -> void;
  static inline REL::Relocation<decltype(melee_collision)> melee_collision_;
};

struct OnHealthMagickaStaminaRegeneration final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("Start install"sv);
    restore_stamina_ = trampoline.write_call<5>(
        Adresses::on_regeneration_stamina.address() + Offsets::on_regeneration_stamina,
        restore_stamina);
    restore_health_ = trampoline.write_call<5>(
        Adresses::on_regeneration_health.address() + Offsets::on_regeneration_health,
        restore_health);
    restore_magicka_ = trampoline.write_call<5>(
        Adresses::on_regeneration_magicka.address() + Offsets::on_regeneration_magicka,
        restore_magicka);
    logger::info("Finish install"sv);
  }

  private:
  static auto restore_stamina(RE::Actor* actor, RE::ActorValue av, float value) -> void;
  static auto restore_health(RE::Actor* actor, RE::ActorValue av, float value) -> void;
  static auto restore_magicka(RE::Actor* actor, RE::ActorValue av, float value) -> void;

  static inline REL::Relocation<decltype(restore_stamina)> restore_stamina_;
  static inline REL::Relocation<decltype(restore_health)> restore_health_;
  static inline REL::Relocation<decltype(restore_magicka)> restore_magicka_;
};

struct OnCheckResistance final
{
  public:
  static auto install_hook() -> void
  {
    logger::info("start hook OnCheckResistance"sv);
    check_resistance_npc_ = Adresses::on_check_resistance_npc.write_vfunc(
        Offsets::on_check_resistance_npc,
        check_resistance_npc);
    check_resistance_pc_ = Adresses::on_check_resistance_pc.write_vfunc(
        Offsets::on_check_resistance_pc,
        check_resistance_pc);
    logger::info("finish hook OnCheckResistance"sv);
  }

  private:
  static auto check_resistance_npc(
      RE::MagicTarget* this_,
      RE::MagicItem* magic_item,
      RE::Effect* effect,
      RE::TESBoundObject* bound_object) -> float;
  static auto check_resistance_pc(
      RE::MagicTarget* this_,
      RE::MagicItem* magic_item,
      RE::Effect* effect,
      RE::TESBoundObject* bound_object) -> float;

  static inline REL::Relocation<decltype(check_resistance_npc)> check_resistance_npc_;
  static inline REL::Relocation<decltype(check_resistance_pc)> check_resistance_pc_;
};

struct OnEnchIgnoresResistance final
{
  public:
  static auto install_hook() -> void
  {
    logger::info("start hook OnEnchIgnoresResistance"sv);
    ignores_resistance_ = Adresses::on_ench_ignores_resistance.write_vfunc(
        Offsets::on_ench_ignores_resistance,
        ignores_resistance);
    logger::info("finish hook OnEnchIgnoresResistance"sv);
  }

  private:
  static auto ignores_resistance(RE::MagicItem* this_) -> bool;

  static inline REL::Relocation<decltype(ignores_resistance)> ignores_resistance_;
};

struct OnEnchGetNoAbsorb final
{
  public:
  static auto install_hook() -> void
  {
    logger::info("start hook OnEnchGetNoAbsorb"sv);
    get_no_absorb_ =
        Adresses::on_ench_get_no_absorb.write_vfunc(Offsets::on_ench_get_no_absorb, get_no_absorb);
    logger::info("finish hook OnEnchGetNoAbsorb"sv);
  }

  private:
  static auto get_no_absorb(RE::MagicItem* this_) -> bool;

  static inline REL::Relocation<decltype(get_no_absorb)> get_no_absorb_;
};

struct OnActorValueOwner final
{
  public:
  static auto install_hook() -> void
  {
    logger::info("start hook OnActorValueOwner"sv);
    get_actor_value_npc_ = Adresses::on_value_owner_get_actor_value_npc.write_vfunc(
        Offsets::on_value_owner_get_actor_value_npc,
        get_actor_value_npc);
    set_actor_value_npc_ = Adresses::on_value_owner_set_actor_value_npc.write_vfunc(
        Offsets::on_value_owner_set_actor_value_npc,
        set_actor_value_npc);
    mod_actor_value_npc_ = Adresses::on_value_owner_mod_actor_value_npc.write_vfunc(
        Offsets::on_value_owner_mod_actor_value_npc,
        mod_actor_value_npc);
    get_actor_value_pc_ = Adresses::on_value_owner_get_actor_value_pc.write_vfunc(
        Offsets::on_value_owner_get_actor_value_pc,
        get_actor_value_pc);
    set_actor_value_pc_ = Adresses::on_value_owner_set_actor_value_pc.write_vfunc(
        Offsets::on_value_owner_set_actor_value_pc,
        set_actor_value_pc);
    mod_actor_value_pc_ = Adresses::on_value_owner_mod_actor_value_pc.write_vfunc(
        Offsets::on_value_owner_mod_actor_value_pc,
        mod_actor_value_pc);
    logger::info("finish hook OnActorValueOwner"sv);
  }

  private:
  static auto get_actor_value_npc(RE::ActorValueOwner* this_, RE::ActorValue av) -> float;
  static auto set_actor_value_npc(RE::ActorValueOwner* this_, RE::ActorValue av, float value)
      -> void;
  static auto mod_actor_value_npc(RE::ActorValueOwner* this_, RE::ActorValue av, float value)
      -> void;

  static auto get_actor_value_pc(RE::ActorValueOwner* this_, RE::ActorValue av) -> float;
  static auto set_actor_value_pc(RE::ActorValueOwner* this_, RE::ActorValue av, float value)
      -> void;
  static auto mod_actor_value_pc(RE::ActorValueOwner* this_, RE::ActorValue av, float value)
      -> void;

  static inline REL::Relocation<decltype(get_actor_value_npc)> get_actor_value_npc_;
  static inline REL::Relocation<decltype(set_actor_value_npc)> set_actor_value_npc_;
  static inline REL::Relocation<decltype(mod_actor_value_npc)> mod_actor_value_npc_;

  static inline REL::Relocation<decltype(get_actor_value_pc)> get_actor_value_pc_;
  static inline REL::Relocation<decltype(set_actor_value_pc)> set_actor_value_pc_;
  static inline REL::Relocation<decltype(mod_actor_value_pc)> mod_actor_value_pc_;
};

auto install_hooks() -> void;
} // namespace Hooks