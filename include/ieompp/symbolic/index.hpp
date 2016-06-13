#ifndef IEOMPP_SYMBOLIC_INDEX_HPP_
#define IEOMPP_SYMBOLIC_INDEX_HPP_

#include <sstream>
#include <string>

namespace ieompp
{
    namespace symbolic
    {
        enum IndexType : int {
            IndexTypeUnspecified,
            IndexTypeSpace,
            IndexTypeMomentum,
            IndexTypeSpin
        };

        template <IndexType index_type>
        struct Index {
            static constexpr IndexType type = index_type;
            std::string index;

            Index();
            Index(const std::string& str);
            Index(int i);

            bool operator==(const Index& rhs) const { return index == rhs.index; }
            bool operator!=(const Index& rhs) const { return index != rhs.index; }
        };

        template <>
        Index<IndexTypeSpace>::Index() : index("r")
        {
        }

        template <>
        Index<IndexTypeMomentum>::Index() : index("k")
        {
        }

        template <>
        Index<IndexTypeSpin>::Index() : index(u8"σ")
        {
        }

        template <IndexType index_type>
        Index<index_type>::Index(const std::string& str) : index(str)
        {
        }

        template <>
        Index<IndexTypeSpace>::Index(int i) : index("r_" + std::to_string(i))
        {
        }

        template <>
        Index<IndexTypeMomentum>::Index(int i) : index("k_" + std::to_string(i))
        {
        }

        template <>
        Index<IndexTypeSpin>::Index(int i) : index(u8"σ_" + std::to_string(i))
        {
        }

        using SpaceIndex    = Index<IndexTypeSpace>;
        using MomentumIndex = Index<IndexTypeMomentum>;
        using SpinIndex     = Index<IndexTypeSpin>;

        template <IndexType index_type>
        std::ostream& operator<<(std::ostream& strm, const Index<index_type>& index)
        {
            return strm << index.index;
        }
    }
}

#endif
