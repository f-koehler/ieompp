#ifndef IEOMPP_ALGEBRA_OPERATOR_MAKE_HPP_
#define IEOMPP_ALGEBRA_OPERATOR_MAKE_HPP_

#include <type_traits>

#include "ieompp/algebra/operator/operator.hpp"

namespace ieompp
{
    namespace algebra
    {
        // TODO: decay reference wrappers
        template <typename Index>
        constexpr Operator<typename std::decay<Index>::type> make_creator(Index&& index)
        {
            return Operator<typename std::decay<Index>::type>{true, std::forward<Index>(index)};
        }

        template <typename Index1, typename Index2>
        constexpr Operator<typename std::decay<Index1>::type, typename std::decay<Index2>::type>
        make_creator(Index1&& index1, Index2&& index2)
        {
            return Operator<typename std::decay<Index1>::type, typename std::decay<Index2>::type>{
                true, std::forward<Index1>(index1), std::forward<Index2>(index2)};
        }

        template <typename... Indices>
        constexpr Operator<typename std::decay<Indices>::type...> make_creator(Indices&&... indices)
        {
            return Operator<typename std::decay<Indices>::type...>{
                true, std::make_tuple(std::forward<Indices>(indices)...)};
        }

        template <typename Index>
        constexpr Operator<typename std::decay<Index>::type> make_annihilator(Index&& index)
        {
            return Operator<typename std::decay<Index>::type>{false, std::forward<Index>(index)};
        }

        template <typename Index1, typename Index2>
        constexpr Operator<typename std::decay<Index1>::type, typename std::decay<Index2>::type>
        make_annihilator(Index1&& index1, Index2&& index2)
        {
            return Operator<typename std::decay<Index1>::type, typename std::decay<Index2>::type>{
                false, std::forward<Index1>(index1), std::forward<Index2>(index2)};
        }

        template <typename... Indices>
        constexpr Operator<typename std::decay<Indices>::type...>
        make_annihilator(Indices&&... indices)
        {
            return Operator<typename std::decay<Indices>::type...>{
                false, std::make_tuple(std::forward<Indices>(indices)...)};
        }
    } // namespace algebra
} // namespace ieompp

#endif
