#include "CasterDebuff.hpp"

namespace Reflyem {
namespace CasterDebuff {
auto on_update_actor(RE::Character& character, float, const Config& config) -> void {

  if (character.IsDead()) {
    return;
  }

  const auto caster_left  = character.GetMagicCaster(RE::MagicSystem::CastingSource::kLeftHand);
  const auto caster_right = character.GetMagicCaster(RE::MagicSystem::CastingSource::kRightHand);

  if ((caster_left && caster_left->currentSpell) || (caster_right && caster_right->currentSpell)) {
    character.AddSpell(config.caster_debuff_spell);
  } else {
    character.RemoveSpell(config.caster_debuff_spell);
  }
}
} // namespace CasterDebuff
} // namespace Reflyem