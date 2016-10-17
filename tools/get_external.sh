#!/bin/bash

EIGEN_SHA512="d494995d5e5a264147141e84fda754950dcb41c6eff94ef3bf14411d8e3eaad4376bc3f634a5edcaa5f13c8be1d845784071f5b181e267c8fad9982fdb1a2d37"

git submodule update --init --recursive --jobs $(nproc)
RETVAL=$?
if [ $RETVAL -ne 0 ]; then
    echo "Failed to retrieve git submodules" 1>&2
    exit 1
fi

if [ ! -e external/eigen.tar.gz ]; then
    curl -o external/eigen.tar.gz "https://bitbucket.org/eigen/eigen/get/3.2.10.tar.gz"
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
