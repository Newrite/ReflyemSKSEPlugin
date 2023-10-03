#include "plugin/PapyrusExtender.hpp"
#include "plugin/ItemLimit.hpp"

namespace Reflyem::PapyrusExtender
{

constexpr auto SCRIPT_NAMESPACE = "ReflyemPapyrus"sv;

// Papyrus Function
auto ActorEquippedWeaponEnchantmentHasKeyword(
    RE::StaticFunctionTag*,
    const RE::Actor* actor,
    const RE::BGSKeyword* keyword,
    const bool left_hand) -> bool
{
  if (!keyword || !actor) { return false; }

  let weapon = actor->GetEquippedEntryData(left_hand);

  if (!weapon) { return false; }

  let enchantment = weapon->GetEnchantment();

  if (!enchantment) { return false; }

  return enchantment->HasKeyword(keyword);
}

auto register_functions(RE::BSScript::IVirtualMachine* vm) -> bool
{
  vm->RegisterFunction(SCRIPT_NAMESPACE, "ItemLimit", ItemLimit::UpdateItems, vm);
  vm->RegisterFunction(
      SCRIPT_NAMESPACE,
      "ActorEquippedWeaponEnchantmentHasKeyword",
      ActorEquippedWeaponEnchantmentHasKeyword,
      vm);
  return true;
}

}