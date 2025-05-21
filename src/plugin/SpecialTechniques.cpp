#include "SpecialTechniques.hpp"

#include "Core.hpp"

#include <numbers>

namespace Reflyem::SpecialTechniques {

static constexpr float MAX_TIME_EMULATE_TDM_DODGE = 0.25f;
static float current_time_emulate_tdm_dodge = 0.f;
static bool emulating_tdm_dodge_flag = false;

auto get_actor_value_map(RE::ActorValueOwner* owner) -> std::map<RE::ActorValue, std::map<RE::ActorValue, float>>
{

  auto result = std::map<RE::ActorValue, std::map<RE::ActorValue, float>>{};

  letr config = Config::get_singleton();
  let actor = Core::get_actor_value_owner_as_actor(owner);
  if (actor && config.special_techniques().keyword_actor_value_to_actor_value() && config.special_techniques().enable()) {
    let effects =
        Core::try_get_effects_by_keyword(actor, config.special_techniques().keyword_actor_value_to_actor_value());
    for (const auto effect : effects) {

      if (!effect) {
        continue;
      }

      let primary_av = Core::get_av_from_effect(*effect);
      let second_av = Core::get_second_av_from_effect(*effect);
      let weight = Core::get_second_av_weight_from_effect(*effect);

      if (primary_av == RE::ActorValue::kNone || second_av == RE::ActorValue::kNone || weight <= 0.f) {
        continue;
      }
      
      if (result.contains(primary_av)) {
        auto& map = result[primary_av];
        if (map.contains(second_av)) {
          map[second_av] = map[second_av] + weight;
        } else {
          map[second_av] = weight;
        }
        continue;
      }

      result[primary_av] = {{second_av, weight}};
    }
  }
  return result;
}

auto rotate_blocker(RE::Actor* target, const RE::TESObjectREFR* attacker, const Config& config) -> void
{

  if (!target || !attacker) {
    return;
  }

  if (config.magic_blocker().enable() && config.magic_blocker().disable_auto_rotation360_block()) {
    if (Core::try_has_absolute_keyword(target, config.magic_blocker().magic_blocker_keyword())) {
      return;
    }
  }

  if (Core::try_has_absolute_keyword(target, config.special_techniques().no_block_rotate_keyword())) {
    return;
  }

  if (Core::try_has_absolute_keyword(target, config.special_techniques().enable_360_block())) {

    let attk_angle_z = attacker->data.angle.z;
    let targ_angle_z = target->data.angle.z;
    let angle = fmod((attk_angle_z + std::numbers::pi_v<float>), (2.f * std::numbers::pi_v<float>));

    target->SetRotationZ(angle);

    if (target->IsPlayerRef()) {
      auto thirdPersonState = static_cast<RE::ThirdPersonState*>(
          RE::PlayerCamera::GetSingleton()->cameraStates[RE::CameraState::kThirdPerson].get());

      if (!thirdPersonState) {
        return;
      }

      if (angle > targ_angle_z) {
        thirdPersonState->freeRotation.x = thirdPersonState->freeRotation.x - (angle - targ_angle_z);
      } else {

        thirdPersonState->freeRotation.x = thirdPersonState->freeRotation.x + (targ_angle_z - angle);
      }

      target->SetGraphVariableBool("TDM_Dodge", true);
      emulating_tdm_dodge_flag = true;
      current_time_emulate_tdm_dodge = 0.f;
    }
  }
}

auto normalize_attack(RE::HitData& hit_data, const Config& config, const bool is_player) -> void
{

  if (!hit_data.attackData) {
    return;
  }

  if (is_player) {
    if (config.special_techniques().normalize_power_attack_mult_pc() >= 0.f &&
        hit_data.attackData->data.damageMult != 0.f &&
        hit_data.attackData->data.flags.any(RE::AttackData::AttackFlag::kPowerAttack)) {
      hit_data.totalDamage = hit_data.totalDamage / hit_data.attackData->data.damageMult;
      hit_data.totalDamage = hit_data.totalDamage * config.special_techniques().normalize_power_attack_mult_pc();
    }

    if (config.special_techniques().normalize_normal_attack_mult_pc() >= 0.f &&
        hit_data.attackData->data.damageMult != 0.f &&
        !hit_data.attackData->data.flags.any(RE::AttackData::AttackFlag::kPowerAttack)) {
      hit_data.totalDamage = hit_data.totalDamage / hit_data.attackData->data.damageMult;
      hit_data.totalDamage = hit_data.totalDamage * config.special_techniques().normalize_normal_attack_mult_pc();
    }
  } else {
    if (config.special_techniques().normalize_power_attack_mult_npc() >= 0.f &&
        hit_data.attackData->data.damageMult != 0.f &&
        hit_data.attackData->data.flags.any(RE::AttackData::AttackFlag::kPowerAttack)) {
      hit_data.totalDamage = hit_data.totalDamage / hit_data.attackData->data.damageMult;
      hit_data.totalDamage = hit_data.totalDamage * config.special_techniques().normalize_power_attack_mult_npc();
    }

    if (config.special_techniques().normalize_normal_attack_mult_npc() >= 0.f &&
        hit_data.attackData->data.damageMult != 0.f &&
        !hit_data.attackData->data.flags.any(RE::AttackData::AttackFlag::kPowerAttack)) {
      hit_data.totalDamage = hit_data.totalDamage / hit_data.attackData->data.damageMult;
      hit_data.totalDamage = hit_data.totalDamage * config.special_techniques().normalize_normal_attack_mult_npc();
    }
  }
}

auto update(RE::Character* character, const float delta) -> void
{

  letr config = Config::get_singleton();

  if (character && character->IsPlayerRef() && emulating_tdm_dodge_flag) {
    if (current_time_emulate_tdm_dodge >= MAX_TIME_EMULATE_TDM_DODGE) {
      character->SetGraphVariableBool(TDM_DODGE, false);
      emulating_tdm_dodge_flag = false;
      current_time_emulate_tdm_dodge = 0.f;
    } else {
      current_time_emulate_tdm_dodge += delta;
    }
  }

  if (character) {

    if (config.special_techniques().normalize_max_regen_delay_health() >= 0.f) {
      if (character->GetRegenDelay(RE::ActorValue::kHealth) >
          config.special_techniques().normalize_max_regen_delay_health()) {
        character->UpdateRegenDelay(RE::ActorValue::kHealth,
                                    config.special_techniques().normalize_max_regen_delay_health());
      }
    }

    if (config.special_techniques().normalize_max_regen_delay_stamina() >= 0.f) {
      if (character->GetRegenDelay(RE::ActorValue::kStamina) >
          config.special_techniques().normalize_max_regen_delay_stamina()) {
        character->UpdateRegenDelay(RE::ActorValue::kStamina,
                                    config.special_techniques().normalize_max_regen_delay_stamina());
      }
    }

    if (config.special_techniques().normalize_max_regen_delay_magicka() >= 0.f) {
      if (character->GetRegenDelay(RE::ActorValue::kMagicka) >
          config.special_techniques().normalize_max_regen_delay_magicka()) {
        character->UpdateRegenDelay(RE::ActorValue::kMagicka,
                                    config.special_techniques().normalize_max_regen_delay_magicka());
      }
    }
  }
}

}
