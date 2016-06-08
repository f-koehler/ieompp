#!/bin/bash

CMAKE_NAME="cmake-3.5.2"
CMAKE_URL="https://cmake.org/files/v3.5/cmake-3.5.2.tar.gz"
CMAKE_SHA512="7b08eb9f1b37993553f89c03eceedc465dc52b787dec99b78c74ebff2817d0aac9764e82ad835e8fc39f61cd2d2c0a3544d3f5ae299234ed52e9a940edf30b81  cmake-3.5.2.tar.gz"

# create a dir for cmake
mkdir -p ${CMAKE_NAME}
cd ${CMAKE_NAME}

if [[ -e build.successful ]]; then
    echo "cmake already built; skipping"
    exit 0
fi

echo "cmake not present; building"

# download cmake tarball
if [[ ! -f ${CMAKE_NAME}.tar.gz ]]; then
    wget --no-check-certificate ${CMAKE_URL}
fi

# verify checksum
if [[ "${CMAKE_SHA512}" != "$(sha512sum ${CMAKE_NAME}.tar.gz)" ]]; then
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
CC=gcc CXX=g++ ../${CMAKE_NAME}/configure --parallel=$(nproc) --prefix=$PWD/../dist --system-curl --no-qt-gui
CC=gcc CXX=g++ make -j$(nproc) && make -j$(nproc) install

cd ..
touch build.successful
