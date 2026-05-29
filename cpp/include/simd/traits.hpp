#pragma once

#include <bit>
#include <cstddef>
#include <cstdint>

enum class InstructionSet {
    NONE,
    AVX128,  // AKA AVX
    AVX256,  // AKA AVX2
    AVX512
};

template <InstructionSet I>
struct InstructionSetTraits;

template <>
struct InstructionSetTraits<InstructionSet::NONE> {
    // Here we're using just normal registers which are 64 bits yes, but
    // we're just using this for 4 byte data so we treat them as 32 bit for this.
    // SWAR could be used but thats outside this projects scope.
    static constexpr size_t bits = 32;
    static constexpr size_t bytes = bits / 8;

    using __mi = uint32_t;
    using __m = float;

    // Integer ops
    template <int bit_shift>
    static __mi rol_epi32(__mi a) {
        return (a << bit_shift) | (a >> (32 - bit_shift));
    }

    template <int bit_shift>
    static __mi rol_epi64(__mi a) {
        return rol_epi32<bit_shift>(a);  // We just fall back to 32 bit
    }

    static __mi srli_epi32(__mi a, int bits) { return a >> bits; }
    static __mi slli_epi32(__mi a, int bits) { return a << bits; }
    static __mi set1_epi32(int val) { return val; }
    static __mi mullo_epi32(__mi a, __mi b) { return a * b; }

    // Scalar fallback, treat "epi64" ops as operating on the scalar value
    static __mi slli_epi64(__mi a, int bits) { return slli_epi32(a, bits); }

    // Bit ops
    static __mi xor_si(__mi a, __mi b) { return a ^ b; }
    static __mi or_si(__mi a, __mi b) { return a | b; }
    static void store_si(__mi* mem_addr, __mi source) { *mem_addr = source; }
    static __mi load_si(__mi const* mem_addr) { return *mem_addr; }

    // Float ops
    static __m sub_ps(__m a, __m b) { return a - b; }
    static __m set1_ps(float val) { return val; }
    static __m castsi_ps(__mi a) { return std::bit_cast<__m>(a); }
    static void store_ps(float* mem_addr, __m source) { *mem_addr = source; }
};

#ifdef __AVX__
template <>
struct InstructionSetTraits<InstructionSet::AVX128> {
    static constexpr size_t bits = 128;
    static constexpr size_t bytes = bits / 8;

    using __mi = __m128i;
    using __m = __m128;

    // Integer ops
    template <int bit_shift>
    static __mi rol_epi32(__mi a) {
#if defined(__AVX512F__)
        // this 128 bit register instruction is actually from the AVX512 instruction set
        return _mm_rol_epi32(a, bit_shift);
#else
        // if we do not have the AVX512 instruction set default back to our 128 "emulation"
        return or_si(slli_epi32(a, bit_shift), srli_epi32(a, 32 - bit_shift));
#endif
    }

    template <int bit_shift>
    static __mi rol_epi64(__mi a) {
#if defined(__AVX512F__)
        // this 128 bit register instruction is actually from the AVX512 instruction set
        return _mm_rol_epi64(a, bit_shift);
#else
        // if we do not have the AVX512 instruction set default back to our 128 "emulation"
        return or_si(slli_epi64(a, bit_shift), srli_epi64(a, 64 - bit_shift));
#endif
    }

    static __mi srli_epi32(__mi a, int bits) { return _mm_srli_epi32(a, bits); }
    static __mi slli_epi32(__mi a, int bits) { return _mm_slli_epi32(a, bits); }
    static __mi set1_epi32(int val) { return _mm_set1_epi32(val); }
    static __mi mullo_epi32(__mi a, __mi b) { return _mm_mullo_epi32(a, b); }
    static __mi srli_epi64(__mi a, int bits) { return _mm_srli_epi64(a, bits); }
    static __mi slli_epi64(__mi a, int bits) { return _mm_slli_epi64(a, bits); }
    static __mi add_epi64(__mi a, __mi b) { return _mm_add_epi64(a, b); }
    static __mi set1_epi64(uint64_t a) { return _mm_set1_epi64x(a); }

