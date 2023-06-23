#include "plugin/SoulLink.hpp"
#include "Core.hpp"

namespace Reflyem::SoulLink
{

auto valid_commanded_actor(const RE::Actor* commander, RE::Actor* commanded, const Config& config)
    -> bool
{
  if (!commander || !commanded) { return false; }

  return !Core::try_has_absolute_keyword(commanded, config.soul_link().exclusive_keyword()) &&
         commanded != commander;
}

auto soul_link_commander_actor(RE::Actor* target, float* damage, const Config& config) -> void
{
  if (!target || !damage) { return; }

  let soul_link_percent = [&]() -> float
  {
    let av = target->GetActorValue(config.soul_link().av());
    if (av >= 100.f) { return 100.f; }
    if (av <= 0.f) { return 0.f; }
    return av;
  }();

  if (soul_link_percent <= 0.f) { return; }
  if (*damage <= 0.f) { return; }

  let commanded_actors = Core::get_commanded_actors(target);
  if (!commanded_actors) { return; }

  auto valid_commanded_actors = std::vector<RE::Actor*>();

  for (auto& commanded_data : *commanded_actors)
    {
      let commanded_actor = Core::actor_from_actor_handle(&commanded_data.commandedActor);
      if (valid_commanded_actor(target, commanded_actor, config))
        {
          valid_commanded_actors.push_back(commanded_actor);
          logger::info("COM:Add valid actor to vector {}"sv, commanded_actor->GetDisplayFullName());
        }
    }

  if (valid_commanded_actors.empty())
    {
      logger::info("COM:Vector of valid actors is empty"sv);
      return;
    }

  let damage_after = *damage * (1.f - (soul_link_percent / 100.f));
  let split_damage = (*damage - damage_after) / static_cast<float>(valid_commanded_actors.size());
  logger::info(
      "COM:Damage Before {} Damage After {} Split Damage {} SizeVector {} SoulLinkPercent {}"sv,
      *damage,
      damage_after,
      split_damage,
      valid_commanded_actors.size(),
      soul_link_percent);
  *damage = damage_after;

  for (const auto commanded_actor : valid_commanded_actors)
    {
      logger::info("COM:Damage commanded actor {}"sv, commanded_actor->GetDisplayFullName());
      Core::damage_actor_value(
          *commanded_actor,
          RE::ActorValue::kHealth,
          split_damage * Core::getting_damage_mult(*commanded_actor));
    }
}

auto soul_link_commanded_actor(RE::Actor* target, float* damage, const Config& config) -> void
{
  if (!target || !damage) { return; }

  if (!target->IsCommandedActor() ||
      Core::try_has_absolute_keyword(target, config.soul_link().exclusive_keyword()) ||
      *damage <= 0.f)
    {
      return;
    }

  let commander = Core::get_commander_actor(target);
  if (!commander ||
      !Core::try_has_absolute_keyword(commander, config.soul_link().summons_split_keyword()))
    {
      return;
    }

  let commanded_actors = Core::get_commanded_actors(commander);
  if (!commanded_actors) { return; }

  auto valid_commanded_actors = std::vector<RE::Actor*>();

  for (auto& commanded_data : *commanded_actors)
    {
      let commanded_actor = Core::actor_from_actor_handle(&commanded_data.commandedActor);
      if (valid_commanded_actor(target, commanded_actor, config))
        {
          logger::info("SUM:Add valid actor to vector {}"sv, commanded_actor->GetDisplayFullName());
          valid_commanded_actors.push_back(commanded_actor);
        }
    }

  if (valid_commanded_actors.empty())
    {
      logger::info("SUM:Vector of valid actors is empty"sv);
      return;
    }

  let split_damage = *damage / static_cast<float>(valid_commanded_actors.size());
  *damage = split_damage;

  logger::info(
      "SUM:Damage {} Split Damage {} SizeVector {}"sv,
      *damage,
      split_damage,
      valid_commanded_actors.size());

  for (auto commanded_actor : valid_commanded_actors)
    {
      logger::info("SUM:Damage commanded actor {}"sv, commanded_actor->GetDisplayFullName());
      Core::damage_actor_value(
          *commanded_actor,
          RE::ActorValue::kHealth,
          split_damage * Core::getting_damage_mult(*commanded_actor));
    }
}

auto allow_magic_shield_effect(const RE::ActiveEffect& active_effect, const Config& config) -> bool
{
  if (!active_effect.effect || !active_effect.effect->baseEffect) { return false; }
  const auto base_effect = active_effect.effect->baseEffect;

  if (!config.soul_link().exclusive_keyword()) { return true; }

  return !base_effect->HasKeyword(config.soul_link().exclusive_keyword());
}

auto modify_actor_value(
    const RE::ValueModifierEffect* this_,
    RE::Actor* actor,
    float* value,
    const RE::ActorValue av,
    const Config& config) -> void
{
  if (Core::can_modify_actor_value(this_, actor, *value, av) &&
      allow_magic_shield_effect(*this_, config))
    {
      logger::info("soul link value before: {}"sv, *value);
      *value = std::abs(*value);
      soul_link_commanded_actor(actor, value, config);
      soul_link_commander_actor(actor, value, config);
      *value = -*value;
      logger::info("soul link value after: {}"sv, *value);
    }
}

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void
{
  if (!target) { return; }

  if (hit_data.weapon && config.soul_link().exclusive_keyword() &&
      hit_data.weapon->HasKeyword(config.soul_link().exclusive_keyword()))
    {
      return;
    }
  
  soul_link_commanded_actor(target, std::addressof(hit_data.totalDamage), config);
  soul_link_commander_actor(target, std::addressof(hit_data.totalDamage), config);
}
} // namespace Reflyem::SoulLink