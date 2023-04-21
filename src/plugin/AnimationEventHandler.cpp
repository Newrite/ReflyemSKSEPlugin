#include "plugin/AnimationEventHandler.hpp"
#include "plugin/ResourceManager.hpp"

namespace Reflyem::AnimationEventHandler
{
auto try_find_animation(const std::string& key) -> AnimationEvent
{
  const auto it = animation_map.find(key);
  if (it == animation_map.end()) { return AnimationEvent::kNone; }
  return it->second;
}
} // namespace Reflyem::AnimationEventHandler