#pragma once

namespace Adresses
{
static inline REL::Relocation<uintptr_t> on_weapon_hit{RELOCATION_ID(37673, 0)};
static inline REL::Relocation<uintptr_t> on_arrow_call_hit{RELOCATION_ID(43027, 0)};
static inline REL::Relocation<uintptr_t> on_is_blocked{RELOCATION_ID(42842, 0)};
static inline REL::Relocation<uintptr_t> on_fill_actor_caster_enchant_data{RELOCATION_ID(33370, 0)};
static inline REL::Relocation<uintptr_t> on_container_menu{RELOCATION_ID(268222, 0)};
static inline REL::Relocation<uintptr_t> on_equip_manager_equip_spell{RELOCATION_ID(37939, 0)};
static inline REL::Relocation<uintptr_t> on_spell_equip{RELOCATION_ID(37965, 0)};
static inline REL::Relocation<uintptr_t> on_standard_item_data{RELOCATION_ID(267990, 0)};
static inline REL::Relocation<uintptr_t> on_magic_item_data{RELOCATION_ID(269320, 0)};
static inline REL::Relocation<uintptr_t> on_some_menu_call_equip_slot_01{RELOCATION_ID(50650, 0)};
static inline REL::Relocation<uintptr_t> on_some_menu_call_equip_slot_02{RELOCATION_ID(50657, 0)};
static inline REL::Relocation<uintptr_t> on_some_menu_call_equip_slot_03{RELOCATION_ID(50659, 0)};
static inline REL::Relocation<uintptr_t> on_spell_equip_menu{RELOCATION_ID(51164, 0)};
static inline REL::Relocation<uintptr_t> on_spell_equip_menu_favorite{RELOCATION_ID(50654, 0)};
static inline REL::Relocation<uintptr_t> on_some_equip{RELOCATION_ID(37938, 0)};
static inline REL::Relocation<uintptr_t> on_set_spell_impl{RELOCATION_ID(257613, 0)};
// static inline REL::Relocation<uintptr_t> on_init_hit_data{RELOCATION_ID(37673, 0)};
static inline REL::Relocation<uintptr_t> on_apply_spells_from_attack{RELOCATION_ID(37673, 0)};
static inline REL::Relocation<uintptr_t> on_regeneration_health{RELOCATION_ID(37509, 0)};
static inline REL::Relocation<uintptr_t> on_regeneration_stamina{RELOCATION_ID(37510, 0)};
static inline REL::Relocation<uintptr_t> on_regeneration_stamina_calculate_const_delta{
    RELOCATION_ID(40356, 0)};
static inline REL::Relocation<uintptr_t> on_regeneration_stamina_calculate{RELOCATION_ID(37831, 0)};
static inline REL::Relocation<uintptr_t> on_regeneration_magicka{RELOCATION_ID(37511, 0)};
static inline REL::Relocation<uintptr_t> on_calculate_sprint_cost{RELOCATION_ID(36994, 0)};
static inline REL::Relocation<uintptr_t> on_check_mount_in_encumbered{RELOCATION_ID(36457, 0)};
static inline REL::Relocation<uintptr_t> on_regeneration_permanent_value{RELOCATION_ID(37515, 0)};
static inline REL::Relocation<uintptr_t> on_actor_value_for_cost_during_cast{
    RELOCATION_ID(33362, 0)};
static inline REL::Relocation<uintptr_t> on_actor_value_for_cost_check_cast{
    RELOCATION_ID(33364, 0)};
static inline REL::Relocation<uintptr_t> on_actor_value_for_cost_restore_value{
    RELOCATION_ID(33359, 0)};
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
static inline REL::Relocation<uintptr_t> on_bound_equip_change{RELOCATION_ID(257628, 0)};
static inline REL::Relocation<uintptr_t> on_actor_update_npc{RELOCATION_ID(261397, 0)};
static inline REL::Relocation<uintptr_t> on_actor_add_object_pc{RELOCATION_ID(261916, 0)};
static inline REL::Relocation<uintptr_t> on_actor_pickup_object_pc{RELOCATION_ID(261916, 0)};
static inline REL::Relocation<uintptr_t> on_check_resistance_npc{RELOCATION_ID(261401, 0)};
static inline REL::Relocation<uintptr_t> on_check_resistance_pc{RELOCATION_ID(261920, 0)};
static inline REL::Relocation<uintptr_t> on_drink_potion_pc{RELOCATION_ID(261916, 0)};
static inline REL::Relocation<uintptr_t> on_set_current_spell_pc{RELOCATION_ID(37813, 0)};
static inline REL::Relocation<uintptr_t> on_set_current_spell_block{RELOCATION_ID(41338, 0)};
static inline REL::Relocation<uintptr_t> on_set_current_spell_FUN{RELOCATION_ID(48037, 0)};
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

// FUN_1408793d0+155
// void __fastcall ActorMagicCaster::sub_140542DD0(RE::ActorMagicCaster *caster,
// RE::MagicSystem::CannotCastReason reasonCannotCast)

namespace Offsets
{
static inline auto on_weapon_hit = RELOCATION_OFFSET(0x3C0, 0);
static inline auto on_arrow_call_hit = RELOCATION_OFFSET(0x90, 0);
static inline auto on_is_blocked = RELOCATION_OFFSET(0x2EA, 0);
static inline auto on_container_menu_process_message = RELOCATION_OFFSET(0x04, 0);
static inline auto on_container_menu_accept = RELOCATION_OFFSET(0x01, 0);
static inline auto on_fill_actor_caster_enchant_data = RELOCATION_OFFSET(0x6E, 0);
static inline auto on_equip_manager_equip_spell = RELOCATION_OFFSET(0x47, 0);
static inline auto on_standard_item_data_equip_state = RELOCATION_OFFSET(0x03, 0);
static inline auto on_some_menu_call_equip_slot_01 = RELOCATION_OFFSET(0x1DC, 0);
static inline auto on_some_menu_call_equip_slot_02 = RELOCATION_OFFSET(0x16B, 0);
static inline auto on_some_menu_call_equip_slot_03 = RELOCATION_OFFSET(0x161, 0);
static inline auto on_magic_item_data_equip_state = RELOCATION_OFFSET(0x03, 0);
static inline auto on_spell_equip_left = RELOCATION_OFFSET(0x4C, 0);
static inline auto on_spell_equip_right = RELOCATION_OFFSET(0x6A, 0);
static inline auto on_spell_equip_voice = RELOCATION_OFFSET(0x93, 0);
static inline auto on_spell_equip_menu = RELOCATION_OFFSET(0xFF, 0);
static inline auto on_spell_equip_menu_favorite = RELOCATION_OFFSET(0x155, 0);
static inline auto on_some_equip = RELOCATION_OFFSET(0xE5, 0);
static inline auto on_some_equip_default_slot = RELOCATION_OFFSET(0x7F, 0);
static inline auto on_set_spell_impl = RELOCATION_OFFSET(0x11, 0);
// static inline auto on_init_hit_data                             = RELOCATION_OFFSET(0x18F, 0);
static inline auto on_apply_spells_from_attack = RELOCATION_OFFSET(0x185, 0);
static inline auto on_regeneration_health = RELOCATION_OFFSET(0x68, 0);
static inline auto on_regeneration_stamina = RELOCATION_OFFSET(0x176, 0);
static inline auto on_regeneration_stamina_calculate_const_delta = RELOCATION_OFFSET(0x163, 0);
static inline auto on_regeneration_stamina_calculate = RELOCATION_OFFSET(0x18E, 0);
static inline auto on_regeneration_magicka = RELOCATION_OFFSET(0x68, 0);
static inline auto on_calculate_sprint_cost = RELOCATION_OFFSET(0xC9, 0);
static inline auto on_check_mount_in_encumbered = RELOCATION_OFFSET(0x1E, 0);
static inline auto on_calculate_regeneration_health_magicka = RELOCATION_OFFSET(0x4C, 0);
static inline auto on_regeneration_permanent_value = RELOCATION_OFFSET(0x1AF, 0);
static inline auto on_actor_value_for_cost_during_cast = RELOCATION_OFFSET(0x151, 0);
static inline auto on_actor_value_for_cost_check_cast = RELOCATION_OFFSET(0xC1, 0);
static inline auto on_actor_value_for_cost_restore_value = RELOCATION_OFFSET(0x4C, 0);
static inline auto on_cost_for_cast_conc_during_cast = RELOCATION_OFFSET(0x1C4, 0);
static inline auto on_cost_for_cast_ff_during_cast = RELOCATION_OFFSET(0x1FB, 0);
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
static inline auto on_bound_equip_change = RELOCATION_OFFSET(0x01, 0);
static inline auto on_actor_update_npc = RELOCATION_OFFSET(0xAD, 0);
static inline auto on_actor_add_object_pc = RELOCATION_OFFSET(0x5A, 0);
static inline auto on_actor_pickup_object_pc = RELOCATION_OFFSET(0xCC, 0);
static inline auto on_check_resistance_npc = RELOCATION_OFFSET(0x0A, 0);
static inline auto on_check_resistance_pc = RELOCATION_OFFSET(0x0A, 0);
static inline auto on_drink_potion_pc = RELOCATION_OFFSET(0x10F, 0);
static inline auto on_set_current_spell_pc = RELOCATION_OFFSET(0x74, 0);
static inline auto on_set_current_spell_block = RELOCATION_OFFSET(0xA8, 0);
static inline auto on_set_current_spell_FUN = RELOCATION_OFFSET(0x15E, 0);
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

struct OnActorUpdateEquipBound final
{
  public:
  static auto install_hook() -> void
  {
    logger::info("start hook OnActorUpdateEquipBound"sv);
    process_event_ =
        Adresses::on_bound_equip_change.write_vfunc(Offsets::on_bound_equip_change, process_event);
    logger::info("finish hook OnActorUpdateEquipBound"sv);
  }

