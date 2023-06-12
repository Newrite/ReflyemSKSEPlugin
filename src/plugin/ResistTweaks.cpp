#include "plugin/ResistTweaks.hpp"

#include "Core.hpp"

namespace Reflyem::ResistTweaks
{

namespace EnchFlag
{
enum : int16_t
{
  kNone = 0,
  kIgnoreResistance = 1,
  kNoAbsorb = 2,
  kTotal = 3,
};
}

auto get_no_absorb(const RE::MagicItem& this_) -> bool
{
  return Core::bound_data_comparer(this_.boundData, EnchFlag::kNoAbsorb) ||
         Core::bound_data_comparer(this_.boundData, EnchFlag::kTotal);
}

auto ignores_resistance(const RE::MagicItem& this_) -> bool
{
  return Core::bound_data_comparer(this_.boundData, EnchFlag::kIgnoreResistance) ||
         Core::bound_data_comparer(this_.boundData, EnchFlag::kTotal);
}

auto resist_value_after_penetration(RE::Actor* value_owner, const RE::ActorValue resist_av) -> float
{
  const auto& config = Config::get_singleton();
  const auto resist_value = value_owner->GetActorValue(resist_av);

  if (!config.resist_tweaks().enable_resist_penetration()) { return resist_value; }

  auto penetration_value = value_owner->GetActorValue(config.resist_tweaks().penetration_av());

  if (resist_av == RE::ActorValue::kDamageResist &&
      config.resist_tweaks().enable_damage_resist_tweak() &&
      config.resist_tweaks().no_av_damage_resist_penetration())
    {
      penetration_value = 0.f;
    }

  const auto active_effects_with_keyword =
      Core::get_effects_by_keyword(*value_owner, *config.resist_tweaks().penetration_kw());

  for (const auto active_effect : active_effects_with_keyword)
    {
      if (Core::get_second_av(*active_effect) != resist_av) { continue; }

      if (active_effect->effect->baseEffect->data.flags.any(
              RE::EffectSetting::EffectSettingData::Flag::kDetrimental))
        {
          penetration_value -= active_effect->magnitude;
          continue;
        }

      penetration_value += active_effect->magnitude;
    }

  if (static_cast<int32_t>(penetration_value) > config.resist_tweaks().penetration_high())
    {
      penetration_value = static_cast<float>(config.resist_tweaks().penetration_high());
    }

  if (config.resist_tweaks().flat_penetration()) { return resist_value - penetration_value; }

  if (penetration_value <= 0.f) { return resist_value; }

  if (penetration_value >= 100.f) { return 0.f; }

  return resist_value * (1 - (penetration_value / 100.f));
};

auto check_resistance(
    RE::MagicTarget& this_,
    RE::MagicItem& magic_item,
    const RE::Effect& effect,
    const RE::TESBoundObject* bound_object,
    const Config& config) -> float
{
  logger::debug("Check resist"sv);
  if (magic_item.hostileCount <= 0 || bound_object && bound_object->formType == RE::FormType::Armor)
    {
      logger::debug("Non hostile"sv);
      return 1.f;
    }

  const auto alchemy_item = magic_item.As<RE::AlchemyItem>();

  if (alchemy_item && ((!alchemy_item->IsPoison() && !effect.IsHostile()) ||
                       (alchemy_item->GetSpellType() == RE::MagicSystem::SpellType::kIngredient &&
                        alchemy_item->IsFood())))
    {
      logger::debug("alchemy item non poison"sv);
      return 1.f;
    }

  if (!effect.baseEffect)
    {
      logger::debug("Base effect null"sv);
      return 1.f;
    }

  auto resist_av = effect.baseEffect->data.resistVariable;

  // ReSharper disable once CppCStyleCast  // NOLINT(clang-diagnostic-cast-align)
  const auto actor = (RE::Actor*)((char*)&this_ - 0x98);
  if (!actor)
    {
      logger::debug("Actor is null");
      return 1.f;
    }

  auto max_resist = []() -> float
  {
    const auto resist =
        RE::GameSettingCollection::GetSingleton()->GetSetting("fPlayerMaxResistance")->GetFloat();
    if (resist > 100.f) { return 0.f; }
    if (resist <= 0.f) { return 1.f; }
    return 1 - (resist * 0.01f);
  }();

  if (!config.resist_tweaks().npc_max_resistance() && !actor->IsPlayerRef()) { max_resist = 0.f; }

  const auto high_cap = 1.f / config.resist_tweaks().resist_weight();
  constexpr auto low_cap = 0.f;

  auto second_resist_percent = [&](RE::Actor* value_owner, const RE::MagicItem& item) -> float
  {
    const auto second_resist_av = [&magic_item]() -> RE::ActorValue
    {
      if (magic_item.IsPoison()) { return RE::ActorValue::kPoisonResist; }
      return RE::ActorValue::kResistMagic;
    }();

    if (magic_item.IsPoison() && config.resist_tweaks().no_double_resist_check_poison() && resist_av != RE::ActorValue::kNone)
    {
      return 1.f;
    }

    if (config.resist_tweaks().no_double_resist_check_magick() && resist_av != RE::ActorValue::kNone)
    {
      return 1.f;
    }

    // if (item.IsPoison())
// 
    //   if (item.IsPoison())
    //     {
    //       if (config.resist_tweaks().no_double_resist_check_poison() && true &&
    //           resist_av == RE::ActorValue::kNone)
    //         {
    //           second_resist_av = RE::ActorValue::kPoisonResist;
    //         }
    //       if (config.resist_tweaks().no_double_resist_check_poison()) { return 1.f; }
    //       second_resist_av = RE::ActorValue::kPoisonResist;
    //     }
// 
    // if (config.resist_tweaks().no_double_resist_check_magick() &&
    //     second_resist_av == RE::ActorValue::kResistMagic)
    //   {
    //     if (true && resist_av == RE::ActorValue::kNone)
    //       {
    //         resist_av = RE::ActorValue::kResistMagic;
    //       }
    //     return 1.f;
    //   }

    // if (resist_av == second_resist_av) { return 1.f; }

    const auto second_resist_value = resist_value_after_penetration(value_owner, second_resist_av);
    if (second_resist_value <= low_cap) { return 1.f; }
    if (second_resist_value >= high_cap) { return max_resist; }

    return 1.f - (second_resist_value * config.resist_tweaks().resist_weight());
  }(actor, magic_item);

  if (second_resist_percent < max_resist) { second_resist_percent = max_resist; }

  if (config.resist_tweaks().enable_damage_resist_tweak() &&
      resist_av == RE::ActorValue::kDamageResist)
    {
      if (config.resist_tweaks().no_double_damage_resist_check()) { second_resist_percent = 1.f; }

      const auto armor_scaling =
          RE::GameSettingCollection::GetSingleton()->GetSetting("fArmorScalingFactor")->GetFloat();

      const auto damage_resist_high_cap = 100.f / armor_scaling;

      const auto resist_value = resist_value_after_penetration(actor, resist_av);
      if (resist_value <= low_cap) { return 1.f * second_resist_percent; }
      if (resist_value >= damage_resist_high_cap) { return max_resist * second_resist_percent; }

      auto resist_percent = 1.f - (resist_value * armor_scaling / 100);
      if (resist_percent < max_resist) { resist_percent = max_resist; }

      return resist_percent * second_resist_percent;
    }

  auto resist_value = 0.f;
  if (resist_av != RE::ActorValue::kNone)
    {
      resist_value = resist_value_after_penetration(actor, resist_av);
    }
  if (resist_value <= low_cap) { return 1.f * second_resist_percent; }
  if (resist_value >= high_cap) { return max_resist * second_resist_percent; }

  auto resist_percent = 1.f - (resist_value * config.resist_tweaks().resist_weight());
  if (resist_percent < max_resist) { resist_percent = max_resist; }

  return resist_percent * second_resist_percent;
}
} // namespace Reflyem::ResistTweaks