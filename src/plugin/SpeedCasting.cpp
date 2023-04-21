#include "plugin/SpeedCasting.hpp"

namespace Reflyem::SpeedCasting
{

auto on_update_actor(RE::Character& character, float, const Config& config) -> void
{
  const auto speed_casting_percent = character.GetActorValue(config.speed_casting().av());
  auto float_percent = (1.f - (speed_casting_percent / 100.f));
  if (float_percent > config.speed_casting().high())
    {
      float_percent = config.speed_casting().high();
    }
  if (float_percent < config.speed_casting().low())
    {
      float_percent = config.speed_casting().low();
    }

  config.speed_casting().global()->value = float_percent;
}

} // namespace Reflyem::SpeedCasting