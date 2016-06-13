#ifndef IEOMPP_SYMBOLIC_KRONECKER_HPP_
#define IEOMPP_SYMBOLIC_KRONECKER_HPP_

#include <ostream>
#include <string>

#include "ieompp/symbolic/index.hpp"

namespace ieompp
{
    namespace symbolic
    {
        struct Kronecker {
            std::string a;
            std::string b;

            template <typename Index>
            Kronecker(const Index& idx1, const Index& idx2) : a(idx1.index), b(idx2.index)
            {
            }

            bool operator==(const Kronecker& rhs) const
            {
                return ((a == rhs.a) && (b == rhs.b)) || ((a == rhs.b) && (b == rhs.a));
            }
            bool operator!=(const Kronecker& rhs) const
            {
                // TODO: write explicit form
                return !(*this == rhs);
            }
        };

        std::ostream& operator<<(std::ostream& strm, const Kronecker& kronecker)
        {
            strm << u8"δ_{" << kronecker.a << "," << kronecker.b << "}";
            return strm;
        }
    }
}

#endif
