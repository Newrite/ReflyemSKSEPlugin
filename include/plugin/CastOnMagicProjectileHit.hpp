#pragma once

namespace Reflyem::CastOnMagicProjectileHit
{

auto on_magic_hit(RE::MagicCaster*,
                  RE::NiPoint3*,
                  RE::Projectile*,
                  RE::TESObjectREFR*,
                  float,
                  float,
                  bool,
                  bool) -> void;
}
