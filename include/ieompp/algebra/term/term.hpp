#ifndef IEOMPP_ALGEBRA_TERM_TERM_HPP_
#define IEOMPP_ALGEBRA_TERM_TERM_HPP_

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

            Prefactor prefactor;
            Container operators;

            void conjugate()
            {
                // TODO: conjugate prefactor
                std::reverse(operators.begin(), operators.end());
            }

            Term get_conjugate() const
            {
                // TODO: conjugate prefactor
                Term conj{prefactor, {}};
                std::reverse_copy(operators.begin(), operators.end(),
                                  std::back_inserter(conj.operators));
                for(auto& op : conj.operators) op.creator = !op.creator;
                return conj;
            }

            bool same_operators(const Term& rhs) const {
                if(operators.size() != rhs.operators.size()) return false;
                return std::equal(operators.begin(), operators.end(), rhs.operators.begin());
            }

            Term sub_term(std::size_t pos, std::size_t len) const {
                // TODO: only works with random access iterators
                return Term{Prefactor(1.),
                            Container(operators.begin() + pos, operators.begin() + pos + len)};
            }

            template <typename Iterator>
            Term sub_term(Iterator first, Iterator last)
            {
                return Term{Prefactor(1.), Container(first, last)};
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
                Term t(*this);
                std::copy(rhs.operators.begin(), rhs.operators.end(),
                          std::back_inserter(t.operators));
                t.prefactor *= rhs.prefactor;
                return t;
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
        struct is_term {
            static constexpr bool value = false;
        };

        template <typename Prefactor, typename Operator, typename Container>
        struct is_term<Term<Prefactor, Operator, Container>> {
            static constexpr bool value = true;
        };
    }
}


#endif
