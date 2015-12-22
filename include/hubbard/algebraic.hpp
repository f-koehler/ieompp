#ifndef ALGEBRAIC_HPP_
#define ALGEBRAIC_HPP_

#include <ostream>
#include <string>
#include <vector>

#include "hubbard/types.hpp"

struct AlgebraicSpin {
    static const std::string Up, Down;
    std::string spin;

    AlgebraicSpin(const std::string& spin);

    bool operator==(const AlgebraicSpin& rhs) const;
    bool operator!=(const AlgebraicSpin& rhs) const;
};

std::ostream& operator<<(std::ostream& strm, const AlgebraicSpin& spin);

struct AlgebraicIndex {
    std::string index;

    AlgebraicIndex(const std::string& index);

    bool operator==(const AlgebraicIndex& rhs) const;
    bool operator!=(const AlgebraicIndex& rhs) const;
};

std::ostream& operator<<(std::ostream& strm, const AlgebraicIndex& index);


struct Kronecker {
    std::string left, right;
};

struct AlgebraicPrefactor {
    std::vector<Kronecker> kroneckers;
    Complex prefactor;
};

#include "hubbard/algebraic.hpp"

#endif
