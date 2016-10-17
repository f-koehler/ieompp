#ifndef IEOMPP_HUBBARD_EXPECTATION_VALUES_HPP_
#define IEOMPP_HUBBARD_EXPECTATION_VALUES_HPP_

#include <array>
#include <cmath>

#include <ieompp/constants.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/models/hubbard_explicit/basis.hpp>
#include <ieompp/types/matrix.hpp>
#include <ieompp/types/number.hpp>

namespace ieompp
{
    namespace hubbard
    {
        namespace real_space
        {
            template <typename Lattice>
            class SiteOccupation
            {
                using Index = typename Lattice::Index;
                using Float = typename Lattice::Float;

            private:
                const Index _index, _index3;
                const Index _left, _right;

            public:
                template <typename Term>
                SiteOccupation(const Index& index, const Basis3Operator<Term>& basis,
                               const Lattice& lattice)
                    : _index(index), _index3(basis.get_3op_index(index, index, index)),
                      _left(lattice.neighbors(index)[0]), _right(lattice.neighbors(index)[1])
                {
                }

                template <typename Vector>
                typename types::scalar_type<Vector>::type operator()(const Vector& vec) const
                {
                    static const Float pi = Pi<Float>::value;

                    const auto h0        = vec[_index] + vec[_index3];
                    const auto h1        = vec[_left];
                    const auto h2        = vec[_right];
                    const auto h000      = vec[_index3];
                    const auto h0_conj   = types::conjugate(h0);
                    const auto h1_conj   = types::conjugate(h1);
                    const auto h2_conj   = types::conjugate(h2);
                    const auto h000_conj = types::conjugate(h000);

                    typename types::scalar_type<Vector>::type result = 0.;
                    result += (std::norm(h0) + std::norm(h1) + std::norm(h2)) / 2;
                    result += (h0 * h1_conj + h0 * h2_conj + h1 * h0_conj + h2 * h0_conj) / pi;
                    result += (h0 * h000_conj + h000 * h0_conj + 2 * std::norm(h000)) / 2.;
                    result += (h1 * h000_conj + h2 * h000_conj + h000 * h1_conj + h000 * h2_conj) / pi;
                    return result;
                }
            };

            template <typename Lattice>
            class SiteOccupation1Op
            {
                using Index = typename Lattice::Index;
                using Float = typename Lattice::Float;

            private:
                const Index _index, _left, _right;

            public:
                SiteOccupation1Op(const Index& index, const Lattice& lattice)
                    : _index(index), _left(lattice.neighbors(index)[0]),
                      _right(lattice.neighbors(index)[1])
                {
                }

                template <typename Vector>
                typename types::scalar_type<Vector>::type operator()(const Vector& vec) const
                {
                    static const Float pi = Pi<Float>::value;

                    const auto h0        = vec[_index];
                    const auto h1        = vec[_left];
                    const auto h2        = vec[_right];
                    const auto h0_conj   = types::conjugate(h0);
                    const auto h1_conj   = types::conjugate(h1);
                    const auto h2_conj   = types::conjugate(h2);

                    typename types::scalar_type<Vector>::type result = 0.;
                    result += (std::norm(h0) + std::norm(h1) + std::norm(h2)) / 2;
                    result += (h0 * h1_conj + h0 * h2_conj + h1 * h0_conj + h2 * h0_conj) / pi;
                    return result;
                }
            };
        }
    }
}


#endif
