#include "utils/tuples.hpp"

#include <string>
#include <tuple>
#include <type_traits>

#include "gtest/gtest.h"
#include "utils/packs.hpp"

namespace compecs {

namespace {

template <typename T>
struct IntConvertible : std::bool_constant<std::is_convertible_v<T, int>> {};

template <typename T>
struct IsFloat : std::bool_constant<std::same_as<T, float>> {};
}  // namespace

TEST(TupleUtilities, TupleFilter) {
    std::tuple<int, double, std::string, long, float> tup{};
    TupleFilter<IntConvertible, decltype(tup)>::Result r;

    ASSERT_TRUE((std::same_as<decltype(r), std::tuple<int, double, long, float>>));
    ASSERT_TRUE((std::same_as<TupleFilter<IsFloat, decltype(tup)>::Result, std::tuple<float>>));
}

TEST(TupleUtilities, TupleConvert) {
    TupleConvert<std::tuple<int, long, float>> t{};
    ASSERT_TRUE((std::same_as<TypePack<int, long, float>, decltype(t)::Pack>));
}

}  // namespace compecs
