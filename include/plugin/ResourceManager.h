#pragma once

#include "Config.h"
#include "AnimationEventHandler.h"

namespace Reflyem
{
  namespace ResourceManager
  {

    struct DrainValues
    {
    public:
      float stamina;
      float health;
      float magicka;

      DrainValues(float a_stamina, float a_health, float a_magicka);

      auto drain(RE::Actor& actor) -> void;
    };

    using WeapMask = std::array<std::array<std::int16_t, 3>, 1>;
    using ActorMask = std::array<std::array<std::int16_t, 3>, 3>;

    auto ranged_spend_handler() -> void;

    auto handler(
      Reflyem::AnimationEventHandler::AnimationEvent animation,
      RE::Actor& actor,
      bool is_power_attack,
      const Reflyem::Config config) -> void;

  }
}