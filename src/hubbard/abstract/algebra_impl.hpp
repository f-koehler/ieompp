#include "hubbard/abstract/algebra.hpp"

template<>
AbstractPrefactor anticommutator<AbstractOperator, AbstractPrefactor>(const AbstractOperator& a, const AbstractOperator& b) {
    AbstractPrefactor result;
    result.prefactor.real(1.);
    result.prefactor.imag(0.);
    if(a.index != b.index)
        result.kroneckers.push_back({ a.index.index, b.index.index });
    if(a.spin != b.spin)
        result.kroneckers.push_back({ a.spin.spin, b.spin.spin });
    return result;
}


inline AbstractOperator make_algebraic_creator(const std::string& index, const std::string& spin) {
    return AbstractOperator{ true, AbstractIndex{ index }, AbstractSpin{ spin } };
}

inline AbstractOperator make_algebraic_annihilator(const std::string& index, const std::string& spin) {
    return AbstractOperator{ false, AbstractIndex{ index }, AbstractSpin{ spin } };
}
