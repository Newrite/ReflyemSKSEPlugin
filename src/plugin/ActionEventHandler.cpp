#include "plugin/ActionEventHandler.hpp"
#include "plugin/ResourceManager.hpp"
#include "Core.hpp"

#include <latch>

namespace Reflyem {

[[nodiscard]] ActionEventHandler* ActionEventHandler::get_singleton() noexcept {
  static ActionEventHandler instance;

  static std::atomic_bool initialized;
  static std::latch       latch(1);
  if (!initialized.exchange(true)) {
    logger::info("create instance of action_event_handler"sv);
    latch.count_down();
  }
  latch.wait();

  return &instance;
}

auto ActionEventHandler::ProcessEvent(const SKSE::ActionEvent* event,
                                      RE::BSTEventSource<SKSE::ActionEvent>*)
  -> RE::BSEventNotifyControl {
  if (event && event->actor) {
    switch (event->type.get()) {
    case SKSE::ActionEvent::Type::kSpellCast: {
      if (event->actor->GetActorValue(RE::ActorValue::kStamina) < 25.f) {
        event->actor->InterruptCast(true);
        Core::flash_hud_meter(RE::ActorValue::kStamina);
      }
      break;
    }
    case SKSE::ActionEvent::Type::kSpellFire: {
      ResourceManager::spend_actor_value(*event->actor, RE::ActorValue::kStamina, 25.f);
      break;
    }
    case SKSE::ActionEvent::Type::kWeaponSwing:
      break;
    case SKSE::ActionEvent::Type::kVoiceCast:
      break;
    case SKSE::ActionEvent::Type::kVoiceFire:
      break;
    case SKSE::ActionEvent::Type::kBowDraw:
      break;
    case SKSE::ActionEvent::Type::kBowRelease:
      break;
    case SKSE::ActionEvent::Type::kBeginDraw:
      break;
    case SKSE::ActionEvent::Type::kEndDraw:
      break;
    case SKSE::ActionEvent::Type::kBeginSheathe:
      break;
    case SKSE::ActionEvent::Type::kEndSheathe:
      break;
    }
  }
  return RE::BSEventNotifyControl::kContinue;
}
} // namespace Reflyem
