#ifndef HUBBARD_ALGEBRA_NORMAL_ORDERING_HPP_
#define HUBBARD_ALGEBRA_NORMAL_ORDERING_HPP_

#include "hubbard/algebra/term.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <tuple>
#include <iostream>

namespace hubbard
{
    namespace algebra
    {
        template <typename Term>
        struct IndexedTerm : public std::vector<std::tuple<std::size_t, typename Term::Operator>> {
            IndexedTerm(const Term& t)
            {
                for(std::size_t i = 0; i < t.operators.size(); ++i)
                    this->push_back(std::make_tuple(i, t.operators[i]));
            }
        };

        template <typename Term>
        std::ostream& operator<<(std::ostream& strm, const IndexedTerm<Term>& terms)
        {
            for(auto& entry : terms) strm << std::get<1>(entry) << ' ';
            return strm;
        }

        template <typename Term>
        void normal_order(
            const Term& t/*, TermList<Term>& result,
            std::function<typename Term::Prefactor(const typename Term::Operator::Contraction&)> contractor*/)
        {
            IndexedTerm<Term> term(t);
            const auto size = term.size();
            const auto max_contractions = size / 2;

            if(max_contractions == 0) return;

            bool sign = true;
            do {
                for(auto num_contractions = 1; num_contractions <= max_contractions;
                    ++num_contractions) {
                }

                sign = !sign;
            } while(std::next_permutation(term.begin(), term.end(),
                                          [](const typename IndexedTerm<Term>::value_type& a,
                                             const typename IndexedTerm<Term>::value_type& b) {
                                              return std::get<0>(a) < std::get<0>(b);
                                          }));
        }

        template <typename Contraction>
        inline bool contraction_vanishes(const Contraction& contraction)
        {
            return (std::get<0>(contraction).creator == std::get<1>(contraction).creator)
                   || (std::get<0>(contraction).spin != std::get<1>(contraction).spin);
        }

        template <typename RealSpace, typename FourierSpace, typename Hamiltonian,
                  typename Contraction>
        std::complex<typename RealSpace::Real>
        contract_real_space(const RealSpace& rs, const FourierSpace& fs, const Hamiltonian& H,
                            const Contraction& contraction)
        {
            const auto i       = std::complex<typename RealSpace::Real>(0., 1.);
            const auto delta_r = rs(std::get<1>(contraction)) - rs(std::get<0>(contraction));
            auto sum = std::complex<typename RealSpace::Real>(0., 0.);
            for(auto& k_index : fs.indices) {
                auto& k = fs(k_index);
                if(H.dispersion(k) >= 0.) continue;
                sum += std::exp(i * RealSpace::dot_product(k, delta_r));
            }
            return sum / rs.indices.size();
        }
    }
}

#endif
