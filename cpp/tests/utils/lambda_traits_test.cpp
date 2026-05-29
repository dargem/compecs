#include "utils/lambda_traits.hpp"

#include <string>

#include "gtest/gtest.h"

namespace compecs {

namespace {
using L = decltype([](int a, double b, float& c) { return 5.0f; });
using L2 = decltype([](double a, int b, std::string c) {});
using LInfo = FunctionInfo<L>;
using L2Info = FunctionInfo<L2>;
}  // namespace

TEST(LambdaTraits, ReturnType) {
    // Should deduce float
    ASSERT_TRUE((std::same_as<float, LInfo::ReturnType>));
    ASSERT_TRUE((std::same_as<void, L2Info::ReturnType>));
}

TEST(LambdaTraits, ArgPacks) {
    ASSERT_TRUE((std::same_as<TypePack<int, double, float&>, LInfo::ArgPack>));
    ASSERT_TRUE((std::same_as<TypePack<int, double, float>, LInfo::DecayedArgPack>));

    ASSERT_TRUE((std::same_as<TypePack<double, int, std::string>, L2Info::ArgPack>));
}

}  // namespace compecs