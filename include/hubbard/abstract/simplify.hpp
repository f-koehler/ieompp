#ifndef HUBBARD_SIMPLIFY_HPP_
#define HUBBARD_SIMPLIFY_HPP_

#include "hubbard/abstract/algebra.hpp"

namespace hubbard
{
    namespace abstract
    {

        namespace simplify
        {
            inline AbstractTermList& order(AbstractTermList& terms);
            inline AbstractTermList& join(AbstractTermList& terms);
            inline AbstractTermList& eval_kroneckers(AbstractTermList& terms);
            inline AbstractTermList& filter(AbstractTermList& terms);
        }

        inline AbstractTermList& simplify_terms(AbstractTermList& terms);

    }
}

#include "hubbard/abstract/simplify_impl.hpp"

#endif
