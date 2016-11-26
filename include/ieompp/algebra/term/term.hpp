#ifndef IEOMPP_ALGEBRA_TERM_TERM_HPP_
#define IEOMPP_ALGEBRA_TERM_TERM_HPP_

#include "ieompp/types/number.hpp"

#include <algorithm>
#include <vector>

namespace ieompp
{
    namespace algebra
    {
        template <typename PrefactorT, typename OperatorT,
                  typename ContainerT = std::vector<OperatorT>>
        struct Term {
            using Prefactor = PrefactorT;
            using Operator  = OperatorT;
            using Container = ContainerT;

            Prefactor prefactor{};
            Container operators;

            void conjugate()
            {
                prefactor = types::conjugate(prefactor);
                std::reverse(operators.begin(), operators.end());
                for(auto& op : *this) {
                    op.conjugate();
                }
            }

            Term get_conjugate() const
            {
                Term conj{types::conjugate(prefactor), {}};
                std::reverse_copy(operators.begin(), operators.end(),
                                  std::back_inserter(conj.operators));
                for(auto& op : conj.operators) {
                    op.conjugate();
                }
                return conj;
            }

            bool have_same_operators(const Term& rhs) const
            {
                if(operators.size() != rhs.operators.size()) {
                    return false;
                }
                return std::equal(operators.begin(), operators.end(), rhs.operators.begin());
            }

            Term& operator*=(const Term& rhs)
            {
                prefactor *= rhs.prefactor;
                std::copy(rhs.operators.begin(), rhs.operators.end(),
                          std::back_inserter(operators));
                return *this;
            }

            Term operator*(const Term& rhs) const
            {
                Term prod(*this);
                std::copy(rhs.operators.begin(), rhs.operators.end(),
                          std::back_inserter(prod.operators));
                prod.prefactor *= rhs.prefactor;
                return prod;
            }
        };

        template <typename Prefactor, typename Operator, typename Container = std::vector<Operator>>
        Term<Prefactor, Operator> make_term(Prefactor&& prefactor,
                                            std::initializer_list<Operator> operators)
        {
            return Term<Prefactor, Operator>{
                typename std::decay<Prefactor>::type(std::forward<Prefactor>(prefactor)),
                Container{operators}};
        }

        template <typename T>
        struct IsTerm {
            static constexpr bool value = false;
        };

        template <typename Prefactor, typename Operator, typename Container>
        struct IsTerm<Term<Prefactor, Operator, Container>> {
            static constexpr bool value = true;
        };
    } // namespace algebra
} // namespace ieompp


#endif
