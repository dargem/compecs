#include "utils/aligned_allocator.hpp"

#include <cstdint>
#include <new>

#include "gtest/gtest.h"

namespace compecs {

TEST(AlignedAllocator, CacheLineAlignedAllocation) {
    AlignedAllocator<int> allocator;

    std::vector<int*> allocations;
    constexpr static size_t NUM_CHECKS{1000};
    constexpr static size_t OBJECTS_ALLOCATED{10};

    allocations.reserve(NUM_CHECKS);

    for (size_t i{}; i < NUM_CHECKS; ++i) {
        int* a = allocator.allocate(OBJECTS_ALLOCATED);
        allocations.push_back(a);

        uintptr_t int_address = reinterpret_cast<uintptr_t>(a);
        // Expect it to always be aligned to size of a cache line
        EXPECT_TRUE(int_address % std::hardware_destructive_interference_size == 0);
    }

    while (!allocations.empty()) {
        int* var;
        allocator.deallocate(allocations.back(), OBJECTS_ALLOCATED);
        allocations.pop_back();
    }
}

TEST(AlignedAllocator, VectorGetsAlignedAllocation) {
    constexpr static size_t NUM_TESTS{100};

    for (size_t i{}; i < NUM_TESTS; ++i) {
        std::vector<int, AlignedAllocator<int>> a;
        a.push_back(5);

        std::vector<int, AlignedAllocator<int>> b;
        b.push_back(5);

        EXPECT_TRUE(reinterpret_cast<uintptr_t>(a.data()) %
                        std::hardware_destructive_interference_size ==
                    0);
        EXPECT_TRUE(reinterpret_cast<uintptr_t>(b.data()) %
                        std::hardware_destructive_interference_size ==
                    0);
    }
}

}  // namespace compecs