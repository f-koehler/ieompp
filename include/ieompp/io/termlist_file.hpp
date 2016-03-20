#ifndef IEOMPP_IO_TERMLIST_FILE_HPP_
#define IEOMPP_IO_TERMLIST_FILE_HPP_

#include "ieompp/algebra/term.hpp"
#include <fstream>
#include <sstream>

namespace ieompp
{
    namespace io
    {
        template <typename TermT>
        class TermListFile
        {
            using Term      = TermT;
            using TermList  = algebra::TermList<Term>;
            using Size      = typename TermList::size_type;
            using Prefactor = typename Term::Prefactor;
            using Real      = typename Prefactor::value_type;
            using Operator  = typename Term::Operator;
            using Index     = typename Operator::Index;
            using Spin      = typename Operator::Spin;

            protected:
                std::string _path;

            public:
                TermListFile(const std::string& path);

                virtual void write(const TermList& terms) const;
                virtual TermList read() const;

            private:
                static void write_term(std::ostream& strm, const Term& t);
                static void read_term(std::istream& strm, Term& t);
        };
    }
}

#include "detail/termlist_file_impl.hpp"

#endif
