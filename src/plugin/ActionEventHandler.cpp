#include "plugin/ActionEventHandler.hpp"

#include "AbsorbShield.hpp"
#include "KiEnergyPower.hpp"
#include "StaggerSystem.hpp"
#include "plugin/CasterAdditions.hpp"

namespace Reflyem {

[[nodiscard]] auto ActionEventHandler::get_singleton() noexcept -> ActionEventHandler*
{
  static ActionEventHandler instance;
  return std::addressof(instance);
}

auto ActionEventHandler::Register() -> void
{
  const auto action_source = SKSE::GetActionEventSource();
  logger::info("Start register action handler"sv);
  if (action_source) {
    action_source->AddEventSink(get_singleton());
    logger::info("Finish register action handler"sv);
  }
}

auto ActionEventHandler::ProcessEvent(const SKSE::ActionEvent* event,
                                      RE::BSTEventSource<SKSE::ActionEvent>* event_source) -> RE::BSEventNotifyControl
{
  if (event && event_source) {
    const auto& config = Config::get_singleton();
    if (config.caster_additions().enable()) {
      CasterAdditions::action_event_handler(*event, *event_source);
    }
    if (config.absorb_shield().enable()) {
      AbsorbShield::action_event_handler(*event, *event_source);
    }
    if (config.ki_energy_power().enable()) {
      KiEnergyPower::action_event_handler(*event, *event_source);
    }
    if (config.stagger_system().enable()) {
      StaggerSystem::action_event_handler(*event, *event_source);
    }
  }
  return RE::BSEventNotifyControl::kContinue;
}
} // namespace Reflyem
