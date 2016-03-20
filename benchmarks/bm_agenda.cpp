#include "benchmark/benchmark.h"

#include "ieompp/discretization/linear.hpp"
#include "ieompp/algebra/hamiltonian.hpp"
#include "ieompp/algebra/agenda.hpp"

using namespace ieompp;

static void bm_agenda(benchmark::State& state)
{
    while(state.KeepRunning()) {
        state.PauseTiming();
        auto term =
            algebra::make_term(std::complex<double>(1., 0.), {algebra::make_creator(0ul, true)});
        discretization::LinearDiscretization<double> discretization(100, 1.);
        algebra::Hamiltonian<decltype(term)> hamiltonian;
        algebra::Agenda<decltype(term)> agenda;
        state.ResumeTiming();

        agenda.commutate(term, state.range_x(), hamiltonian, discretization);
    }
}

static void bm_agenda_fourier(benchmark::State& state)
{
    while(state.KeepRunning()) {
        state.PauseTiming();
        auto term =
            algebra::make_term(std::complex<double>(1., 0.), {algebra::make_creator(0ul, true)});
        discretization::LinearDiscretization<double> discretization(100, 1.);
        algebra::Hamiltonian<decltype(term)> hamiltonian;
        algebra::Agenda<decltype(term)> agenda;
        state.ResumeTiming();

        agenda.commutate(term, state.range_x(), hamiltonian, discretization);
    }
}

BENCHMARK(bm_agenda)->Arg(1)->Arg(2)->Arg(3)->Arg(4);
BENCHMARK(bm_agenda_fourier)->Arg(1)->Arg(2)->Arg(3)->Arg(4);
BENCHMARK_MAIN()
