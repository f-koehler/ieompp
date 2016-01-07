#ifndef HUBBARD_ABSTRACT_PREFACTOR_HPP_
#define HUBBARD_ABSTRACT_PREFACTOR_HPP_

#include "hubbard/types.hpp"

#include <string>
#include <vector>

namespace hubbard
{
    namespace abstract
    {

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

            inline bool same_kroneckers(const AbstractPrefactor& rhs) const;

            AbstractPrefactor& operator*=(const AbstractPrefactor& rhs);
            AbstractPrefactor operator*(const AbstractPrefactor& rhs) const;

            template <typename T>
            AbstractPrefactor& operator*=(const T& rhs);
            template <typename T>
            AbstractPrefactor operator*(const T& rhs) const;

            AbstractPrefactor& operator+=(const AbstractPrefactor& rhs);

            inline bool operator==(const AbstractPrefactor& rhs) const;
            inline bool operator!=(const AbstractPrefactor& rhs) const;
        };

        std::ostream& operator<<(std::ostream& strm, const AbstractPrefactor& rhs);
    }

    template <>
    std::ostream& format<abstract::AbstractPrefactor>(std::ostream& strm,
                                                      const abstract::AbstractPrefactor& t)
    {
        return abstract::operator<<(strm, t);
    }
}

#include "hubbard/abstract/prefactor_impl.hpp"

#endif
