#ifndef IEOMPP_TYPES_TRIPLET_HPP_
#define IEOMPP_TYPES_TRIPLET_HPP_

#include "ieompp/types/number.hpp"

#include <algorithm>
#include <vector>

namespace ieompp
{
    namespace types
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

            TripletList make_columns_unique()
            {
                this->sort();
                TripletList filtered;
                auto triplet = this->front();
                for(auto it = this->begin() + 1; it != this->end(); ++it) {
                    if(it->column == triplet.column) {
                        triplet.value += it->value;
                    } else {
                        if(!types::IsZero(triplet.value)) {
                            filtered.push_back(triplet);
                        }
                        triplet = *it;
                    }
                }
                filtered.push_back(triplet);
                return filtered;
            }
        };
    } // namespace types
} // namespace ieompp

#endif
