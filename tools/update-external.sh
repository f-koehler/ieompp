#!/bin/bash

git submodule update --recursive

DIR=$PWD

cd $PWD/external/blaze
git checkout master
git pull origin master

cd $PWD/external/spdlog
git checkout master
git pull origin master
