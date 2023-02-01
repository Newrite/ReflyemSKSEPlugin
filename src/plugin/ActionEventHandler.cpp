#include "plugin/ActionEventHandler.hpp"
#include "plugin/CasterAdditions.hpp"

#include <latch>

namespace Reflyem {

[[nodiscard]] auto ActionEventHandler::get_singleton() noexcept -> ActionEventHandler* {
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

auto ActionEventHandler::ProcessEvent(const SKSE::ActionEvent*               event,
                                      RE::BSTEventSource<SKSE::ActionEvent>* event_source)
    -> RE::BSEventNotifyControl {
  if (event && event_source) {
    const auto& config = Config::get_singleton();
    if (config.caster_additions().stamina_cost()) {
      CasterAdditions::action_event_handler(*event, *event_source);
    }
  }
  return RE::BSEventNotifyControl::kContinue;
}
} // namespace Reflyem
