#ifndef COMPECS_PACKS_HPP
#define COMPECS_PACKS_HPP

#include <tuple>
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

template <template <typename> typename F, typename T>
    requires(std::convertible_to<decltype(F<T>::value), bool>)
struct PackFilter {
    static_assert(false, "PackFilter called without a pack");
};

namespace detail {

template <typename... T>
struct PackConcat {
    static_assert(false, "PackConcat called without packs");
};

template <typename... Ts>
struct PackConcat<TypePack<Ts...>> {
    using type = TypePack<Ts...>;
};

template <typename... Ts, typename... Us, typename... Rest>
struct PackConcat<TypePack<Ts...>, TypePack<Us...>, Rest...> {
    using type = PackConcat<TypePack<Ts..., Us...>, Rest...>::type;
};

}  // namespace detail

template <template <typename> typename F, typename... Ts>
    requires(std::convertible_to<decltype(F<Ts>::value), bool> && ...)
struct PackFilter<F, TypePack<Ts...>> {
    using Result =
        detail::PackConcat<std::conditional_t<F<Ts>::value, TypePack<Ts>, TypePack<>>...>::type;
};

template <typename T>
struct PackConvert {
    static_assert(false, "PackConvert called without using a pack");
};

template <typename... Ts>
struct PackConvert<TypePack<Ts...>> {
    using Tuple = std::tuple<Ts...>;
};

}  // namespace compecs

#endif