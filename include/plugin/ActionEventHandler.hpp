#pragma once

namespace Reflyem {
struct ActionEventHandler final : public RE::BSTEventSink<SKSE::ActionEvent> {
public:
  [[nodiscard]] static ActionEventHandler* get_singleton() noexcept;

  auto ProcessEvent(const SKSE::ActionEvent* event, RE::BSTEventSource<SKSE::ActionEvent>* event_source)
      -> RE::BSEventNotifyControl override;
};
} // namespace Reflyem