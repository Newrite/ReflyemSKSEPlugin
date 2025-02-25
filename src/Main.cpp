// #include "Config2.hpp"

#include "AbsorbShield.hpp"
#include "CellFullyLoadedEventHandler.hpp"
#include "Core.hpp"
#include "GameSettingsHandler.hpp"
#include "Hooks.hpp"
#include "KiEnergyPower.hpp"
#include "PotionsDrinkLimit.hpp"
#include "PrecisionAPI.hpp"
#include "SoulTrapEventHandler.hpp"
#include "StaggerSystem.hpp"
#include "TrueHUDAPI.hpp"
#include "plugin/ActionEventHandler.hpp"
#include "plugin/DeathEventHandler.hpp"
#include "plugin/InputEventHandler.hpp"
#include "plugin/ItemLimit.hpp"
#include "plugin/KillEventHandler.hpp"
#include "plugin/MenuEventHandler.hpp"
#include "plugin/PapyrusExtender.hpp"
#include "plugin/ParryBash.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

void init_logging()
{
  auto path = logger::log_directory();
  if (!path)
    return;

  const auto plugin = SKSE::PluginDeclaration::GetSingleton();
  *path /= std::format("{}.log", plugin->GetName());

  std::vector<spdlog::sink_ptr> sinks{std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true),
                                      std::make_shared<spdlog::sinks::msvc_sink_mt>()};

  auto logger = std::make_shared<spdlog::logger>("global", sinks.begin(), sinks.end());
  logger->set_level(spdlog::level::info);
  logger->flush_on(spdlog::level::info);

  spdlog::set_default_logger(std::move(logger));
  spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);
}

