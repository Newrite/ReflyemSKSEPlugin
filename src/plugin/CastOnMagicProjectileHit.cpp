#include "plugin/CastOnMagicProjectileHit.hpp"
#include "Core.hpp"

namespace Reflyem::CastOnMagicProjectileHit {

auto on_magic_hit(RE::MagicCaster* magic_caster,
                  RE::NiPoint3*,
                  RE::Projectile* projectile,
                  RE::TESObjectREFR* refr_target,
                  float,
                  float,
                  bool,
                  bool) -> void
{

  if (!magic_caster || !projectile || !refr_target) {
    logd("Cast OnMagicHit: some null, false");
    return;
  }

  if (!projectile->spell) {
    return;
  }

  if (refr_target->IsDead()) {
    return;
  }

  let refr_actor = refr_target->As<RE::Actor>();
  if (!refr_actor) {
    return;
  }

  let shooter = magic_caster->GetCasterAsActor();
  if (shooter && shooter->GetFormID() == refr_target->GetFormID()) {
    logd("Cast OnMagicHit: target is caster");
    return;
  }

  auto is_concentration_spell = false;
  if (!projectile->weaponSource && projectile->spell) {
    if (projectile->spell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration) {
      is_concentration_spell = true;
    }
  }

  auto power_mult = 1.f;
  if (is_concentration_spell && projectile->GetProjectileBase()) {
    power_mult = projectile->GetProjectileBase()->data.relaunchInterval;
  }

  if (power_mult > 1.f) {
    power_mult = 1.f;
  }

  letr config = Config::get_singleton();

  Core::cast_on_handle_formlists(config.cast_on_magic_projectile_hit().formlist_needkw(),
                                 config.cast_on_magic_projectile_hit().formlist_spells(),
                                 magic_caster->GetCasterAsActor(),
                                 refr_target,
                                 Core::kNone,
                                 power_mult);
}
} // namespace Reflyem::CastOnKill
