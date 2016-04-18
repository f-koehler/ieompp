#ifndef IEOMPP_ALGEBRA_OPERATOR_HPP_
#define IEOMPP_ALGEBRA_OPERATOR_HPP_

#include <ostream>
#include <tuple>

namespace ieompp
{
    namespace algebra {

        /*!
         * @brief This struct is a spin type for operators with no spin
         */
        struct NoSpin {
            NoSpin() = default;
            NoSpin(bool) {}

            /*!
             * @brief Check if two NoSpin instances are equal (always true)
             *
             * @param rhs The NoSpin this one will be compared to.
             * @return Always true.
             */
            bool operator==(const NoSpin& rhs) const;

            /*!
             * @brief Check if two NoSpin instances are not equal (always false)
             *
             * @param rhs The NoSpin this one will be compared to.
             * @return Always false.
             */
            bool operator!=(const NoSpin& rhs) const;
        };

        /*!
         * @brief This struct resembles a fermionic creation/annihilation operator
         *
         * @tparam Index The type that is used to store the spatial coordinate of the operator (e.g.
         * lattice position in real or k-space)
         * @tparam Spin The type that is used to store the spin of the operator
         */
        template <typename IndexT, typename SpinT = NoSpin>
        struct Operator {
            //! Forward Index type
            using Index = IndexT;

            //! Forward Spin type
            using Spin = SpinT;

            //! Type to hold the contration of two of these operators
            using ContractionType = std::tuple<Operator, Operator>;

            //! This bool denotes whether the operator is acreation operator
            bool creator;

            //! Spatial index of the operator
            Index index;

            //! Spin of the operator
            Spin spin;

            /*!
             * @brief Check if two operators are equal
             *
             * @details Two operators are equal if they have the same spin, index and are of same
             * type (creator or annihilator)
             *
             * @param rhs The operator to compare this one with
             * @return true if the operators are equal, false otherwise
             */
            bool operator==(const Operator& rhs) const;

            /*!
             * @brief Check if two operators are not equal
             *
             * @details Two operators are not equal if they have the different spin, index or are of
             * different type (creator or annihilator)
             *
             * @param rhs The operator to compare this one with
             * @return true if the operators are not equal, false otherwise
             */
            bool operator!=(const Operator& rhs) const;

            bool operator<(const Operator& rhs) const;
            bool operator>(const Operator& rhs) const;
        };


        /*!
         * @brief Operator to print an operator to an ostream
         *
         * @tparam Index The type that is used to store the spatial coordinate of the operator (e.g.
         * lattice position in real or k-space)
         * @tparam Spin The type that is used to store the spin of the operator
         *
         * @param strm The ostream that will be used for output
         * @param op The operator that will be printed
         * @return reference to strm
         */
        template <typename Index, typename Spin>
        std::ostream& operator<<(std::ostream& strm, const Operator<Index, Spin>& op);

        /*!
         * @brief Create an creation operator with the given index and spin
         *
         * @tparam Index The type that is used to store the spatial coordinate of the operator (e.g.
         * lattice position in real or k-space)
         * @tparam Spin The type that is used to store the spin of the operator
         *
         * @param index Index of the new operator
         * @param spin Spin of the new operator
         * @return The newly created creation operator
         */
        template <typename Index, typename Spin>
        Operator<Index, Spin> make_creator(const Index& index, const Spin& spin);

        /*!
         * @brief Create an spinless creation operator with the given index
         *
         * @tparam Index The type that is used to store the spatial coordinate of the operator (e.g.
         * lattice position in real or k-space)
         *
         * @param index Index of the new operator
         * @return The newly created creation operator
         */
        template <typename Index>
        Operator<Index, NoSpin> make_creator(const Index& index);

        /*!
         * @brief Create an annihilation operator with the given index and spin
         *
         * @tparam Index The type that is used to store the spatial coordinate of the operator (e.g.
         * lattice position in real or k-space)
         * @tparam Spin The type that is used to store the spin of the operator
         *
         * @param index Index of the new operator
         * @param spin Spin of the new operator
         * @return The newly created annihilation operator
         */
        template <typename Index, typename Spin>
        Operator<Index, Spin> make_annihilator(const Index& index, const Spin& spin);

        /*!
         * @brief Create a spinless annihilation operator with the given index
         *
         * @tparam Index The type that is used to store the spatial coordinate of the operator (e.g.
         * lattice position in real or k-space)
         *
         * @param index Index of the new operator
         * @return The newly created annihilation operator
         */
        template <typename Index>
        Operator<Index, NoSpin> make_annihilator(const Index& index);
    }
}

#include "detail/operator_impl.hpp"

#endif
