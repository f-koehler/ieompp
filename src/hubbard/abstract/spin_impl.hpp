#include "hubbard/abstract/spin.hpp"

namespace hubbard
{
    namespace abstract
    {

        const std::string AbstractSpin::Up   = "\u2191";
        const std::string AbstractSpin::Down = "\u2193";

        AbstractSpin::AbstractSpin(const std::string& s) : spin(s) {}

        inline bool AbstractSpin::operator==(const AbstractSpin& rhs) const { return spin == rhs.spin; }

        inline bool AbstractSpin::operator!=(const AbstractSpin& rhs) const { return spin != rhs.spin; }

        std::ostream& operator<<(std::ostream& strm, const AbstractSpin& spin)
        {
            strm << spin.spin;
            return strm;
        }
    }
}
