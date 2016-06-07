#ifndef IEOMPP_TERM_HPP_
#define IEOMPP_TERM_HPP_

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <ostream>
#include <type_traits>
#include <vector>

namespace ieompp
{
    template <typename PrefactorT, typename OperatorT, typename ContainerT = std::vector<OperatorT>>
    struct Term {
        using Prefactor = PrefactorT;
        using Operator  = OperatorT;
        using Container = ContainerT;

        Prefactor prefactor;
        Container operators;
    };

    template <typename Prefactor, typename Operator, typename Container = std::vector<Operator>>
    Term<Prefactor, Operator> make_term(Prefactor&& prefactor,
                                        std::initializer_list<Operator> operators)
    {
        return Term<Prefactor, Operator>{
            typename std::decay<Prefactor>::type(std::forward<Prefactor>(prefactor)),
            Container{operators}};
    }

    template <typename Prefactor, typename Operator, typename Container>
    std::ostream& operator<<(std::ostream& strm, const Term<Prefactor, Operator, Container>& t)
    {
        strm << t.prefactor << u8" ⋅ ";
        std::copy(t.operators.begin(), --t.operators.end(),
                  std::ostream_iterator<Operator>(strm, " "));
        strm << t.operators.back();
        return strm;
    }
}

#endif
