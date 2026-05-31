#include "utils/packs.hpp"

#include <string>
#include <type_traits>

#include "gtest/gtest.h"

namespace compecs {

template <typename T>
struct IntConvertible : std::bool_constant<std::convertible_to<T, int>> {};

TEST(PackTests, PackFilter) {
    using A = TypePack<double, int, std::string, long>;
    using R = PackFilter<IntConvertible, A>::Result;
    // using R = TypePack<double, int>;

    ASSERT_TRUE((std::same_as<TypePack<double, int, long>, R>));
}

}  // namespace compecs