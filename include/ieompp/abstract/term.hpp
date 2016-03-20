#ifndef IEOMPP_ABSTRACT_TERM_HPP_
#define IEOMPP_ABSTRACT_TERM_HPP_

#include "ieompp/algebra/term.hpp"
#include "ieompp/abstract/prefactor.hpp"

namespace ieompp
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
