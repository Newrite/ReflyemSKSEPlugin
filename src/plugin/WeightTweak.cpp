#include "plugin/WeightTweak.hpp"

namespace Reflyem::WeightTweak {
auto get_actor_value(RE::ActorValueOwner& this_, const RE::ActorValue av) -> std::optional<float> {
  // ReSharper disable once CppCStyleCast
  const auto actor = (RE::Actor*)((char*)&this_ - 0xB0);
  if (!actor) {
    return std::nullopt;
  }
  if (av == RE::ActorValue::kInventoryWeight) {
    return {actor->equippedWeight};
  }
  return std::nullopt;
}

auto set_actor_value(RE::ActorValueOwner& this_, const RE::ActorValue av, const float value) -> float {
  // ReSharper disable once CppCStyleCast
  const auto actor = (RE::Actor*)((char*)&this_ - 0xB0);
  if (!actor) {
    return value;
  }
  if (av == RE::ActorValue::kInventoryWeight) {
    return actor->equippedWeight;
  }
  return value;
}

auto mod_actor_value(RE::ActorValueOwner& this_, const RE::ActorValue av, const float value) -> float {
  // ReSharper disable once CppCStyleCast
  const auto actor = (RE::Actor*)((char*)&this_ - 0xB0);
  if (!actor) {
    return value;
  }
  if (av == RE::ActorValue::kInventoryWeight) {
    return 0.f;
  }
  return value;
}
}