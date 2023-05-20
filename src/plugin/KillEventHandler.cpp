﻿#include "plugin/KillEventHandler.hpp"
#include "Config.hpp"
#include "plugin/CastOnKill.hpp"

namespace Reflyem
{

[[nodiscard]] auto KillEventHandler::get_singleton() noexcept -> KillEventHandler*
{
  static KillEventHandler instance;
  return std::addressof(instance);
}

auto KillEventHandler::Register() -> void
{
  const auto kill_event_source = RE::ActorKill::GetEventSource();
  logger::info("Start register kill handler"sv);
  if (kill_event_source)
    {
      kill_event_source->AddEventSink(get_singleton());
      logger::info("Finish register kill handler"sv);
    }
}

auto KillEventHandler::ProcessEvent(
    const RE::ActorKill::Event* event,
    RE::BSTEventSource<RE::ActorKill::Event>*) -> RE::BSEventNotifyControl
{
  if (event)
    {
      const auto killer = event->killer;
      const auto victim = event->victim;

      if (killer && victim)
        {
          logger::debug("{} kill {}"sv, killer->GetDisplayFullName(), victim->GetDisplayFullName());

          const auto& config = Config::get_singleton();

          if (config.cast_on_kill().enable())
            {
            logger::debug("Cast on kill enabled, call kill_handler");
              CastOnKill::kill_handler(*killer, *victim, config);
            }
        }
    }
  return RE::BSEventNotifyControl::kContinue;
}

} // namespace Reflyem