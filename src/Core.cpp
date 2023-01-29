#include "Core.hpp"
#include <random>

namespace Reflyem {
namespace Core {

using EffectSettingFlag = RE::EffectSetting::EffectSettingData::Flag;

struct SafetyEffectData {
private:
  RE::EffectSetting* effect_setting_;
  RE::ActiveEffect*  active_effect_;
  RE::Effect*        effect_;

  explicit SafetyEffectData(RE::ActiveEffect* active_effect) {
    effect_         = active_effect->effect;
    effect_setting_ = active_effect->effect->baseEffect;
    active_effect_  = active_effect;
  }

public:
  [[nodiscard]] auto get_effect_setting() const -> RE::EffectSetting& { return *effect_setting_; }
  [[nodiscard]] auto get_effect() const -> RE::Effect& { return *effect_; }
  [[nodiscard]] auto get_active_effect() const -> RE::ActiveEffect& { return *active_effect_; }

  [[nodiscard]] auto is_active_effect() const -> bool {
    return active_effect_->flags.any(RE::ActiveEffect::Flag::kInactive);
  }

  [[nodiscard]] auto is_detrimental() const -> bool {
    return effect_setting_->data.flags.any(EffectSettingFlag::kDetrimental);
  }

  static auto create_safety_effect_data(RE::ActiveEffect* active_effect)
      -> std::optional<SafetyEffectData> {

    if (!active_effect) {
      return std::nullopt;
    }

    if (!active_effect->effect) {
      return std::nullopt;
    }

    if (!active_effect->effect->baseEffect) {
      return std::nullopt;
    }

    return std::optional{SafetyEffectData(active_effect)};
  }
};

auto get_random_int() -> int {
  // StackOverflow:
  // https://stackoverflow.com/questions/5008804/generating-a-random-integer-from-a-range
  static std::random_device rd;        // Only used once to initialise (seed) engine
  static std::mt19937       rng(rd()); // Random-number engine used (Mersenne-Twister in this case)
  static std::uniform_int_distribution<int> uni(0, 100); // Guaranteed unbiased
  return uni(rng);
}

auto flash_hud_meter(const RE::ActorValue av) -> void {
  using Func_T = decltype(&flash_hud_meter);
  REL::Relocation<Func_T> func{ REL::ID(51907) };
  return func(av);
}

auto character_timer_map_handler(const ULONGLONG                      now_time,
                                 std::map<std::uintptr_t, ULONGLONG>& character_timer_map) -> void {
  logger::debug("handle timer map");
  for (auto& [character_ptr, old_time] : character_timer_map) {

    logger::debug("Pre time check");
    if (now_time - old_time >= 5000) {
      character_timer_map.erase(character_ptr);
      continue;
    }

    logger::debug("After time check");
  }
}

auto damage_actor_value(RE::Actor& actor, const RE::ActorValue av, float value) -> void {
  if (value > 0.f) {
    value = value * -1.f;
  }
  actor.RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, av, value);
}

auto can_modify_actor_value(const RE::ValueModifierEffect* a_this, const RE::Actor* a_actor,
                            const float a_value, const RE::ActorValue av) -> bool {

  if (av != RE::ActorValue::kNone && av != RE::ActorValue::kHealth) {
    return false;
  }

  if (av == RE::ActorValue::kNone && a_this->actorValue != RE::ActorValue::kHealth) {
    return false;
  }

  if (a_value >= 0.f) {
    return false;
  }

  const auto caster_ptr = a_this->GetCasterActor();
  if (!caster_ptr || !a_actor || !a_this->effect || !a_this->effect->baseEffect) {
    return false;
  }

  const auto caster = caster_ptr.get();
  if (!caster) {
    return false;
  }

  if (caster == a_actor) {
    return false;
  }

  if (a_this->effect->baseEffect->data.flags.any(EffectSettingFlag::kRecover)) {
    return false;
  }

  if (!a_this->effect->baseEffect->data.flags.any(EffectSettingFlag::kDetrimental)) {
    return false;
  }

  return true;
}

// auto get_first_active_effect_by_keyword(RE::BSSimpleList<RE::ActiveEffect*>* active_effects,
//                                         RE::BGSKeyword&                      keyword)
//     -> std::optional<RE::ActiveEffect*> {
// 
//   if (!active_effects) {
//     return std::optional<RE::ActiveEffect*>{std::nullopt};
//   }
// 
//   for (const auto active_effect : *active_effects) {
//     return std::optional<RE::ActiveEffect*>{std::nullopt};
//   }
// 
//   return std::optional<RE::ActiveEffect*>{std::nullopt};
// }

auto get_effects_magnitude_sum(const std::vector<RE::ActiveEffect*>& effects)
    -> std::optional<float> {
  auto pos_value = 0.f;
  auto neg_value = 0.f;

  for (const auto active_effect : effects) {
    if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) ||
        !active_effect->effect || !active_effect->effect->baseEffect) {
      continue;
    }

