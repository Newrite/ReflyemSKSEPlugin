#include "plugin/CastOnKill.hpp"
#include "Core.hpp"

namespace Reflyem::CastOnKill
{

auto kill_handler(RE::Actor& killer, RE::Actor& victim, const Config& config) -> void
{
  Core::cast_on_handle_formlists(
      config.cast_on_kill().formlist_needkw(),
      config.cast_on_kill().formlist_spells(),
      killer,
      victim);
}
} // namespace Reflyem::CastOnKill