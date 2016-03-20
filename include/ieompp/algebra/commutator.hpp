#ifndef IEOMPP_ALGEBRA_COMMUTATOR_HPP_
#define IEOMPP_ALGEBRA_COMMUTATOR_HPP_

#include "ieompp/algebra/term.hpp"

namespace ieompp
{
    namespace algebra {

        /*!
         * @brief Check whether the anticommutator of two operators vanishes
         *
         * @tparam Operator Type of the operator
         *
         * @param a The left operator
         * @param b The right operator
         * @return true if the anticommutator vanishes, false otherwise
         */
        template <typename Operator>
        inline bool anticommutates(const Operator& a, const Operator& b);

        /*!
         * @brief Calculates the anticommutator of two operators
         *
         * @tparam Operator Type of the operator
         * @tparam Prefactor The prefactor type of the anticommutator
         *
         * @param a The left operator
         * @param b The right operator
         * @return The resulting prefactor
         */
        template <typename Operator, typename Prefactor = std::complex<double>>
        inline Prefactor anticommutator(const Operator& a, const Operator& b);


        /*!
         * @brief Calculates the commutator between two terms
         *
         * @tparam Term The type of the terms to commutate
         *
         * @param a The left term
         * @param b The right term
         * @param terms Termlist where all resulting terms will be stored
         */
        template <typename Term>
        void commutate(const Term& a, const Term& b, TermList<Term>& terms);
    }
}

#include "detail/commutator_impl.hpp"

#endif
