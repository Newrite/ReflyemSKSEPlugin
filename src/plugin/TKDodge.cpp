#include "plugin/TKDodge.hpp"
#include "Core.hpp"
#include "plugin/AnimationEventHandler.hpp"
#include "plugin/ResourceManager.hpp"

namespace Reflyem::TkDodge
{

enum class MovementDirections
{
  kNone = 0,
  kForward,
  kRight,
  kBackward,
  kLeft,
};

using MovementPair = std::pair<MovementDirections, std::string_view>;
using DrainValues = ResourceManager::ResourceDrain;
using MgefMask = ResourceManager::FormMask;

enum : uint32_t
{
  kInvalid = static_cast<uint32_t>(-1),
  kKeyboardOffset = 0,
  kMouseOffset = 256,
  kGamepadOffset = 266
};

const std::vector DIRECTIONS{
    MovementPair(MovementDirections::kForward, "Forward"),
    MovementPair(MovementDirections::kBackward, "Back"),
    MovementPair(MovementDirections::kLeft, "Strafe Left"),
    MovementPair(MovementDirections::kRight, "Strafe Right")};

auto get_actor_effects_mask(RE::Actor& actor, const Config& config) -> std::unique_ptr<MgefMask>
{
  MgefMask f_mask{{{0, 0, 0}}};
  logger::debug("start get actor mgef keyword"sv);
  f_mask.at(0).at(0) = 1;
  if (Core::try_has_absolute_keyword(&actor, config.tk_dodge().health_kw()))
    {
      f_mask.at(0).at(0) = 0;
      f_mask.at(0).at(1) = 1;
    }
  if (Core::try_has_absolute_keyword(&actor, config.tk_dodge().magicka_kw()))
    {
      f_mask.at(0).at(0) = 0;
      f_mask.at(0).at(2) = 1;
    }
  if (Core::try_has_absolute_keyword(&actor, config.tk_dodge().stamina_kw()))
    {
      f_mask.at(0).at(0) = 1;
    }
  logger::debug("end get actor mgef keyword"sv);
  return std::make_unique<MgefMask>(f_mask);
}

auto calc_dodge_cost(RE::Actor& actor, const Config& config) -> float
{
  auto cost = actor.equippedWeight * config.tk_dodge().equipped_weight_mult();
  cost += config.tk_dodge().flat_cost();
  cost += Core::get_effects_magnitude_sum(
              Core::try_get_effects_by_keyword(&actor, config.tk_dodge().cost_from_mgef_kw()))
              .value_or(0.f);

  if (cost > config.tk_dodge().max_cost()) { cost = config.tk_dodge().max_cost(); }

  if (cost < config.tk_dodge().min_cost()) { cost = config.tk_dodge().min_cost(); }

  return cost;
}

auto get_drain_value(RE::Actor& actor, const Config& config) -> std::shared_ptr<DrainValues>
{
  const auto mgef_mask = get_actor_effects_mask(actor, config);
  const auto mask_sum = ResourceManager::calc_mask_sum(*mgef_mask);
  const auto dodge_cost = calc_dodge_cost(actor, config);
  auto drain_values = ResourceManager::handle_mask_sum_for_drain_values(mask_sum, dodge_cost);

  return drain_values;
}

auto is_allow_pc_control_for_dodge(RE::PlayerCharacter& player) -> bool
{
  const auto& config = Config::get_singleton();

  auto is_attacking = config.tk_dodge().block_dodge_when_attack();
  if (config.tk_dodge().block_dodge_when_attack_keyword() &&
      Core::try_has_absolute_keyword(&player, config.tk_dodge().block_dodge_when_attack_keyword()))
    {
      is_attacking = !is_attacking;
    }

  auto is_power_attacking = config.tk_dodge().block_dodge_when_power_attack();
  if (config.tk_dodge().block_dodge_when_power_attack_keyword() &&
      Core::try_has_absolute_keyword(
          &player,
          config.tk_dodge().block_dodge_when_power_attack_keyword()))
    {
      is_power_attacking = !is_power_attacking;
    }

  auto is_casting = config.tk_dodge().block_dodge_when_casting();
  if (config.tk_dodge().block_dodge_when_casting_keyword() &&
      Core::try_has_absolute_keyword(&player, config.tk_dodge().block_dodge_when_casting_keyword()))
    {
      is_casting = !is_casting;
    }

  if (is_attacking) { is_attacking = player.IsAttacking(); }

  if (is_power_attacking) { is_power_attacking = Core::is_power_attacking(player); }

  if (is_casting) { is_casting = Core::is_casting_actor(player); }

  return player.GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal &&
         player.GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal &&
         player.GetFlyState() == RE::FLY_STATE::kNone && !player.IsSneaking() && !is_attacking &&
         !is_casting && !is_power_attacking && !player.IsSwimming() && !player.IsInKillMove();
}

auto is_enough_resource_for_dodge(RE::Actor& actor, const DrainValues& drain_values) -> bool
{
  auto magicka = actor.GetActorValue(RE::ActorValue::kMagicka);
  auto health = actor.GetActorValue(RE::ActorValue::kHealth);
  auto stamina = actor.GetActorValue(RE::ActorValue::kStamina);

  auto d_magicka = std::abs(drain_values.magicka);
  auto d_health = std::abs(drain_values.health);
  auto d_stamina = std::abs(drain_values.stamina);

  logger::debug(
      "Check allow dodge, magicka: {} health: {} stamina: {} dv m h s: {} {} {}"sv,
      magicka,
      health,
      stamina,
      d_magicka,
      d_health,
      d_stamina);

  if (d_magicka > 0.f && d_magicka > magicka) { return false; }
  if (d_health > 0.f && d_health > health) { return false; }
  if (d_stamina > 0.f && d_stamina > stamina) { return false; }

  logger::debug("Allow dodge"sv);

  return true;
}

auto is_allow_dodge(RE::PlayerCharacter& player, const DrainValues& drain_values) -> bool
{
  return is_allow_pc_control_for_dodge(player) &&
         is_enough_resource_for_dodge(player, drain_values);
}

auto get_key_hold_duration(const std::uint32_t a_index, float& result) -> bool
{
  const auto input_mgr = RE::BSInputDeviceManager::GetSingleton();
  if (!input_mgr) { return false; }
  const auto keyboard = input_mgr->GetKeyboard();
  if (!keyboard) { return false; }

  let key_state = keyboard->curState[a_index]; // uint8_t
  let is_pressed = (key_state & 0x80) != 0; // bool

  logger::debug("KeyState {} IsPressed {}"sv, key_state, is_pressed);

  if (is_pressed && keyboard->deviceButtons.find(a_index)->second)
    {
      result = keyboard->deviceButtons.find(a_index)->second->heldDownSecs;
      return true;
    }

  return false;
}

auto get_keyboard_direction_value() -> MovementDirections
{
  const auto ctrl_map = RE::ControlMap::GetSingleton();

  logger::debug("Get keyboard direction value"sv);

  std::optional<MovementDirections> packed_dir;
  float record_dur = 0.f;

  for (auto& dir : DIRECTIONS)
    {
      auto key = ctrl_map->GetMappedKey(dir.second, RE::INPUT_DEVICE::kKeyboard);

      float hold_dur;
      if (get_key_hold_duration(key, hold_dur) &&
          (!packed_dir.has_value() || hold_dur <= record_dur))
        {
          packed_dir.emplace(dir.first);
          record_dur = hold_dur;
          logger::debug("Get a Held Direction Key Index {:x}, Duration {}"sv, key, record_dur);
        }
    }

  return packed_dir.has_value() ? packed_dir.value() : MovementDirections::kNone;
}

auto normal_absolute_angle(float angle) -> float
{
  static constexpr auto pi = 3.1415927f;

  while (angle < 0) angle += 2 * pi;
  while (angle > 2 * pi) angle -= 2 * pi;
  return angle;
}

auto get_game_pad_direction_value() -> MovementDirections
{
  const auto input_mgr = RE::BSInputDeviceManager::GetSingleton();

  logger::debug("Get gamepad direction value"sv);

  if (input_mgr && input_mgr->GetGamepad())
    {
      // ReSharper disable once CppCStyleCast
      if (const auto game_pad = (RE::BSWin32GamepadDevice*)(input_mgr->GetGamepad()))
        {
          logger::debug(
              "Current LX is {}, Current LY is {}"sv,
              game_pad->currentLX,
              game_pad->currentLY);

          const float dir_xy[2] = {game_pad->currentLX, game_pad->currentLY};
          static constexpr float dir_base[2] = {0, 1.0f};

          float power = sqrt(std::powf(game_pad->currentLX, 2) + std::powf(game_pad->currentLY, 2));
          logger::debug("Current Power is {}"sv, power);

          float theta = (dir_xy[0] * dir_base[0] + dir_xy[1] * dir_base[1]) /
                        sqrt(dir_xy[0] * dir_xy[0] + dir_xy[1] * dir_xy[1]) /
                        sqrt(dir_base[0] * dir_base[0] + dir_base[1] * dir_base[1]);

          theta = game_pad->currentLX >= 0.f ? std::acos(theta) : -std::acos(theta);
          logger::debug("theta is {}"sv, theta);

          auto dir = normal_absolute_angle(theta);
          dir /= 6.283185f;
          dir += 0.125f;
          dir *= 4.0f;
          dir = fmod(dir, 4.0f);
          dir = floor(dir);
          dir += 1.0f;
          logger::debug("GamePad Direction is {}"sv, dir);

          auto& config = Config::get_singleton();

          if (power > config.tk_dodge().gamepad_treshold())
            {
              switch (static_cast<std::int32_t>(dir))
                {
                case 1: return MovementDirections::kForward;
                case 2: return MovementDirections::kRight;
                case 3: return MovementDirections::kBackward;
                case 4: return MovementDirections::kLeft;
                default: return MovementDirections::kNone;
                }
            }
        }
    }

  return MovementDirections::kNone;
}

auto get_dodge_event() -> std::string
{
  auto direction = MovementDirections::kNone;

  const auto input_mgr = RE::BSInputDeviceManager::GetSingleton();

  if (input_mgr && input_mgr->GetKeyboard() && input_mgr->GetKeyboard()->IsEnabled() &&
      !input_mgr->IsGamepadConnected())
    {
      direction = get_keyboard_direction_value();
    }
  else if (input_mgr->IsGamepadConnected()) { direction = get_game_pad_direction_value(); }

  const auto tdm_free_mov = RE::TESForm::LookupByEditorID<RE::TESGlobal>("TDM_DirectionalMovement");
  // ReSharper disable once CppUseStructuredBinding
  auto& config = Config::get_singleton();

  if (tdm_free_mov && tdm_free_mov->value != 0.f && direction != MovementDirections::kNone)
    {
      logger::debug("TDM Free Movement, Force to Forward Dodge!"sv);
      return "TKDodgeForward";
    }
  if (tdm_free_mov && tdm_free_mov->value == 0.f &&
      RE::PlayerCharacter::GetSingleton()->IsSprinting() &&
      !config.tk_dodge().sprint_tapping_dodge())
    {
      logger::debug("Player is Sprinting in TDM target-lock, Disable Dodge!"sv);
      return "";
    }

  logger::debug("GetDodgeEvent: Direction {}"sv, static_cast<int>(direction));

  switch (direction)
    {
    case MovementDirections::kForward: return "TKDodgeForward";
    case MovementDirections::kBackward: return "TKDodgeBack";
    case MovementDirections::kLeft: return "TKDodgeLeft";
    case MovementDirections::kRight: return "TKDodgeRight";
    case MovementDirections::kNone:
    default: return "";
    }
}

auto get_gamepad_index(const RE::BSWin32GamepadDevice::Key a_key) -> std::uint32_t
{
  using Key = RE::BSWin32GamepadDevice::Key;

  std::uint32_t index;
  switch (a_key)
    {
    case Key::kUp: index = 0; break;
    case Key::kDown: index = 1; break;
    case Key::kLeft: index = 2; break;
    case Key::kRight: index = 3; break;
    case Key::kStart: index = 4; break;
    case Key::kBack: index = 5; break;
    case Key::kLeftThumb: index = 6; break;
    case Key::kRightThumb: index = 7; break;
    case Key::kLeftShoulder: index = 8; break;
    case Key::kRightShoulder: index = 9; break;
    case Key::kA: index = 10; break;
    case Key::kB: index = 11; break;
    case Key::kX: index = 12; break;
    case Key::kY: index = 13; break;
    case Key::kLeftTrigger: index = 14; break;
    case Key::kRightTrigger: index = 15; break;
    default: index = kInvalid; break;
    }

  return index != kInvalid ? index + kGamepadOffset : kInvalid;
}

auto get_sprint_key(const RE::INPUT_DEVICE a_device) -> std::uint32_t
{
  using DeviceType = RE::INPUT_DEVICE;

  const auto control_map = RE::ControlMap::GetSingleton();
  auto key = control_map->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, a_device);

