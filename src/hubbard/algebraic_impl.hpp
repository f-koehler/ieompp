#include "hubbard/algebraic.hpp"

#include <cassert>

const std::string AlgebraicSpin::Up   = "\u2191";
const std::string AlgebraicSpin::Down = "\u2193";

AlgebraicSpin::AlgebraicSpin(const std::string& s)
    : spin(s)
{}

inline bool AlgebraicSpin::operator==(const AlgebraicSpin& rhs) const {
    return spin == rhs.spin;
}

inline bool AlgebraicSpin::operator!=(const AlgebraicSpin& rhs) const {
    return spin != rhs.spin;
}

std::ostream& operator<<(std::ostream& strm, const AlgebraicSpin& spin) {
    strm << spin.spin;
    return strm;
}


AlgebraicIndex::AlgebraicIndex(const std::string& i)
    : index(i)
{}

inline bool AlgebraicIndex::operator==(const AlgebraicIndex& rhs) const {
    return index == rhs.index;
}

inline bool AlgebraicIndex::operator!=(const AlgebraicIndex& rhs) const {
    return index != rhs.index;
}

std::ostream& operator<<(std::ostream& strm, const AlgebraicIndex& index) {
    strm << index.index;
    return strm;
}


AlgebraicPrefactor& AlgebraicPrefactor::operator*=(const AlgebraicPrefactor& rhs) {
    prefactor *= rhs.prefactor;
    std::copy(
        rhs.kroneckers.begin(),
        rhs.kroneckers.end(),
        std::back_inserter(kroneckers)
    );
    return *this;
}

AlgebraicPrefactor AlgebraicPrefactor::operator*(const AlgebraicPrefactor& rhs) const {
    AlgebraicPrefactor result(*this);
    result.prefactor *= rhs.prefactor;
    std::copy(
        rhs.kroneckers.begin(),
        rhs.kroneckers.end(),
        std::back_inserter(result.kroneckers)
    );
    return result;
}

template<typename T>
AlgebraicPrefactor& AlgebraicPrefactor::operator*=(const T& rhs) {
    prefactor *= rhs;
    return *this;
} 

template<typename T>
AlgebraicPrefactor AlgebraicPrefactor::operator*(const T& rhs) const {
    AlgebraicPrefactor result(*this);
    result.prefactor *= rhs;
    return result;
}


template<>
AlgebraicPrefactor anticommutator<AlgebraicOperator, AlgebraicPrefactor>(const AlgebraicOperator& a, const AlgebraicOperator& b) {
    AlgebraicPrefactor result;
    result.prefactor.real(1.);
    result.prefactor.imag(0.);
    if(a.spin != b.spin)
        result.kroneckers.push_back({ a.spin.spin, b.spin.spin });
    if(a.index != b.index)
        result.kroneckers.push_back({ a.index.index, b.index.index });
    return result;
}


inline AlgebraicOperator make_algebraic_creator(const std::string& index, const std::string& spin) {
    return AlgebraicOperator{ true, AlgebraicIndex{ index }, AlgebraicSpin{ spin } };
}

inline AlgebraicOperator make_algebraic_annihilator(const std::string& index, const std::string& spin) {
    return AlgebraicOperator{ false, AlgebraicIndex{ index }, AlgebraicSpin{ spin } };
}
