#pragma once

#include <type_traits>

namespace compecs {

// If its a const reference or just a copy its just a read
// std::is_const_v would not work for const int&
// this is because its a non const reference, pointing to a const object
// aka it is low level const, but not high level const
// is_const_v only checks high level const
template <typename T>
concept CheckRead = std::is_const_v<std::remove_reference_t<T>> ||
                    (!std::is_reference_v<T> && !std::is_rvalue_reference_v<T>);

template <typename T>
struct IsRead : std::bool_constant<CheckRead<T>> {};

// A non const reference is a write
template <typename T>
concept CheckWrite = std::is_lvalue_reference_v<T> && !std::is_const_v<std::remove_reference_t<T>>;

template <typename T>
struct IsWrite : std::bool_constant<CheckWrite<T>> {};



}  // namespace compecs