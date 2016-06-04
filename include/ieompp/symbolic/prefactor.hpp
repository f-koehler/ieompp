#ifndef IEOMPP_SYMBOLIC_PREFACTOR_HPP_
#define IEOMPP_SYMBOLIC_PREFACTOR_HPP_

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <ostream>
#include <vector>

#include "ieompp/symbolic/kronecker.hpp"

namespace ieompp
{
    namespace symbolic
    {
        template <typename ValueT, typename ContainerT = std::vector<Kronecker>>
        struct Prefactor {
            using Value     = ValueT;
            using Container = ContainerT;

            Value value;
            Container kroneckers;

            Prefactor& operator*=(const Kronecker& rhs)
            {
                kroneckers.push_back(rhs);
                return *this;
            }

            Prefactor& operator*=(const std::vector<Kronecker>& rhs)
            {
                std::copy(rhs.begin(), rhs.end(), std::back_inserter(kroneckers));
                return *this;
            }

            template <typename Value_, typename Container_>
            Prefactor& operator*=(const Prefactor<Value_, Container_>& rhs)
            {
                std::copy(rhs.kroneckers.begin(), rhs.kroneckers.end(),
                          std::back_inserter(kroneckers));
                value *= rhs.value;

                return *this;
            }

            template <typename Number>
            Prefactor& operator*=(const Number& number)
            {
                value *= number;
                return *this;
            }

            template <typename Value_, typename Container_>
            Prefactor operator*(const Prefactor<Value_, Container_>& rhs) const
            {
                Prefactor copy(*this);
                copy *= rhs;
                return copy;
            }
        };

        template <typename Value>
        Prefactor<Value> make_prefactor(
            const Value& val,
            std::initializer_list<Kronecker> kroneckers = std::initializer_list<Kronecker>())
        {
            return Prefactor<Value>{val, kroneckers};
        }

        template <typename Value, typename Container>
        std::ostream& operator<<(std::ostream& strm, const Prefactor<Value, Container>& rhs)
        {
            strm << rhs.value;
            if(!rhs.kroneckers.empty()) {
                strm << u8" ⋅ ";
                std::copy(rhs.kroneckers.begin(), --rhs.kroneckers.end(),
                          std::ostream_iterator<Kronecker>(strm, " "));
                strm << rhs.kroneckers.back();
            }
            return strm;
        }
    }
}

#endif
