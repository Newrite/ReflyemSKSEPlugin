#pragma once

namespace Reflyem::MagicBlocker {

auto get_ward_magnitude(RE::Actor* actor) -> float;

auto get_ward_associated_skill(RE::Actor* actor) -> RE::ActorValue;

auto handle_hit_data_block_percent(RE::HitData& hit_data, RE::Actor* target) -> void;

}
