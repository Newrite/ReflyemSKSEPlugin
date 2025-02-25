#include "DeclutteredLoot.hpp"

#include "Core.hpp"

namespace Reflyem::DeclutteredLoot {

bool declutter_mode = true;

static std::vector<RE::ShaderReferenceEffect*> shader_refs{};

auto gold_threshold(const RE::TESBoundObject* object) -> bool
{

  if (!object) {
    return false;
  }

  let gold_value = static_cast<float>(object->GetGoldValue());

  letr config = Config::get_singleton();
  if (config.decluttered_loot().enable_gold_threshold() &&
      object->GetGoldValue() >= config.decluttered_loot().gold_threshold()) {
    return true;
  }

  if (config.decluttered_loot().enable_gold_weight_threshold()) {
    let weight = object->GetWeight() > 0.0f ? object->GetWeight() : 1.0f;
    if ((gold_value / weight) >= config.decluttered_loot().gold_weight_threshold()) {
      return true;
    }
  }

  return false;
}

auto is_clutter_object(RE::TESBoundObject* object) -> bool
{

  letr config = Config::get_singleton();

  let is_misc = config.decluttered_loot().hide_misc() && Core::is_misc_form(object);
  let is_food = config.decluttered_loot().hide_food() && Core::is_food_form(object);
  let is_ingredient = config.decluttered_loot().hide_ingredient() && Core::is_ingredient_form(object);

  if (object && (is_misc || is_food || is_ingredient) && !Core::is_key_form(object) &&
      !Core::is_soul_gem_form(object) && !object->IsGold() && !object->IsLockpick() && !gold_threshold(object) &&
      !Core::try_form_has_keyword(object, Core::get_magic_disallow_keyword()) &&
      !Core::try_form_has_keyword(object, Core::get_gem_keyword()) &&
      !Core::try_form_has_keyword(object, config.decluttered_loot().exclusive_keyword()) &&
      !Core::try_form_has_keyword(object, Core::get_ore_ingot_keyword()) &&
      !Core::try_form_has_keyword(object, Core::get_soul_gem_keyword()) &&
      !Core::try_form_has_keyword(object, Core::get_vendor_item_key())) {
    logd("Junk object: {}", object->GetName());
    return true;
  }
  if (object) {
    logd("Not junk object: {}", object->GetName());
  }
  return false;
}

auto is_clutter_object(RE::TESObjectREFR* ref_object) -> bool
{

  letr config = Config::get_singleton();

  if (ref_object && is_clutter_object(ref_object->GetObjectReference()) && !ref_object->HasQuestObject() &&
      !Core::try_form_has_keyword(ref_object, config.decluttered_loot().exclusive_keyword()) &&
      !Core::is_script_attached(ref_object)) {
    logd("Junk object: {}", ref_object->GetDisplayFullName());
    return true;
  }
  if (ref_object) {
    logd("Not junk object: {}", ref_object->GetDisplayFullName());
  }

  return false;
}

auto can_highlight(RE::TESObjectREFR* ref_object, const float max_distance) -> bool
{
  if (!ref_object) {
    logd("REF: null");
    return false;
  }

  let distance = RE::PlayerCharacter::GetSingleton()->GetPosition().GetSquaredDistance(ref_object->GetPosition());

  if (distance > max_distance) {
    logd("REF: {} distance {} > max_distance {}", ref_object->GetDisplayFullName(), distance, max_distance);
    return false;
  }

  let object = ref_object->GetObjectReference();
  if (!object) {
    logd("REF: {} null object", ref_object->GetDisplayFullName());
    return false;
  }

  if (ref_object->IsDead()) {
    return true;
  }

  if (!Core::is_pickup_sound_form(object) && !Core::is_activator_form(object) && !Core::is_container_form(object)) {
    logd("REF: {} no sound \\ activator \\ container", ref_object->GetDisplayFullName());
    return false;
  }

  if (Core::is_furniture_form(object)) {
    logd("REF: {} is furniture", ref_object->GetDisplayFullName());
    return false;
  }

  logd("REF: {} CAN HIGHLIGHT, DISTANCE: {}", ref_object->GetDisplayFullName(), distance);
  return true;
}

auto process_input_event(RE::InputEvent* const* e, RE::BSTEventSource<RE::InputEvent*>*) -> void
{

  static constexpr auto HIGHLIGHT_DISTANCE = 3000000.f;

  static bool is_held_switch = false;
  static bool is_held_highlight = false;

  if (!e) {
    return;
  }

  let player = RE::PlayerCharacter::GetSingleton();
  if (!player || !player->Is3DLoaded()) {
    return;
  }

  if (!Core::is_menu_allow()) {
    return;
  }

  letr config = Config::get_singleton();

  let switch_key = config.decluttered_loot().switch_key();
  let highlight_key = config.decluttered_loot().highlight_key();

  for (auto event = *e; event; event = event->next) {
    if (let button = event->AsButtonEvent(); button) {
      let device = event->GetDevice();

      auto key = button->GetIDCode();

      switch (device) {
        case RE::INPUT_DEVICE::kMouse:
          key += SKSE::InputMap::kMacro_MouseButtonOffset;
          break;
        case RE::INPUT_DEVICE::kGamepad:
          key = SKSE::InputMap::GamepadMaskToKeycode(key);
          break;
        default:
          break;
      }

      if (key == highlight_key) {
        logd("Highlight key, {}", highlight_key);

        if (is_held_highlight && button->HeldDuration() < config.decluttered_loot().hold_highlight_key()) {
          logd("Key trigger, held mode highlight: {}", is_held_highlight);
          is_held_highlight = false;
        }

        if (!is_held_highlight && button->HeldDuration() >= config.decluttered_loot().hold_highlight_key()) {
          // for (auto shader_ref : shader_refs) {
          //   if (shader_ref) {
          //     shader_ref->ClearTarget();
          //     shader_ref->Detach();
          //   }
          // }
          shader_refs.clear();
          RE::TES::GetSingleton()->ForEachReference([=](RE::TESObjectREFR* ref_object) {
            if (ref_object && !ref_object->IsDisabled() && !ref_object->IsDeleted() && !is_clutter_object(ref_object) &&
                can_highlight(ref_object, HIGHLIGHT_DISTANCE)) {

              auto shader = config.decluttered_loot().shader();
              if (shader) {
                logd("AddShader: {}", shader->GetFormEditorID());
                shader_refs.push_back(
                    ref_object->ApplyEffectShader(shader, config.decluttered_loot().highlight_duration()));
              }
            }
            return RE::BSContainer::ForEachResult::kContinue;
          });
          if (config.decluttered_loot().enable_sound()) {
            Core::play_sound(config.decluttered_loot().highlight_activation_sound(),
                             RE::PlayerCharacter::GetSingleton());
          }
          is_held_highlight = true;
        }
      }

      if (key == switch_key) {
        logd("Switch key, {}", switch_key);

        if (is_held_switch && button->HeldDuration() < config.decluttered_loot().hold_switch_key()) {
          logd("Key trigger, held mode switch: {}", is_held_switch);
          is_held_switch = false;
        }

        if (!is_held_switch && button->HeldDuration() >= config.decluttered_loot().hold_switch_key()) {
          logd("Key trigger, current mode: {}", declutter_mode);
          is_held_switch = true;
          declutter_mode = !declutter_mode;
          RE::PlayerCharacter::GetSingleton()->UpdateCrosshairs();
        }
      }
    }
  }
}

}
