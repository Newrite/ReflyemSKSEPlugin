#pragma once

#include "CheatDeath.h"
#include "Core.h"

namespace Reflyem
{
	namespace CheatDeath
	{

		auto cheat_death(
			RE::Actor&             target,
			float&                 damage_value,
			const Reflyem::Config& config) -> void
		{
			auto effects =
				Reflyem::Core::get_effects_by_keyword(target, *config.cheat_death_percent_keyword);
			auto cheat_death_percent =
				Reflyem ::Core ::get_effects_magnitude_sum(effects).value_or(100.0f);

			if (cheat_death_percent >= 100.f) {
				return;
			}
			if (cheat_death_percent < 0.f) {
				cheat_death_percent = 0.f;
			}

			auto damage_mult = Reflyem::Core::getting_damage_mult(target);

			auto health_treshold = target.GetActorValue(RE::ActorValue::kHealth) * (cheat_death_percent / 100.f);

			if ((damage_value * damage_mult) >= health_treshold) {
				damage_value = health_treshold / damage_mult;
			}
		}

		auto modify_actor_value(
			RE::ValueModifierEffect* a_this,
			RE::Actor* a_actor, float& a_value,
			RE::ActorValue         av,
			const Reflyem::Config& config) -> void
		{
			if (Reflyem::Core::can_modify_actor_value(a_this, a_actor, a_value, av)) {
				a_value = std::abs(a_value);
				cheat_death(*a_actor, a_value, config);
				a_value = -a_value;
			}
		}

		auto on_weapon_hit(
			RE::Actor*             target,
			RE::HitData&           hit_data,
			const Reflyem::Config& config) -> void
		{
			cheat_death(*target, hit_data.totalDamage, config);
		}
	}
}