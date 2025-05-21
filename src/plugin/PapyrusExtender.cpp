#include "plugin/PapyrusExtender.hpp"

#include "AbsorbShield.hpp"
#include "Core.hpp"
#include "KiEnergyPower.hpp"
#include "PotionsDrinkLimit.hpp"
#include "plugin/ItemLimit.hpp"

namespace Reflyem::PapyrusExtender {

static bool fake_bow_power = false;

auto is_papyrus_call_bow() -> bool
{
  return fake_bow_power;
}

auto papyrus_call_bow(const bool fake) -> void
{
  fake_bow_power = fake;
}

constexpr auto SCRIPT_NAMESPACE = "ReflyemPapyrus"sv;

// Papyrus Function
auto ActorEquippedWeaponEnchantmentHasKeyword(RE::StaticFunctionTag*,
                                              const RE::TESObjectREFR* object,
                                              const RE::BGSKeyword* keyword,
                                              const bool left_hand) -> bool
{
  if (!keyword || !object) {
    return false;
  }

  let actor = object->As<RE::Actor>();

  if (!actor) {
    return false;
  }

  let weapon = actor->GetEquippedEntryData(left_hand);

  if (!weapon) {
    return false;
  }

  let enchantment = weapon->GetEnchantment();

  if (!enchantment) {
    return false;
  }

  return enchantment->HasKeyword(keyword);
}

// Papyrus Function
auto ApplyEffectShader(RE::StaticFunctionTag*,
                       RE::Actor* refr,
                       RE::TESEffectShader* shader,
                       const float duration,
                       RE::BSFixedString node_name) -> void
{

  Core::console_log(std::format("ApplyShaderStart script, duration: {} node_name: {}", duration, node_name.c_str()));

  if (!refr || !shader) {
    return;
  }

  let node = refr->GetNodeByName(node_name);

  if (!node) {
    return;
  }

  Core::console_log("ApplyShaderStart native");
  if (node_name != "NONE") {
    refr->ApplyEffectShader(shader, duration, nullptr, false, false, node, false);
  } else {
    refr->ApplyEffectShader(shader, duration);
  }
}

// Papyrus Function
auto ApplyArtObject(RE::StaticFunctionTag*,
                    RE::Actor* refr,
                    RE::BGSArtObject* art_object,
                    const float duration,
                    RE::BSFixedString node_name) -> void
{

  Core::console_log(std::format("ApplyArtStart script, duration: {} node_name: {}", duration, node_name.c_str()));

  if (!refr || !art_object) {
    return;
  }

  let node = refr->GetNodeByName(node_name);

  if (!node) {
    return;
  }

  Core::console_log("ApplyArtStart native");
  if (node_name != "NONE") {
    refr->ApplyArtObject(art_object, duration, nullptr, false, false, node, false);
  } else {
    refr->ApplyArtObject(art_object, duration);
  }
}

// Papyrus Function
auto BowFire(RE::StaticFunctionTag*, RE::Actor* refr) -> void
{

  if (!refr) {
    logi("Bow fire: refr null");
    return;
  }

  logi("Bow fire: proc papyrus");
  fake_bow_power = true;
  Core::bow_fire(refr);
  fake_bow_power = false;
}

// Papyrus Function
auto GetMaxHealth(RE::StaticFunctionTag*, RE::Actor* actor) -> float
{

  if (actor) {
    return Core::get_actor_value_max(actor, RE::ActorValue::kHealth);
  }

  return 0.f;

}

// Papyrus Function
auto GetMaxStamina(RE::StaticFunctionTag*, RE::Actor* actor) -> float
{

  if (actor) {
    return Core::get_actor_value_max(actor, RE::ActorValue::kStamina);
  }

  return 0.f;

}

// Papyrus Function
auto GetMaxMagicka(RE::StaticFunctionTag*, RE::Actor* actor) -> float
{

  if (actor) {
    return Core::get_actor_value_max(actor, RE::ActorValue::kMagicka);
  }

  return 0.f;

}

// Papyrus Function
auto SummonFollowersAndSummonsToPlayer(RE::StaticFunctionTag*, const bool followers, const bool summons) -> void
{

  let player = RE::PlayerCharacter::GetSingleton();

  if (followers) {
    let teammates = Core::get_player_followers();
    for (auto teammate : teammates) {
      if (teammate) {
        teammate->MoveTo(player);
      }
    }
  }
  if (summons) {
    let array_summons = Core::get_commanded_actors(player);
    if (!array_summons) {
      return;
    }
    for (const auto& summon_data : *array_summons) {
      if (summon_data.commandedActor && summon_data.commandedActor.get() && summon_data.commandedActor.get().get()) {
        summon_data.commandedActor.get()->MoveTo(player);
      }
    }
  }
}

auto register_functions(RE::BSScript::IVirtualMachine* vm) -> bool
{
  vm->RegisterFunction("ItemLimit", SCRIPT_NAMESPACE, ItemLimit::UpdateItems, vm);
  vm->RegisterFunction(
      "ActorEquippedWeaponEnchantmentHasKeyword", SCRIPT_NAMESPACE, ActorEquippedWeaponEnchantmentHasKeyword, vm);
  vm->RegisterFunction("ApplyEffectShader", SCRIPT_NAMESPACE, ApplyEffectShader, vm);
  vm->RegisterFunction("ApplyArtObject", SCRIPT_NAMESPACE, ApplyArtObject, vm);
  vm->RegisterFunction("SummonFollowersAndSummonsToPlayer", SCRIPT_NAMESPACE, SummonFollowersAndSummonsToPlayer, vm);
  vm->RegisterFunction("ModifyPotionCooldown", SCRIPT_NAMESPACE, PotionsDrinkLimit::ModifyPotionCooldown, vm);
  vm->RegisterFunction("ModifyAbsorbShieldValue", SCRIPT_NAMESPACE, AbsorbShield::Papyrus::ModifyAbsorbShieldValue, vm);
  vm->RegisterFunction(
      "GetCurrentAbsorbShieldValue", SCRIPT_NAMESPACE, AbsorbShield::Papyrus::GetCurrentAbsorbShieldValue, vm);
  vm->RegisterFunction("GetMaxAbsorbShieldValue", SCRIPT_NAMESPACE, AbsorbShield::Papyrus::GetMaxAbsorbShieldValue, vm);
  vm->RegisterFunction(
      "GetAbsorbShieldValueDegenDelay", SCRIPT_NAMESPACE, AbsorbShield::Papyrus::GetAbsorbShieldValueDegenDelay, vm);
  vm->RegisterFunction(
      "SetAbsorbShieldValueDegenDelay", SCRIPT_NAMESPACE, AbsorbShield::Papyrus::SetAbsorbShieldValueDegenDelay, vm);
  vm->RegisterFunction("GetAvailablePotions", SCRIPT_NAMESPACE, PotionsDrinkLimit::GetAvailablePotions, vm);
  vm->RegisterFunction("ModifyKiEnergyCooldown", SCRIPT_NAMESPACE, KiEnergyPower::ModifyKiEnergyCooldown, vm);
  vm->RegisterFunction("SpendKiEnergy", SCRIPT_NAMESPACE, KiEnergyPower::SpendKiEnergy, vm);
  vm->RegisterFunction("CallKiSummonsHandle", SCRIPT_NAMESPACE, KiEnergyPower::CallKiSummonsHandle, vm);
  vm->RegisterFunction("GetMaxHealth", SCRIPT_NAMESPACE, GetMaxHealth, vm);
  vm->RegisterFunction("GetMaxStamina", SCRIPT_NAMESPACE, GetMaxStamina, vm);
  vm->RegisterFunction("GetMaxMagicka", SCRIPT_NAMESPACE, GetMaxMagicka, vm);
  vm->RegisterFunction("GetAvailableKiEnergy", SCRIPT_NAMESPACE, KiEnergyPower::GetAvailableKiEnergy, vm);
  vm->RegisterFunction("BowFire", SCRIPT_NAMESPACE, BowFire, vm);
  return true;
}

}
