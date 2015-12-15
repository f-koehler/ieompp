#!/bin/bash
clang-tidy -p build $(find include -type f -name "*.cpp" -or -name "*.hpp") -checks=*
