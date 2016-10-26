#ifndef IEOMPP_HUBBARD_EXPECTATION_VALUE_HPP_
#define IEOMPP_HUBBARD_EXPECTATION_VALUE_HPP_

#include <functional>

#include "ieompp/constants.hpp"
#include "ieompp/models/hubbard/operator.hpp"
#include "ieompp/types/number.hpp"

namespace ieompp
{
    namespace hubbard
    {
        namespace real_space
        {
            template <typename Float, typename Operator, typename Lattice>
            struct ExpectationValue1DHalfFilled {
                std::reference_wrapper<Lattice> lattice;

                Float operator()(const Operator& a, const Operator& b) const
                {
                    static_assert(is_hubbard_operator<Operator>::value,
                                  "Operator must be of Hubbard type");
                    static_assert(Operator::number_of_indices == 2, "Operator must have 2 indices");

                    static const auto pi = Pi<Float>::value;

                    if(a.index1 == b.index1) {
                        return static_cast<Float>(0.5);
                    }

                    const auto dist = lattice.get().lattice_distance(a.index1, b.index1);
                    if(dist % 2 == 0) {
                        return 0.;
                    }
                    if(((dist - 1) / 2) % 2 == 0) {
                        return 1 / (dist * pi);
                    }
                    return -1 / (dist * pi);
                }
            };
        } // namespace real_space
    }     // namespace hubbard
} // namespace ieompp

#endif
