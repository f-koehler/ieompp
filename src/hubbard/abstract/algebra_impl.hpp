#include "hubbard/abstract/algebra.hpp"

#include <algorithm>

inline void AbstractTerm::order_kroneckers() {
    for(auto& k : prefactor.kroneckers) k.order_indices();
    std::sort(prefactor.kroneckers.begin(), prefactor.kroneckers.end(),
              [](const Kronecker& k1, const Kronecker& k2) { return k1.left < k2.left; });
}

template<>
bool anticommutates<AbstractOperator>(const AbstractOperator& a, const AbstractOperator& b) {
    if(a.creator == b.creator) return true;
    return false;
}

template<>
AbstractPrefactor anticommutator<AbstractOperator, AbstractPrefactor>(const AbstractOperator& a, const AbstractOperator& b) {
    AbstractPrefactor result;
    result.prefactor.real(1.);
    result.prefactor.imag(0.);
    if(a.index != b.index)
        result.kroneckers.push_back({ a.index.index, b.index.index });
    if(a.spin != b.spin)
        result.kroneckers.push_back({ a.spin.spin, b.spin.spin });
    return result;
}


inline AbstractOperator make_abstract_creator(const std::string& index, const std::string& spin) {
    return AbstractOperator{ true, AbstractIndex{ index }, AbstractSpin{ spin } };
}

inline AbstractOperator make_abstract_annihilator(const std::string& index, const std::string& spin) {
    return AbstractOperator{ false, AbstractIndex{ index }, AbstractSpin{ spin } };
}


inline void sort_kronecker_indices(Kronecker& k) {
    if(k.left < k.right) return;
    std::swap(k.left, k.right);
}


AbstractTermList& simplify(AbstractTermList& terms) {
    for(auto& term : terms) term.order_kroneckers();

    for(auto iter = terms.begin(); iter != terms.end(); ++iter) {
        auto found = std::find_if(iter + 1, terms.end(), [&iter](const AbstractTerm& t) {
            return t.same_operators(*iter);
        });
        while(found != terms.end()) {
            auto& kroneckers1 = iter->prefactor.kroneckers;
            auto& kroneckers2 = found->prefactor.kroneckers;
            if(kroneckers1.size() != kroneckers2.size()) {
                found = std::find_if(found + 1, terms.end(), [&iter](const AbstractTerm& t) {
                    return t.same_operators(*iter);
                });
                continue;
            }
            if(!std::equal(kroneckers1.begin(), kroneckers1.end(), kroneckers2.end())) {
                found = std::find_if(found + 1, terms.end(), [&iter](const AbstractTerm& t) {
                    return t.same_operators(*iter);
                });
                continue;
            }
            iter->prefactor.prefactor += found->prefactor.prefactor;
            auto prev = found - 1;
            terms.erase(found);
            found = std::find_if(prev + 1, terms.end(), [&iter](const AbstractTerm& t) {
                return t.same_operators(*iter);
            });
        }
    }

    return terms;
}
