#pragma once

namespace GIGAWIDGET_API {
constexpr auto GigaWidgetPluginName = "GigaWidget";

enum class InterfaceVersion : uint8_t
{
  V1
};

enum class APIResult : uint8_t
{
  OK,
  AlreadyRegistered,
  NotRegistered
};

enum class PotionType : uint8_t
{
  Health,
  Stamina,
  Magicka,
  Other,
  Poison
};

struct AvailablePotionSlotResult
{
  std::uint16_t duration;
  std::uint16_t slots;
};

using SetAvailablePotionSlotCallback = std::function<AvailablePotionSlotResult(PotionType)>;
using SetFrameForPotionCallback = std::function<uint8_t(PotionType)>;

class IVGigaWidget1
{
public:
  virtual APIResult add_set_available_potion_slot_callback(SKSE::PluginHandle plugin_handle,
                                                           SetAvailablePotionSlotCallback&& callback) noexcept = 0;
  virtual APIResult add_set_frame_for_potion_callback(
      SKSE::PluginHandle plugin_handle,
      SetFrameForPotionCallback&& callback) noexcept = 0;
};

typedef void* (*_RequestPluginAPI)(const InterfaceVersion interface_version);

[[nodiscard]] inline void* RequestPluginAPI(const InterfaceVersion interface_version = InterfaceVersion::V1)
{
  auto pluginHandle = GetModuleHandleA("GigaWidget.dll");
  _RequestPluginAPI requestAPIFunction = (_RequestPluginAPI)GetProcAddress(pluginHandle, "RequestPluginAPI");
  if (requestAPIFunction) {
    return requestAPIFunction(interface_version);
  }
  return nullptr;
}

}
