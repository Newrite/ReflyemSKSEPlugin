#include "plugin/Crit.hpp"
#include "Core.hpp"

namespace Reflyem {
namespace Crit {

auto crit(RE::Actor& aggressor, RE::Actor& target, float& damage,
          const RE::ActorValue crit_chance_av, const RE::ActorValue crit_damage_av,
          const std::int32_t crit_damage_high, const bool physical_crit, const Config& config)
    -> void {
  auto crit_chance = static_cast<int>(aggressor.GetActorValue(crit_chance_av));
  auto crit_damage = static_cast<int>(aggressor.GetActorValue(crit_damage_av));

  if (crit_chance <= 0 || crit_damage <= 0) {
    return;
  }
  if (crit_chance > 100) {
    crit_chance = 100;
  }
  if (crit_damage > crit_damage_high) {
    crit_damage = crit_damage_high;
  }

  const auto random = Core::get_random_int();

  if (crit_chance > random) {
    const auto damage_mult = 1 + (crit_damage / 100.f);
    damage *= damage_mult;

    if (physical_crit && config.cast_on_crit_enable && config.cast_on_crit_formlist_needkw &&
        config.cast_on_crit_formlist_spells) {
      const auto length_kw = config.cast_on_crit_formlist_needkw->forms.size();
      const auto length_sp = config.cast_on_crit_formlist_spells->forms.size();
      for (std::uint32_t index = 0u; index < length_kw && index < length_sp; index++) {
        Core::cast_on_handle(config.cast_on_crit_formlist_needkw->forms[index],
                             config.cast_on_crit_formlist_spells->forms[index], target, aggressor);
      }
    }
  }
}

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void {
  const auto aggressor = hit_data.aggressor.get();

  if (!aggressor || target->IsDead()) {
    return;
  }

  crit(*aggressor, *target, hit_data.totalDamage, config.weapon_crit_chance_av,
       config.weapon_crit_damage_av, config.weapon_crit_high, true, config);
}

auto modify_actor_value(const RE::ValueModifierEffect* this_, RE::Actor* actor, float& value,
                        const RE::ActorValue av, const Config& config) -> void {
  if (Core::can_modify_actor_value(this_, actor, value, av)) {
    const auto caster = this_->GetCasterActor().get();
    value             = std::abs(value);
    crit(*caster, *actor, value, config.magic_crit_chance_av, config.magic_crit_damage_av,
         config.magic_crit_high, false, config);
    value = -value;
  }
}

} // namespace Crit
} // namespace Reflyem