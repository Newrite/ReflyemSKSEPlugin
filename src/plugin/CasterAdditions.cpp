#include "plugin/CasterAdditions.hpp"
#include "Core.hpp"
#include "plugin/ResourceManager.hpp"

namespace Reflyem::CasterAdditions {

auto on_update_actor(RE::Character& character, float, const Config& config) -> void
{

  if (!config.caster_additions().spell_add_when_casting()) {
    return;
  }

  if (Core::is_casting_actor(character)) {
    character.AddSpell(config.caster_additions().spell_add_when_casting());
  } else {
    character.RemoveSpell(config.caster_additions().spell_add_when_casting());
  }
}

auto eval_spell_cast_cost(const RE::TESForm* spell_form, RE::Actor& caster) -> float
{
  if (!spell_form) {
    return 0.f;
  }

  const auto spell = spell_form->As<RE::SpellItem>();

  if (!spell) {
    return 0.f;
  }

  logger::debug("{} eval cost {}"sv, caster.GetDisplayFullName(), spell->GetFullName());

  const auto cost =
      spell->CalculateMagickaCost(&caster) * Config::get_singleton().caster_additions().stamina_cost_mult();
  logger::debug("SpellCost in stamina: {}"sv, cost);
  return cost;
}

auto eval_spell_cast_cost_rally(const RE::TESForm* spell_form, RE::Actor* caster) -> float
{
  if (!spell_form || !caster) {
    return 0.f;
  }

  const auto spell = spell_form->As<RE::SpellItem>();

  if (!spell) {
    return 0.f;
  }

  if (spell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration) {
    return 0.f;
  }

  const auto& config = Config::get_singleton();

  logger::debug("{} eval rally cost {}"sv, caster->GetDisplayFullName(), spell->GetFullName());

  const auto cost =
      spell->CalculateMagickaCost(caster) * config.caster_additions().rally_manacost_mult();

  if (cost <= 0.f) {
    return 0.f;
  }

  let magicka_percent = Core::get_actor_value_max(caster, RE::ActorValue::kMagicka) * config.caster_additions().rally_magicka_mult();
  
  logger::debug("SpellCost in stamina: {}"sv, cost + magicka_percent);
  return cost + magicka_percent;
}

auto action_event_handler(const SKSE::ActionEvent& event, RE::BSTEventSource<SKSE::ActionEvent>&) -> void
{

  const auto& config = Config::get_singleton();

  if (!event.actor) {
    return;
  }

  switch (event.type.get()) {
    case SKSE::ActionEvent::Type::kSpellCast: {
      if (Config::get_singleton().caster_additions().allow_cast_without_stamina() || !config.caster_additions().stamina_cost()) {
        break;
      }
      if (event.actor->GetActorValue(RE::ActorValue::kStamina) < eval_spell_cast_cost(event.sourceForm, *event.actor)) {
        event.actor->InterruptCast(true);
        Core::play_sound(Config::get_singleton().caster_additions().mag_fail_sound(), event.actor);
        Core::flash_hud_meter(RE::ActorValue::kStamina);
      }
      break;
    }
    case SKSE::ActionEvent::Type::kSpellFire: {
      if (config.caster_additions().stamina_cost()) {
        ResourceManager::spend_actor_value(
            *event.actor, RE::ActorValue::kStamina, eval_spell_cast_cost(event.sourceForm, *event.actor));
      }
      if (config.caster_additions().enable_rally_manacost()) {
        
        let rally_cost = eval_spell_cast_cost_rally(event.sourceForm, event.actor);
        if (rally_cost <= 0.f) {
          break;
        }
        
        auto& actor_cache = Core::ActorsCache::get_singleton();
        auto& actor_data = actor_cache.get_or_add(event.actor->formID).get();
        let new_rally_cost = actor_data.rally_magicka_cost() + rally_cost;
        logi("Rallys for FF: {} {}", rally_cost, new_rally_cost);
        actor_data.rally_magicka_cost(new_rally_cost);
        
      }
      break;
    }
    case SKSE::ActionEvent::Type::kWeaponSwing:
    case SKSE::ActionEvent::Type::kVoiceCast:
    case SKSE::ActionEvent::Type::kVoiceFire:
    case SKSE::ActionEvent::Type::kBowDraw:
    case SKSE::ActionEvent::Type::kBowRelease:
    case SKSE::ActionEvent::Type::kBeginDraw:
    case SKSE::ActionEvent::Type::kEndDraw:
    case SKSE::ActionEvent::Type::kBeginSheathe:
    case SKSE::ActionEvent::Type::kEndSheathe:
      break;
  }
}
} // namespace Reflyem::CasterAdditions
