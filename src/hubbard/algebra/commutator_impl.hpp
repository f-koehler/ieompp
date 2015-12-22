#include "hubbard/algebra/commutator.hpp"

template<typename Operator>
inline bool anticommutates(const Operator& a, const Operator& b) {
    return (a.creator == b.creator)
        || (a.spin    != b.spin)
        || (a.index   != b.index);
}

template<typename Term>
TermList<Term> commutate(const Term& a, const Term& b) {
    const auto& a_ops = a.operators;
    const auto& b_ops = b.operators;
    const auto n = a_ops.size();
    const auto m = b_ops.size();
    using UInt = decltype(a_ops.size());

    assert(m*n % 2 == 0);

    TermList<Term> list;

    const auto prefactor = a.prefactor*b.prefactor;
    for(UInt k = 1; k <= n; ++k) {
        for(UInt l = 1; l <= m; ++l) {
            if(anticommutates(a_ops[k-1], b_ops[l-1]))
                continue;

            double coefficient = 1.;
            if((m*(n-k)+l-1) % 2)
                coefficient = -1.;

            Term&& new_term = Term();
            new_term.prefactor = prefactor*coefficient;
            auto& new_ops = new_term.operators;

            if(k > 1)
                std::copy(a_ops.begin(), a_ops.begin()+k-1, std::back_inserter(new_ops));
            if(l > 1)
                std::copy(b_ops.begin(), b_ops.begin()+l-1, std::back_inserter(new_ops));
            std::copy(b_ops.begin()+l, b_ops.end(), std::back_inserter(new_ops));
            std::copy(a_ops.begin()+k, a_ops.end(), std::back_inserter(new_ops));

            list.emplace_back(new_term);
        }
    }
    return list;
}
