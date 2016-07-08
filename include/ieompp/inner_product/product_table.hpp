#ifndef IEOMPP_INNER_PRODUCT_PRODUCT_TABLE_HPP_
#define IEOMPP_INNER_PRODUCT_PRODUCT_TABLE_HPP_

#include <unordered_map>

#include <ieompp/inner_product/term_structure.hpp>
#include <ieompp/inner_product/inner_product.hpp>

namespace ieompp
{
    namespace inner_product
    {
        struct ProductTableIndex {
            TermStructure left_structure;
            TermStructure right_structure;

            bool operator==(const ProductTableIndex& rhs) const
            {
                return (left_structure == rhs.left_structure)
                       && (right_structure == rhs.right_structure);
            }
            
            bool operator!=(const ProductTableIndex& rhs) const
            {
                return (left_structure != rhs.left_structure)
                       || (right_structure != rhs.right_structure);
            }
        };
    }
}

namespace std {
    template <>
    struct hash<ieompp::inner_product::ProductTableIndex>
    {
        using argument_type = ieompp::inner_product::ProductTableIndex;
        using result_type   = std::size_t;

        result_type operator()(const argument_type& val) const {
            const std::hash<ieompp::inner_product::TermStructure> hash_structure{};
            return ieompp::combine_hashes(hash_structure(val.left_structure),
                                          hash_structure(val.right_structure));
        }
    };
}

namespace ieompp
{
    namespace inner_product
    {
        class ProductTable : public std::unordered_map<ProductTableIndex, InnerProduct>
        {
            protected:
                using BaseClass = std::unordered_map<ProductTableIndex, InnerProduct>;
                using BaseClass::operator[];
                using BaseClass::at;

            public:
                InnerProduct& operator()(const ProductTableIndex& idx)
                {
                    auto& ref = (*this)[idx];
                    if(ref.left_structure.types.empty())
                        ref = InnerProduct(idx.left_structure, idx.right_structure);
                    return ref;
                }

                template <typename Term>
                InnerProduct& operator()(const Term& a, const Term& b)
                {
                    return (*this)(ProductTableIndex{get_term_structure(a), get_term_structure(b)});
                }
        };
    }
}

#endif
