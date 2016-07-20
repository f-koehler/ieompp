#ifndef IEOMPP_INNER_PRODUCT_INNER_PRODUCT_HPP_
#define IEOMPP_INNER_PRODUCT_INNER_PRODUCT_HPP_

#include <unordered_map>
#include <iostream>

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/inner_product/term_structure.hpp>
#include <ieompp/symbolic/prefactor.hpp>

namespace ieompp
{
    namespace inner_product
    {
        struct Kronecker
        {
            std::size_t a, b;
        };

        struct InnerProduct
        {
            using Operator  = ieompp::algebra::Operator<std::size_t>;
            using Prefactor = ieompp::symbolic::Prefactor<double, Kronecker>;
            using Term      = ieompp::algebra::Term<Prefactor, Operator>;

            struct Result : public std::vector<Prefactor>
            {
                bool calculated = false;
            };

            TermStructure left_structure;
            TermStructure right_structure;

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

        std::ostream& operator<<(std::ostream& strm, const Kronecker& k)
        {
            strm << u8"δ_{i" << k.a << ","
                 << "i" << k.b << "}";
            return strm;
        }

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
        struct InnerProductTable {
            std::unordered_map<InnerProduct, std::size_t> indices;
            std::vector<typename InnerProduct::Result> results;

            InnerProductTable()
            {
                auto idx = add({1, {true}}, {1, {true}});
                results[idx].calculated = true;
                results[idx].emplace_back(
                    typename InnerProduct::Prefactor{0.5, {Kronecker{0, 1}}});

                idx = add({1, {false}}, {1, {false}});
                results[idx].calculated = true;
                results[idx].emplace_back(
                    typename InnerProduct::Prefactor{0.5, {Kronecker{0, 1}}});
            }

            std::size_t add(const TermStructure& left, const TermStructure& right)
            {
                InnerProduct prod{left, right};
                auto inserted = indices.insert({prod, 0ul});
                if(!inserted.second) return inserted.first->second;
                auto idx = results.size();
                results.emplace_back(typename InnerProduct::Result{});
                inserted.first->second = idx;
                return idx;
            }
        };
    }
}

#endif
