#include "hubbard/algebraic.hpp"

const std::string AlgebraicSpin::Up   = "\u2191";
const std::string AlgebraicSpin::Down = "\u2193";

AlgebraicSpin::AlgebraicSpin(const std::string& s)
    : spin(s)
{}

bool AlgebraicSpin::operator==(const AlgebraicSpin& rhs) const {
    return spin == rhs.spin;
}

bool AlgebraicSpin::operator!=(const AlgebraicSpin& rhs) const {
    return spin != rhs.spin;
}

std::ostream& operator<<(std::ostream& strm, const AlgebraicSpin& spin) {
    strm << spin.spin;
    return strm;
}


AlgebraicIndex::AlgebraicIndex(const std::string& i)
    : index(i)
{}

bool AlgebraicIndex::operator==(const AlgebraicIndex& rhs) const {
    return index == rhs.index;
}

bool AlgebraicIndex::operator!=(const AlgebraicIndex& rhs) const {
    return index != rhs.index;
}

std::ostream& operator<<(std::ostream& strm, const AlgebraicIndex& index) {
    strm << index.index;
    return strm;
}