  private:
  static auto process_event(
      const RE::ActorInventoryEvent* event,
      RE::BSTEventSource<RE::ActorInventoryEvent>* event_source) -> RE::BSEventNotifyControl;

  static inline REL::Relocation<decltype(process_event)> process_event_;
};

struct OnContainerMenu final
{
public:
    static auto install_hook() -> void
    {
        logger::info("start hook OnContainerMenu"sv);
        process_message_ =
            Adresses::on_container_menu.write_vfunc(Offsets::on_container_menu_process_message, process_message);
        accept_ =
            Adresses::on_container_menu.write_vfunc(Offsets::on_container_menu_accept, accept);
        logger::info("finish hook OnContainerMenu"sv);
    }

private:
    static auto process_message(RE::ContainerMenu* this_, RE::UIMessage& message) -> RE::UI_MESSAGE_RESULTS;
    static auto accept(RE::ContainerMenu* this_, RE::CallbackProcessor* processor) -> void;

    static inline REL::Relocation<decltype(process_message)> process_message_;
    static inline REL::Relocation<decltype(accept)> accept_;
};

struct OnStandardItemData final
{
public:
    static auto install_hook() -> void
    {
        logger::info("start hook OnStandardItemData"sv);
        get_equip_state_ =
            Adresses::on_standard_item_data.write_vfunc(Offsets::on_standard_item_data_equip_state, get_equip_state);
        logger::info("finish hook OnStandardItemData"sv);
    }

private:
    static auto get_equip_state(
        RE::StandardItemData* this_) -> std::uint32_t;

