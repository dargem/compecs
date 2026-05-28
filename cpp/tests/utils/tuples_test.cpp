#include "utils/tuples.hpp"

#include <string>
#include <tuple>
#include <type_traits>

#include "gtest/gtest.h"

namespace compecs {

namespace {

template <typename T>
struct IntConvertible : std::bool_constant<std::is_convertible_v<T, int>> {};

}  // namespace

TEST(TupleUtilities, TupleFilter) {
    std::tuple<int, double, std::string, long> tup{};
    TupleFilter<IntConvertible, decltype(tup)>::Result r;

    ASSERT_TRUE((std::same_as<decltype(r), std::tuple<int, double, long>>));
}

}  // namespace compecs
