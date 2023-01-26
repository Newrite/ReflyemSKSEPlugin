#include "SpeedCasting.hpp"

namespace Reflyem {
namespace SpeedCasting {

auto on_update_actor(RE::Character& character, float, const Config& config) -> void {

  const auto speed_casting_percent = character.GetActorValue(config.speed_casting_av);
  auto       float_percent         = (1.f - (speed_casting_percent / 100.f));
  if (float_percent > config.speed_casting_high) {
    float_percent = config.speed_casting_high;
  }
  if (float_percent < config.speed_casting_low) {
    float_percent = config.speed_casting_low;
  }

  config.speed_casting_global->value = float_percent;
}

} // namespace SpeedCasting
} // namespace Reflyem