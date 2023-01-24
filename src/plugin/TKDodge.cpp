#include "TKDodge.hpp"
#include "Core.hpp"
#include "ResourceManager.hpp"

namespace Reflyem {
namespace TKDodge {

enum class MovementDirections {
  kNone = 0,
  kForward,
  kRight,
  kBackward,
  kLeft,
};

using MovementPair = std::pair<MovementDirections, std::string_view>;
using DrainValues  = Reflyem::ResourceManager::DrainValues;
using MGEFMask     = Reflyem::ResourceManager::FormMask;

enum : uint32_t { kInvalid = static_cast<uint32_t>(-1), kKeyboardOffset = 0, kMouseOffset = 256, kGamepadOffset = 266 };

const std::vector<MovementPair> Directions{
    MovementPair(MovementDirections::kForward, "Forward"), MovementPair(MovementDirections::kBackward, "Back"),
    MovementPair(MovementDirections::kLeft, "Strafe Left"), MovementPair(MovementDirections::kRight, "Strafe Right")};

auto get_actor_effects_mask(RE::Actor& actor, const Reflyem::Config& config) -> std::unique_ptr<MGEFMask> {
  MGEFMask f_mask{{{0, 0, 0}}};
  logger::debug("start get actor mgef keyword");
  f_mask.at(0).at(0) = 1;
  if (Reflyem::Core::actor_has_active_mgef_with_keyword(actor, *config.tk_dodge_health_kw)) {
    f_mask.at(0).at(0) = 0;
    f_mask.at(0).at(1) = 1;
  }
  if (Reflyem::Core::actor_has_active_mgef_with_keyword(actor, *config.tk_dodge_magicka_kw)) {
    f_mask.at(0).at(0) = 0;
    f_mask.at(0).at(2) = 1;
  }
  if (Reflyem::Core::actor_has_active_mgef_with_keyword(actor, *config.tk_dodge_stamina_kw)) {
    f_mask.at(0).at(0) = 1;
  }
  logger::debug("end get actor mgef keyword");
  return std::make_unique<MGEFMask>(f_mask);
}

auto calc_dodge_cost(RE::Actor& actor, const Reflyem::Config& config) -> float {
  auto cost = actor.equippedWeight * config.tk_dodge_equipped_weight_mult;
  cost += config.tk_dodge_flat_cost;

  if (cost > config.tk_dodge_max_cost) {
    cost = config.tk_dodge_max_cost;
  }

  if (cost < config.tk_dodge_min_cost) {
    cost = config.tk_dodge_min_cost;
  }

  return cost;
}

auto get_drain_value(RE::Actor& actor, const Reflyem::Config& config) -> std::shared_ptr<DrainValues> {
  auto mgef_mask    = get_actor_effects_mask(actor, config);
  auto mask_sum     = Reflyem ::ResourceManager ::calc_mask_sum(*mgef_mask);
  auto dodge_cost   = calc_dodge_cost(actor, config);
  auto drain_values = Reflyem ::ResourceManager ::handle_mask_sum_for_drain_values(mask_sum, dodge_cost);

  return drain_values;
}

auto is_allow_pc_control_for_dodge(RE::PlayerCharacter& player, bool check_attack, bool check_power_attack) -> bool {
  auto is_attacking       = false;
  auto is_power_attacking = false;
  if (check_attack) {
    is_attacking = player.IsAttacking();
  }
  if (check_power_attack) {
    is_power_attacking = Reflyem::Core::is_power_attacking(player);
  }
  return player.GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal &&
         player.GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal && player.GetFlyState() == RE::FLY_STATE::kNone &&
         !player.IsSneaking() && !is_attacking && !is_power_attacking && !player.IsSwimming() && !player.IsInKillMove();
}

auto is_enough_resource_for_dodge(RE::Actor& actor, DrainValues& drain_values) -> bool {
  auto magicka = actor.GetActorValue(RE::ActorValue::kMagicka);
  auto health  = actor.GetActorValue(RE::ActorValue::kHealth);
  auto stamina = actor.GetActorValue(RE::ActorValue::kStamina);

  auto d_magicka = std::abs(drain_values.magicka);
  auto d_health  = std::abs(drain_values.health);
  auto d_stamina = std::abs(drain_values.stamina);

  logger::debug("Check allow dodge, magicka: {} health: {} stamina: {} dv m h s: {} {} {}", magicka, health, stamina,
                d_magicka, d_health, d_stamina);

  if (d_magicka > 0.f && d_magicka > magicka) {
    return false;
  }
  if (d_health > 0.f && d_health > health) {
    return false;
  }
  if (d_stamina > 0.f && d_stamina > stamina) {
    return false;
  }

  logger::debug("Allow dodge");

  return true;
}

auto is_allow_dodge(RE::PlayerCharacter& player, DrainValues& drain_values, bool check_attack, bool check_power_attack)
    -> bool {
  return is_allow_pc_control_for_dodge(player, check_attack, check_power_attack) &&
         is_enough_resource_for_dodge(player, drain_values);
}

auto get_key_hold_duration(const std::uint32_t a_index, float& result) -> bool const {
  auto input_mgr = RE::BSInputDeviceManager::GetSingleton();

  if (input_mgr && input_mgr->GetKeyboard()->IsPressed(a_index) &&
      input_mgr->GetKeyboard()->deviceButtons.find(a_index)->second) {
    result = input_mgr->GetKeyboard()->deviceButtons.find(a_index)->second->heldDownSecs;
    return true;
  }

  return false;
}

auto get_keyboard_direction_value() -> MovementDirections {
  auto ctrl_map = RE::ControlMap::GetSingleton();

  std::optional<MovementDirections> packed_dir;
  float                             record_dur = 0.f;

  for (auto& dir : Directions) {
    auto key = ctrl_map->GetMappedKey(dir.second, RE::INPUT_DEVICE::kKeyboard);

    float hold_dur;
    if (get_key_hold_duration(key, hold_dur) && (!packed_dir.has_value() || hold_dur <= record_dur)) {
      packed_dir.emplace(dir.first);
      record_dur = hold_dur;
      logger::debug(FMT_STRING("Get a Held Direction Key Index {:x}, Duration {}"), key, record_dur);
    }
  }

  return packed_dir.has_value() ? packed_dir.value() : MovementDirections::kNone;
}

auto normal_absolute_angle(float angle) -> float {
  static const auto PI = 3.1415927f;

  while (angle < 0)
    angle += 2 * PI;
  while (angle > 2 * PI)
    angle -= 2 * PI;
  return angle;
}

auto get_game_pad_direction_value() -> MovementDirections {
  auto input_mgr = RE::BSInputDeviceManager::GetSingleton();

  if (input_mgr && input_mgr->GetGamepad()) {
    auto game_pad = (RE::BSWin32GamepadDevice*)(input_mgr->GetGamepad());

    if (game_pad) {
      logger::debug(FMT_STRING("Current LX is {}, Current LY is {}"), game_pad->curLX, game_pad->curLY);

      float              dir_xy[2]   = {game_pad->curLX, game_pad->curLY};
      static const float dir_base[2] = {0, 1.0f};

      float power = sqrt(std::powf(game_pad->curLX, 2) + std::powf(game_pad->curLY, 2));
      logger::debug(FMT_STRING("Current Power is {}"), power);

      float theta = (dir_xy[0] * dir_base[0] + dir_xy[1] * dir_base[1]) /
                    sqrt(dir_xy[0] * dir_xy[0] + dir_xy[1] * dir_xy[1]) /
                    sqrt(dir_base[0] * dir_base[0] + dir_base[1] * dir_base[1]);

      theta = game_pad->curLX >= 0.f ? std::acos(theta) : -std::acos(theta);
      logger::debug(FMT_STRING("theta is {}"), theta);

      auto dir = normal_absolute_angle(theta);
      dir /= 6.283185f;
      dir += 0.125f;
      dir *= 4.0f;
      dir = fmod(dir, 4.0f);
      dir = floor(dir);
      dir += 1.0f;
      logger::debug(FMT_STRING("GamePad Direction is {}"), dir);

      auto& config = Reflyem::Config::get_singleton();

      if (power > config.tk_dodge_gamepad_treshold) {
        switch (std::int32_t(dir)) {
        case 1:
          return MovementDirections::kForward;
        case 2:
          return MovementDirections::kRight;
        case 3:
          return MovementDirections::kBackward;
        case 4:
          return MovementDirections::kLeft;
        default:
          return MovementDirections::kNone;
        }
      }
    }
  }

  return MovementDirections::kNone;
}

auto get_dodge_event() -> std::string {
  auto direction = MovementDirections::kNone;

  auto input_mgr = RE::BSInputDeviceManager::GetSingleton();

  if (input_mgr && input_mgr->GetKeyboard() && input_mgr->GetKeyboard()->IsEnabled() &&
      !input_mgr->IsGamepadConnected()) {
    direction = get_keyboard_direction_value();
  } else if (input_mgr->IsGamepadConnected()) {
    direction = get_game_pad_direction_value();
  }

  auto  tdm_freeMov = RE::TESForm::LookupByEditorID<RE::TESGlobal>("TDM_DirectionalMovement");
  auto& config      = Reflyem::Config::get_singleton();

  if (tdm_freeMov && tdm_freeMov->value && direction != MovementDirections::kNone) {
    logger::debug("TDM Free Movement, Force to Forward Dodge!");
    return "TKDodgeForward";
  } else if (tdm_freeMov && !tdm_freeMov->value && RE::PlayerCharacter::GetSingleton()->IsSprinting() &&
             !config.tk_dodge_sprint_tapping_dodge) {
    logger::debug("Player is Sprinting in TDM target-lock, Disable Dodge!");
    return "";
  }

  switch (direction) {
  case MovementDirections::kForward:
    return "TKDodgeForward";
  case MovementDirections::kBackward:
    return "TKDodgeBack";
  case MovementDirections::kLeft:
    return "TKDodgeLeft";
  case MovementDirections::kRight:
    return "TKDodgeRight";
  default:
    return "";
  }
}

auto get_gamepad_index(RE::BSWin32GamepadDevice::Key a_key) -> std::uint32_t {
  using Key = RE::BSWin32GamepadDevice::Key;

  std::uint32_t index;
  switch (a_key) {
  case Key::kUp:
    index = 0;
    break;
  case Key::kDown:
    index = 1;
    break;
  case Key::kLeft:
    index = 2;
    break;
  case Key::kRight:
    index = 3;
    break;
  case Key::kStart:
    index = 4;
    break;
  case Key::kBack:
    index = 5;
    break;
  case Key::kLeftThumb:
    index = 6;
    break;
  case Key::kRightThumb:
    index = 7;
    break;
  case Key::kLeftShoulder:
    index = 8;
    break;
  case Key::kRightShoulder:
    index = 9;
    break;
  case Key::kA:
    index = 10;
    break;
  case Key::kB:
    index = 11;
    break;
  case Key::kX:
    index = 12;
    break;
  case Key::kY:
    index = 13;
    break;
  case Key::kLeftTrigger:
    index = 14;
    break;
  case Key::kRightTrigger:
    index = 15;
    break;
  default:
    index = kInvalid;
    break;
  }

  return index != kInvalid ? index + kGamepadOffset : kInvalid;
}

auto get_sprint_key(RE::INPUT_DEVICE a_device) -> std::uint32_t {
  using DeviceType = RE::INPUT_DEVICE;

  const auto control_map = RE::ControlMap::GetSingleton();
  auto       key         = control_map->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, a_device);

