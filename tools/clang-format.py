#!/usr/bin/env python3
from subprocess import check_output
from os import walk
from os.path import join, splitext

search_paths = [
    "include/ieompp",
    "src",
    "tests"
]

extensions = [
    ".cpp",
    ".hpp"
]

if __name__ == "__main__":
    for search_path in search_paths:
        for root, _, files in walk(search_path):
            for file in files:
                path = join(root, file)
                ext = splitext(path)[1]
                if ext not in extensions:
                    continue
                output = check_output(["clang-format", path]).decode()
                with open(path, "w") as f:
                    f.write(output)
