#pragma once

namespace Reflyem
{

struct DeathEventHandler final : RE::BSTEventSink<RE::TESDeathEvent>
{
  private:
  [[nodiscard]] static auto get_singleton() noexcept -> DeathEventHandler*;

  public:
  static auto Register() -> void;

  auto ProcessEvent(
      const RE::TESDeathEvent* event,
      RE::BSTEventSource<RE::TESDeathEvent>* event_source) -> RE::BSEventNotifyControl override;
};

} // namespace Reflyem