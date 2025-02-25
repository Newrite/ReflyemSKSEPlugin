#include "plugin/CastOnSoulTrap.hpp"
#include "Core.hpp"

namespace Reflyem::CastOnSoulTrap
{

auto soul_trap_handler(RE::Actor& trapper, RE::Actor& target, const Config& config) -> void
{
  Core::cast_on_handle_formlists(
      config.cast_on_soul_trap().formlist_needkw(),
      config.cast_on_soul_trap().formlist_spells(),
      trapper,
      target);
}
} // namespace Reflyem::CastOnSoulTrap
