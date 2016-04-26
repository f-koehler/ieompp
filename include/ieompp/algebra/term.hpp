#ifndef IEOMPP_ALGEBRA_TERM_HPP_
#define IEOMPP_ALGEBRA_TERM_HPP_

#include "ieompp/types.hpp"

#include <complex>
#include <vector>

namespace ieompp
{
    namespace algebra
    {

        /*!
         * @brief This struct resembles a term of fermionic operators
         *
         * @tparam Operator Type of the operators
         * @tparam Prefactor Type of the prefactor
         */
        template <typename OperatorT, typename PrefactorT = std::complex<double>>
        struct Term {
            //! The type of the operators
            using Operator = OperatorT;

            //! The type of the prefactor
            using Prefactor = PrefactorT;

            //! The type for real numbers
            using Real = typename PrefactorT::value_type;


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
            bool same_operators(const Term& rhs) const;

            typename std::vector<Operator>::const_iterator find_first_displaced_operator() const;
            typename std::vector<Operator>::iterator find_first_displaced_operator();


            /*!
             * @brief Check if two terms are equal
             *
             * @details Two terms are equal if they have the same prefactor and the same operators
             * (in the same order)
             *
             * @param rhs The term to compare this one to
             * @return true if the terms are equal, false otherwise
             */
            bool operator==(const Term& rhs) const;

            /*!
             * @brief Check if two terms are not equal
             *
             * @details Two terms are equal if they have different prefactors or different operators
             *
             * @param rhs The term to compare this one to
             * @return true if the terms are not equal, false otherwise
             */
            bool operator!=(const Term& rhs) const;

            bool operator<(const Term& rhs) const;
            bool operator>(const Term& rhs) const;
        };

        /*!
         * @brief A structure to hold multiple terms
         *
         * @tparam Term Type of the terms
         */
        template <typename TermT>
        struct TermList : public std::vector<TermT>
        {
            using Term = TermT;
            using std::vector<TermT>::vector;

            void sum();
            void sort();
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

        template <typename Term, bool Erase = true>
        void remove_vanishing_terms(TermList<Term>& terms);
    }
}


#include "detail/term_impl.hpp"

#endif
