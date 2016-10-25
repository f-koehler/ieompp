#ifndef IEOMPP_ALGEBRA_TERM_IO_HPP_
#define IEOMPP_ALGEBRA_TERM_IO_HPP_

#include <iterator>
#include <ostream>

#include <ieompp/algebra/term/term.hpp>

namespace ieompp
{
    namespace algebra
    {
        template <typename Prefactor, typename Operator, typename Container>
        std::ostream& operator<<(std::ostream& strm, const Term<Prefactor, Operator, Container>& t)
        {
            strm << t.prefactor << u8" ⋅ ";
            std::copy(t.operators.begin(), --t.operators.end(),
                      std::ostream_iterator<Operator>(strm, " "));
            strm << t.operators.back();
            return strm;
        }
    } // namespace algebra
} // namespace ieompp

#endif
