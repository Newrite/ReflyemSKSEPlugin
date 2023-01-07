#pragma once

namespace addresses
{
  constexpr auto on_weapon_hit = 37673;
  constexpr auto on_main_update = 35551;
  constexpr auto on_adjust_active_effect = 33763;
}

namespace offsets
{
  constexpr auto on_weapon_hit = 0x3C0;
  constexpr auto on_main_update = 0x11f;
  constexpr auto on_adjust_active_effect = 0x4A3;
}

namespace hooks
{

  struct on_adjust_active_effect
  {
  public:
    static auto install_hook(SKSE::Trampoline& trampoline) -> void
    {
      logger::info("start hook on_adjust_active_effect");
      REL::Relocation<uintptr_t> hook{ RELOCATION_ID(addresses::on_adjust_active_effect, 0) };
      _adjust_active_effect = trampoline.write_call<5>(hook.address() + RELOCATION_OFFSET(offsets::on_adjust_active_effect, 0), adjust_active_effect);
      logger::info("on_adjust_active_effect hook install");
    }

  private:
    static auto adjust_active_effect(RE::ActiveEffect* a_this, float a_power, bool a_arg3) -> void;
    static inline REL::Relocation<decltype(adjust_active_effect)> _adjust_active_effect;
  };

  struct on_main_update
  {
  public:
    static auto install_hook(SKSE::Trampoline& trampoline) -> void
    {
      logger::info("start hoon on_main_update");
      REL::Relocation<uintptr_t> hook{ RELOCATION_ID(addresses::on_main_update, 0) };
      _main_update = trampoline.write_call<5>(hook.address() + RELOCATION_OFFSET(offsets::on_main_update, 0), main_update);
      logger::info("on_main_update hook install");
    }

  private:
    static auto main_update(RE::Main* a_this, float a2) -> void;
    static inline REL::Relocation<decltype(main_update)> _main_update;
  };

  struct on_weapon_hit
  {
  public:
    static auto install_hook(SKSE::Trampoline& trampoline) -> void
    {
      logger::info("start hook on_weapon_hit");
      REL::Relocation<uintptr_t> hook{ RELOCATION_ID(addresses::on_weapon_hit, 0) };
      _weapon_hit = trampoline.write_call<5>(hook.address() + RELOCATION_OFFSET(offsets::on_weapon_hit, 0), weapon_hit);
      logger::info("on_weapon_hit hook install");
    }

  private:
    static auto weapon_hit(RE::Actor* target, RE::HitData& hit_data) -> void;
    static inline REL::Relocation<decltype(weapon_hit)> _weapon_hit;
  };

  auto install_hooks() -> void;
}