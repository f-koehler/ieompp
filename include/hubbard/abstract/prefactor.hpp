#ifndef HUBBARD_ABSTRACT_PREFACTOR_HPP_
#define HUBBARD_ABSTRACT_PREFACTOR_HPP_

#include "hubbard/types.hpp"

#include <string>
#include <vector>

struct Kronecker {
    std::string left, right;

    inline void order_indices();

    inline bool operator==(const Kronecker& rhs) const;
    inline bool operator!=(const Kronecker& rhs) const;
};
std::ostream& operator<<(std::ostream& strm, const Kronecker& rhs);

struct AbstractPrefactor {
    Complex prefactor;
    std::vector<Kronecker> kroneckers;

    AbstractPrefactor& operator*=(const AbstractPrefactor& rhs);
    AbstractPrefactor operator*(const AbstractPrefactor& rhs) const;

    template<typename T>
    AbstractPrefactor& operator*=(const T& rhs); 
    template<typename T>
    AbstractPrefactor operator*(const T& rhs) const;

    AbstractPrefactor& operator+=(const AbstractPrefactor& rhs);

    inline bool operator==(const AbstractPrefactor& rhs) const;
    inline bool operator!=(const AbstractPrefactor& rhs) const;
};

std::ostream& operator<<(std::ostream& strm, const AbstractPrefactor& rhs);

#include "hubbard/abstract/prefactor_impl.hpp"

#endif
