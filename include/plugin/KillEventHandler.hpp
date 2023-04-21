#pragma once

namespace Reflyem
{
struct KillEventHandler final : RE::BSTEventSink<RE::ActorKill::Event>
{
  private:
  [[nodiscard]] static auto get_singleton() noexcept -> KillEventHandler*;

  public:
  static auto Register() -> void;

  auto ProcessEvent(
      const RE::ActorKill::Event* event,
      RE::BSTEventSource<RE::ActorKill::Event>* event_source) -> RE::BSEventNotifyControl override;
};
} // namespace Reflyem