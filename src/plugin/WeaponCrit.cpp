#include "WeaponCrit.h"
#include "Core.h"

namespace Reflyem
{
  namespace WeaponCrit
  {

    auto on_weapon_hit(
      RE::Actor* target,
      RE::HitData& hit_data,
      const Reflyem::Config& config) -> void
    {

      auto agressor = hit_data.aggressor.get();

      if (!agressor || target->IsDead()) { return; }

      auto crit_chance = 
        static_cast<int>(agressor->GetActorValue(config.weapon_crit_chance_av));
      auto crit_damage = 
        static_cast<int>(agressor->GetActorValue(config.weapon_crit_damage_av));

      if (crit_chance <= 0 || crit_damage <= 0) { return; }
      if (crit_chance > 100) { crit_chance = 100; }
      if (crit_damage > config.weapon_crit_high) { crit_damage = config.weapon_crit_high; }

      auto random = Reflyem::Core::get_rundom_int();

      if (crit_chance > random)
      {
        auto damage_mult = 1 + (crit_damage / 100.f);
        hit_data.totalDamage *= damage_mult;

        if (config.cast_on_crit_enable 
          && config.cast_on_crit_formlist_needkw 
          && config.cast_on_crit_formlist_spells)
        {
          auto length_kw = 
            config.cast_on_crit_formlist_needkw->forms.size();
          auto length_sp =
            config.cast_on_crit_formlist_spells->forms.size();
          for (std::uint32_t index = 0u; index < length_kw && index < length_sp; index++)
          {
            Reflyem::Core::cast_on_handle(
              config.cast_on_crit_formlist_needkw->forms[index],
              config.cast_on_crit_formlist_spells->forms[index],
              *target,
              *agressor);
          }

        }

      }

    }
  }
}