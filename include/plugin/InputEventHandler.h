#pragma once

namespace Reflyem
{

	class InputEventHandler : public RE::BSTEventSink<RE::InputEvent*>
	{
	public:
		auto ProcessEvent(
			RE::InputEvent* const* a_event,
			RE::BSTEventSource<RE::InputEvent*>* a_eventSource) -> RE::BSEventNotifyControl override;

		static auto Register() -> void;

	private:
		static auto get_singleton() -> InputEventHandler*;

	};
}