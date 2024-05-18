#include "plugin/ItemLimit.hpp"
#include "Core.hpp"

namespace Reflyem::ItemLimit
{

using LimitedPair = std::pair<int32_t, RE::TESBoundObject*>;

struct StorageLimitedItem final
{
  int32_t limit_count{0};
  std::vector<LimitedPair> items_vector{};
  StorageLimitedItem() = default;
  StorageLimitedItem(const StorageLimitedItem& other) = default;
  StorageLimitedItem(StorageLimitedItem&& other) noexcept = default;
  StorageLimitedItem& operator=(const StorageLimitedItem& other) = default;
  StorageLimitedItem& operator=(StorageLimitedItem&& other) noexcept = default;
};

struct LimitedItem final
{
  int32_t limit_count{0};
  int32_t count_of_items{0};
  std::vector<LimitedPair> items_vector{};
  LimitedItem() = default;
  LimitedItem(const LimitedItem& other) = default;
  LimitedItem(LimitedItem&& other) noexcept = default;
  LimitedItem& operator=(const LimitedItem& other) = default;
  LimitedItem& operator=(LimitedItem&& other) noexcept = default;
};

using LimitedItemsMap = std::map<RE::BGSKeyword*, LimitedItem>;
using StorageLimitedItemsMap = std::map<RE::BGSKeyword*, StorageLimitedItem>;

auto get_inventory_map_container(RE::TESObjectREFR& refr) -> RE::TESObjectREFR::InventoryItemMap
{
  return refr.GetInventory(
      [](const RE::TESBoundObject& object) -> bool
      { return object.formType.get() == RE::AlchemyItem::FORMTYPE; });
  // return refr.GetInventory();
}

auto is_limited_item(const RE::TESBoundObject* item, const Config& config)
    -> std::pair<std::optional<uint32_t>, RE::BGSKeyword*>
{
  const auto keywords_item = config.item_limit().keywords_item();

  if (!keywords_item || keywords_item->forms.empty())
    {
      logger::debug(
          "list null of empty, size: {}"sv,
          keywords_item ? 0 : keywords_item->forms.size());
      return {std::nullopt, nullptr};
    }

  for (uint32_t index : views::iota(0u, keywords_item->forms.size()))
    {
      const auto keyword_ptr = keywords_item->forms[index];

      if (!keyword_ptr)
        {
          logger::debug("Keyword ptr null {}"sv, item->GetName());
          continue;
        }

      const auto keyword = keyword_ptr->As<RE::BGSKeyword>();

      if (Core::try_form_has_keyword(item, keyword))
        {
          logger::debug("Found limit keyword: {} index: {}"sv, item->GetName(), index);
          return {index, keyword};
        }
    }

  logger::debug("Not found limit keyword: {}"sv, item->GetName());

  return {std::nullopt, nullptr};
}

auto get_limited_item_count(
    RE::Character& character,
    const uint32_t keyword_index,
    const Config& config) -> int32_t
{
  auto count = 0;

  const auto keyword_mod = [&]() -> RE::BGSKeyword*
  {
    const auto keyword_list = config.item_limit().keywords_cap_mod();

    if (!keyword_list) { return nullptr; }

    if (keyword_list->forms.size() <= keyword_index) { return nullptr; }

    const auto keyword = keyword_list->forms[keyword_index]->As<RE::BGSKeyword>();

    return keyword ? keyword : nullptr;
  }();

  if (keyword_mod)
    {
      count += static_cast<int32_t>(
          Core::get_effects_magnitude_sum(Core::get_effects_by_keyword(character, *keyword_mod))
              .value_or(0.f));
    }

  if (count < 0) { count = 0; }

  const auto globals = config.item_limit().globals();

  if (globals && globals->forms.size() > keyword_index)
    {
      const auto global_ptr = globals->forms[keyword_index];

      if (!global_ptr) { return count; }

      const auto global = global_ptr->As<RE::TESGlobal>();

      if (!global) { return count; }

      count += static_cast<int32_t>(global->value);
    }

  if (count < 0) { count = 0; }

  return count;
}

auto objects_sort_by_gold_value(const LimitedPair& pair_one, const LimitedPair& pair_two) -> bool
{
  return pair_one.second->GetGoldValue() < pair_two.second->GetGoldValue();
}

auto get_storage_limited_items(
    RE::Character& character,
    const RE::TESObjectREFR::InventoryItemMap& inventory,
    const Config& config) -> StorageLimitedItemsMap
{
  auto storage_limited_items_map = StorageLimitedItemsMap{};

  // ReSharper disable once CppUseElementsView
  for (const auto& [object, pair] : inventory)
    {
      if (!pair.second || !pair.second->object) { continue; }

      logger::debug("Check item for limited"sv);

      const auto& item = pair.second;
      auto count = pair.first;

      if (Core::try_form_has_keyword(item->object, config.item_limit().exclusive_restore_keyword()))
        {
          continue;
        }

      // ReSharper disable once CppTooWideScopeInitStatement
      const auto [index, keyword] = is_limited_item(item->object, config);

      logger::debug(
          "Result of limited item {}: {}",
          item->GetDisplayName(),
          index.has_value() ? index.value() : -1);

      if (keyword && index.has_value())
        {
          logger::debug("Item is limited: {}"sv, item->GetDisplayName());

          // ReSharper disable once CppTooWideScopeInitStatement
          const auto limit_count = get_limited_item_count(character, index.value(), config);

          if (storage_limited_items_map.contains(keyword))
            {
              logger::debug(
                  "Key contain, add new item: count {} name {}"sv,
                  count,
                  object->GetName());
              auto& limited_item = storage_limited_items_map[keyword];
              limited_item.items_vector.emplace_back(count, item->object);
            }
          else
            {
              logger::debug(
                  "Key not contain, init key, item: count {} name {} limit_count {}"sv,
                  count,
                  object->GetName(),
                  limit_count);
              auto limited_item = StorageLimitedItem{};
              limited_item.limit_count = limit_count;
              limited_item.items_vector.emplace_back(count, item->object);
              storage_limited_items_map[keyword] = limited_item;
            }
        }
    }

  return storage_limited_items_map;
}

auto get_character_limited_items(
    RE::Character& character,
    const RE::TESObjectREFR::InventoryItemMap& inventory,
    const Config& config) -> LimitedItemsMap
{
  auto limited_items_map = LimitedItemsMap{};

  // ReSharper disable once CppUseElementsView
  for (const auto& [object, pair] : inventory)
    {
      if (!pair.second || !pair.second->object) { continue; }

      logger::debug("Check item for limited"sv);

      const auto& item = pair.second;
      auto count = pair.first;
      // ReSharper disable once CppTooWideScopeInitStatement
      const auto [index, keyword] = is_limited_item(item->object, config);

      logger::debug(
          "Result of limited item {}: {}",
          item->GetDisplayName(),
          index.has_value() ? index.value() : -1);

      if (keyword && index.has_value())
        {
          logger::debug("Item is limited: {}"sv, item->GetDisplayName());

          // ReSharper disable once CppTooWideScopeInitStatement
          const auto limit_count = get_limited_item_count(character, index.value(), config);

          if (limited_items_map.contains(keyword))
            {
              logger::debug(
                  "Key contain, add new item: count {} name {}"sv,
                  count,
                  object->GetName());
              auto& limited_item = limited_items_map[keyword];
              limited_item.count_of_items += count;
              limited_item.items_vector.emplace_back(count, item->object);
            }
          else
            {
              logger::debug(
                  "Key not contain, init key, item: count {} name {} limit_count {}"sv,
                  count,
                  object->GetName(),
                  limit_count);
              auto limited_item = LimitedItem{};
              limited_item.limit_count = limit_count;
              limited_item.count_of_items = count;
              limited_item.items_vector.emplace_back(count, item->object);
              limited_items_map[keyword] = limited_item;
            }
        }
    }

  return limited_items_map;
}

auto move_overlimited_items_to_storage(
    RE::Character& character,
    LimitedItemsMap& limited_items_map,
    RE::TESObjectREFR* storage) -> void
{
  logger::debug("Start iterate limited items map"sv);
  for (auto& limited_item : limited_items_map | views::values)
    {
      if (limited_item.count_of_items < limited_item.limit_count)
        {
          logger::debug("Count of items < limited item"sv);
          continue;
        }

      logger::debug("Sort vector of items, size is: {}"sv, limited_item.items_vector.size());
      std::ranges::sort(limited_item.items_vector, objects_sort_by_gold_value);

      for (auto& pair : limited_item.items_vector)
        {
          auto& count = pair.first;
          const auto object = pair.second;

          auto count_to_remove = 0;
          while (limited_item.count_of_items > limited_item.limit_count && count > 0)
            {
              count_to_remove += 1;
              count -= 1;
              limited_item.count_of_items -= 1;
            }
          logger::debug(
              "After while cycle {}: remove_count {} count {} count_of_item {} limit_count {}"sv,
              object->GetName(),
              count_to_remove,
              count,
              limited_item.count_of_items,
              limited_item.limit_count);

          if (count_to_remove > 0)
            {
              logger::debug(
                  "Item: {} count: {} count_to_remove: {}, limit_count: {}"sv,
                  object->GetName(),
                  count,
                  count_to_remove,
                  limited_item.limit_count);

              character.RemoveItem(
                  object,
                  count_to_remove,
                  RE::ITEM_REMOVE_REASON::kStoreInContainer,
                  nullptr,
                  storage);
            }
        }
    }
}

auto move_no_overlimited_items_from_storage(
    LimitedItemsMap& limited_items_map,
    StorageLimitedItemsMap& storage_limited_items_map,
    RE::Character& character,
    RE::TESObjectREFR* storage) -> void
{
  for (auto& [keyword, storage_limited_item] : storage_limited_items_map)
    {
      auto count_of_items = 0;

      if (limited_items_map.contains(keyword))
        {
          count_of_items = limited_items_map[keyword].count_of_items;
        }

      if (count_of_items >= storage_limited_item.limit_count) { continue; }

      ranges::sort(storage_limited_item.items_vector, objects_sort_by_gold_value);
      ranges::reverse(storage_limited_item.items_vector);

      for (auto& [count, object] : storage_limited_item.items_vector)
        {
          auto count_to_add = 0;
          while (count_of_items < storage_limited_item.limit_count && count > 0)
            {
              count_to_add += 1;
              count -= 1;
              count_of_items += 1;
            }
          logger::debug(
              "After while cycle {}: add_count {} count {} count_of_item {} limit_count {}"sv,
              object->GetName(),
              count_to_add,
              count,
              count_of_items,
              storage_limited_item.limit_count);

          if (count_to_add > 0)
            {
              logger::debug(
                  "Item: {} count: {} count_to_add: {}, limit_count: {}"sv,
                  object->GetName(),
                  count,
                  count_to_add,
                  storage_limited_item.limit_count);

              storage->RemoveItem(
                  object,
                  count_to_add,
                  RE::ITEM_REMOVE_REASON::kStoreInContainer,
                  nullptr,
                  &character);
            }
        }
    }
}

// papyrus function
// ReSharper disable once CppInconsistentNaming
auto UpdateItems(RE::StaticFunctionTag*) -> void
{
  const auto& config = Config::get_singleton();

  if (!config.item_limit().storage()) { return; }

  const auto player_ptr = RE::PlayerCharacter::GetSingleton();

  if (!player_ptr) { return; }

  auto& character = *player_ptr;

  // ReSharper disable once CppTooWideScopeInitStatement
  const auto character_inventory = get_inventory_map_container(character);
  auto limited_items_map = get_character_limited_items(character, character_inventory, config);

  move_overlimited_items_to_storage(character, limited_items_map, config.item_limit().storage());

  const auto storage_inventory = get_inventory_map_container(*config.item_limit().storage());
  auto storage_limited_items_map = get_storage_limited_items(character, storage_inventory, config);

  move_no_overlimited_items_from_storage(
      limited_items_map,
      storage_limited_items_map,
      character,
      config.item_limit().storage());
}

auto update_actor(RE::Character& character, const float, const Config& config) -> void
{
  if (!config.item_limit().storage()) { return; }

  // ReSharper disable once CppTooWideScopeInitStatement
  const auto character_inventory = get_inventory_map_container(character);
  auto limited_items_map = get_character_limited_items(character, character_inventory, config);

  move_overlimited_items_to_storage(character, limited_items_map, config.item_limit().storage());

  if (config.item_limit().enable_auto_restore() && !character.IsInCombat())
    {
      if (!config.item_limit().auto_restore_block_keyword() ||
          Core::has_absolute_keyword(character, *config.item_limit().auto_restore_block_keyword()))
        {
          return;
        }

      const auto storage_inventory = get_inventory_map_container(*config.item_limit().storage());
      auto storage_limited_items_map =
          get_storage_limited_items(character, storage_inventory, config);

      move_no_overlimited_items_from_storage(
          limited_items_map,
          storage_limited_items_map,
          character,
          config.item_limit().storage());
    }
}
} // namespace Reflyem::ItemLimit
