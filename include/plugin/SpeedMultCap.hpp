#pragma once

namespace Reflyem::SpeedMultCap
{

auto get_actor_value(
    RE::ActorValueOwner* this_,
    const float speed_mult_value) -> std::optional<float>;
}