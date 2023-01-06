namespace reflyem
{
	namespace utils
	{

		auto get_first_active_effect_with_keyword_magnitude(
			RE::Actor& actor,
			RE::BGSKeyword& keyword) -> std::optional<float>
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

				if (!base_effect->HasKeywordID(keyword.formID))
				{
					continue;
				}

				return std::optional<float> { active_effect->magnitude };

			}

			return std::nullopt;

		}

		auto getting_damage_mult(RE::Actor& actor) -> float
		{

			// auto settings_collection = RE::GameSettingCollection().GetSingleton();
			// auto player = RE::PlayerCharacter::GetSingleton();
			// 
			// if (!settings_collection || !player)
			// {
			// 	return 1.f;
			// }
			// 
			// auto fDiffMultHPByPCL = settings_collection->GetSetting("fDiffMultHPByPCL");
			// auto fDiffMultHPToPCL = settings_collection->GetSetting("fDiffMultHPToPCL");
			// 
			// if (actor.IsPlayer() || actor.IsPlayerTeammate() || actor.IsHostileToActor(player))
			// {
			// 	return fDiffMultHPToPCL->GetFloat();
			// }
			// else
			// {
			// 	return fDiffMultHPByPCL->GetFloat();
			// }

			return true ? 1.f : actor.armorRating;

		}
	}
}