#pragma once

#include "Config.h"
#include "AnimationEventHandler.h"

namespace reflyem
{
  namespace resource_manager
  {

    struct drain_values
    {
    public:
      float stamina;
      float health;
      float magicka;

      drain_values(float a_stamina, float a_health, float a_magicka);

      auto drain(RE::Actor& actor) -> void;
    };

    using weap_mask = std::array<std::array<std::int16_t, 3>, 1>;
    using actor_mask = std::array<std::array<std::int16_t, 3>, 3>;

    auto ranged_spend_handler() -> void;

    auto handler(
      reflyem::animation_event_handler::animation_event animation,
      RE::Actor& actor,
      bool is_power_attack,
      const reflyem::config config) -> void;

  }
}