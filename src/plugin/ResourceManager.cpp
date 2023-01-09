#include "ResourceManager.h"
#include "Core.h"

namespace Reflyem
{
  namespace ResourceManager
  {

    using WeaponOrArmor = Reflyem::Core::Either<RE::TESObjectWEAP*, RE::TESObjectARMO*>;

    static std::map<RE::Actor*, std::shared_ptr<DrainValues>> rm_map;

    DrainValues::DrainValues(float a_stamina, float a_health, float a_magicka)
    {
      logger::debug("create drain values: S{} H{} M{}", a_stamina, a_health, a_magicka);
      stamina = a_stamina * -1.f;
      health = a_health * -1.f;
      magicka = a_magicka * -1.f;
    }

    auto DrainValues::drain(RE::Actor& actor) -> void
    {
      logger::debug("drain values: S{} H{} M{}", stamina, health, magicka);
      actor.RestoreActorValue(
        RE::ACTOR_VALUE_MODIFIER::kDamage,
        RE::ActorValue::kHealth, health);
      actor.RestoreActorValue(
        RE::ACTOR_VALUE_MODIFIER::kDamage,
        RE::ActorValue::kMagicka, magicka);
      actor.RestoreActorValue(
        RE::ACTOR_VALUE_MODIFIER::kDamage,
        RE::ActorValue::kStamina, stamina);
    }

    auto weap_actor_mask_multiply(FormMask& matrix1, ActorMask& matrix2) -> std::unique_ptr<FormMask>
    {

      const auto row = 1;
      const auto column = 3;

      FormMask result{ { {0,0,0} } };
      for (auto x = 0; x < row; x++)
      {
        for (auto y = 0; y < column; y++)
        {
          std::int16_t acc = 0;
          for (auto z = 0; z < 3; z++)
          {
            acc = acc + matrix1.at(x).at(z) * matrix2.at(z).at(y);
          }
          result.at(x).at(y) = acc;
        }
      }
      return std::make_unique<FormMask>(result);
    }

    auto handle_mask_sum_for_drain_values(std::int32_t mask_sum, float cost) -> std::shared_ptr<DrainValues>
    {
      switch (mask_sum)
      {
      case 1:
        return std::make_shared<DrainValues>(DrainValues(cost, 0, 0));
      case 2:
        return std::make_shared<DrainValues>(DrainValues(0, cost, 0));
      case 4:
        return std::make_shared<DrainValues>(DrainValues(0, 0, cost));
      case 3:
        cost = cost * 0.5f;
        return std::make_shared<DrainValues>(DrainValues(cost, cost, 0));
      case 5:
        cost = cost * 0.5f;
        return std::make_shared<DrainValues>(DrainValues(cost, 0, cost));
      case 6:
        cost = cost * 0.5f;
        return std::make_shared<DrainValues>(DrainValues(0, cost, cost));
      case 7:
        cost = cost * 0.35f;
        return std::make_shared<DrainValues>(DrainValues(cost, cost, cost));
      default:
        logger::debug("default switch on eval drain value: mask {}", mask_sum);
        return std::make_shared<DrainValues>(DrainValues(cost, 0, 0));
      }
    }

    auto get_form_mask(RE::BGSKeywordForm& form, const Reflyem::Config& config) -> std::unique_ptr<FormMask>
    {
      FormMask f_mask{ {{0,0,0}} };
      logger::debug("start get weapon keyword");
      f_mask.at(0).at(0) = 1;
      if (form.HasKeyword(config.resource_manager_health_kw))
      {
        f_mask.at(0).at(0) = 0;
        f_mask.at(0).at(1) = 1;
      }
      if (form.HasKeyword(config.resource_manager_magicka_kw))
      {
        f_mask.at(0).at(0) = 0;
        f_mask.at(0).at(2) = 1;
      }
      if (form.HasKeyword(config.resource_manager_stamina_kw))
      {
        f_mask.at(0).at(0) = 1;
      }
      logger::debug("end get weapon keyword");
      return std::make_unique<FormMask>(f_mask);
    }

    auto calc_mask_sum(FormMask& f_mask) -> std::int32_t
    {
      auto mask_sum = 0;
      if (f_mask.at(0).at(0) > 0) { mask_sum += 1; }
      if (f_mask.at(0).at(1) > 0) { mask_sum += 2; }
      if (f_mask.at(0).at(2) > 0) { mask_sum += 4; }
      return mask_sum;
    }

