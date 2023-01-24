#include "SpeedCasting.hpp"

namespace Reflyem {
namespace SpeedCasting {

auto on_update_actor(RE::Character& player, float, const Reflyem::Config& config) -> void {

  auto speed_casting_percent = player.GetActorValue(config.speed_casting_av);
  auto float_percent         = (1.f - (speed_casting_percent / 100.f));
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