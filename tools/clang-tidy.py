#!/bin/env python3
from os import walk
from os.path import abspath, dirname, join, realpath, splitext
from subprocess import call
from sys import argv, stderr

checks = [
    "clang-analyzer-*",
    "cppcoreguidelines-*",
    "llvm-namespace-comment",
    "modernize-*",
    "performance-*",
    "readability-*",
]

if __name__ == "__main__":
    if len(argv) < 2:
        print("Specify path to build directory", file=stderr)
        exit(1)

    build_dir = argv[1]
    project_dir = abspath(join(dirname(realpath(__file__)), ".."))

    for root, _, files in walk("src"):
        for file in files:
            path = join(root, file)
            if splitext(path)[1] != ".cpp":
                continue
            cmd = [
                "clang-tidy",
                "-p", build_dir,
                "-header-filter=" + join(project_dir, "include", "ieompp")+".*",
                "-checks=" + ','.join(checks),
                "-fix",
                path
            ]
            print(path)
            call(cmd)