    const auto effect      = active_effect->effect;
    const auto base_effect = effect->baseEffect;

    if (base_effect->data.flags.any(RE::EffectSetting::EffectSettingData::Flag::kDetrimental)) {
      neg_value += active_effect->magnitude;
    } else {
      pos_value += active_effect->magnitude;
    }
  }

  if (pos_value > 0.f || neg_value > 0.f) {
    return std::optional{pos_value - neg_value};
  }
  return std::nullopt;
}

auto get_effects_by_keyword(RE::Actor& actor, const RE::BGSKeyword& keyword)
    -> std::vector<RE::ActiveEffect*> {
  auto                           active_effects = actor.GetActiveEffectList();
  std::vector<RE::ActiveEffect*> effects        = {};
  for (auto active_effect : *active_effects) {
    if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) ||
        !active_effect->effect || !active_effect->effect->baseEffect) {
      continue;
    }

    const auto effect = active_effect->effect;

    if (const auto base_effect = effect->baseEffect; !base_effect->HasKeywordID(keyword.formID)) {
      continue;
    }

    effects.push_back(active_effect);
  }

  return effects;
}

auto get_dual_value_mult(const RE::ValueModifierEffect& value_effect) -> float {
  if (!value_effect.effect || !value_effect.effect->baseEffect) {
    return 0.f;
  }
  return value_effect.effect->baseEffect->data.secondAVWeight;
}

auto get_second_av(const RE::ValueModifierEffect& value_effect) -> RE::ActorValue {
  if (!value_effect.effect || !value_effect.effect->baseEffect) {
    return RE::ActorValue::kNone;
  }

  if (!value_effect.effect->baseEffect->HasArchetype(
          RE::EffectSetting::Archetype::kDualValueModifier)) {
    return RE::ActorValue::kNone;
  }

  return value_effect.effect->baseEffect->data.secondaryAV;
}

auto set_av_regen_delay(RE::AIProcess* process, RE::ActorValue av, float time) -> void {

  if (!process) {
    return;
  }

  using FuncT = decltype(&set_av_regen_delay);
  const REL::Relocation<FuncT> func{RELOCATION_ID(38526, 0)};
  return func(process, av, time);
}

auto getting_damage_mult(RE::Actor& actor) -> float {
  const auto settings_collection = RE::GameSettingCollection::GetSingleton();
  const auto player              = RE::PlayerCharacter::GetSingleton();

  if (!settings_collection || !player) {
    return 1.f;
  }

  const auto f_diff_mult_hp_by_pcl = settings_collection->GetSetting("fDiffMultHPByPCL");
  const auto f_diff_mult_hp_to_pcl = settings_collection->GetSetting("fDiffMultHPToPCL");

  if (!f_diff_mult_hp_by_pcl || !f_diff_mult_hp_to_pcl) {
    return 1.f;
  }

  if (actor.IsPlayer() || actor.IsPlayerTeammate() || !actor.IsHostileToActor(player)) {
    return f_diff_mult_hp_to_pcl->GetFloat();
  } else {
    return f_diff_mult_hp_by_pcl->GetFloat();
  }
}

