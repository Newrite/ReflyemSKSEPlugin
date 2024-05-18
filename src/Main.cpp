// #include "Config2.hpp"

#include "Core.hpp"
#include "Hooks.hpp"
#include "PotionsDrinkLimit.hpp"
#include "PrecisionAPI.hpp"
#include "plugin/ActionEventHandler.hpp"
#include "plugin/DeathEventHandler.hpp"
#include "plugin/InputEventHandler.hpp"
#include "plugin/ItemLimit.hpp"
#include "plugin/KillEventHandler.hpp"
#include "plugin/MenuEventHandler.hpp"
#include "plugin/PapyrusExtender.hpp"
#include "plugin/ParryBash.hpp"

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
#ifndef DEBUG
  auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
  auto path = logger::log_directory();
  if (!path) {
    return false;
  }

  *path /= Version::PROJECT;
  *path += ".log"sv;
  auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

  auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef DEBUG
  log->set_level(spdlog::level::trace);
#else
  log->set_level(spdlog::level::info);
  log->flush_on(spdlog::level::info);
#endif

  spdlog::set_default_logger(std::move(log));
  spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

  logger::info(FMT_STRING("{} v{}"), Version::PROJECT, Version::NAME);

  a_info->infoVersion = SKSE::PluginInfo::kVersion;
  a_info->name = Version::PROJECT.data();
  a_info->version = Version::MAJOR;

  if (a_skse->IsEditor()) {
    logger::critical("Loaded in editor, marking as incompatible"sv);
    return false;
  }

  const auto ver = a_skse->RuntimeVersion();
  if (ver < SKSE::RUNTIME_1_5_39) {
    logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
    return false;
  }

  return true;
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
        const auto precision_api =
            // ReSharper disable once CppReinterpretCastFromVoidPtr
            reinterpret_cast<PRECISION_API::IVPrecision4*>(PRECISION_API::RequestPluginAPI());
        if (precision_api) {
          precision_api->AddPreHitCallback(SKSE::GetPluginHandle(), Reflyem::ParryBash::precision_pre_hit_callback);
          precision_api->AddWeaponWeaponCollisionCallback(SKSE::GetPluginHandle(),
                                                          Reflyem::ParryBash::precision_weapons_collide_callback);
          logger::info("Enabled compatibility with Precision");
        } else {
          logger::info("Precision not found");
        }

        let giga_widget =
            // ReSharper disable once CppReinterpretCastFromVoidPtr
            reinterpret_cast<GIGAWIDGET_API::IVGigaWidget1*>(GIGAWIDGET_API::RequestPluginAPI());
        if (!giga_widget) {
          logi("GigaWidget not found"sv);
        } else {
          giga_widget->add_set_available_potion_slot_callback(
              SKSE::GetPluginHandle(), Reflyem::PotionsDrinkLimit::Callbacks::set_available_potion_slot_callback);
          giga_widget->add_set_frame_for_potion_callback(
              SKSE::GetPluginHandle(), Reflyem::PotionsDrinkLimit::Callbacks::set_frame_for_potion_callback);
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
        Hooks::install_hooks();
        let _ = Reflyem::Config::get_singleton();
        Reflyem::ActionEventHandler::Register();
        Reflyem::InputEventHandler::Register();
        Reflyem::KillEventHandler::Register();
        Reflyem::MenuEventHandler::Register();
        Reflyem::DeathEventHandler::Register();
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

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
  auto g_messaging =
      reinterpret_cast<SKSE::MessagingInterface*>(a_skse->QueryInterface(SKSE::LoadInterface::kMessaging));
  if (!g_messaging) {
    logger::critical("Failed to load messaging interface! This error is fatal, plugin will not load.");
    return false;
  }

  logger::info("loaded");

  Init(a_skse);

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
