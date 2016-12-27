#ifndef IEOMPP_MODELS_HUBBARD_REAL_SPACE_EXPECTATION_VALUE_HPP_
#define IEOMPP_MODELS_HUBBARD_REAL_SPACE_EXPECTATION_VALUE_HPP_

#include <functional>
#include <vector>

#include "ieompp/constants.hpp"
#include "ieompp/models/hubbard/dispersion.hpp"
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
                ExpectationValue1DHalfFilled(const Lattice& lattice, const Float& J,
                                             const Float& filling_factor)
                    : _lattice(lattice), _values(lattice.size() / 2 + 1, 0.)
                {
                    static const auto pi = Pi<Float>::value;
                    const auto max_dist  = lattice.size() / 2;

                    _values[0] = static_cast<Float>(filling_factor);

                    const auto fermi_momentum =
                        hubbard::calculate_fermi_momentum_1d(J, filling_factor);

#pragma omp parallel for
                    for(typename Lattice::SiteIndex dist = 1; dist <= max_dist; ++dist) {
                        _values[dist] = std::sin(fermi_momentum * dist * lattice.dx())
                                        / (pi * dist * lattice.dx());
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

            template <typename Float, typename Lattice>
            class ExpectationValue2DHalfFilled
            {
            public:
                using SiteIndex = typename Lattice::SiteIndex;

            private:
                const std::reference_wrapper<const Lattice> _lattice;
                const SiteIndex _max_dist_x, _max_dist_y;
                std::vector<Float> _values;

            public:
                ExpectationValue2DHalfFilled(const Lattice& lattice)
                    : _lattice(lattice), _max_dist_x(lattice.size_x() / 2),
                      _max_dist_y(lattice.size_y() / 2),
                      _values((_max_dist_x + 1) * (_max_dist_y + 1))
                {
                    static const auto prefactor1 = 2. / (Pi<Float>::value * Pi<Float>::value);
                    static const auto prefactor2 = 1. / Pi<Float>::value;
                    static const auto pi_half    = HalfPi<Float>::value;

#pragma omp parallel for
                    for(SiteIndex dist_x = 0; dist_x <= _max_dist_x; ++dist_x) {
                        for(SiteIndex dist_y = 0; dist_y <= _max_dist_y; ++dist_y) {
                            const auto index = (_max_dist_y + 1) * dist_x + dist_y;
                            if(dist_x == dist_y) {
                                _values[index] = 0.5;
                                continue;
                            }

                            const auto sum  = dist_x + dist_y;
                            const auto diff = (dist_x > dist_y) ? dist_x - dist_y : dist_y - dist_x;

                            _values[index] = prefactor1 * (std::sin(pi_half * sum) / sum)
                                             * (std::sin(pi_half * diff) / diff);
                        }
                    }
                }

                Float operator()(const SiteIndex& a, const SiteIndex& b) const
                {
                    const auto dist_x = _lattice.get().lattice_distance_x(a, b);
                    const auto dist_y = _lattice.get().lattice_distance_y(a, b);
                    return _values[(_max_dist_y + 1) * dist_x + dist_y];
                }
            };
        } // namespace hubbard_real_space
    }     // namespace models
} // namespace ieompp

#endif
