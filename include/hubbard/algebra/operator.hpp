#ifndef HUBBARD_ALGEBRA_OPERATOR_HPP_
#define HUBBARD_ALGEBRA_OPERATOR_HPP_

#include <ostream>

namespace hubbard
{
    namespace algebra {

        template <typename Index, typename Spin>
        struct Operator {
            bool creator;
            Index index;
            Spin spin;

            inline bool operator==(const Operator& rhs) const;
            inline bool operator!=(const Operator& rhs) const;
        };

        template <typename Index, typename Spin>
        std::ostream& operator<<(std::ostream& strm, const Operator<Index, Spin>& op);

        template <typename Index, typename Spin>
        inline Operator<Index, Spin> make_creator(const Index& index, const Spin& spin);

        template <typename Index, typename Spin>
        inline Operator<Index, Spin> make_annihilator(const Index& index, const Spin& spin);
    }
}

#include "hubbard/algebra/operator_impl.hpp"

#endif
