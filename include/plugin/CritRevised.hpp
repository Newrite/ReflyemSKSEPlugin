#pragma once

#include "Config.hpp"

// Шанс высчитывается глобально каждый фрейм.
// Урон для магии также считается в тиках активных маг эффектов.
// Не работает вместе со старым критом (отключает его).
// CastOnCrit срабатывает максимум один раз за фрейм.
// CastOnCrit срабатывает только при Hit (магический, или физический).
// Есть AV и для урона и для шанса. Их можно дополнительно модифицировать маг эффектами с нужными кейвордами.

namespace Reflyem::CritRevised {

auto update(RE::Actor* actor, const float) -> void;

auto on_magic_hit(const RE::Projectile* projectile, RE::TESObjectREFR* refr_target) -> void;

auto on_weapon_hit(RE::HitData& hit_data, RE::Actor* target) -> void;

auto modify_actor_value(const RE::ValueModifierEffect* this_,
                               RE::Actor* actor,
                               float& value,
                               const RE::ActorValue av,
                               const Config& config) -> void;

}
