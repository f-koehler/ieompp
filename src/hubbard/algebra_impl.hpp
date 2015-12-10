#include "hubbard/algebra.hpp"


template<typename Index, typename Spin>
int Operator<Index, Spin>::anticommutator(const Operator<Index, Spin>& a, const Operator<Index, Spin>& b) {
    if(a.m_creator == b.m_creator) return 0;
    if(a.m_spin != b.m_spin) return 0;
    return a.m_index == b.m_index ? 1 : 0;
}


template<typename Index, typename Spin>
Operator<Index, Spin>::Operator(bool creator, const Index& index, const Spin& spin)
    : m_creator(creator), m_index(index), m_spin(spin)
{
}


template<typename Term>
TermList<Term>& TermList<Term>::operator+=(const Term& rhs) {
    push_back(rhs);
    return *this;
}

template<typename Term>
TermList<Term> TermList<Term>::operator+(const Term& rhs) const {
    TermList list(*this);
    list += rhs;
    return list;
}


template<typename Operator>
typename std::vector<Operator>::size_type Term<Operator>::length() const {
    return operators.length();
}

template<typename Operator>
Term<Operator>& Term<Operator>::operator*=(const Operator& rhs) {
    operators.push_back(rhs);
}

template<typename Operator>
Term<Operator>& Term<Operator>::operator*=(const Term& rhs) {
    prefactor *= rhs.prefactor;
    std::copy(
        rhs.operators.begin(),
        rhs.operators.end(),
        std::back_inserter(operators)
    );
    return *this;
}

template<typename Operator>
Term<Operator> Term<Operator>::operator*(const Operator& rhs) const {
    Term t(*this);
    t *= rhs;
    return rhs;
}

template<typename Operator>
Term<Operator> Term<Operator>::operator*(const Term& rhs) const {
    Term t(*this);
    t *= rhs;
    return t;
}

template<typename Operator>
std::vector<Term<Operator>> Term<Operator>::operator+(const Term& rhs) const {
    return std::vector<Term>{ *this, rhs };
}

template<typename Operator>
const Operator& Term<Operator>::operator[](std::size_t idx) const {
    return operators[idx];
}

template<typename Operator>
Operator& Term<Operator>::operator[](std::size_t idx) {
    return operators[idx];
}


template<typename Term>
std::vector<Term> commutate(const Term& a, const Term& b) {
    auto n = a.length();
    auto m = b.length();
    for(auto k = 1; k <= n; ++k) {
        for(auto l = 1; l <= m; ++l) {
            auto prefactor = -1;
            if((m*(n-k)+l-1) % 2 == 0) prefactor = 1;
        }
    }
}
