#ifndef HUBBARD_ABSTRACT_TERM_HPP_
#define HUBBARD_ABSTRACT_TERM_HPP_

#include "hubbard/algebra/term.hpp"
#include "hubbard/abstract/prefactor.hpp"

namespace hubbard
{
    namespace abstract
    {
        struct AbstractTerm : public algebra::Term<AbstractOperator, AbstractPrefactor> {
            inline void order_kroneckers();
        };

        //! Instantiation of TermList<> for AbstractTerm
        using AbstractTermList = algebra::TermList<AbstractTerm>;

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

#include "detail/term_impl.hpp"

#endif
