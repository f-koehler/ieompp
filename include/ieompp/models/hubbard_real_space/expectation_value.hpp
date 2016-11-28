#ifndef IEOMPP_MODELS_HUBBARD_REAL_SPACE_EXPECTATION_VALUE_HPP_
#define IEOMPP_MODELS_HUBBARD_REAL_SPACE_EXPECTATION_VALUE_HPP_

#include <functional>
#include <vector>

#include "ieompp/constants.hpp"
#include "ieompp/models/hubbard_common/operator_traits.hpp"
#include "ieompp/models/hubbard_real_space/basis.hpp"
#include "ieompp/types/number.hpp"

namespace ieompp
{
    namespace models
    {
        namespace hubbard_real_space
        {
            template <typename Float, typename Lattice>
            class ExpectationValue1DHalfFilled
            {
            private:
                std::reference_wrapper<const Lattice> _lattice;
                std::vector<Float> _values;

            public:
                ExpectationValue1DHalfFilled(const Lattice& lattice)
                    : _lattice(lattice), _values(lattice.size() / 2 + 1, 0.)
                {
                    static const auto pi = Pi<Float>::value;
                    const auto max_dist  = lattice.size() / 2;

                    _values[0] = static_cast<Float>(0.5);

#pragma omp parallel for
                    for(typename Lattice::SiteIndex dist = 1; dist <= max_dist; ++dist) {
                        if(dist % 2 == 0) {
                            continue;
                        }
                        if(((dist - 1) / 2) % 2 == 0) {
                            _values[dist] = 1 / (dist * pi);
                        }
                        _values[dist] = -1 / (dist * pi);
                    }
                }

                template <typename Operator>
                Float operator()(const Operator& a, const Operator& b) const
                {
                    static_assert(hubbard_common::IsHubbardOperator<Operator>::value,
                                  "Operator must be of Hubbard type");
                    static_assert(Operator::number_of_indices == 2, "Operator must have 2 indices");

                    return _values[_lattice.get().lattice_distance(a.index1, b.index1)];
                }
            };
        } // namespace hubbard_real_space
    }     // namespace models
} // namespace ieompp

#endif