    auto get_drain_value(
      RE::Actor& actor,
      RE::BGSKeywordForm& form,
      const Reflyem::Config config,
      float cost,
      bool enable_conversions) -> std::shared_ptr<DrainValues>
    {
      
      auto f_mask = get_form_mask(form, config);

      auto mask_sum = 0;

      if (!enable_conversions)
      {
        mask_sum = calc_mask_sum(*f_mask);

        return handle_mask_sum_for_drain_values(mask_sum, cost);

      }

      auto stamina_to_health =
        Reflyem::Core::has_absolute_keyword(actor, *config.resource_manager_convert_stamina_health_kw);
      logger::debug("key conversion1");
      auto stamina_to_magicka =
        Reflyem::Core::has_absolute_keyword(actor, *config.resource_manager_convert_stamina_magicka_kw);
      logger::debug("key conversion2");
      auto health_to_stamina =
        Reflyem::Core::has_absolute_keyword(actor, *config.resource_manager_convert_health_stamina_kw);
      logger::debug("key conversion3");
      auto health_to_magicka =
        Reflyem::Core::has_absolute_keyword(actor, *config.resource_manager_convert_health_magicka_kw);
      logger::debug("key conversion4");
      auto magicka_to_stamina =
        Reflyem::Core::has_absolute_keyword(actor, *config.resource_manager_convert_magicka_stamina_kw);
      logger::debug("key conversion5");
      auto magicka_to_health =
        Reflyem::Core::has_absolute_keyword(actor, *config.resource_manager_convert_magicka_health_kw);
      logger::debug("end get conversions");

      ActorMask a_mask
      { {{1, 0, 0},
         {0, 1, 0},
         {0, 0, 1}} };

      logger::debug("start mutate actor mask");
      if (stamina_to_health) { a_mask.at(0) = { {0, 1, 0} }; }
      if (stamina_to_magicka) { a_mask.at(0) = { {0, 0, 1} }; }
      if (health_to_magicka) { a_mask.at(1) = { {0, 0, 1} }; }
      if (health_to_stamina) { a_mask.at(1) = { {1, 0, 0} }; }
      if (magicka_to_health) { a_mask.at(2) = { {0, 1, 0} }; }
      if (magicka_to_stamina) { a_mask.at(2) = { {1, 0, 0} }; }
      logger::debug("end mutate actor mask");

      logger::debug("before start multiply");
      auto mask = weap_actor_mask_multiply(*f_mask, a_mask);
      logger::debug("before end multiply");

      mask_sum = calc_mask_sum(*mask);

      return handle_mask_sum_for_drain_values(mask_sum, cost);

    }

    auto get_bash_drain_value(
      RE::Actor& actor,
      WeaponOrArmor& form,
      const Reflyem::Config config,
      float cost,
      bool enable_conversions) -> std::shared_ptr<DrainValues>
    {
      if (form.isL)
      {
        return get_drain_value(actor, *form.left, config, cost, enable_conversions);
      }
      else
      {
        return get_drain_value(actor, *form.right, config, cost, enable_conversions);
      }
    }

    auto get_weapon_weight(
      RE::TESObjectWEAP& weapon,
      const Reflyem::Config& config) -> float
    {

      auto weight = weapon.weight;
      if (weight > 0.f)
      {
        return weight * config.resource_manager_weight_mult;
      }

      auto damage = 
        weapon.attackDamage * config.resource_manager_damage_mult;
      if (damage > 0.f) {
        return damage * config.resource_manager_weight_mult;
      }

      auto critical_damage =
        weapon.criticalData.damage * config.resource_manager_damage_mult;
      if (critical_damage > 0.f)
      {
        return critical_damage * config.resource_manager_weight_mult;
      }

      // 5.f value as fallback
      return 5.f;

    }

    auto get_shield_weight(
      RE::TESObjectARMO& shield,
      const Reflyem::Config& config) -> float
    {
      auto weight = shield.weight;
      if (weight > 0.f)
      {
        return weight * config.resource_manager_weight_mult;
      }

      auto armor = shield.armorRating;
      if (armor > 0.f)
      {
        return armor * config.resource_manager_weight_mult * config.resource_manager_armor_mult;
      }

      // 5.f value as fallback
      return 5.f;

    }

    auto get_sale_for_spend_from_av(
      std::int32_t av_value,
      std::int32_t high_value,
      std::int32_t low_value) -> float
    {
      if (high_value > 100) { high_value = 100; }
      if (av_value > high_value) { av_value = high_value; }
      if (av_value < low_value) { av_value = low_value; }

      return 1.f - (av_value / 100.f);

    }

