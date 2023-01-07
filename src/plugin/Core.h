#pragma once

namespace reflyem
{
  namespace core
  {

    auto get_rundom_int() -> int;

    auto actor_has_active_mgef_with_keyword(RE::Actor& actor, RE::BGSKeyword& keyword) -> bool;

    auto get_effect_with_keyword_value(
      RE::Actor& actor,
      RE::BGSKeyword& keyword) -> std::optional<float>;

    auto getting_damage_mult(RE::Actor& actor) -> float;

    auto cast_on_handle(
      RE::TESForm* keyword,
      RE::TESForm* spell,
      RE::Actor& target,
      RE::Actor& caster) -> void;

  }
}