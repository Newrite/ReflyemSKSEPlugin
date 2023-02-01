#include "plugin/MagicResistRescaled.hpp"

namespace Reflyem::MagicResistRescaled {
auto check_resistance(RE::MagicTarget&            this_, RE::MagicItem& magic_item,
                      const RE::Effect&         effect,
                      const RE::TESBoundObject* bound_object, const Config&) -> float {

  logger::debug("Check resist"sv);
  if (magic_item.hostileCount <= 0 || bound_object && bound_object->formType ==
      RE::FormType::Armor) {
    logger::debug("Non hostile"sv);
    return 1.f;
  }

  const auto alchemy_item = magic_item.As<RE::AlchemyItem>();

  if (alchemy_item &&
      ((!alchemy_item->IsPoison() && !effect.IsHostile()) || (alchemy_item->GetSpellType() ==
      RE::MagicSystem::SpellType::kIngredient && alchemy_item->IsFood()))) {
    logger::debug("alchemy item non poison"sv);
    return 1.f;
  }

  if (!effect.baseEffect) {
    logger::debug("Base effect null"sv);
    return 1.f;
  }

  const auto resist_av = effect.baseEffect->data.resistVariable;
  if (resist_av == RE::ActorValue::kNone) {
    logger::debug("Resist kNone"sv);
    return 1.f;
  }

  // ReSharper disable once CppCStyleCast  // NOLINT(clang-diagnostic-cast-align)
  const auto actor = (RE::Actor*)(&this_ - 0x98);
  if (!actor) {
    logger::debug("Actor is null");
    return 1.f;
  }

  const auto resist_value   = actor->GetActorValue(resist_av);
  auto       resist_percent = 1.f - (resist_value / 100.f);
  if (resist_value >= 100.f) {
    resist_percent = 0.f;
  }
  if (resist_value <= 0.f) {
    resist_percent = 1.f;
  }

  logger::debug("ResistAV: {} ResistPercent: {}"sv, static_cast<int32_t>(resist_av), resist_percent);

  return resist_percent;

}
}
