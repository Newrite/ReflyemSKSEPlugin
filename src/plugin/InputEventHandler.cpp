#include "plugin/InputEventHandler.hpp"

#include "Core.hpp"
#include "DeclutteredLoot.hpp"
#include "plugin/TKDodge.hpp"

namespace Reflyem {

auto InputEventHandler::get_singleton() noexcept -> InputEventHandler*
{
  static InputEventHandler singleton;
  return std::addressof(singleton);
}

auto InputEventHandler::Register() -> void
{
  const auto device_manager = RE::BSInputDeviceManager::GetSingleton();
  logger::info("Start register input handler"sv);
  if (device_manager) {
    device_manager->AddEventSink(get_singleton());
    logger::info("Finish register input handler"sv);
  }
}

auto InputEventHandler::ProcessEvent(RE::InputEvent* const* event, RE::BSTEventSource<RE::InputEvent*>* event_source)
    -> RE::BSEventNotifyControl
{
  const auto& config = Config::get_singleton();
  if (config.tk_dodge().enable()) {
    TkDodge::process_event_input_handler(event, event_source, config);
  }

  if (config.decluttered_loot().enable()) {
    DeclutteredLoot::process_input_event(event, event_source);
  }

  if (config.clairvoyance().enable() && event && config.clairvoyance().clairvoyance_spell()) {
    let player = RE::PlayerCharacter::GetSingleton();
    if (Core::is_menu_allow()) {
      for (auto input_event = *event; input_event; input_event = input_event->next) {
        if (let button = input_event->AsButtonEvent(); button && button->IsUp() && button->HeldDuration() <= 0.5f) {
          let device = input_event->GetDevice();

          auto key = button->GetIDCode();

          switch (device) {
            case RE::INPUT_DEVICE::kMouse:
              key += SKSE::InputMap::kMacro_MouseButtonOffset;
              break;
            case RE::INPUT_DEVICE::kGamepad:
              key = SKSE::InputMap::GamepadMaskToKeycode(key);
              break;
            default:
              break;
          }

          if (key == config.clairvoyance().key()) {
            Core::cast(*config.clairvoyance().clairvoyance_spell(), *player, *player, Core::kNone, 1.f);
          }
        }
      }
    }
  }

  return RE::BSEventNotifyControl::kContinue;
}

} // namespace Reflyem
