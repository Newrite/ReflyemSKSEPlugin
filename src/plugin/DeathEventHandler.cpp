#include "plugin/DeathEventHandler.hpp"
#include "Config.hpp"
#include "plugin/DeathLoot.hpp"

namespace Reflyem {
auto DeathEventHandler::get_singleton() noexcept -> DeathEventHandler*
{
  static DeathEventHandler singleton;
  return std::addressof(singleton);
}

auto DeathEventHandler::Register() -> void
{
  const auto script_event_source_holder = RE::ScriptEventSourceHolder::GetSingleton();
  logger::info("Start register death handler"sv);

  if (script_event_source_holder) {
    const auto script_event_source = script_event_source_holder->GetEventSource<RE::TESDeathEvent>();

    if (script_event_source) {
      script_event_source->AddEventSink(get_singleton());
      logger::info("Finish register death handler"sv);
    }
  }
}

auto DeathEventHandler::ProcessEvent(const RE::TESDeathEvent* event, RE::BSTEventSource<RE::TESDeathEvent>*)
    -> RE::BSEventNotifyControl
{
  if (event) {
    letr config = Config::get_singleton();
    if (config.death_loot().enable()) {
      DeathLoot::process_death(event);
    }
  }
  return RE::BSEventNotifyControl::kContinue;
}

} // namespace Reflyem
