#ifndef IEOMPP_ALGEBRA_ANTICOMMUTATOR_HPP_
#define IEOMPP_ALGEBRA_ANTICOMMUTATOR_HPP_

#include <cstddef>
#include <type_traits>
#include <vector>

#include "ieompp/algebra/kronecker.hpp"
#include "ieompp/algebra/operator/get_index.hpp"
#include "ieompp/algebra/operator/is_symbolic.hpp"
#include "ieompp/algebra/operator/operator.hpp"

namespace ieompp
{
    namespace algebra
    {
        namespace detail
        {
            template <typename Operator, std::size_t I, std::size_t N>
            struct AnticommutatesHelper {
                static bool call(const Operator& a, const Operator& b)
                {
                    if(IsSymbolicIndex<I, Operator>::value) {
                        return AnticommutatesHelper<Operator, I + 1, N>::call(a, b);
                    }
                    if(get_index<I>(a) != get_index<I>(b)) {
                        return true;
                    }
                    return AnticommutatesHelper<Operator, I + 1, N>::call(a, b);
                }
            };

            template <typename Operator, std::size_t N>
            struct AnticommutatesHelper<Operator, N, N> {
                static bool call(const Operator& a, const Operator& b)
                {
                    if(IsSymbolicIndex<N, Operator>::value) {
                        return false;
                    }
                    return (get_index<N>(a) != get_index<N>(b));
                }
            };

            template <typename Operator, typename Prefactor, std::size_t I, std::size_t N>
            struct AnticommutatorHelper {
                static void call(const Operator& op1, const Operator& op2, Prefactor& prefactor)
                {
                    if(IsSymbolicIndex<I, Operator>::value) {
                        auto& idx1 = get_index<I>(op1);
                        auto& idx2 = get_index<I>(op2);
                        if(idx1 != idx2) {
                            prefactor *= make_kronecker(idx1, idx2);
                        }
                    }
                    return AnticommutatorHelper<Operator, Prefactor, I + 1, N>::call(op1, op2,
                                                                                     prefactor);
                }
            };

            template <typename Operator, typename Prefactor, std::size_t N>
            struct AnticommutatorHelper<Operator, Prefactor, N, N> {
                static void call(const Operator& op1, const Operator& op2, Prefactor& prefactor)
                {
                    if(IsSymbolicIndex<N, Operator>::value) {
                        auto& idx1 = get_index<N>(op1);
                        auto& idx2 = get_index<N>(op2);
                        if(idx1 != idx2) {
                            prefactor *= make_kronecker(idx1, idx2);
                        }
                    }
                }
            };
        } // namespace detail

        template <typename Operator>
        typename std::enable_if<!IsSymbolicOperator<Operator>::value, bool>::type
        anticommutates(const Operator& op1, const Operator& op2)
        {
            return (op1.creator == op2.creator) || !op1.same_indices(op2);
        }

        template <typename Operator>
        typename std::enable_if<IsSymbolicOperator<Operator>::value, bool>::type
        anticommutates(const Operator& op1, const Operator& op2)
        {
            return (op1.creator == op2.creator)
                   || detail::AnticommutatesHelper<Operator, 0,
                                                   Operator::number_of_indices - 1>::call(op1, op2);
        }

        template <typename Term>
        typename std::enable_if<IsSymbolicOperator<typename Term::Operator>::value, void>::type
        anticommutator(const typename Term::Operator& op1, const typename Term::Operator& op2,
                       typename Term::Prefactor& prefactor)
        {
            detail::AnticommutatorHelper<typename Term::Operator, typename Term::Prefactor, 0,
                                         Term::Operator::number_of_indices - 1>::call(op1, op2,
                                                                                      prefactor);
        }

        template <typename Term>
        typename std::enable_if<!IsSymbolicOperator<typename Term::Operator>::value, void>::type
        anticommutator(const typename Term::Operator& op1, const typename Term::Operator& op2,
                       typename Term::Prefactor& prefactor)
        {
            (void)op1;
            (void)op2;
            (void)prefactor;
        }
    } // namespace algebra
} // namespace ieompp

#endif
