#ifndef HUBBARD_ABSTRACT_PREFACTOR_HPP_
#define HUBBARD_ABSTRACT_PREFACTOR_HPP_

#include "hubbard/abstract/operator.hpp"

#include <complex>
#include <string>
#include <vector>

namespace hubbard
{
    namespace abstract
    {
        /*!
         * @brief This struct represents a Kronecker delta.
         */
        struct Kronecker {
            //! First index of the Kronecker
            std::string left;
            
            //! Second index of the Kronecker
            std::string right;

            /*!
             * @brief Order the two indices lexicographically
             */
            inline void order_indices();


            /*!
             * @brief Check if two Kroneckers are equal. This is the case when the indices are the
             * same (order does not matter)
             *
             * @param rhs The Kronecker this one will be compared to
             * @return true if the Kroneckers are the same, false otherwise
             */
            inline bool operator==(const Kronecker& rhs) const;

            /*!
             * @brief Check if two Kroneckers are not equal. This is the case when the indices are
             * different (order does not matter)
             *
             * @param rhs The Kronecker this one will be compared to
             * @return true if the Kroneckers are the different, false otherwise
             */
            inline bool operator!=(const Kronecker& rhs) const;
        };

        /*!
         * @brief Operator to print a Kronecker to a std::ostream.
         *
         * @details This operator will print a Kronecker to the given std::ostream using the δ
         * unicode symbol. Assuming rhs is a Kronecker with the left index "k_1" and the right index
         * "k_2" the string "δ_{k_1,k_2}" will be written.
         *
         * @param strm The strm that will be used for the output operation. The reference will also
         * be returned on completion.
         * @param rhs The Kronecker symbol to print
         * @return The std::ostream used for the output
         */
        std::ostream& operator<<(std::ostream& strm, const Kronecker& rhs);

        struct AbstractPrefactor {
            //! Typedef for the real part of the prefactor (for compability to std::complex<>)
            using value_type = double;

            //! The complex prefactor component
            std::complex<double> number;

            //! The Kronecker deltas in the prefactor
            std::vector<Kronecker> kroneckers;

            inline bool same_kroneckers(const AbstractPrefactor& rhs) const;

            AbstractPrefactor& operator*=(const AbstractPrefactor& rhs);
            AbstractPrefactor operator*(const AbstractPrefactor& rhs) const;

            template <typename T>
            AbstractPrefactor& operator*=(const T& rhs);
            template <typename T>
            AbstractPrefactor operator*(const T& rhs) const;

            AbstractPrefactor& operator+=(const AbstractPrefactor& rhs);

            inline bool operator==(const AbstractPrefactor& rhs) const;
            inline bool operator!=(const AbstractPrefactor& rhs) const;
        };

        std::ostream& operator<<(std::ostream& strm, const AbstractPrefactor& rhs);
    }
}

#include "detail/prefactor_impl.hpp"

#endif
