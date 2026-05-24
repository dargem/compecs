#ifndef COMPECS_PACKS_HPP
#define COMPECS_PACKS_HPP

#include <type_traits>

namespace compecs {

template <typename... Ts>
struct TypePack {};

namespace detail {
template <typename... Ts>
struct IsTypePackImpl {
    // Calling ::value type trait forces full instantiation, SFINAE only catches error in immediate
    // context so the parameter substitution itself, once evaluating the members its past that
    // point. So this hard fails if it isn't a type pack which is wanted.
    static_assert(false, "This is not a TypePack");
};

// This will capture if it is a type pack due to higher specificity
template <typename... Ts>
struct IsTypePackImpl<TypePack<Ts...>> : std::true_type {};
}  // namespace detail

template <typename T>
concept IsTypePack = detail::IsTypePackImpl<T>::value;

}  // namespace compecs

#endif