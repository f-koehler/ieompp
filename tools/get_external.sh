#!/bin/bash

EIGEN_SHA512="d40fe1c315536d14c42aae782cd7f8c96ed5993ba4f285c67ace90f99b3794b777508bef7cb47c3979f928d06bfc2237f103a25089117797cc9105b3b09be15a"
VERSION="3.3"

git submodule update --init --recursive
RETVAL=$?
if [ $RETVAL -ne 0 ]; then
    echo "Failed to retrieve git submodules" 1>&2
    exit 1
fi

if [ ! -e external/eigen.tar.gz ]; then
    curl -o external/eigen.tar.gz "https://bitbucket.org/eigen/eigen/get/$VERSION.tar.gz"
    RETVAL=$?
    if [ $RETVAL -ne 0 ]; then
        echo "Failed to download eigen" 1>&2
        exit 1
    fi
fi
EIGEN_DOWNLOADED_SHA512=`/usr/bin/sha512sum external/eigen.tar.gz | cut -d " " -f 1`
if [ $EIGEN_SHA512 != $EIGEN_DOWNLOADED_SHA512 ]; then
    echo "Failed to verify downloaded Eigen by sha512" 1>&2
    exit 1
fi

mkdir -p external/eigen && tar -x --strip-components=1 -C external/eigen -f external/eigen.tar.gz
RETVAL=$?
if [ $RETVAL -ne 0 ]; then
    echo "Failed to extract eigen" 1>&2
    exit 1
fi
