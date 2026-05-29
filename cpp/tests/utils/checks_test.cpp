#pragma once

#include "utils/checks.hpp"

#include "gtest/gtest.h"

namespace compecs {

TEST(ECSConceptTests, IsRead) {
    // checks IsRead properly filters
    EXPECT_TRUE((CheckRead<int>)) << "Copy can't modify OG data so should be read";
    EXPECT_TRUE((CheckRead<const int>)) << "Const is superfluous but should also be a read";
    EXPECT_FALSE((CheckRead<int&>)) << "Non const reference is obv not just a read";
    EXPECT_TRUE((CheckRead<const int&>)) << "you wouldn't use const cast would you";
}

TEST(ECSConceptTests, IsWrite) {
    // checks IsRead properly filters
    EXPECT_FALSE((CheckWrite<int>)) << "Copy can't modify OG data so not a write";
    EXPECT_FALSE((CheckWrite<const int>)) << "Const is superfluous, is also be not a write";
    EXPECT_TRUE((CheckWrite<int&>)) << "Non const reference is a write";
    EXPECT_FALSE((CheckWrite<const int&>)) << "you wouldn't use const cast would you";
}

}  // namespace compecs