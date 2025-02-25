#pragma once

namespace Reflyem::CastOnMetaFlags {

enum class Flag
{
  kCasterHasKeyword = 1 << 0,
  kFormHasKeyword = 1 << 1,
  kTargetHasKeyword = 1 << 2,
  kNotConcentrateSpell = 1 << 3,
  kConcentrateFullPower = 1 << 4,
  kSelfIsPowerAttacking = 1 << 5,
  kSelfIsAttacking = 1 << 6,
  kSelfIsCasting = 1 << 7,
  kTargetIsPowerAttacking = 1 << 8,
  kTargetIsAttacking = 1 << 9,
  kTargetIsCasting = 1 << 10,
  kIsDualCast = 1 << 11,
  kCastCostMagic = 1 << 12,
  kCastCostStamina = 1 << 13,
};

using MetaFlags = stl::enumeration<Flag, std::uint32_t>;

static auto a() -> void
{
  auto meta = MetaFlags{Flag::kFormHasKeyword, Flag::kConcentrateFullPower};
}

}
