#include "plugin/AnimationEventHandler.hpp"
#include "plugin/ResourceManager.hpp"

namespace Reflyem::AnimationEventHandler
{

static constexpr std::string_view ANNOTATION_EVENT = "FireFullPowerBowArrow"sv;

static bool fake_bow_call = false;

auto is_annotation_bow_call() -> bool
{
  return fake_bow_call;
}

auto annotation_bow_call(const bool fake_call) -> void
{
  fake_bow_call=  fake_call;
}

auto try_find_animation(const std::string& key) -> AnimationEvent
{
  const auto it = animation_map.find(key);
  if (it == animation_map.end()) { return AnimationEvent::kNone; }
  return it->second;
}

auto animation_handler(const RE::BSAnimationGraphEvent& event, const Config& config) -> void
{

  let actor = const_cast<RE::Actor *>(event.holder->As<RE::Actor>());
  if (!actor) {
    return;
  }
  
  if (event.payload.contains(ANNOTATION_EVENT)) {
    fake_bow_call = true;
    Core::bow_fire(actor);
    fake_bow_call = false;
  }
}

} // namespace Reflyem::AnimationEventHandler
