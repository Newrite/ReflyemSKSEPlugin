#include "CastOnHit.h"
#include "Core.h"

namespace Reflyem {
namespace CastOnHit {

auto
on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Reflyem::Config& config) -> void {
  auto agressor = hit_data.aggressor.get();

  if (!agressor || target->IsDead()) {
    return;
  }

  auto length_kw = config.cast_on_crit_formlist_needkw->forms.size();
  auto length_sp = config.cast_on_crit_formlist_spells->forms.size();
  for (std::uint32_t index = 0u; index < length_kw && index < length_sp; index++) {
    Reflyem::Core::cast_on_handle(config.cast_on_crit_formlist_needkw->forms[index],
                                  config.cast_on_crit_formlist_spells->forms[index], *target, *agressor);
  }
}
} // namespace CastOnHit
} // namespace Reflyem