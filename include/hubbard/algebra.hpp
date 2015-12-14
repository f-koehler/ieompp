#ifndef ALGEBRA_HPP_
#define ALGEBRA_HPP_

#include <algorithm>
#include <complex>
#include <ostream>
#include <vector>

#include "hubbard/iterator_range.hpp"

using Real = double;
using Complex = std::complex<Real>;

template<typename Index, typename Spin>
struct Operator {
    bool  creator;
    Index index;
    Spin  spin;

    bool operator==(const Operator& rhs) const;
    bool operator!=(const Operator& rhs) const;
};

template<typename Operator>
struct Term {
    Complex prefactor;
    std::vector<Operator> operators;
};

template<typename Term>
struct TermList : public std::vector<Term> {
};

template<typename Index, typename Spin>
Operator<Index, Spin> make_creator(const Index& index, const Spin& spin);

template<typename Index, typename Spin>
Operator<Index, Spin> make_annihilator(const Index& index, const Spin& spin);

template<typename Operator>
inline bool anticommutates(const Operator& a, const Operator& b);

template<typename Term>
void commutator(const Term& a, const Term& b);


template<typename Index, typename Spin>
std::ostream& operator<<(std::ostream& strm, const Operator<Index, Spin>& op);

template<typename Operator>
std::ostream& operator<<(std::ostream& strm, const Term<Operator>& term);


#include "hubbard/algebra_impl.hpp"

#endif
