#ifndef IEOMPP_HASH_HPP_
#define IEOMPP_HASH_HPP_

#include <cstddef>
#include <functional>

namespace ieompp
{
    std::size_t combine_hashes(std::size_t a, std::size_t b) {
        a ^= b + 0x9e3779b9 + (a << 6) + (a >> 2);
        return a;
    }
}

#endif
