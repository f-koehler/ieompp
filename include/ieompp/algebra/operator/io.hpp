#ifndef IEOMPP_ALGEBRA_OPERATOR_IO_HPP_
#define IEOMPP_ALGEBRA_OPERATOR_IO_HPP_

#include <ostream>

#include <ieompp/tuple.hpp>
#include <ieompp/algebra/operator/operator.hpp>

namespace ieompp
{
    namespace algebra
    {
        template <typename Index>
        std::ostream& operator<<(std::ostream& strm, const Operator<Index>& op)
        {
            strm << "c_{" << op.index << "}";
            if(op.creator) strm << u8"^†";
            return strm;
        }

        template <typename Index1, typename Index2>
        std::ostream& operator<<(std::ostream& strm, const Operator<Index1, Index2>& op)
        {
            strm << "c_{" << op.index1 << "," << op.index2 << "}";
            if(op.creator) strm << u8"^†";
            return strm;
        }

        template <typename... IndexTypes>
        std::ostream& operator<<(std::ostream& strm, const Operator<IndexTypes...>& op)
        {
            strm << "c_";
            print_tuple(strm, op.indices, ",", "{", "}");
            if(op.creator) strm << u8"^†";
            return strm;
        }
    }
}

#endif
