#include "hubbard/abstract/index.hpp"

namespace hubbard
{
    namespace abstract
    {
        AbstractIndex::AbstractIndex(const std::string& i) : index(i) {}

        inline bool AbstractIndex::operator==(const AbstractIndex& rhs) const
        {
            return index == rhs.index;
        }

        inline bool AbstractIndex::operator!=(const AbstractIndex& rhs) const
        {
            return index != rhs.index;
        }

        std::ostream& operator<<(std::ostream& strm, const AbstractIndex& index)
        {
            strm << index.index;
            return strm;
        }
    }
}
