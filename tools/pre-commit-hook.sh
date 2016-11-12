#!/bin/bash

echo "pre-commit: run include-guard.py"
tools/include-guards.py > /dev/null

echo "pre-commit: run include-style.py"
tools/include-style.py > /dev/null

echo "pre-commit: run clang-format.py"
tools/clang-format.py
