#ifndef HUBBARD_ABSTRACT_PREFACTOR_HPP_
#define HUBBARD_ABSTRACT_PREFACTOR_HPP_

#include "hubbard/types.hpp"

#include <string>
#include <vector>

struct Kronecker {
    std::string left, right;

    inline bool operator==(const Kronecker& rhs) const;
    inline bool operator!=(const Kronecker& rhs) const;
};
std::ostream& operator<<(std::ostream& strm, const Kronecker& rhs);

struct AbstractPrefactor {
    std::vector<Kronecker> kroneckers;
    Complex prefactor;

    AbstractPrefactor& operator*=(const AbstractPrefactor& rhs);
    AbstractPrefactor operator*(const AbstractPrefactor& rhs) const;

    template<typename T>
    AbstractPrefactor& operator*=(const T& rhs); 
    template<typename T>
    AbstractPrefactor operator*(const T& rhs) const;
};

std::ostream& operator<<(std::ostream& strm, const AbstractPrefactor& rhs);

#include "hubbard/abstract/prefactor_impl.hpp"

#endif
