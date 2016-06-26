#ifndef IEOMPP_ALGEBRA_ANTICOMMUTATOR_HPP_
#define IEOMPP_ALGEBRA_ANTICOMMUTATOR_HPP_

#include <cstddef>
#include <type_traits>
#include <vector>

#include <ieompp/algebra/operator.hpp>
#include <ieompp/symbolic/kronecker.hpp>

namespace ieompp
{
    namespace algebra
    {
        namespace detail
        {
            template <typename Operator, std::size_t I, std::size_t N>
            struct anticommutator_helper {
                void operator()(const Operator& op1, const Operator& op2,
                                std::vector<symbolic::Kronecker>& kroneckers)
                {
                    if(!std::is_fundamental<typename index_type<N, Operator>::type>::value) {
                        auto& idx1 = get_index<I>(op1);
                        auto& idx2 = get_index<I>(op2);
                        if(idx1 != idx2) kroneckers.emplace_back(symbolic::Kronecker(idx1, idx2));
                    }
                    anticommutator_helper<Operator, I + 1, N>()(op1, op2, kroneckers);
                }
            };

            template <typename Operator, std::size_t N>
            struct anticommutator_helper<Operator, N, N> {
                void operator()(const Operator& op1, const Operator& op2,
                                std::vector<symbolic::Kronecker>& kroneckers)
                {
                    if(!std::is_fundamental<typename index_type<N, Operator>::type>::value) {
                        auto& idx1 = get_index<N>(op1);
                        auto& idx2 = get_index<N>(op2);
                        if(idx1 != idx2) kroneckers.emplace_back(symbolic::Kronecker(idx1, idx2));
                    }
                }
            };
        }

        template <typename Operator>
        typename std::enable_if<!has_symbolic_index<Operator>::value, bool>::type
        anticommutates(const Operator& op1, const Operator& op2)
        {
            return (op1.creator == op2.creator) || !op1.same_indices(op2);
        }

        template <typename Operator>
        typename std::enable_if<has_symbolic_index<Operator>::value, bool>::type
        anticommutates(const Operator& op1, const Operator& op2)
        {
            return (op1.creator == op2.creator);
        }

        template <typename Operator>
        typename std::enable_if<has_symbolic_index<Operator>::value,
                                std::vector<symbolic::Kronecker>>::type
        anticommutator(const Operator& op1, const Operator& op2)
        {
            std::vector<symbolic::Kronecker> kroneckers;
            detail::anticommutator_helper<Operator, 0, Operator::number_of_indices - 1>()(
                op1, op2, kroneckers);
            return kroneckers;
        }

        template <typename Operator>
        typename std::enable_if<!has_symbolic_index<Operator>::value, int>::type
        anticommutator(const Operator& op1, const Operator& op2)
        {
            (void)op1;
            (void)op2;
            return 1;
        }
    }
}

#endif
