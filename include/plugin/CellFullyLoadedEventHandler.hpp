#pragma once

namespace Reflyem
{

struct CellFullyLoadedEventHandler final : RE::BSTEventSink<RE::TESCellFullyLoadedEvent>
{
private:
  [[nodiscard]] static auto get_singleton() noexcept -> CellFullyLoadedEventHandler*;

public:
  static auto Register() -> void;

  auto ProcessEvent(
      const RE::TESCellFullyLoadedEvent* event,
      RE::BSTEventSource<RE::TESCellFullyLoadedEvent>* event_source) -> RE::BSEventNotifyControl override;
};

} // namespace Reflyem
