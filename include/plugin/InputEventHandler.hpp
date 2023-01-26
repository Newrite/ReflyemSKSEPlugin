#pragma once

namespace Reflyem {

class InputEventHandler final : public RE::BSTEventSink<RE::InputEvent*> {
public:
  auto ProcessEvent(RE::InputEvent* const* event, RE::BSTEventSource<RE::InputEvent*>* event_source)
      -> RE::BSEventNotifyControl override;

  static auto Register() -> void;

private:
  static auto get_singleton() -> InputEventHandler*;
};
} // namespace Reflyem