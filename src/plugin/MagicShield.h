#pragma once

#include "Config.h"

namespace reflyem
{
	namespace magic_shield
	{
		auto on_weapon_hit(
			RE::Actor* target,
			RE::HitData& hit_data,
			const reflyem::config& config) -> void;
	}
}