    static inline REL::Relocation<decltype(get_equip_state)> get_equip_state_;
};

struct OnMagicItemData final
{
public:
    static auto install_hook() -> void
    {
        logger::info("start hook OnMagicItemData"sv);
        get_equip_state_ =
            Adresses::on_magic_item_data.write_vfunc(Offsets::on_magic_item_data_equip_state, get_equip_state);
        logger::info("finish hook OnMagicItemData"sv);
    }

private:
    static auto get_equip_state(
        RE::StandardItemData* this_) -> std::uint32_t;

    static inline REL::Relocation<decltype(get_equip_state)> get_equip_state_;
};

struct OnActorMagicCaster final
{
  public:
  static auto install_hook() -> void
  {
    logger::info("start hook OnActorMagicCaster"sv);
    on_set_spell_impl_ =
        Adresses::on_set_spell_impl.write_vfunc(Offsets::on_set_spell_impl, on_set_spell_impl);
    logger::info("finish hook OnActorMagicCaster"sv);
  }

  private:
  static auto on_set_spell_impl(RE::ActorMagicCaster* caster, RE::MagicItem* magic_item) -> void;
  static inline REL::Relocation<decltype(on_set_spell_impl)> on_set_spell_impl_;
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

struct OnDrinkPotion final
{
  public:
  static auto install_hook() -> void
  {
    logger::info("start hook OnActorDrinkPotion"sv);
    drink_potion_ =
        Adresses::on_drink_potion_pc.write_vfunc(Offsets::on_drink_potion_pc, drink_potion);
    logger::info("finish hook OnActorDrinkPotion"sv);
  }

