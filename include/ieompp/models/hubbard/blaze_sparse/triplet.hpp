#ifndef IEOMPP_HUBBARD_BLAZE_SPARSE_HPP_
#define IEOMPP_HUBBARD_BLAZE_SPARSE_HPP_

#include <algorithm>
#include <vector>

namespace ieompp
{
    namespace hubbard
    {
        template <typename ScalarT, typename IndexT>
        struct Triplet {
            using Scalar = ScalarT;
            using Index  = IndexT;

            Index row, column;
            Scalar value;

            Triplet(Index i, Index j, Scalar val) : row(i), column(j), value(std::move(val)) {}
        };

        template <typename Scalar, typename Index>
        struct TripletList : public std::vector<Triplet<Scalar, Index>> {
            void sort()
            {
                std::sort(this->begin(), this->end(),
                          [](const Triplet<Scalar, Index>& a, const Triplet<Scalar, Index>& b) {
                              return a.column < b.column;
                          });
            }
        };
    } // namespace hubbard
} // namespace ieompp

#endif