  switch (a_device)
    {
      case DeviceType::kMouse: {
        key += kMouseOffset;
        break;
      }
      case DeviceType::kKeyboard: {
        key += kKeyboardOffset;
        break;
      }
      case DeviceType::kGamepad: {
        key = get_gamepad_index(static_cast<RE::BSWin32GamepadDevice::Key>(key));
        break;
      }
    case DeviceType::kNone:
    case DeviceType::kVirtualKeyboard:
    case DeviceType::kTotal:
    default: return 255;
    }

  return key;
}

auto animation_handler(const RE::BSAnimationGraphEvent& event, const Config& config) -> void
{
  if (!event.holder->IsPlayerRef()) { return; }

  if (const auto actor = const_cast<RE::Actor*>(event.holder->As<RE::Actor>()); actor)
    {
      switch (AnimationEventHandler::try_find_animation(event.tag.c_str()))
        {
          case AnimationEventHandler::AnimationEvent::kTkDodgeStart: {
            if (const auto drain_values = get_drain_value(*actor, config))
              {
                drain_values->drain(*actor);
              }

            logger::debug("TK Dodge Start, Time {}!"sv, clock());
            return;
          }
          case AnimationEventHandler::AnimationEvent::kTkDodgeIFrameEnd: {
            logger::debug("Invulnerable Frame End!, Time {}"sv, clock());
            return;
          }
        case AnimationEventHandler::AnimationEvent::kWeaponSwing:
        case AnimationEventHandler::AnimationEvent::kWeaponLeftSwing:
        case AnimationEventHandler::AnimationEvent::kJumpUp:
        case AnimationEventHandler::AnimationEvent::kBowDrawStart:
        case AnimationEventHandler::AnimationEvent::kBashExit:
        case AnimationEventHandler::AnimationEvent::kNone:;
        case AnimationEventHandler::AnimationEvent::kPreHitFrame:
        case AnimationEventHandler::AnimationEvent::kHitFrame:;
        }
    }
}

