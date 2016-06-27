#ifndef IEOMPP_AGENDA_HPP_
#define IEOMPP_AGENDA_HPP_

#include <algorithm>
#include <functional>
#include <list>
#include <ostream>
#include <tuple>
#include <vector>

#include <ieompp/algebra/term_comparison.hpp>

namespace ieompp
{

    template <typename TermT>
    class Agenda
    {
        using Term      = TermT;
        using Prefactor = typename Term::Prefactor;

        template <typename Container = std::vector<Term>>
        using Generator              = std::function<void(const Term&, Container&)>;

        struct Coefficient
        {
            std::size_t index;
            Prefactor prefactor;
        };

        private:
            std::vector<Term> _terms;
            std::list<std::size_t> _known;
            std::vector<std::size_t> _todo;
            std::vector<std::vector<Coefficient>> _coefficients;

        public:
            void reset()
            {
                _terms.clear();
                _known.clear();
                _todo.clear();
                _coefficients.clear();
            }

            std::tuple<bool, typename decltype(_known)::const_iterator> is_known(const Term& term)
            {
                static const algebra::TermSmaller<Term> term_smaller{};

                auto pos   = _known.begin();
                bool found = false;
                for(; pos != _known.end(); ++pos) {
                    const auto& other = _terms[*pos];
                    if(term.same_operators(other)) {
                        found = true;
                        break;
                    }
                    if(term_smaller(term, other)) break;
                }

                return std::make_tuple(found, pos);
            }

            std::size_t add_new_term(const Term& term,
                                 typename decltype(_known)::const_iterator position_in_known_list)
            {
                const auto new_pos = _terms.size();
                _terms.push_back(term);
                _known.insert(position_in_known_list, new_pos);
                _todo.push_back(new_pos);
                _coefficients.push_back(std::vector<Coefficient>());
                return new_pos;
            }

            template <typename Container = std::vector<Term>>
            void commutate(const Term& term, const std::size_t num_commutations,
                           const Generator<Container>& gen)
            {
                reset();
                add_new_term(term, _known.begin());
                commutate(num_commutations, gen);
            }

            template <typename Container>
            void commutate(const std::size_t num_commutations, const Generator<Container>& gen)
            {
                if(num_commutations == 0) return;
                if(_todo.empty()) return;

                const bool last_commutation = (num_commutations == 1);

                std::vector<std::size_t> current_todo;
                current_todo.swap(_todo);

                for(auto current_index : current_todo) {
                    const auto current = _terms[current_index];

                    Container generated;
                    gen(current, generated);

                    for(const auto& new_term : generated) {
                        const auto ret       = is_known(new_term);
                        const auto known     = std::get<0>(ret);
                        auto insert_position = std::get<1>(ret);

                        if(!known) {
                            if(last_commutation) continue;
                            const auto new_index = add_new_term(new_term, insert_position);
                            _coefficients[current_index].emplace_back(
                                Coefficient{new_index, new_term.prefactor});
                            continue;
                        }

                        auto& current_coefficients = _coefficients[current_index];
                        auto index                 = *insert_position;

                        auto find = std::find_if(
                            current_coefficients.begin(), current_coefficients.end(),
                            [&index](const Coefficient& c) { return c.index == index; });

                        if(find != current_coefficients.end())
                            find->prefactor += new_term.prefactor;
                        else
                            current_coefficients.emplace_back(
                                Coefficient{index, new_term.prefactor});
                    }
                }

                commutate(num_commutations - 1, gen);
            }

            const std::vector<Term>& terms() const { return _terms; }
            const std::vector<std::size_t>& todo() const { return _todo; }
            const std::vector<Coefficient>& operator[](std::size_t i) const
            {
                return _coefficients[i];
            }
    };

    template <typename Term>
    std::ostream& operator<<(std::ostream& strm, const Agenda<Term>& agenda)
    {
        auto& terms = agenda.terms();
        auto& todo  = agenda.todo();

        strm << "terms:" << std::endl;
        for(auto& t : terms) strm << "\t" << t << std::endl;

        strm << std::endl << "terms todo:" << std::endl;
        for(auto& t : todo) strm << "\t" << terms[t] << std::endl;

        for(std::size_t i = 0; i < terms.size(); ++i) {
            strm << std::endl << "coefficients for " << terms[i] << std::endl;
            auto& coeffs = agenda[i];
            for(auto& coeff : coeffs)
                strm << "\t" << coeff.prefactor << "\t" << terms[coeff.index] << std::endl;
        }

        return strm;
    }
}

#endif
