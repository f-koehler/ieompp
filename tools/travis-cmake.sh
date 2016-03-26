#!/bin/bash

CMAKE_NAME="cmake-3.5.1"
CMAKE_URL="https://cmake.org/files/v3.5/cmake-3.5.1.tar.gz"
CMAKE_SHA512="f78185eb1a2344167888b13ce2a0e8657594bc9c4195c99f5a2be3f953dd2ff630081c2d63b180215851eec8c9565fab8ee1c481ca4c0e6eb9dc27b574d45616  cmake-3.5.1.tar.gz"

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
