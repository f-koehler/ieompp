#ifndef HUBBARD_ABSTRACT_ALGEBRA_HPP_
#define HUBBARD_ABSTRACT_ALGEBRA_HPP_

#include "hubbard/algebra.hpp"
#include "hubbard/abstract/index.hpp"
#include "hubbard/abstract/prefactor.hpp"
#include "hubbard/abstract/spin.hpp"

namespace hubbard
{
    namespace abstract {

        using AbstractOperator = algebra::Operator<AbstractIndex, AbstractSpin>;

        struct AbstractTerm : public algebra::Term<AbstractOperator, AbstractPrefactor> {
            inline void order_kroneckers();
        };

        using AbstractTermList = algebra::TermList<AbstractTerm>;

        inline AbstractOperator make_creator(const std::string& index, const std::string& spin);
        inline AbstractOperator make_annihilator(const std::string& index, const std::string& spin);


        namespace simplify
        {
            inline AbstractTermList& order(AbstractTermList& terms);
            inline AbstractTermList& join(AbstractTermList& terms);
            inline AbstractTermList& filter(AbstractTermList& terms);
        }

        inline AbstractTermList& simplify_terms(AbstractTermList& terms);
    }
}

#include "hubbard/abstract/algebra_impl.hpp"

#endif
