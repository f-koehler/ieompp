#ifndef IEOMPP_INNER_PRODUCT_TERM_SCTRUCTURE_HPP_
#define IEOMPP_INNER_PRODUCT_TERM_SCTRUCTURE_HPP_

#include <cstddef>
#include <vector>

#include <ieompp/hash.hpp>

namespace ieompp
{
    namespace inner_product
    {
        struct TermStructure {
            std::size_t num_operators;
            std::vector<bool> types;

            /*     bool operator<(const TermStructure& rhs) const */
            /*     { */
            /*         if(num_operators != rhs.num_operators) return num_operators <
             * rhs.num_operators; */
            /*         for(std::size_t i = 0; i < num_operators; ++i) */
            /*             if(types[i] != rhs.types[i]) return types[i]; */
            /*         return false; */
            /*     } */

            /*     bool operator>(const TermStructure& rhs) const */
            /*     { */
            /*         if(num_operators != rhs.num_operators) return num_operators >
             * rhs.num_operators; */
            /*         for(std::size_t i = 0; i < num_operators; ++i) */
            /*             if(types[i] != rhs.types[i]) return !types[i]; */
            /*         return false; */
            /*     } */

            bool operator==(const TermStructure& rhs) const
            {
                return (num_operators == rhs.num_operators)
                       && std::equal(types.begin(), types.end(), rhs.types.begin());
            }

            bool operator!=(const TermStructure& rhs) const
            {
                return (num_operators != rhs.num_operators)
                       || std::equal(types.begin(), types.end(), rhs.types.begin());
            }
        };

        template <typename Term>
        TermStructure get_term_structure(const Term& t)
        {
            TermStructure s{t.operators.size(), {}};
            s.types.reserve(s.num_operators);
            for(auto& op : t.operators) s.types.push_back(op.creator);
            return s;
        }
    }
}

namespace std
{
    template <>
    struct hash<ieompp::inner_product::TermStructure> {
        using argument_type = ieompp::inner_product::TermStructure;
        using result_type   = std::size_t;

        result_type operator()(const argument_type& val) const
        {
            std::hash<std::vector<bool>> hash_vector{};
            return ieompp::combine_hashes(val.num_operators, hash_vector(val.types));
        }
    };
}

#endif
