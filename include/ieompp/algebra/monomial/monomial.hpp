#ifndef IEOMPP_ALGEBRA_MONOMIAL_MONOMIAL_HPP_
#define IEOMPP_ALGEBRA_MONOMIAL_MONOMIAL_HPP_

#include <algorithm>
#include <vector>

namespace ieompp
{
    namespace algebra
    {
        template <typename OperatorT>
        struct Monomial : public std::vector<OperatorT> {
            using Operator = OperatorT;

            // make Monomial construtible in the same manner as std::vector
            using std::vector<Operator>::vector;

            void conjugate()
            {
                std::reverse(this->begin(), this->end());
                for(auto& op : *this) {
                    op.conjugate();
                }
            }

            Monomial get_conjugate() const
            {
                Monomial conj;
                std::reverse_copy(this->begin(), this->end(), std::back_inserter(conj));
                for(auto& op : conj) {
                    op.conjugate();
                }
                return conj;
            }

            bool operator==(const Monomial& rhs) const
            {
                if(this->size() != rhs.size()) {
                    return false;
                }
                return std::equal(this->begin(), this->end(), rhs.begin());
            }

            bool operator!=(const Monomial& rhs) const
            {
                if(this->size() != rhs.size()) {
                    return true;
                }
                return !std::equal(this->begin(), this->end(), rhs.begin());
            }

            Monomial& operator*=(const Monomial& rhs)
            {
                std::copy(rhs.begin(), rhs.end(), std::back_inserter(*this));
                return *this;
            }

            Monomial operator*(const Monomial& rhs) const
            {
                Monomial prod(*this);
                std::copy(rhs.begin(), rhs.end(), std::back_inserter(prod));
                return prod;
            }
        };
    } // namespace algebra
} // namespace ieompp

#endif
