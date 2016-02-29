#!/bin/bash

CMAKE_NAME="cmake-3.4.3"
CMAKE_URL="https://cmake.org/files/v3.4/cmake-3.4.3.tar.gz"
CMAKE_SHA512="a326c39cfe9f5408b4586ddd291cabfa400a2442947fb46d94dd6c44db3087c48c84a9a22ca5ed38707467448150383935ed59c669fbbfb20a968d4031eaaa11  cmake-3.4.3.tar.gz"

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
