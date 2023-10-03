#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

#include "Core.hpp"
#include "Hooks.hpp"
#include "PrecisionAPI.hpp"
#include "plugin/ActionEventHandler.hpp"
#include "plugin/DeathEventHandler.hpp"
#include "plugin/InputEventHandler.hpp"
#include "plugin/ItemLimit.hpp"
#include "plugin/KillEventHandler.hpp"
#include "plugin/MenuEventHandler.hpp"
#include "plugin/PapyrusExtender.hpp"
#include "plugin/ParryBash.hpp"

auto init_logger() -> void
{
  // ReSharper disable once CppLocalVariableMayBeConst
  auto path = logger::log_directory();
  if (!path) return;

  const auto plugin = SKSE::PluginDeclaration::GetSingleton();
  *path /= fmt::format(FMT_STRING("{}.log"), plugin->GetName());

  std::shared_ptr<spdlog::sinks::sink> sink;
  if (WinAPI::IsDebuggerPresent()) { sink = std::make_shared<spdlog::sinks::msvc_sink_mt>(); }
  else { sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true); }

  auto log = std::make_shared<spdlog::logger>("global log"s, sink);
  log->set_level(spdlog::level::info);
  log->flush_on(spdlog::level::info);

  set_default_logger(std::move(log));
  spdlog::set_pattern("%s(%#): [%^%l%$] %v"s);
}

auto initialize_messaging() -> void
{
  // ReSharper disable once CppParameterMayBeConstPtrOrRef
  if (!SKSE::GetMessagingInterface()->RegisterListener(
          [](SKSE::MessagingInterface::Message* message)
          {
            switch (message->type)
              {
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
                      reinterpret_cast<PRECISION_API::IVPrecision4*>(
                          PRECISION_API::RequestPluginAPI());
                  if (precision_api)
                    {
                      precision_api->AddPreHitCallback(
                          SKSE::GetPluginHandle(),
                          Reflyem::ParryBash::precision_pre_hit_callback);
                      logger::info("Enabled compatibility with Precision");
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
                  Reflyem::ActionEventHandler::Register();
                  Reflyem::InputEventHandler::Register();
                  Reflyem::KillEventHandler::Register();
                  Reflyem::MenuEventHandler::Register();
                  Reflyem::DeathEventHandler::Register();
                  break;
                }
              // Skyrim game events.
              case SKSE::MessagingInterface::kNewGame: // Player starts a new game from main menu.
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
          }))
    {
      stl::report_and_fail("Unable to register message listener."sv);
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
  init_logger();

  const auto plugin = SKSE::PluginDeclaration::GetSingleton();
  logger::info("{} v{}"sv, plugin->GetName(), plugin->GetVersion());

  Init(skse);

  const auto papyrus_vm = SKSE::GetPapyrusInterface();
  if (!papyrus_vm || !papyrus_vm->Register(Reflyem::PapyrusExtender::register_functions))
    {
      logger::info("papyrus_vm is null or can't register papyrus functions"sv);
      return false;
    }

  initialize_messaging();

  const auto serialization = SKSE::GetSerializationInterface();
  if (!serialization)
    {
      logger::info("Serialization interface is null"sv);
      return false;
    }

  serialization->SetUniqueID('REFL');
  serialization->SetSaveCallback(Reflyem::Core::ActorsCache::skse_save_callback);
  serialization->SetLoadCallback(Reflyem::Core::ActorsCache::skse_load_callback);

  logger::info("{} loaded"sv, plugin->GetName());

  return true;
}