#pragma once

#include "utils/checks.hpp"

#include "gtest/gtest.h"

namespace compecs {

TEST(ECSConceptTests, IsRead) {
    // checks IsRead properly filters
    EXPECT_TRUE((IsRead<int>)) << "Copy can't modify OG data so should be read";
    EXPECT_TRUE((IsRead<const int>)) << "Const is superfluous but should also be a read";
    EXPECT_FALSE((IsRead<int&>)) << "Non const reference is obv not just a read";
    EXPECT_TRUE((IsRead<const int&>)) << "you wouldn't use const cast would you";
}

TEST(ECSConceptTests, IsWrite) {
    // checks IsRead properly filters
    EXPECT_FALSE((IsWrite<int>)) << "Copy can't modify OG data so not a write";
    EXPECT_FALSE((IsWrite<const int>)) << "Const is superfluous, is also be not a write";
    EXPECT_TRUE((IsWrite<int&>)) << "Non const reference is a write";
    EXPECT_FALSE((IsWrite<const int&>)) << "you wouldn't use const cast would you";
}

}  // namespace compecs