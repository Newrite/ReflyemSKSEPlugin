#pragma once

namespace Reflyem
{
struct MenuEventHandler final : RE::BSTEventSink<RE::MenuOpenCloseEvent>
{
  private:
  [[nodiscard]] static auto get_singleton() noexcept -> MenuEventHandler*;

  public:
  static auto Register() -> void;

  auto ProcessEvent(
      const RE::MenuOpenCloseEvent* event,
      RE::BSTEventSource<RE::MenuOpenCloseEvent>* event_source)
      -> RE::BSEventNotifyControl override;
};
} // namespace Reflyem