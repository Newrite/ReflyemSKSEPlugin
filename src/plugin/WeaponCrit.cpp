#include "WeaponCrit.h"
#include "Core.h"

namespace reflyem
{
  namespace weapon_crit
  {

    auto cast_on_crit_handle(
      RE::TESForm* keyword,
      RE::TESForm* spell,
      RE::Actor& target,
      RE::Actor& caster) -> void
    {
      if (!keyword || !spell) { return; }

      auto keyword_ptr = keyword->As<RE::BGSKeyword>();
      auto spell_ptr = spell->As<RE::SpellItem>();

      if (!keyword_ptr || !spell_ptr) { return; }

      if (spell_ptr->data.delivery == RE::MagicSystem::Delivery::kSelf)
      {
        caster
          .GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
          ->CastSpellImmediate(
            spell_ptr,
            true,
            &caster,
            1.00f,
            false,
            0.0f,
            &caster
          );
      }
      else
      {
        caster
          .GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
          ->CastSpellImmediate(
            spell_ptr,
            true,
            &target,
            1.00f,
            false,
            0.0f,
            &caster
          );
      }
      
    }

    auto on_weapon_hit(
      RE::Actor* target,
      RE::HitData& hit_data,
      const reflyem::config& config) -> void
    {

      auto agressor = hit_data.aggressor.get();

      if (!agressor) { return; }

      auto crit_chance = 
        static_cast<int>(agressor->GetActorValue(config.weapon_crit_chance_av));
      auto crit_damage = 
        static_cast<int>(agressor->GetActorValue(config.weapon_crit_damage_av));

      if (crit_chance <= 0 || crit_damage <= 0) { return; }
      if (crit_chance > 100) { crit_chance = 100; }
      if (crit_damage > config.weapon_crit_high) { crit_damage = config.weapon_crit_high; }

      auto random = reflyem::core::get_rundom_int();

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
            cast_on_crit_handle(
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