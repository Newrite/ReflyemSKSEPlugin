#include "plugin/InputEventHandler.hpp"
#include "plugin/TKDodge.hpp"

namespace Reflyem
{

auto InputEventHandler::get_singleton() noexcept -> InputEventHandler*
{
  static InputEventHandler singleton;
  return std::addressof(singleton);
}

auto InputEventHandler::Register() -> void
{
  const auto device_manager = RE::BSInputDeviceManager::GetSingleton();
  logger::info("Start register input handler"sv);
  if (device_manager)
    {
      device_manager->AddEventSink(get_singleton());
      logger::info("Finish register input handler"sv);
    }
}

auto InputEventHandler::ProcessEvent(
    RE::InputEvent* const* event,
    RE::BSTEventSource<RE::InputEvent*>* event_source) -> RE::BSEventNotifyControl
{
  const auto& config = Config::get_singleton();
  if (config.tk_dodge().enable())
    {
      return TkDodge::process_event_input_handler(event, event_source, config);
    }
  return RE::BSEventNotifyControl::kContinue;
}

} // namespace Reflyem