  switch (a_device) {
  case DeviceType::kMouse:
    key += kMouseOffset;
    break;
  case DeviceType::kKeyboard:
    key += kKeyboardOffset;
    break;
  case DeviceType::kGamepad:
    key = get_gamepad_index((RE::BSWin32GamepadDevice::Key)key);
    break;
  default:
    return 255;
  }

  return key;
}

auto process_event_player_animation(const RE::BSAnimationGraphEvent* a_event,
                                    RE::BSTEventSource<RE::BSAnimationGraphEvent>*, const Reflyem::Config& config)
    -> RE::BSEventNotifyControl {
  if (!a_event) {
    return RE::BSEventNotifyControl::kContinue;
  }

  if (a_event->holder && a_event->holder->IsPlayerRef()) {
    if (a_event->tag == "TKDR_DodgeStart") {
      auto actor = const_cast<RE::Actor*>(a_event->holder->As<RE::Actor>());
      if (!actor) {
        return RE::BSEventNotifyControl::kContinue;
      }

      auto drain_values = get_drain_value(*actor, config);
      if (drain_values) {
        drain_values->drain(*actor);
      }
      logger::debug(FMT_STRING("TK Dodge Start, Time {}!"), clock());

    } else if (a_event->tag == "TKDR_IFrameEnd") {
      logger::debug(FMT_STRING("Invulnerable Frame End!, Time {}"), clock());
    }
  }

  return RE::BSEventNotifyControl::kContinue;
}

auto is_dodge_button_actived(RE::ButtonEvent& button, std::uint32_t key, std::uint32_t const sprint_key,
                             const Reflyem::Config& config) -> bool {
  return config.tk_dodge_sprint_tapping_dodge
             ? button.IsUp() && button.HeldDuration() <= config.tk_dodge_key_up_delay && key == sprint_key
             : button.IsPressed() && key == config.tk_dodge_key;
};

auto is_not_allow_control_for_dodge(RE::UI& ui, RE::ControlMap& control_map, RE::PlayerControls& player_controls)
    -> bool {
  return ui.GameIsPaused() || !control_map.IsMovementControlsEnabled() || !control_map.IsFightingControlsEnabled() ||
         !player_controls.attackBlockHandler || !player_controls.attackBlockHandler->inputEventHandlingEnabled ||
         !player_controls.movementHandler || !player_controls.movementHandler->inputEventHandlingEnabled;
}

auto process_event_input_handler(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*,
                                 const Reflyem::Config& config) -> RE::BSEventNotifyControl {
  using EventType  = RE::INPUT_EVENT_TYPE;
  using DeviceType = RE::INPUT_DEVICE;

  if (!a_event) {
    return RE::BSEventNotifyControl::kContinue;
  }

  const auto ui               = RE::UI::GetSingleton();
  const auto control_map      = RE::ControlMap::GetSingleton();
  auto       player_character = RE::PlayerCharacter::GetSingleton();
  auto       player_controls  = RE::PlayerControls::GetSingleton();

  if ((!ui || !control_map || !player_character || !player_controls) ||
      is_not_allow_control_for_dodge(*ui, *control_map, *player_controls)) {
    return RE::BSEventNotifyControl::kContinue;
  }
  for (auto event = *a_event; event; event = event->next) {
    if (event->eventType != EventType::kButton) continue;

    const auto button = static_cast<RE::ButtonEvent*>(event);
    if (!button || (!button->IsPressed() && !button->IsUp())) continue;

    auto key = button->GetIDCode();
    switch (button->device.get()) {
    case DeviceType::kMouse:
      key += kMouseOffset;
      break;
    case DeviceType::kKeyboard:
      key += kKeyboardOffset;
      break;
    case DeviceType::kGamepad:
      key = get_gamepad_index((RE::BSWin32GamepadDevice::Key)key);
      break;
    default:
      continue;
    }

    const auto sprint_key = get_sprint_key(button->device.get());

    if (is_dodge_button_actived(*button, key, sprint_key, config)) {
      logger::debug("Dodge Key Press!");
      const std::string dodge_event = Reflyem::TKDodge::get_dodge_event();

      auto drain_values = get_drain_value(*player_character, config);
      if (!drain_values) {
        return RE::BSEventNotifyControl::kContinue;
      }

      if (!dodge_event.empty() &&
          is_allow_dodge(*player_character, *drain_values, config.tk_dodge_block_dodge_when_attack,
                         config.tk_dodge_block_dodge_when_power_attack)) {
        logger::debug(FMT_STRING("{} Trigger!"), dodge_event);
        bool IsDodging = false;
        if (player_character->GetGraphVariableBool("bIsDodging", IsDodging) && IsDodging) {
          logger::debug("Player is already dodging!");
          return RE::BSEventNotifyControl::kContinue;
        }

        player_character->SetGraphVariableInt("iStep", config.tk_dodge_step); // Set Step Dodge
        player_character->SetGraphVariableFloat("TKDR_IframeDuration",
                                                config.tk_dodge_iframe_duration); // Set invulnerable frame duration
        PlayerAnimationHandler::register_sink(player_character);                  // Register event sink
        player_character->NotifyAnimationGraph(dodge_event);                      // Send TK Dodge Event
        return RE::BSEventNotifyControl::kContinue;

      } else
        logger::debug("No Dodge Event Get!");
    }
  }

  return RE::BSEventNotifyControl::kContinue;
}

} // namespace TKDodge
} // namespace Reflyem