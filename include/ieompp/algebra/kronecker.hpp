#ifndef IEOMPP_SYMBOLIC_KRONECKER_HPP_
#define IEOMPP_SYMBOLIC_KRONECKER_HPP_

#include <ostream>
#include <string>
#include <type_traits>

namespace ieompp
{
    namespace algebra
    {
        struct Kronecker {
            std::string a;
            std::string b;

            bool operator==(const Kronecker& rhs) const
            {
                return ((a == rhs.a) && (b == rhs.b)) || ((a == rhs.b) && (b == rhs.a));
            }
            bool operator!=(const Kronecker& rhs) const
            {
                // TODO: write explicit form
                return !(*this == rhs);
            }

            void order()
            {
                if(a > b) {
                    std::swap(a, b);
                }
            }
        };

        std::ostream& operator<<(std::ostream& strm, const Kronecker& kronecker)
        {
            strm << u8"δ_{" << kronecker.a << "," << kronecker.b << "}";
            return strm;
        }

        Kronecker make_kronecker(const std::string& a, const std::string& b)
        {
            return Kronecker{a, b};
        }

        template <typename IndexType>
        typename std::enable_if<std::is_arithmetic<IndexType>::value, Kronecker>::type
        make_kronecker(const IndexType& a, const IndexType& b)
        {
            return Kronecker{to_string(a), to_string(b)};
        }
    } // namespace algebra
} // namespace ieompp

#endif
