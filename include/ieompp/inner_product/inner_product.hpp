#ifndef IEOMPP_INNER_PRODUCT_INNER_PRODUCT_HPP_
#define IEOMPP_INNER_PRODUCT_INNER_PRODUCT_HPP_

#include <unordered_set>
#include <iostream>

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/inner_product/term_structure.hpp>

namespace ieompp
{
    namespace inner_product
    {
        struct InnerProductTable;

        struct InnerProduct {
            struct Kronecker {
                const std::size_t a, b;
            };

            struct Expression {
                double real;
                Kronecker kronecker;
            };

            using Operator = ieompp::algebra::Operator<std::size_t>;
            using Term     = ieompp::algebra::Term<double, Operator>;
            using Result   = std::vector<Expression>;

            TermStructure left_structure;
            TermStructure right_structure;
            Result rhs;

            InnerProduct() = default;
            InnerProduct(const TermStructure& a, const TermStructure& b)
                : left_structure(a), right_structure(b)
            {
            }

            void calculate()
            {
                if((left_structure.num_operators == 0) || (right_structure.num_operators == 0)) {
                    if(left_structure.types[0] != right_structure.types[0])
                        rhs.emplace_back(Expression{0.5, Kronecker{0, 0}});
                    return;
                }

                Term left_term, right_term;

                left_term.prefactor  = 1.;
                right_term.prefactor = 1.;

                std::size_t idx = 0;

                for(auto t : left_structure.types)
                    left_term.operators.emplace_back(Operator{t, idx++});
                for(auto t : right_structure.types)
                    right_term.operators.emplace_back(Operator{t, idx++});
                left_term.conjugate();

                std::cout << left_term << "\t\t" << right_term << std::endl;
            }

            bool operator==(const InnerProduct& rhs) const
            {
                return (left_structure == rhs.left_structure)
                       && (right_structure == rhs.right_structure);
            }

            bool operator!=(const InnerProduct& rhs) const
            {
                return (left_structure != rhs.left_structure)
                       || (right_structure != rhs.right_structure);
            }
        };

        std::ostream& operator<<(std::ostream& strm, const InnerProduct& product)
        {
            auto idx = 0ul;
            strm << "(";
            for(std::size_t i = 0; i < product.left_structure.num_operators; ++i) {
                strm << "c_{i" << idx << "}";
                if(product.left_structure.types[i]) strm << u8"^†";
                strm << " ";
                ++idx;
            }
            strm << "| ";
            for(std::size_t i = 0; i < product.right_structure.num_operators - 1; ++i) {
                strm << "c_{i" << idx << "}";
                if(product.left_structure.types[i]) strm << u8"^†";
                strm << " ";
                ++idx;
            }
            strm << "c_{i" << idx << "}";
            if(product.left_structure.types[product.right_structure.num_operators - 1])
                strm << u8"^†";
            ++idx;
            strm << ")";
            return strm;
        }
    }
}

namespace std {
    template <>
    struct hash<ieompp::inner_product::InnerProduct> {
        using argument_type  = ieompp::inner_product::InnerProduct;
        using result_type    = std::size_t;

        result_type operator()(const argument_type& val) const
        {
            const typename std::hash<ieompp::inner_product::TermStructure> hash_structure{};
            return ieompp::combine_hashes(hash_structure(val.left_structure),
                                          hash_structure(val.right_structure));
        }
    };
}

namespace ieompp
{
    namespace inner_product
    {
        struct InnerProductTable : public std::unordered_set<InnerProduct> {
        };
    }
}

#endif
