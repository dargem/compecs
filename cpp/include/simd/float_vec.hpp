#pragma once

#include <cassert>
#include <cstdint>

#include "simd/traits.hpp"

namespace compecs {

using S = simd_traits::Set;

// This is effectively a wrapper around our SIMD Set trait for an int vector register
class alignas(S::bytes) FloatVec {
   public:
    static constexpr size_t ELEMENTS{S::bytes / sizeof(uint32_t)};
    static constexpr size_t BYTES{S::bytes};

    S::__mi data;
    UInt32Vec(uint32_t* mem_addr) : data(S::load_si(reinterpret_cast<S::__mi*>(mem_addr))) {
        assert(reinterpret_cast<uintptr_t>(mem_addr) % BYTES == 0);
    }

    UInt32Vec(S::__mi d) : data(d) {};

    void store(uint32_t* mem_addr) { S::store_si(reinterpret_cast<S::__mi*>(mem_addr), data); }

    friend UInt32Vec operator+(const UInt32Vec& a, const UInt32Vec& b) {
        return S::add_epi32(a.data, b.data);
    }

    friend UInt32Vec operator-(const UInt32Vec& a, const UInt32Vec& b) {
        return S::sub_epi32(a.data, b.data);
    }

    // This is a mullo multiply so will trim overflow
    friend UInt32Vec operator*(const UInt32Vec& a, const UInt32Vec& b) {
        return S::mullo_epi32(a.data, b.data);
    }

    UInt32Vec operator<<(int bits) { return S::slli_epi32(data, bits); }

    UInt32Vec operator>>(int bits) { return S::srli_epi32(data, bits); }

    UInt32Vec& operator+=(const UInt32Vec& other) {
        data = S::add_epi32(data, other.data);
        return *this;
    }

    UInt32Vec& operator-=(const UInt32Vec& other) {
        data = S::sub_epi32(data, other.data);
        return *this;
    }

    UInt32Vec& operator*=(const UInt32Vec& other) {
        data = S::mullo_epi32(data, other.data);
        return *this;
    }

    UInt32Vec& operator<<=(int bits) {
        data = S::slli_epi32(data, bits);
        return *this;
    }

    UInt32Vec& operator>>=(int bits) {
        data = S::srli_epi32(data, bits);
        return *this;
    }
};

}  // namespace compecs