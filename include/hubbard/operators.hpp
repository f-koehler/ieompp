#ifndef OPERATORS_HPP_
#define OPERATORS_HPP_

#include <algorithm>
#include <cmath>
#include <ostream>
#include <vector>

#include "hubbard/types.hpp"

template<typename Index, typename Spin>
struct Operator {
    bool  creator;
    Index index;
    Spin  spin;

    inline bool operator==(const Operator& rhs) const;
    inline bool operator!=(const Operator& rhs) const;
};

template<typename Operator, typename Prefactor = Complex>
struct Term {
    Prefactor prefactor;
    std::vector<Operator> operators;

    inline bool same_operators(const Term& rhs) const;
    inline bool ordered() const;

    inline bool operator==(const Term& rhs) const;
    inline bool operator!=(const Term& rhs) const;
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
TermList<Term> commutate(const Term& a, const Term& b);


template<typename Index, typename Spin>
std::ostream& operator<<(std::ostream& strm, const Operator<Index, Spin>& op);

template<typename Operator, typename Prefactor>
std::ostream& operator<<(std::ostream& strm, const Term<Operator, Prefactor>& term);


#include "hubbard/operators_impl.hpp"

#endif
