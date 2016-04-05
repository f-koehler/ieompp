#ifndef IEOMPP_IEOM_SYSTEM_HPP_
#define IEOMPP_IEOM_SYSTEM_HPP_

#include <vector>

namespace ieompp
{
    namespace ieom
    {
        template <typename TermT, typename ComplexT>
        struct System
        {
            using Term    = TermT;
            using Complex = ComplexT;
            using Index   = std::size_t;

            struct Coefficient {
                Index index;
                Complex prefactor;
            };

            std::vector<Term> terms;
            std::vector<std::vector<Coefficient>> coefficients;
        };
    }
}

#endif
