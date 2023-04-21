namespace Reflyem::DeathLoot
{

[[nodiscard]] auto is_playable(const RE::TESForm* form) noexcept -> bool
{
  return (form->GetFormFlags() & RE::TESForm::RecordFlags::kPlayable) != 0;
}

auto is_loot(const std::unique_ptr<RE::InventoryEntryData>& data) -> bool
{
  if (!is_playable(data->object)) { return false; }

  if (data->IsEnchanted() || data->IsQuestObject()) { return true; }

  const auto form_type = data->object->formType.get();

  if (form_type == RE::TESObjectWEAP::FORMTYPE || form_type == RE::TESObjectARMO::FORMTYPE)
    {
      return false;
    }

  return true;
}

auto process_death(const RE::TESDeathEvent* event, RE::BSTEventSource<RE::TESDeathEvent>*) -> void
{
  if (event->dead)
    {
      logger::info("Dying is dead"sv);
      return;
    }

  let actor_from_pointer = [](const RE::NiPointer<RE::TESObjectREFR>* ni_actor) -> RE::Actor*
  {
    if (!ni_actor) { return nullptr; }
    let refr = ni_actor->get();
    if (!refr) { return nullptr; }

    return refr->As<RE::Actor>();
  };

  let dead_actor = actor_from_pointer(&event->actorDying);

  let killer_actor = actor_from_pointer(&event->actorKiller);

  // let is_player_ally = [](RE::Actor* actor) -> bool
  // {
  //   if (!actor) { return false; }
  //   let player = RE::PlayerCharacter::GetSingleton();
  //   if (!player) { return false; }
  //   return actor->IsPlayerTeammate() || !actor->IsHostileToActor(player);
  // };

  // let can_process = [&]() -> bool
  // {
  //   if (killer_actor && killer_actor->IsPlayerRef()) { return true; }
  //   if (dead_actor && is_player_ally(dead_actor)) { return false; }
  //   return false;
  // }();

  if (!dead_actor)
    {
      logger::info("Dead actor is null"sv);
      return;
    }

  if (!killer_actor)
    {
      logger::info("Killer actor is null"sv);
      return;
    }

  let gold = RE::TESForm::LookupByID<RE::TESObjectMISC>(0xf);

  auto gold_count = 0;

  for (auto& [object, pair] : dead_actor->GetInventory())
    {
      if (!object || !pair.second || !pair.second->object)
        {
          logger::info("Object null"sv);
          continue;
        }

      if (object->formID != gold->formID) { gold_count += object->GetGoldValue() * pair.first; }

      logger::info("Check loot: {}"sv, pair.second->GetDisplayName());
      if (is_loot(pair.second))
        {
          logger::info("This is loot: {}"sv, pair.second->GetDisplayName());
          dead_actor->RemoveItem(
              pair.second->object,
              pair.first,
              RE::ITEM_REMOVE_REASON::kStoreInContainer,
              nullptr,
              killer_actor);
        }
    }

  logger::info("gold count: {}"sv, gold_count);
  if (gold_count > 0) { killer_actor->AddObjectToContainer(gold, nullptr, gold_count, nullptr); }
}

} // namespace Reflyem::DeathLoot