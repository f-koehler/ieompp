#ifndef IEOMPP_ALGEBRA_OPERATOR_GET_INDEX_HPP_
#define IEOMPP_ALGEBRA_OPERATOR_GET_INDEX_HPP_

#include "ieompp/algebra/operator/operator.hpp"

#include <type_traits>

namespace ieompp
{
    namespace algebra
    {
        template <std::size_t I, typename Operator>
        struct IndexType {
            using Type = typename std::tuple_element<I, typename Operator::Indices>::type;
        };

        template <std::size_t I, typename Index>
        constexpr typename std::enable_if<I == 0, Index&>::type& get_index(Operator<Index>& op)
        {
            return op.index;
        }

        template <std::size_t I, typename Index1, typename Index2>
        constexpr typename std::enable_if<I == 0, Index1&>::type&
        get_index(Operator<Index1, Index2>& op)
        {
            return op.index1;
        }

        template <std::size_t I, typename Index1, typename Index2>
        constexpr typename std::enable_if<I == 1, Index2&>::type&
        get_index(Operator<Index1, Index2>& op)
        {
            return op.index2;
        }

        template <std::size_t I, typename... IndexTypes>
        constexpr typename std::tuple_element<I, typename Operator<IndexTypes...>::Indices>::type&
        get_index(Operator<IndexTypes...>& op)
        {
            return std::get<I>(op.indices);
        }

        template <std::size_t I, typename Index>
        constexpr typename std::enable_if<I == 0, Index&>::type&& get_index(Operator<Index>&& op)
        {
            return op.index;
        }

        template <std::size_t I, typename Index1, typename Index2>
        constexpr typename std::enable_if<I == 0, Index1&>::type&&
        get_index(Operator<Index1, Index2>&& op)
        {
            return op.index1;
        }

        template <std::size_t I, typename Index1, typename Index2>
        constexpr typename std::enable_if<I == 1, Index2&>::type&&
        get_index(Operator<Index1, Index2>&& op)
        {
            return op.index2;
        }

        template <std::size_t I, typename... IndexTypes>
        constexpr typename std::tuple_element<I, typename Operator<IndexTypes...>::Indices>::type&&
        get_index(Operator<IndexTypes...>&& op)
        {
            return std::get<I>(op.indices);
        }

        template <std::size_t I, typename Index>
        constexpr typename std::enable_if<I == 0, const Index&>::type&
        get_index(const Operator<Index>& op)
        {
            return op.index;
        }

        template <std::size_t I, typename Index1, typename Index2>
        constexpr typename std::enable_if<I == 0, const Index1&>::type&
        get_index(const Operator<Index1, Index2>& op)
        {
            return op.index1;
        }

        template <std::size_t I, typename Index1, typename Index2>
        constexpr typename std::enable_if<I == 1, const Index2&>::type&
        get_index(const Operator<Index1, Index2>& op)
        {
            return op.index2;
        }

        template <std::size_t I, typename... IndexTypes>
        constexpr const typename std::tuple_element<I, typename Operator<IndexTypes...>::Indices>::
            type&
            get_index(const Operator<IndexTypes...>& op)
        {
            return std::get<I>(op.indices);
        }
    } // namespace algebra
} // namespace ieompp

#endif
