#include "hubbard/operators.hpp"


template<typename Operator, typename Prefactor>
inline bool Term<Operator, Prefactor>::ordered() const { 
    bool annihilator = false;
    for(auto& op : operators) {
        if(op.creator) {
            if(annihilator) return false;
        } else {
            annihilator = true;
        }
    }
    return true;
}

template<typename Operator, typename Prefactor>
inline bool Term<Operator, Prefactor>::same_operators(const Term<Operator, Prefactor>& rhs) const {
    if(operators.size() != rhs.operators.size())
        return false;
    return std::equal(operators.begin(), operators.end(), rhs.operators.begin());
}

template<typename Index, typename Spin>
inline bool Operator<Index, Spin>::operator==(const Operator<Index, Spin>& rhs) const {
    return (creator == rhs.creator)
        && (spin    == rhs.spin)
        && (index   == rhs.index);
}

template<typename Index, typename Spin>
inline bool Operator<Index, Spin>::operator!=(const Operator<Index, Spin>& rhs) const {
    return (creator != rhs.creator)
        || (spin    != rhs.spin)
        || (index   != rhs.index);
}

template<typename Operator, typename Prefactor>
inline bool Term<Operator, Prefactor>::operator==(const Term<Operator, Prefactor>& rhs) const {
    return same_operators(rhs) && prefactor == rhs.prefactor;
}

template<typename Operator, typename Prefactor>
inline bool Term<Operator, Prefactor>::operator!=(const Term<Operator, Prefactor>& rhs) const {
    return !same_operators(rhs) || prefactor != rhs.prefactor;
}


template<typename Operator>
inline bool anticommutates(const Operator& a, const Operator& b) {
    return (a.creator == b.creator)
        || (a.spin    != b.spin)
        || (a.index   != b.index);
}


template<typename Index, typename Spin>
Operator<Index, Spin> make_creator(const Index& index, const Spin& spin) {
    return Operator<Index, Spin>{ true, index, spin };
}

template<typename Index, typename Spin>
Operator<Index, Spin> make_annihilator(const Index& index, const Spin& spin) {
    return Operator<Index, Spin>{ false, index, spin };
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

template<typename Index, typename Spin>
std::ostream& operator<<(std::ostream& strm, const Operator<Index, Spin>& op) {
    strm << "c";
    if(op.creator)
        strm << "\u2020";
    strm << "(" << op.index << "," << op.spin << ")";
    return strm;
}

template<typename Operator, typename Prefactor>
std::ostream& operator<<(std::ostream& strm, const Term<Operator, Prefactor>& term) {
    /* strm << "(" << term.prefactor.real(); */
    /* if(term.prefactor.imag() < 0) */
    /*     strm << "-"; */
    /* else */
    /*     strm << "+"; */
    /* strm << std::abs(term.prefactor.imag()) << "i) \u22C5"; */
    strm << term.prefactor << " \u22C5";
    for(auto& op : term.operators)
        strm << " " << op;
    return strm;
}
