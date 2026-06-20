#pragma once

#include <cassert>
#include <cstdint>

#include "simd/traits.hpp"

namespace compecs {

using S = simd_traits::Set;

// This is effectively a wrapper around our SIMD Set trait for an int vector register
class alignas(S::bytes) FloatVec {
   public:
    static constexpr size_t ELEMENTS{S::bytes / sizeof(float)};
    static constexpr size_t BYTES{S::bytes};

    S::__m data;
    FloatVec(float* mem_addr) : data(S::load_ps(mem_addr)) {
        assert(reinterpret_cast<uintptr_t>(mem_addr) % BYTES == 0);
    }

    FloatVec(S::__mi d) : data(d) {};

    void store(uint32_t* mem_addr) { S::store_si(reinterpret_cast<S::__mi*>(mem_addr), data); }

    friend FloatVec operator+(const FloatVec& a, const FloatVec& b) {
        return S::add_epi32(a.data, b.data);
    }

    friend FloatVec operator-(const FloatVec& a, const FloatVec& b) {
        return S::sub_epi32(a.data, b.data);
    }

    // This is a mullo multiply so will trim overflow
    friend FloatVec operator*(const FloatVec& a, const FloatVec& b) {
        return S::mullo_epi32(a.data, b.data);
    }

    FloatVec operator<<(int bits) { return S::slli_epi32(data, bits); }

    FloatVec operator>>(int bits) { return S::srli_epi32(data, bits); }

    FloatVec& operator+=(const FloatVec& other) {
        data = S::add_epi32(data, other.data);
        return *this;
    }

    FloatVec& operator-=(const FloatVec& other) {
        data = S::sub_epi32(data, other.data);
        return *this;
    }

    FloatVec& operator*=(const FloatVec& other) {
        data = S::mullo_epi32(data, other.data);
        return *this;
    }

    FloatVec& operator<<=(int bits) {
        data = S::slli_epi32(data, bits);
        return *this;
    }

    FloatVec& operator>>=(int bits) {
        data = S::srli_epi32(data, bits);
        return *this;
    }
};

}  // namespace compecs