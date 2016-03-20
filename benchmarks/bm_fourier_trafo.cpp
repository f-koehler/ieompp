#include "benchmark/benchmark.h"
#include "ieompp/discretization/linear.hpp"
#include "ieompp/algebra/fourier.hpp"
#include "ieompp/algebra/operator.hpp"

static void transform_linear(benchmark::State& state) {
    using Discretization = ieompp::discretization::LinearDiscretization<double>;
    using Transformation = ieompp::algebra::
        Transformation<ieompp::algebra::Term<ieompp::algebra::Operator<std::size_t, bool>,
                                              std::complex<Discretization::Real>>,
                       ieompp::algebra::Term<ieompp::algebra::Operator<std::size_t, bool>,
                                              std::complex<Discretization::Real>>,
                       Discretization, Discretization>;
    state.PauseTiming();
    Discretization real_space(state.range_x(), 1.);
    Discretization fourier_space(state.range_x(), 1.);
    Transformation transform(real_space, fourier_space);
    auto term = ieompp::algebra::make_term(std::complex<Discretization::Real>(1., 0.),
                                            {ieompp::algebra::make_creator(0ul, true)});
    ieompp::algebra::TermList<Transformation::TermFourier> result;
    state.ResumeTiming();
    transform.transform(term, result);
}

BENCHMARK(transform_linear)->Range(4, 2<<16);

BENCHMARK_MAIN()
