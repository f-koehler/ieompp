#ifndef IEOMPP_OPERATOR_HPP_
#define IEOMPP_OPERATOR_HPP_

#include <ostream>
#include <tuple>
#include <type_traits>

#include "ieompp/tuple.hpp"

namespace ieompp
{
    template <typename... IndexTs>
    struct Operator {
        using Indices = std::tuple<IndexTs...>;

        static constexpr auto number_of_indices = sizeof...(IndexTs);

        bool creator;
        Indices indices;

        bool operator==(const Operator& rhs) const
        {
            return (creator == rhs.creator) && (indices == rhs.indices);
        }

        bool operator!=(const Operator& rhs) const
        {
            return (creator != rhs.creator) || (indices != rhs.indices);
        }

        bool same_indices(const Operator& rhs) const { return indices == rhs.indices; }
    };

    template <typename IndexT>
    struct Operator<IndexT> {
        using Index = IndexT;

        static constexpr auto number_of_indices = 1ul;

        bool creator;
        Index index;

        bool operator==(const Operator& rhs) const
        {
            return (creator == rhs.creator) && (index == rhs.index);
        }

        bool operator!=(const Operator& rhs) const
        {
            return (creator != rhs.creator) || (index != rhs.index);
        }

        bool same_indices(const Operator& rhs) const { return index == rhs.index; }
    };

    template <typename Index1T, typename Index2T>
    struct Operator<Index1T, Index2T> {
        using Index1 = Index1T;
        using Index2 = Index2T;

        static constexpr auto number_of_indices = 2ul;

        bool creator;
        Index1 index1;
        Index2 index2;

        bool operator==(const Operator& rhs) const
        {
            return (creator == rhs.creator) && (index1 == rhs.index1) && (index2 == rhs.index2);
        }

        bool operator!=(const Operator& rhs) const
        {
            return (creator != rhs.creator) || (index1 != rhs.index1) || (index2 != rhs.index2);
        }

        bool same_indices(const Operator& rhs) const
        {
            return (index1 == rhs.index1) && (index2 == rhs.index2);
        }
    };

    template <typename Index>
    constexpr Operator<Index> make_creator(Index&& index)
    {
        return Operator<Index>{true, index};
    }

    template <typename Index1, typename Index2>
    constexpr Operator<Index1, Index2> make_creator(Index1&& index1, Index2&& index2)
    {
        return Operator<Index1, Index2>{true, index1, index2};
    }

    template <typename... Indices>
    constexpr Operator<Indices...> make_creator(Indices&&... indices)
    {
        return Operator<Indices...>{true, std::make_tuple(indices...)};
    }

    template <typename Index>
    constexpr Operator<Index> make_annihilator(Index&& index)
    {
        return Operator<Index>{false, index};
    }

    template <typename Index1, typename Index2>
    constexpr Operator<Index1, Index2> make_annihilator(Index1&& index1, Index2&& index2)
    {
        return Operator<Index1, Index2>{false, index1, index2};
    }

    template <typename... Indices>
    constexpr Operator<Indices...> make_annihilator(Indices&&... indices)
    {
        return Operator<Indices...>{false, std::make_tuple(indices...)};
    }

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
    constexpr const typename std::tuple_element<I, typename Operator<IndexTypes...>::Indices>::type&
    get_index(const Operator<IndexTypes...>& op)
    {
        return std::get<I>(op.indices);
    }

    template <std::size_t I, typename OperatorT>
    struct index_type
    {
        using type = decltype(get_index<I>(OperatorT()));
    };

    template <typename Index>
    std::ostream& operator<<(std::ostream& strm, const Operator<Index>& op)
    {
        strm << "c_{" << op.index << "}";
        if(op.creator) strm << u8"^†";
        return strm;
    }

    template <typename Index1, typename Index2>
    std::ostream& operator<<(std::ostream& strm, const Operator<Index1, Index2>& op)
    {
        strm << "c_{" << op.index1 << "," << op.index2 << "}";
        if(op.creator) strm << u8"^†";
        return strm;
    }

    template <typename... IndexTypes>
    std::ostream& operator<<(std::ostream& strm, const Operator<IndexTypes...>& op)
    {
        strm << "c_";
        print_tuple(strm, op.indices, ",", "{", "}");
        if(op.creator) strm << u8"^†";
        return strm;
    }

    namespace detail
    {
        template <typename Operator, std::size_t I, std::size_t N>
        struct has_symbolic_index_helper {
            static constexpr bool value =
                !std::is_fundamental<typename index_type<I, Operator>::type>::value
                || has_symbolic_index_helper<Operator, I + 1, N>::value;
        };

        template <typename Operator, std::size_t N>
        struct has_symbolic_index_helper<Operator, N, N> {
            static constexpr bool value =
                !std::is_fundamental<typename index_type<N, Operator>::type>::value;
        };
    }

    template <typename Operator>
    struct has_symbolic_index
    {
        static constexpr bool value =
            detail::has_symbolic_index_helper<Operator, 0, Operator::number_of_indices - 1>::value;
    };
}

#endif