  private:
  static auto drink_potion(
      RE::Actor* this_,
      RE::AlchemyItem* potion,
      RE::ExtraDataList* extra_data_list) -> bool;

  static inline REL::Relocation<decltype(drink_potion)> drink_potion_;
};

struct OnSetCurrentSpell final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnSetCurrentSpell"sv);
    set_spell_impl_pc_ = trampoline.write_call<5>(
        Adresses::on_set_current_spell_pc.address() + Offsets::on_set_current_spell_pc,
        set_spell_impl_pc);
    set_spell_impl_block_ = trampoline.write_call<5>(
        Adresses::on_set_current_spell_block.address() + Offsets::on_set_current_spell_block,
        set_spell_impl_block);
    set_spell_impl_FUN_ = trampoline.write_call<5>(
        Adresses::on_set_current_spell_FUN.address() + Offsets::on_set_current_spell_FUN,
        set_spell_impl_fun);
    logger::info("finish hook OnSetCurrentSpell"sv);
  }

  private:
  static auto set_spell_impl_pc(
      RE::ActorMagicCaster* caster,
      RE::MagicItem* magic_item,
      RE::TESObjectREFR* ref,
      bool a4) -> bool;
  static auto set_spell_impl_block(
      RE::ActorMagicCaster* caster,
      RE::MagicItem* magic_item,
      RE::TESObjectREFR* ref,
      bool a4) -> bool;
  static auto set_spell_impl_fun(
      RE::ActorMagicCaster* caster,
      RE::MagicItem* magic_item,
      RE::TESObjectREFR* ref,
      bool a4) -> bool;

