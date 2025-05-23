﻿#include "MagicWeapon.hpp"

#include "Config.hpp"
#include "Core.hpp"

namespace Reflyem::MagicWeapon {

struct MagnitudeData final
{
  float magnitude;
  bool only_first_effect;
  float diff_mult;
};

auto eval_percent(const float value) -> float
{
  if (value > 100.f) {
    return 1.f;
  }

  if (value <= 0.f) {
    return 0.f;
  }

  return value / 100.f;
};

auto percent_from_enchantment(RE::EnchantmentItem& weapon_ench, const RE::BGSKeyword& keyword) -> std::optional<float>
{
  for (const auto& effect : weapon_ench.effects) {
    if (effect && effect->baseEffect && effect->baseEffect->HasKeyword(&keyword)) {
      return {eval_percent(effect->effectItem.magnitude)};
    }
  }
  return std::nullopt;
}

auto handle_cast(RE::Actor& caster, RE::Actor& target, RE::SpellItem& spell, const MagnitudeData& data) -> void
{
  logger::debug("Start handle cast");
  const auto set_magnitude = [data](RE::Effect* effect) -> void {
    if (effect->baseEffect && effect->baseEffect->data.primaryAV != RE::ActorValue::kHealth) {
      effect->effectItem.magnitude = data.magnitude * data.diff_mult;
    } else {
      effect->effectItem.magnitude = data.magnitude;
    }
  };

  if (data.only_first_effect && !spell.effects.empty() && spell.effects[0]) {
    spell.effects[0]->effectItem.magnitude = data.magnitude;
    set_magnitude(spell.effects[0]);
  } else {
    for (const auto& effect : spell.effects) {
      if (effect) {
        set_magnitude(effect);
      }
    }
  }
  logger::debug("handle cast: call cast");
  Core::cast(spell, target, caster, Core::kNone, 1.f);
}

auto handle_cast_magic_weapon_spell(RE::Actor& caster, RE::Actor& target, float real_damage, RE::HitData& hit_data)
    -> void
{
  const auto& config = Config::get_singleton();

  const auto spells = config.magic_weapon().spells();
  const auto keywords = config.magic_weapon().keywords();
  const auto globals = config.magic_weapon().globals();

  const auto spells_size = spells->forms.size();

  logger::debug("SPSIZE {} GSIZE {} KSIZE {}", spells_size, globals->forms.size(), keywords->forms.size());
  if (spells_size != globals->forms.size() || spells_size != keywords->forms.size()) {
    return;
  }

  const auto total_damage = real_damage;
  const auto physical_total_damage = hit_data.totalDamage;

  const auto eval_magnitude = [&](const RE::SpellItem& spell, const float percent) -> MagnitudeData {
    logger::debug("Start eval magnitude");
    auto magnitude = total_damage * percent;

    if (real_damage >= magnitude) {
      real_damage = real_damage - magnitude;
    } else {
      magnitude = real_damage;
      real_damage = 0.f;
    }

    if (const auto minus_damage = physical_total_damage * percent; hit_data.totalDamage >= minus_damage) {
      hit_data.totalDamage = hit_data.totalDamage - minus_damage;
    } else {
      hit_data.totalDamage = 0.f;
    }

    const auto mult = [&]() -> float {
      if (Core::bound_data_comparer(spell.boundData, Flag::kDivideMagnitude) ||
          Core::bound_data_comparer(spell.boundData, Flag::kDivideMagnitude + Flag::kNoDamageDiffMult)) {
        logger::debug("magic weapon:: mult count of spell"sv);
        return static_cast<float>(spell.effects.size());
      }
      logger::debug("magic weapon:: one count of spell"sv);
      return 1.f;
    }();

    return MagnitudeData{magnitude / mult,
                         (Core::bound_data_comparer(spell.boundData, Flag::kFirstEffectOnly) ||
                          Core::bound_data_comparer(spell.boundData, Flag::kFirstEffectOnly + Flag::kNoDamageDiffMult)),
                         Core::getting_damage_mult(target)};
  };

  logger::debug("Start cycle");
  for (uint32_t index = 0; index < spells_size; index++) {
    if (real_damage <= 0.f || hit_data.totalDamage <= 0.f) {
      return;
    }

    logger::debug("Current index: {}", index);

    const auto spell = spells->forms[index]->As<RE::SpellItem>();
    const auto global = globals->forms[index]->As<RE::TESGlobal>();
    const auto keyword = keywords->forms[index]->As<RE::BGSKeyword>();

    if (!spell || !keyword) {
      continue;
    }

    if (spell->data.delivery != RE::MagicSystem::Delivery::kTouch) {
      continue;
    }

    if (global && global->value != 0.f) {
      logger::debug("In global, global is {}", global->value);
      float value;
      if (global->value < 0.f) {
        logger::debug("Global check weapon keyword");
        if (hit_data.weapon && !hit_data.weapon->HasKeyword(keyword)) {
          continue;
        }
        value = std::abs(global->value);
        logger::debug("Value is: {}", value);
      } else {
        logger::debug("Global check has_absolute_keyword");
        if (!Core::has_absolute_keyword(caster, *keyword)) {
          continue;
        }
        value = global->value;
        logger::debug("Value is: {}", value);
      }

      logger::debug("Call handle cast");
      handle_cast(caster, target, *spell, eval_magnitude(*spell, eval_percent(value)));
    } else {
      logger::debug("In ench");
      if (!hit_data.weapon) {
        continue;
      }

      logger::debug("In ench, call get ench");
      // ReSharper disable once CppTooWideScopeInitStatement
      const auto weapon_ench = hit_data.weapon->formEnchanting;
      logger::debug("In ench, check null and keyword on ench");
      if (!weapon_ench || !weapon_ench->HasKeyword(keyword)) {
        continue;
      }

      logger::debug("Get percent from enchantment");
      const auto percent = percent_from_enchantment(*weapon_ench, *keyword);
      if (!percent.has_value()) {
        continue;
      }
      logger::debug("Percent from enchantment: {}, start handle cast", percent.value());
      handle_cast(caster, target, *spell, eval_magnitude(*spell, percent.value()));
    }
  }
}

auto on_weapon_hit(RE::Actor& target, RE::HitData& hit_data) -> void
{
  const auto ni_aggressor = hit_data.aggressor.get();
  if (!ni_aggressor) {
    return;
  }
  const auto aggressor = ni_aggressor.get();
  if (!aggressor) {
    return;
  }

  let real_damage = Core::get_full_damage_from_hit_data(&hit_data);
  if (!real_damage.has_value()) {
    return;
  }

  handle_cast_magic_weapon_spell(*aggressor, target, real_damage.value(), hit_data);
}

} // namespace Reflyem::MagicWeapon
