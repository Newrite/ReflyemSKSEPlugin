#include "plugin/ResourceManager.hpp"

namespace Reflyem {
namespace ResourceManager {

using WeaponOrArmor = Core::Either<RE::TESObjectWEAP*, RE::TESObjectARMO*>;

constexpr auto REGEN_DELAY = 2.5f;

// ReSharper disable once CppParameterMayBeConst
auto spend_actor_value(RE::Actor& actor, const RE::ActorValue av, float value) -> void {
  const auto& config = Config::get_singleton();
  if (config.resource_manager_regeneration_enable) {

    auto& actor_data = Core::ActorsCache::get_singleton().get_or_add(actor.formID).get();

    switch (av) {
    case RE::ActorValue::kStamina: {
      const auto stamina = actor.GetActorValue(RE::ActorValue::kStamina);
      auto       delay   = REGEN_DELAY;
      if (stamina - value < 0.f) {
        delay *= 1.5f;
      }
      actor_data.set_regen_stamina_delay(delay);
    }
    case RE::ActorValue::kHealth: {
      const auto health = actor.GetActorValue(RE::ActorValue::kHealth);
      auto       delay  = REGEN_DELAY;
      if (health - value < 0.f) {
        delay *= 1.5f;
      }
      actor_data.set_regen_health_delay(delay);
    }
    case RE::ActorValue::kMagicka: {
      const auto magicka = actor.GetActorValue(RE::ActorValue::kMagicka);
      auto       delay   = REGEN_DELAY;
      if (magicka - value < 0.f) {
        delay *= 1.5f;
      }
      actor_data.set_regen_magicka_delay(delay);
    }
    default:
      break;
    }
  } else {
    Core::set_av_regen_delay(actor.currentProcess, av, REGEN_DELAY);
  }

  Core::damage_actor_value(actor, av, value);
}

auto regeneration_actor_value(RE::Character& character, const RE::ActorValue av, const RE::ActorValue regen_av,
                              const RE::ActorValue regen_av_mult, const float delta) -> void {
  auto mult = (1.f + (character.GetActorValue(regen_av_mult) / 100.f));
  if (mult < 0.f) {
    mult = 1.f;
  }
  auto       flat_regen         = character.GetActorValue(regen_av) * mult;
  const auto regeneration_value = flat_regen * delta;
  logger::debug("MultRegen {} RegenValue {} FlatRegenBase {}", mult, regeneration_value, flat_regen);
  character.RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, av, regeneration_value);
}

auto regeneration_stamina(RE::Character& character, const Core::ActorsCache::Data& actor_data) -> void {
  if (actor_data.regen_stamina_delay() <= 0.f) {
    auto constexpr av            = RE::ActorValue::kStamina;
    auto constexpr regen_av      = RE::ActorValue::kStaminaRate;
    auto constexpr regen_av_mult = RE::ActorValue::kStaminaRateMult;
    regeneration_actor_value(character, av, regen_av, regen_av_mult, actor_data.delta_update());
  }
}

auto regeneration_health(RE::Character& character, const Core::ActorsCache::Data& actor_data) -> void {
  if (actor_data.regen_health_delay() <= 0.f) {
    auto constexpr av            = RE::ActorValue::kHealth;
    auto constexpr regen_av      = RE::ActorValue::kHealRate;
    auto constexpr regen_av_mult = RE::ActorValue::kHealRateMult;
    regeneration_actor_value(character, av, regen_av, regen_av_mult, actor_data.delta_update());
  }
}

auto regeneration_magicka(RE::Character& character, const Core::ActorsCache::Data& actor_data) -> void {
  if (actor_data.regen_magicka_delay() <= 0.f) {
    auto constexpr av            = RE::ActorValue::kMagicka;
    auto constexpr regen_av      = RE::ActorValue::kMagickaRate;
    auto constexpr regen_av_mult = RE::ActorValue::kMagickaRateMult;
    regeneration_actor_value(character, av, regen_av, regen_av_mult, actor_data.delta_update());
  }
}

auto on_update_actor_regeneration(RE::Character& character, Core::ActorsCache::Data& actor_data) -> void {
  regeneration_health(character, actor_data);
  regeneration_stamina(character, actor_data);
  regeneration_magicka(character, actor_data);

  if (Core::is_casting_actor(character)) {
    actor_data.set_regen_magicka_delay(REGEN_DELAY);
  }

  if (character.IsSprinting()) {
    actor_data.set_regen_stamina_delay(REGEN_DELAY);
  }
}

ResourceDrain::ResourceDrain(float a_stamina, float a_health, float a_magicka) {
  logger::debug("create drain values: S{} H{} M{}"sv, a_stamina, a_health, a_magicka);
  stamina = a_stamina * -1.f;
  health  = a_health * -1.f;
  magicka = a_magicka * -1.f;
}

auto ResourceDrain::drain(RE::Actor& actor) -> void {
  logger::debug("drain values: S{} H{} M{}"sv, stamina, health, magicka);
  if (health != 0.f) {
    spend_actor_value(actor, RE::ActorValue::kHealth, std::abs(health));
  }
  if (magicka != 0.f) {
    spend_actor_value(actor, RE::ActorValue::kMagicka, std::abs(magicka));
  }
  if (stamina != 0.f) {
    spend_actor_value(actor, RE::ActorValue::kStamina, std::abs(stamina));
  }
}

static std::map<RE::Actor*, std::shared_ptr<ResourceDrain>> rm_map;

auto weap_actor_mask_multiply(const FormMask& matrix1, const ActorMask& matrix2) -> std::unique_ptr<FormMask> {
  constexpr auto row = 1;

  FormMask result{{{0, 0, 0}}};
  for (auto x = 0; x < row; x++) {

    constexpr auto column = 3;

    for (auto y = 0; y < column; y++) {
      auto acc = 0i16;
      for (auto z = 0; z < 3; z++) {
        acc = acc + matrix1.at(x).at(z) * matrix2.at(z).at(y);
      }
      result.at(x).at(y) = acc;
    }
  }
  return std::make_unique<FormMask>(result);
}

auto handle_mask_sum_for_drain_values(std::int32_t mask_sum, float cost) -> std::shared_ptr<ResourceDrain> {
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

auto get_form_mask(const RE::BGSKeywordForm& form, const Config& config) -> std::unique_ptr<FormMask> {
  FormMask f_mask{{{0, 0, 0}}};
  logger::debug("start get weapon keyword"sv);
  f_mask.at(0).at(0) = 1;
  if (form.HasKeyword(config.resource_manager_health_kw)) {
    f_mask.at(0).at(0) = 0;
    f_mask.at(0).at(1) = 1;
  }
  if (form.HasKeyword(config.resource_manager_magicka_kw)) {
    f_mask.at(0).at(0) = 0;
    f_mask.at(0).at(2) = 1;
  }
  if (form.HasKeyword(config.resource_manager_stamina_kw)) {
    f_mask.at(0).at(0) = 1;
  }
  logger::debug("end get weapon keyword"sv);
  return std::make_unique<FormMask>(f_mask);
}

auto calc_mask_sum(const FormMask& f_mask) -> std::int32_t {
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

auto get_drain_value(RE::Actor& actor, const RE::BGSKeywordForm& form, const Config& config, const float cost,
                     const bool enable_conversions) -> std::shared_ptr<ResourceDrain> {
  const auto f_mask = get_form_mask(form, config);

  auto mask_sum = 0;

  if (!enable_conversions) {
    mask_sum = calc_mask_sum(*f_mask);

    return handle_mask_sum_for_drain_values(mask_sum, cost);
  }

  const auto stamina_to_health = Core::has_absolute_keyword(actor, *config.resource_manager_convert_stamina_health_kw);
  logger::debug("key conversion1"sv);
  const auto stamina_to_magicka =
      Core::has_absolute_keyword(actor, *config.resource_manager_convert_stamina_magicka_kw);
  logger::debug("key conversion2"sv);
  const auto health_to_stamina = Core::has_absolute_keyword(actor, *config.resource_manager_convert_health_stamina_kw);
  logger::debug("key conversion3"sv);
  const auto health_to_magicka = Core::has_absolute_keyword(actor, *config.resource_manager_convert_health_magicka_kw);
  logger::debug("key conversion4"sv);
  const auto magicka_to_stamina =
      Core::has_absolute_keyword(actor, *config.resource_manager_convert_magicka_stamina_kw);
  logger::debug("key conversion5"sv);
  const auto magicka_to_health = Core::has_absolute_keyword(actor, *config.resource_manager_convert_magicka_health_kw);
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

auto get_bash_drain_value(RE::Actor& actor, const WeaponOrArmor& form, const Config& config, const float cost,
                          const bool enable_conversions) -> std::shared_ptr<ResourceDrain> {
  if (form.is_l) {
    return get_drain_value(actor, *form.left, config, cost, enable_conversions);
  } else {
    return get_drain_value(actor, *form.right, config, cost, enable_conversions);
  }
}

auto get_weapon_weight(const RE::TESObjectWEAP& weapon, const Config& config) -> float {

  logger::debug("get weight of weapon"sv);
  const auto weight = weapon.weight;
  if (weight > 0.f) {
    return weight * config.resource_manager_weight_mult;
  }

  logger::debug("get damage of weapon"sv);
  const auto damage = static_cast<float>(weapon.attackDamage) * config.resource_manager_damage_mult;
  if (damage > 0.f) {
    return damage * config.resource_manager_weight_mult;
  }

  logger::debug("get critical damage of weapon"sv);
  const auto critical_damage = static_cast<float>(weapon.criticalData.damage) * config.resource_manager_damage_mult;
  if (critical_damage > 0.f) {
    return critical_damage * config.resource_manager_weight_mult;
  }

  logger::debug("return fallback"sv);
  // 5.f value as fallback
  return 5.f;
}

auto get_shield_weight(const RE::TESObjectARMO& shield, const Config& config) -> float {
  const auto weight = shield.weight;
  if (weight > 0.f) {
    return weight * config.resource_manager_weight_mult;
  }

  const auto armor = static_cast<float>(shield.armorRating);
  if (armor > 0.f) {
    return armor * config.resource_manager_weight_mult * config.resource_manager_armor_mult;
  }

  // 5.f value as fallback
  return 5.f;
}

auto get_sale_for_spend_from_av(std::int32_t av_value, std::int32_t high_value, const std::int32_t low_value) -> float {
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

auto get_attack_drain_cost(RE::Actor& actor, const RE::TESObjectWEAP& weapon, const bool is_power_attack,
                           const Config& config) -> float {

  logger::debug("Get actor infamy for actor: {} with level: {}"sv, actor.GetName(), actor.GetLevel());
  auto infamy = actor.GetActorValue(RE::ActorValue::kInfamy);
  if (infamy < 1.f || !config.resource_manager_infamy_enable) {
    infamy = 1.f;
  }

  logger::debug("Get actor power attack sale mult"sv);
  float power_attack_cost_mult = 0.f;
  if (is_power_attack) {
    const auto av_value = static_cast<std::int32_t>(actor.GetActorValue(config.resource_manager_power_attack_cost_av));
    power_attack_cost_mult = get_sale_for_spend_from_av(av_value, config.resource_manager_power_attack_cost_high,
                                                        config.resource_manager_power_attack_cost_low);
  }

  logger::debug("Get actor normal attack av cost sale"sv);
  const auto av_value         = static_cast<std::int32_t>(actor.GetActorValue(config.resource_manager_attack_cost_av));
  auto       attack_cost_mult = get_sale_for_spend_from_av(av_value, config.resource_manager_attack_cost_high,
                                                           config.resource_manager_attack_cost_low);

  logger::debug("Get actor weapon weight"sv);
  const auto weapon_weight = get_weapon_weight(weapon, config);

  logger::debug("attack cost mult: {}, power attack cost mult: {}"sv, attack_cost_mult, power_attack_cost_mult);

  const auto cost = (weapon_weight * attack_cost_mult * infamy) +
                    (weapon_weight * infamy * power_attack_cost_mult * config.resource_manager_power_attack_mult);

  return cost * config.resource_manager_global_mult;
}

auto get_bash_drain_cost(RE::Actor& actor, const WeaponOrArmor& form, const bool is_power_attack, const Config& config)
    -> float {
  auto infamy = actor.GetActorValue(RE::ActorValue::kInfamy);
  if (infamy < 1.f || !config.resource_manager_infamy_enable) {
    infamy = 1.f;
  }

  float power_bash_cost_mult = 0.f;
  if (is_power_attack) {
    const auto av_value  = static_cast<std::int32_t>(actor.GetActorValue(RE::ActorValue::kBlock) * 0.25f);
    power_bash_cost_mult = get_sale_for_spend_from_av(av_value, 100, -100);
  }

  const auto av_value       = static_cast<std::int32_t>(actor.GetActorValue(RE::ActorValue::kBlock) * 0.5f);
  auto       bash_cost_mult = get_sale_for_spend_from_av(av_value, 100, -100);

  auto weight = 0.f;
  if (form.is_l) {
    weight = get_weapon_weight(*form.left, config);
  }
  if (!form.is_l) {
    weight = get_shield_weight(*form.right, config);
  }

  logger::debug("bash cost mult: {}, power bash cost mult: {}"sv, bash_cost_mult, power_bash_cost_mult);

  const auto cost = (weight * bash_cost_mult * infamy) +
                    (weight * infamy * power_bash_cost_mult * config.resource_manager_power_attack_mult);

  return cost * config.resource_manager_global_mult;
}

auto melee_weapon_spend(RE::Actor& actor, const RE::TESObjectWEAP& weapon, const bool is_power_attack,
                        const Config& config) -> void {
  logger::debug("melee_weapon_spend"sv);
  auto cost = get_attack_drain_cost(actor, weapon, is_power_attack, config);
  logger::debug("end drain cost: cost {}"sv, cost);
  const auto drain_value = get_drain_value(actor, weapon, config, cost, true);
  logger::debug("end drain value"sv);
  drain_value->drain(actor);
  logger::debug("end drain"sv);
  return;
}

auto bash_spend(RE::Actor& actor, const WeaponOrArmor& form, const bool is_power_attack, const Config& config) -> void {
  logger::debug("bash_spend"sv);
  auto cost = get_bash_drain_cost(actor, form, is_power_attack, config);
  logger::debug("end drain cost: cost {}"sv, cost);
  const auto drain_value = get_bash_drain_value(actor, form, config, cost, false);
  logger::debug("end drain value"sv);
  drain_value->drain(actor);
  logger::debug("end drain"sv);
  return;
}

auto ranged_weapon_spend(RE::Actor& actor, const RE::TESObjectWEAP& weapon, const Config& config) -> void {
  const auto cost        = get_attack_drain_cost(actor, weapon, false, config) * 0.1f;
  const auto drain_value = get_drain_value(actor, weapon, config, cost, true);
  drain_value->drain(actor);
  rm_map[&actor] = drain_value;
  return;
}

auto ranged_spend_handler() -> void {
  for (auto& [actor, drain_value] : rm_map) {
    if (actor && drain_value) {
      const auto state   = actor->actorState1.meleeAttackState;
      auto       is_draw = state == RE::ATTACK_STATE_ENUM::kBowDraw || state == RE::ATTACK_STATE_ENUM::kBowDrawn ||
                     state == RE::ATTACK_STATE_ENUM::kBowAttached;
      logger::debug("actor with level: {} is drawn: {}"sv, actor->GetLevel(), is_draw);
      logger::debug("actor state: {}"sv, static_cast<std::uint32_t>(actor->actorState1.meleeAttackState));
      if (is_draw) {
        drain_value->drain(*actor);
      } else {
        rm_map.erase(actor);
      }
    } else {
      rm_map.erase(actor);
    }
  }
}

auto jump_spend(RE::Actor& actor, const Config& config) -> void {
  spend_actor_value(actor, RE::ActorValue::kStamina, -config.resource_manager_jump_cost);
}

auto get_weapon(const RE::Actor& actor, const bool is_left_hand, const Config& config) -> RE::TESObjectWEAP& {
  logger::debug("get weapon start"sv);
  const auto weapon = actor.GetEquippedObject(is_left_hand);
  if (!weapon) {
    return *config.resource_manager_unarmed_weapon;
  }

  const auto as_weapon = weapon->As<RE::TESObjectWEAP>();

  if (!as_weapon) {
    return *config.resource_manager_unarmed_weapon;
  }

  return *as_weapon;
}

auto get_weapon_or_shield(const RE::Actor& actor) -> std::optional<WeaponOrArmor> {
  logger::debug("get weapon or shield start"sv);
  const auto form = actor.GetEquippedObject(false);
  if (!form) {
    return std::nullopt;
  }

  if (const auto weapon = form->As<RE::TESObjectWEAP>()) {
    auto either = Reflyem::Core::Either<RE::TESObjectWEAP*, RE::TESObjectARMO*>(weapon);
    return {either};
  }
  if (const auto armo = form->As<RE::TESObjectARMO>()) {
    auto either = Reflyem::Core::Either<RE::TESObjectWEAP*, RE::TESObjectARMO*>(armo);
    return {either};
  }

  return std::nullopt;
}

auto handle_resource_for_action(RE::Character& actor, const ResourceDrain& drain_values, RE::SpellItem& block_spell)
    -> void {

  logger::debug("handle_resource_for_action get AVs"sv);
  auto magicka = actor.GetActorValue(RE::ActorValue::kMagicka);
  auto health  = actor.GetActorValue(RE::ActorValue::kHealth);
  auto stamina = actor.GetActorValue(RE::ActorValue::kStamina);

  logger::debug("handle_resource_for_action get values drain"sv);
  auto d_magicka = std::abs(drain_values.magicka);
  auto d_health  = std::abs(drain_values.health);
  auto d_stamina = std::abs(drain_values.stamina);

  logger::debug("handle_resource_for_action, magicka: {} health: {} stamina: {} dv m h s: {} {} {}"sv, magicka, health,
                stamina, d_magicka, d_health, d_stamina);

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

auto update_actor(RE::Character& character, float, const Config& config) -> void {

  if (config.resource_manager_weapon_spend_enable) {
    logger::debug("Update actor in resource get weapon"sv);
    const auto& attack_weapon = get_weapon(character, false, config);

    logger::debug("Update actor in resource get weapon costs"sv);
    const auto normal_attack_cost = get_attack_drain_cost(character, attack_weapon, false, config);
    const auto power_attack_cost  = get_attack_drain_cost(character, attack_weapon, true, config);

    logger::debug("Update actor in resource get weapon drains"sv);
    const auto normal_attack_drain_value = get_drain_value(character, attack_weapon, config, normal_attack_cost, true);
    const auto power_attack_drain_value  = get_drain_value(character, attack_weapon, config, power_attack_cost, true);

    logger::debug("Update actor in resource before handle resource"sv);
    handle_resource_for_action(character, *normal_attack_drain_value, *config.resource_manage_spell_block_attack);
    handle_resource_for_action(character, *power_attack_drain_value, *config.resource_manage_spell_block_power_attack);
  }

  if (config.resource_manager_bash_spend_enable) {

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
    handle_resource_for_action(character, *bash_drain_value, *config.resource_manage_spell_block_bash);
  }
}

auto animation_handler(const AnimationEventHandler::AnimationEvent animation, RE::Actor& actor,
                       const bool is_power_attack, const Config& config) -> void {
  switch (animation) {
  case AnimationEventHandler::AnimationEvent::kBashExit: {
    if (!config.resource_manager_bash_spend_enable) {
      return;
    }

    const auto weapon_or_shield = get_weapon_or_shield(actor);
    if (!weapon_or_shield.has_value()) {
      return;
    }

    const auto& value = weapon_or_shield.value();
    logger::debug("get value from option (weapon or shield)"sv);
    bash_spend(actor, value, is_power_attack, config);
    return;
  }
  case AnimationEventHandler::AnimationEvent::kWeaponSwing: {
    if (!config.resource_manager_weapon_spend_enable) {
      return;
    }

    const auto& weapon = get_weapon(actor, false, config);
    logger::debug("get weapon end");
    melee_weapon_spend(actor, weapon, is_power_attack, config);
    return;
  }
  case AnimationEventHandler::AnimationEvent::kWeaponLeftSwing: {
    if (!config.resource_manager_weapon_spend_enable) {
      return;
    }

    const auto& weapon = get_weapon(actor, true, config);
    logger::debug("get weapon end"sv);
    melee_weapon_spend(actor, weapon, is_power_attack, config);
    return;
  }
  case AnimationEventHandler::AnimationEvent::kBowDrawStart: {
    if (!config.resource_manager_weapon_spend_enable) {
      return;
    }

    const auto& weapon = get_weapon(actor, false, config);
    logger::debug("get weapon end"sv);
    ranged_weapon_spend(actor, weapon, config);
    return;
  }
  case AnimationEventHandler::AnimationEvent::kJumpUp: {
    jump_spend(actor, config);
    return;
  }
  case AnimationEventHandler::AnimationEvent::kNone:
  default: {
    return;
  }
  }
}

auto handle_block_hit(RE::Actor& target, float total_damage, const float av_value, const float damage_mult,
                      const RE::ActorValue av) -> float {
  if (total_damage >= av_value) {
    total_damage = av_value;
    target.RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, av, -av_value);
  } else {
    target.RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, av, -total_damage);
  }
  return total_damage / damage_mult;
}

auto handle_hit_data(RE::HitData& hit_data, const float av_part_damage) -> void {
  hit_data.totalDamage -= av_part_damage;
  if (hit_data.totalDamage < 0.f) {
    hit_data.totalDamage = 0.f;
  }
  return;
}

auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Config& config) -> void {
  logger::debug("on_weapon_hit resource manager"sv);

  if (!hit_data.flags.any(RE::HitData::Flag::kBlocked) || hit_data.totalDamage <= 0.f) {
    return;
  }

  logger::debug("on_weapon_hit blocked hit"sv);

  const auto form = target->GetEquippedObject(false);
  if (!form) {
    return;
  }

  logger::debug("success get form"sv);

  const auto keyword_form = form->As<RE::BGSKeywordForm>();
  if (!keyword_form) {
    return;
  }

  logger::debug("success get keyword form"sv);

  const auto f_mask = get_form_mask(*keyword_form, config);

  logger::debug("mask values: {} {} {}"sv, f_mask->at(0).at(0), f_mask->at(0).at(1), f_mask->at(0).at(2));

  const auto stamina = target->GetActorValue(RE::ActorValue::kStamina);
  const auto health  = target->GetActorValue(RE::ActorValue::kHealth);
  const auto magicka = target->GetActorValue(RE::ActorValue::kMagicka);

  auto damage_mult  = Core::getting_damage_mult(*target);
  auto total_damage = hit_data.totalDamage * damage_mult;

  auto mask_sum = calc_mask_sum(*f_mask);

  logger::debug("mask_sum {} damage_mult {} total_damage {}"sv, mask_sum, damage_mult, total_damage);

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
        handle_block_hit(*target, total_damage * 0.5f, health, damage_mult, RE::ActorValue::kHealth);
    handle_hit_data(hit_data, health_part_damage);

    const auto stamina_part_damage =
        handle_block_hit(*target, total_damage * 0.5f, stamina, damage_mult, RE::ActorValue::kStamina);
    handle_hit_data(hit_data, stamina_part_damage);
    return;
  }
  case 5: // magicka stamina
  {
    const auto magicka_part_damage =
        handle_block_hit(*target, total_damage * 0.5f, magicka, damage_mult, RE::ActorValue::kMagicka);
    handle_hit_data(hit_data, magicka_part_damage);

    const auto stamina_part_damage =
        handle_block_hit(*target, total_damage * 0.5f, stamina, damage_mult, RE::ActorValue::kStamina);
    handle_hit_data(hit_data, stamina_part_damage);
    return;
  }
  case 6: // health magicka
  {
    const auto health_part_damage =
        handle_block_hit(*target, total_damage * 0.5f, health, damage_mult, RE::ActorValue::kHealth);
    handle_hit_data(hit_data, health_part_damage);

    const auto magicka_part_damage =
        handle_block_hit(*target, total_damage * 0.5f, magicka, damage_mult, RE::ActorValue::kMagicka);
    handle_hit_data(hit_data, magicka_part_damage);
    return;
  }
  case 7: // health magicka stamina
  {
    const auto health_part_damage =
        handle_block_hit(*target, total_damage * 0.35f, health, damage_mult, RE::ActorValue::kHealth);
    handle_hit_data(hit_data, health_part_damage);

    const auto magicka_part_damage =
        handle_block_hit(*target, total_damage * 0.35f, magicka, damage_mult, RE::ActorValue::kMagicka);
    handle_hit_data(hit_data, magicka_part_damage);

    const auto stamina_part_damage =
        handle_block_hit(*target, total_damage * 0.35f, stamina, damage_mult, RE::ActorValue::kStamina);
    handle_hit_data(hit_data, stamina_part_damage);
    return;
  }
  default: {
  }
  }
}

} // namespace ResourceManager
} // namespace Reflyem