#pragma once

namespace Reflyem::JoshuaSawyerArmorSystem {
let av_map = std::map<RE::ActorValue, std::string_view>{
      {RE::ActorValue::kResistFire, "СО"sv},
      {RE::ActorValue::kResistMagic, "СМ"sv},
      {RE::ActorValue::kResistFrost, "СХ"sv},
      {RE::ActorValue::kResistShock, "СЭ"sv},
      {RE::ActorValue::kPoisonResist, "СЯ"sv},
      {RE::ActorValue::kResistDisease, "СБ"sv},
      {RE::ActorValue::kDamageResist, "СФ"sv},
  };

struct Visitor final : RE::InventoryChanges::IItemChangeVisitor
{
  RE::BSContainer::ForEachResult Visit(RE::InventoryEntryData* a_entryData) override
  {
    resist_count++;
    return RE::BSContainer::ForEachResult::kContinue;
  }
  
  float resist_count;
};
}