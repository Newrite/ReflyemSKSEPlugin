#include "ActionEventHandler.h"

namespace reflyem
{

  [[nodiscard]] action_event_handler* action_event_handler::get_singleton() noexcept
  {
    static action_event_handler instance;

    static std::atomic_bool initialized;
    static std::latch latch(1);
    if (!initialized.exchange(true))
    {
      logger::info("create instance of action_event_handler");
      latch.count_down();
    }
    latch.wait();

    return &instance;
  }

  auto action_event_handler::ProcessEvent(
    const SKSE::ActionEvent* event,
    RE::BSTEventSource<SKSE::ActionEvent>*) -> RE::BSEventNotifyControl
  {
    logger::info("actor level: {}", event->actor->GetLevel());
    logger::info("get action event {}", static_cast<int>(event->type.get()));
    logger::info("slot is {}", static_cast<int>(event->slot.get()));
    return RE::BSEventNotifyControl::kContinue;
  }
}