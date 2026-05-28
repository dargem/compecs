#include "utils/lambda_traits.hpp"

#include "gtest/gtest.h"

namespace compecs {

namespace {
using L = decltype([](int a, double b, float& c) { return 5.0f; });
using Info = FunctionInfo<L>;
}  // namespace

TEST(LambdaTraits, ReturnType) {
    // Should deduce float
    ASSERT_TRUE((std::same_as<float, Info::ReturnType>));
}

TEST(LambdaTraits, ArgPack) {
    ASSERT_TRUE((std::same_as<TypePack<int, double, float&>, Info::ArgPack>));
    ASSERT_TRUE((std::same_as<TypePack<int, double, float>, Info::DecayedArgPack>));
}

}  // namespace compecs