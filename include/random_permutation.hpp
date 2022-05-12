/**
 * random_permutation.hpp
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

#ifndef _RANDOM_PERMUTATION_HPP
#define _RANDOM_PERMUTATION_HPP

#include <chrono>
#include <cstdint>
#include <iterator>

namespace pdinklag {

/**
 * \brief Generates a random permutation of positive numbers from a universe of given size with near-uniform distribution
 * 
 * This is based on an article by Jeff Preshing (https://preshing.com/20121224/how-to-generate-a-sequence-of-unique-random-integers),
 * who describes how to generate random permutations of 32-bit numbers using quadratic residues of primes.
 * It has been modified to support an arbitrary universe size up to 2^64-1.
 */
class RandomPermutation {
private:
    // finds the largest prime p less than or equal to x that satisfies p = (3 mod 4)
    static uint64_t prev_prime_3mod4(uint64_t const x);

    uint64_t universe_;
    uint64_t seed_;
    uint64_t prime_;

    // permute the given number
    uint64_t permute(uint64_t const x) const;

    class Iterator {
    private:
        RandomPermutation const* perm_;
        uint64_t x_;
        bool overflow_;

    public:
        using iterator_category = std::input_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = uint64_t;
        using pointer           = uint64_t*;
        using reference         = uint64_t&;

        Iterator(RandomPermutation const& perm, uint64_t const x) : perm_(&perm), x_(x), overflow_(x_ > perm.universe_) {}
        Iterator(RandomPermutation const& perm, uint64_t const x, bool const overflow) : perm_(&perm), x_(x), overflow_(overflow) {}

        Iterator(Iterator const&) = default;
        Iterator(Iterator&&) = default;
        Iterator& operator=(Iterator const&) = default;
        Iterator& operator=(Iterator&&) = default;

        bool operator==(Iterator const&) const = default;
        bool operator!=(Iterator const&) const = default;

        inline uint64_t operator*() const { return (*perm_)(x_); }
        inline Iterator& operator++() { overflow_ = (x_ == perm_->universe_);  ++x_; return *this; }
        inline Iterator operator++(int) { Iterator copy = *this; ++*this; return copy; }
    };

public:
    /**
     * \brief Returns the current timestamp of the system's highest resolution clock
     * 
     * \return the current timestamp of the system's highest resolution clock 
     */
    static inline uint64_t timestamp() { return std::chrono::high_resolution_clock::now().time_since_epoch().count(); }

    /**
     * \brief Initializes an empty permutation that contains only zero
     */
    RandomPermutation();
    
    RandomPermutation(RandomPermutation const&) = default;
    RandomPermutation(RandomPermutation&&) = default;
    RandomPermutation& operator=(RandomPermutation const&) = default;
    RandomPermutation& operator=(RandomPermutation&&) = default;

    /**
     * \brief Initializes a permutation with a given random seed
     * 
     * \param universe the size of the universe
     * \param seed the random seed
     */
    RandomPermutation(uint64_t const universe, uint64_t const seed = timestamp());

    /**
     * \brief Computes the i-th number of the permutation
     * 
     * \param i the number to permute
     * \return the permuted number
     */
    uint64_t operator()(uint64_t const i) const;

    /**
     * \brief Returns an iterator over the entire permutation
     * 
     * \return an iterator over the entire permutation
     */
    Iterator begin() const { return Iterator(*this, 0); }

    /**
     * \brief Returns an iterator starting at the i-th number of the permutation
     * 
     * \param i the number to start from
     * \return an iterator starting at the i-th number of the permutation 
     */
    Iterator at(uint64_t i) const { return Iterator(*this, i); }

    /**
     * \brief Returns the end iterator of the permutation
     * 
     * \return the end iterator
     */
    Iterator end() const { return Iterator(*this, universe_+1, true); }
};

}

#endif
