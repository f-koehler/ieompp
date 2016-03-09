#include "hubbard/io/termlist_file.hpp"

namespace hubbard
{
    namespace io
    {
        template <typename Term>
        TermListFile<Term>::TermListFile(const std::string& path)
            : _path(path)
        {
        }

        template <typename Term>
        void TermListFile<Term>::write(const TermList& terms) const
        {
            std::ofstream strm(_path, std::ios_base::out | std::ios_base::trunc);
            for(auto& term : terms) write_term(strm, term);
            strm.close();
        }

        template <typename Term>
        typename TermListFile<Term>::TermList TermListFile<Term>::read() const
        {
            std::ifstream strm(_path, std::ios_base::in);
            TermList terms;
            while(!strm.eof()) {
                Term t;
                read_term(strm, t);
                terms.push_back(t);
            }
            terms.erase(terms.end() - 1);
            strm.close();
            return terms;
        }

        template <typename Term>
        void TermListFile<Term>::write_term(std::ostream& strm, const Term& t)
        {
            strm << t.prefactor << ' ' << t.operators.size() << ' ';
            for(auto& op : t.operators) {
                if(op.creator)
                    strm << "c(";
                else
                    strm << "a(";
                strm << op.index << ',' << op.spin << ") ";
            }
            strm << std::endl;
        }

        template <typename Term>
        void TermListFile<Term>::read_term(std::istream& strm, Term& t)
        {
            std::string buffer;
            std::getline(strm, buffer);
            std::istringstream ss(buffer);

            ss >> t.prefactor;

            Size num;
            ss >> num;
            t.operators.resize(num);

            for(Size i = 0; i < num; ++i) {
                while(ss.peek() == ' ') ss.get();
                t.operators[i].creator = (ss.get() == 'c');
                ss.get();

                ss >> t.operators[i].index;
                ss.get();
                ss >> t.operators[i].spin;
                ss.get();
            }
        }
    }
}
