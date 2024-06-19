#include "plugin/MenuEventHandler.hpp"
#include "Config.hpp"

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
    const RE::MenuOpenCloseEvent* menu_event,
    RE::BSTEventSource<RE::MenuOpenCloseEvent>*) -> RE::BSEventNotifyControl
{
  if (menu_event && !menu_event->opening) {
    if (menu_event->menuName == RE::JournalMenu::MENU_NAME) {
      auto& config = Config::get_singleton();
      config.load();
    }
  }
  return RE::BSEventNotifyControl::kContinue;
}

} // namespace Reflyem
