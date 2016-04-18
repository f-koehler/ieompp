#include "ieompp/algebra/operator.hpp"

namespace ieompp
{
    namespace algebra
    {

        inline bool NoSpin::operator==(const NoSpin& rhs) const
        {
            (void)rhs;
            return true;
        }

        inline bool NoSpin::operator!=(const NoSpin& rhs) const{
            (void)rhs;
            return false;
        }

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
        inline bool Operator<Index, Spin>::operator<(const Operator& rhs) const
        {
            if((*this) == rhs) return false;
            if(creator > rhs.creator) return true;
            if(index < rhs.index) return true;
            if(spin < rhs.spin) return true;
            return false;
        }

        template <typename Index, typename Spin>
        inline bool Operator<Index, Spin>::operator>(const Operator& rhs) const
        {
            if((*this) == rhs) return false;
            if(creator < rhs.creator) return true;
            if(index > rhs.index) return true;
            if(spin > rhs.spin) return true;
            return false;
        }

        template <typename Index, typename Spin>
        inline Operator<Index, Spin> make_creator(const Index& index, const Spin& spin)
        {
            return Operator<Index, Spin>{ true, index, spin };
        }

        template <typename Index>
        inline Operator<Index, NoSpin> make_creator(const Index& index)
        {
            return Operator<Index, NoSpin>{ true, index, NoSpin() };
        }

        template <typename Index, typename Spin>
        inline Operator<Index, Spin> make_annihilator(const Index& index, const Spin& spin)
        {
            return Operator<Index, Spin>{ false, index, spin };
        }

        template <typename Index>
        inline Operator<Index, NoSpin> make_annihilator(const Index& index)
        {
            return Operator<Index, NoSpin>{ false, index, NoSpin() };
        }

        template <typename Index, typename Spin>
        std::ostream& operator<<(std::ostream& strm, const Operator<Index, Spin>& rhs)
        {
            strm << "c";
            if(rhs.creator) strm << u8"^†";
            strm << "_{" << rhs.index << "," << rhs.spin << "}";
            return strm;
        }

        template <typename Index>
        std::ostream& operator<<(std::ostream& strm, const Operator<Index, NoSpin>& rhs)
        {
            strm << "c";
            if(rhs.creator) strm << u8"^†";
            strm << "_{" << rhs.index << "}";
            return strm;
        }

    }
}
