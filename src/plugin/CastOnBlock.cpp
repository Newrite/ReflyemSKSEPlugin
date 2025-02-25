#include "plugin/CastOnBlock.hpp"
#include "Core.hpp"

namespace Reflyem::CastOnBlock {
auto on_weapon_hit(RE::Actor* target, const RE::HitData& hit_data, const Config& config) -> void
{
  const auto aggressor = hit_data.aggressor.get();

  if (!aggressor || target->IsDead() ||
      !(hit_data.flags.any(RE::HitData::Flag::kBlocked) || hit_data.flags.any(RE::HitData::Flag::kBlockWithWeapon))) {
    return;
  }

  auto is_magic_blocker = false;
  if (config.magic_blocker().enable() &&
      Core::try_has_absolute_keyword(target, config.magic_blocker().magic_blocker_keyword())) {
    is_magic_blocker = true;
  }

  if (config.cast_on_magic_blocker().enable() && is_magic_blocker) {
    Core::cast_on_handle_formlists(config.cast_on_magic_blocker().form_list_block_keyword(),
                                   config.cast_on_magic_blocker().form_list_block_spells(),
                                   *target, *aggressor);
  }

  if (!is_magic_blocker) {
    Core::cast_on_handle_formlists(
        config.cast_on_block().formlist_needkw(), config.cast_on_block().formlist_spells(), *target, *aggressor);
  }
}
} // namespace Reflyem::CastOnBlock
