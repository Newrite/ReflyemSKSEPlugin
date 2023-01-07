namespace reflyem
{
  namespace utils
  {

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

      if (actor.IsPlayer() || actor.IsPlayerTeammate() || actor.IsHostileToActor(player))
      {
        return fDiffMultHPToPCL->GetFloat();
      }
      else
      {
        return fDiffMultHPByPCL->GetFloat();
      }

    }
  }
}