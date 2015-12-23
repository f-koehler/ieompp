#!/bin/bash

CMAKE_NAME="cmake-3.4.1"
CMAKE_URL="https://cmake.org/files/v3.4/cmake-3.4.1.tar.gz"
CMAKE_SHA512="072bbfc3ffe3a838945ce6e7c4bbce267362673c3b870886accdc5a5ee01dbd071604fca6aa519d077676423d37bfe987b71f22f14a194f8d6ec8fb9de0a6049  cmake-3.4.1.tar.gz"

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
tar xf cmake-3.4.1.tar.gz

# create build dir
mkdir -p build
cd build
../${CMAKE_NAME}/configure --parallel=$(nproc) --prefix=$PWD/../dist
make -j$(nproc) && make -j$(nproc) install
