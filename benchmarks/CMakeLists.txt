include(ExternalProject)
ExternalProject_Add(
    project_benchmark
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/benchmark
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_SOURCE_DIR}/external/benchmark/dist -DCMAKE_BUILD_TYPE=Release
)

include_directories(${CMAKE_SOURCE_DIR}/external/benchmark/include)

set(benchmarks commutate)

foreach(bm ${benchmarks})
    add_executable(${bm} "bm_${bm}.cpp")
    target_link_libraries(${bm} ${CMAKE_SOURCE_DIR}/external/benchmark/dist/lib/libbenchmark.a -lpthread)
endforeach()
