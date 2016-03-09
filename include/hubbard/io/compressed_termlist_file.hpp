#ifndef HUBBARD_IO_COMPRESSED_TERMLIST_FILE_HPP_
#define HUBBARD_IO_COMPRESSED_TERMLIST_FILE_HPP_

#if HUBBARD_HAS_BOOST

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/traits.hpp>

#include "hubbard/io/termlist_file.hpp"

namespace hubbard
{
    namespace io
    {
        template <typename TermT>
        class CompressedTermListFile : public TermListFile<TermT>
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
                using TermListFile<Term>::_path;

            public:
                using TermListFile<Term>::TermListFile;

                void write(const TermList& terms) const
                {
                    boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
                    in.push(boost::iostreams::gzip_decompressor());
                }
                /* TermList read() const {} */
        };
    }
}

#endif

#endif
