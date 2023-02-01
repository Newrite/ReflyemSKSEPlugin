#include "Config.hpp"
#include "Core.hpp"

namespace Reflyem::MagicWeapon {

auto eval_percent(const float value) -> float {
  if (value > 100.f) {
    return 1.f;
  }

  if (value < 0.f) {
    return 0.f;
  }

  return 1.f - (value / 100.f);
};

auto percent_from_enchantment(RE::EnchantmentItem& weapon_ench, const RE::BGSKeyword& keyword)
  -> std::optional<float> {
  for (const auto& effect : weapon_ench.effects) {

    if (effect && effect->baseEffect && effect->baseEffect->HasKeyword(&keyword)) {
      return {eval_percent(effect->effectItem.magnitude)};
    }
  }
  return std::nullopt;
}

auto handle_cast(RE::Actor&  caster, RE::Actor& target, RE::SpellItem& spell,
                 const float magnitude)
  -> void {
  for (const auto& effect : spell.effects) {
    if (effect) {
      effect->effectItem.magnitude = magnitude;
    }
  }
  Core::cast(spell, target, caster);
};

auto handle_cast_magic_weapon_spell(RE::Actor&   caster, RE::Actor& target, float real_damage,
                                    RE::HitData& hit_data) -> void {

  const auto& config = Config::get_singleton();

  const auto spells   = config.magic_weapon().spells();
  const auto keywords = config.magic_weapon().keywords();
  const auto globals  = config.magic_weapon().globals();

  const auto spells_size = spells->forms.size();

  if (spells_size != globals->forms.size() || spells_size != keywords->forms.size()) {
    return;
  }

  const auto total_damage          = real_damage;
  const auto physical_total_damage = hit_data.totalDamage;

  const auto eval_magnitude = [&](const RE::SpellItem& spell, const float percent) -> float {

    auto magnitude = total_damage * percent;

    if (real_damage >= magnitude) {
      real_damage = real_damage - magnitude;
    } else {
      magnitude   = real_damage;
      real_damage = 0.f;
    }

    if (const auto minus_damage = physical_total_damage * percent;
      hit_data.totalDamage >= minus_damage) {
      hit_data.totalDamage = hit_data.totalDamage - minus_damage;
    } else {
      hit_data.totalDamage = 0.f;
    }

    const auto mult = [&]() -> float {
      if (1 == spell.boundData.boundMax.x == spell.boundData.boundMax.y == spell.boundData.boundMax.
          z == spell.boundData.boundMin.x == spell.boundData.boundMin.y == spell.boundData.boundMin.
          z) {
        logger::info("magic weapon:: mult count of spell");
        return static_cast<float>(spell.effects.size());
      }
      logger::info("magic weapon:: one count of spell");
      return 1.f;
    }();

    return magnitude * mult;
  };

  for (uint32_t index = 0; index < spells_size; index++) {

    if (real_damage <= 0.f || hit_data.totalDamage <= 0.f) {
      return;
    }

    const auto spell   = spells->forms[index]->As<RE::SpellItem>();
    const auto global  = globals->forms[index]->As<RE::TESGlobal>();
    const auto keyword = keywords->forms[index]->As<RE::BGSKeyword>();

    if (!spell || !keyword) {
      continue;
    }

    if (spell->data.delivery != RE::MagicSystem::Delivery::kTouch) {
      continue;
    }

    if (global) {

      if (!Core::has_absolute_keyword(caster, *keyword)) {
        continue;
      }

      handle_cast(caster, target, *spell, eval_magnitude(*spell, eval_percent(global->value)));

    } else {
      // ReSharper disable once CppTooWideScopeInitStatement
      const auto weapon_ench = hit_data.weapon->formEnchanting;
      if (!weapon_ench && !weapon_ench->HasKeyword(keyword)) {
        continue;
      }

      const auto percent = percent_from_enchantment(*weapon_ench, *keyword);
      if (!percent.has_value()) {
        continue;
      }
      handle_cast(caster, target, *spell, eval_magnitude(*spell, percent.value()));
    }
  }
}

auto on_weapon_hit(RE::Actor& target, RE::HitData& hit_data) -> void {
  const auto ni_aggressor = hit_data.aggressor.get();
  if (!ni_aggressor) {
    return;
  }
  const auto aggressor = ni_aggressor.get();
  if (!aggressor) {
    return;
  }

  auto damage_resist = 1.f;
  RE::BGSEntryPoint::HandleEntryPoint(RE::BGSEntryPoint::ENTRY_POINT::kModIncomingDamage, &target,
                                      hit_data.weapon, aggressor, std::addressof(damage_resist));

  const auto real_damage = hit_data.physicalDamage * damage_resist;

  handle_cast_magic_weapon_spell(*aggressor, target, real_damage, hit_data);
}

} // namespace Reflyem::MagicWeapon
