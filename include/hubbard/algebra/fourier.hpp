#ifndef HUBBARD_ALGEBRA_FOURIER_HPP_
#define HUBBARD_ALGEBRA_FOURIER_HPP_

#include <typeinfo>
#include "hubbard/algebra/term.hpp"

namespace hubbard
{
    namespace algebra
    {
        template <typename TermA, typename TermB, typename DiscretizationA,
                  typename DiscretizationB>
        struct Transformation {
            using OperatorA = typename TermA::OperatorType;
            using OperatorB = typename TermB::OperatorType;
            using TermAType = TermA;
            using TermBType = TermB;

            const DiscretizationA disc_a;
            const DiscretizationB disc_b;

            Transformation(const DiscretizationA& _disc_a, const DiscretizationB& _disc_b)
                : disc_a(_disc_a), disc_b(_disc_b)
            {
            }

            void transform(const TermA& term, TermList<TermB>& result)
            {
                assert(term.operators.size() == 1);
                const auto& op      = term.operators.front();
                const auto exponent = op.creator ? typename TermB::PrefactorType(0., -1.)
                                                 : typename TermB::PrefactorType(0., 1.);
                for(const auto& idx : disc_a.indices) {
                    auto factor =
                        term.prefactor * std::exp(exponent * disc_a(op.index) * disc_b(idx));
                    result.push_back(TermB{factor, {OperatorB{op.creator, idx, op.spin}}});
                }
            }
        };
    }
}

#endif
