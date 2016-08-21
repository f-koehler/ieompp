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
        template <typename Term>
        using Basis = std::set<Term, algebra::TermSmaller<Term>>;

        template <typename Term, typename Generator>
        void create_basis(const Term& initial_term, Basis<Term>& basis, const Generator& generator,
                          std::size_t num_iterations)
        {
            basis.clear();
            basis.insert(initial_term);

            std::vector<Term> todo;
            todo.push_back(initial_term);

            for(std::size_t i = 1; i <= num_iterations; ++i) {
                std::vector<Term> new_terms;
                for(auto& t : todo) generator(t, new_terms);
                todo.clear();
                for(auto& t : new_terms) {
                    auto ins = basis.insert(t);
                    if(ins.second) todo.push_back(t);
                }
            }
        }
    }
}

#endif
