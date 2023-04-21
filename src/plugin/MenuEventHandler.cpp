#include "plugin/MenuEventHandler.hpp"

namespace Reflyem
{

[[nodiscard]] auto MenuEventHandler::get_singleton() noexcept -> MenuEventHandler*
{
  static MenuEventHandler instance;
  return std::addressof(instance);
}

auto MenuEventHandler::Register() -> void
{
  logger::info("Start register menu open close handler"sv);
  if (const auto ui = RE::UI::GetSingleton())
    {
      ui->AddEventSink(get_singleton());
      logger::info("Finish register menu open close handler"sv);
    }
}

auto MenuEventHandler::ProcessEvent(
    const RE::MenuOpenCloseEvent*,
    RE::BSTEventSource<RE::MenuOpenCloseEvent>*) -> RE::BSEventNotifyControl
{
  return RE::BSEventNotifyControl::kContinue;
}

} // namespace Reflyem