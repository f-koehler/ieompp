#ifndef EIGEN_TRIPLET_LIST_HPP_
#define EIGEN_TRIPLET_LIST_HPP_

#include <algorithm>
#include <vector>

#include <Eigen/Sparse>

#include <ieompp/description.hpp>
#include <ieompp/types/description.hpp>

namespace ieompp
{
    namespace types
    {
        template <typename ScalarT, typename IndexT = typename Eigen::SparseMatrix<ScalarT>::Index>
        class TripletList : public std::vector<Eigen::Triplet<ScalarT, IndexT>>
        {
        public:
            using Scalar  = ScalarT;
            using Index   = IndexT;
            using Triplet = Eigen::Triplet<Scalar, Index>;

        private:
            const Index _rows, _cols;

        public:
            TripletList(Index rows, Index cols) : _rows(rows), _cols(cols) {}

            Index rows() const { return _rows; }
            Index cols() const { return _rows; }

            template <bool ColumnMajor = true>
            void sort() {
                if(ColumnMajor) {
                    std::sort(this->begin(), this->end(), [](const Triplet& a, const Triplet& b) {
                        if(a.row() < b.row()) return true;
                        return false;
                    });
                } else {
                    std::sort(this->begin(), this->end(), [](const Triplet& a, const Triplet& b) {
                        if(a.col() < b.col()) return true;
                        return false;
                    });
                }
            }
        };
    }

    template <typename Scalar, typename Index>
    struct TypeDescription<types::TripletList<Scalar, Index>> {
        static Description get()
        {
            return {{"Eigen triplet list", ""},
                    {"  scalar", compose(TypeProperties<Scalar>::name, ' ', "(size ",
                                         TypeProperties<Scalar>::size, ")")}};
        }
    };
}


#endif
