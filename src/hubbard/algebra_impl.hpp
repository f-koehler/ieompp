#include "hubbard/algebra.hpp"


template<typename Index, typename Spin>
bool Operator<Index, Spin>::operator==(const Operator<Index, Spin>& rhs) const {
    return (creator == rhs.creator)
        && (spin    == rhs.spin)
        && (index   == rhs.index);
}

template<typename Index, typename Spin>
bool Operator<Index, Spin>::operator!=(const Operator<Index, Spin>& rhs) const {
    return (creator != rhs.creator)
        || (spin    != rhs.spin)
        || (index   != rhs.index);
}

template<typename Operator>
inline bool anticommutates(const Operator& a, const Operator& b) {
    return (a.creator == b.creator)
        || (a.spin    != b.spin)
        || (a.index   != b.index);
}


template<typename Index, typename Spin>
Operator<Index, Spin> make_creator(const Index& index, const Spin& spin) {
    return std::move(Operator<Index, Spin>{ true, index, spin });
}

template<typename Index, typename Spin>
Operator<Index, Spin> make_annihilator(const Index& index, const Spin& spin) {
    return std::move(Operator<Index, Spin>{ false, index, spin });
}


template<typename Term>
void commutator(const Term& a, const Term& b) {
    const auto& a_ops = a.operators;
    const auto& b_ops = b.operators;
    const auto n = a_ops.size();
    const auto m = b_ops.size();
    using UInt = decltype(a_ops.size());

    const auto prefactor = a.prefacor*b.prefacor;
    for(UInt k = 1; k <= n; ++k) {
        for(UInt l = 1; l <= m; ++l) {
            if(anticommutates(a[k-1], b[l-1]))
                continue;

            int coefficient = 1;
            if((m*(n-k)+l-1) % 2)
                coefficient = -1;

            Term&& new_term = Term();
            new_term.prefacor = prefactor*coefficient;
            auto& new_ops = new_term.operators;

            std::copy(a_ops.begin(), a_ops.end()+k, std::back_inserter(new_ops));
            std::copy(b_ops.begin(), b_ops.end()+l, std::back_inserter(new_ops));
            std::copy(a_ops.begin()+l, a_ops.end(), std::back_inserter(new_ops));
            std::copy(b_ops.begin()+k, b_ops.end(), std::back_inserter(new_ops));
        }
    }
}

template<typename Index, typename Spin>
std::ostream& operator<<(std::ostream& strm, const Operator<Index, Spin>& op) {
    strm << "c";
    if(op.creator)
        strm << "\u2020";
    strm << "(" << op.index << "," << op.spin << ")";
    return strm;
}

template<typename Operator>
std::ostream& operator<<(std::ostream& strm, const Term<Operator>& term) {
    strm << "(" << term.prefactor.real() << "+" << term.prefactor.imag() << "i) \u22C5";
    for(auto& op : term.operators)
        strm << " " << op;
    return strm;
}
