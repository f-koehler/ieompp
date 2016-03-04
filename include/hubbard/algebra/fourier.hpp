#ifndef HUBBARD_ALGEBRA_FOURIER_HPP_
#define HUBBARD_ALGEBRA_FOURIER_HPP_

#include <typeinfo>
#include "hubbard/algebra/term.hpp"

namespace hubbard
{
    namespace algebra
    {
        template <typename TermT, typename TermFourierT, typename Discretization,
                  typename DiscretizationFourier>
        struct Transformation {
            using Operator        = typename TermT::Operator;
            using OperatorFourier = typename TermFourierT::Operator;
            using Term            = TermT;
            using TermFourier     = TermFourierT;

            const Discretization disc_a;
            const DiscretizationFourier disc_b;

            Transformation(const Discretization& _disc_a, const DiscretizationFourier& _disc_b)
                : disc_a(_disc_a), disc_b(_disc_b)
            {
            }

            void transform(const Term& term, TermList<TermFourier>& result)
            {
                assert(term.operators.size() == 1);
                const auto& op      = term.operators.front();
                const auto exponent = op.creator ? typename TermFourier::Prefactor(0., -1.)
                                                 : typename TermFourier::Prefactor(0., 1.);
                for(const auto& idx : disc_a.indices) {
                    auto factor =
                        term.prefactor * std::exp(exponent * disc_a(op.index) * disc_b(idx));
                    result.push_back(TermFourier{factor, {OperatorFourier{op.creator, idx, op.spin}}});
                }
            }
        };
    }
}

#endif
