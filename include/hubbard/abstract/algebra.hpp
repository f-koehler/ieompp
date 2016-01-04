#ifndef HUBBARD_ABSTRACT_ALGEBRA_HPP_
#define HUBBARD_ABSTRACT_ALGEBRA_HPP_

#include "hubbard/algebra/operator.hpp"
#include "hubbard/algebra/term.hpp"
#include "hubbard/algebra/commutator.hpp"

#include "hubbard/abstract/index.hpp"
#include "hubbard/abstract/prefactor.hpp"
#include "hubbard/abstract/spin.hpp"

namespace hubbard
{
    namespace abstract
    {
        using AbstractOperator = algebra::Operator<AbstractIndex, AbstractSpin>;

        struct AbstractTerm : public algebra::Term<AbstractOperator, AbstractPrefactor> {
            inline void order_kroneckers();
        };

        using AbstractTermList = algebra::TermList<AbstractTerm>;

        inline AbstractOperator make_creator(const std::string& index, const std::string& spin);
        inline AbstractOperator make_annihilator(const std::string& index, const std::string& spin);
        inline AbstractTerm make_term(const AbstractPrefactor& prefactor,
                                      const std::initializer_list<AbstractOperator>& operators);

    }
}

#include "hubbard/abstract/algebra_impl.hpp"

#endif
