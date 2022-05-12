/**
 * random_permutation.cpp
 * part of pdinklag/random_permutation
 * 
 * MIT License
 * 
 * Copyright (c) 2022 Patrick Dinklage
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <random_permutation.hpp>
#include "prime_utils.hpp"

using namespace pdinklag;

// some common universe sizes and the corresponding primes that satisfy 3mod4
struct CommonUniverse { uint64_t universe, prime; };

constexpr uint64_t pow2(int x) { return uint64_t(1) << x; }
CommonUniverse common_universes[] = {
    { pow2(16) - 2, pow2(16) - 17 },
    { pow2(16) - 1, pow2(16) - 17 },
    { pow2(24) - 2, pow2(24) - 17 },
    { pow2(24) - 1, pow2(24) - 17 },
    { pow2(32) - 2, pow2(32) - 5 },
    { pow2(32) - 1, pow2(32) - 5 },
    { pow2(40) - 2, pow2(40) - 213 },
    { pow2(40) - 1, pow2(40) - 213 },
    { pow2(48) - 2, pow2(48) - 65 },
    { pow2(48) - 1, pow2(48) - 65 },
    { pow2(56) - 2, pow2(56) - 5 },
    { pow2(56) - 1, pow2(56) - 5 },
    { pow2(63) - 2, pow2(63) - 25 },
    { pow2(63) - 1, pow2(63) - 25 },
    { UINT64_MAX-1, 0xFFFFFFFFFFFFFF43ULL },
    { UINT64_MAX,   0xFFFFFFFFFFFFFF43ULL },
    { 0, 0 }
};

// provides a decent distribution of 64 bits
constexpr uint64_t SHUFFLE1 = 0x9696594B6A5936B2ULL;
constexpr uint64_t SHUFFLE2 = 0xD2165B4B66592AD6ULL;

uint64_t RandomPermutation::prev_prime_3mod4(uint64_t const universe) {
    // test if universe is common
    for(unsigned i = 0; common_universes[i].prime > 0; i++) {
        if(universe == common_universes[i].universe) {
            return common_universes[i].prime;
        }
    }
    
    // otherwise, do it the hard way
    uint64_t p = prime_predecessor(universe);
    while((p & 3ULL) != 3ULL) {
        p = prime_predecessor(p - 1);
    }
    return p;
}

uint64_t RandomPermutation::permute(uint64_t const x) const {
    if(x >= prime_) {
        // map numbers in gap to themselves - shuffling will take care of this
        return x;
    } else {
        // use quadratic residue
        const uint64_t r = ((__uint128_t)x * (__uint128_t)x) % (__uint128_t)prime_;
        return (x <= (prime_ >> 1ULL)) ? r : prime_ - r;
    }
}

RandomPermutation::RandomPermutation() : universe_(1), seed_(0), prime_(0) {
}

RandomPermutation::RandomPermutation(const uint64_t universe, const uint64_t seed)
    : universe_(universe),
      prime_(prev_prime_3mod4(universe)),
      seed_((seed ^ SHUFFLE1) ^ SHUFFLE2) {
}

uint64_t RandomPermutation::operator()(uint64_t const i) const {
    return permute((seed_ + permute(i)) % universe_);
}
