#pragma once

namespace Reflyem
{
struct ActionEventHandler final : RE::BSTEventSink<SKSE::ActionEvent>
{
  private:
  [[nodiscard]] static auto get_singleton() noexcept -> ActionEventHandler*;

  public:
  static auto Register() -> void;

  auto ProcessEvent(
      const SKSE::ActionEvent* event,
      RE::BSTEventSource<SKSE::ActionEvent>* event_source) -> RE::BSEventNotifyControl override;
};
} // namespace Reflyem