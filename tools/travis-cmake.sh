#!/bin/bash

CMAKE_NAME="cmake-3.5.0"
CMAKE_URL="https://cmake.org/files/v3.5/cmake-3.5.0.tar.gz"
CMAKE_SHA512="4fef712560f1975f8d19e897c7b1903811f7bad9314602961b5b32ac58f367bf1308f96cc5edaceb99ac9084b1e0517e4ecb4b5996d2ad53b6aec305ae879b09  cmake-3.5.0.tar.gz"

# create a dir for cmake
mkdir -p ${CMAKE_NAME}
cd ${CMAKE_NAME}

# download cmake tarball
if [ ! -f ${CMAKE_NAME}.tar.gz ]; then
    wget --no-check-certificate ${CMAKE_URL}
fi

# verify checksum
if [ "${CMAKE_SHA512}" != "$(sha512sum ${CMAKE_NAME}.tar.gz)" ]; then
    echo "Bad checksum, aborting!"
    echo "$(sha512sum ${CMAKE_NAME}.tar.gz)"
    rm -f ${CMAKE_NAME}.tar.gz
    exit 1
fi

# extract tarball
tar xf ${CMAKE_NAME}.tar.gz

# create build dir
mkdir -p build
cd build
../${CMAKE_NAME}/configure --parallel=$(nproc) --prefix=$PWD/../dist --system-curl --no-qt-gui
make -j$(nproc) && make -j$(nproc) install
