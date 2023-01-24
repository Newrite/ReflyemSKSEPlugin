#include "CasterDebuff.hpp"

namespace Reflyem {
namespace CasterDebuff {
auto on_update_actor(RE::Character& actor, float, const Reflyem::Config& config) -> void {

  if (actor.IsDead()) {
    return;
  }

  auto caster_left  = actor.GetMagicCaster(RE::MagicSystem::CastingSource::kLeftHand);
  auto caster_right = actor.GetMagicCaster(RE::MagicSystem::CastingSource::kRightHand);

  if ((caster_left && caster_left->currentSpell) || (caster_right && caster_right->currentSpell)) {
    actor.AddSpell(config.caster_debuff_spell);
  } else {
    actor.RemoveSpell(config.caster_debuff_spell);
  }
}
} // namespace CasterDebuff
} // namespace Reflyem