static void SKSEMessageHandler(SKSE::MessagingInterface::Message* message)
{
  switch (message->type) {
    // Skyrim lifecycle events.
    case SKSE::MessagingInterface::kPostLoad: // Called after all plugins have finished
                                              // running
    // SKSEPlugin_Load. It is now safe to do
    // multithreaded operations, or operations against
    // other plugins.
    case SKSE::MessagingInterface::kPostPostLoad: // Called after all kPostLoad message
                                                  // handlers
      // have run.
      {
        // ReSharper disable once CppTooWideScope
        Reflyem::Core::ExternalApi::precision_api =
            // ReSharper disable once CppReinterpretCastFromVoidPtr
            reinterpret_cast<PRECISION_API::IVPrecision4*>(PRECISION_API::RequestPluginAPI());
        if (Reflyem::Core::ExternalApi::precision_api) {
          Reflyem::Core::ExternalApi::precision_api->AddPreHitCallback(SKSE::GetPluginHandle(),
                                                                       Reflyem::ParryBash::precision_pre_hit_callback);
          Reflyem::Core::ExternalApi::precision_api->AddWeaponWeaponCollisionCallback(
              SKSE::GetPluginHandle(), Reflyem::ParryBash::precision_weapons_collide_callback);
          logger::info("Enabled compatibility with Precision");
        } else {
          logger::info("Precision not found");
        }

        // ReSharper disable once CppTooWideScope
        Reflyem::Core::ExternalApi::true_hud_api =
            // ReSharper disable once CppReinterpretCastFromVoidPtr
            reinterpret_cast<TRUEHUD_API::IVTrueHUD4*>(TRUEHUD_API::RequestPluginAPI());
        if (Reflyem::Core::ExternalApi::true_hud_api) {
          logger::info("Enabled compatibility with true hud");
        } else {
          logger::info("true hud not found");
        }

        Reflyem::Core::ExternalApi::giga_widget_api =
            // ReSharper disable once CppReinterpretCastFromVoidPtr
            reinterpret_cast<GIGAWIDGET_API::IVGigaWidget1*>(GIGAWIDGET_API::RequestPluginAPI());
        if (!Reflyem::Core::ExternalApi::giga_widget_api) {
          logi("GigaWidget not found"sv);
        } else {
          Reflyem::Core::ExternalApi::giga_widget_api->add_set_available_potion_slot_callback(
              SKSE::GetPluginHandle(), Reflyem::PotionsDrinkLimit::Callbacks::set_available_potion_slot_callback);
          Reflyem::Core::ExternalApi::giga_widget_api->add_set_frame_for_potion_callback(
              SKSE::GetPluginHandle(), Reflyem::PotionsDrinkLimit::Callbacks::set_frame_for_potion_callback);
          Reflyem::Core::ExternalApi::giga_widget_api->add_set_available_ki_energy_callback(
              SKSE::GetPluginHandle(), Reflyem::KiEnergyPower::Callbacks::set_available_ki_energy_callback);
          Reflyem::Core::ExternalApi::giga_widget_api->add_set_frame_for_ki_energy_callback(
              SKSE::GetPluginHandle(), Reflyem::KiEnergyPower::Callbacks::set_frame_for_ki_energy_callback);
          Reflyem::Core::ExternalApi::giga_widget_api->add_set_frame_for_absorb_shield_callback(
              SKSE::GetPluginHandle(), Reflyem::AbsorbShield::Callbacks::set_frame_for_absorb_shield_callback);
          logger::info("Add giga_widget callback"sv);
        }
      }
    case SKSE::MessagingInterface::kInputLoaded: // Called when all game data has been
                                                 // found.
      break;
    case SKSE::MessagingInterface::kDataLoaded: // All ESM/ESL/ESP plugins have loaded,
      // main menu is now active.
      {
        // It is now safe to access form data.
        logger::info("Get data loaded message"sv);
        letr config = Reflyem::Config::get_singleton();
        Hooks::handle_all_forms();
        Hooks::install_hooks();
        if (config.stagger_system().enable() && Reflyem::Core::ExternalApi::true_hud_api) {
          if (Reflyem::Core::ExternalApi::true_hud_api->RequestSpecialResourceBarsControl(SKSE::GetPluginHandle()) ==
              TRUEHUD_API::APIResult::OK) {
            Reflyem::Core::ExternalApi::true_hud_api->RegisterSpecialResourceFunctions(
                SKSE::GetPluginHandle(),
                Reflyem::StaggerSystem::TrueHUD::get_current_resource_special_bar,
                Reflyem::StaggerSystem::TrueHUD::get_max_resource_special_bar,
                true);
          }
        }
        if (!config.stagger_system().enable() && config.misc_fixes().enable_blade_and_blunt_true_hud_special_bar() &&
            Reflyem::Core::ExternalApi::true_hud_api) {
          if (Reflyem::Core::ExternalApi::true_hud_api->RequestSpecialResourceBarsControl(SKSE::GetPluginHandle()) ==
              TRUEHUD_API::APIResult::OK) {
            Reflyem::Core::ExternalApi::true_hud_api->RegisterSpecialResourceFunctions(
                SKSE::GetPluginHandle(),
                Reflyem::Core::ExternalApi::get_current_resource_special_bar,
                Reflyem::Core::ExternalApi::get_max_resource_special_bar,
                true);
          }
        }
        Reflyem::ActionEventHandler::Register();
        // Reflyem::CellFullyLoadedEventHandler::Register();
        Reflyem::InputEventHandler::Register();
        Reflyem::KillEventHandler::Register();
        Reflyem::SoulTrapEventHandler::Register();
        Reflyem::MenuEventHandler::Register();
        Reflyem::DeathEventHandler::Register();
        Reflyem::GameSettingsHandler::GMST::game_settings_handler(config);
        break;
      }
    // Skyrim game events.
    case SKSE::MessagingInterface::kNewGame:     // Player starts a new game from main menu.
    case SKSE::MessagingInterface::kPreLoadGame: // Player selected a game to load, but it
    // hasn't loaded yet. Data will be the name of
    // the loaded save.
    case SKSE::MessagingInterface::kPostLoadGame: // Player's selected save game has
                                                  // finished
    // loading. Data will be a boolean indicating
    // whether the load was successful.
    case SKSE::MessagingInterface::kSaveGame: // The player has saved a game.
    // Data will be the save name.
    case SKSE::MessagingInterface::kDeleteGame: // The player deleted a saved game from
                                                // within
      // the load menu.
      break;
    default: {
      break;
    }
  }
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{

  init_logging();

  const auto plugin = SKSE::PluginDeclaration::GetSingleton();
  logger::info("{} v{} is loading...", plugin->GetName(), plugin->GetVersion());

  logger::info("loaded");

  Init(a_skse);

  auto g_messaging =
      reinterpret_cast<SKSE::MessagingInterface*>(a_skse->QueryInterface(SKSE::LoadInterface::kMessaging));
  if (!g_messaging) {
    logger::critical("Failed to load messaging interface! This error is fatal, plugin will not load.");
    return false;
  }
  const auto papyrus_vm = SKSE::GetPapyrusInterface();
  if (!papyrus_vm || !papyrus_vm->Register(Reflyem::PapyrusExtender::register_functions)) {
    logger::info("papyrus_vm is null or can't register papyrus functions"sv);
    return false;
  }

  g_messaging->RegisterListener("SKSE", SKSEMessageHandler);

  const auto serialization = SKSE::GetSerializationInterface();
  if (!serialization) {
    logger::info("Serialization interface is null"sv);
    return false;
  }

  serialization->SetUniqueID('REFL');
  serialization->SetSaveCallback(Reflyem::Core::ActorsCache::skse_save_callback);
  serialization->SetLoadCallback(Reflyem::Core::ActorsCache::skse_load_callback);

  // logger::info("{} loaded"sv, plugin->GetName());

  return true;
}
