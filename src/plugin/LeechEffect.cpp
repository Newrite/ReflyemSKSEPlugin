#include "plugin/LeechEffect.hpp"
#include "MagicWeapon.hpp"

namespace Reflyem::LeechEffect {

auto is_first_only(RE::TESBoundObject::BOUND_DATA& bound_data) -> bool
{
  return Core::bound_data_comparer(bound_data, MagicWeapon::Flag::kFirstEffectOnly) ||
         Core::bound_data_comparer(bound_data,
                                   MagicWeapon::Flag::kFirstEffectOnly + MagicWeapon::Flag::kNoDamageDiffMult);
}

auto cast_leech_effect(RE::Actor* actor, const float magnitude, RE::BGSListForm* keywords, RE::BGSListForm* spells) -> void
{

  if (!actor) {
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

    if (!Core::try_has_absolute_keyword(actor, keyword)) {
      continue;
    }

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
    cast(*spell, *actor, *actor, Core::kNone, 1.f);
  }
}

auto character_update(RE::Character& character, float, const Config& config, Core::ActorsCache::Data& actor_data) -> void
{
  const auto leech_magnitude = actor_data.leech_accumulator();
  if (leech_magnitude > 0.f) {
    actor_data.leech_accumulator(0.f);
    cast_leech_effect(&character, leech_magnitude, config.leech_effect().keywords_magic(), config.leech_effect().spells_magic());
  }
}

auto modify_actor_value(const RE::ValueModifierEffect* this_,
                        RE::Actor* actor,
                        float& value,
                        const RE::ActorValue av,
                        const Config&) -> void
{
  if (Core::can_modify_actor_value(this_, actor, value, av)) {
    value = std::abs(value);
    const auto damage_mult = Core::getting_damage_mult(*actor);
    const auto mult_value = value * damage_mult;
    auto& leech_data = Core::ActorsCache::get_or_add_actor_data(this_->caster.get()->formID);
    leech_data.mod_leech_accumulator(mult_value);
    value = -value;
  }
}

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void
{

  let aggressor = hit_data.aggressor.get();
  if (target && !target->IsDead() && aggressor && aggressor.get()) {

    let is_ranged = hit_data.weapon ? hit_data.weapon->IsRanged() : false;
    auto keywords = is_ranged ? config.leech_effect().keywords_ranged() : config.leech_effect().keywords_melee();
    auto spells = is_ranged ? config.leech_effect().spells_ranged() : config.leech_effect().spells_melee();
    
    const auto damage_mult = Core::getting_damage_mult(*target);
    const auto mult_value = hit_data.totalDamage * damage_mult;
    cast_leech_effect(aggressor.get(), mult_value, keywords, spells);
  }
}
} // namespace Reflyem::PetrifiedBlood
