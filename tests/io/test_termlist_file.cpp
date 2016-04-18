#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/algebra/operator.hpp"
#include "ieompp/io/termlist_file.hpp"
#include <random>
#include <functional>

using namespace ieompp;

TEST_CASE("read & write")
{
    const std::size_t num_terms = 2 << 15;
    std::mt19937_64 prng;
    auto num     = std::bind(std::uniform_int_distribution<std::size_t>(0, 10), std::ref(prng));
    auto creator = std::bind(std::uniform_int_distribution<std::size_t>(0, 1), std::ref(prng));
    auto index   = std::bind(std::uniform_int_distribution<std::size_t>(0, 1000), std::ref(prng));
    auto spin    = std::bind(std::uniform_int_distribution<std::size_t>(0, 1), std::ref(prng));
    auto real    = std::bind(std::uniform_real_distribution<double>(-10., 10.), std::ref(prng));

    using Complex  = std::complex<double>;
    using Operator = algebra::Operator<std::size_t, bool>;
    using Term     = algebra::Term<Operator, Complex>;
    algebra::TermList<Term> out;
    out.reserve(num_terms);

    for(std::size_t i = 0; i < num_terms; ++i) {
        out.push_back(
            algebra::make_term(Complex(real(prng), real(prng)), std::initializer_list<Operator>()));
        auto& t = out.back();
        t.operators.resize(num());
        for(auto& op : t.operators) {
            op.creator = creator();
            op.index   = index();
            op.spin    = spin();
        }
    }

    io::TermListFile<Term> file("plain_text.txt");
    file.write(out);
    auto in = file.read();

    REQUIRE(in.size() == num_terms);
    for(std::size_t i = 0; i < num_terms; ++i) {
        assert(out[i].same_operators(in[i]));
    }
}
