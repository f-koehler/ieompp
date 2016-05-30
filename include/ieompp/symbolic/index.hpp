#ifndef IEOMPP_SYMBOLIC_INDEX_HPP_
#define IEOMPP_SYMBOLIC_INDEX_HPP_

#include <ostream>
#include <sstream>
#include <string>

namespace ieompp
{
    namespace symbolic
    {
        static constexpr int IndexTypeSpace    = 0;
        static constexpr int IndexTypeMomentum = 1;
        static constexpr int IndexTypeSpin     = 2;

        template <typename ValueT, int IndexType>
        struct Index {
            using Value = ValueT;

            static constexpr int type = IndexType;
            Value value;

            bool operator==(const Index& rhs) const { return value == rhs.value; }
            bool operator!=(const Index& rhs) const { return value != rhs.value; }
        };

        template <typename Value>
        using SpaceIndex = Index<Value, IndexTypeSpace>;

        template <typename Value>
        using MomentumIndex = Index<Value, IndexTypeMomentum>;

        template <typename Value>
        using SpinIndex = Index<Value, IndexTypeSpin>;

        template <typename IndexT>
        struct is_space_index {
            using Index                 = IndexT;
            static constexpr bool value = (Index::type == IndexTypeSpace);
        };

        template <typename IndexT>
        struct is_momentum_index {
            using Index                 = IndexT;
            static constexpr bool value = (Index::type == IndexTypeMomentum);
        };

        template <typename IndexT>
        struct is_spin_index {
            using Index                 = IndexT;
            static constexpr bool value = (Index::type == IndexTypeSpin);
        };

        template <typename Value>
        std::ostream& operator<<(std::ostream& strm, const SpaceIndex<Value>& index)
        {
            strm << "r_" << index.value;
            return strm;
        }

        template <typename Value>
        std::ostream& operator<<(std::ostream& strm, const MomentumIndex<Value>& index)
        {
            strm << "k_" << index.value;
            return strm;
        }

        template <typename Value>
        std::ostream& operator<<(std::ostream& strm, const SpinIndex<Value>& index)
        {
            strm << u8"σ_" << index.value;
            return strm;
        }

        template <typename Value, int IndexType>
        std::string str(const Index<Value, IndexType>& index)
        {
            std::ostringstream strm;
            strm << index;
            return strm.str();
        }
    }
}

#endif
