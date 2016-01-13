#ifndef HUBBARD_ALGEBRA_TERM_HPP_
#define HUBBARD_ALGEBRA_TERM_HPP_

#include "hubbard/types.hpp"

#include <vector>

namespace hubbard
{
    namespace algebra
    {

        /*!
         * @brief This struct resembles a term of fermionic operators
         *
         * @tparam Operator Type of the operators
         * @tparam Prefactor Type of the prefactor
         */
        template <typename Operator, typename Prefactor = Complex>
        struct Term {
            //! The type of the operators
            using OperatorType  = Operator;

            //! The type of the prefactor
            using PrefactorType = Prefactor;


            //! Prefactor of the term
            Prefactor prefactor;

            //! Vector that holde the operators
            std::vector<Operator> operators;

            /*!
             * @brief Check if two terms have the same operators (in the same order)
             *
             * @param rhs The term to compare to
             * @return true if the operators are the same, false otherwise
             */
            inline bool same_operators(const Term& rhs) const;

            /*!
             * @brief Check if the operators are ordered.
             *
             * @details The operators are ordered if all creators are in the beginning followed by
             * the annihilators
             *
             * @return true if the term is ordered, false otherwise
             */
            inline bool ordered() const;


            /*!
             * @brief Check if two terms are equal
             *
             * @details Two terms are equal if they have the same prefactor and the same operators
             * (in the same order)
             *
             * @param rhs The term to compare this one to
             * @return true if the terms are equal, false otherwise
             */
            inline bool operator==(const Term& rhs) const;

            /*!
             * @brief Check if two terms are not equal
             *
             * @details Two terms are equal if they have different prefactors or different operators
             *
             * @param rhs The term to compare this one to
             * @return true if the terms are not equal, false otherwise
             */
            inline bool operator!=(const Term& rhs) const;
        };

        /*!
         * @brief A structure to hold multiple terms
         *
         * @tparam Term Type of the terms
         */
        template <typename Term>
        class TermList : public std::vector<Term>
        {
            using std::vector<Term>::vector;
        };

        /*!
         * @brief Operator to print a term to an ostream
         *
         * @tparam Operator The type of the operators in the term
         * @tparam Prefactor The type of the prefactor of the term
         *
         * @param strm The ostream that will be used for output
         * @param term The term that will be printed
         * @return reference to strm
         */
        template <typename Operator, typename Prefactor>
        std::ostream& operator<<(std::ostream& strm, const Term<Operator, Prefactor>& term);


        /*!
         * @brief Create a term given a prefactor and a list of operators
         *
         * @tparam Operator The type of the operators in the term
         * @tparam Prefactor The type of the prefactor of the term
         *
         * @param prefactor Prefactor of the newly created term
         * @param operators List with the initial operators within the term
         * @return The newly created term
         */
        template <typename Prefactor, typename Operator>
        Term<Operator, Prefactor> make_term(const Prefactor& prefactor,
                                            const std::initializer_list<Operator>& operators);

        /*!
         * @brief Find the first operator that destroy the order of operators within the term
         *
         * @tparam Term The type of the term
         *
         * @param term The term to scan
         * @return Iterator to the first unordered operator or end to the operator list if the term
         * is ordered
         */
        template <typename Term>
        auto find_unordered_operator(Term& term) ->
            typename std::vector<typename Term::OperatorType>::iterator;
    }
}


#include "hubbard/algebra/term_impl.hpp"

#endif
