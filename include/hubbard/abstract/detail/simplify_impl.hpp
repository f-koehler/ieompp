#include "hubbard/abstract/simplify.hpp"

namespace hubbard
{
    namespace abstract
    {
        namespace simplify
        {
            inline AbstractTermList& order(AbstractTermList& terms)
            {
                for(auto& term : terms) term.order_kroneckers();
                return terms;
            }

            inline AbstractTermList& join(AbstractTermList& terms)
            {
                for(auto iter = terms.begin(); iter != terms.end(); ++iter) {
                    auto found =
                        std::find_if(iter + 1, terms.end(), [&iter](const AbstractTerm& t) {
                            return t.same_operators(*iter)
                                   && t.prefactor.same_kroneckers(iter->prefactor);
                        });

                    while(found != terms.end()) {
                        iter->prefactor.prefactor += found->prefactor.prefactor;
                        auto prev = found - 1;
                        terms.erase(found);
                        found = std::find_if(prev + 1, terms.end(), [&iter](const AbstractTerm& t) {
                            return t.same_operators(*iter)
                                   && t.prefactor.same_kroneckers(iter->prefactor);
                        });
                    }
                }
                return terms;
            }
            
            inline AbstractTermList& eval_kroneckers(AbstractTermList& terms)
            {
                for(auto& term : terms) {
                    auto& kroneckers = term.prefactor.kroneckers;
                    kroneckers.erase(
                        std::remove_if(kroneckers.begin(), kroneckers.end(),
                                       [](const Kronecker& k) { return k.left == k.right; }),
                        kroneckers.end());
                }
                return terms;
            }

            inline AbstractTermList& filter(AbstractTermList& terms)
            {
                terms.erase(std::remove_if(terms.begin(), terms.end(), [](const AbstractTerm& t) {
                    return is_zero(t.prefactor.prefactor);
                }));
                return terms;
            }
        }

        inline AbstractTermList& simplify_terms(AbstractTermList& terms)
        {
            return simplify::filter(
                simplify::eval_kroneckers(simplify::join(simplify::order(terms))));
        }
    }
}
