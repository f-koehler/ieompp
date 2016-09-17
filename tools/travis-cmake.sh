#!/bin/bash

CMAKE_NAME="cmake-3.6.2"
CMAKE_URL="https://cmake.org/files/v3.6/cmake-3.6.2.tar.gz"
CMAKE_SHA512="50b217d3205cc65c2b55ece4ba050b1aa80dfa8a7e22ee0ec16c741cd99a639847d197a2e8448d7710c2a6ea7ba7bbcd6e3f12810f65dcc74bd27bcdb02f844a  cmake-3.6.2.tar.gz"

cd ..
if [[ -e ${CMAKE_NAME}.successful ]]; then
    echo "${CMAKE_NAME} already built; skipping"
    exit 0
fi

echo "${CMAKE_NAME} not present; building"

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
rm ${CMAKE_NAME}.tar.gz

# create build dir
mkdir -p ${CMAKE_NAME}-build
cd ${CMAKE_NAME}-build
CC=gcc CXX=g++ ../${CMAKE_NAME}/configure --parallel=$(nproc) --prefix=$HOME/local/ --system-curl --no-qt-gui
CC=gcc CXX=g++ make -j$(nproc) && make -j$(nproc) install

cd ..
rm -rf ${CMAKE_NAME}
rm -rf ${CMAKE_NAME}-build
touch ${CMAKE_NAME}.successful