  static inline REL::Relocation<decltype(set_spell_impl_pc)> set_spell_impl_pc_;
  static inline REL::Relocation<decltype(set_spell_impl_block)> set_spell_impl_block_;
  static inline REL::Relocation<decltype(set_spell_impl_fun)> set_spell_impl_FUN_;
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

struct OnSomeMenuEquipState final
{
public:
    static auto install_hook(SKSE::Trampoline& trampoline) -> void
    {
        logger::info("start hook OnSomeMenuEquipState"sv);
        get_equip_state_01_ = trampoline.write_call<5>(
            Adresses::on_some_menu_call_equip_slot_01.address() + Offsets::on_some_menu_call_equip_slot_01,
            get_equip_state_01);
        get_equip_state_02_ = trampoline.write_call<5>(
            Adresses::on_some_menu_call_equip_slot_02.address() + Offsets::on_some_menu_call_equip_slot_02,
            get_equip_state_01);
        get_equip_state_03_ = trampoline.write_call<5>(
            Adresses::on_some_menu_call_equip_slot_03.address() + Offsets::on_some_menu_call_equip_slot_03,
            get_equip_state_01);
        logger::info("finish hook OnSomeMenuEquipState"sv);
    }

private:
    static auto get_equip_state_01(RE::Actor* actor, RE::TESBoundObject* object, void* a3, void* a4)
        -> std::uint32_t;
    static auto get_equip_state_02(RE::Actor* actor, RE::TESBoundObject* object, void* a3, void* a4)
        -> std::uint32_t;
    static auto get_equip_state_03(RE::Actor* actor, RE::TESBoundObject* object, void* a3, void* a4)
        -> std::uint32_t;
    static inline REL::Relocation<decltype(get_equip_state_01)> get_equip_state_01_;
    static inline REL::Relocation<decltype(get_equip_state_02)> get_equip_state_02_;
    static inline REL::Relocation<decltype(get_equip_state_03)> get_equip_state_03_;
};

struct OnSpellEquip final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnSpellEquip"sv);
    equip_spell_left_ = trampoline.write_call<5>(
        Adresses::on_spell_equip.address() + Offsets::on_spell_equip_left,
        equip_spell_left);
    equip_spell_right_ = trampoline.write_call<5>(
        Adresses::on_spell_equip.address() + Offsets::on_spell_equip_right,
        equip_spell_right);
    logger::info("finish hook OnSpellEquip"sv);
  }

  private:
  static auto equip_spell_left(
      RE::Actor* actor,
      RE::Actor::SlotTypes slot_type,
      RE::MagicItem* magic_item) -> void*;
  static auto equip_spell_right(
      RE::Actor* actor,
      RE::Actor::SlotTypes slot_type,
      RE::MagicItem* magic_item) -> void*;
  static auto equip_spell_voice(RE::Actor* actor, RE::MagicItem* magic_item) -> void*;
  static inline REL::Relocation<decltype(equip_spell_left)> equip_spell_left_;
  static inline REL::Relocation<decltype(equip_spell_right)> equip_spell_right_;
  static inline REL::Relocation<decltype(equip_spell_voice)> equip_spell_voice_;
};

struct OnSomeEquip final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnSomeEquip"sv);
    some_equip_ = trampoline.write_call<5>(
        Adresses::on_some_equip.address() + Offsets::on_some_equip,
        some_equip);
    some_equip_default_equip_ = trampoline.write_call<5>(
        Adresses::on_some_equip.address() + Offsets::on_some_equip_default_slot,
        some_equip_default_equip);
    logger::info("finish hook OnSomeEquip"sv);
  }

  private:
  static auto some_equip(
      RE::ActorEquipManager* equip_manager,
      RE::Actor* actor,
      RE::TESBoundObject* bound_object,
      void* extra_data_list) -> void;
  static auto some_equip_default_equip(
      RE::ActorEquipManager* equip_manager,
      RE::Actor* actor,
      RE::TESBoundObject* bound_object) -> RE::BGSEquipSlot*;
  static inline REL::Relocation<decltype(some_equip)> some_equip_;
  static inline REL::Relocation<decltype(some_equip_default_equip)> some_equip_default_equip_;
};

struct OnEquipManagerEquipSpell final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnEquipManagerEquipSpell"sv);
    equip_spell_ = trampoline.write_call<5>(
        Adresses::on_equip_manager_equip_spell.address() + Offsets::on_equip_manager_equip_spell,
        equip_spell);
    logger::info("finish hook OnEquipManagerEquipSpell"sv);
  }

  private:
  static auto equip_spell(
      RE::ActorEquipManager* equip_manager,
      RE::Actor* actor,
      RE::TESBoundObject* bound_object,
      RE::BGSEquipSlot* equip_slot) -> void*;
  static inline REL::Relocation<decltype(equip_spell)> equip_spell_;
};

struct OnFillCasterEnchantData final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnFillCasterEnchantData"sv);
    fill_caster_enchant_data_ = trampoline.write_call<5>(
        Adresses::on_fill_actor_caster_enchant_data.address() +
            Offsets::on_fill_actor_caster_enchant_data,
        fill_caster_enchant_data);
    logger::info("finish hook OnFillCasterEnchantData"sv);
  }

  private:
  static auto fill_caster_enchant_data(RE::ActorMagicCaster* caster) -> void;
  static inline REL::Relocation<decltype(fill_caster_enchant_data)> fill_caster_enchant_data_;
};

