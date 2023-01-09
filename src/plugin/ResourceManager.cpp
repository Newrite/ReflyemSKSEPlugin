#include "ResourceManager.h"
#include "Core.h"

namespace Reflyem
{
  namespace ResourceManager
  {

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

    auto weap_actor_mask_multiply(WeapMask& matrix1, ActorMask& matrix2) -> WeapMask
    {

      const auto row = 1;
      const auto column = 3;

      WeapMask result{ { {0,0,0} } };
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
      return result;
    }

    auto get_drain_value(
      RE::Actor& actor,
      RE::BGSKeywordForm& form,
      const Reflyem::Config config,
      float cost) -> std::shared_ptr<DrainValues>
    {

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

      WeapMask w_mask{ {{0,0,0}} };
      ActorMask a_mask
      { {{1, 0, 0},
         {0, 1, 0},
         {0, 0, 1}} };

      logger::debug("start get weapon keyword");
      w_mask.at(0).at(0) = 1;
      if (form.HasKeyword(config.resource_manager_health_kw))
      {
        w_mask.at(0).at(0) = 0;
        w_mask.at(0).at(1) = 1;
      }
      if (form.HasKeyword(config.resource_manager_magicka_kw))
      {
        w_mask.at(0).at(0) = 0;
        w_mask.at(0).at(2) = 1;
      }
      if (form.HasKeyword(config.resource_manager_stamina_kw))
      {
        w_mask.at(0).at(0) = 1;
      }
      logger::debug("end get weapon keyword");

      logger::debug("start mutate actor mask");
      if (stamina_to_health) { a_mask.at(0) = { {0, 1, 0} }; }
      if (stamina_to_magicka) { a_mask.at(0) = { {0, 0, 1} }; }
      if (health_to_magicka) { a_mask.at(1) = { {0, 0, 1} }; }
      if (health_to_stamina) { a_mask.at(1) = { {1, 0, 0} }; }
      if (magicka_to_health) { a_mask.at(2) = { {0, 1, 0} }; }
      if (magicka_to_stamina) { a_mask.at(2) = { {1, 0, 0} }; }
      logger::debug("end mutate actor mask");

      logger::debug("before start multiply");
      auto mask = weap_actor_mask_multiply(w_mask, a_mask);
      logger::debug("before end multiply");

      auto mask_sum = 0;

      if (mask.at(0).at(0) > 0) { mask_sum += 1; }
      if (mask.at(0).at(1) > 0) { mask_sum += 2; }
      if (mask.at(0).at(2) > 0) { mask_sum += 4; }

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

    auto get_attack_mult(
      RE::Actor& actor,
      RE::ActorValue av,
      std::int32_t high_value,
      std::int32_t low_value) -> float
    {
      auto av_value = static_cast<std::int32_t>(actor.GetActorValue(av));
      if (high_value > 100) { high_value = 100; }
      if (av_value > high_value) { av_value = high_value; }
      if (av_value < low_value) { av_value = low_value; }

      return 1.f - (av_value / 100.f);

    }

    auto get_drain_cost(
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
        power_attack_cost_mult =
          get_attack_mult(
            actor,
            config.resource_manager_power_attack_cost_av,
            config.resource_manager_power_attack_cost_high,
            config.resource_manager_power_attack_cost_low);
      }

      auto attack_cost_mult =
        get_attack_mult(
          actor,
          config.resource_manager_attack_cost_av,
          config.resource_manager_attack_cost_high,
          config.resource_manager_attack_cost_low);

      auto weapon_weight = get_weapon_weight(weapon, config);

      auto flat_value = 3.f;

      logger::debug("attack cost mult: {}, power attack cost mult: {}", attack_cost_mult, power_attack_cost_mult);

      auto cost =
        (weapon_weight * attack_cost_mult * infamy)
        + (weapon_weight * infamy * power_attack_cost_mult * config.resource_manager_power_attack_mult)
        + flat_value;

      return cost * config.resource_manager_global_mult;

    }

    auto melee_weapon_spend(RE::Actor& actor,
      RE::TESObjectWEAP& weapon,
      bool is_power_attack,
      const Reflyem::Config config) -> void
    {
      logger::debug("melee_weapon_spend");
      auto cost = get_drain_cost(actor, weapon, is_power_attack, config);
      logger::debug("end drain cost: cost {}", cost);
      auto drain_value = get_drain_value(actor, weapon, config, cost);
      logger::debug("end drain value");
      drain_value->drain(actor);
      logger::debug("end drain");
      return;
    }

     auto ranged_weapon_spend(RE::Actor& actor,
       RE::TESObjectWEAP& weapon,
       const Reflyem::Config config) -> void
     {
       auto cost = get_drain_cost(actor, weapon, false, config) * 0.1f;
       auto drain_value = get_drain_value(actor, weapon, config, cost);
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

    auto handler(
      Reflyem::AnimationEventHandler::AnimationEvent animation,
      RE::Actor& actor,
      bool is_power_attack,
      const Reflyem::Config config) -> void
    {

      switch (animation)
      {
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

  }
}