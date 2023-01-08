#include <random>

namespace reflyem
{
  namespace core
  {

    std::random_device rd;     // Only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(0, 100); // Guaranteed unbiased

    auto get_rundom_int() -> int
    {
      return uni(rng);
    }

    auto get_effect_with_keyword_value(
      RE::Actor& actor,
      RE::BGSKeyword& keyword) -> std::optional<float>
    {

      auto pos_value = 0.f;
      auto neg_value = 0.f;

      auto active_effects = actor.GetActiveEffectList();
      for (auto active_effect : *active_effects)
      {

        if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive)
          || !active_effect->effect
          || !active_effect->effect->baseEffect)
        {
          continue;
        }

        auto effect = active_effect->effect;
        auto base_effect = effect->baseEffect;

        if (!base_effect->HasKeywordID(keyword.formID))
        {
          continue;
        }

        if (base_effect->data.flags.any(RE::EffectSetting::EffectSettingData::Flag::kDetrimental))
        {
          neg_value += active_effect->magnitude;
        }
        else
        {
          pos_value += active_effect->magnitude;
        }

      }

      if (pos_value > 0.f || neg_value > 0.f)
      {
        return std::optional<float> { pos_value - neg_value };
      }
      else
      {
        return std::nullopt;
      }

    }

    auto getting_damage_mult(RE::Actor& actor) -> float
    {

      auto settings_collection = RE::GameSettingCollection::GetSingleton();
      auto player = RE::PlayerCharacter::GetSingleton();

      if (!settings_collection || !player)
      {
        return 1.f;
      }

      auto fDiffMultHPByPCL = settings_collection->GetSetting("fDiffMultHPByPCL");
      auto fDiffMultHPToPCL = settings_collection->GetSetting("fDiffMultHPToPCL");

      if (actor.IsPlayer() || actor.IsPlayerTeammate() || !actor.IsHostileToActor(player))
      {
        return fDiffMultHPToPCL->GetFloat();
      }
      else
      {
        return fDiffMultHPByPCL->GetFloat();
      }

    }

    auto actor_has_active_mgef_with_keyword(RE::Actor& actor, RE::BGSKeyword& keyword) -> bool
    {
      auto active_effects = actor.GetActiveEffectList();
      for (auto active_effect : *active_effects)
      {

        if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive)
          || !active_effect->effect
          || !active_effect->effect->baseEffect)
        {
          continue;
        }
        auto effect = active_effect->effect;
        auto base_effect = effect->baseEffect;

        if (base_effect->HasKeywordID(keyword.formID))
        {
          return true;
        }
      }
      return false;
    }

    auto cast_on_handle(
      RE::TESForm* keyword,
      RE::TESForm* spell,
      RE::Actor& target,
      RE::Actor& caster) -> void
    {
      if (!keyword || !spell) { return; }

      auto keyword_ptr = keyword->As<RE::BGSKeyword>();
      auto spell_ptr = spell->As<RE::SpellItem>();

      if (!keyword_ptr || !spell_ptr) { return; }

      if (!reflyem::core::actor_has_active_mgef_with_keyword(caster, *keyword_ptr)) { return; }

      if (spell_ptr->data.delivery == RE::MagicSystem::Delivery::kSelf)
      {
        caster
          .GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
          ->CastSpellImmediate(
            spell_ptr,
            true,
            &caster,
            1.00f,
            false,
            0.0f,
            &caster
          );
      }
      else
      {
        caster
          .GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
          ->CastSpellImmediate(
            spell_ptr,
            true,
            &target,
            1.00f,
            false,
            0.0f,
            &caster
          );
      }

    }

    auto is_power_attacking(RE::Actor& actor) -> bool
    {

      auto current_process = actor.GetActorRuntimeData().currentProcess;
      if (!current_process) { return false; }

      auto hight_process = current_process->high;
      if (!hight_process) { return false; }

      auto& attack_data = hight_process->attackData;
      if (!attack_data) { return false; }

      auto flags = attack_data->data.flags;
      return flags.any(RE::AttackData::AttackFlag::kPowerAttack);

    }

    auto has_absolute_keyword(RE::Actor& actor, RE::BGSKeyword& keyword) -> bool
    {
      return 
        actor.HasKeyword(&keyword)
        || actor_has_active_mgef_with_keyword(actor, keyword);
    }

  }
}