struct OnEquipSpellMenu final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnEquipSpellMenu"sv);
    equip_spell_menu_ = trampoline.write_call<5>(
        Adresses::on_spell_equip_menu.address() + Offsets::on_spell_equip_menu,
        equip_spell_menu);
    equip_spell_menu_favorite_ = trampoline.write_call<5>(
        Adresses::on_spell_equip_menu_favorite.address() + Offsets::on_spell_equip_menu_favorite,
        equip_spell_menu_favorite);
    logger::info("finish hook OnEquipSpellMenu"sv);
  }

  private:
  static auto equip_spell_menu(
      RE::ActorEquipManager* equip_manager,
      RE::Actor* actor,
      RE::TESBoundObject* bound_object,
      RE::BGSEquipSlot* equip_slot) -> RE::BGSEquipSlot*;
  static auto equip_spell_menu_favorite(
      RE::ActorEquipManager* equip_manager,
      RE::Actor* actor,
      RE::TESBoundObject* bound_object,
      RE::BGSEquipSlot* equip_slot) -> RE::BGSEquipSlot*;
  static inline REL::Relocation<decltype(equip_spell_menu)> equip_spell_menu_;
  static inline REL::Relocation<decltype(equip_spell_menu_favorite)> equip_spell_menu_favorite_;
};

struct OnActorIsOverEncumbered final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnActorIsOverEncumbered"sv);
    is_actor_on_mount_ = trampoline.write_call<5>(
        Adresses::on_check_mount_in_encumbered.address() + Offsets::on_check_mount_in_encumbered,
        is_actor_on_mount);
    logger::info("finish hook OnActorIsOverEncumbered"sv);
  }

  private:
  static auto is_actor_on_mount(RE::Actor* actor) -> bool;
  static inline REL::Relocation<decltype(is_actor_on_mount)> is_actor_on_mount_;
};

struct OnCalculatePlayerSprintCost final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start OnCalculatePlayerSprintCost"sv);
    calculate_sprint_cost_ = trampoline.write_call<5>(
        Adresses::on_calculate_sprint_cost.address() + Offsets::on_calculate_sprint_cost,
        calculate_sprint_cost);
    logger::info("finish OnCalculatePlayerSprintCost"sv);
  }

  private:
  static auto calculate_sprint_cost(float player_stamina, float maybe_delta) -> float;
  static inline REL::Relocation<decltype(calculate_sprint_cost)> calculate_sprint_cost_;
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

struct OnApplySpellsFromAttack final
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

struct OnRegenerationPermanentValue final
{
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnRegenerationPermanentValue"sv);
    calculate_regeneration_value_health_ = trampoline.write_call<5>(
        Adresses::on_regeneration_health.address() +
            Offsets::on_calculate_regeneration_health_magicka,
        calculate_regeneration_value_health);
    calculate_regeneration_value_magicka_ = trampoline.write_call<5>(
        Adresses::on_regeneration_magicka.address() +
            Offsets::on_calculate_regeneration_health_magicka,
        calculate_regeneration_value_magicka);
    calculate_regeneration_value_stamina_const_delta_ = trampoline.write_call<5>(
        Adresses::on_regeneration_stamina_calculate_const_delta.address() +
            Offsets::on_regeneration_stamina_calculate_const_delta,
        calculate_regeneration_value_stamina_const_delta);
    calculate_regeneration_value_stamina_ = trampoline.write_call<5>(
        Adresses::on_regeneration_stamina_calculate.address() +
            Offsets::on_regeneration_stamina_calculate,
        calculate_regeneration_value_stamina);
    logger::info("finish hook OnRegenerationPermanentValue"sv);
  }

  static auto calculate_regeneration_value_health(RE::Character* character, RE::ActorValue av)
      -> float;
  static auto calculate_regeneration_value_magicka(RE::Character* character, RE::ActorValue av)
      -> float;
  static auto calculate_regeneration_value_stamina_const_delta(RE::Actor* actor, float delta)
      -> void;
  static auto calculate_regeneration_value_stamina(RE::Actor* actor, float delta) -> void;
  static inline REL::Relocation<decltype(calculate_regeneration_value_health)>
      calculate_regeneration_value_health_;
  static inline REL::Relocation<decltype(calculate_regeneration_value_magicka)>
      calculate_regeneration_value_magicka_;
  static inline REL::Relocation<decltype(calculate_regeneration_value_stamina_const_delta)>
      calculate_regeneration_value_stamina_const_delta_;
  static inline REL::Relocation<decltype(calculate_regeneration_value_stamina)>
      calculate_regeneration_value_stamina_;
};

