#include "plugin/EquipLoad.hpp"
#include "Core.hpp"

namespace Reflyem::EquipLoad
{

auto eval_equip_load(const RE::Actor& actor) -> float
{
  const auto& config = Config::get_singleton();

  const auto right_equipped_weapon = Core::get_weapon(actor, false, nullptr);
  const auto left_equipped_weapon = Core::get_weapon(actor, true, nullptr);

  const auto weapon_weight = [&]() -> float
  {
    auto weight = 0.f;
    if (right_equipped_weapon) { weight += right_equipped_weapon->weight; }
    if (left_equipped_weapon) { weight += left_equipped_weapon->weight; }
    return weight;
  }();

  const auto armor_weight = actor.equippedWeight - weapon_weight;

  return config.equip_load().equip_weight_mult() *
         ((armor_weight * config.equip_load().armor_weight_mult()) +
          (weapon_weight * config.equip_load().weapon_weight_mult()));
}

auto update_actor(RE::Actor& actor, const Config& config) -> void
{
  if (!config.equip_load().low_equip_spell() || !config.equip_load().med_equip_spell() ||
      !config.equip_load().hig_equip_spell())
    {
      return;
    }
  auto equip_weight = actor.GetActorValue(RE::ActorValue::kInventoryWeight);
  if (equip_weight < 0.f) { equip_weight = 0.f; }

  auto carry_weight = actor.GetActorValue(RE::ActorValue::kCarryWeight);
  if (carry_weight < 0.f) { carry_weight = 0.f; }

  if (equip_weight >= carry_weight)
    {
      actor.AddSpell(config.equip_load().hig_equip_spell());
      actor.RemoveSpell(config.equip_load().low_equip_spell());
      actor.RemoveSpell(config.equip_load().med_equip_spell());
      return;
    }

  const auto load_percent = equip_weight / (carry_weight / 100.f);
  if (load_percent <= config.equip_load().low_percent())
    {
      actor.AddSpell(config.equip_load().low_equip_spell());
      actor.RemoveSpell(config.equip_load().hig_equip_spell());
      actor.RemoveSpell(config.equip_load().med_equip_spell());
      return;
    }
  if (load_percent > config.equip_load().high_percent())
    {
      actor.AddSpell(config.equip_load().hig_equip_spell());
      actor.RemoveSpell(config.equip_load().low_equip_spell());
      actor.RemoveSpell(config.equip_load().med_equip_spell());
      return;
    }

  actor.AddSpell(config.equip_load().med_equip_spell());
  actor.RemoveSpell(config.equip_load().low_equip_spell());
  actor.RemoveSpell(config.equip_load().hig_equip_spell());
}

auto get_actor_value(RE::ActorValueOwner& this_, const RE::ActorValue av) -> std::optional<float>
{
  // ReSharper disable once CppCStyleCast
  const auto actor = (RE::Actor*)((char*)&this_ - 0xB0);
  if (!actor) { return std::nullopt; }
  if (av == RE::ActorValue::kInventoryWeight) { return {eval_equip_load(*actor)}; }
  return std::nullopt;
}

auto set_actor_value(RE::ActorValueOwner& this_, const RE::ActorValue av, const float value)
    -> float
{
  // ReSharper disable once CppCStyleCast
  const auto actor = (RE::Actor*)((char*)&this_ - 0xB0);
  if (!actor) { return value; }
  if (av == RE::ActorValue::kInventoryWeight) { return {eval_equip_load(*actor)}; }
  return value;
}

auto mod_actor_value(RE::ActorValueOwner& this_, const RE::ActorValue av, const float value)
    -> float
{
  // ReSharper disable once CppCStyleCast
  const auto actor = (RE::Actor*)((char*)&this_ - 0xB0);
  if (!actor) { return value; }
  if (av == RE::ActorValue::kInventoryWeight) { return 0.f; }
  return value;
}
} // namespace Reflyem::EquipLoad