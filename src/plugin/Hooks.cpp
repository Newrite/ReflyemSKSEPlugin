#include "Hooks.h"
#include "Vampirism.h"
#include "MagicShield.h"

namespace hooks
{
	auto on_weapon_hit::weapon_hit(RE::Actor* target, RE::HitData& hit_data) -> void
	{

		if (!target) {
			_weapon_hit(target, hit_data);
			return;
		}

		auto& config = reflyem::config::get_singleton();

		if (config.vampirism_enable)
		{
			reflyem::vampirism::on_weapon_hit(target, hit_data, config);
		}

		if (config.magic_shield_enable)
		{
			reflyem::magic_shield::on_weapon_hit(target, hit_data, config);
		}

		_weapon_hit(target, hit_data);
		return;
	}

	auto install_hooks() -> void
	{
		logger::info("start install hooks");
		auto& trampoline = SKSE::GetTrampoline();
		trampoline.create(64);
		on_weapon_hit::install_hook(trampoline);
		logger::info("finish install hooks");
	}
}