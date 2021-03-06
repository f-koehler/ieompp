#ifndef TEST_ALGEBRA_OPERATOR_HPP_
#define TEST_ALGEBRA_OPERATOR_HPP_

#include <ieompp/algebra/operator/io.hpp>
#include <ieompp/algebra/operator/is_symbolic.hpp>
#include <ieompp/algebra/operator/operator.hpp>

#include <type_traits>

#include <catch.hpp>

using Operator1 = ieompp::algebra::Operator<std::size_t>;
using Operator2 = ieompp::algebra::Operator<std::size_t, bool>;
using Operator3 = ieompp::algebra::Operator<std::size_t, bool, char>;

namespace ieompp
{
    namespace algebra
    {
        // check number of indices
        static_assert(Operator1::number_of_indices == 1, "Operator1 should have 1 index");
        static_assert(Operator2::number_of_indices == 2, "Operator2 should have 2 indices");
        static_assert(Operator3::number_of_indices == 3, "Operator3 should have 3 indices");


        // check index tuple type
        static_assert(std::is_same<Operator1::Indices, std::tuple<std::size_t>>::value,
                      "Index tuple should be of type std::tuple<std::size_t>");
        static_assert(std::is_same<Operator2::Indices, std::tuple<std::size_t, bool>>::value,
                      "Index tuple should be of type std::tuple<std::size_t, bool>");
        static_assert(std::is_same<Operator3::Indices, std::tuple<std::size_t, bool, char>>::value,
                      "Index tuple should be of type std::tuple<std::size_t, bool>");


        // check types of individual indices
        static_assert(std::is_same<Operator1::Index, std::size_t>::value,
                      "Index type should be std::size_t");

        static_assert(std::is_same<Operator2::Index1, std::size_t>::value,
                      "Index type should be std::size_t");
        static_assert(std::is_same<Operator2::Index2, bool>::value,
                      "Index type should be std::size_t");
        // TODO(fkoehler): check Operator3


        // check IsOperator
        static_assert(IsOperator<Operator1>::value, "Operator1 should be a Operator type");
        static_assert(IsOperator<Operator2>::value, "Operator2 should be a Operator type");
        static_assert(IsOperator<Operator3>::value, "Operator3 should be a Operator type");
        static_assert(!IsOperator<int>::value, "int should not be a Operator type");

        // check is_symbolic
        static_assert(!IsSymbolicOperator<Operator<int>>::value,
                      "Operator<int> should not be symbolic");
        static_assert(!IsSymbolicOperator<Operator<int, bool>>::value,
                      "Operator<int, bool> should not be symbolic");
        static_assert(IsSymbolicOperator<Operator<std::string>>::value,
                      "Operator<std::string> should be symbolic");
        static_assert(IsSymbolicOperator<Operator<int, bool, std::string>>::value,
                      "Operator<int, bool, std::string> should be symbolic");

        // check is_symbolic index
        static_assert(!IsSymbolicIndex<0, Operator<int>>::value,
                      "Index 0 of Operator<int> should not be symbolic");
        static_assert(!IsSymbolicIndex<0, Operator<int, bool>>::value,
                      "Index 0 of Operator<int, bool> should not be symbolic");
        static_assert(!IsSymbolicIndex<1, Operator<int, bool>>::value,
                      "Index 1 of Operator<int, bool> should not be symbolic");
        static_assert(IsSymbolicIndex<0, Operator<std::string>>::value,
                      "Index 0 of Operator<std::string> should be symbolic");
        static_assert(!IsSymbolicIndex<0, Operator<int, bool, std::string>>::value,
                      "Index 0 of Operator<int, bool, std::string> should not be symbolic");
        static_assert(!IsSymbolicIndex<1, Operator<int, bool, std::string>>::value,
                      "Index 1 of Operator<int, bool, std::string> should not be symbolic");
        static_assert(IsSymbolicIndex<2, Operator<int, bool, std::string>>::value,
                      "Index 2 of Operator<int, bool, std::string> should be symbolic");
    }
}

#endif
