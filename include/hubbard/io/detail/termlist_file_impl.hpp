#include "hubbard/io/termlist_file.hpp"

namespace hubbard
{
    namespace io
    {
        template <typename Term>
        TermListFile<Term>::TermListFile(const std::string& path, bool binary)
            : _path(path), _binary(binary)
        {
        }

        template <typename Term>
        void TermListFile<Term>::write(const TermList& terms) const
        {
            std::ofstream strm(
                _path, _binary ? (std::ios_base::out | std::ios_base::binary | std::ios_base::trunc)
                               : (std::ios_base::out | std::ios_base::trunc));
            if(_binary)
                for(auto& term : terms) write_term_binary(strm, term);
            else
                for(auto& term : terms) write_term(strm, term);
            strm.close();
        }

        template <typename Term>
        void TermListFile<Term>::append(const TermList& terms) const
        {
            std::ofstream strm(
                _path, _binary ? (std::ios_base::out | std::ios_base::binary | std::ios_base::app)
                               : (std::ios_base::out | std::ios_base::app));
            if(_binary)
                for(auto& term : terms) write_term_binary(strm, term);
            else
                for(auto& term : terms) write_term(strm, term);
            strm.close();
        }

        template <typename Term>
        typename TermListFile<Term>::TermList TermListFile<Term>::read() const
        {
            std::ifstream strm(_path, _binary ? (std::ios_base::in | std::ios_base::binary)
                                              : std::ios_base::in);
            TermList terms;
            if(_binary) {
                while(!strm.eof()) {
                    Term t;
                    read_term_binary(strm, t);
                    terms.push_back(t);
                }
            } else {
                while(!strm.eof()) {
                    Term t;
                    read_term(strm, t);
                    terms.push_back(t);
                }
                terms.erase(terms.end() - 1);
            }
            strm.close();
            return terms;
        }

        template <typename Term>
        void TermListFile<Term>::write_term_binary(std::ofstream& strm, const Term& t)
        {
            strm << t.prefactor.real() << t.prefactor.imag() << t.operators.size();
            for(auto& op : t.operators) strm << op.creator << op.index << op.spin;
            return;
        }

        template <typename Term>
        void TermListFile<Term>::write_term(std::ofstream& strm, const Term& t)
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
        void TermListFile<Term>::read_term_binary(std::ifstream& strm, Term& t)
        {
            strm >> t.prefactor;

            Size num;
            strm >> num;
            t.operators.resize(num);

            for(Size i = 0; i < num; ++i)
                strm >> t.operators[i].creator >> t.operators[i].index >> t.operators[i].spin;
        }

        template <typename Term>
        void TermListFile<Term>::read_term(std::ifstream& strm, Term& t)
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
