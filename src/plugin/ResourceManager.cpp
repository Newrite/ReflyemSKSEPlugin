#include "plugin/ResourceManager.hpp"
#include "Core.hpp"
#include "plugin/AnimationEventHandler.hpp"

// TODO: Переписать с хуков анимаций на прямые хуки в атаку
namespace Reflyem::ResourceManager {

// ReSharper disable once CppParameterMayBeConst
auto spend_actor_value(RE::Actor& actor, const RE::ActorValue av, float value, const bool no_delay) -> void
{

  letr config = Config::get_singleton();

  Core::set_av_regen_delay(actor.currentProcess, av, no_delay ? 0.f : config.resource_manager().regen_delay());

  Core::damage_actor_value(actor, av, value);
}

auto regeneration_actor_value(RE::Actor& actor, const RE::ActorValue regen_av, const RE::ActorValue regen_av_mult)
    -> float
{
  auto mult = 1.f + (actor.GetActorValue(regen_av_mult) / 100.f);
  if (mult < 0.f) {
    mult = 1.f;
  }

  const auto flat_regen = [&]() -> float {
    let regen = actor.GetActorValue(regen_av);
    return regen * mult;
  }();

  return flat_regen;
}

// auto regeneration_stamina(RE::Actor& actor, const float delta) -> float
// {
//   auto constexpr av = RE::ActorValue::kStamina;
//   auto constexpr regen_av = RE::ActorValue::kStaminaRate;
//   auto constexpr regen_av_mult = RE::ActorValue::kStaminaRateMult;
//   return regeneration_actor_value(actor, av, regen_av, regen_av_mult, delta);
// }
//
// auto regeneration_health(RE::Actor& actor, const float delta) -> float
// {
//   auto constexpr av = RE::ActorValue::kHealth;
//   auto constexpr regen_av = RE::ActorValue::kHealRate;
//   auto constexpr regen_av_mult = RE::ActorValue::kHealRateMult;
//   return regeneration_actor_value(actor, av, regen_av, regen_av_mult, delta);
// }
//
// auto regeneration_magicka(RE::Actor& actor, const float delta) -> float
// {
//   auto constexpr av = RE::ActorValue::kMagicka;
//   auto constexpr regen_av = RE::ActorValue::kMagickaRate;
//   auto constexpr regen_av_mult = RE::ActorValue::kMagickaRateMult;
//   return regeneration_actor_value(actor, av, regen_av, regen_av_mult, delta);
// }
//
// auto regeneration(RE::Actor& actor, const RE::ActorValue av, const float delta) -> float
// {
//   switch (av)
//     {
//       case RE::ActorValue::kHealth: {
//         return regeneration_health(actor, delta);
//       }
//       case RE::ActorValue::kStamina: {
//         return regeneration_stamina(actor, delta);
//       }
//       case RE::ActorValue::kMagicka: {
//         return regeneration_magicka(actor, delta);
//       }
//       default: {
//         return 0.0;
//       }
//     }
// }

auto weap_actor_mask_multiply(const FormMask& matrix1, const ActorMask& matrix2) -> std::unique_ptr<FormMask>
{
  constexpr auto row = 1;

  FormMask result{{{0, 0, 0}}};
  for (auto x = 0; x < row; x++) {
    constexpr auto column = 3;

    for (auto y = 0; y < column; y++) {
      int32_t acc = 0;
      for (auto z = 0; z < 3; z++) {
        acc += matrix1.at(x).at(z) * matrix2.at(z).at(y);
      }
      result.at(x).at(y) = acc;
    }
  }
  return std::make_unique<FormMask>(result);
}

auto handle_mask_sum_for_drain_values(std::int32_t mask_sum, float cost) -> std::shared_ptr<ResourceDrain>
{
  switch (mask_sum) {
    case 1:
      return std::make_shared<ResourceDrain>(ResourceDrain(cost, 0, 0));
    case 2:
      return std::make_shared<ResourceDrain>(ResourceDrain(0, cost, 0));
    case 4:
      return std::make_shared<ResourceDrain>(ResourceDrain(0, 0, cost));
    case 3:
      cost = cost * 0.5f;
      return std::make_shared<ResourceDrain>(ResourceDrain(cost, cost, 0));
    case 5:
      cost = cost * 0.5f;
      return std::make_shared<ResourceDrain>(ResourceDrain(cost, 0, cost));
    case 6:
      cost = cost * 0.5f;
      return std::make_shared<ResourceDrain>(ResourceDrain(0, cost, cost));
    case 7:
      cost = cost * 0.35f;
      return std::make_shared<ResourceDrain>(ResourceDrain(cost, cost, cost));
    default:
      logger::debug("default switch on eval drain value: mask {}"sv, mask_sum);
      return std::make_shared<ResourceDrain>(ResourceDrain(cost, 0, 0));
  }
}

auto get_form_mask(const RE::BGSKeywordForm& form, const Config& config, const std::int32_t default_stat)
    -> std::unique_ptr<FormMask>
{
  FormMask f_mask{{{0, 0, 0}}};
  logger::debug("start get form mask"sv);
  f_mask.at(0).at(default_stat) = 1;
  bool check_no_set_zero = false;
  if (config.resource_manager().health_kw() &&
      Core::try_keyword_form_has_keyword(&form, config.resource_manager().health_kw())) {
    if (default_stat == 1) {
      check_no_set_zero = true;
    }
    if (!check_no_set_zero) {
      f_mask.at(0).at(default_stat) = 0;
    }
    f_mask.at(0).at(1) = 1;
  }
  if (config.resource_manager().magicka_kw() &&
      Core::try_keyword_form_has_keyword(&form, config.resource_manager().magicka_kw())) {
    if (default_stat == 2) {
      check_no_set_zero = true;
    }
    if (!check_no_set_zero) {
      f_mask.at(0).at(default_stat) = 0;
    }
    f_mask.at(0).at(2) = 1;
  }
  if (config.resource_manager().stamina_kw() &&
      Core::try_keyword_form_has_keyword(&form, config.resource_manager().stamina_kw())) {
    if (default_stat == 0) {
      check_no_set_zero = true;
    }
    if (!check_no_set_zero) {
      f_mask.at(0).at(default_stat) = 0;
    }
    f_mask.at(0).at(0) = 1;
  }
  logger::debug("end get form mask"sv);
  return std::make_unique<FormMask>(f_mask);
}

auto get_actor_mask(RE::Actor& form, const Config& config, const std::int32_t default_stat) -> std::unique_ptr<FormMask>
{
  FormMask f_mask{{{0, 0, 0}}};
  logger::debug("start get actor mask"sv);
  f_mask.at(0).at(default_stat) = 1;
  if (config.resource_manager().health_kw() &&
      Core::try_has_absolute_keyword(&form, config.resource_manager().health_kw())) {
    f_mask.at(0).at(default_stat) = 0;
    f_mask.at(0).at(1) = 1;
  }
  if (config.resource_manager().magicka_kw() &&
      Core::try_has_absolute_keyword(&form, config.resource_manager().magicka_kw())) {
    f_mask.at(0).at(default_stat) = 0;
    f_mask.at(0).at(2) = 1;
  }
  if (config.resource_manager().stamina_kw() &&
      Core::try_has_absolute_keyword(&form, config.resource_manager().stamina_kw())) {
    f_mask.at(0).at(default_stat) = 0;
    f_mask.at(0).at(0) = 1;
  }
  logger::debug("end get actor mask"sv);
  return std::make_unique<FormMask>(f_mask);
}

auto calc_mask_sum(const FormMask& f_mask) -> std::int32_t
{
  auto mask_sum = 0;
  if (f_mask.at(0).at(0) > 0) {
    mask_sum += 1;
  }
  if (f_mask.at(0).at(1) > 0) {
    mask_sum += 2;
  }
  if (f_mask.at(0).at(2) > 0) {
    mask_sum += 4;
  }
  return mask_sum;
}

auto get_drain_value(RE::Actor& actor,
                     const RE::BGSKeywordForm& form,
                     const Config& config,
                     const float cost,
                     const bool enable_conversions,
                     const std::int32_t default_stat = 0) -> std::shared_ptr<ResourceDrain>
{
  const auto f_mask = get_form_mask(form, config, default_stat);

  int32_t mask_sum;

  if (!enable_conversions) {
    mask_sum = calc_mask_sum(*f_mask);

    return handle_mask_sum_for_drain_values(mask_sum, cost);
  }

  const auto stamina_to_health =
      Core::try_has_absolute_keyword(&actor, config.resource_manager().convert_stamina_health_kw());
  logger::debug("key conversion1"sv);
  const auto stamina_to_magicka =
      Core::try_has_absolute_keyword(&actor, config.resource_manager().convert_stamina_magicka_kw());
  logger::debug("key conversion2"sv);
  const auto health_to_stamina =
      Core::try_has_absolute_keyword(&actor, config.resource_manager().convert_health_stamina_kw());
  logger::debug("key conversion3"sv);
  const auto health_to_magicka =
      Core::try_has_absolute_keyword(&actor, config.resource_manager().convert_health_magicka_kw());
  logger::debug("key conversion4"sv);
  const auto magicka_to_stamina =
      Core::try_has_absolute_keyword(&actor, config.resource_manager().convert_magicka_stamina_kw());
  logger::debug("key conversion5"sv);
  const auto magicka_to_health =
      Core::try_has_absolute_keyword(&actor, config.resource_manager().convert_magicka_health_kw());
  logger::debug("end get conversions"sv);

  ActorMask a_mask{{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}};

  logger::debug("start mutate actor mask"sv);
  if (stamina_to_health) {
    a_mask.at(0) = {{0, 1, 0}};
  }
  if (stamina_to_magicka) {
    a_mask.at(0) = {{0, 0, 1}};
  }
  if (health_to_magicka) {
    a_mask.at(1) = {{0, 0, 1}};
  }
  if (health_to_stamina) {
    a_mask.at(1) = {{1, 0, 0}};
  }
  if (magicka_to_health) {
    a_mask.at(2) = {{0, 1, 0}};
  }
  if (magicka_to_stamina) {
    a_mask.at(2) = {{1, 0, 0}};
  }
  logger::debug("end mutate actor mask"sv);

  logger::debug("before start multiply"sv);
  const auto mask = weap_actor_mask_multiply(*f_mask, a_mask);
  logger::debug("before end multiply"sv);

  mask_sum = calc_mask_sum(*mask);

  return handle_mask_sum_for_drain_values(mask_sum, cost);
}

auto get_bash_drain_value(RE::Actor& actor,
                          const WeaponOrArmor& form,
                          const Config& config,
                          const float cost,
                          const bool enable_conversions) -> std::shared_ptr<ResourceDrain>
{
  if (form.is_l) {
    return get_drain_value(actor, *form.left, config, cost, enable_conversions);
  } else {
    return get_drain_value(actor, *form.right, config, cost, enable_conversions);
  }
}

auto get_weapon_weight(const RE::TESObjectWEAP* weapon, const Config& config) -> float
{

  static constexpr float FALLBACK_VALUE = 5.f;

  if (!weapon) {
    logi("Null weapon, return fallback value: {}", FALLBACK_VALUE);
    return FALLBACK_VALUE;
  }

  logger::debug("get weight of weapon"sv);
  if (const auto weight = weapon->weight; weight > 0.f) {
    return weight * config.resource_manager().weight_mult();
  }

  logger::debug("get damage of weapon"sv);
  if (const auto damage = static_cast<float>(weapon->attackDamage) * config.resource_manager().damage_mult();
      damage > 0.f) {
    return damage * config.resource_manager().weight_mult();
  }

  logger::debug("get critical damage of weapon"sv);
  if (const auto critical_damage =
          static_cast<float>(weapon->criticalData.damage) * config.resource_manager().damage_mult();
      critical_damage > 0.f) {
    return critical_damage * config.resource_manager().weight_mult();
  }

  logger::debug("return fallback"sv);

  return FALLBACK_VALUE;
}

auto get_shield_weight(const RE::TESObjectARMO& shield, const Config& config) -> float
{
  const auto weight = shield.weight;
  if (weight > 0.f) {
    return weight * config.resource_manager().weight_mult();
  }

  const auto armor = static_cast<float>(shield.armorRating);
  if (armor > 0.f) {
    return armor * config.resource_manager().weight_mult() * config.resource_manager().armor_mult();
  }

  // 5.f value as fallback
  return 5.f;
}

auto get_sale_for_spend_from_av(std::int32_t av_value, std::int32_t high_value, const std::int32_t low_value) -> float
{
  if (high_value > 100) {
    high_value = 100;
  }
  if (av_value > high_value) {
    av_value = high_value;
  }
  if (av_value < low_value) {
    av_value = low_value;
  }

  return 1.f - (static_cast<float>(av_value) / 100.f);
}

auto get_attack_drain_cost(RE::Actor& actor,
                           const RE::TESObjectWEAP* weapon,
                           const bool is_power_attack,
                           const Config& config) -> float
{

  logger::debug("Get actor infamy for actor: {} with level: {}"sv, actor.GetName(), actor.GetLevel());
  auto infamy = actor.GetActorValue(RE::ActorValue::kInfamy);
  if (infamy < 1.f || !config.resource_manager().infamy_enable()) {
    infamy = 1.f;
  }

  logger::debug("Get actor power attack sale mult"sv);
  float power_attack_cost_mult = 0.f;
  if (is_power_attack) {
    const auto av_value =
        static_cast<std::int32_t>(actor.GetActorValue(config.resource_manager().power_attack_cost_av()));
    power_attack_cost_mult = get_sale_for_spend_from_av(av_value,
                                                        config.resource_manager().power_attack_cost_high(),
                                                        config.resource_manager().power_attack_cost_low());
  }

  logger::debug("Get actor normal attack av cost sale"sv);
  const auto av_value = static_cast<std::int32_t>(actor.GetActorValue(config.resource_manager().attack_cost_av()));
  auto attack_cost_mult = get_sale_for_spend_from_av(
      av_value, config.resource_manager().attack_cost_high(), config.resource_manager().attack_cost_low());

  logger::debug("Get actor weapon weight"sv);
  const auto weapon_weight = get_weapon_weight(weapon, config);

  logger::debug("attack cost mult: {}, power attack cost mult: {}"sv, attack_cost_mult, power_attack_cost_mult);

  const auto cost = (weapon_weight * attack_cost_mult * infamy) +
                    (weapon_weight * infamy * power_attack_cost_mult * config.resource_manager().power_attack_mult());

  return cost * config.resource_manager().global_mult();
}

auto get_bash_drain_cost(RE::Actor& actor, const WeaponOrArmor& form, const bool is_power_attack, const Config& config)
    -> float
{
  auto infamy = actor.GetActorValue(RE::ActorValue::kInfamy);
  if (infamy < 1.f || !config.resource_manager().infamy_enable()) {
    infamy = 1.f;
  }

  float power_bash_cost_mult = 0.f;
  if (is_power_attack) {
    const auto av_value = static_cast<std::int32_t>(actor.GetActorValue(RE::ActorValue::kBlock) * 0.25f);
    power_bash_cost_mult = get_sale_for_spend_from_av(av_value, 100, -100);
  }

  const auto av_value = static_cast<std::int32_t>(actor.GetActorValue(RE::ActorValue::kBlock) * 0.5f);
  auto bash_cost_mult = get_sale_for_spend_from_av(av_value, 100, -100);

  auto weight = 0.f;
  if (form.is_l) {
    weight = get_weapon_weight(form.left, config);
  }
  if (!form.is_l) {
    weight = get_shield_weight(*form.right, config);
  }

  logger::debug("bash cost mult: {}, power bash cost mult: {}"sv, bash_cost_mult, power_bash_cost_mult);

  const auto cost = (weight * bash_cost_mult * infamy) +
                    (weight * infamy * power_bash_cost_mult * config.resource_manager().power_attack_mult());

  return cost * config.resource_manager().global_mult();
}

struct ResourceAttackDataContext
{
  bool is_power;
  bool is_left;
  bool is_bash;
  float stamina_mult;

