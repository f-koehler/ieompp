#!/bin/bash

SOURCES="-p=../ieompp-build/ -include=include,tests"
OPTIONS="-risk=reasonable -summary -final-syntax-check"
TRANSFORMS="-add-override -loop-convert -pass-by-value -replace-auto_ptr -use-auto -use-nullptr"
clang-modernize ${OPTIONS} ${TRANSFORMS} ${SOURCES}
