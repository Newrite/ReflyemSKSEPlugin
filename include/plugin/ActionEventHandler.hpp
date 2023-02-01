#pragma once

namespace Reflyem {
struct ActionEventHandler final : public RE::BSTEventSink<SKSE::ActionEvent> {
public:
  [[nodiscard]] static auto get_singleton() noexcept -> ActionEventHandler*;

  auto ProcessEvent(const SKSE::ActionEvent*               event,
                    RE::BSTEventSource<SKSE::ActionEvent>* event_source)
      -> RE::BSEventNotifyControl override;
};
} // namespace Reflyem