#include "hubbard/algebra/operator.hpp"

namespace hubbard
{
    namespace algebra
    {

        template <typename Index, typename Spin>
        inline bool Operator<Index, Spin>::operator==(const Operator<Index, Spin>& rhs) const
        {
            return (creator == rhs.creator)
                && (spin    == rhs.spin)
                && (index   == rhs.index);
        }

        template <typename Index, typename Spin>
        inline bool Operator<Index, Spin>::operator!=(const Operator<Index, Spin>& rhs) const
        {
            return (creator != rhs.creator)
                || (spin    != rhs.spin)
                || (index   != rhs.index);
        }

        template <typename Index, typename Spin>
        inline Operator<Index, Spin> make_creator(const Index& index, const Spin& spin)
        {
            return Operator<Index, Spin>{ true, index, spin };
        }

        template <typename Index, typename Spin>
        inline Operator<Index, Spin> make_annihilator(const Index& index, const Spin& spin)
        {
            return Operator<Index, Spin>{ false, index, spin };
        }
        
        template <typename Index, typename Spin>
        std::ostream& operator<<(std::ostream& strm, const Operator<Index, Spin>& rhs)
        {
            strm << "c";
            if(rhs.creator) strm << u8"^†";
            strm << "_{" << rhs.index << "," << rhs.spin << "}";
            return strm;
        }

    }
}
