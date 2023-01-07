#pragma once

namespace addresses
{
  constexpr auto on_weapon_hit = 37673;
}

namespace offsets
{
  constexpr auto on_weapon_hit = 0x3C0;
}

namespace hooks
{
  class on_weapon_hit
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