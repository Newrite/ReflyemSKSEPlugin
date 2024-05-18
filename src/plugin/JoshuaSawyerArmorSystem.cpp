#include "Config.hpp"
#include "Core.hpp"

namespace Reflyem::JoshuaSawyerArmorSystem
{

/*
 * Функционал
 */

// MRL_END_SuperArmor|70:20.0:30:10.0|70:10.0:30:5.0

using AVArmorValuesPair = std::pair<RE::ActorValue, float>;
using AVArmorValuesPairsVector = std::vector<AVArmorValuesPair>;

struct ThresholdAndReductionData {
  AVArmorValuesPairsVector threshold_;
  AVArmorValuesPairsVector reduction_;
};

auto parse_values(const std::string& values_in_string) -> AVArmorValuesPairsVector {
  
  auto av_armor_values_pair_vector = AVArmorValuesPairsVector{};
  
  let split_values_in_string = Core::string_split(values_in_string,':');
  if (split_values_in_string.empty()) {
    return av_armor_values_pair_vector;
  }
  
  let values_vector_size = split_values_in_string.size();
  if (values_vector_size / 2 != 0) {
    logger::info(" Not paired"sv);
    return av_armor_values_pair_vector;
  }
  
  std::size_t index_av = 0;
  std::size_t index_value = 1;
  while (index_value < values_vector_size) {
    
    let int_from = Core::string_to_int(split_values_in_string.at(index_av));
    let float_from = Core::string_to_float(split_values_in_string.at(index_value));
    
    if (!int_from.has_value() || !float_from.has_value()) {
      index_av = index_av + 1;
      index_value = index_value + 1;
      continue;
    }
    
    let av = RE::ActorValue{int_from.value()};
    if (av == RE::ActorValue::kNone) {
      index_av = index_av + 1;
      index_value = index_value + 1;
      continue;
    }
    
    index_av = index_av + 1;
    index_value = index_value + 1;
    av_armor_values_pair_vector.emplace_back(av, float_from.value());
    
  }

  return av_armor_values_pair_vector;
}


auto get_threshold_and_reduction_from_armor(const RE::TESObjectARMO* armor) -> std::optional<ThresholdAndReductionData> {
  
  if (!armor) {
    return std::nullopt;
  }
  
  const std::string editor_id = armor->GetFormEditorID();

  let splited_strings = Core::string_split(editor_id, '|');
  
}

struct VisitArmorParseThresholdReduction : RE::InventoryChanges::IItemChangeVisitor {
  ~VisitArmorParseThresholdReduction() override;
  RE::BSContainer::ForEachResult Visit(RE::InventoryEntryData* a_entryData) override {
    if (a_entryData && a_entryData->object) {
      
    }
    return RE::BSContainer::ForEachResult::kContinue;
  }
  bool ShouldVisit(RE::InventoryEntryData* a_entryData, RE::TESBoundObject* a_object) override {
    if (a_object && a_entryData) {
      return a_object->IsArmor();
    }
    return false;
  }
};

// auto update(RE::Actor* _) -> void {
//   // let inv = actor->GetInventoryChanges();
// }

auto on_weapon_hit(RE::Actor* target, RE::HitData&, const Config&) -> void {
  if (!target) {
    return;
  }
}

}