  static auto create(const bool is_power, const bool is_left, const bool is_bash, const float stamina_mult)
      -> ResourceAttackDataContext
  {
    return {is_power, is_left, is_bash, stamina_mult};
  }
};

struct ResourceFormsContext
{
  RE::ActorValueOwner* owner;
  RE::TESWeightForm* form_weight;
  RE::TESForm* attack_form;

  static auto create(RE::ActorValueOwner* owner, RE::TESWeightForm* form_weight, RE::TESForm* attack_form)
      -> ResourceFormsContext
  {
    return {owner, form_weight, attack_form};
  }
};

auto calculate_attack_cost(const ResourceFormsContext& forms_context,
                           const ResourceAttackDataContext attack_context,
                           RE::GameSettingCollection* settings,
                           const Config& config,
                           const bool no_log = true) -> float
{
  let fStaminaPowerBashBase = settings->GetSetting("fStaminaPowerBashBase")->GetFloat();
  let fStaminaBashBase = settings->GetSetting("fStaminaBashBase")->GetFloat();
  let fStaminaAttackWeaponMult = settings->GetSetting("fStaminaAttackWeaponMult")->GetFloat();
  let fStaminaAttackWeaponBase = settings->GetSetting("fStaminaAttackWeaponBase")->GetFloat();
  let fPowerAttackStaminaPenalty = settings->GetSetting("fPowerAttackStaminaPenalty")->GetFloat();
  let fStaminaPowerAttackWeaponBase = config.resource_manager().stamina_power_attack_weapon_base();
  let fPowerBashStaminaPenalty = config.resource_manager().power_bash_stamina_penalty();
  let fStaminaBashMult = config.resource_manager().stamina_bash_mult();

  if (forms_context.owner->GetIsPlayerOwner() && !no_log) {
    Core::console_log(std::format("RM: CALC_ATTACK_COST fStaminaPowerBashBase: {} fStaminaBashBase: {} "
                                  "fStaminaAttackWeaponMult: {} fStaminaAttackWeaponBase: {}",
                                  fStaminaPowerBashBase,
                                  fStaminaBashBase,
                                  fStaminaAttackWeaponMult,
                                  fStaminaAttackWeaponBase));
  }

  if (forms_context.owner->GetIsPlayerOwner() && !no_log) {
    Core::console_log(
        std::format("RM: CALC_ATTACK_COST fPowerAttackStaminaPenalty: {} fStaminaPowerAttackWeaponBase: {} "
                    "fPowerBashStaminaPenalty: {} fStaminaBashMult: {}",
                    fPowerAttackStaminaPenalty,
                    fStaminaPowerAttackWeaponBase,
                    fPowerBashStaminaPenalty,
                    fStaminaBashMult));
  }

  let is_power = attack_context.is_power;
  let is_bash = attack_context.is_bash;

  auto weight = forms_context.form_weight->weight;
  if (weight <= 0.f) {
    if (let shield = forms_context.attack_form->As<RE::TESObjectARMO>(); shield) {
      weight = static_cast<float>(shield->armorRating) * config.resource_manager().armor_mult();
    }
    if (let weapon = forms_context.attack_form->As<RE::TESObjectWEAP>(); weapon) {
      weight = static_cast<float>(weapon->GetAttackDamage()) * config.resource_manager().damage_mult();
    }
  }

  weight = weight * config.resource_manager().weight_mult();
  auto cost = weight;

  if (config.resource_manager().use_stamina_mult_from_attack_data()) {
    cost = cost * attack_context.stamina_mult;
  }
  if (config.resource_manager().infamy_enable()) {
    if (let infamy = forms_context.owner->GetActorValue(RE::ActorValue::kInfamy); infamy > 1.f) {
      cost = cost * infamy;
    }
  }

  if (is_power) {
    if (is_bash) {
      cost = cost * fPowerBashStaminaPenalty;
    } else {
      cost = cost * fPowerAttackStaminaPenalty;
    }
  }
  if (is_bash) {
    cost = cost * fStaminaBashMult;
  } else {
    cost = cost * fStaminaAttackWeaponMult;
  }

  if (is_bash) {
    if (is_power) {
      cost = cost + fStaminaPowerBashBase;
    } else {
      cost = cost + fStaminaBashBase;
    }
  } else {
    if (is_power) {
      cost = cost + fStaminaPowerAttackWeaponBase;
    } else {
      cost = cost + fStaminaAttackWeaponBase;
    }
  }

  let normal_cost_av = forms_context.owner->GetActorValue(config.resource_manager().attack_cost_av());
  let normal_cost_high = config.resource_manager().attack_cost_high();
  let normal_cost_low = config.resource_manager().attack_cost_low();
  let normal_cost_mult =
      get_sale_for_spend_from_av(static_cast<std::int32_t>(normal_cost_av), normal_cost_high, normal_cost_low);

  let power_cost_av = forms_context.owner->GetActorValue(config.resource_manager().power_attack_cost_av());
  let power_cost_high = config.resource_manager().power_attack_cost_high();
  let power_cost_low = config.resource_manager().power_attack_cost_low();
  let power_cost_mult =
      get_sale_for_spend_from_av(static_cast<std::int32_t>(power_cost_av), power_cost_high, power_cost_low);

  if (is_power) {
    cost = cost * power_cost_mult;
  }

  if (is_power && config.resource_manager().enable_overall_attack_cost_av()) {
    cost = cost * normal_cost_mult;
  }

  if (!is_power) {
    cost = cost * normal_cost_mult;
  }

  cost = cost * config.resource_manager().global_mult();

  if (forms_context.owner->GetIsPlayerOwner() && !no_log) {
    Core::console_log(
        std::format("RM: CALC_ATTACK_COST IsBash: {} IsPower: {} Weight: {} NormalCostMult: {} PowerCostMult: {}",
                    is_bash,
                    is_power,
                    weight,
                    normal_cost_mult,
                    power_cost_mult));
  }

  return cost;
}

auto handle_attack_cost(RE::Actor* actor, const ResourceAttackDataContext attack_ctx, const bool no_log = true)
    -> std::optional<std::shared_ptr<ResourceDrain>>
{
  if (!actor) {
    logi("Actor is null!");
    return std::nullopt;
  }

  let game_settings = RE::GameSettingCollection::GetSingleton();
  if (!game_settings) {
    logi("Game settings is null!");
    return std::nullopt;
  }

  letr config = Config::get_singleton();

  let is_left = attack_ctx.is_left;

  RE::TESForm* attack_form = actor->GetEquippedObject(is_left);
  if (!attack_form) {
    if (!config.resource_manager().unarmed_weapon()) {
      logi("Fallback unarmed weapon is null, return 0 cost");
      return std::nullopt;
    }
    attack_form = config.resource_manager().unarmed_weapon();
  }

  let form_weight = attack_form->As<RE::TESWeightForm>();
  let form_keyword = attack_form->As<RE::BGSKeywordForm>();

  if (!form_keyword || !form_weight) {
    logi("Attacker form don't have keyword form or weight form");
    return std::nullopt;
  }

  let is_bash = attack_ctx.is_bash;

  if (is_bash && !config.resource_manager().bash_spend_enable()) {
    return std::nullopt;
  }

  if (!is_bash && !config.resource_manager().weapon_spend_enable()) {
    return std::nullopt;
  }

  if (actor->IsPlayerRef() && !no_log) {
    Core::console_log("-------------------------RM HANDLE ATTACK COST: START-------------------------");
    Core::console_log(std::format("RM: GET_ATTACK_COST AttackFormName: {} AttackFormEditorId: {} AttackFormFormId: {}",
                                  attack_form->GetName(),
                                  attack_form->GetFormEditorID(),
                                  attack_form->formID));
  }

  let forms_ctx = ResourceFormsContext::create(actor, form_weight, attack_form);

  auto cost = calculate_attack_cost(forms_ctx, attack_ctx, game_settings, config);
  let drain_value = get_drain_value(*actor, *form_keyword, config, cost, true);

  if (actor->IsPlayerRef() && !no_log) {
    Core::console_log(
        std::format("RM: GET_ATTACK_COST Cost: {} HealthCost: {} StaminaCost: {} MagickaCost: {} IsLeft: {}",
                    cost,
                    drain_value->health,
                    drain_value->stamina,
                    drain_value->magicka,
                    is_left));
    Core::console_log("--------------------------RM HANDLE ATTACK COST: END--------------------------");
  }

  return {drain_value};
}

auto melee_weapon_spend(RE::Actor& actor,
                        const RE::TESObjectWEAP* weapon,
                        const bool is_power_attack,
                        const Config& config) -> void
{
  if (!weapon) {
    loge("melee_weapon_spend: Null weapon");
    return;
  }
  logger::debug("melee_weapon_spend"sv);
  auto cost = get_attack_drain_cost(actor, weapon, is_power_attack, config);
  logger::debug("end drain cost: cost {}"sv, cost);
  const auto drain_value = get_drain_value(actor, *weapon, config, cost, true);
  logger::debug("end drain value"sv);
  drain_value->drain(actor);
  logger::debug("end drain"sv);
}

auto bash_spend(RE::Actor& actor, const WeaponOrArmor& form, const bool is_power_attack, const Config& config) -> void
{
  logger::debug("bash_spend"sv);
  auto cost = get_bash_drain_cost(actor, form, is_power_attack, config);
  logger::debug("end drain cost: cost {}"sv, cost);
  const auto drain_value = get_bash_drain_value(actor, form, config, cost, false);
  logger::debug("end drain value"sv);
  drain_value->drain(actor);
  logger::debug("end drain"sv);
}

// TODO: Нужен прямой хук в прыжок, для совместимости с CGO
auto jump_spend(RE::Actor& actor, const Config& config) -> void
{
  spend_actor_value(actor, RE::ActorValue::kStamina, -config.resource_manager().jump_cost());
}

auto ranged_spend_handler(RE::Character& character, const Config& config) -> void
{
  const auto weapon = Core::get_weapon(character, false, config.resource_manager().unarmed_weapon());
  if (!weapon) {
    loge("Error, null fallback weapon");
    return;
  }
  const auto state = character.actorState1.meleeAttackState;
  auto is_draw = state == RE::ATTACK_STATE_ENUM::kBowDraw || state == RE::ATTACK_STATE_ENUM::kBowDrawn ||
                 state == RE::ATTACK_STATE_ENUM::kBowAttached;

  if (weapon->IsCrossbow()) {
    is_draw = state == RE::ATTACK_STATE_ENUM::kBowAttached;
  }

  if ((weapon->IsBow() || weapon->IsCrossbow()) && is_draw) {
    const auto cost = get_attack_drain_cost(character, weapon, false, config) * 0.1f;
    const auto drain_value = get_drain_value(character, *weapon, config, cost, true);
    drain_value->drain(character);
  }
}

auto get_weapon_or_shield(const RE::Actor& actor) -> std::optional<WeaponOrArmor>
{
  logger::debug("get weapon or shield start"sv);
  let form_right = actor.GetEquippedObject(false);
  let form_left = actor.GetEquippedObject(true);
  if (!form_right && !form_left) {
    return std::nullopt;
  }

  if (form_right) {
    if (const auto weapon = form_right->As<RE::TESObjectWEAP>()) {
      auto either = Reflyem::Core::Either<RE::TESObjectWEAP*, RE::TESObjectARMO*>(weapon);
      return {either};
    }
  }

  if (form_left) {
    if (const auto armo = form_left->As<RE::TESObjectARMO>()) {
      auto either = Reflyem::Core::Either<RE::TESObjectWEAP*, RE::TESObjectARMO*>(armo);
      return {either};
    }
  }

  return std::nullopt;
}

auto handle_resource_for_action(RE::Character& actor, const ResourceDrain& drain_values, RE::SpellItem& block_spell)
    -> void
{
  logger::debug("handle_resource_for_action get AVs"sv);
  auto magicka = actor.GetActorValue(RE::ActorValue::kMagicka);
  auto health = actor.GetActorValue(RE::ActorValue::kHealth);
  auto stamina = actor.GetActorValue(RE::ActorValue::kStamina);

  logger::debug("handle_resource_for_action get values drain"sv);
  auto d_magicka = std::abs(drain_values.magicka);
  auto d_health = std::abs(drain_values.health);
  auto d_stamina = std::abs(drain_values.stamina);

  logger::debug("handle_resource_for_action, magicka: {} health: {} stamina: {} dv m h "
                "s: {} {} {}"sv,
                magicka,
                health,
                stamina,
                d_magicka,
                d_health,
                d_stamina);

  if (d_magicka > 0.f && d_magicka > magicka) {
    logger::debug("not enough magicka"sv);
    actor.AddSpell(&block_spell);
    return;
  }
  if (d_health > 0.f && d_health > health) {
    logger::debug("not enough health"sv);
    actor.AddSpell(&block_spell);
    return;
  }
  if (d_stamina > 0.f && d_stamina > stamina) {
    logger::debug("not enough stamina"sv);
    actor.AddSpell(&block_spell);
    return;
  }

  logger::debug("Enough resource"sv);

  actor.RemoveSpell(&block_spell);
  logger::debug("resource remove spell"sv);
  return;
}

auto update_actor(RE::Character& character, float, const Config& config) -> void
{
  if (config.resource_manager().weapon_spend_enable()) {
    logger::debug("Update actor in resource get weapon"sv);
    const auto attack_weapon = Core::get_weapon(character, false, config.resource_manager().unarmed_weapon());

    if (!attack_weapon) {
      loge("Error, null fallback weapon");
      return;
    }

    if (!config.resource_manager().enable_alternate_melee_cost() && config.resource_manager().spell_block_attack() &&
        config.resource_manager().spell_block_power_attack()) {
      logger::debug("Update actor in resource get weapon costs"sv);
      const auto normal_attack_cost = get_attack_drain_cost(character, attack_weapon, false, config);
      const auto power_attack_cost = get_attack_drain_cost(character, attack_weapon, true, config);

      logger::debug("Update actor in resource get weapon drains"sv);
      const auto normal_attack_drain_value =
          get_drain_value(character, *attack_weapon, config, normal_attack_cost, true);
      const auto power_attack_drain_value = get_drain_value(character, *attack_weapon, config, power_attack_cost, true);

      logger::debug("Update actor in resource before handle resource"sv);
      handle_resource_for_action(
          character, *normal_attack_drain_value, *config.resource_manager().spell_block_attack());
      handle_resource_for_action(
          character, *power_attack_drain_value, *config.resource_manager().spell_block_power_attack());
    } else {
      let drain_value_normal =
          handle_attack_cost(&character, ResourceAttackDataContext::create(false, false, false, 1.f));
      let drain_value_power =
          handle_attack_cost(&character, ResourceAttackDataContext::create(true, false, false, 1.f));
      if (drain_value_normal.has_value() && config.resource_manager().spell_block_attack()) {
        handle_resource_for_action(
            character, *drain_value_normal.value(), *config.resource_manager().spell_block_attack());
      }
      if (drain_value_power.has_value() && config.resource_manager().spell_block_power_attack()) {
        handle_resource_for_action(
            character, *drain_value_power.value(), *config.resource_manager().spell_block_power_attack());
      }
    }
  }

  if (config.resource_manager().bash_spend_enable()) {

    if (!config.resource_manager().enable_alternate_melee_cost() && config.resource_manager().spell_block_bash()) {
      logger::debug("Update actor in resource get bash"sv);
      const auto bash_form = get_weapon_or_shield(character);

      if (!bash_form.has_value()) {
        return;
      }

      logger::debug("Update actor in resource get bash cost"sv);
      const auto bash_cost = get_bash_drain_cost(character, bash_form.value(), false, config);

      logger::debug("Update actor in resource get bash drains"sv);
      const auto bash_drain_value = get_bash_drain_value(character, bash_form.value(), config, bash_cost, false);

      logger::debug("Update actor in resource bash before handler resource"sv);
      handle_resource_for_action(character, *bash_drain_value, *config.resource_manager().spell_block_bash());
    } else {
      let drain_value_bash_normal =
          handle_attack_cost(&character, ResourceAttackDataContext::create(false, false, true, 1.f));
      if (drain_value_bash_normal.has_value() && config.resource_manager().spell_block_bash()) {
        handle_resource_for_action(
            character, *drain_value_bash_normal.value(), *config.resource_manager().spell_block_bash());
      }
    }
  }
}

auto animation_handler(const RE::BSAnimationGraphEvent& event, const Config& config) -> void
{
  const auto actor = const_cast<RE::Actor*>(event.holder->As<RE::Actor>());

  if (!actor) {
    return;
  }

  const auto is_power_attack = Core::is_power_attacking(*actor);

  switch (AnimationEventHandler::try_find_animation(event.tag.c_str())) {
    case AnimationEventHandler::AnimationEvent::kBashExit: {
      if (!config.resource_manager().bash_spend_enable() || config.resource_manager().enable_alternate_melee_cost()) {
        return;
      }

      const auto weapon_or_shield = get_weapon_or_shield(*actor);
      if (!weapon_or_shield.has_value()) {
        return;
      }

      const auto& value = weapon_or_shield.value();
      logger::debug("get value from option (weapon or shield)"sv);
      bash_spend(*actor, value, is_power_attack, config);
      return;
    }
    case AnimationEventHandler::AnimationEvent::kWeaponSwing: {
      // Check is attacking because weapon draw proc weapon swing
      if (!config.resource_manager().weapon_spend_enable() || !actor->IsAttacking() ||
          config.resource_manager().enable_alternate_melee_cost()) {
        return;
      }

      const auto weapon = Core::get_weapon(*actor, false, config.resource_manager().unarmed_weapon());
      logger::debug("get weapon end");
      melee_weapon_spend(*actor, weapon, is_power_attack, config);
      return;
    }
    case AnimationEventHandler::AnimationEvent::kWeaponLeftSwing: {
      // Check is attacking because weapon draw proc weapon swing
      if (!config.resource_manager().weapon_spend_enable() || !actor->IsAttacking() ||
          config.resource_manager().enable_alternate_melee_cost()) {
        return;
      }

      const auto weapon = Core::get_weapon(*actor, true, config.resource_manager().unarmed_weapon());
      logger::debug("get weapon end"sv);
      melee_weapon_spend(*actor, weapon, is_power_attack, config);
      return;
    }
    case AnimationEventHandler::AnimationEvent::kJumpUp: {
      jump_spend(*actor, config);
      return;
    }
  }
}

auto handle_block_hit(RE::Actor& target,
                      float total_damage,
                      const float av_value,
                      const float damage_mult,
                      const RE::ActorValue av) -> float
{
  if (total_damage >= av_value) {
    total_damage = av_value;
    spend_actor_value(target, av, av_value);
  } else {
    spend_actor_value(target, av, total_damage);
  }
  return total_damage / damage_mult;
}

auto handle_hit_data(RE::HitData& hit_data, const float av_part_damage) -> void
{
  hit_data.totalDamage -= av_part_damage;
  if (hit_data.totalDamage < 0.f) {
    hit_data.totalDamage = 0.f;
  }
  return;
}

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void
{
  logd("on_weapon_hit resource manager"sv);

  if (!hit_data.flags.any(RE::HitData::Flag::kBlocked) || hit_data.totalDamage <= 0.f) {
    return;
  }

  logd("on_weapon_hit blocked hit"sv);

  let weapon_or_shield = get_weapon_or_shield(*target);
  let form = [&]() -> RE::TESForm* {

    if (config.magic_blocker().enable() &&
    Core::try_has_absolute_keyword(target, config.magic_blocker().magic_blocker_keyword())) {
      return config.magic_blocker().resource_manager_keyword_holder();
    }
    
    if (weapon_or_shield.has_value()) {
      if (weapon_or_shield.value().is_l) {
        return weapon_or_shield.value().left;
      }
      return weapon_or_shield.value().right;
    }
    return nullptr;
  }();
  if (!form) {
    return;
  }

  logd("success get form"sv);

  const auto keyword_form = form->As<RE::BGSKeywordForm>();
  if (!keyword_form) {
    return;
  }

  logd("success get keyword form"sv);

  const auto f_mask = get_form_mask(*keyword_form, config);

  logd("mask values: {} {} {}"sv, f_mask->at(0).at(0), f_mask->at(0).at(1), f_mask->at(0).at(2));

  const auto stamina = target->GetActorValue(RE::ActorValue::kStamina);
  const auto health = target->GetActorValue(RE::ActorValue::kHealth);
  const auto magicka = target->GetActorValue(RE::ActorValue::kMagicka);

  auto damage_mult = Core::getting_damage_mult(*target);
  auto total_damage = hit_data.totalDamage * damage_mult;

  auto mask_sum = calc_mask_sum(*f_mask);

  constexpr auto half_part = 0.5f;
  constexpr auto three_part = 0.35f;

  logd("mask_sum {} damage_mult {} total_damage {}"sv, mask_sum, damage_mult, total_damage);

  switch (mask_sum) {
    case 1: // stamina
    {
      const auto stamina_part_damage =
          handle_block_hit(*target, total_damage, stamina, damage_mult, RE::ActorValue::kStamina);
      handle_hit_data(hit_data, stamina_part_damage);
      return;
    }
    case 2: // health
    {
      const auto health_part_damage =
          handle_block_hit(*target, total_damage, health, damage_mult, RE::ActorValue::kHealth);
      handle_hit_data(hit_data, health_part_damage);
      return;
    }
    case 4: // magicka
    {
      const auto magicka_part_damage =
          handle_block_hit(*target, total_damage, magicka, damage_mult, RE::ActorValue::kMagicka);
      handle_hit_data(hit_data, magicka_part_damage);
      return;
    }
    case 3: // health stamina
    {
      const auto health_part_damage =
          handle_block_hit(*target, total_damage * half_part, health, damage_mult, RE::ActorValue::kHealth);
      handle_hit_data(hit_data, health_part_damage);

      const auto stamina_part_damage =
          handle_block_hit(*target, total_damage * half_part, stamina, damage_mult, RE::ActorValue::kStamina);
      handle_hit_data(hit_data, stamina_part_damage);
      return;
    }
    case 5: // magicka stamina
    {
      const auto magicka_part_damage =
          handle_block_hit(*target, total_damage * half_part, magicka, damage_mult, RE::ActorValue::kMagicka);
      handle_hit_data(hit_data, magicka_part_damage);

      const auto stamina_part_damage =
          handle_block_hit(*target, total_damage * half_part, stamina, damage_mult, RE::ActorValue::kStamina);
      handle_hit_data(hit_data, stamina_part_damage);
      return;
    }
    case 6: // health magicka
    {
      const auto health_part_damage =
          handle_block_hit(*target, total_damage * half_part, health, damage_mult, RE::ActorValue::kHealth);
      handle_hit_data(hit_data, health_part_damage);

      const auto magicka_part_damage =
          handle_block_hit(*target, total_damage * half_part, magicka, damage_mult, RE::ActorValue::kMagicka);
      handle_hit_data(hit_data, magicka_part_damage);
      return;
    }
    case 7: // health magicka stamina
    {
      const auto health_part_damage =
          handle_block_hit(*target, total_damage * three_part, health, damage_mult, RE::ActorValue::kHealth);
      handle_hit_data(hit_data, health_part_damage);

      const auto magicka_part_damage =
          handle_block_hit(*target, total_damage * three_part, magicka, damage_mult, RE::ActorValue::kMagicka);
      handle_hit_data(hit_data, magicka_part_damage);

      const auto stamina_part_damage =
          handle_block_hit(*target, total_damage * three_part, stamina, damage_mult, RE::ActorValue::kStamina);
      handle_hit_data(hit_data, stamina_part_damage);
      return;
    }
    default: {
    }
  }
}

auto get_attack_cost(RE::ActorValueOwner* owner, const RE::BGSAttackData* attack_data) -> bool
{
  if (!owner || !attack_data) {
    return false;
  }

  auto actor = (RE::Actor*)((char*)owner - 0xB0);
  if (!actor) {
    logi("get actor cost: actor null");
    return false;
  }

  let is_left = attack_data->IsLeftAttack();
  let is_power = attack_data->data.flags.any(RE::AttackData::AttackFlag::kPowerAttack);
  let is_bash = attack_data->data.flags.any(RE::AttackData::AttackFlag::kBashAttack);

  let attack_ctx = ResourceAttackDataContext::create(is_power, is_left, is_bash, attack_data->data.staminaMult);
  if (let drain_value = handle_attack_cost(actor, attack_ctx, false); drain_value.has_value()) {
    drain_value.value()->drain(*actor, true);
    if (actor->IsPlayerRef()) {

      letr config = Config::get_singleton();

      if (owner->GetActorValue(RE::ActorValue::kHealth) <= 0.f && drain_value.value()->health > 0.f) {
        Core::flash_hud_meter(RE::ActorValue::kHealth);
        Core::set_av_regen_delay(
            actor->currentProcess, RE::ActorValue::kHealth, config.resource_manager().regen_delay());
      }
      if (owner->GetActorValue(RE::ActorValue::kStamina) <= 0.f && drain_value.value()->stamina > 0.f) {
        Core::flash_hud_meter(RE::ActorValue::kStamina);
        Core::set_av_regen_delay(
            actor->currentProcess, RE::ActorValue::kStamina, config.resource_manager().regen_delay());
      }
      if (owner->GetActorValue(RE::ActorValue::kMagicka) <= 0.f && drain_value.value()->magicka > 0.f) {
        Core::flash_hud_meter(RE::ActorValue::kMagicka);
        Core::set_av_regen_delay(
            actor->currentProcess, RE::ActorValue::kMagicka, config.resource_manager().regen_delay());
      }
      return true;
    }
  }

  return false;
}

} // namespace Reflyem::ResourceManager
