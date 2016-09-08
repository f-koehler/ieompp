#ifndef IEOMPP_SYMBOLIC_PREFACTOR_HPP_
#define IEOMPP_SYMBOLIC_PREFACTOR_HPP_

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <ostream>
#include <type_traits>
#include <vector>

#include <ieompp/algebra/kronecker.hpp>

namespace ieompp
{
    namespace symbolic
    {
        template <typename ValueT, typename KroneckerT = algebra::Kronecker>
        struct Prefactor {
            using Value     = ValueT;
            using Kronecker = KroneckerT;

            Value value;
            std::vector<Kronecker> kroneckers;

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
        Prefactor<Value> make_prefactor(const Value& val,
                                        std::initializer_list<algebra::Kronecker> kroneckers =
                                            std::initializer_list<algebra::Kronecker>())
        {
            return Prefactor<Value>{val, kroneckers};
        }

        template <typename Value, typename Kronecker>
        std::ostream& operator<<(std::ostream& strm, const Prefactor<Value, Kronecker>& rhs)
        {
            if(rhs.kroneckers.empty()) {
                strm << rhs.value;
                return strm;
            }
            if(std::is_signed<Value>::value && (rhs.value == Value(-1)))
                strm << "-";
            else if(rhs.value != Value(1))
                strm << rhs.value << u8" ⋅ ";
            std::copy(rhs.kroneckers.begin(), --rhs.kroneckers.end(),
                      std::ostream_iterator<Kronecker>(strm, " "));
            strm << rhs.kroneckers.back();
            return strm;
        }
    }
}

#endif
