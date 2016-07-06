#ifndef IEOMPP_SCALAR_PRODUCT_HPP_
#define IEOMPP_SCALAR_PRODUCT_HPP_

#include <cstddef>
#include <vector>

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>

namespace ieompp
{
    struct TermStructure {
        std::size_t num_operators;
        std::vector<bool> types;
    };

    struct ScalarProduct {
        using Operator = algebra::Operator<std::size_t>;
        TermStructure left_structure;
        TermStructure right_structure;

        template <typename Term>
        ScalarProduct(const Term& a, const Term& b)
            : left_structure{a.operators.size(), {}}, right_structure{b.operators.size(), {}}
        {
            for(auto& op : a.operators) left_structure.types.push_back(op.creator);
            for(auto& op : b.operators) right_structure.types.push_back(op.creator);
        }
    };
}

#endif
