#include "Config.hpp"
#include "Core.hpp"
namespace Reflyem::DeathLoot
{
static constexpr const char* HIDE_TAG = "hide_tag";
static constexpr float HEALTH_TAG = 777.7f;

auto is_loot(const RE::InventoryEntryData* data) -> bool
{
  if (!data->object->GetPlayable()) { return false; }

  if (data->IsEnchanted() || data->IsQuestObject())
    {
      logger::info("Enchanted {} or Quested {}"sv, data->IsEnchanted(), data->IsQuestObject());
      return true;
    }

  const auto form_type = data->object->formType.get();

  if (form_type == RE::TESObjectWEAP::FORMTYPE || form_type == RE::TESObjectARMO::FORMTYPE || form_type == RE::TESAmmo::FORMTYPE)
    {
      logger::info("Weapon or Armo: {}"sv, form_type);
      return false;
    }

  return true;
}

auto is_tagged_extra_text_display_data(const RE::ExtraTextDisplayData* data) -> bool
{
  if (data && data->displayName == HIDE_TAG)
    {
      logger::info("DisplayData {} == {}"sv, data->displayName.c_str(), HIDE_TAG);
      return true;
    }
  logger::info("Display data null or not equal"sv);
  return false;
}

auto is_tagged_extra_text_health_data(const RE::ExtraHealth* data) -> bool
{
  if (data && (std::abs(data->health - HEALTH_TAG) <= 0.2f))
    {
      logger::info("DisplayData {} == {}"sv, data->health, HEALTH_TAG);
      return true;
    }
  logger::info("Display data null or not equal"sv);
  return false;
}

auto is_tagged(const RE::InventoryEntryData* item) -> bool
{
  if (!item || !item->extraLists) { return false; }
  logger::info("Check is tag for {}"sv, item->object->GetName());
  const auto end = item->extraLists->end();
  for (auto it = item->extraLists->begin(); it != end; ++it)
    {
      const auto& list = *it;
      // if (is_tagged_extra_text_display_data(list->GetExtraTextDisplayData()))
      //   {
      //     logger::info("Item {} is tagged"sv, item->object->GetName());
      //     return true;
      //   }
      if (is_tagged_extra_text_health_data(list->GetByType<RE::ExtraHealth>()))
        {
          logger::info("Item {} is tagged"sv, item->object->GetName());
          return true;
        }
    }

  return false;
}

auto add_tag(RE::InventoryEntryData* item) -> void
{
  // if (is_tagged(item)) { return; }

  if (!item) { logger::info("Null item in add tag"sv); }
  if (!item->extraLists)
    {
      logger::info("Null extra lists for item {}"sv, item->object->GetName());
      item->AddExtraList(new RE::ExtraDataList());
    }
  logger::info("Check add tag for {}"sv, item->object->GetName());

  const auto end = item->extraLists->end();
  for (auto it = item->extraLists->begin(); it != end; ++it)
    {
      const auto& list = *it;
      const auto extra_data = list->GetByType<RE::ExtraHealth>();

      if (extra_data)
        {
          logger::info("Item {} displayName change to tag"sv, item->object->GetName());
          extra_data->health = HEALTH_TAG;
          return;
        }

      logger::info("Item {} tag added"sv, item->object->GetName());
      list->Add(new RE::ExtraHealth(HEALTH_TAG));
      return;
    }
}

auto process_death(const RE::TESDeathEvent* event, RE::BSTEventSource<RE::TESDeathEvent>*) -> void
{
  letr config = Config::get_singleton();

  let dead_actor = Core::actor_from_ni_pointer(&event->actorDying);

  let killer_actor = Core::actor_from_ni_pointer(&event->actorKiller);

  if (!dead_actor)
    {
      logger::info("Dead actor is null"sv);
      return;
    }

  let can_process = [&]() -> bool
  {
    if (killer_actor && (killer_actor->IsPlayerRef() || Core::is_player_ally(killer_actor)))
      {
        return true;
      }
    return false;
  }();

  let gold = RE::TESForm::LookupByID<RE::TESObjectMISC>(0xf);

  auto gold_count = 0;

  logger::info("CanProcess: {}"sv, can_process);

  for (auto& [object, pair] : dead_actor->GetInventory())
    {
      const auto count = pair.first;
      const auto& entry_data = pair.second;

      if (!object || !entry_data || !entry_data->object)
        {
          logger::info("Object null"sv);
          continue;
        }

      const auto is_loot_item = is_loot(entry_data.get());

      logger::info("Check loot: {} result: {}"sv, entry_data->GetDisplayName(), is_loot_item);
      if (is_loot_item)
        {
          logger::info("This is loot: {}"sv, entry_data->GetDisplayName());
          if (config.death_loot().auto_loot() && can_process)
            {
              dead_actor->RemoveItem(
                  entry_data->object,
                  count,
                  RE::ITEM_REMOVE_REASON::kStoreInContainer,
                  nullptr,
                  killer_actor);
            }
          continue;
        }
      if (!object->IsGold() && entry_data->object->GetPlayable())
        {
          gold_count += static_cast<int>(
              static_cast<float>(object->GetGoldValue()) * config.death_loot().gold_value_mult() *
              static_cast<float>(count));
        }
    }

  logger::info("gold count: {}"sv, gold_count);
  if (gold_count > 0 && can_process)
    {
      killer_actor->AddObjectToContainer(gold, nullptr, gold_count, nullptr);
    }
}

} // namespace Reflyem::DeathLoot