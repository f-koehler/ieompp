#include "benchmark/benchmark.h"
#include <random>
#include "hubbard/algebra.hpp"

std::mt19937_64 prng;
std::uniform_int_distribution<int> index_dist(-5, 5);
std::uniform_int_distribution<int> spin_dist(0, 1);

static void bm_commutate(benchmark::State& state) {
    using Operator = Operator<int, bool>;
    using Term = Term<Operator>;
    while(state.KeepRunning()) {
        state.PauseTiming();
        Term t1;
        Term t2;
        for(auto i = 0; i < state.range_x(); ++i) {
            Operator op;
            if(spin_dist(prng))
                op.creator = true;
            else
                op.creator = false;
            op.index = index_dist(prng);
            op.spin = spin_dist(prng);
            t1.operators.push_back(op);
        }
        for(auto i = 0; i < state.range_y(); ++i) {
            Operator op;
            if(spin_dist(prng))
                op.creator = true;
            else
                op.creator = false;
            op.index = index_dist(prng);
            op.spin = spin_dist(prng);
            t2.operators.push_back(op);
        }
        state.ResumeTiming();
        auto result = commutate(t1, t2);
    }
}

BENCHMARK(bm_commutate)->RangePair(2,32,2,32);

BENCHMARK_MAIN()
