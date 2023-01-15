#include "Crit.h"
#include "Core.h"

namespace Reflyem
{
	namespace Crit
	{

		auto crit(
			RE::Actor&             agressor,
			RE::Actor&             target,
			float&                 damage,
			RE::ActorValue         crit_chance_av,
			RE::ActorValue         crit_damage_av,
			std::int32_t           crit_damage_high,
			bool                   physical_crit,
			const Reflyem::Config& config) -> void
		{
			auto crit_chance =
				static_cast<int>(agressor.GetActorValue(crit_chance_av));
			auto crit_damage =
				static_cast<int>(agressor.GetActorValue(crit_damage_av));

			if (crit_chance <= 0 || crit_damage <= 0) {
				return;
			}
			if (crit_chance > 100) {
				crit_chance = 100;
			}
			if (crit_damage > crit_damage_high) {
				crit_damage = crit_damage_high;
			}

			auto random = Reflyem::Core::get_random_int();

			if (crit_chance > random) {
				auto damage_mult = 1 + (crit_damage / 100.f);
				damage *= damage_mult;

				if (physical_crit && config.cast_on_crit_enable && config.cast_on_crit_formlist_needkw && config.cast_on_crit_formlist_spells) {
					auto length_kw =
						config.cast_on_crit_formlist_needkw->forms.size();
					auto length_sp =
						config.cast_on_crit_formlist_spells->forms.size();
					for (std::uint32_t index = 0u; index < length_kw && index < length_sp; index++) {
						Reflyem::Core::cast_on_handle(
							config.cast_on_crit_formlist_needkw->forms[index],
							config.cast_on_crit_formlist_spells->forms[index],
							target,
							agressor);
					}
				}
			}
		}

		auto on_weapon_hit(
			RE::Actor*             target,
			RE::HitData&           hit_data,
			const Reflyem::Config& config) -> void
		{
			auto agressor = hit_data.aggressor.get();

			if (!agressor || target->IsDead()) {
				return;
			}

			crit(
				*agressor, *target, hit_data.totalDamage,
				config.weapon_crit_chance_av, config.weapon_crit_damage_av,
				config.weapon_crit_high, true, config);
		}

		auto modify_actor_value(
			RE::ValueModifierEffect* a_this,
			RE::Actor* a_actor, float& a_value,
			RE::ActorValue         av,
			const Reflyem::Config& config) -> void
		{
			if (Reflyem::Core::can_modify_actor_value(a_this, a_actor, a_value, av)) {
				auto caster = a_this->GetCasterActor().get();
				a_value = std::abs(a_value);
				crit(
					*caster, *a_actor, a_value,
					config.magic_crit_chance_av, config.magic_crit_damage_av,
					config.magic_crit_high, false, config);
				a_value = -a_value;
			}
		}

	}
}