#include "ieompp/abstract/commutator.hpp"

namespace ieompp
{
    namespace algebra
    {
        template <>
        bool anticommutates<abstract::AbstractOperator>(const abstract::AbstractOperator& a,
                                                        const abstract::AbstractOperator& b)
        {
            if(a.creator == b.creator) return true;
            return false;
        }

        template <>
        abstract::AbstractPrefactor
        anticommutator<abstract::AbstractOperator, abstract::AbstractPrefactor>(
            const abstract::AbstractOperator& a, const abstract::AbstractOperator& b)
        {
            abstract::AbstractPrefactor result;
            result.number.real(1.);
            result.number.imag(0.);
            if(a.index != b.index) result.kroneckers.push_back({a.index, b.index});
            if(a.spin != b.spin) result.kroneckers.push_back({a.spin, b.spin});
            return result;
        }
    }
}
