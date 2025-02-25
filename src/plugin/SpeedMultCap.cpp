#include "plugin/SpeedMultCap.hpp"
#include "Config.hpp"
#include "Core.hpp"

namespace Reflyem::SpeedMultCap
{


auto get_max_speed_mult(RE::Actor* actor, const Config& config) -> float
{
  if (!actor)
    {
      logi("Null actor in eval max speed mult"sv);
      return 100.f;
    }

  let max_speed_mult_effects_mutate_sum =
      Core::get_effects_magnitude_sum(Core::try_get_effects_by_keyword(
                                          actor,
                                          config.speed_mult_cap().effect_mutate_cap()))
          .value_or(0.f);


  return config.speed_mult_cap().cap_base() + max_speed_mult_effects_mutate_sum;
}

auto eval_speed_mult(RE::Actor* actor, const float speed_mult_value) -> std::optional<float>
{
  if (!actor)
    {
      logi("Null actor or function in eval speed mult"sv);
      return std::nullopt;
    }

  letr config = Config::get_singleton();

  if (config.speed_mult_cap().exclusive_keyword() &&
      actor->HasKeyword(config.speed_mult_cap().exclusive_keyword()))
    {
      return std::nullopt;
    }

  let max_speed_mult_effects_allow_overcap_sum = [&]
  {
    let value =
        Core::get_effects_magnitude_sum(Core::try_get_effects_by_keyword(
                                            actor,
                                            config.speed_mult_cap().effect_allow_overcap()))
            .value_or(0.f);

    if (value < 0.f) { return 0.f; }

    return value;
  }();

  let max_speed_mult = get_max_speed_mult(actor, config);

  float current_speed_mult = speed_mult_value - max_speed_mult_effects_allow_overcap_sum;

  if (current_speed_mult > max_speed_mult)
    {
      return {max_speed_mult + max_speed_mult_effects_allow_overcap_sum};
    }

  return {current_speed_mult + max_speed_mult_effects_allow_overcap_sum};
}

// auto eval_value_for_mod(RE::Actor* actor, const float mod_value) -> float
// {
//   if (!actor)
//     {
//       logi("Null actor in eval value for mod"sv);
//       return mod_value;
//     }
//
//   letr config = Config::get_singleton();
//   let max_speed_mult = get_max_speed_mult(actor, config);
//
//   let a = max_speed_mult - (actor->GetActorValue(RE::ActorValue::kSpeedMult) + mod_value);
//   if (a > 0.f) { return mod_value; }
//
//   return mod_value;
// }

auto get_actor_value(RE::ActorValueOwner* this_, const float speed_mult_value)
    -> std::optional<float>
{
  return {eval_speed_mult(Core::get_actor_value_owner_as_actor(this_), speed_mult_value)};
}

// auto set_actor_value(RE::ActorValueOwner& this_, const RE::ActorValue av, const float value)
//     -> float
// {
//   if (av == RE::ActorValue::kSpeedMult)
//     {
//       return eval_speed_mult(Core::get_actor_value_owner_as_actor(&this_));
//     }
//   return value;
// }

// auto mod_actor_value(RE::ActorValueOwner& this_, const RE::ActorValue av, const float value)
//     -> float
// {
//   if (av == RE::ActorValue::kInventoryWeight)
//     {
//       return eval_value_for_mod(Core::get_actor_value_owner_as_actor(&this_), value);
//     }
//   return value;
// }
}