    // Bit ops
    static __mi xor_si(__mi a, __mi b) { return _mm_xor_si128(a, b); }
    static __mi or_si(__mi a, __mi b) { return _mm_or_si128(a, b); }
    static void store_si(__mi* mem_addr, __mi source) { _mm_store_si128(mem_addr, source); }
    static __mi load_si(__mi const* mem_addr) { return _mm_load_si128(mem_addr); }

    // Float ops
    static __m sub_ps(__m a, __m b) { return _mm_sub_ps(a, b); }
    static __m set1_ps(float val) { return _mm_set1_ps(val); }
    static __m castsi_ps(__mi a) { return _mm_castsi128_ps(a); }
    static void store_ps(float* mem_addr, __m source) { _mm_store_ps(mem_addr, source); }
    // static void store_si(__mi* mem_addr, __mi source) { _mm_store_si128(mem_addr, source); }
};
#endif

#ifdef __AVX2__
template <>
struct InstructionSetTraits<InstructionSet::AVX256> {
    static constexpr size_t bits = 256;
    static constexpr size_t bytes = bits / 8;

    using __mi = __m256i;
    using __m = __m256;

    // Integer ops
    template <int bit_shift>
    static __mi rol_epi32(__mi a) {
#if defined(__AVX512F__)
        // this 256 bit register instruction is actually from the AVX512 instruction set
        return _mm256_rol_epi32(a, bit_shift);
#else
        // if we do not have the AVX512 instruction set default back to our 256 "emulation"
        return or_si(slli_epi32(a, bit_shift), srli_epi32(a, 32 - bit_shift));
#endif
    }

    template <int bit_shift>
    static __mi rol_epi64(__mi a) {
#if defined(__AVX512F__)
        // this 256 bit register instruction is actually from the AVX512 instruction set
        return _mm256_rol_epi64(a, bit_shift);
#else
        // if we do not have the AVX512 instruction set default back to our 256 "emulation"
        return or_si(slli_epi64(a, bit_shift), srli_epi64(a, 64 - bit_shift));
#endif
    }

    static __mi srli_epi32(__mi a, int bits) { return _mm256_srli_epi32(a, bits); }
    static __mi slli_epi32(__mi a, int bits) { return _mm256_slli_epi32(a, bits); }
    static __mi set1_epi32(int val) { return _mm256_set1_epi32(val); }
    static __mi mullo_epi32(__mi a, __mi b) { return _mm256_mullo_epi32(a, b); }
    static __mi srli_epi64(__mi a, int bits) { return _mm256_srli_epi64(a, bits); }
    static __mi slli_epi64(__mi a, int bits) { return _mm256_slli_epi64(a, bits); }
    static __mi add_epi64(__mi a, __mi b) { return _mm256_add_epi64(a, b); }
    static __mi set1_epi64(uint64_t a) { return _mm256_set1_epi64x(a); }

    // Bit ops
    static __mi xor_si(__mi a, __mi b) { return _mm256_xor_si256(a, b); }
    static __mi or_si(__mi a, __mi b) { return _mm256_or_si256(a, b); }
    static void store_si(__mi* mem_addr, __mi source) { _mm256_store_si256(mem_addr, source); }
    static __mi load_si(__mi const* mem_addr) { return _mm256_load_si256(mem_addr); }

    // Float ops
    static __m sub_ps(__m a, __m b) { return _mm256_sub_ps(a, b); }
    static __m set1_ps(float val) { return _mm256_set1_ps(val); }
    static __m castsi_ps(__mi a) { return _mm256_castsi256_ps(a); }
    static void store_ps(float* mem_addr, __m source) { _mm256_store_ps(mem_addr, source); }
};
#endif
