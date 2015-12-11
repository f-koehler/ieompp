#include "hubbard/algebra.hpp"


template<typename Index, typename Spin>
int Operator<Index, Spin>::anticommutator(const Operator<Index, Spin>& a, const Operator<Index, Spin>& b) {
    if(a.m_creator == b.m_creator) return 0;
    if(a.m_spin != b.m_spin) return 0;
    return a.m_index == b.m_index ? 1 : 0;
}



/*
 * Operator
 */
template<typename Index, typename Spin>
Operator<Index, Spin>::Operator(bool creator, const Index& index, const Spin& spin)
    : m_creator(creator), m_index(index), m_spin(spin)
{
}

template<typename Index, typename Spin>
bool Operator<Index, Spin>::operator==(const Operator& rhs) const {
    return (m_creator == rhs.m_creator)
        && (m_spin    == rhs.m_spin)
        && (m_index   == rhs.m_index);
}

template<typename Index, typename Spin>
bool Operator<Index, Spin>::operator!=(const Operator& rhs) const {
    return (m_creator != rhs.m_creator)
        || (m_spin    != rhs.m_spin)
        || (m_index   != rhs.m_index);
}



/*
 * TermList
 */
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



/*
 * Term
 */
template<typename Operator>
typename std::vector<Operator>::size_type Term<Operator>::length() const {
    return m_operators.length();
}

template<typename Operator>
const Complex& Term<Operator>::prefactor() const {
    return m_prefactor;
}

template<typename Operator>
Term<Operator>& Term<Operator>::operator*=(const Operator& rhs) {
    m_operators.push_back(rhs);
    return *this;
}

template<typename Operator>
Term<Operator>& Term<Operator>::operator*=(const Term& rhs) {
    m_prefactor *= rhs.m_prefactor;
    std::copy(
        rhs.m_operators.begin(),
        rhs.m_operators.end(),
        std::back_inserter(m_operators)
    );
    return *this;
}

template<typename Operator>
Term<Operator> Term<Operator>::operator*(const Operator& rhs) const {
    Term t(*this);
    t *= rhs;
    return t;
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
    return m_operators[idx];
}

template<typename Operator>
Operator& Term<Operator>::operator[](std::size_t idx) {
    return m_operators[idx];
}

template<typename Operator>
typename Term<Operator>::OperatorCRange Term<Operator>::operator()(std::size_t start, std::size_t end) const {
    return OperatorCRange(
        m_operators.cbegin()+start,
        m_operators.cend()+end
    );
}

template<typename Operator>
typename Term<Operator>::OperatorRange Term<Operator>::operator()(std::size_t start, std::size_t end) {
    return OperatorRange(
        m_operators.begin()+start,
        m_operators.end()+end
    );
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
