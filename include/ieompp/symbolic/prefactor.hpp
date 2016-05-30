#ifndef IEOMPP_SYMBOLIC_PREFACTOR_HPP_
#define IEOMPP_SYMBOLIC_PREFACTOR_HPP_

#include <algorithm>
#include <initializer_list>
#include <vector>

#include "ieompp/symbolic/kronecker.hpp"

namespace ieompp
{
    namespace symbolic
    {
        template <typename ValueT, typename ContainerT = std::vector<Kronecker>>
        struct Prefactor {
            typename Value     = ValueT;
            typename Container = ContainerT;

            Value value;
            Container kroneckers;

            const Prefactor& operator*=(const Kronecker& rhs)
            {
                kroneckers.push_back(rhs);
                return *this;
            }

            const Prefactor& operator*=(const std::vector<Kronecker>& rhs)
            {
                std::copy(rhs.begin(), rhs.end(), std::back_inserter(kroneckers));
                return *this;
            }

            template <typename Value_, typename Container_>
            const Prefactor& operator*=(const Prefactor<Value_, Container_>& rhs)
            {
                std::copy(rhs.kroneckers.begin(), rhs.kroneckers.end(),
                          std::back_inserter(kroneckers));
                value *= rhs.value;

                return *this;
            }
        };

        template <typename Value>
        Prefactor<Value> make_prefactor(const Value& val,
                                        std::initializer_list<Kronecker> kroneckers)
        {
            return Prefactor<Value>{val, kroneckers};
        }
    }
}

#endif
