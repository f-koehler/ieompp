#include "hubbard/abstract/prefactor.hpp"

namespace hubbard
{
    namespace abstract
    {
        inline void Kronecker::order_indices()
        {
            if(left < right) return;
            std::swap(left, right);
        }

        inline bool Kronecker::operator==(const Kronecker& rhs) const
        {
            return ((left == rhs.left)  && (right == rhs.right))
                || ((left == rhs.right) && (right == rhs.left));
        }

        inline bool Kronecker::operator!=(const Kronecker& rhs) const
        {
            return ((left != rhs.left)  || (right != rhs.right))
                && ((left != rhs.right) || (right != rhs.left));
        }

        std::ostream& operator<<(std::ostream& strm, const Kronecker& rhs)
        {
            strm << "\u03B4_{" << rhs.left << ", " << rhs.right << "}";
            return strm;
        }


        inline bool AbstractPrefactor::same_kroneckers(const AbstractPrefactor& rhs) const
        {
            return (kroneckers.size() == rhs.kroneckers.size())
                || std::equal(kroneckers.begin(), kroneckers.end(), rhs.kroneckers.begin());
        }

        AbstractPrefactor& AbstractPrefactor::operator*=(const AbstractPrefactor& rhs)
        {
            prefactor *= rhs.prefactor;
            std::copy(rhs.kroneckers.begin(), rhs.kroneckers.end(), std::back_inserter(kroneckers));
            return *this;
        }

        AbstractPrefactor AbstractPrefactor::operator*(const AbstractPrefactor& rhs) const
        {
            AbstractPrefactor result(*this);
            result.prefactor *= rhs.prefactor;
            std::copy(rhs.kroneckers.begin(), rhs.kroneckers.end(),
                      std::back_inserter(result.kroneckers));
            return result;
        }

        template <typename T>
        AbstractPrefactor& AbstractPrefactor::operator*=(const T& rhs)
        {
            prefactor *= rhs;
            return *this;
        }

        template <typename T>
        AbstractPrefactor AbstractPrefactor::operator*(const T& rhs) const
        {
            AbstractPrefactor result(*this);
            result.prefactor *= rhs;
            return result;
        }

        inline bool AbstractPrefactor::operator==(const AbstractPrefactor& rhs) const
        {
            return (prefactor == rhs.prefactor) && (kroneckers.size() == rhs.kroneckers.size())
                   && std::equal(kroneckers.begin(), kroneckers.end(), rhs.kroneckers.begin());
        }

        inline bool AbstractPrefactor::operator!=(const AbstractPrefactor& rhs) const
        {
            return (prefactor != rhs.prefactor) || (kroneckers.size() != rhs.kroneckers.size())
                   || !std::equal(kroneckers.begin(), kroneckers.end(), rhs.kroneckers.begin());
        }

        std::ostream& operator<<(std::ostream& strm, const AbstractPrefactor& rhs)
        {
            strm << rhs.prefactor;
            if(!rhs.kroneckers.empty()) {
                strm << "\u22C5";
                for(auto& k : rhs.kroneckers) strm << k;
            }
            return strm;
        }
    }
}
