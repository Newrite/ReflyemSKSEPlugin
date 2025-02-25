#pragma once

namespace Reflyem::CastOnMagicProjectileGetHit
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
