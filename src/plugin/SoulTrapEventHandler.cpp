#include "plugin/SoulTrapEventHandler.hpp"
#include "Config.hpp"
#include "RE/S/SoulsTrapped.h"
#include "plugin/CastOnSoulTrap.hpp"

namespace Reflyem {

[[nodiscard]] auto SoulTrapEventHandler::get_singleton() noexcept -> SoulTrapEventHandler*
{
  static SoulTrapEventHandler instance;
  return std::addressof(instance);
}

auto SoulTrapEventHandler::Register() -> void
{
  const auto soul_trap_event_source = RE::SoulsTrapped::GetEventSource();
  logger::info("Start register soul trap handler"sv);
  if (soul_trap_event_source) {
    soul_trap_event_source->AddEventSink(get_singleton());
    logger::info("Finish register soul trap handler"sv);
  }
}

auto SoulTrapEventHandler::ProcessEvent(const RE::SoulsTrapped::Event* event, RE::BSTEventSource<RE::SoulsTrapped::Event>*)
    -> RE::BSEventNotifyControl
{
  if (event) {
    const auto trapper = event->trapper;
    const auto target = event->target;

    if (trapper && target) {
      logger::debug("{} trap {}"sv, trapper->GetDisplayFullName(), target->GetDisplayFullName());

      const auto& config = Config::get_singleton();

      if (config.cast_on_soul_trap().enable()) {
        logger::debug("Cast on soul trap enabled, call soul_trap_handler");
        CastOnSoulTrap::soul_trap_handler(*trapper, *target, config);
      }
      
    }
  }
  return RE::BSEventNotifyControl::kContinue;
}

} // namespace Reflyem
