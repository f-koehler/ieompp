#ifndef HUBBARD_ABSTRACT_OPERATOR_HPP_
#define HUBBARD_ABSTRACT_OPERATOR_HPP_

#include "hubbard/algebra/operator.hpp"

namespace hubbard
{
    namespace abstract
    {
        //! An abstract index can be represented using a string
        using AbstractIndex = std::string;

        //! An abstract spin can be represented using a string
        using AbstractSpin = std::string;

        //! An AbstractOperator is an instantiation of Operator<> for AbstractIndex and AbstractSpin
        using AbstractOperator = algebra::Operator<AbstractIndex, AbstractSpin>;
        
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
    }
}

#include "hubbard/abstract/detail/operator_impl.hpp"

#endif
