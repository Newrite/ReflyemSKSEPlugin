#pragma once

namespace reflyem
{
  namespace utils
  {
    auto get_effect_with_keyword_value(
      RE::Actor& actor,
      RE::BGSKeyword& keyword) -> std::optional<float>;
    auto getting_damage_mult(RE::Actor& actor) -> float;
  }
}