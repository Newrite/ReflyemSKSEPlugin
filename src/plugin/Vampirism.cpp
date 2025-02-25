#include "plugin/Vampirism.hpp"
#include "Core.hpp"

namespace Reflyem::Vampirism {

auto vampirism(RE::Actor& target,
               RE::Actor& aggressor,
               const float damage_value,
               const RE::ActorValue av,
               float vampirism_percent,
               const bool leech_vampirism) -> void
{
  if (vampirism_percent <= 0.f) {
    return;
  }

  if (vampirism_percent > 100.f) {
    vampirism_percent = 100.f;
  }

  auto damage_mult = Core::getting_damage_mult(target);
  if (av != RE::ActorValue::kHealth) {
    damage_mult = 1.f;
  }

  const auto target_value = static_cast<double>(target.GetActorValue(RE::ActorValue::kHealth));
  const auto vampirism_value = (static_cast<double>(damage_value) * static_cast<double>(damage_mult)) *
                               static_cast<double>((vampirism_percent / 100.f));

  auto& actor_data = Core::ActorsCache::get_or_add_actor_data(aggressor.formID);

  if (vampirism_value > target_value) {
    if (leech_vampirism) {
      actor_data.mod_vampirism_accumulator(target_value);
    } else {
      Core::restore_actor_value(aggressor, av, static_cast<float>(target_value));
    }
  } else {
    if (leech_vampirism) {
      actor_data.mod_vampirism_accumulator(vampirism_value);
    } else {
      Core::restore_actor_value(aggressor, av, static_cast<float>(vampirism_value));
    }
  }
}

auto av_vampirism(RE::Actor& target,
                  RE::Actor& aggressor,
                  const float& damage_value,
                  const RE::ActorValue vampirism_av,
                  const bool leech_vampirism) -> void
{
  const auto vampirism_percent = aggressor.GetActorValue(vampirism_av);
  vampirism(target, aggressor, damage_value, RE::ActorValue::kHealth, vampirism_percent, leech_vampirism);
}

auto mgef_vampirism(RE::Actor& target,
                    RE::Actor& aggressor,
                    const float damage_value,
                    const RE::BGSKeyword* key,
                    const RE::ActorValue av,
                    const bool leech_vampirism) -> void
{
  const auto effects = Core::try_get_effects_by_keyword(&aggressor, key);
  const auto vampirism_percent = Core::get_effects_magnitude_sum(effects).value_or(0.f);

  vampirism(target, aggressor, damage_value, av, vampirism_percent, leech_vampirism);
}

auto allow_vampirism_effect(const RE::ActiveEffect& active_effect, const Config& config) -> bool
{
  if (!active_effect.effect || !active_effect.effect->baseEffect) {
    return false;
  }
  const auto base_effect = active_effect.effect->baseEffect;
  if (config.magic_vampirism().must_be_or_not_be()) {
    return Core::try_form_has_keyword(base_effect, config.magic_vampirism().mgef_keyword());
  }
  return !Core::try_form_has_keyword(base_effect, config.magic_vampirism().mgef_keyword());
}

auto modify_actor_value(const RE::ValueModifierEffect* this_,
                        RE::Actor* actor,
                        float value,
                        const RE::ActorValue av,
                        const Config& config) -> void
{
  if (Core::can_modify_actor_value(this_, actor, value, av)) {
    const auto aggressor = this_->GetCasterActor().get();

    if (!aggressor) {
      return;
    }

    value = std::abs(value);

    if (config.magic_vampirism().enable() && allow_vampirism_effect(*this_, config)) {
      av_vampirism(
          *actor, *aggressor, value, config.magic_vampirism().av(), config.magic_vampirism().leech_vampirism());
    }

    if (config.magic_vampirism().mgef_health_enable() && allow_vampirism_effect(*this_, config)) {
      mgef_vampirism(*actor,
                     *aggressor,
                     value,
                     config.magic_vampirism().mgef_health_keyword(),
                     RE::ActorValue::kHealth,
                     config.magic_vampirism().leech_vampirism());
    }

    if (config.magic_vampirism().mgef_stamina_enable() && allow_vampirism_effect(*this_, config)) {
      mgef_vampirism(*actor,
                     *aggressor,
                     value,
                     config.magic_vampirism().mgef_stamina_keyword(),
                     RE::ActorValue::kStamina,
                     false);
    }

    if (config.magic_vampirism().mgef_magicka_enable() && allow_vampirism_effect(*this_, config)) {
      mgef_vampirism(*actor,
                     *aggressor,
                     value,
                     config.magic_vampirism().mgef_magicka_keyword(),
                     RE::ActorValue::kMagicka,
                     false);
    }
  }
}

auto on_weapon_hit(RE::Actor* target, const RE::HitData& hit_data, const Config& config) -> void
{
  const auto aggressor = hit_data.aggressor.get();

  if (!aggressor || target->IsDead()) {
    return;
  }

  if (config.vampirism().enable()) {
    av_vampirism(
        *target, *aggressor, hit_data.totalDamage, config.vampirism().av(), config.vampirism().leech_vampirism());
  }

  if (config.vampirism().mgef_health_enable()) {
    mgef_vampirism(*target,
                   *aggressor,
                   hit_data.totalDamage,
                   config.vampirism().mgef_health_keyword(),
                   RE::ActorValue::kHealth,
                   config.vampirism().leech_vampirism());
  }

  if (config.vampirism().mgef_stamina_enable()) {
    mgef_vampirism(*target,
                   *aggressor,
                   hit_data.totalDamage,
                   config.vampirism().mgef_stamina_keyword(),
                   RE::ActorValue::kStamina,
                   false);
  }

  if (config.vampirism().mgef_magicka_enable()) {
    mgef_vampirism(*target,
                   *aggressor,
                   hit_data.totalDamage,
                   config.vampirism().mgef_magicka_keyword(),
                   RE::ActorValue::kMagicka,
                   false);
  }
}

auto character_update(RE::Character& character, float, const Config& config, Core::ActorsCache::Data& actor_data)
    -> void
{
  auto vampirism_magnitude = static_cast<float>(actor_data.vampirism_accumulator());
  if (vampirism_magnitude > 0.f) {
    actor_data.vampirism_accumulator(0.f);
    let character_health = Core::get_actor_value_max(&character, RE::ActorValue::kHealth) -
                           character.GetActorValue(RE::ActorValue::kHealth);
    if (vampirism_magnitude > character_health) {
      vampirism_magnitude = character_health;
    }
    RE::SpellItem* spell = nullptr;
    if (config.vampirism().leech_vampirism_spell()) {
      spell = config.vampirism().leech_vampirism_spell();
    } else if (config.magic_vampirism().leech_vampirism_spell()) {
      spell = config.magic_vampirism().leech_vampirism_spell();
    }
    if (spell && vampirism_magnitude > 0.f) {
      for (auto effect : spell->effects) {
        if (effect && effect->effectItem.duration > 0) {
          effect->effectItem.magnitude = vampirism_magnitude / static_cast<float>(effect->effectItem.duration);
        }
      }
      cast(*spell, character, character, Core::kNone, 1.f);
    }
  }
}

} // namespace Reflyem::Vampirism
