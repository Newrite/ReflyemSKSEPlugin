#pragma once

#include <latch>

namespace reflyem
{
  struct action_event_handler final : public RE::BSTEventSink<SKSE::ActionEvent>
  {
  public:

    [[nodiscard]] static action_event_handler* get_singleton() noexcept;

    auto ProcessEvent(
      const SKSE::ActionEvent* event,
      RE::BSTEventSource<SKSE::ActionEvent>*) -> RE::BSEventNotifyControl override;
  };
}