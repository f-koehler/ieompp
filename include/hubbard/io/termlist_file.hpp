#ifndef HUBBARD_IO_TERMLIST_FILE_HPP_
#define HUBBARD_IO_TERMLIST_FILE_HPP_

#include "hubbard/algebra/term.hpp"
#include <fstream>
#include <sstream>

namespace hubbard
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

            private:
                std::string _path;
                bool _binary;

            public:
                TermListFile(const std::string& path, bool binary);

                void write(const TermList& terms) const;
                void append(const TermList& terms) const;
                TermList read() const;

            private:
                static void write_term_binary(std::ofstream& strm, const Term& t);
                static void write_term(std::ofstream& strm, const Term& t);
                static void read_term_binary(std::ifstream& strm, Term& t);
                static void read_term(std::ifstream& strm, Term& t);
        };
    }
}

#include "detail/termlist_file_impl.hpp"

#endif
