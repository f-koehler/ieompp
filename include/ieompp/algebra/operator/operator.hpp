#ifndef IEOMPP_ALGEBRA_OPERATOR_OPERATOR_HPP_
#define IEOMPP_ALGEBRA_OPERATOR_OPERATOR_HPP_

#include <tuple>

namespace ieompp
{
    namespace algebra
    {
        template <typename... IndexTs>
        struct Operator {
            using Indices = std::tuple<IndexTs...>;

            static constexpr auto number_of_indices = sizeof...(IndexTs);

            static constexpr Operator make_creator(IndexTs&&... indices)
            {
                return Operator{true, std::make_tuple(std::forward<IndexTs>(indices)...)};
            }
            static constexpr Operator make_annihilator(IndexTs&&... indices)
            {
                return Operator{false, std::make_tuple(std::forward<IndexTs>(indices)...)};
            }

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
            Indices index_tuple() const { return indices; }

            void conjugate() { creator = !creator; }
            Operator get_conjugate() const { return Operator{!creator, indices}; }
        };

        template <typename IndexT>
        struct Operator<IndexT> {
            using Index   = IndexT;
            using Indices = std::tuple<Index>;

            static constexpr auto number_of_indices = 1ul;

            static constexpr Operator make_creator(Index&& index)
            {
                return Operator{true, std::forward<Index>(index)};
            }
            static constexpr Operator make_annihilator(Index&& index)
            {
                return Operator{false, std::forward<Index>(index)};
            }

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
            Indices index_tuple() const { return std::make_tuple(index); }

            void conjugate() { creator = !creator; }
            Operator get_conjugate() const { return Operator{!creator, index}; }
        };

        template <typename Index1T, typename Index2T>
        struct Operator<Index1T, Index2T> {
            using Index1  = Index1T;
            using Index2  = Index2T;
            using Indices = std::tuple<Index1, Index2>;

            static constexpr auto number_of_indices = 2ul;

            static constexpr Operator make_creator(Index1&& index1, Index2&& index2)
            {
                return Operator{true, std::forward<Index1>(index1), std::forward<Index2>(index2)};
            }
            static constexpr Operator make_annihilator(Index1&& index1, Index2&& index2)
            {
                return Operator{false, std::forward<Index1>(index1), std::forward<Index2>(index2)};
            }

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
            Indices index_tuple() const { return std::make_tuple(index1, index2); }

            void conjugate() { creator = !creator; }
            Operator get_conjugate() const { return Operator{!creator, index1, index2}; }
        };

        template <typename T>
        struct IsOperator {
            static constexpr bool value = false;
        };

        template <typename... Ts>
        struct IsOperator<Operator<Ts...>> {
            static constexpr bool value = true;
        };
    } // namespace algebra
} // namespace ieompp

#endif
