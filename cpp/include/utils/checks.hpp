#pragma once

#include <type_traits>

namespace compecs {

// If its a const reference or just a copy its just a read
// std::is_const_v would not work for const int&
// this is because its a non const reference, pointing to a const object
// aka it is low level const, but not high level const
// is_const_v only checks high level const
template <typename T>
concept IsRead = std::is_const_v<std::remove_reference_t<T>> ||
                 (!std::is_reference_v<T> && !std::is_rvalue_reference_v<T>);

// A non const reference is a write
template <typename T>
concept IsWrite = std::is_lvalue_reference_v<T> && !std::is_const_v<std::remove_reference_t<T>>;

}  // namespace compecs