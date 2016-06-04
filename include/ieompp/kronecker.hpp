#ifndef KRONECKER_HPP_
#define KRONECKER_HPP_

namespace ieompp
{
    template <typename IndexT>
    struct KroneckerDelta
    {
        using Index = IndexT;
        Index a, b;

        bool operator()() const { return a == b; }
    };
}

#endif
