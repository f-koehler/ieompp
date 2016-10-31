#!/bin/env python3
from os import walk
from os.path import join, splitext
from subprocess import call
from sys import argv

iwyu_args = []
iwyu_clang_args = [
    "-std=c++14",
    "-I./include",
    "-isystem./external/blaze",
    "-isystem./external/eigen",
    "-isystem./external/spdlog/include",
]

if __name__ == "__main__":
    if "-v" in argv:
        iwyu_args += ["-Xiwyu", "--verbose=3"]

    hpp_files = []
    for root, _, files in walk("include/ieompp"):
        for file in files:
            path = join(root, file)
            if splitext(path)[1] != ".hpp":
                continue
            hpp_files.append(path)

    iwyu_hpp_args = []
    # for hpp_file in hpp_files:
    #     iwyu_hpp_args += ["-Xiwyu", "--check_also="+hpp_file]

    for root, _, files in walk("src"):
        for file in files:
            path = join(root, file)
            if splitext(path)[1] != ".cpp":
                continue
            iwyu_cmd = ["include-what-you-use"] + iwyu_hpp_args + iwyu_args + iwyu_clang_args
            iwyu_cmd.append(path)

            call(iwyu_cmd)