auto actor_has_active_mgef_with_keyword(RE::Actor& actor, const RE::BGSKeyword& keyword) -> bool {
  auto active_effects = actor.GetActiveEffectList();
  for (const auto active_effect : *active_effects) {
    if (!active_effect || active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) ||
        !active_effect->effect || !active_effect->effect->baseEffect) {
      continue;
    }
    const auto base_effect = active_effect->effect->baseEffect;

    if (base_effect->HasKeywordID(keyword.formID)) {
      return true;
    }
  }
  return false;
}

auto cast(RE::SpellItem& spell, RE::Actor& target, RE::Actor& caster) -> void {
  if (spell.data.delivery == RE::MagicSystem::Delivery::kSelf) {
    caster.GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
        ->CastSpellImmediate(&spell, true, &caster, 1.00f, false, 0.0f, &caster);
  } else {
    caster.GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
        ->CastSpellImmediate(&spell, true, &target, 1.00f, false, 0.0f, &caster);
  }
}

auto cast_on_handle(RE::TESForm* keyword, RE::TESForm* spell, RE::Actor& target, RE::Actor& caster)
    -> void {
  if (!spell) {
    return;
  }

  bool allow_cast = false;

  RE::BGSKeyword* keyword_ptr = nullptr;

  if (keyword) {
    keyword_ptr = keyword->As<RE::BGSKeyword>();
  }
  const auto spell_ptr = spell->As<RE::SpellItem>();

  if (!spell_ptr) {
    return;
  }

  if (!keyword_ptr) {
    allow_cast = true;
  }

  if (!allow_cast && !actor_has_active_mgef_with_keyword(caster, *keyword_ptr)) {
    return;
  }

  cast(*spell_ptr, target, caster);
}

auto is_power_attacking(RE::Actor& actor) -> bool {
  const auto current_process = actor.GetActorRuntimeData().currentProcess;
  if (!current_process) {
    return false;
  }

  const auto high_process = current_process->high;
  if (!high_process) {
    return false;
  }

  const auto& attack_data = high_process->attackData;
  if (!attack_data) {
    return false;
  }

  const auto flags = attack_data->data.flags;
  return flags.any(RE::AttackData::AttackFlag::kPowerAttack);
}

auto worn_has_keyword(RE::Actor* actor, RE::BGSKeyword* keyword) -> bool {
  if (!actor || keyword) {
    return false;
  }
  auto inv = actor->GetInventoryChanges();
  if (!inv) {
    return false;
  }
  using FuncT = bool (*)(RE::InventoryChanges*, RE::BGSKeyword*);
  const REL::Relocation<FuncT> func{RELOCATION_ID(15808, 0)};
  return func(inv, keyword);
}

auto do_combat_spell_apply(RE::Actor* actor, RE::SpellItem* spell, RE::TESObjectREFR* target)
    -> void {
  if (!actor || !spell || !target) {
    return;
  }

  using FuncT = void (*)(int32_t, int32_t, RE::Actor*, RE::SpellItem*, RE::TESObjectREFR*);
  const REL::Relocation<FuncT> func{RELOCATION_ID(54124, 0)};
  return func(0, 0, actor, spell, target);
}

auto has_absolute_keyword(RE::Actor& actor, RE::BGSKeyword& keyword) -> bool {
  return actor.HasKeyword(&keyword) || actor_has_active_mgef_with_keyword(actor, keyword) ||
         worn_has_keyword(&actor, &keyword);
}

auto is_casting_actor(RE::Character& character) -> bool {

  if (character.IsDead()) {
    return false;
  }

  const auto caster_left  = character.GetMagicCaster(RE::MagicSystem::CastingSource::kLeftHand);
  const auto caster_right = character.GetMagicCaster(RE::MagicSystem::CastingSource::kRightHand);

  if ((caster_left && caster_left->currentSpell) || (caster_right && caster_right->currentSpell)) {
    return true;
  }
  return false;
}

} // namespace Core
} // namespace Reflyem