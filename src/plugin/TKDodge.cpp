#include "plugin/TKDodge.hpp"
#include "Core.hpp"
#include "ResourceManager.hpp"
#include "plugin/AnimationEventHandler.hpp"

namespace Reflyem::TkDodge {

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

static bool is_staggered = false;
static float stagger_duration = 0.3f;
static std::string current_dodge_event = "none";
static float current_frame_duration = 0.f;

auto get_current_event_name() -> std::string&
{
  return current_dodge_event;
}
auto get_current_frame_duration() -> float
{
  return current_frame_duration;
}

enum : uint32_t
{
  kInvalid = static_cast<uint32_t>(-1),
  kKeyboardOffset = 0,
  kMouseOffset = 256,
  kGamepadOffset = 266
};

const std::vector DIRECTIONS{MovementPair(MovementDirections::kForward, "Forward"),
                             MovementPair(MovementDirections::kBackward, "Back"),
                             MovementPair(MovementDirections::kLeft, "Strafe Left"),
                             MovementPair(MovementDirections::kRight, "Strafe Right")};

static constexpr std::string_view bLeftForward = "bTkLeftForward"sv;
static constexpr std::string_view bLeftBackward = "bTkLeftBackward"sv;
static constexpr std::string_view bRightForward = "bTkRightForward"sv;
static constexpr std::string_view bRightBackward = "bTkRightBackward"sv;
static constexpr std::string_view bNeutral = "bTkNeutral"sv;

auto reset_all_graph_bool() -> void
{
  let player = RE::PlayerCharacter::GetSingleton();
  player->SetGraphVariableInt(bLeftForward, 0);
  player->SetGraphVariableInt(bLeftBackward, 0);
  player->SetGraphVariableInt(bRightForward, 0);
  player->SetGraphVariableInt(bRightBackward, 0);
  player->SetGraphVariableInt(bNeutral, 0);
}

auto set_graph_bool(const int value) -> void
{
  logi("Set PreDodgeBool to: {}", value);
  RE::PlayerCharacter::GetSingleton()->SetGraphVariableInt("PreDodgeBool", value);
}

auto calc_dodge_cost(RE::Actor& actor, const Config& config) -> float
{
  auto cost = actor.equippedWeight * config.tk_dodge().equipped_weight_mult();
  cost += (Core::get_actor_value_max(&actor, RE::ActorValue::kStamina) * (config.tk_dodge().percent_cost() / 100.f));
  cost += config.tk_dodge().flat_cost();
  cost +=
      Core::get_effects_magnitude_sum(Core::try_get_effects_by_keyword(&actor, config.tk_dodge().cost_from_mgef_kw()))
          .value_or(0.f);

  let keyword = config.tk_dodge().enable_dodge_in_stagger_keyword();
  if (keyword && Core::try_has_absolute_keyword(&actor, keyword) && is_staggered) {
    cost = cost * config.tk_dodge().stagger_cost_penalty_mult();
  }

  if (cost > config.tk_dodge().max_cost()) {
    cost = config.tk_dodge().max_cost();
  }

  if (cost < config.tk_dodge().min_cost()) {
    cost = config.tk_dodge().min_cost();
  }

  return cost;
}

auto get_actor_resources_mask(RE::Actor& actor, const Config& config) -> std::unique_ptr<ResourceManager::FormMask>
{
  ResourceManager::FormMask f_mask{{{0, 0, 0}}};
  logger::debug("start get actor mgef keyword"sv);
  f_mask.at(0).at(0) = 1;
  if (Core::try_has_absolute_keyword(&actor, config.tk_dodge().health_kw())) {
    f_mask.at(0).at(0) = 0;
    f_mask.at(0).at(1) = 1;
  }
  if (Core::try_has_absolute_keyword(&actor, config.tk_dodge().magicka_kw())) {
    f_mask.at(0).at(0) = 0;
    f_mask.at(0).at(2) = 1;
  }
  if (Core::try_has_absolute_keyword(&actor, config.tk_dodge().stamina_kw())) {
    f_mask.at(0).at(0) = 1;
  }
  logger::debug("end get actor mgef keyword"sv);
  return std::make_unique<ResourceManager::FormMask>(f_mask);
}

auto get_drain_value(RE::Actor& actor, const Config& config) -> std::shared_ptr<DrainValues>
{
  const auto actor_mask = get_actor_resources_mask(actor, config);
  const auto mask_sum = ResourceManager::calc_mask_sum(*actor_mask);
  const auto dodge_cost = calc_dodge_cost(actor, config);
  auto drain_values = ResourceManager::handle_mask_sum_for_drain_values(mask_sum, dodge_cost);

  return drain_values;
}

auto is_allow_pc_control_for_dodge(RE::PlayerCharacter& player) -> bool
{
  const auto& config = Config::get_singleton();

  let keyword = config.tk_dodge().enable_dodge_in_stagger_keyword();
  auto& actor_cache = Core::ActorsCache::get_singleton();
  letr actor_data = actor_cache.get_or_add(player.formID).get();
  if (keyword && Core::try_has_absolute_keyword(&player, keyword) && is_staggered &&
      actor_data.stagger_duration() >= stagger_duration) {
    return true;
  }

  auto is_attacking = config.tk_dodge().block_dodge_when_attack();
  if (config.tk_dodge().block_dodge_when_attack_keyword() &&
      Core::try_has_absolute_keyword(&player, config.tk_dodge().block_dodge_when_attack_keyword())) {
    is_attacking = !is_attacking;
  }

  auto is_power_attacking = config.tk_dodge().block_dodge_when_power_attack();
  if (config.tk_dodge().block_dodge_when_power_attack_keyword() &&
      Core::try_has_absolute_keyword(&player, config.tk_dodge().block_dodge_when_power_attack_keyword())) {
    is_power_attacking = !is_power_attacking;
  }

  auto is_casting = config.tk_dodge().block_dodge_when_casting();
  if (config.tk_dodge().block_dodge_when_casting_keyword() &&
      Core::try_has_absolute_keyword(&player, config.tk_dodge().block_dodge_when_casting_keyword())) {
    is_casting = !is_casting;
  }

  if (is_attacking) {
    is_attacking = player.IsAttacking();
  }

  if (is_power_attacking) {
    is_power_attacking = Core::is_power_attacking(player);
  }

  if (is_casting) {
    is_casting = Core::is_casting_actor(player);
  }

  if (config.tk_dodge().enable_dodge_when_attack_start()) {
    auto& actors_cache = Core::ActorsCache::get_singleton();
    auto& player_data = actors_cache.get_or_add(player.formID).get();
    if (player_data.start_hit_flag() && player_data.start_attack_duration() > 0.f) {
      is_power_attacking = false;
      is_attacking = false;
    }
  }

  let object = player.GetEquippedObject(false);
  if (object && object->IsWeapon() && object->As<RE::TESObjectWEAP>()->IsRanged()) {
    is_attacking = false;
  }

  auto is_cast_attack_allow = !is_attacking && !is_casting && !is_power_attacking;
  if (config.tk_dodge().enable_mco_recovery()) {
    bool MCO_IsInRecovery = false;
    if (player.GetGraphVariableBool("MCO_IsInRecovery", MCO_IsInRecovery) && MCO_IsInRecovery) {
      is_cast_attack_allow = true;
    }
  }

  return player.GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal &&
         player.GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal && player.GetFlyState() == RE::FLY_STATE::kNone &&
         !player.IsSneaking() && is_cast_attack_allow && !player.IsSwimming() && !player.IsInKillMove();
}

auto is_enough_resource_for_dodge(RE::Actor& actor, const DrainValues& drain_values) -> bool
{

  letr config = Config::get_singleton();

  auto magicka = actor.GetActorValue(RE::ActorValue::kMagicka);
  auto health = actor.GetActorValue(RE::ActorValue::kHealth);
  auto stamina = actor.GetActorValue(RE::ActorValue::kStamina);

  auto d_magicka = std::abs(drain_values.magicka);
  auto d_health = std::abs(drain_values.health);
  auto d_stamina = std::abs(drain_values.stamina);

  logger::debug("Check allow dodge, magicka: {} health: {} stamina: {} dv m h s: {} {} {}"sv,
                magicka,
                health,
                stamina,
                d_magicka,
                d_health,
                d_stamina);

  if (d_magicka > 0.f && d_magicka > magicka) {
    if (magicka > 0.f && config.tk_dodge().enable_dodge_when_not_enough_resource() &&
        magicka >= config.tk_dodge().min_cost()) {
    } else {
      Core::flash_hud_meter(RE::ActorValue::kMagicka);
      return false;
    }
  }
  if (d_health > 0.f && d_health > health) {
    if (health > 0.f && config.tk_dodge().enable_dodge_when_not_enough_resource() &&
        health >= config.tk_dodge().min_cost()) {
    } else {
      Core::flash_hud_meter(RE::ActorValue::kHealth);
      return false;
    }
  }
  if (d_stamina > 0.f && d_stamina > stamina) {
    if (stamina > 0.f && config.tk_dodge().enable_dodge_when_not_enough_resource() &&
        stamina >= config.tk_dodge().min_cost()) {
    } else {
      Core::flash_hud_meter(RE::ActorValue::kStamina);
      return false;
    }
  }

  logger::debug("Allow dodge"sv);

  return true;
}

auto is_allow_dodge(RE::PlayerCharacter& player, const DrainValues& drain_values) -> bool
{
  return is_allow_pc_control_for_dodge(player) && is_enough_resource_for_dodge(player, drain_values);
}

auto get_key_hold_duration(const std::uint32_t a_index, float& result) -> bool
{
  const auto input_mgr = RE::BSInputDeviceManager::GetSingleton();
  if (!input_mgr) {
    return false;
  }
  const auto keyboard = input_mgr->GetKeyboard();
  if (!keyboard) {
    return false;
  }

  let key_state = keyboard->curState[a_index]; // uint8_t
  let is_pressed = (key_state & 0x80) != 0;    // bool

  logger::debug("KeyState {} IsPressed {}"sv, key_state, is_pressed);

  if (is_pressed && keyboard->deviceButtons.find(a_index)->second) {
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

  for (auto& dir : DIRECTIONS) {
    auto key = ctrl_map->GetMappedKey(dir.second, RE::INPUT_DEVICE::kKeyboard);

    float hold_dur;
    if (get_key_hold_duration(key, hold_dur) && (!packed_dir.has_value() || hold_dur <= record_dur)) {
      packed_dir.emplace(dir.first);
      record_dur = hold_dur;
      logger::debug("Get a Held Direction Key Index {:x}, Duration {}"sv, key, record_dur);
    }
  }

  return packed_dir.has_value() ? packed_dir.value() : MovementDirections::kNone;
}

auto normal_absolute_angle(float angle) -> float
{
  static constexpr auto pi = 3.14159265f;
  static constexpr auto evaluted = 2 * pi;

  while (angle < 0) {
    angle = angle + evaluted;
  }
  while (angle > 2 * pi) {
    angle = angle - evaluted;
  }
  return angle;
}

inline float Vec2Length(const RE::NiPoint2& vec)
{
  return std::sqrtf(vec.x * vec.x + vec.y * vec.y);
}

inline RE::NiPoint2 Vec2Normalize(RE::NiPoint2& vec)
{
  RE::NiPoint2 ret(0.f, 0.f);
  float vecLength = Vec2Length(vec);
  if (vecLength == 0) {
    return ret;
  }
  float invlen = 1.0f / vecLength;
  ret.x = vec.x * invlen;
  ret.y = vec.y * invlen;
  return ret;
}

inline float DotProduct(RE::NiPoint2& a, RE::NiPoint2& b)
{
  return a.x * b.x + a.y * b.y;
}

inline float CrossProduct(RE::NiPoint2& a, RE::NiPoint2& b)
{
  return a.x * b.y - a.y * b.x;
}

inline float GetAngle(RE::NiPoint2& a, RE::NiPoint2& b)
{
  return atan2(CrossProduct(a, b), DotProduct(a, b));
}

#define PI8 0.39269908f

MovementDirections GetDodgeEvent()
{
  auto normalizedInputDirection = Vec2Normalize(RE::PlayerControls::GetSingleton()->data.prevMoveVec);
  if (normalizedInputDirection.x == 0.f && normalizedInputDirection.y == 0.f) {
    return MovementDirections::kNone;
  }

  auto tdm_freeMov = RE::TESForm::LookupByEditorID<RE::TESGlobal>("TDM_DirectionalMovement");
  if (tdm_freeMov && tdm_freeMov->value) {
    return MovementDirections::kForward;
  } else {
    RE::NiPoint2 forwardVector(0.f, 1.f);
    float dodgeAngle = GetAngle(normalizedInputDirection, forwardVector);
    if (dodgeAngle >= -2 * PI8 && dodgeAngle < 2 * PI8) {
      return MovementDirections::kForward;
    } else if (dodgeAngle >= -6 * PI8 && dodgeAngle < -2 * PI8) {
      return MovementDirections::kLeft;
    } else if (dodgeAngle >= 6 * PI8 || dodgeAngle < -6 * PI8) {
      return MovementDirections::kBackward;
    } else if (dodgeAngle >= 2 * PI8 && dodgeAngle < 6 * PI8) {
      return MovementDirections::kRight;
    }
  }

  return MovementDirections::kNone;
}

auto get_game_pad_direction_value() -> MovementDirections
{
  const auto input_mgr = RE::BSInputDeviceManager::GetSingleton();

  logger::debug("Get gamepad direction value"sv);

  if (input_mgr && input_mgr->GetGamepad()) {
    // ReSharper disable once CppCStyleCast
    if (const auto game_pad = (RE::BSWin32GamepadDevice*)(input_mgr->GetGamepad())) {
      logger::debug("Current LX is {}, Current LY is {}"sv, game_pad->currentLX, game_pad->currentLY);

      const float dir_xy[2] = {game_pad->currentLX, game_pad->currentLY};
      static constexpr float dir_base[2] = {0, 1.0f};

      float power = sqrt(std::powf(game_pad->currentLX, 2) + std::powf(game_pad->currentLY, 2));
      logger::debug("Current Power is {}"sv, power);

      float theta = (dir_xy[0] * dir_base[0] + dir_xy[1] * dir_base[1]) /
                    sqrt(dir_xy[0] * dir_xy[0] + dir_xy[1] * dir_xy[1]) /
                    sqrt(dir_base[0] * dir_base[0] + dir_base[1] * dir_base[1]);

      theta = game_pad->currentLX >= 0.f ? std::acos(theta) : -std::acos(theta);
      logger::debug("theta is {}"sv, theta);

      logi("pre dir");
      auto dir = normal_absolute_angle(theta);
      logi("post dir");
      dir /= 6.283185f;
      dir += 0.125f;
      dir *= 4.0f;
      dir = fmod(dir, 4.0f);
      dir = floor(dir);
      dir += 1.0f;
      logger::debug("GamePad Direction is {}"sv, dir);

      auto& config = Config::get_singleton();

      if (power > config.tk_dodge().gamepad_treshold()) {
        switch (static_cast<std::int32_t>(dir)) {
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

auto get_dodge_event() -> std::string
{
  auto direction = GetDodgeEvent();

  // const auto input_mgr = RE::BSInputDeviceManager::GetSingleton();
  //
  // if (input_mgr && input_mgr->GetKeyboard() && input_mgr->GetKeyboard()->IsEnabled() &&
  //     !input_mgr->IsGamepadConnected()) {
  //   direction = get_keyboard_direction_value();
  // } else if (input_mgr->IsGamepadConnected()) {
  //   direction = get_game_pad_direction_value();
  // }

  const auto tdm_free_mov = RE::TESForm::LookupByEditorID<RE::TESGlobal>("TDM_DirectionalMovement");
  // ReSharper disable once CppUseStructuredBinding
  auto& config = Config::get_singleton();

  // if (tdm_free_mov && tdm_free_mov->value != 0.f && direction != MovementDirections::kNone) {
  //   logger::debug("TDM Free Movement, Force to Forward Dodge!"sv);
  //   return "TKDodgeForward";
  // }
  if (tdm_free_mov && tdm_free_mov->value == 0.f && RE::PlayerCharacter::GetSingleton()->IsSprinting() &&
      !config.tk_dodge().sprint_tapping_dodge()) {
    logger::debug("Player is Sprinting in TDM target-lock, Disable Dodge!"sv);
    return "";
  }

  logger::debug("GetDodgeEvent: Direction {}"sv, static_cast<int>(direction));

  switch (direction) {
    case MovementDirections::kForward:
      return "TKDodgeForward";
    case MovementDirections::kBackward:
      return "TKDodgeBack";
    case MovementDirections::kLeft:
      return "TKDodgeLeft";
    case MovementDirections::kRight:
      return "TKDodgeRight";
    case MovementDirections::kNone:
    default:
      return "";
  }
}

std::string get_dodge_event_dodge_framework_all_directions()
{
  reset_all_graph_bool();
  let player = RE::PlayerCharacter::GetSingleton();
  let playerControls = RE::PlayerControls::GetSingleton();
  auto normalizedInputDirection = Vec2Normalize(playerControls->data.prevMoveVec);
  if (normalizedInputDirection.x == 0.f && normalizedInputDirection.y == 0.f) {
    logger::debug("neutral");
    player->SetGraphVariableFloat(bNeutral, 1);
    return "TKDodgeBack";
  }

  RE::NiPoint2 forwardVector(0.f, 1.f);
  float dodgeAngle = GetAngle(normalizedInputDirection, forwardVector);

  if (dodgeAngle >= -PI8 && dodgeAngle < PI8) {
    logger::debug("forward");
    return "TKDodgeForward";
  }
  if (dodgeAngle >= PI8 && dodgeAngle < 3 * PI8) {
    player->SetGraphVariableFloat(bRightForward, 1);
    logger::debug("right-forward");
    return "TKDodgeForward";
  }
  if (dodgeAngle >= 3 * PI8 && dodgeAngle < 5 * PI8) {
    logger::debug("right");
    return "TKDodgeRight";
  }
  if (dodgeAngle >= 5 * PI8 && dodgeAngle < 7 * PI8) {
    player->SetGraphVariableFloat(bRightBackward, 1);
    logger::debug("right-backward");
    return "TKDodgeBack";
  }
  if (dodgeAngle >= 7 * PI8 || dodgeAngle < 7 * -PI8) {
    logger::debug("backward");
    return "TKDodgeBack";
  }
  if (dodgeAngle >= 7 * -PI8 && dodgeAngle < 5 * -PI8) {
    player->SetGraphVariableFloat(bLeftBackward, 1);
    logger::debug("left-backward");
    return "TKDodgeBack";
  }
  if (dodgeAngle >= 5 * -PI8 && dodgeAngle < 3 * -PI8) {
    logger::debug("left");
    return "TKDodgeLeft";
  }
  if (dodgeAngle >= 3 * -PI8 && dodgeAngle < -PI8) {
    player->SetGraphVariableFloat(bLeftForward, 1);
    logger::debug("left-forward");
    return "TKDodgeForward";
  }

  logger::debug("neutral");
  player->SetGraphVariableFloat(bNeutral, 1);
  return "TKDodgeBack";
}

std::string get_dodge_event_dodge_framework()
{
  auto normalizedInputDirection = Vec2Normalize(RE::PlayerControls::GetSingleton()->data.prevMoveVec);
  if (normalizedInputDirection.x == 0.f && normalizedInputDirection.y == 0.f) {
    return "TKDodgeBack";
  }
  RE::NiPoint2 forwardVector(0.f, 1.f);
  float dodgeAngle = GetAngle(normalizedInputDirection, forwardVector);
  if (dodgeAngle >= -2 * PI8 && dodgeAngle < 2 * PI8) {
    return "TKDodgeForward";
  } else if (dodgeAngle >= -6 * PI8 && dodgeAngle < -2 * PI8) {
    return "TKDodgeLeft";
  } else if (dodgeAngle >= 6 * PI8 || dodgeAngle < -6 * PI8) {
    return "TKDodgeBack";
  } else if (dodgeAngle >= 2 * PI8 && dodgeAngle < 6 * PI8) {
    return "TKDodgeRight";
  }

  return "TKDodgeBack";
}

auto get_gamepad_index(const RE::BSWin32GamepadDevice::Key a_key) -> std::uint32_t
{
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

auto get_sprint_key(const RE::INPUT_DEVICE a_device) -> std::uint32_t
{
  using DeviceType = RE::INPUT_DEVICE;

  const auto control_map = RE::ControlMap::GetSingleton();
  auto key = control_map->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, a_device);

  switch (a_device) {
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
    default:
      return 255;
  }

  return key;
}

static bool allow_cost = false;
auto animation_handler(const RE::BSAnimationGraphEvent& event, const Config& config) -> void
{
  if (!event.holder->IsPlayerRef()) {
    return;
  }

  if (!allow_cost) {
    return;
  }

  if (const auto actor = const_cast<RE::Actor*>(event.holder->As<RE::Actor>()); actor) {
    switch (AnimationEventHandler::try_find_animation(event.tag.c_str())) {
      case AnimationEventHandler::AnimationEvent::kTkDodgeStart: {
        if (const auto drain_values = get_drain_value(*actor, config)) {
          drain_values->drain(*actor, true);
          set_graph_bool(0);

          if (actor->GetActorValue(RE::ActorValue::kHealth) <= 0.f && drain_values->health > 0.f) {
            Core::flash_hud_meter(RE::ActorValue::kHealth);
            Core::set_av_regen_delay(
                actor->currentProcess, RE::ActorValue::kHealth, config.resource_manager().regen_delay());
          }
          if (actor->GetActorValue(RE::ActorValue::kStamina) <= 0.f && drain_values->stamina > 0.f) {
            Core::flash_hud_meter(RE::ActorValue::kStamina);
            Core::set_av_regen_delay(
                actor->currentProcess, RE::ActorValue::kStamina, config.resource_manager().regen_delay());
          }
          if (actor->GetActorValue(RE::ActorValue::kMagicka) <= 0.f && drain_values->magicka > 0.f) {
            Core::flash_hud_meter(RE::ActorValue::kMagicka);
            Core::set_av_regen_delay(
                actor->currentProcess, RE::ActorValue::kMagicka, config.resource_manager().regen_delay());
          }
        }
        reset_all_graph_bool();
        is_staggered = false;
        allow_cost = false;

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

auto is_dodge_button_key(const std::uint32_t key, std::uint32_t const sprint_key, const Config& config) -> bool
{
  return config.tk_dodge().sprint_tapping_dodge() ? key == sprint_key : key == config.tk_dodge().key();
};

auto is_dodge_button_active(const RE::ButtonEvent& button, const Config& config) -> bool
{
  return config.tk_dodge().sprint_tapping_dodge()
             ? button.IsUp() && button.HeldDuration() <= config.tk_dodge().key_up_delay()
             : button.IsPressed();
};

auto is_not_allow_control_for_dodge(const RE::ControlMap& control_map, const RE::PlayerControls& player_controls)
    -> bool
{

  letr config = Config::get_singleton();
  auto player = RE::PlayerCharacter::GetSingleton();
  let keyword = config.tk_dodge().enable_dodge_in_stagger_keyword();
  let staggered = static_cast<bool>(player->actorState2.staggered);
  auto& actor_cache = Core::ActorsCache::get_singleton();
  letr actor_data = actor_cache.get_or_add(player->formID).get();
  if (keyword && Core::try_has_absolute_keyword(player, keyword) && staggered &&
      actor_data.stagger_duration() >= stagger_duration) {
    logd("Allow control from stagger");
    return false;
  }

  return !control_map.IsMovementControlsEnabled() || !control_map.IsFightingControlsEnabled() ||
         !player_controls.attackBlockHandler || !player_controls.attackBlockHandler->inputEventHandlingEnabled ||
         !player_controls.movementHandler || !player_controls.movementHandler->inputEventHandlingEnabled;
}

static bool dodge_key_pressed_in_menu = false;

auto process_event_input_handler(RE::InputEvent* const* event,
                                 RE::BSTEventSource<RE::InputEvent*>*,
                                 const Config& config) -> RE::BSEventNotifyControl
{
  using EventType = RE::INPUT_EVENT_TYPE;
  using DeviceType = RE::INPUT_DEVICE;

  if (!event) {
    return RE::BSEventNotifyControl::kContinue;
  }

  const auto control_map = RE::ControlMap::GetSingleton();
  const auto player_character = RE::PlayerCharacter::GetSingleton();
  const auto player_controls = RE::PlayerControls::GetSingleton();

  for (auto event_now = *event; event_now; event_now = event_now->next) {
    if (event_now->eventType != EventType::kButton)
      continue;

    const auto button = event_now->AsButtonEvent();
    if (!button) {
      continue;
    }

    auto key = button->GetIDCode();
    switch (button->device.get()) {
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
      default:
        continue;
    }

    const auto sprint_key = get_sprint_key(button->device.get());

    if (is_dodge_button_key(key, sprint_key, config)) {

      if (button->heldDownSecs <= 0.0) {
        logi("HoldTime 0, set flag to false");
        dodge_key_pressed_in_menu = false;
      }

      if ((!control_map || !player_character || !player_controls) ||
          is_not_allow_control_for_dodge(*control_map, *player_controls) || !Core::is_menu_allow()) {
        logi("MenuOpen OR NotAllow, skip: HoldSec {} Up {} Down {}",
             button->heldDownSecs,
             button->IsUp(),
             button->IsDown());
        if (button->heldDownSecs <= 0.0) {
          logi("HoldTime 0, set flag to true");
          dodge_key_pressed_in_menu = true;
        }
        return RE::BSEventNotifyControl::kContinue;
      }

      if (dodge_key_pressed_in_menu) {
        logi("Skip from up menu active, skip");
        return RE::BSEventNotifyControl::kContinue;
      }

      if (!button->IsPressed() && !button->IsUp()) {
        return RE::BSEventNotifyControl::kContinue;
      }

      if (!is_dodge_button_active(*button, config)) {
        return RE::BSEventNotifyControl::kContinue;
      }

      logi("DodgeAllow: HoldSec {} Up {} Down {}", button->heldDownSecs, button->IsUp(), button->IsDown());

      logger::debug("Dodge Key Press!"sv);
      // const std::string dodge_event = get_dodge_event();
      const std::string dodge_event = get_dodge_event_dodge_framework_all_directions();
      logger::debug("Dodge event is: {}"sv, dodge_event);
      bool is_dodging = false;
      if (player_character->GetGraphVariableBool("bIsDodging", is_dodging) && is_dodging) {
        logger::debug("Player is already dodging!"sv);
        return RE::BSEventNotifyControl::kContinue;
      }
      is_staggered = static_cast<bool>(player_character->actorState2.staggered);

      auto drain_values = get_drain_value(*player_character, config);
      if (!drain_values) {
        return RE::BSEventNotifyControl::kContinue;
      }

      if (!dodge_event.empty() && is_allow_dodge(*player_character, *drain_values)) {
        logger::debug("{} Trigger!"sv, dodge_event);
        logi("Allow dodge when IsStagger: {}", is_staggered);

        auto i_frame_duration = config.tk_dodge().iframe_duration();
        i_frame_duration +=
            Core::get_effects_magnitude_sum(
                Core::try_get_effects_by_keyword(player_character, config.tk_dodge().iframe_duration_mgef_kw()))
                .value_or(0.f);
        if (i_frame_duration < 0.f) {
          i_frame_duration = 0.f;
        }
        current_dodge_event = dodge_event;
        current_frame_duration = i_frame_duration;
        allow_cost = true;
        auto& cache = Core::ActorsCache::get_singleton();
        auto& data = cache.get_or_add(player_character->formID).get();
        if (is_staggered && !data.start_dodge_in_next_frame() &&
            Core::try_has_absolute_keyword(player_character, config.tk_dodge().enable_dodge_in_stagger_keyword())) {
          // player_character->NotifyAnimationGraph("IdleForceDefaultState");
          // player_character->NotifyAnimationGraph("TailCombatState");
          // player_character->NotifyAnimationGraph("TailCombatIdle");
          player_character->NotifyAnimationGraph("IdleStop");
          player_character->NotifyAnimationGraph("IdleStopInstant");
          player_character->NotifyAnimationGraph("staggerStop");
          player_character->SetGraphVariableInt("IsStaggering", 0);
          data.start_dodge_in_next_frame(true);
        } else {

          if (config.tk_dodge().interrupt_cast_when_dodge() && Core::is_casting_actor(*player_character)) {
            player_character->InterruptCast(true);
          }

          player_character->SetGraphVariableInt("iStep",
                                                config.tk_dodge().step()); // Set Step Dodge
          player_character->SetGraphVariableFloat("TKDR_IframeDuration",
                                                  i_frame_duration); // Set invulnerable frame duration
          // Send TK Dodge Event
          if (player_character->NotifyAnimationGraph(dodge_event) && config.cast_on_dodge().enable()) {
            Core::cast_on_handle_formlists(config.cast_on_dodge().formlist_needkw(),
                                           config.cast_on_dodge().formlist_spells(),
                                           player_character,
                                           nullptr);
          }
        }
        return RE::BSEventNotifyControl::kContinue;
      }
      logger::debug("No Dodge Event Get!"sv);
    }
  }

  return RE::BSEventNotifyControl::kContinue;
}

} // namespace Reflyem::TkDodge
