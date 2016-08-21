#ifndef IEOMPP_IEOM_BASIS_HPP_
#define IEOMPP_IEOM_BASIS_HPP_

#include <algorithm>
#include <functional>
#include <set>
#include <vector>

#include <ieompp/algebra/term_comparison.hpp>

namespace ieompp
{
    namespace ieom
    {
        template <typename TermT>
        struct Basis : public std::vector<TermT>
        {
            using Term = TermT;

            typename std::vector<Term>::const_iterator find(const Term& t) const
            {
                return std::find_if(this->cbegin(), this->cend(),
                                    [&t](const Term& a) { return a.same_operators(t); });
            }

            typename std::vector<Term>::iterator find(const Term& t)
            {
                return std::find_if(this->begin(), this->end(),
                                    [&t](const Term& a) { return a.same_operators(t); });
            }
        };

        template <typename Term, typename Generator>
        Basis<Term> make_basis(const Term& initial_term, const Generator& generator,
                               std::size_t num_iterations)
        {
            Basis<Term> basis;
            std::set<Term, algebra::TermSmaller<Term>> term_set;

            std::vector<Term> todo;
            todo.push_back(initial_term);

            for(std::size_t i = 1; i <= num_iterations - 1; ++i) {
                std::vector<Term> new_terms;
                for(const auto& t : todo) generator(t, new_terms);
                todo.clear();
                for(const auto& t : new_terms) {
                    auto ins = term_set.insert(t);
                    if(ins.second) todo.push_back(t);
                }
            }

            std::copy(term_set.begin(), term_set.end(), std::back_inserter(basis));
            return basis;
        }
    }
}

#endif
