# Generating Random Permutations in C++

This small header-only C++20 library provides a pseudo-random permutation generator.

The key property of a permutation is that every number in it occurs exactly once, i.e., this can be used to generate a sequence of random numbers without repetitions. It can be seeded to reproduce the same permutation multiple times at will.

Generation is done on the fly: other than the size of the universe and the random seed and a prime, nothing needs to be saved. This allows generating only a portion of a potentially very large permutation with no additional memory overhead whatsoever. Permuting a number involves only few arithmetic operations and is thus very fast. The only time consuming part (in the order of few milliseconds) is the one-time initialization that must find a suitable prime &ndash; see [how it works](#how-it-works).

This repository also comes with a handy [command line tool](#command-line-tool).

### Requirements

The library and command line tool are written in C++20, a corresponding compiler is required. Tests have been done only with GCC 11.

If you desire to use the [command line tool](#command-line-tool), you need CMake.

### License

```
MIT License

Copyright (c) 2022 Patrick Dinklage

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## How it works

The generator is based on [an article by Jeff Preshing](https://preshing.com/20121224/how-to-generate-a-sequence-of-unique-random-integers), who described how to generate random permutations of 32-bit numbers using quadratic residues of primes.

This library extends the idea to support an arbitrary universe size of up to 2^64-1. It requires finding the largest prime that (1) lies within the universe and (2) satisfies `(3 mod 4)`. The prime search is implemented rather naively in this library using a folklore algorithm, but is still reasonably fast for all possible universes.

Randomness is achieved by applying a two-phase permutation: we first permute the original number, and then permute the permuted value offset by the random seed. The outcome is reasonably well distributed.

## Usage

The library is header only, so all you need to do is make sure it's in your include path and:

```cpp
#include "random_permutation.hpp"
```

### API

The API is very straightforward. Here's an example that generates a random permutation of one hundred 16-bit numbers:

```cpp
#include <iostream>
#include "random_permutation.hpp"

int main(int argc, char** argv) {
    // print the first 100 numbers from a random permutation of [0, 2^16-1]
    auto perm = pdinklag::RandomPermutation(UINT16_MAX);
    for(unsigned i = 0; i < 100; i++) {
        std::cout << perm(x) << std::endl;
    }
    return 0;
}
```

The system's high resolution timestamp is used as the random seed by default, which can be acquired by calling `pdinklag::RandomPermutation::timestamp()`. You can also provide an explicit random seed as the second parameter to the constructor.

The `RandomPermutation` class also provides the STL-style iterators `begin` and `end`, allowing ranged for loops:

```cpp
#include <random_permutation.hpp>
#include <iostream>

int main(int argc, char** argv) {
    // print a random permutation of the numbers from 0 to 100
    auto perm = pdinklag::RandomPermutation(100);
    for(auto x : perm) std::cout << x << std::endl;
    return 0;
}
```

You can also use the `at` iterator to start or stop at a certain point.

## Command Line Tool

If you need a permutation in a file, you can use the provided command-line tool powered by the [tlx](https://tlx.github.io/) command line parser.

Build it as follows:

```sh
git clone https://github.com/pdinklag/random-permutation.git
cd random-permutation
mkdir build; cd build
cmake ..
make generate
```

You may then run:

```sh
src/generate --help
```

The output is one number per line in the standard output; process it as needed.

