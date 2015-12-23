#ifndef HUBBARD_ABSTRACT_ALGEBRA_HPP_
#define HUBBARD_ABSTRACT_ALGEBRA_HPP_

#include "hubbard/algebra.hpp"
#include "hubbard/abstract/index.hpp"
#include "hubbard/abstract/prefactor.hpp"
#include "hubbard/abstract/spin.hpp"

using AbstractOperator = Operator<AbstractIndex, AbstractSpin>;

struct AbstractTerm : public Term<AbstractOperator, AbstractPrefactor> {
    inline void order_kroneckers();
};

using AbstractTermList = TermList<AbstractTerm>;

inline AbstractOperator make_abstract_creator(const std::string& index, const std::string& spin);
inline AbstractOperator make_abstract_annihilator(const std::string& index,
                                                  const std::string& spin);

AbstractTermList& simplify(AbstractTermList& terms);

#include "hubbard/abstract/algebra_impl.hpp"

#endif
