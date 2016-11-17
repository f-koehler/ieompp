#ifndef IEOMPP_TYPES_EIGEN_TRIPLET_LIST_HPP_
#define IEOMPP_TYPES_EIGEN_TRIPLET_LIST_HPP_

#include "ieompp/description.hpp"
#include "ieompp/types/description.hpp"
#include "ieompp/types/matrix.hpp"

#include <algorithm>
#include <vector>

namespace ieompp
{
    namespace types
    {
        namespace detail
        {
            template <typename ScalarT, typename IndexT>
            struct Triplet {
                using Scalar = ScalarT;
                using Index  = IndexT;

                Index row, column;
                Scalar value;

                Triplet(Index i, Index j, Scalar val) : row(i), column(j), value(val) {}
            };
        } // namespace detail

        template <typename ScalarT, typename IndexT = std::uint64_t>
        class TripletList : public std::vector<detail::Triplet<ScalarT, IndexT>>
        {
        public:
            using Scalar  = ScalarT;
            using Index   = IndexT;
            using Triplet = detail::Triplet<Scalar, Index>;

        private:
            const Index _rows, _cols;

        public:
            TripletList(Index rows, Index cols) : _rows(rows), _cols(cols) {}

            Index rows() const { return _rows; }
            Index cols() const { return _rows; }

            void sort_column_major()
            {
                std::sort(this->begin(), this->end(), [](const Triplet& a, const Triplet& b) {
                    return static_cast<bool>(a.row < b.row);
                });
            }

            void sort_row_major()
            {
                std::sort(this->begin(), this->end(), [](const Triplet& a, const Triplet& b) {
                    return static_cast<bool>(a.column < b.column);
                });
            }
        };

        template <typename Scalar, typename Index>
        void matrix_insert(TripletList<Scalar, Index>& matrix, Index row, Index col,
                           const Scalar& value)
        {
            matrix.emplace_back(row, col, value);
        }

        template <typename Scalar, typename Index>
        struct ScalarType<TripletList<Scalar, Index>> {
            using type = Scalar;
        };

        template <typename Scalar, typename Index>
        struct IndexType<TripletList<Scalar, Index>> {
            using type = Index;
        };
    } // namespace types


    template <typename Scalar, typename Index>
    struct TypeDescription<types::TripletList<Scalar, Index>> {
        static Description description()
        {
            return Description{{"type", "TripletList"}, {"scalar", ""}}
                   + get_type_description<Scalar>().indent(2) + Description{{"index", ""}}
                   + get_type_description<Index>().indent(2);
        }
    };

    template <typename Scalar, typename Index>
    struct InstanceDescription<types::TripletList<Scalar, Index>> {
        static Description description(const types::TripletList<Scalar, Index>& list)
        {
            return Description{{"rows", std::to_string(list.rows())},
                               {"cols", std::to_string(list.cols())}};
        }
    };
} // namespace ieompp


#endif
