#ifndef IEOMPP_ALGEBRA_OPERATOR_DESCRIPTION_HPP_
#define IEOMPP_ALGEBRA_OPERATOR_DESCRIPTION_HPP_

#include <ieompp/algebra/operator/operator.hpp>
#include <ieompp/description.hpp>
#include <ieompp/types/description.hpp>

namespace ieompp
{
    template <typename... Indices>
    struct TypeDescription<algebra::Operator<Indices...>> {
        static Description description()
        {
            return Description{
                {"fermionic operator", ""},
                {"indices", std::to_string(algebra::Operator<Indices...>::number_of_indices)}};
            // TODO: print index types as below
        }
    };

    template <typename Index>
    struct TypeDescription<algebra::Operator<Index>> {
        static Description description()
        {
            return Description{{"fermionic operator", ""}, {"  indices", "1"}, {"  index0", ""}}
                   + get_type_description<Index>();
        }
    };

    template <typename Index1, typename Index2>
    struct TypeDescription<algebra::Operator<Index1, Index2>> {
        static Description description()
        {
            return Description{{"fermionic operator", ""}, {"indices", "2"}, {"index0", ""}}
                   + get_type_description<Index1>().indent(2) + Description{{"index1", ""}}
                   + get_type_description<Index2>().indent(2);
        }
    };
}

#endif
