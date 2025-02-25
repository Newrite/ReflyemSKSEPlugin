#pragma once

namespace Reflyem::ProjectileBlock
{
enum class ProjectileType
{
  kArrow,
  kMissile,
  kCone,
  kFlame,
  kBeam
};
auto should_ignore_hit(
    RE::Projectile* projectile,
    RE::hkpAllCdPointCollector* collidable_collector,
    const ProjectileType projectile_type) -> bool;

auto update(const float delta) -> void;

auto on_magic_hit(RE::MagicCaster* magic_caster,
                                 RE::NiPoint3* ni_point3,
                                 RE::Projectile* projectile,
                                 RE::TESObjectREFR* refr_target,
                                 float a5,
                                 float a6,
                                 bool a7,
                                 bool a8) -> bool;
}
