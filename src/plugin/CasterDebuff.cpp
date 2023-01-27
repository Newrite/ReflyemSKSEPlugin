#include "plugin/CasterDebuff.hpp"
#include "Core.hpp"

namespace Reflyem {
namespace CasterDebuff {
auto on_update_actor(RE::Character& character, float, const Config& config) -> void {

  if (Core::is_casting_actor(character)) {
    character.AddSpell(config.caster_debuff_spell);
  } else {
    character.RemoveSpell(config.caster_debuff_spell);
  }
}
} // namespace CasterDebuff
} // namespace Reflyem