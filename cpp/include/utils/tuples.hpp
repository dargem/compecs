#ifndef COMPECS_TUPLES_HPP
#define COMPECS_TUPLES_HPP

#include <concepts>
#include <tuple>
#include <type_traits>

namespace compecs {

template <template <typename> typename F, typename T>
struct TupleFilter {
    static_assert(false, "TupleFilter called without using a tuple");
};

template <template <typename> typename F, typename... Ts>
    requires(std::convertible_to<decltype(F<Ts>::value), bool> && ...)
struct TupleFilter<F, std::tuple<Ts...>> {
    using Result = decltype(std::tuple_cat(
        std::conditional_t<F<Ts>::value, std::tuple<Ts>, std::tuple<>>{}...));
};

template <typename T>
struct TupleConvert {
    static_assert(false, "Tuple convert called on non tuple type");
};

// Forward declare type pack
template <typename... Ts>
struct TypePack;

template <typename... Ts>
struct TupleConvert<std::tuple<Ts...>> {
    using Pack = TypePack<Ts...>;
};

}  // namespace compecs

#endif