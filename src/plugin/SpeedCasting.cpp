#include "SpeedCasting.h"

namespace reflyem
{
  namespace speed_casting
  {
    auto on_main_update(
      RE::Main* a_this,
      float a2,
      const reflyem::config& config) -> void
    {
      auto player = RE::PlayerCharacter::GetSingleton();

      if (!player) { return; }

      auto speed_casting_percent = player->GetActorValue(config.speed_casting_av);
      auto float_percent = (1.f - (speed_casting_percent / 100.f));
      if (float_percent > config.speed_casting_high)
      {
        float_percent = config.speed_casting_high;
      }
      if (float_percent < config.speed_casting_low)
      {
        float_percent = config.speed_casting_low;
      }

      config.speed_casting_global->value = float_percent;

    }
  }
}