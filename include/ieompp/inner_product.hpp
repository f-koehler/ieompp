#ifndef IEOMPP_SCALAR_PRODUCT_HPP_
#define IEOMPP_SCALAR_PRODUCT_HPP_

#include <algorithm>
#include <cstddef>
#include <map>
#include <ostream>
#include <vector>

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>

namespace ieompp
{
    struct TermStructure {
        std::size_t num_operators;
        std::vector<bool> types;

        bool operator<(const TermStructure& rhs) const {
            if(num_operators != rhs.num_operators) return num_operators < rhs.num_operators;
            for(std::size_t i = 0; i < num_operators; ++i)
                if(types[i] != rhs.types[i]) return types[i];
            return false;
        }

        bool operator>(const TermStructure& rhs) const {
            if(num_operators != rhs.num_operators) return num_operators > rhs.num_operators;
            for(std::size_t i = 0; i < num_operators; ++i)
                if(types[i] != rhs.types[i]) return !types[i];
            return false;
        }

        bool operator==(const TermStructure& rhs) const {
            return (num_operators == rhs.num_operators)
                   && std::equal(types.begin(), types.end(), rhs.types.begin());
        }

        bool operator!=(const TermStructure& rhs) const {
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

    struct InnerProductContainer;

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

        void calculate() {
            if(left_structure.num_operators == right_structure.num_operators == 0) {
                if(left_structure.types[0] != right_structure.types[0])
                    rhs.emplace_back(0.5, Kronecker{0, 0});
                return;
            }

            Term left_term, right_term;

            left_term.prefactor  = 1.;
            right_term.prefactor = 1.;
        }
    };

    std::ostream& operator<<(std::ostream& strm, const InnerProduct& product)
    {
        auto idx = 0;
        strm << "(";
        for(auto t : product.left_structure.types) {
            strm << "c_{i" << idx << "}";
            if(t) strm << u8"^†";
            ++idx;
        }
        strm << "|";
        for(auto t : product.right_structure.types) {
            strm << "c_{i" << idx << "}";
            if(t) strm << u8"^†";
            ++idx;
        }
        strm << ")";
        return strm;
    }

    struct InnerProductContainer
        : public std::map<TermStructure, std::map<TermStructure, InnerProduct>> {

        using Element = std::map<TermStructure, InnerProduct>;
        using Map     = std::map<TermStructure, Element>;

        InnerProduct& insert(const TermStructure& a, const TermStructure& b)
        {
            InnerProduct& p = (*this)[a][b];
            p               = InnerProduct(a, b);
            return p;
        }

        InnerProduct& operator()(const TermStructure& a, const TermStructure& b)
        {
            auto& m = (*this)[a];
            if(m.empty()) return insert(a, b);
            auto pos = m.find(b);
            if(pos == m.end()) return insert(a, b);
            return pos->second;
        }

        template <typename Term>
        InnerProduct& operator()(const Term& t1, const Term& t2) {
            return (*this)(get_term_structure(t1), get_term_structure(t2));
        }
    };
}

#endif
