#ifndef IEOMPP_OPENMP_HPP_
#define IEOMPP_OPENMP_HPP_

#ifdef _OPENMP

#include <omp.h>

#else

#define omp_get_thread_num() 0
#define omp_get_max_threads() 1
#define omp_get_num_threads() 0

#endif

#endif
