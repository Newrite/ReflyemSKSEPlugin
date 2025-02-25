#include "plugin/CellFullyLoadedEventHandler.hpp"
#include "Config.hpp"

namespace Reflyem {
auto CellFullyLoadedEventHandler::get_singleton() noexcept -> CellFullyLoadedEventHandler*
{
  static CellFullyLoadedEventHandler singleton;
  return std::addressof(singleton);
}

auto CellFullyLoadedEventHandler::Register() -> void
{
  const auto script_event_source_holder = RE::ScriptEventSourceHolder::GetSingleton();
  logger::info("Start register cell loaded handler"sv);

  if (script_event_source_holder) {
    const auto script_event_source = script_event_source_holder->GetEventSource<RE::TESCellFullyLoadedEvent>();

    if (script_event_source) {
      script_event_source->AddEventSink(get_singleton());
      logger::info("Finish register cell loaded handler"sv);
    }
  }
}

auto CellFullyLoadedEventHandler::ProcessEvent(const RE::TESCellFullyLoadedEvent* event, RE::BSTEventSource<RE::TESCellFullyLoadedEvent>*)
    -> RE::BSEventNotifyControl
{
  if (event && event->cell) {
    logi("LoadedCell: {}", event->cell->fullName.c_str());
    auto location = event->cell->GetLocation();
    if (location) {
      logi("LoadedLocation: {}", location->fullName.c_str());
    }
    let ui = RE::UI::GetSingleton();
    if (ui) {
      auto hud_menu = ui->GetMenu(RE::HUDMenu::MENU_NAME);
      if (hud_menu && hud_menu->uiMovie) {
        logi("Call set location name");
        const RE::GFxValue location_name[2]{event->cell->fullName.c_str(), ""};
        hud_menu->uiMovie->InvokeNoReturn("HUDMovieBaseInstance.QuestUpdateBaseInstance.AnimatedLetter_mc.ShowQuestUpdate", location_name, 2);
      }
    }
  }
  return RE::BSEventNotifyControl::kContinue;
}

} // namespace Reflyem
