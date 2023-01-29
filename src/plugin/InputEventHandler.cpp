#include "plugin/InputEventHandler.hpp"
#include "plugin/TKDodge.hpp"

namespace Reflyem {
auto InputEventHandler::Register() -> void {
  logger::info("Start Registered!"sv);
  const auto device_manager = RE::BSInputDeviceManager::GetSingleton();
  device_manager->AddEventSink(get_singleton());
  logger::info("Registered {}"sv, typeid(RE::InputEvent).name());
}

auto InputEventHandler::get_singleton() -> InputEventHandler* {
  static InputEventHandler singleton;
  return std::addressof(singleton);
}

auto InputEventHandler::ProcessEvent(RE::InputEvent* const*               event,
                                     RE::BSTEventSource<RE::InputEvent*>* event_source)
    -> RE::BSEventNotifyControl {
  auto& config = Config::get_singleton();
  if (config.tk_dodge_enable) {
    return TkDodge::process_event_input_handler(event, event_source, config);
  }
  return RE::BSEventNotifyControl::kContinue;
}

} // namespace Reflyem