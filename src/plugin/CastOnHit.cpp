#include "CastOnHit.h"
#include "Core.h"

namespace reflyem
{
  namespace cast_on_hit
  {

    auto on_weapon_hit(
      RE::Actor* target,
      RE::HitData& hit_data,
      const reflyem::config& config) -> void
    {

      auto agressor = hit_data.aggressor.get();

      if (!agressor) { return; }

      auto length_kw =
        config.cast_on_crit_formlist_needkw->forms.size();
      auto length_sp =
        config.cast_on_crit_formlist_spells->forms.size();
      for (std::uint32_t index = 0u; index < length_kw && index < length_sp; index++)
      {
        reflyem::core::cast_on_handle(
          config.cast_on_crit_formlist_needkw->forms[index],
          config.cast_on_crit_formlist_spells->forms[index],
          *target,
          *agressor);
      }

    }
  }
}