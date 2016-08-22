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
        };

        template <typename IndexT>
        struct Operator<IndexT> {
            using Index   = IndexT;
            using Indices = std::tuple<Index>;

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
            Indices index_tuple() const { return std::make_tuple(index); }
        };

        template <typename Index1T, typename Index2T>
        struct Operator<Index1T, Index2T> {
            using Index1  = Index1T;
            using Index2  = Index2T;
            using Indices = std::tuple<Index1, Index2>;

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
            Indices index_tuple() const { return std::make_tuple(index1, index2); }
        };
    }
}

#endif