struct OnArrowCallHit final
{
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnArrowCallHit"sv);
    arrow_call_hit_ = trampoline.write_call<5>(
        Adresses::on_arrow_call_hit.address() + Offsets::on_arrow_call_hit,
        arrow_call_hit);
    logger::info("finish hook OnArrowCallHit"sv);
  }

  static auto arrow_call_hit(
      RE::Character* attacker,
      RE::Actor* target,
      RE::Projectile* projectile,
      bool is_left) -> void;
  static inline REL::Relocation<decltype(arrow_call_hit)> arrow_call_hit_;
};

struct OnAttackIsBlocked final
{
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnAttackIsBlocked"sv);
    is_blocked_ = trampoline.write_call<5>(
        Adresses::on_is_blocked.address() + Offsets::on_is_blocked,
        is_blocked);
    logger::info("finish hook OnAttackIsBlocked"sv);
  }

  static auto is_blocked(
      RE::Actor* target,
      RE::Actor* attacker,
      float* attacker_location_x,
      float* target_location_x,
      void* arg5,
      float arg6,
      float* arg7,
      char arg8) -> bool;
  static inline REL::Relocation<decltype(is_blocked)> is_blocked_;
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

struct OnCastActorValue final
{
  public:
  static auto install_hook(SKSE::Trampoline& trampoline) -> void
  {
    logger::info("start hook OnCastActorValue"sv);
    actor_value_for_cost_check_cast_ = trampoline.write_call<5>(
        Adresses::on_actor_value_for_cost_check_cast.address() +
            Offsets::on_actor_value_for_cost_check_cast,
        actor_value_for_cost_check_cast);
    actor_value_for_cost_during_cast_ = trampoline.write_call<5>(
        Adresses::on_actor_value_for_cost_during_cast.address() +
            Offsets::on_actor_value_for_cost_during_cast,
        actor_value_for_cost_during_cast);
    actor_value_for_cost_restore_value_ = trampoline.write_call<5>(
        Adresses::on_actor_value_for_cost_restore_value.address() +
            Offsets::on_actor_value_for_cost_restore_value,
        actor_value_for_cost_restore_value);
    logger::info("OnCastActorValue hook install"sv);
  }

  private:
  static auto actor_value_for_cost_check_cast(
      RE::MagicItem* magic_item,
      RE::MagicSystem::CastingSource cast_source) -> RE::ActorValue;
  static auto actor_value_for_cost_during_cast(
      RE::MagicItem* magic_item,
      RE::MagicSystem::CastingSource cast_source) -> RE::ActorValue;
  static auto actor_value_for_cost_restore_value(
      RE::MagicItem* magic_item,
      RE::MagicSystem::CastingSource cast_source) -> RE::ActorValue;
  static inline REL::Relocation<decltype(actor_value_for_cost_check_cast)>
      actor_value_for_cost_check_cast_;
  static inline REL::Relocation<decltype(actor_value_for_cost_during_cast)>
      actor_value_for_cost_during_cast_;
  static inline REL::Relocation<decltype(actor_value_for_cost_restore_value)>
      actor_value_for_cost_restore_value_;
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