    auto get_attack_drain_cost(
      RE::Actor& actor,
      RE::TESObjectWEAP& weapon,
      bool is_power_attack,
      const Reflyem::Config& config) -> float
    {

      auto infamy = actor.GetActorValue(RE::ActorValue::kInfamy);
      if (infamy < 1.f || !config.resource_manager_infamy_enable)
      {
        infamy = 1.f;
      }

      float power_attack_cost_mult = 0.f;
      if (is_power_attack)
      {
        auto av_value = 
          static_cast<std::int32_t>(actor.GetActorValue(config.resource_manager_power_attack_cost_av));
        power_attack_cost_mult =
          get_sale_for_spend_from_av(
            av_value,
            config.resource_manager_power_attack_cost_high,
            config.resource_manager_power_attack_cost_low);
      }

      auto av_value =
        static_cast<std::int32_t>(actor.GetActorValue(config.resource_manager_attack_cost_av));
      auto attack_cost_mult =
        get_sale_for_spend_from_av(
          av_value,
          config.resource_manager_attack_cost_high,
          config.resource_manager_attack_cost_low);

      auto weapon_weight = get_weapon_weight(weapon, config);

      logger::debug("attack cost mult: {}, power attack cost mult: {}", attack_cost_mult, power_attack_cost_mult);

      auto cost =
        (weapon_weight * attack_cost_mult * infamy)
        + (weapon_weight * infamy * power_attack_cost_mult * config.resource_manager_power_attack_mult);

      return cost * config.resource_manager_global_mult;

    }

    auto get_bash_drain_cost(
      RE::Actor& actor,
      WeaponOrArmor& form,
      bool is_power_attack,
      const Reflyem::Config& config) -> float
    {

      auto infamy = actor.GetActorValue(RE::ActorValue::kInfamy);
      if (infamy < 1.f || !config.resource_manager_infamy_enable)
      {
        infamy = 1.f;
      }

      float power_bash_cost_mult = 0.f;
      if (is_power_attack)
      {
        auto av_value =
          static_cast<std::int32_t>(actor.GetActorValue(RE::ActorValue::kBlock) * 0.25);
        power_bash_cost_mult =
          get_sale_for_spend_from_av(
            av_value,
            100,
            -100);
      }

      auto av_value =
        static_cast<std::int32_t>(actor.GetActorValue(RE::ActorValue::kBlock) * 0.5);
      auto bash_cost_mult =
        get_sale_for_spend_from_av(av_value, 100, -100);

      auto weight = 0.f;
      if (form.isL)
      {
        weight = get_weapon_weight(*form.left, config);
      }
      if (!form.isL)
      {
        weight = get_shield_weight(*form.right, config);
      }

      logger::debug("bash cost mult: {}, power bash cost mult: {}", bash_cost_mult, power_bash_cost_mult);

      auto cost =
        (weight * bash_cost_mult * infamy)
        + (weight * infamy * power_bash_cost_mult * config.resource_manager_power_attack_mult);

      return cost * config.resource_manager_global_mult;

    }

    auto melee_weapon_spend(RE::Actor& actor,
      RE::TESObjectWEAP& weapon,
      bool is_power_attack,
      const Reflyem::Config config) -> void
    {
      logger::debug("melee_weapon_spend");
      auto cost = get_attack_drain_cost(actor, weapon, is_power_attack, config);
      logger::debug("end drain cost: cost {}", cost);
      auto drain_value = get_drain_value(actor, weapon, config, cost, true);
      logger::debug("end drain value");
      drain_value->drain(actor);
      logger::debug("end drain");
      return;
    }

    auto bash_spend(RE::Actor& actor,
      WeaponOrArmor& form,
      bool is_power_attack,
      const Reflyem::Config config) -> void
    {
      logger::debug("bash_spend");
      auto cost = get_bash_drain_cost(actor, form, is_power_attack, config);
      logger::debug("end drain cost: cost {}", cost);
      auto drain_value = get_bash_drain_value(actor, form, config, cost, false);
      logger::debug("end drain value");
      drain_value->drain(actor);
      logger::debug("end drain");
      return;
    }

     auto ranged_weapon_spend(RE::Actor& actor,
       RE::TESObjectWEAP& weapon,
       const Reflyem::Config config) -> void
     {
       auto cost = get_attack_drain_cost(actor, weapon, false, config) * 0.1f;
       auto drain_value = get_drain_value(actor, weapon, config, cost, true);
       drain_value->drain(actor);
       rm_map[&actor] = drain_value;
       return;
     }

