// ReSharper disable CppInconsistentNaming
#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <Windows.h>
#include <Nameof.hpp>
#include <ranges>

namespace stl
{
using namespace SKSE::stl;

template <class F, size_t index, class T>
void write_vfunc()
{
  REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[index] };
  T::func = vtbl.write_vfunc(T::size, T::thunk);
}

template <class F, class T>
void write_vfunc()
{
  write_vfunc<F, 0, T>();
}

template <class T>
void write_thunk_call(std::uintptr_t a_src)
{
  auto& trampoline = SKSE::GetTrampoline();
  SKSE::AllocTrampoline(14);

  T::func = trampoline.write_call<5>(a_src, T::thunk);
}
}

namespace logger = SKSE::log;
using namespace std::literals;

namespace ranges = std::ranges;
namespace views = ranges::views;


#define RELOCATION_OFFSET(SE, AE) SE

#define let const auto
#define letr const auto&
#define letp const auto*
#define let_expr constexpr auto

#define logi logger::info
#define loge logger::error
#define logd logger::debug
#define logw logger::warning


