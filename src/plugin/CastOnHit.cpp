#include "plugin/CastOnHit.hpp"
#include "Core.hpp"

namespace Reflyem::CastOnHit
{

auto on_weapon_hit(RE::Actor* target, const RE::HitData& hit_data, const Config& config) -> void
{
  const auto aggressor = hit_data.aggressor.get();

  if (!aggressor || target->IsDead()) { return; }

  if (!config.cast_on_hit().formlist_needkw() || !config.cast_on_hit().formlist_spells())
    {
      return;
    }

  const auto length_kw = config.cast_on_hit().formlist_needkw()->forms.size();
  const auto length_sp = config.cast_on_hit().formlist_spells()->forms.size();
  for (std::uint32_t index = 0u; index < length_kw && index < length_sp; index++)
    {
      Core::cast_on_handle(
          config.cast_on_hit().formlist_needkw()->forms[index],
          config.cast_on_hit().formlist_spells()->forms[index],
          *target,
          *aggressor);
    }
}
} // namespace Reflyem::CastOnHit