     auto ranged_spend_handler() -> void
     {
       for (auto& [actor, drain_value] : rm_map)
       {
         if (actor && drain_value)
         {
           auto state = actor->actorState1.meleeAttackState;
             auto isDraw = state == RE::ATTACK_STATE_ENUM::kBowDraw
             || state == RE::ATTACK_STATE_ENUM::kBowDrawn
             || state == RE::ATTACK_STATE_ENUM::kBowAttached;
           logger::debug("actor with level: {} is drawn: {}", actor->GetLevel(), isDraw);
           logger::debug("actor state: {}", static_cast<std::uint32_t>(actor->actorState1.meleeAttackState));
           if (isDraw)
           {
             drain_value->drain(*actor);
           }
           else
           {
             rm_map.erase(actor);
           }
         }
         else
         {
           rm_map.erase(actor);
         }
       }
     }

    auto jump_spend(RE::Actor& actor, const Reflyem::Config config) -> void
    {
      actor.RestoreActorValue(
        RE::ACTOR_VALUE_MODIFIER::kDamage, 
        RE::ActorValue::kStamina, 
        -config.resource_manager_jump_cost);
    }

    auto get_weapon(RE::Actor& actor,
      bool is_left_hand,
      const Reflyem::Config config) -> RE::TESObjectWEAP&
    {
      logger::debug("get weapon start");
      auto weapon = actor.GetEquippedObject(is_left_hand);
      if (weapon)
      {
        return *weapon->As< RE::TESObjectWEAP>();
      }
      else
      {
        return *config.resource_manager_unarmed_weapon;
      }
    }

    auto get_weapon_or_shield(RE::Actor& actor) -> std::optional<WeaponOrArmor>
    {
      logger::debug("get weapon or shield start");
      auto form = actor.GetEquippedObject(false);
      if (!form) { return std::nullopt; }

      auto weapon = form->As<RE::TESObjectWEAP>();
      if (weapon)
      {
        auto either = Reflyem::Core::Either<RE::TESObjectWEAP*, RE::TESObjectARMO*>(weapon);
        return std::optional<WeaponOrArmor>(either);
      }
      auto armo = form->As<RE::TESObjectARMO>();
      if (armo)
      {
        auto either = Reflyem::Core::Either<RE::TESObjectWEAP*, RE::TESObjectARMO*>(armo);
        return std::optional<WeaponOrArmor>(either);
      }

      return std::nullopt;
    }

    auto handler(
      Reflyem::AnimationEventHandler::AnimationEvent animation,
      RE::Actor& actor,
      bool is_power_attack,
      const Reflyem::Config config) -> void
    {

      switch (animation)
      {
      case Reflyem::AnimationEventHandler::AnimationEvent::kBashExit:
      {
        auto weapon_or_shield = get_weapon_or_shield(actor);
        if (!weapon_or_shield.has_value()) { return; }

        auto& value = weapon_or_shield.value();
        logger::debug("get value from option (weapon or shield)");
        bash_spend(actor, value, is_power_attack, config);
        return;
      }
      case Reflyem::AnimationEventHandler::AnimationEvent::kWeaponSwing:
      {
        auto& weapon = get_weapon(actor, false, config);
        logger::debug("get weapon end");
        melee_weapon_spend(actor, weapon, is_power_attack, config);
        return;
      }
      case Reflyem::AnimationEventHandler::AnimationEvent::kWeaponLeftSwing:
      {
        auto& weapon = get_weapon(actor, true, config);
        logger::debug("get weapon end");
        melee_weapon_spend(actor, weapon, is_power_attack, config);
        return;
      }
      case Reflyem::AnimationEventHandler::AnimationEvent::kBowDrawStart:
      {
        auto& weapon = get_weapon(actor, false, config);
        logger::debug("get weapon end");
        ranged_weapon_spend(actor, weapon, config);
        return;
      }
      case Reflyem::AnimationEventHandler::AnimationEvent::kJumpUp:
      {
        jump_spend(actor, config);
        return;
      }
      case Reflyem::AnimationEventHandler::AnimationEvent::kNone:
        return;
      default:
        return;
      }
    }

