#include "plugin/Crit.hpp"
#include "Core.hpp"

namespace Reflyem::Crit
{

auto is_critical(RE::Actor& aggressor, const RE::ActorValue crit_chance_av) -> bool
{
  const auto crit_chance = static_cast<int>(aggressor.GetActorValue(crit_chance_av));

  if (crit_chance <= 0) { return false; }
  if (crit_chance >= 100) { return true; }
  if (const auto random = Core::get_random_int(); crit_chance > random) { return true; }
  return false;
}

auto cast_on_crit(RE::Actor& aggressor, RE::Actor& target, const Config& config) -> void
{
  const auto length_kw = config.cast_on_crit().formlist_needkw()->forms.size();
  const auto length_sp = config.cast_on_crit().formlist_spells()->forms.size();

  for (std::uint32_t index = 0u; index < length_kw && index < length_sp; index++)
    {
      Core::cast_on_handle(
          config.cast_on_crit().formlist_needkw()->forms[index],
          config.cast_on_crit().formlist_spells()->forms[index],
          target,
          aggressor);
    }
}

auto crit(
    RE::Actor& aggressor,
    float& damage,
    const RE::ActorValue crit_damage_av,
    const std::int32_t crit_damage_high) -> void
{
  auto crit_damage = static_cast<int>(aggressor.GetActorValue(crit_damage_av));

  if (crit_damage <= 0) { return; }
  if (crit_damage > crit_damage_high) { crit_damage = crit_damage_high; }

  const auto damage_mult = 1 + static_cast<float>(crit_damage) / 100.f;
  damage *= damage_mult;
}

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void
{
  const auto aggressor = hit_data.aggressor.get();

  if (!aggressor || target->IsDead() ||
      Core::has_absolute_keyword(*target, *config.weapon_crit().keyword_immun()))
    {
      return;
    }

  if (is_critical(*aggressor, config.weapon_crit().chance_av()))
    {
      if (config.cast_on_crit().enable() && config.cast_on_crit().enable_weapon() &&
          hit_data.weapon)
        {
          Core::cast_on_handle(nullptr, hit_data.weapon->criticalData.effect, *target, *aggressor);
        }

      if (config.cast_on_crit().enable())
        {
          Core::cast_on_handle_formlists(
              config.cast_on_crit().formlist_needkw(),
              config.cast_on_crit().formlist_spells(),
              *aggressor,
              *target);
        }

      crit(
          *aggressor,
          hit_data.totalDamage,
          config.weapon_crit().damage_av(),
          config.weapon_crit().high());
    }
}

auto allow_magic_crit(
    const RE::ActiveEffect& active_effect,
    const Config& config,
    RE::Actor& target) -> bool
{
  if (Core::has_absolute_keyword(target, *config.magick_crit().keyword_immun())) { return false; }
  if (!active_effect.effect || !active_effect.effect->baseEffect) { return false; }
  const auto base_effect = active_effect.effect->baseEffect;
  if (config.magick_crit().must_be_or_not_be())
    {
      return base_effect->HasKeyword(config.magick_crit().mgef_keyword());
    }
  return !base_effect->HasKeyword(config.magick_crit().mgef_keyword());
}

auto modify_actor_value(
    const RE::ValueModifierEffect* this_,
    RE::Actor* actor,
    float& value,
    const RE::ActorValue av,
    const Config& config) -> void
{
  const auto is_allow_cast_on_crit = [config](const RE::FormID form_id) -> bool
  {
    if (!config.cast_on_crit().enable()) { return false; }

    auto& actors_cache = Core::ActorsCache::get_singleton();
    auto& actor_data = actors_cache.get_or_add(form_id).get();

    if (actor_data.cast_on_crit_delay() <= 0.f)
      {
        actor_data.cast_on_crit_delay(config.cast_on_crit().magick_cooldawn());
        return true;
      }

    return false;
  };

  if (Core::can_modify_actor_value(this_, actor, value, av) &&
      allow_magic_crit(*this_, config, *actor))
    {
      const auto caster = this_->GetCasterActor().get();

      value = std::abs(value);

      if (is_critical(*caster, config.magick_crit().chance_av()))
        {
          if (is_allow_cast_on_crit(caster->formID))
            {
              Core::cast_on_handle_formlists(
                  config.cast_on_crit().formlist_needkw(),
                  config.cast_on_crit().formlist_spells(),
                  *caster,
                  *actor);
            }

          crit(*caster, value, config.magick_crit().damage_av(), config.magick_crit().high());
        }

      value = -value;
    }
}

} // namespace Reflyem::Crit