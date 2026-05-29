#pragma once

#include <cassert>
#include <cstdint>

#include "simd/traits.hpp"

namespace compecs {

using S = simd_traits::Set;

// This is effectively a wrapper around our SIMD Set trait for an int vector register
class alignas(S::bytes) IntVec {
   public:
    static constexpr size_t ELEMENTS{S::bytes / sizeof(int)};
    static constexpr size_t BYTES{S::bytes};

    S::__mi data;
    IntVec(int* mem_addr) : data(S::load_si((S::__mi*)mem_addr)) {
        assert(reinterpret_cast<uintptr_t>(mem_addr) % BYTES == 0);
    }

    IntVec(S::__mi d) : data(d) {};

    friend IntVec operator+(IntVec a, IntVec b) { return {S::add_epi32(a.data, b.data)}; }
    friend IntVec operator-(IntVec a, IntVec b) { return {S::sub_epi32(a.data, b.data)}; }
    IntVec& operator+=(IntVec other) {
        data = S::add_epi32(data, other.data);
        return *this;
    }
    IntVec& operator-=(IntVec other) {
        data = S::sub_epi32(data, other.data);
        return *this;
    }
};

}  // namespace compecs