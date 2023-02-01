#include "plugin/CasterAdditions.hpp"
#include "Core.hpp"
#include "plugin/ResourceManager.hpp"

namespace Reflyem::CasterAdditions {
auto on_update_actor(RE::Character& character, float, const Config& config) -> void {

  if (Core::is_casting_actor(character)) {
    character.AddSpell(config.caster_additions().spell_add_when_casting());
  } else {
    character.RemoveSpell(config.caster_additions().spell_add_when_casting());
  }
}

auto action_event_handler(const SKSE::ActionEvent& event, RE::BSTEventSource<SKSE::ActionEvent>&)
    -> void {
  if (event.actor) {
    switch (event.type.get()) {
    case SKSE::ActionEvent::Type::kSpellCast: {
      if (event.actor->GetActorValue(RE::ActorValue::kStamina) < 25.f) {
        event.actor->InterruptCast(true);
        Core::flash_hud_meter(RE::ActorValue::kStamina);
      }
      break;
    }
    case SKSE::ActionEvent::Type::kSpellFire: {
      ResourceManager::spend_actor_value(*event.actor, RE::ActorValue::kStamina, 25.f);
      break;
    }
    case SKSE::ActionEvent::Type::kWeaponSwing:
    case SKSE::ActionEvent::Type::kVoiceCast:
    case SKSE::ActionEvent::Type::kVoiceFire:
    case SKSE::ActionEvent::Type::kBowDraw:
    case SKSE::ActionEvent::Type::kBowRelease:
    case SKSE::ActionEvent::Type::kBeginDraw:
    case SKSE::ActionEvent::Type::kEndDraw:
    case SKSE::ActionEvent::Type::kBeginSheathe:
    case SKSE::ActionEvent::Type::kEndSheathe:
      break;
    }
  }
}
} // namespace Reflyem::CasterAdditions
