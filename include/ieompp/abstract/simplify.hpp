#ifndef IEOMPP_SIMPLIFY_HPP_
#define IEOMPP_SIMPLIFY_HPP_

#include "ieompp/abstract/term.hpp"

namespace ieompp
{
    namespace abstract
    {

        namespace simplify
        {
            AbstractTermList& order(AbstractTermList& terms);
            AbstractTermList& join(AbstractTermList& terms);
            AbstractTermList& eval_kroneckers(AbstractTermList& terms);
            AbstractTermList& filter(AbstractTermList& terms);
        }

        AbstractTermList& simplify_terms(AbstractTermList& terms);

    }
}

#include "detail/simplify_impl.hpp"

#endif
