#include "benchmark/benchmark.h"
#include "hubbard/discretization/linear.hpp"
#include "hubbard/algebra/fourier.hpp"
#include "hubbard/algebra/operator.hpp"

static void transform_linear(benchmark::State& state) {
    using Discretization = hubbard::discretization::LinearDiscretization<double>;
    using Transformation = hubbard::algebra::
        Transformation<hubbard::algebra::Term<hubbard::algebra::Operator<std::size_t, bool>,
                                              std::complex<Discretization::Real>>,
                       hubbard::algebra::Term<hubbard::algebra::Operator<std::size_t, bool>,
                                              std::complex<Discretization::Real>>,
                       Discretization, Discretization>;
    state.PauseTiming();
    Discretization real_space(state.range_x(), 1.);
    Discretization fourier_space(state.range_x(), 1.);
    Transformation transform(real_space, fourier_space);
    auto term = hubbard::algebra::make_term(std::complex<Discretization::Real>(1., 0.),
                                            {hubbard::algebra::make_creator(0ul, true)});
    hubbard::algebra::TermList<Transformation::TermFourier> result;
    state.ResumeTiming();
    transform.transform(term, result);
}

BENCHMARK(transform_linear)->Range(4, 2<<16);

BENCHMARK_MAIN()
