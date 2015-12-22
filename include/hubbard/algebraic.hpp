#ifndef ALGEBRAIC_HPP_
#define ALGEBRAIC_HPP_

#include <ostream>
#include <string>
#include <vector>

#include "hubbard/types.hpp"
#include "hubbard/algebra.hpp"

struct AlgebraicSpin {
    static const std::string Up, Down;
    std::string spin;

    AlgebraicSpin(const std::string& spin);

    inline bool operator==(const AlgebraicSpin& rhs) const;
    inline bool operator!=(const AlgebraicSpin& rhs) const;
};

std::ostream& operator<<(std::ostream& strm, const AlgebraicSpin& spin);

struct AlgebraicIndex {
    std::string index;

    AlgebraicIndex(const std::string& index);

    inline bool operator==(const AlgebraicIndex& rhs) const;
    inline bool operator!=(const AlgebraicIndex& rhs) const;
};

std::ostream& operator<<(std::ostream& strm, const AlgebraicIndex& index);


struct Kronecker {
    std::string left, right;
};

struct AlgebraicPrefactor {
    std::vector<Kronecker> kroneckers;
    Complex prefactor;

    AlgebraicPrefactor& operator*=(const AlgebraicPrefactor& rhs);
    AlgebraicPrefactor operator*(const AlgebraicPrefactor& rhs) const;

    template<typename T>
    AlgebraicPrefactor& operator*=(const T& rhs); 
    template<typename T>
    AlgebraicPrefactor operator*(const T& rhs) const;
};

using AlgebraicOperator = Operator<AlgebraicIndex, AlgebraicSpin>;
using AlgebraicTerm = Term<AlgebraicOperator, AlgebraicPrefactor>;
using AlgebraicTermList = TermList<AlgebraicTerm>;

inline AlgebraicOperator make_algebraic_creator(const std::string& index, const std::string& spin);
inline AlgebraicOperator make_algebraic_annihilator(const std::string& index, const std::string& spin);

#include "hubbard/algebraic_impl.hpp"

#endif