auto is_dodge_button_active(
    const RE::ButtonEvent& button,
    const std::uint32_t key,
    std::uint32_t const sprint_key,
    const Config& config) -> bool
{
  return config.tk_dodge().sprint_tapping_dodge()
             ? button.IsUp() && button.HeldDuration() <= config.tk_dodge().key_up_delay() &&
                   key == sprint_key
             : button.IsPressed() && key == config.tk_dodge().key();
};

auto is_not_allow_control_for_dodge(
    RE::UI& ui,
    const RE::ControlMap& control_map,
    const RE::PlayerControls& player_controls) -> bool
{
  return ui.GameIsPaused() || !control_map.IsMovementControlsEnabled() ||
         !control_map.IsFightingControlsEnabled() || !player_controls.attackBlockHandler ||
         !player_controls.attackBlockHandler->inputEventHandlingEnabled ||
         !player_controls.movementHandler ||
         !player_controls.movementHandler->inputEventHandlingEnabled;
}

auto process_event_input_handler(
    RE::InputEvent* const* event,
    RE::BSTEventSource<RE::InputEvent*>*,
    const Config& config) -> RE::BSEventNotifyControl
{
  using EventType = RE::INPUT_EVENT_TYPE;
  using DeviceType = RE::INPUT_DEVICE;

  if (!event) { return RE::BSEventNotifyControl::kContinue; }

  const auto ui = RE::UI::GetSingleton();
  const auto control_map = RE::ControlMap::GetSingleton();
  const auto player_character = RE::PlayerCharacter::GetSingleton();
  const auto player_controls = RE::PlayerControls::GetSingleton();

  if ((!ui || !control_map || !player_character || !player_controls) ||
      is_not_allow_control_for_dodge(*ui, *control_map, *player_controls))
    {
      return RE::BSEventNotifyControl::kContinue;
    }
  for (auto event_now = *event; event_now; event_now = event_now->next)
    {
      if (event_now->eventType != EventType::kButton) continue;

      const auto button = event_now->AsButtonEvent();
      if (!button || (!button->IsPressed() && !button->IsUp())) continue;

      auto key = button->GetIDCode();
      switch (button->device.get())
        {
          case DeviceType::kMouse: {
            key += kMouseOffset;
            break;
          }
          case DeviceType::kKeyboard: {
            key += kKeyboardOffset;
            break;
          }
          case DeviceType::kGamepad: {
            key = get_gamepad_index(static_cast<RE::BSWin32GamepadDevice::Key>(key));
            break;
          }
        case DeviceType::kNone:
        case DeviceType::kVirtualKeyboard:
        case DeviceType::kTotal:
        default: continue;
        }

      const auto sprint_key = get_sprint_key(button->device.get());

      if (is_dodge_button_active(*button, key, sprint_key, config))
        {
          logger::debug("Dodge Key Press!"sv);
          const std::string dodge_event = get_dodge_event();
          logger::debug("Dodge event is: {}"sv, dodge_event);

          auto drain_values = get_drain_value(*player_character, config);
          if (!drain_values) { return RE::BSEventNotifyControl::kContinue; }

          if (!dodge_event.empty() && is_allow_dodge(*player_character, *drain_values))
            {
              logger::debug("{} Trigger!"sv, dodge_event);
              bool is_dodging = false;
              if (player_character->GetGraphVariableBool("bIsDodging", is_dodging) && is_dodging)
                {
                  logger::debug("Player is already dodging!"sv);
                  return RE::BSEventNotifyControl::kContinue;
                }

              auto i_frame_duration = config.tk_dodge().iframe_duration();
              i_frame_duration +=
                  Core::get_effects_magnitude_sum(Core::try_get_effects_by_keyword(
                                                      player_character,
                                                      config.tk_dodge().iframe_duration_mgef_kw()))
                      .value_or(0.f);
              if (i_frame_duration < 0.f) { i_frame_duration = 0.f; }

              player_character->SetGraphVariableInt(
                  "iStep",
                  config.tk_dodge().step()); // Set Step Dodge
              player_character->SetGraphVariableFloat(
                  "TKDR_IframeDuration",
                  i_frame_duration); // Set invulnerable frame duration
              player_character->NotifyAnimationGraph(dodge_event); // Send TK Dodge Event
              return RE::BSEventNotifyControl::kContinue;
            }
          logger::debug("No Dodge Event Get!"sv);
        }
    }

  return RE::BSEventNotifyControl::kContinue;
}

} // namespace Reflyem::TkDodge