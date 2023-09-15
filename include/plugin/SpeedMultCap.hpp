#pragma once

namespace Reflyem::SpeedMultCap
{

using get_actor_value_t = REL::Relocation<auto(RE::ActorValueOwner*, RE::ActorValue)->float>;

auto get_actor_value(
    RE::ActorValueOwner& this_,
    const RE::ActorValue av,
    get_actor_value_t get_av_func) -> std::optional<float>;
}