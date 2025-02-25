#include "Config.hpp"
#include "Core.hpp"
#include "TimingBlock.hpp"

namespace Reflyem::MagicBlocker {

auto get_ward_magnitude(RE::Actor* actor) -> float
{

  if (!actor) {
    return 0.f;
  }
  
  letr config = Config::get_singleton();
  if (let effect = Core::try_get_effect_by_keyword(actor, config.magic_blocker().magic_blocker_keyword())) {
    return effect->GetMagnitude();
  }
  return 0.f;
}

auto get_ward_associated_skill(RE::Actor* actor) -> RE::ActorValue
{

  if (!actor) {
    return RE::ActorValue::kBlock;
  }
  
  letr config = Config::get_singleton();
  if (let effect = Core::try_get_effect_by_keyword(actor, config.magic_blocker().magic_blocker_keyword())) {
    let skill = effect->effect->baseEffect->data.associatedSkill;
    if (skill != RE::ActorValue::kNone) {
      return skill;
    }
  }
  return RE::ActorValue::kBlock;
}

auto get_spell_form(RE::Actor* actor) -> RE::MagicItem*
{

  if (!actor) {
    return nullptr;
  }
  
  letr config = Config::get_singleton();
  if (let effect = Core::try_get_effect_by_keyword(actor, config.magic_blocker().magic_blocker_keyword())) {
    return effect->spell;
  }
  
  return nullptr;
}

auto spawn_sparks(RE::Actor* blocker,
                  const Config& config) -> void
{
  if (!config.magic_blocker().spark()) {
    return;
  }

  const auto block_node = TimingBlock::place_form_at_me(blocker, config.magic_blocker().blank_activator());
  if (!block_node) {
    return;
  }

  const auto node = "SHIELD"sv;

  block_node->MoveToNode(blocker, node);

  TimingBlock::place_form_at_me(block_node, config.magic_blocker().spark());
  
  block_node->SetDelete(true);
}

auto handle_hit_data_block_percent(RE::HitData& hit_data, RE::Actor* target) -> void
{
  if (!target) {
    logd("MagicBlocker: target null");
    return;
  }

  if (!hit_data.flags.any(RE::HitData::Flag::kBlocked)) {
    logd("MagicBlocker: not blocked");
    return;
  }

  letr config = Config::get_singleton();

  if (!Core::try_has_absolute_keyword(target, config.magic_blocker().magic_blocker_keyword())) {
    logd("MagicBlocker:  not magic block");
    return;
  }

  let blocked_divider = 1.f - hit_data.percentBlocked;
  if (blocked_divider == 0.f) {
    logd("MagicBlocker: divider zero");
    return;
  }
  
  hit_data.totalDamage = hit_data.totalDamage / blocked_divider;

  const auto associated_skill = get_ward_associated_skill(target);
  
  let ward_power = get_ward_magnitude(target);
  let is_power_attack = hit_data.flags.any(RE::HitData::Flag::kPowerAttack);
  let block_av = target->GetActorValue(associated_skill);
  let block_shield_av = target->GetActorValue(RE::ActorValue::kShieldPerks);
      
  auto blocked_percent = Core::calculate_block_cost(
      ward_power, block_av, block_shield_av, false, false, is_power_attack);

  Core::console_log(std::format("MagicBlocker: Divider: {} WardPower: {} IsPowerAttack: {} BlockAv: {} BlockShieldAv: {} BlockedPercent: {}", blocked_divider, ward_power, is_power_attack, block_av, block_shield_av, blocked_percent));
  
  // blocked_percent = blocked_percent * (1.f + (block_av / 100.f) + ((ward_power * 2.f) / 100.f));
  // blocked_percent = blocked_percent * (1.f + ((block_av * 0.7f) / 100.f) + ((ward_power) / 100.f));
  blocked_percent = blocked_percent * (1.f  + (ward_power / 100.f));

  Core::console_log(std::format("MagicBlocker: BlockedPercent After: {}", blocked_percent));
  
  if (blocked_percent > 0.9f) {
    blocked_percent = 0.9f;
  }
  
  if (blocked_percent > 0.0f) {
    hit_data.totalDamage = hit_data.totalDamage * (1.0f - blocked_percent);
    hit_data.percentBlocked = blocked_percent;
  }

  Core::play_sound_base(config.magic_blocker().block_sound(), target);
  spawn_sparks(target, config);


  if (target->IsPlayerRef()) {
    if (const auto spell_form = get_spell_form(target)) {
      RE::PlayerCharacter::GetSingleton()->AddSkillExperience(associated_skill, (hit_data.physicalDamage - hit_data.totalDamage) + 3.f);
    }
  }
  
}

}
