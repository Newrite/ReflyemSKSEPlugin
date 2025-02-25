#include "DeathLoot.hpp"
#include "Config.hpp"

namespace Reflyem::DeathLoot {
static constexpr const char* HIDE_TAG = "hide_tag";
static constexpr uint8_t MAX_FRAMES_DELAY = 30;

auto is_tagged_extra_text_display_data(const RE::ExtraTextDisplayData* data) -> bool
{
  if (data && data->displayName == HIDE_TAG) {
    logger::info("DisplayData {} == {}"sv, data->displayName.c_str(), HIDE_TAG);
    return true;
  }
  logger::info("Display data null or not equal"sv);
  return false;
}

auto is_tagged(const RE::InventoryEntryData* item) -> bool
{
  if (!item || !item->extraLists) {
    return false;
  }
  logger::info("Check is tag for {}"sv, item->object->GetName());
  const auto end = item->extraLists->end();
  for (auto it = item->extraLists->begin(); it != end; ++it) {
    const auto& list = *it;
    if (is_tagged_extra_text_display_data(list->GetExtraTextDisplayData())) {
      logger::info("Item {} is tagged"sv, item->object->GetName());
      return true;
    }
  }

  return false;
}

auto get_extra_data_list(const RE::InventoryEntryData* item) -> RE::ExtraDataList*
{
  if (!item || !item->extraLists) {
    return nullptr;
  }

  const auto end = item->extraLists->end();
  for (auto it = item->extraLists->begin(); it != end; ++it) {
    auto list = *it;
    if (list) {
      return list;
    }
  }

  return nullptr;
}

bool is_script_attached(const RE::TESForm* a_form, const RE::BSFixedString&)
{
  const auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
  if (!vm) {
    return false;
  }

  const auto policy = vm->GetObjectHandlePolicy();
  if (!policy) {
    return false;
  }

  const auto check_script = [&](const RE::TESForm* b_form) {
    const auto handle = policy->GetHandleForObject(static_cast<RE::VMTypeID>(b_form->GetFormType()), b_form);
    if (handle == policy->EmptyHandle()) {
      return false;
    }
    return true;
  };

  auto result = check_script(a_form);
  if (!result) {
    const auto ref = a_form->As<RE::TESObjectREFR>();
    const auto base = ref ? ref->GetObjectReference() : nullptr;
    if (base) {
      result = check_script(base);
    }
  }

  return result;
}

auto tag_item(void* menu, RE::InventoryEntryData* item, RE::ExtraDataList* extralist, const char* name) -> void
{
  const REL::Relocation<decltype(&tag_item)> func{RELOCATION_ID(50530, 0)};
  return func(menu, item, extralist, name);
}

void tag_item(RE::InventoryEntryData* item)
{
  tag_item(nullptr, item, nullptr, HIDE_TAG);
}

void tag_item(RE::InventoryChanges* changes, RE::TESBoundObject* base)
{
  auto new_entry = new RE::InventoryEntryData(base, 0);
  changes->AddEntryData(new_entry);
  tag_item(new_entry);
}

bool should_tag(const RE::TESBoundObject* base)
{

  if (!base) {
    return false;
  }

  if (!base->As<RE::TESObjectWEAP>() && !base->As<RE::TESAmmo>() && !base->As<RE::TESObjectARMO>()) {
    return false;
  }

  let weapon = base->As<RE::TESObjectWEAP>();
  let armor = base->As<RE::TESObjectARMO>();

  letr config = Config::get_singleton();

  if (base->As<RE::TESAmmo>() && config.death_loot().loot_arrow()) {
    return false;
  }

  if (weapon && (weapon->formEnchanting || Core::try_keyword_form_has_keyword(weapon, config.death_loot().exclusive_keyword()) || Core::try_keyword_form_has_keyword(weapon, Core::get_magic_disallow_keyword()))) {
    return false;
  }

  if (armor && (armor->formEnchanting || Core::try_keyword_form_has_keyword(armor, config.death_loot().exclusive_keyword()) || Core::try_keyword_form_has_keyword(armor, Core::get_magic_disallow_keyword()))) {
    return false;
  }

  if (Core::try_form_has_keyword(base, config.death_loot().exclusive_keyword()) || Core::try_form_has_keyword(base, Core::get_magic_disallow_keyword())) {
    return false;
  }

  return true;
}

bool should_tag(const RE::InventoryEntryData* item)
{

  if (!item) {
    return false;
  }

  if (item->IsEnchanted() || item->IsQuestObject() || !item->object->GetPlayable()) {
    return false;
  }

  return should_tag(item->object);
}

auto handle_actor(DeathLootData& data) -> void
{

  logi("HandleActor: Start");

  if (!data.victim) {
    return;
  }

  letr config = Config::get_singleton();

  let actor_base = data.victim->GetActorBase();
  auto no_filter = false;
  if (actor_base) {
    no_filter = config.death_loot().loot_uniques() && actor_base->IsUnique();
  }
  if (!no_filter && config.death_loot().loot_bosses()) {
    for (auto& id : location_bosses) {
      if (data.victim->formID == id) {
        no_filter = true;
        break;
      }
    }
  }

  auto _tag_item = [&](RE::InventoryEntryData* entry) {
    if (!no_filter && should_tag(entry)) {
      tag_item(entry);
    }
  };

  std::set<RE::TESBoundObject*> inv;

  auto changes = data.victim->GetInventoryChanges();
  if (!no_filter && changes && changes->entryList) {
    for (auto entry : *changes->entryList) {
      if (entry && entry->object) {
        _tag_item(entry);
        inv.insert(entry->object);
      }
    }
  }

  auto container = data.victim->GetContainer();
  if (!no_filter && container) {
    const auto ignore = [&](RE::TESBoundObject* obj) {
      const auto it = inv.find(obj);
      bool ans = it != inv.end();
      ans = ans || obj->formType == RE::FormType::LeveledItem;
      return ans;
    };

    container->ForEachContainerObject([&](RE::ContainerObject& a_entry) {
      auto obj = a_entry.obj;
      if (obj && !ignore(obj)) {
        auto it = inv.find(obj);
        if (it == inv.end()) {
          if (should_tag(obj)) {
            inv.insert(obj);
            tag_item(changes, obj);
          }
        }
      }
      return RE::BSContainer::ForEachResult::kContinue;
    });
  }

  logi("HandleActor: Container handled, {}", data.victim->GetDisplayFullName());

  let auto_loot = data.is_player_loot() && config.death_loot().auto_loot();

  if (!auto_loot) {
    logi("HandleActor: No Auto Loot");
    return;
  }

  for (auto& [object, pair] : data.victim->GetInventory()) {
    const auto count = pair.first;
    const auto& entry_data = pair.second;

    if (object && object->IsGold()) {
      data.victim->RemoveItem(object,
                              count,
                              RE::ITEM_REMOVE_REASON::kStoreInContainer,
                              nullptr,
                              config.death_loot().storage() ? config.death_loot().storage()
                                                            : RE::PlayerCharacter::GetSingleton());
    }

    if (!object || !entry_data || !entry_data->object) {
      logd("HandleActor: Object null"sv);
      continue;
    }

    const auto is_loot_item = !is_tagged(entry_data.get());

    logi("HandleActor: Check loot: {} result: {}"sv, entry_data->GetDisplayName(), is_loot_item);
    if (is_loot_item) {
      logi("HandleActor: This is loot: {}"sv, entry_data->GetDisplayName());
      if (object->GetPlayable()) {
        data.victim->RemoveItem(entry_data->object,
                                count,
                                RE::ITEM_REMOVE_REASON::kStoreInContainer,
                                get_extra_data_list(entry_data.get()),
                                config.death_loot().storage() ? config.death_loot().storage()
                                                              : RE::PlayerCharacter::GetSingleton());
      }
    }
  }
}

auto handle_killed_actors() -> void
{

  bool can_clean = true;

  for (auto& killed_actor_data : killed_actors) {
    if (killed_actor_data.victim && killed_actor_data.frame_count >= MAX_FRAMES_DELAY && !killed_actor_data.is_looted) {
      handle_actor(killed_actor_data);
      killed_actor_data.is_looted = true;
    } else if (!killed_actor_data.is_looted) {
      can_clean = false;
      killed_actor_data.frame_count += 1;
    }
  }

  if (can_clean && !killed_actors.empty()) {
    logi("HandleKilledActors: clean");
    killed_actors.clear();
  }
}

auto process_death(const RE::TESDeathEvent* event) -> void
{

  if (!event->actorDying || event->dead) {
    return;
  }

  let victim = event->actorDying->As<RE::Actor>();

  if (!victim) {
    return;
  }

  if (victim->IsSummoned()) {
    return;
  }

  if (victim->IsPlayerRef() || victim->IsPlayerTeammate()) {
    return;
  }

  logi("ProcessDeath: V {}", victim->GetDisplayFullName());
  killed_actors.push_back({victim, event->actorKiller ? event->actorKiller->As<RE::Actor>() : nullptr, 0, false});
}

auto process_kill(const RE::ActorKill::Event* event) -> void
{

  if (!event->victim) {
    return;
  }

  if (event->victim->IsSummoned()) {
    return;
  }

  if (event->victim->IsPlayerRef() || event->victim->IsPlayerTeammate()) {
    return;
  }

  logi("ProcessKill: V {}", event->victim->GetDisplayFullName());
  killed_actors.push_back({event->victim, event->killer, 0, false});
}

} // namespace Reflyem::DeathLoot
