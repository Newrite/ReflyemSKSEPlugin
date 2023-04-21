#include "plugin/CasterAdditions.hpp"
#include "Core.hpp"
#include "plugin/ResourceManager.hpp"

namespace Reflyem::CasterAdditions
{

auto on_update_actor(RE::Character& character, float, const Config& config) -> void
{
  if (Core::is_casting_actor(character))
    {
      character.AddSpell(config.caster_additions().spell_add_when_casting());
    }
  else { character.RemoveSpell(config.caster_additions().spell_add_when_casting()); }
}

auto eval_spell_cast_cost(const RE::TESForm* spell_form, RE::Actor& caster) -> float
{
  if (!spell_form) { return 0.f; }

  const auto spell = spell_form->As<RE::SpellItem>();

  if (!spell) { return 0.f; }

  logger::debug("{} eval cost {}"sv, caster.GetDisplayFullName(), spell->GetFullName());

  const auto cost = spell->CalculateMagickaCost(&caster) *
                    Config::get_singleton().caster_additions().stamina_cost_mult();
  logger::debug("SpellCost in stamina: {}"sv, cost);
  return cost;
}

auto action_event_handler(const SKSE::ActionEvent& event, RE::BSTEventSource<SKSE::ActionEvent>&)
    -> void
{
  if (event.actor)
    {
      switch (event.type.get())
        {
          case SKSE::ActionEvent::Type::kSpellCast: {
            if (Config::get_singleton().caster_additions().allow_cast_without_stamina()) { break; }
            if (event.actor->GetActorValue(RE::ActorValue::kStamina) <
                eval_spell_cast_cost(event.sourceForm, *event.actor))
              {
                event.actor->InterruptCast(true);
                Core::play_sound(
                    Config::get_singleton().caster_additions().mag_fail_sound(),
                    event.actor);
                Core::flash_hud_meter(RE::ActorValue::kStamina);
              }
            break;
          }
          case SKSE::ActionEvent::Type::kSpellFire: {
            ResourceManager::spend_actor_value(
                *event.actor,
                RE::ActorValue::kStamina,
                eval_spell_cast_cost(event.sourceForm, *event.actor));
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
        case SKSE::ActionEvent::Type::kEndSheathe: break;
        }
    }
}
} // namespace Reflyem::CasterAdditions