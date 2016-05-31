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

        template <int IndexType>
        struct default_symbol;

        template <>
        struct default_symbol<IndexTypeSpace> {
            static const std::string symbol;
        };

        template <>
        struct default_symbol<IndexTypeMomentum> {
            static const std::string symbol;
        };

        template <>
        struct default_symbol<IndexTypeSpin> {
            static const std::string symbol;
        };

        const std::string default_symbol<IndexTypeSpace>::symbol("r");
        const std::string default_symbol<IndexTypeMomentum>::symbol("k");
        const std::string default_symbol<IndexTypeSpin>::symbol(u8"σ");

        template <typename ValueT, int IndexType>
        struct Index {
            using Value = ValueT;

            static constexpr int type = IndexType;

            Value value;
            std::string symbol;

            Index() : value(0), symbol(default_symbol<IndexType>::symbol) {}
            Index(const Value& val, const std::string& sym = default_symbol<IndexType>::symbol)
                : value(val), symbol(sym)
            {
            }

            bool operator==(const Index& rhs) const
            {
                return (value == rhs.value) && (symbol == rhs.symbol);
            }
            bool operator!=(const Index& rhs) const
            {
                return (value != rhs.value) || (symbol != rhs.symbol);
            }
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
            strm << index.symbol << "_" << index.value;
            return strm;
        }

        template <typename Value>
        std::ostream& operator<<(std::ostream& strm, const SpinIndex<Value>& index)
        {
            strm << index.symbol << "_" << index.value;
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
