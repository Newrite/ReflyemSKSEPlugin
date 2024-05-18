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
}