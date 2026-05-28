#ifndef COMPECS_LAMBDA_TRAITS_HPP
#define COMPECS_LAMBDA_TRAITS_HPP

#include <tuple>
#include <type_traits>

#include "utils/packs.hpp"

namespace compecs {

// Extract
template <typename T>
struct Disassemble {
    using type = void;
};

template <typename Ret, typename Class, typename... Args>
struct Disassemble<Ret (Class::*)(Args...) const> {
    using ReturnType = Ret;
    using ArgPack = TypePack<Args...>;
    using DecayedArgPack = TypePack<std::remove_cvref_t<Args>...>;
};

// Decltype the function pointer to the operator() then disassemble
template <typename F>
    requires requires(F f) { &F::operator(); }
using FunctionInfo = Disassemble<decltype(&F::operator())>;

}  // namespace compecs

#endif