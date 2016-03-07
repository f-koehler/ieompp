#ifndef HUBBARD_IO_TERMLIST_FILE_HPP_
#define HUBBARD_IO_TERMLIST_FILE_HPP_

#include <fstream>
#include <string>

#include "hubbard/algebra/term.hpp"

namespace hubbard
{
    namespace io
    {
        template <typename Term>
        void write(const std::string& path, const algebra::TermList<Term>& terms)
        {
            std::ofstream file(path.c_str(), std::ios_base::out | std::ios_base::binary);
            for(auto& term : terms) {
                file << term.prefactor;
                file << term.operators.size();
                for(auto& op : term.operators) {
                    file << op.creator << op.index << op.spin;
                }
            }
            file.flush();
            file.close();
        }
        
        template <typename Term>
        void read(const std::string& path, algebra::TermList<Term>& terms)
        {
            std::ifstream file(path.c_str(), std::ios_base::in | std::ios_base::binary);
            while(!file.eof()) {
                Term t;
                file >> t.prefactor;

                std::size_t num_ops;
                file >> num_ops;

                for(std::size_t i = 0; i < num_ops; ++i) {
                    typename Term::Operator op;
                    file >> op.creator >> op.index >> op.spin;
                    t.operators.push_back(op);
                }
                terms.push_back(t);
            }
            file.close();
        }
    }
}

#endif