    auto handle_block_hit(
      RE::Actor& target,
      float total_damage,
      float av_value,
      float damage_mult,
      RE::ActorValue av) -> float
    {
      if (total_damage >= av_value)
      {
        total_damage = av_value;
        target.RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, av, -av_value);
      }
      else
      {
        target.RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, av, -total_damage);
      }
      return total_damage / damage_mult;
    }

    auto handle_hit_data(RE::HitData& hit_data, float av_part_damge) -> void
    {
      hit_data.totalDamage -= av_part_damge;
      if (hit_data.totalDamage < 0.f)
      {
        hit_data.totalDamage = 0.f;
      }
      return;
    }

    auto on_weapon_hit(RE::Actor* target, RE::HitData& hit_data, const Reflyem::Config& config) -> void
    {

      logger::debug("on_weapon_hit resource manager");

      if (!hit_data.flags.any(RE::HitData::Flag::kBlocked) || hit_data.totalDamage <= 0.f) { return; }

      logger::debug("on_weapon_hit blocked hit");

      auto form = target->GetEquippedObject(false);
      if (!form) { return; }

      logger::debug("success get form");

      auto keyword_form = form->As<RE::BGSKeywordForm>();
      if (!keyword_form) { return; }

      logger::debug("success get keyword form");

      auto f_mask = get_form_mask(*keyword_form, config);

      logger::debug("mask values: {} {} {}", f_mask->at(0).at(0), f_mask->at(0).at(1), f_mask->at(0).at(2));

      auto stamina = target->GetActorValue(RE::ActorValue::kStamina);
      auto health = target->GetActorValue(RE::ActorValue::kHealth);
      auto magicka = target->GetActorValue(RE::ActorValue::kMagicka);

      auto damage_mult = Reflyem::Core::getting_damage_mult(*target);
      auto total_damage = hit_data.totalDamage * damage_mult;

      auto mask_sum = calc_mask_sum(*f_mask);

      logger::debug("mask_sum {} damage_mult {} total_damage {}", mask_sum, damage_mult, total_damage);

      switch (mask_sum)
      {
      case 1: // stamina
      {
        auto stamina_part_damage =
          handle_block_hit(*target, total_damage, stamina, damage_mult, RE::ActorValue::kStamina);
        handle_hit_data(hit_data, stamina_part_damage);
        return;
      }
      case 2: // health
      {
        auto health_part_damage =
          handle_block_hit(*target, total_damage, health, damage_mult, RE::ActorValue::kHealth);
        handle_hit_data(hit_data, health_part_damage);
        return;
      }
      case 4: // magicka
      {
        auto magicka_part_damage =
          handle_block_hit(*target, total_damage, magicka, damage_mult, RE::ActorValue::kMagicka);
        handle_hit_data(hit_data, magicka_part_damage);
        return;
      }
      case 3: // health stamina
      {
        auto health_part_damage =
          handle_block_hit(*target, total_damage * 0.5f, health, damage_mult, RE::ActorValue::kHealth);
        handle_hit_data(hit_data, health_part_damage);

        auto stamina_part_damage =
          handle_block_hit(*target, total_damage * 0.5f, stamina, damage_mult, RE::ActorValue::kStamina);
        handle_hit_data(hit_data, stamina_part_damage);
        return;
      }
      case 5: // magicka stamina
      {
        auto magicka_part_damage =
          handle_block_hit(*target, total_damage * 0.5f, magicka, damage_mult, RE::ActorValue::kMagicka);
        handle_hit_data(hit_data, magicka_part_damage);

        auto stamina_part_damage =
          handle_block_hit(*target, total_damage * 0.5f, stamina, damage_mult, RE::ActorValue::kStamina);
        handle_hit_data(hit_data, stamina_part_damage);
        return;
      }
      case 6: // health magicka
      {
        auto health_part_damage =
          handle_block_hit(*target, total_damage * 0.5f, health, damage_mult, RE::ActorValue::kHealth);
        handle_hit_data(hit_data, health_part_damage);

        auto magicka_part_damage =
          handle_block_hit(*target, total_damage * 0.5f, magicka, damage_mult, RE::ActorValue::kMagicka);
        handle_hit_data(hit_data, magicka_part_damage);
        return;
      }
      case 7: // health magicka stamina
      {
        auto health_part_damage =
          handle_block_hit(*target, total_damage * 0.35f, health, damage_mult, RE::ActorValue::kHealth);
        handle_hit_data(hit_data, health_part_damage);

        auto magicka_part_damage =
          handle_block_hit(*target, total_damage * 0.35f, magicka, damage_mult, RE::ActorValue::kMagicka);
        handle_hit_data(hit_data, magicka_part_damage);

        auto stamina_part_damage =
          handle_block_hit(*target, total_damage * 0.35f, stamina, damage_mult, RE::ActorValue::kStamina);
        handle_hit_data(hit_data, stamina_part_damage);
        return;
      }
      }

      return;
    }

  }
}