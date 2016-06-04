#ifndef IEOMPP_SYMBOLIC_KRONECKER_HPP_
#define IEOMPP_SYMBOLIC_KRONECKER_HPP_

#include <ostream>

#include "ieompp/symbolic/index.hpp"

namespace ieompp
{
    namespace symbolic
    {
        struct Kronecker {
            std::string a;
            std::string b;

            template <typename Index>
            Kronecker(const Index& idx1, const Index& idx2) : a(str(idx1)), b(str(idx2))
            {
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
