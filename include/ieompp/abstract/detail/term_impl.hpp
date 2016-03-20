#include "ieompp/abstract/term.hpp"

#include <algorithm>

namespace ieompp
{
    namespace abstract
    {
        inline void AbstractTerm::order_kroneckers()
        {
            for(auto& k : prefactor.kroneckers) k.order_indices();
            std::sort(prefactor.kroneckers.begin(), prefactor.kroneckers.end(),
                      [](const Kronecker& k1, const Kronecker& k2) { return k1.left < k2.left; });
        }

        inline AbstractTerm make_term(const AbstractPrefactor& prefactor,
                                      const std::initializer_list<AbstractOperator>& operators)
        {
            AbstractTerm term;
            term.prefactor = prefactor;
            term.operators = operators;
            return term;
        }


        inline void sort_kronecker_indices(Kronecker& k)
        {
            if(k.left < k.right) return;
            std::swap(k.left, k.right);
        }
    }
}
