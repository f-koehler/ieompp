#include "hubbard/abstract/algebra.hpp"

#include <algorithm>

namespace hubbard
{
    namespace algebra
    {
        template <>
        bool anticommutates<abstract::AbstractOperator>(const abstract::AbstractOperator& a,
                                                        const abstract::AbstractOperator& b)
        {
            if(a.creator == b.creator) return true;
            return false;
        }

        template <>
        abstract::AbstractPrefactor
        anticommutator<abstract::AbstractOperator, abstract::AbstractPrefactor>(
            const abstract::AbstractOperator& a, const abstract::AbstractOperator& b)
        {
            abstract::AbstractPrefactor result;
            result.prefactor.real(1.);
            result.prefactor.imag(0.);
            if(a.index != b.index) result.kroneckers.push_back({a.index.index, b.index.index});
            if(a.spin != b.spin) result.kroneckers.push_back({a.spin.spin, b.spin.spin});
            return result;
        }
    }

    namespace abstract
    {
        std::ostream& operator<<(std::ostream& strm, const AbstractOperator& rhs)
        {
            strm << "c";
            if(rhs.creator) strm << u8"^†";
            strm << "_{" << rhs.index << "," << rhs.spin << "}";
            return strm;
        }

        inline void AbstractTerm::order_kroneckers()
        {
            for(auto& k : prefactor.kroneckers) k.order_indices();
            std::sort(prefactor.kroneckers.begin(), prefactor.kroneckers.end(),
                      [](const Kronecker& k1, const Kronecker& k2) { return k1.left < k2.left; });
        }

        inline AbstractOperator make_creator(const std::string& index, const std::string& spin)
        {
            return AbstractOperator{true, AbstractIndex{index}, AbstractSpin{spin}};
        }

        inline AbstractOperator make_annihilator(const std::string& index, const std::string& spin)
        {
            return AbstractOperator{false, AbstractIndex{index}, AbstractSpin{spin}};
        }

        inline AbstractTerm make_term(const AbstractPrefactor& prefactor,
                                      const std::initializer_list<AbstractOperator>& operators)
        {
            AbstractTerm term;
            term.prefactor = prefactor;
            term.operators = operators;
            return term;
        }


        inline void sort_kronecker_indices(Kronecker& k)
        {
            if(k.left < k.right) return;
            std::swap(k.left, k.right);
        }


    }
}
