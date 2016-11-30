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
            public:
                using SiteIndex = typename Lattice::SiteIndex;

            private:
                std::reference_wrapper<const Lattice> _lattice;
                std::vector<Float> _values;

            public:
                ExpectationValue1DHalfFilled(const Lattice& lattice)
                    : _lattice(lattice), _values(lattice.size() / 2 + 1, 0.)
                {
                    static const auto pi      = Pi<Float>::value;
                    static const auto pi_half = HalfPi<Float>::value;
                    const auto max_dist       = lattice.size() / 2;

                    _values[0] = static_cast<Float>(0.5);

#pragma omp parallel for
                    for(typename Lattice::SiteIndex dist = 1; dist <= max_dist; ++dist) {
                        _values[dist] =
                            std::sin(pi_half * dist * lattice.dx()) / (pi * dist * lattice.dx());
                    }
                }

                Float operator()(const SiteIndex& a, const SiteIndex& b) const
                {
                    return _values[_lattice.get().lattice_distance(a, b)];
                }

                auto lattice_distance(const SiteIndex& a, const SiteIndex& b) const
                {
                    return _lattice.get().lattice_distance(a, b);
                }
            };
        } // namespace hubbard_real_space
    }     // namespace models
} // namespace ieompp

#endif
