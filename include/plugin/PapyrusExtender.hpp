#pragma once

namespace Reflyem::PapyrusExtender
{

auto is_papyrus_call_bow() -> bool;
auto papyrus_call_bow(const bool fake) -> void;

auto register_functions(RE::BSScript::IVirtualMachine* vm) -> bool;

}
