/**
 * generate.cpp
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

#include <cassert>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <vector>

#include <random_permutation.hpp>

#include <tlx/cmdline_parser.hpp>

int main(int argc, char** argv) {
    uint64_t u = 0xFFFFFFFFULL;
    uint64_t seed = pdinklag::RandomPermutation::timestamp();
    uint64_t num = 10ULL;
    bool check = false;

    tlx::CmdlineParser cp;
    cp.set_description("Generates a random permutation of a universe and prints it to the standard output.");
    cp.set_author("Patrick Dinklage");
    cp.add_bytes('n', "num", num, "The number of numbers to generate (default: 10).");
    cp.add_bytes('u', "universe", u, "The universe to draw numbers from (default: 32-bit numbers).");
    cp.add_size_t('s', "seed", seed, "The random seed (default: high-res timestamp).");
#ifndef NDEBUG
    cp.add_flag('c', "check", check, "Check that a permutation is generated (debug).");
#endif

    if(!cp.process(argc, argv)) {
        return -1;
    }

    if(u < num) {
        std::cerr << "the universe must be at least as large as the number of generated numbers" << std::endl;
        return -1;
    }

    // generate numbers
    auto perm = pdinklag::RandomPermutation(u, seed);

#ifndef NDEBUG
    if(check) {
        std::vector<bool> b(u);
        for(uint64_t i = 0; i < u; i++) {
            const uint64_t j = perm(i);
            assert(!b[j]);
            b[j] = 1;
        }
    }
#endif

    for(uint64_t i = 0; i < num; i++) {
        std::cout << perm(i) << std::endl;
    }
    return 0;
}
