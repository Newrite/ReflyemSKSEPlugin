#pragma once

namespace Reflyem
{
struct SoulTrapEventHandler final : RE::BSTEventSink<RE::SoulsTrapped::Event>
{
private:
  [[nodiscard]] static auto get_singleton() noexcept -> SoulTrapEventHandler*;

public:
  static auto Register() -> void;

  auto ProcessEvent(
      const RE::SoulsTrapped::Event* event,
      RE::BSTEventSource<RE::SoulsTrapped::Event>* event_source) -> RE::BSEventNotifyControl override;
};
} // namespace Reflyem
