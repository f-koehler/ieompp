#ifndef SPIN_HPP_
#define SPIN_HPP_

#include <string>

namespace hubbard
{
    namespace abstract
    {

        struct AbstractSpin {
            static const std::string Up, Down;
            std::string spin;

            AbstractSpin(const std::string& spin);

            inline bool operator==(const AbstractSpin& rhs) const;
            inline bool operator!=(const AbstractSpin& rhs) const;
        };

        std::ostream& operator<<(std::ostream& strm, const AbstractSpin& spin);
    }
}

#include "hubbard/abstract/spin_impl.hpp"

#endif
