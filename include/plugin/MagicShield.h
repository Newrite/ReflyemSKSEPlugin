#pragma once

#include "Config.h"

namespace Reflyem
{
  namespace MagicShield
  {
    auto on_weapon_hit(
      RE::Actor* target,
      RE::HitData& hit_data,
      const Reflyem::Config& config) -> void;

    auto modify_actor_value(
      RE::ValueModifierEffect* a_this,
      RE::Actor* a_actor, float& a_value,
      RE::ActorValue a_actorValue,
      const Reflyem::Config& config) -> void;
  }
}