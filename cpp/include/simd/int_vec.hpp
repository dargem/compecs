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
    IntVec(int* mem_addr) : data(S::load_si(reinterpret_cast<S::__mi*>(mem_addr))) {
        assert(reinterpret_cast<uintptr_t>(mem_addr) % BYTES == 0);
    }

    IntVec(S::__mi d) : data(d) {};

    void store(int* mem_addr) { S::store_si(reinterpret_cast<S::__mi*>(mem_addr), data); }

    friend IntVec operator+(const IntVec& a, const IntVec& b) {
        return S::add_epi32(a.data, b.data);
    }

    friend IntVec operator-(const IntVec& a, const IntVec& b) {
        return S::sub_epi32(a.data, b.data);
    }

    // This is a mullo multiply so will trim overflow
    friend IntVec operator*(const IntVec& a, const IntVec& b) {
        return S::mullo_epi32(a.data, b.data);
    }

    IntVec operator<<(int bits) { return S::slli_epi32(data, bits); }

    IntVec operator>>(int bits) { return S::srli_epi32(data, bits); }

    IntVec& operator+=(const IntVec& other) {
        data = S::add_epi32(data, other.data);
        return *this;
    }

    IntVec& operator-=(const IntVec& other) {
        data = S::sub_epi32(data, other.data);
        return *this;
    }

    IntVec& operator*=(const IntVec& other) {
        data = S::mullo_epi32(data, other.data);
        return *this;
    }

    IntVec& operator<<=(int bits) {
        data = S::slli_epi32(data, bits);
        return *this;
    }

    IntVec& operator>>=(int bits) {
        data = S::srli_epi32(data, bits);
        return *this;
    }
};

}  // namespace compecs