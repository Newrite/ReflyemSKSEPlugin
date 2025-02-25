#include "plugin/CastOnHit.hpp"
#include "Core.hpp"

namespace Reflyem::CastOnHit {

auto on_weapon_hit(RE::Actor* target, const RE::HitData& hit_data, const Config& config) -> void
{
  const auto aggressor = hit_data.aggressor.get();

  if (!aggressor || target->IsDead()) {
    return;
  }

  let is_bash = Core::is_bashing(aggressor.get());
  let is_melee = hit_data.weapon && !hit_data.weapon->IsRanged();
  let is_ranged = hit_data.weapon && hit_data.weapon->IsRanged();

  if (is_melee && !is_bash) {
    Core::cast_on_handle_formlists(config.cast_on_hit().formlist_melee_needkw(),
                                   config.cast_on_hit().formlist_melee_spells(),
                                   *aggressor,
                                   *target);
  }

  if (is_bash) {
    Core::cast_on_handle_formlists(
        config.cast_on_hit().formlist_bash_needkw(), config.cast_on_hit().formlist_bash_spells(), *aggressor, *target);
  }

  if (is_ranged) {
    Core::cast_on_handle_formlists(config.cast_on_hit().formlist_ranged_needkw(),
                                   config.cast_on_hit().formlist_ranged_spells(),
                                   *aggressor,
                                   *target);
  }
}
} // namespace Reflyem::CastOnHit
