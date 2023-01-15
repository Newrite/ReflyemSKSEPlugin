#include "InputEventHandler.h"
#include "AnimationEventHandler.h"
#include "TKDodge.h"

namespace Reflyem {
auto
InputEventHandler::Register() -> void {
  logger::info("Start Registered!");
  auto device_manager = RE::BSInputDeviceManager::GetSingleton();
  device_manager->AddEventSink(InputEventHandler::get_singleton());
  logger::info("Registered {}"sv, typeid(RE::InputEvent).name());
}

auto
InputEventHandler::get_singleton() -> InputEventHandler * {
  static InputEventHandler singleton;
  return std::addressof(singleton);
}

auto
InputEventHandler::ProcessEvent(RE::InputEvent *const *a_event, RE::BSTEventSource<RE::InputEvent *> *a_eventSource)
    -> RE::BSEventNotifyControl {
  auto &config = Reflyem::Config::get_singleton();
  if (config.tk_dodge_enable) {
    return Reflyem::TKDodge::process_event_input_handler(a_event, a_eventSource, config);
  }
  return RE::BSEventNotifyControl::kContinue;
}

} // namespace Reflyem