#ifndef INDEX_HPP_
#define INDEX_HPP_

#include <ostream>

namespace ieompp
{
    static constexpr int SymbolicIndexTypeSpace    = 0;
    static constexpr int SymbolicIndexTypeMomentum = 1;
    static constexpr int SymbolicIndexTypeSpin     = 2;

    template <typename ValueT, int IndexType>
    struct Index {
        using Value = ValueT;

        static constexpr int type = IndexType;
        Value value;
    };

    template <typename Value>
    using SpaceIndex = Index<Value, SymbolicIndexTypeSpace>;

    template <typename Value>
    using MomentumIndex = Index<Value, SymbolicIndexTypeMomentum>;

    template <typename Value>
    using SpinIndex = Index<Value, SymbolicIndexTypeSpin>;

    template <typename IndexT>
    struct is_space_index {
        using Index                 = IndexT;
        static constexpr bool value = (Index::type == SymbolicIndexTypeSpace);
    };

    template <typename IndexT>
    struct is_momentum_index {
        using Index                 = IndexT;
        static constexpr bool value = (Index::type == SymbolicIndexTypeMomentum);
    };

    template <typename IndexT>
    struct is_spin_index {
        using Index                 = IndexT;
        static constexpr bool value = (Index::type == SymbolicIndexTypeSpin);
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
}

#endif
