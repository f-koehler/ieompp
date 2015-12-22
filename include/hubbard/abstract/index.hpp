#ifndef HUBBARD_ABSTRACT_INDEX_HPP_
#define HUBBARD_ABSTRACT_INDEX_HPP_

#include <string>

struct AbstractIndex {
    std::string index;

    AbstractIndex(const std::string& index);

    inline bool operator==(const AbstractIndex& rhs) const;
    inline bool operator!=(const AbstractIndex& rhs) const;
};

std::ostream& operator<<(std::ostream& strm, const AbstractIndex& index);

#include "hubbard/abstract/index_impl.hpp"

#endif
