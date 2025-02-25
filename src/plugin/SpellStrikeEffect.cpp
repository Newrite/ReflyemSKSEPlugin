#include "plugin/LeechEffect.hpp"
#include "MagicWeapon.hpp"

namespace Reflyem::SpellStrikeEffect {

auto is_first_only(RE::TESBoundObject::BOUND_DATA& bound_data) -> bool
{
  return Core::bound_data_comparer(bound_data, MagicWeapon::Flag::kFirstEffectOnly) ||
         Core::bound_data_comparer(bound_data,
                                   MagicWeapon::Flag::kFirstEffectOnly + MagicWeapon::Flag::kNoDamageDiffMult);
}

auto cast_spell_strike_effect(RE::Actor* caster, RE::Actor* target, const float magnitude, RE::BGSListForm* keywords, RE::BGSListForm* spells) -> void
{

  if (!caster) {
    return;
  }

  if (!keywords || !spells) {
    return;
  }

  const auto length_kw = keywords->forms.size();
  const auto length_sp = spells->forms.size();

  for (std::uint32_t index = 0u; index < length_kw && index < length_sp; index++) {

    let keyword_form = keywords->forms[index];
    let spell_form = spells->forms[index];

    if (!keyword_form || !spell_form) {
      continue;
    }

    let keyword = keyword_form->As<RE::BGSKeyword>();
    let spell = spell_form->As<RE::SpellItem>();

    if (!keyword || !spell) {
      continue;
    }

    if (!Core::try_has_absolute_keyword(caster, keyword)) {
      continue;
    }

    // if (spell->data.delivery != RE::MagicSystem::Delivery::kSelf && !target) {
    //   continue;
    // }

    if (spell->effects.empty()) {
      continue;
    }

    if (is_first_only(spell->boundData)) {
      spell->effects[0]->effectItem.magnitude = magnitude;
    } else {
      for (auto effect : spell->effects) {
        if (effect) {
          effect->effectItem.magnitude = magnitude;
        }
      }
    }
    cast(*spell, *target, *caster, Core::kNone, 1.f);
  }
}

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void
{

  let aggressor = hit_data.aggressor.get();
  if (target && !target->IsDead() && aggressor && aggressor.get()) {

    let is_ranged = hit_data.weapon ? hit_data.weapon->IsRanged() : false;
    auto keywords = is_ranged ? config.spell_strike_effect().keywords_ranged() : config.spell_strike_effect().keywords_melee();
    auto spells = is_ranged ? config.spell_strike_effect().spells_ranged() : config.spell_strike_effect().spells_melee();
    
    const auto damage_mult = Core::getting_damage_mult(*target);
    const auto mult_value = hit_data.totalDamage * damage_mult;
    cast_spell_strike_effect(aggressor.get(), target, mult_value, keywords, spells);
  }
}
} // namespace Reflyem::PetrifiedBlood
