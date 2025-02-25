#pragma once

namespace Reflyem::MagicWeapon
{

namespace Flag
{
enum : int16_t
{
  kNone = 0,
  kDivideMagnitude = 1,
  kFirstEffectOnly = 2,
  kNoDamageDiffMult = 4,
};
}

auto on_weapon_hit(RE::Actor& target, RE::HitData& hit_data) -> void;
}
