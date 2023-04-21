#pragma once

namespace Reflyem
{

struct InputEventHandler final : RE::BSTEventSink<RE::InputEvent*>
{
  private:
  [[nodiscard]] static auto get_singleton() noexcept -> InputEventHandler*;

  public:
  static auto Register() -> void;

  auto ProcessEvent(RE::InputEvent* const* event, RE::BSTEventSource<RE::InputEvent*>* event_source)
      -> RE::BSEventNotifyControl override;
};
} // namespace Reflyem