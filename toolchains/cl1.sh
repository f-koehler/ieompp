#!/bin/bash
NICE_LVL=19
NPROC=6
COMMON_FLAGS="-pipe -m64 -isystem/net/sw/gcc/5.3.0/rft/include -L/net/sw/gcc/5.3.0/rtf/lib64"

module load gcc/5.3.0-full 
module load python/3.4.2

# sandy bridge build
mkdir build-sandybridge
cd build-sandybridge
cmake                                                      \
    -DCMAKE_C_FLAGS="${COMMON_FLAGS} -march=sandybridge"   \
    -DCMAKE_CXX_FLAGS="${COMMON_FLAGS} -march=sandybridge" \
    -DCMAKE_INSTALL_PREFIX=$HOME/sandybridge               \
    -DCMAKE_C_COMPILER=/net/sw/gcc/5.3.0/rtf/bin/gcc       \
    -DCMAKE_CXX_COMPILER=/net/sw/gcc/5.3.0/rtf/bin/g++     \
    -DCMAKE_BUILD_TYPE=Release                             \
    -Dieompp_OVERRIDE_BOOST=ON                             \
    -Dieompp_ENABLE_TESTS=OFF                              \
    -Dieompp_NATIVE_BINARIES=OFF                           \
    ..
nice -n 19 make -j${NPROC} install
cd ..

# haswell build
mkdir build-haswell
cd build-haswell
cmake                                                  \
    -DCMAKE_C_FLAGS="${COMMON_FLAGS} -march=haswell"   \
    -DCMAKE_CXX_FLAGS="${COMMON_FLAGS} -march=haswell" \
    -DCMAKE_INSTALL_PREFIX=$HOME/haswell               \
    -DCMAKE_C_COMPILER=/net/sw/gcc/5.3.0/rtf/bin/gcc   \
    -DCMAKE_CXX_COMPILER=/net/sw/gcc/5.3.0/rtf/bin/g++ \
    -DCMAKE_BUILD_TYPE=Release                         \
    -Dieompp_OVERRIDE_BOOST=ON                         \
    -Dieompp_ENABLE_TESTS=OFF                          \
    -Dieompp_NATIVE_BINARIES=OFF                       \
    ..
nice -n 19 make -j${NPROC} install
cd ..

# broadwell build
mkdir build-broadwell
cd build-broadwell
cmake                                                    \
    -DCMAKE_C_FLAGS="${COMMON_FLAGS} -march=broadwell"   \
    -DCMAKE_CXX_FLAGS="${COMMON_FLAGS} -march=broadwell" \
    -DCMAKE_INSTALL_PREFIX=$HOME/broadwell               \
    -DCMAKE_C_COMPILER=/net/sw/gcc/5.3.0/rtf/bin/gcc     \
    -DCMAKE_CXX_COMPILER=/net/sw/gcc/5.3.0/rtf/bin/g++   \
    -DCMAKE_BUILD_TYPE=Release                           \
    -Dieompp_OVERRIDE_BOOST=ON                           \
    -Dieompp_ENABLE_TESTS=OFF                            \
    -Dieompp_NATIVE_BINARIES=OFF                         \
    ..
nice -n 19 make -j${NPROC} install
cd ..
