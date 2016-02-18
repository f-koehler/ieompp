include(ExternalProject)
ExternalProject_Add(
    google_benchmark
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/benchmark
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/google_benchmark -DCMAKE_BUILD_TYPE=Release
)

include_directories(${CMAKE_SOURCE_DIR}/external/benchmark/include)

set(benchmarks commutate)

foreach(bm ${benchmarks})
    add_executable(${bm} "bm_${bm}.cpp")
    add_dependencies(${bm} google_benchmark)
    target_link_libraries(${bm} ${CMAKE_BINARY_DIR}/google_benchmark/lib/libbenchmark.a -lpthread)
endforeach()
