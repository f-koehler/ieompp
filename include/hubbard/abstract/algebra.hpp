#ifndef HUBBARD_ABSTRACT_ALGEBRA_HPP_
#define HUBBARD_ABSTRACT_ALGEBRA_HPP_

#include "hubbard/algebra/operator.hpp"
#include "hubbard/algebra/term.hpp"
#include "hubbard/algebra/commutator.hpp"

#include "hubbard/abstract/prefactor.hpp"

namespace hubbard
{
    namespace abstract
    {
        //! An abstract index can be represented using a string
        using AbstractIndex    = std::string;

        //! An abstract spin can be represented using a string
        using AbstractSpin     = std::string;

        //! An AbstractOperator is an instantiation of Operator<> for AbstractIndex and AbstractSpin
        using AbstractOperator = algebra::Operator<AbstractIndex, AbstractSpin>;

        struct AbstractTerm : public algebra::Term<AbstractOperator, AbstractPrefactor> {
            inline void order_kroneckers();
        };

        //! Instantiation of TermList<> for AbstractTerm
        using AbstractTermList = algebra::TermList<AbstractTerm>;

        
        /*!
         * @brief Create an abstract creation operator with the given index and spin
         *
         * @param index Index of the new operator
         * @param spin Spin of the new operator
         * @return The newly created creation operator
         */
        inline AbstractOperator make_creator(const std::string& index, const std::string& spin);
        
        /*!
         * @brief Create an abstract creation annihilation with the given index and spin
         *
         * @param index Index of the new operator
         * @param spin Spin of the new operator
         * @return The newly created annihilation operator
         */
        inline AbstractOperator make_annihilator(const std::string& index, const std::string& spin);

        /*!
         * @brief Create an abstract term given a prefactor and a list of operators
         *
         * @param prefactor Prefactor of the newly created term
         * @param operators List with the initial operators within the term
         * @return The newly created term
         */
        inline AbstractTerm make_term(const AbstractPrefactor& prefactor,
                                      const std::initializer_list<AbstractOperator>& operators);

    }
}

#include "detail/algebra_impl.hpp"

#endif
