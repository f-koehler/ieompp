#ifndef IEOMPP_IO_ALGEBRA_HPP_
#define IEOMPP_IO_ALGEBRA_HPP_

#include "ieompp/algebra/operator.hpp"
#include "ieompp/algebra/term.hpp"
#include "ieompp/algebra/agenda.hpp"
#include "ieompp/io/fundamental.hpp"

namespace ieompp
{
    namespace io
    {
        template <typename OStream, typename Index, typename Spin>
        inline void write_binary(OStream& strm, const algebra::Operator<Index, Spin>& op)
        {
            write_binary(strm, op.creator);
            write_binary(strm, op.index);
            write_binary(strm, op.spin);
        }

        template <typename OStream, typename Operator, typename Prefactor>
        inline void write_binary(OStream& strm, const algebra::Term<Operator, Prefactor>& t)
        {
            write_binary(strm, t.prefactor);
            write_binary(strm, t.operators.size());
            for(auto& op : t.operators) write_binary(strm, op);
        }

        template <typename OStream, typename Term>
        void write_binary(OStream& strm, const algebra::Agenda<Term>& agenda)
        {
            write_binary(strm, agenda.terms().size());
            for(auto& term : agenda.terms()) write_binary(strm, term);
            for(auto& known : agenda.known()) write_binary(strm, known);
            write_binary(strm, agenda.todo().size());
            for(auto& todo : agenda.todo()) write_binary(strm, todo);
            write_binary(strm, agenda.results().size());
            for(auto& row : agenda.results()) {
                write_binary(strm, row.size());
                for(auto& entry : row) {
                    write_binary(strm, entry.index);
                    write_binary(strm, entry.prefactor);
                }
            }
        }

        template <typename IStream, typename Index, typename Spin>
        inline void read_binary(IStream& strm, algebra::Operator<Index, Spin>& op)
        {
            read_binary(strm, op.creator);
            read_binary(strm, op.index);
            read_binary(strm, op.spin);
        }

        template <typename IStream, typename Operator, typename Prefactor>
        inline void read_binary(IStream& strm, algebra::Term<Operator, Prefactor>& t)
        {
            read_binary(strm, t.prefactor);
            auto size = t.operators.size();
            read_binary(strm, size);
            t.operators.resize(size);
            for(auto& op : t.operators) read_binary(strm, op);
        }

        template <typename IStream, typename Term>
        void read_binary(IStream& strm, algebra::Agenda<Term>& agenda) {
            std::vector<Term> terms;
            auto size = terms.size();
            read_binary(strm, size);
            terms.resize(size);

            for(auto& term : terms) read_binary(strm, term);

            std::list<std::size_t> known;
            known.resize(size);
            for(auto& k : known) read_binary(strm, k);

            read_binary(strm, size);
            std::vector<std::size_t> todo(size);
            for(auto& t : todo) read_binary(strm, t);

            read_binary(strm, size);
            std::vector<std::vector<typename algebra::Agenda<Term>::Entry>> results(size);
            for(auto& row : results) {
                read_binary(strm, size);
                row.resize(size);
                for(auto& e : row) {
                    read_binary(strm, e.index);
                    read_binary(strm, e.prefactor);
                }
            }

            agenda = algebra::Agenda<Term>(terms, known, todo, results);
        }
    }
}

#endif
