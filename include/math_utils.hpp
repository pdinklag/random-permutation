/**
 * math_utils.hpp
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

#ifndef _MATH_UTILS_HPP
#define _MATH_UTILS_HPP

#include <bit>
#include <cstdint>

namespace pdinklag::random_permutation_internals {

/**
 * \brief Computes 2 to the x-th power
 * 
 * \param x the exponent
 * \return 2 to the x-th power
 */
constexpr uint64_t pow2(int x) { return uint64_t(1) << x; }

/**
 * \brief Computes the integer square root of the given number (rounded down)
 * 
 * \param x the number in question
 * \return the integer square root of the given number (rounded down)
 */
constexpr uint64_t isqrt_floor(const uint64_t x) {
    if(x < 4ULL)[[unlikely]] {
        return uint64_t(x > 0);
    }
    
    uint64_t e = std::bit_width(x) >> 1;
    uint64_t r = 1ULL;

    while(e--) {
        const uint64_t cur = x >> (e << 1ULL);
        const uint64_t sm = r << 1ULL;
        const uint64_t lg = sm + 1ULL;
        r = (sm + (lg * lg <= cur));
    }
    return r;
}

/**
 * \brief Computes the integer square root of the given number (rounded up)
 * 
 * \param x the number in question
 * \return the integer square root of the given number (rounded up)
 */
constexpr uint64_t isqrt_ceil(uint64_t const x) {
    uint64_t const r = isqrt_floor(x);
    return r + (r * r < x);
}

// the first 54 prime numbers -- should fit into a cache line
constexpr uint8_t SMALL_PRIMES[] = {
    1,2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,
    103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,
    211,223,227,229,233,239,241,251
};
constexpr unsigned NUM_SMALL_PRIMES = 55;

constexpr bool is_prime(uint64_t const p) {
    if(p % 2 == 0) {
        return false;
    } else {    
        uint64_t const m = isqrt_ceil(p);
        uint64_t i;

        // first, check against small primes less than sqrt(p)
        {
            unsigned j = 2;
            i = SMALL_PRIMES[j];
            while(i <= m && j < NUM_SMALL_PRIMES - 1) {
                if((p % i) == 0) return false;
                i = SMALL_PRIMES[++j];
            }
        }

        // afterwards, use a folklore algorithm for i less than sqrt(p)
        i = 5ULL + ((i - 5ULL) / 6ULL) * 6ULL;
        while(i <= m) {
            if(((p % i) == 0) || (p % (i+2) == 0)) return false;
            i += 6ULL;
        }
        return true;
    }
}

/**
 * \brief Finds the greatest prime number less than or equal to the given number
 * 
 * \param p the number to start searching from
 * \return the greatest prime number less than or equal to the given number
 */
constexpr uint64_t prime_predecessor(uint64_t p) {
    if(p == 0)[[unlikely]] return 0;
    if(p == 2)[[unlikely]] return 2;
    if(p % 2 == 0) --p; // all primes > 2 are odd

    // linear search - the gap between two primes is hopefully very low
    // in the worst case, because there must be a prime between p and 2p, this takes p steps
    while(!is_prime(p)) p -= 2;
    return p;
}

}

#endif
