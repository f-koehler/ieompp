#ifndef IEOMPP_ALGEBRA_TERM_CHECKS_HPP_
#define IEOMPP_ALGEBRA_TERM_CHECKS_HPP_

#include <cstddef>
#include <type_traits>

#include "ieompp/algebra/operator.hpp"

namespace ieompp
{
    namespace algebra
    {
        template <typename Term, std::size_t Index, typename Discretization>
        typename Discretization::Vector total_momentum(const Term& t,
                                                       const Discretization& momentum_space)
        {
            typename Discretization::Vector momentum = 0.;
            for(auto& op : t.operators) {
                if(op.creator)
                    momentum += total_momentum(momentum_space[get_index<Index>(op)]);
                else
                    momentum -= total_momentum(momentum_space[get_index<Index>(op)]);
            }
            return momentum;
        }

        template <std::size_t Index, typename Term>
        int total_spin(const Term& t)
        {
            static_assert(std::is_same<typename index_type<Index, typename Term::Operator>::type,
                                       bool>::value,
                          "spin index must be a boolean");

            int spin = 0;
            for(auto& op : t.operators) {
                if(op.creator)
                    spin += get_index<Index>(op) ? 1 : -1;
                else
                    spin -= get_index<Index>(op) ? 1 : -1;
            }
            return spin;
        }

        template <typename Term>
        int total_creations(const Term& t)
        {
            int creations = 0;
            for(auto& op : t.operators) {
                if(op.creator)
                    ++creations;
                else
                    --creations;
            }
            return creations;
        }
    }
}

#endif
