#ifndef IEOMPP_IEOM_SYSTEM_HPP_
#define IEOMPP_IEOM_SYSTEM_HPP_

#include <ostream>
#include <vector>

namespace ieompp
{
    namespace ieom
    {
        template <typename TermT, typename ComplexT>
        struct DynamicalSystem
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

        template <typename Term, typename Complex>
        std::ostream& operator<<(std::ostream& strm, const DynamicalSystem<Term, Complex>& system)
        {
            strm << "terms:" << std::endl;
            for(auto& term : system.terms) strm << term << std::endl;
            strm << std::endl << std::endl;

            strm << "results:" << std::endl;
            std::size_t i = 0;
            for(auto& line : system.coefficients) {
                strm << system.terms[i] << ":" << std::endl;
                for(auto& entry : line) {
                    auto term      = system.terms[entry.index];
                    term.prefactor = entry.prefactor;
                    strm << "\t" << term << std::endl;
                }
                ++i;
                strm << std::endl;
            }
            return strm;
        }
    }
}

#endif
