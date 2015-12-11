#ifndef ALGEBRA_HPP_
#define ALGEBRA_HPP_

#include <algorithm>
#include <complex>
#include <vector>

#include "hubbard/iterator_range.hpp"

using Real = double;
using Complex = std::complex<Real>;

template<typename Term>
std::vector<Term> commutate(const Term& a, const Term& b);

template<typename Index, typename Spin>
class Operator {
    private:
        bool  m_creator;
        Index m_index;
        Spin  m_spin;

    public:
        static int anticommutator(const Operator& a, const Operator& b);

        Operator(bool creator, const Index& index, const Spin& spin);
        Operator(const Operator&) = default;
        Operator& operator=(const Operator&) = default;

        bool operator==(const Operator& rhs) const;
        bool operator!=(const Operator& rhs) const;
};

template<typename Term>
class TermList : public std::vector<Term> {
    public:
        TermList& operator+=(const Term& rhs);
        TermList operator+(const Term& rhs) const;
};


template<typename Operator>
class Term {
    public:
        using OperatorCRange = IteratorRange<typename std::vector<Operator>::const_iterator>;
        using OperatorRange  = IteratorRange<typename std::vector<Operator>::iterator>;

    private:
        Complex m_prefactor;
        std::vector<Operator> m_operators;

    public:
        Term() = default;
        Term(const Term&) = default;
        Term& operator=(const Term&) = default;

        typename std::vector<Operator>::size_type length() const;
        const Complex& prefactor() const;

        Term& operator*=(const Operator& rhs);
        Term& operator*=(const Term& rhs);
        Term  operator*(const Operator& rhs) const;
        Term  operator*(const Term& rhs) const;

        std::vector<Term> operator+(const Term& rhs) const;

        const Operator& operator[](std::size_t idx) const;
        Operator& operator[](std::size_t idx);
        
        OperatorCRange operator()(std::size_t start, std::size_t end) const;
        OperatorRange operator()(std::size_t start, std::size_t end);
};


#include "hubbard/algebra_impl.hpp"

#endif
