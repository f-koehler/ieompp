#ifndef IEOMPP_ALGEBRA_TERM_CHECKS_HPP_
#define IEOMPP_ALGEBRA_TERM_CHECKS_HPP_

#include <cstddef>
#include <type_traits>

#include "ieompp/algebra/operator/get_index.hpp"
#include "ieompp/algebra/operator/operator.hpp"

namespace ieompp
{
    namespace algebra
    {
        template <std::size_t Index, typename Term, typename Discretization>
        typename Discretization::Vector total_momentum(const Term& t,
                                                       const Discretization& momentum_space)
        {
            auto momentum = momentum_space.lattice_vectors()[0];
            momentum.setZero();
            for(auto& op : t.operators) {
                if(op.creator) {
                    momentum += momentum_space[get_index<Index>(op)];
                } else {
                    momentum -= momentum_space[get_index<Index>(op)];
                }
            }
            return momentum;
        }

        template <std::size_t Index, typename Term>
        int total_spin(const Term& t)
        {
            static_assert(
                std::is_same<typename IndexType<Index, typename Term::Operator>::type, bool>::value,
                "spin index must be a boolean");

            int spin = 0;
            for(auto& op : t.operators) {
                if(op.creator) {
                    spin += get_index<Index>(op) ? 1 : -1;
                } else {
                    spin -= get_index<Index>(op) ? 1 : -1;
                }
            }
            return spin;
        }

        template <typename Term>
        int total_creations(const Term& t)
        {
            int creations = 0;
            for(auto& op : t.operators) {
                if(op.creator) {
                    ++creations;
                } else {
                    --creations;
                }
            }
            return creations;
        }
    } // namespace algebra
} // namespace ieompp

#endif
