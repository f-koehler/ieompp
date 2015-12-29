#include "hubbard/abstract/algebra.hpp"

#include <algorithm>

namespace hubbard
{
    namespace algebra
    {
        template <>
        bool anticommutates<abstract::AbstractOperator>(const abstract::AbstractOperator& a,
                                                        const abstract::AbstractOperator& b)
        {
            if(a.creator == b.creator) return true;
            return false;
        }

        template <>
        abstract::AbstractPrefactor
        anticommutator<abstract::AbstractOperator, abstract::AbstractPrefactor>(
            const abstract::AbstractOperator& a, const abstract::AbstractOperator& b)
        {
            abstract::AbstractPrefactor result;
            result.prefactor.real(1.);
            result.prefactor.imag(0.);
            if(a.index != b.index) result.kroneckers.push_back({a.index.index, b.index.index});
            if(a.spin != b.spin) result.kroneckers.push_back({a.spin.spin, b.spin.spin});
            return result;
        }
    }

    namespace abstract
    {

        inline void AbstractTerm::order_kroneckers()
        {
            for(auto& k : prefactor.kroneckers) k.order_indices();
            std::sort(prefactor.kroneckers.begin(), prefactor.kroneckers.end(),
                      [](const Kronecker& k1, const Kronecker& k2) { return k1.left < k2.left; });
        }


        inline AbstractOperator make_creator(const std::string& index, const std::string& spin)
        {
            return AbstractOperator{true, AbstractIndex{index}, AbstractSpin{spin}};
        }

        inline AbstractOperator make_annihilator(const std::string& index, const std::string& spin)
        {
            return AbstractOperator{false, AbstractIndex{index}, AbstractSpin{spin}};
        }


        inline void sort_kronecker_indices(Kronecker& k)
        {
            if(k.left < k.right) return;
            std::swap(k.left, k.right);
        }


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
                    auto found = std::find_if(iter + 1, terms.end(), [&iter](const AbstractTerm& t) {
                        return t.same_operators(*iter) && t.prefactor.same_kroneckers(iter->prefactor);
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

            inline AbstractTermList& filter(AbstractTermList& terms)
            {
                terms.erase(std::remove_if(terms.begin(), terms.end(), [](const AbstractTerm& t) {
                    return t.prefactor.prefactor == 0.;
                }));
                return terms;
            }
        }

        inline AbstractTermList& simplify_terms(AbstractTermList& terms)
        {
            return simplify::filter(simplify::join(simplify::order(terms)));
        }
    }
}
