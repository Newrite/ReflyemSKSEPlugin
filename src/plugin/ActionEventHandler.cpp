#include "ActionEventHandler.h"

namespace Reflyem {

[[nodiscard]] ActionEventHandler *
ActionEventHandler::get_singleton() noexcept {
  static ActionEventHandler instance;

  static std::atomic_bool initialized;
  static std::latch       latch(1);
  if (!initialized.exchange(true)) {
    logger::info("create instance of action_event_handler");
    latch.count_down();
  }
  latch.wait();

  return &instance;
}

auto
ActionEventHandler::ProcessEvent(const SKSE::ActionEvent *event, RE::BSTEventSource<SKSE::ActionEvent> *)
    -> RE::BSEventNotifyControl {
  logger::info("actor level: {}", event->actor->GetLevel());
  logger::info("get action event {}", static_cast<int>(event->type.get()));
  logger::info("slot is {}", static_cast<int>(event->slot.get()));
  return RE::BSEventNotifyControl::kContinue;
}
} // namespace Reflyem